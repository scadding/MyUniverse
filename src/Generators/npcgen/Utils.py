#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: Utils.py,v $
 * $Date: 2010-03-02 14:49:43 -0600 (Tue, 02 Mar 2010) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * Utility support classes and static functions.
 *
 *******************************************************************************
"""

import sys, os, string, urllib
from sets import ImmutableSet
from time import time, timezone
from xmlrpclib import Transport
from threading import Condition, Lock, RLock
from random import normalvariate, randint, random, choice
from collections import deque

from NPCExceptions import NPCError


# static
kZeroIntSet		= ImmutableSet(('-', ' -', '- ', '-*', '- *', '', ' '))
kIntStringSet	= ImmutableSet(('*', 't', 'b'))
kTypeDict		= {'int':int, 'float':float, 'long':long}
MINIMUMFONTSIZE = 4 
# need this because winzip is annoying
#kTimeZoneDifference = timezone - 18000
kTimeZoneDifference = 0


def IntRound(x):
	return int(round(x))


def RandInt(min, max, mode=None):
	if max <= min:
		return min
	result = randint(min, max)
	if (type(mode) == type(0)) and result != mode:
		result = randint(min, max)
	return result


def RandDistribInt(args=None, **kwords):
	argsDict = {}
	if (type(args) == type({})):
		argsDict = args
	argsDict.update(kwords) # note that a kword overrides an arg
	typef = IntRound
	if 'type' in argsDict:
		typef = kTypeDict[argsDict['type']]
	#print argsDict
	# convert all to float
	for key in ('min', 'max', 'mode', 'mean', 'sdev', 'precision'):
		# temp HACK: backward compat. for old modes (e.g.: "9, 10, 11")
		if key in argsDict:
			value = argsDict[key]
			#@if type(value) == type(u''): print 'hey!!'
			try:
				argsDict[key] = float(value)
			except Exception, why:
				print why, key, value
				raise NPCError("num format problem: %s %s %s" % (why, key, value))
	if ('mean' in argsDict):
		if ('sdev' not in argsDict):
			argsDict['sdev'] = 1
		result = normalvariate(argsDict['mean'], argsDict['sdev'])
		# check mode
		if ('mode' in argsDict) and (typef(result) != argsDict['mode']):
			result = normalvariate(argsDict['mean'], argsDict['sdev'])
		# now safe to check min/max
		if ('min' in argsDict) and (result < argsDict['min']):
			result = argsDict['min']
		if ('max' in argsDict) and (result > argsDict['max']):
			result = argsDict['max']
		if 'precision' in argsDict:
			precision = float(argsDict['precision'])
			result = round(result/precision) * precision
	elif ('min' in argsDict) and ('max' in argsDict) and ('mode' in argsDict):
		result = RandInt(argsDict['min'], argsDict['max'], argsDict['mode'])
	elif ('min' in argsDict) and ('max' in argsDict):
		result = RandInt(argsDict['min'], argsDict['max'])
	elif ('min' in argsDict):
		result = argsDict['min']
	elif ('max' in argsDict):
		result = argsDict['max']
	else:
		result = 777
		print 'randDistribInt: what, not enought args: %s   result: %s' % (argsDict, result)
	# CHECK-ME: what about int(round(result)) for typef == int ???
	#           for now, default typef = IntRound
	##if typef == kTypeDict['float']: print 'result', result
	return typef(result)


def Randint(min, max):
	'DEPRECATED: make sure min/max values are valid; return randint i such that min <= i <= max'
	if min == max:
		return max
	elif max < min:
		raise NPCError('received invalid min/max values: %s/%s' % (min, max))
	else:
		return randint(min, max)


def GetSimpleHTML(npcString, encoding):
	htmlString = ''
	for i, line in enumerate(npcString.split('\n')[1:]):
		if not i:
			line = '<FONT FACE="Helvetica" SIZE=5 WEIGHT=800><B>' + line + '<BR><BR></B></FONT>'
		elif i == 1:
			htmlString += '<FONT FACE="Courier New" SIZE=2>'
		else:
			line = line + '<BR>\n'
		newline = line.replace(' ', '&nbsp;')
		newline = newline.replace('\t', '&nbsp;'*8)
		htmlString += newline

	# close out font tag
	htmlString += '</FONT>'
	return htmlString


class ReadWriteLock(object):
	'A lock object that allows many simultaneous "read-locks", but only one "write-lock".'
	def __init__(self):
		self.readReady = Condition(Lock())
		self.readers = 0

	def acquireRead(self):
		'Acquire a read-lock. Blocks only if some thread has acquired write-lock.'
		self.readReady.acquire()
		try:
			self.readers += 1
		finally:
			self.readReady.release()

	def releaseRead(self):
		'Release a read-lock.'
		self.readReady.acquire()
		try:
			self.readers -= 1
			if not self.readers:
				self.readReady.notifyAll()
		finally:
			self.readReady.release()

	def acquireWrite(self):
		'Acquire a write lock. Blocks until there are no acquired read- or write-locks.'
		self.readReady.acquire()
		while self.readers > 0:
			self.readReady.wait()

	def releaseWrite(self):
		'Release a write-lock.'
		self.readReady.release()


def GetInt(intString):
	'useful for parsing int when it might end in * or equal "-"'
	intString = str(intString)
	if intString in kZeroIntSet:
		realInt = 0
	elif ('/' in intString):
		realInt = int(intString[0])
	elif intString[-1] in kIntStringSet:
		realInt = int(intString[:-1])
	else:
		realInt = int(intString)
	return realInt


def GetElapsedTime(start, stop=None):
	'return elapsed time in minutes:seconds friendly string'
	if not stop:
		stop = time()
	minutes, seconds = divmod((stop - start), 60)
	elapsedTime = '%s min., %s sec.' % (int(minutes), round(seconds, 2))
	return elapsedTime


def getModeValue(min, max, modeString):
	'get random value with mode; modeString can be delimited with "," -- or ", " as any whitespace around delimiter is ignored!'
	try:
		value = randint(min, max)
		if modeString and (value not in [int(mode) for mode in modeString.split(',')]):
			value = randint(min, max)
		return value
	except ValueError, ex:
		raise NPCError('received improperly-delimited mode: %s' % (str(modeString),))


def GetRootDir():
	'we seem to need this one a lot'
	args = sys.argv
	
	# make sure programPath is in upper() for lame case-insensitive systems
	programPath = args[0].upper()
	#print programPath

	if 'PYTHONSERVICE.EXE' in programPath:
		rootDir = r'C:\Utils\Webware\WebKit\NPCGen'
	elif programPath == 'CMAKER-GUI.PY':
		rootDir = os.getcwd()
	elif programPath == 'LAUNCH.PY':
		rootDir = os.getcwd() + '/NPCGen'
	elif programPath in ('NPCGEN.EXE', 'CHARACTERMAKER.EXE'):
		#rootDir = os.path.abspath(os.getcwd() + os.sep + '..')
		rootDir = os.getcwd()
	elif string.find(programPath, 'CMAKER-GUI.PY') != -1:
		rootDir = programPath[:string.find(programPath, 'CMAKER-GUI.PY')]
	elif string.find(programPath, 'NPCGEN.EXE') != -1:
		rootDir = os.path.abspath(programPath[:string.find(programPath, 'NPCGEN.EXE')]) #  + '..'
	elif os.name == 'posix':
		rootDir = os.getcwd()
	elif programPath == './CHARACTERMAKER.PY':
		rootDir = '.'
	else:
		rootDir = programPath[:string.rfind(programPath, os.sep)]

	#print programPath, rootDir
	if len(rootDir) and (rootDir[-1] == os.sep):
		rootDir = rootDir[:-1]
	elif not len(rootDir):
		rootDir = os.getcwd()

	return rootDir


class URLLibTransport(Transport):
	'''Handles an HTTP transaction to an XML-RPC server via urllib
	(urllib includes proxy-server support)
	jjk  07/02/99'''
	def __init__(self, proxy=None):
		self.proxy = proxy
		self._use_datetime = 1

	def request(self, host, handler, request_body, verbose=0):
		'issue an XML-RPC request, testing for proxy that this class may have been initialized with'
		self.verbose = verbose
		if self.proxy:
			urlopener = urllib.FancyURLopener({'http':self.proxy})
		else:
			# use default proxy settings found in this os -- or not -- let's try empty dict...
			urlopener = urllib.FancyURLopener({})
		urlopener.addheaders = [('User-agent', self.user_agent)]
		# probably should use appropriate 'join' methods instead of 'http://'+host+handler
		remoteFile = urlopener.open('https://'+host+handler, request_body)
		return (self.parse_response(remoteFile))


def LruCacheDecorator(maxsize=100, log=False):
	'''Decorator applying a least-recently-used cache with the given maximum size.

	Arguments to the cached function must be hashable.
	Cache performance statistics stored in f.hits and f.misses.
	'''
	def decorating_function(f):
		cache = {}              # mapping of args to results
		queue = deque()         # order that keys have been accessed
		refcount = {}           # number of times each key is in the access queue
		lock = RLock()

		def wrapper(*args):
			# localize variable access (ugly but fast)
			_lock=lock; _cache=cache; _len=len; _refcount=refcount; _maxsize=maxsize; _log=log
			queue_append=queue.append; queue_popleft = queue.popleft

			# get cache entry or compute if not found
			_lock.acquire()
			try:
				result = _cache[args]
				wrapper.hits += 1
			except KeyError:
				result = _cache[args] = f(*args)
				wrapper.misses += 1

			# record that this key was recently accessed
			queue_append(args)
			_refcount[args] = _refcount.get(args, 0) + 1

			# Purge least recently accessed cache contents
			while _len(_cache) > _maxsize:
				k = queue_popleft()
				_refcount[k] -= 1
				if not _refcount[k]:
					# it's possible (though not likely) that k is not in cache because it was 'invalidated' externally
					if k in _cache:
						if log: print ' -*- purging <args: %s, resultSize: %s> from cache -*- ' % (str(k), len(str(_cache[k])))
						del _cache[k]
					del _refcount[k]

			# Periodically compact the queue by duplicate keys
			if _len(queue) > _maxsize * 4:
				for i in [None] * _len(queue):
					k = queue_popleft()
					if _refcount[k] == 1:
						queue_append(k)
					else:
						_refcount[k] -= 1
				# comment out assert of invariants for now, because external invalidation of cache may have occured
				##assert len(queue) == len(cache) == len(refcount) == sum(refcount.itervalues())
			_lock.release()
			return result

		wrapper.__doc__ = f.__doc__
		wrapper.__name__ = f.__name__
		wrapper.hits = wrapper.misses = 0
		wrapper.cache = cache
		return wrapper

	return decorating_function


class SingletonMetaClass(type):
	'metaclass that can be used to enforce singleton pattern'
	def __init__(cls,name,bases,dic):
		super(SingletonMetaClass,cls).__init__(name,bases,dic)
		cls.instance=None

	def __call__(cls,*args,**kw):
		if cls.instance is None:
			cls.instance=super(SingletonMetaClass,cls).__call__(*args,**kw)
		return cls.instance


if __name__ == '__main__':
	args = sys.argv
	if len(args) > 1:
		arglist = args[1:]
		argstr  = ', '.join(arglist)
		adict = eval('dict(%s)' % argstr)
		print adict
		for i in range(60):
			#print RandDistribInt(min="-2", max="2", mode="0", mean="0", sdev=".75")
			#min="0" max="1" mode="0" mean="0" sdev="0.25" precision="0.25" type="float"
			#print RandDistribInt(min="-1", max="1", mode="0", mean="0", sdev=".175", precision="0.25", type="float")
			print RandDistribInt(adict)


