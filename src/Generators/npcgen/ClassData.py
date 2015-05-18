#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: ClassData.py,v $
 * $Date: 2008-05-25 11:00:39 -0500 (Sun, 25 May 2008) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * All Template container Classes, populated with XML data,
 * used to generate NPCData objects, the actual NPCs.
 *
 *******************************************************************************
"""

import sys, os, re
from sets import Set, ImmutableSet
from random import shuffle, choice

from Utils import RandInt, getModeValue, RandDistribInt
from NPCExceptions import InfiniteLoopError


# constants
kClassKeys				= ('nationality', 'gender-ratio', 'item-mod-inc', 'hair', 'eyes', 'age', 'tech-level', 'images-male', 'images-female')
kRangeSet				= ImmutableSet(('min', 'max', 'mode', 'mean', 'sdev', 'precision', 'type'))
kItemModifyKeys			= ImmutableSet(('fine-craftmanship', 'adamantine', 'lightened'))
kItemModifyDamageKeys	= ImmutableSet(('fine-craftmanship', 'adamantine'))


# class data containers

class ClassType(object):
	'a container for character class data'
	def __init__(self, attrs=(), genre='', classFilePath=None):
		self.mData = {}
		self.iData = {}
		self.ancestors = Set()
		self.parentsWalked = 0
		self.infiniteLoopChecked = 0
		localDict = {}
		if classFilePath:
			localDict['path'] = classFilePath
		for dataPair in attrs:
			name, value = dataPair
			localDict[name] = value
		self.mData['class'] = localDict
		self.mData['class']['genre'] = genre
		self.iData['class'] = localDict.copy()
		self.iData['class']['genre'] = genre

	def getData(self, classes):
		'return mData: all classType data, including inherited data'
		if not self.parentsWalked:
			self.inheritParents(classes, self.getParents(classes))
		return self.mData

	def hasParents(self):
		'return boolean: whether or not this classType inherits from any parents'
		return self.mData['class'].has_key('parents')

	def isAbstract(self):
		'return boolean: whether or not this classType is abstract'
		return (self.mData['class'].has_key('abstract') and int(self.mData['class']['abstract']))

	def getAlias(self):
		'return alias'
		return self.mData['class']['alias']

	def __repr__(self):
		return self.getName()

	def getName(self):
		'return name'
		return self.mData['class']['name']

	def getItemModInc(self):
		'return item-mod-inc'
		if self.mData['class'].has_key('item-mod-inc'):
			return int(self.mData['class']['item-mod-inc'])
		else:
			return 0

	def getParentAliases(self):
		'return sequence of parent aliases'
		return self.mData['class']['parents'].split(', ')

	def getParents(self, classes):
		'return parents if this classType has any'
		parents = []
		if self.hasParents():
			parents = [classes[parentAlias] for parentAlias in self.getParentAliases()]
		return parents


	def checkForInfiniteLoop(self, classes):
		'make sure that this class does not have an ancestor tree that contains an infinite loop'
		# go first level of parents, if they have not been checked
		for parent in self.getParents(classes):
			if not parent.infiniteLoopChecked and parent.hasParents():
				parent.checkRootAlias(parent.getAlias(), classes)
		# tail recursion
		self.checkRootAlias(self.getAlias(), classes)

	def checkRootAlias(self, rootAlias, classes):
		'tail recursion function used to check each parent recursively against a rootAlias'
		for parent in self.getParents(classes):
			if parent.getAlias() != rootAlias:
				# recurse for each parent
				if parent.hasParents():
					parent.checkRootAlias(rootAlias, classes)
			else:
				raise InfiniteLoopError('infinite ancestor loop detected for class alias: %s' % rootAlias, self.getName())
		# done, flip flag
		self.infiniteLoopChecked = 1


	def inheritParents(self, classes, parents):
		'copy appropriate iData values from tree of parent classes; do so in breadth-first order'
		if not self.infiniteLoopChecked:
			self.checkForInfiniteLoop(classes)
		for parent in parents:
			if parent not in self.ancestors:
				parentClassData = parent.iData
				for parentKey in parentClassData:
					if parentKey == 'class':
						# inherit valid class-specific keys from parent
						for classKey in kClassKeys:
							if (classKey in parentClassData[parentKey]) and (classKey not in self.mData[parentKey]):
								self.mData[parentKey][classKey] = parentClassData[parentKey][classKey]
					else:
						# deep copy dict
						parentKeyDict = parentClassData[parentKey].copy()
						if parentKey in self.mData:
							self.updateDict(self.mData[parentKey], parentKeyDict, parentKey)
						else:
							self.mData[parentKey] = parentKeyDict
				# done inheriting, append to ancestors set
				self.ancestors.add(parent)
		# recurse in breadth-first order
		grandparents = [grandparent for parent in parents for grandparent in parent.getParents(classes)]
		if grandparents: self.inheritParents(classes, grandparents)
		# set this flag so walking tree of parents is only done ONCE for this classType
		self.parentsWalked = 1


	def updateDict(self, classDict, parentDict, grandparentKey):
		'do a "safe" deep update (do not overwrite existing subkeys)'
		for parentKey, parentValue in parentDict.items():
			if parentKey not in classDict:
				if (parentKey != 'req'):
					classDict[parentKey] = parentValue
				#	else: print 'class %s will not add req: %s, dict: %s, key: %s' % (self.getAlias(), parentValue, str(classDict), str(grandparentKey))
			elif type(parentValue) == type({}):
				self.updateDict(classDict[parentKey], parentValue, parentKey)
	

	def inheritParentsDepthFirst(self, classes):
		'for historical posterity, this is how "inheritParents" used to be done...'
		for parent in self.getParents(classes):
			if not parent.parentsWalked:
				# go recursive on his ass
				parent.inheritParents(classes)
			# now that the parent tree has been safely walked, populate with this parentClassData
			parentClassData = parent.mData
			for parentKey in parentClassData.keys():
				if parentKey == 'class':
					# inherit valid class-specific keys from parent
					for classKey in ('nationality', 'gender-ratio', 'item-mod-inc', 'hair', 'eyes', 'age', 'tech-level'):
						if parentClassData[parentKey].has_key(classKey) and not self.mData[parentKey].has_key(classKey):
							self.mData[parentKey][classKey] = parentClassData[parentKey][classKey]
				else:
					# make shallow copy of dict
					parentKeyDict = parentClassData[parentKey].copy()
					if self.mData.has_key(parentKey):
						# do a 'safe' update (do not overwrite existing subkeys)
						for safeUpdateKey in parentKeyDict.keys():
							if not self.mData[parentKey].has_key(safeUpdateKey):
								self.mData[parentKey][safeUpdateKey] = parentKeyDict[safeUpdateKey]
					else:
						self.mData[parentKey] = parentKeyDict
		# set this flag so walking tree of parents is only done ONCE for this classType
		self.parentsWalked = 1


	def addAttribSet(self, attribSet, attrs):
		'add an attribSet header with set defs...'
		localDict = {}
		for dataPair in attrs:
			name, value = dataPair
			localDict[name] = value
		self.mData[attribSet] = localDict
		self.iData[attribSet] = localDict.copy()

	def addAttrib(self, attribSet, attrib, attrs):
		'add attrib to an attribSet key'
		localDict = {}
		for dataPair in attrs:
			name, value = dataPair
			localDict[name] = value
		self.mData[attribSet][attrib] = localDict
		self.iData[attribSet][attrib] = localDict.copy()

	def getAttribValue(self, attribDict, defaultMin=0):
		'return value based on min, max, mode in attribDict'
		if 'max' not in attribDict:
			return 0
		else:
			# set defaultMin for this attrib
			if 'min' not in attribDict:
				attribDict['min'] = defaultMin

			#attribVal = getModeValue(min, max, attribDict.get('mode'))
			return RandDistribInt(attribDict)

	def hasAttribSet(self, attribSet):
		'return boolean: true if this character has this attribSet key'
		return self.mData.has_key(attribSet)

	def getAttribSet(self, attribSet, skillmap={}):
		'return: localAttribDict{} from self.mData[attribSet]'
		localAttribDict = {}
		attributes = self.mData[attribSet].keys()
		onlyReq = 0
		total = 0
		if ('min' in self.mData[attribSet]) and ('max' in self.mData[attribSet]):
			# get total attributes
			total = RandDistribInt(self.mData[attribSet])
			# if total is zero now, set onlyReq (only case where total=0 is ok)
			onlyReq = 1
			# we only have to shuffle if min/max was specified
			shuffle(attributes)

		# remove the non-attrib keys (more efficient)
		# note: we must use slice copy on attributes 'cause we're removing from it!!!
		for temp in attributes[:]:
			if temp in kRangeSet:
				attributes.remove(temp)

		# now, get total if we didn't set from min/max
		if not total and not onlyReq:
			total = len(attributes)

		# populate localAttribDict and return
		currentTotal = 0
		defaultMin = None
		for attrib in attributes:
			attribDict = self.mData[attribSet][attrib]
			#@remove debugging lines below...
			#@if type(attribDict) != type({}): print attrib, attribDict, attributes
			if attribDict.has_key('req'):
				if attribDict['req'] == '1':
					reqVal  = 1
					reqRand = 1
				elif attribDict['req'] == '-1':
					reqVal  = -1
					reqRand =  1
				else:
					reqFloat = float(attribDict['req'])
					reqVal   = int(reqFloat * 100)
					reqRand  = (RandInt(0, (9999)) < reqVal)
			else:
				reqVal  = 0
				reqRand = 0
			# if we have not reached max total OR req is 1 (or RandInt(1, req)==1)
			if ((currentTotal < total) and (reqVal == 0)) or \
			   ((reqVal > 0) and reqRand):
				if skillmap and (attrib in skillmap) and ('min' in skillmap[attrib]):
					defaultMin = int(skillmap[attrib]['min'])
				localAttribDict[attrib] = self.getAttribValue(attribDict, defaultMin)

				# some req attribs do not count against our total 
				if reqVal != 1:
					currentTotal += 1
		return localAttribDict


class AttributeDependencyMap(dict):
	'a container for attribute dependencies'
	def addAttribSet(self, attribSet, attrs):
		'add an attribSet header with set defs...'
		localDict = {}
		for dataPair in attrs:
			name, value = dataPair
			localDict[name] = value
		self[attribSet] = localDict

	def addAttrib(self, attribSet, attrib, attrs):
		'add attrib to an attribSet key'
		localDict = {}
		for dataPair in attrs:
			name, value = dataPair
			localDict[name] = value
		self[attribSet][attrib] = localDict


class ItemTable(object):
	'a container for weapon/armor class data'
	def __init__(self):
		self.mItemStats = {}
		self.mItemMods = {}
		self.mHandItemMods = {}
		self.mModInc = 0

	def getItem(self, itemAlias, classTypeModInc=0):
		'return item{} from mItemStats with possible (%5) item mod'
		# note, we want to do shallow copy of item dict here to ensure that each item is unique
		item = self.mItemStats[itemAlias].copy()

		# n% chance that this item is cool
		if (item.has_key('req-mods')) or \
		   ((item.has_key('mod') and int(item['mod']) != -1) and \
		   (RandInt(0, 99) < (classTypeModInc + self.mModInc + int(item['mod'])))):
			if item.has_key('damage'):
				itemModDict = self.mHandItemMods
			else:
				itemModDict = self.mItemMods

			# randomize the available mods; set totalMods
			premodKeys = itemModDict.keys()
			shuffle(premodKeys)
			maxMods = 1
			if not RandInt(0, 9):
				maxMods += 1
			if not RandInt(0, 31):
				maxMods += RandInt(2, 3)
			totalMods = RandInt(1, maxMods)

			# handle any req-mods ...
			if item.has_key('req-mods'):
				reqMods = item['req-mods'].split(', ')
				modKeys = []; modKeys.extend(reqMods)

				if (RandInt(0, 99) < (classTypeModInc + self.mModInc + int(item['mod']))):
					for reqMod in reqMods:
						premodKeys.remove(reqMod)
					modKeys.extend(premodKeys)
					orig = totalMods
					totalMods += len(reqMods)
				else:
					totalMods = len(reqMods)
			else:
				modKeys = premodKeys

			itemNumber = 0
			for i in range(totalMods):
				key = modKeys[i]
				#min = int(itemModDict[key]['min'])
				#max = int(itemModDict[key]['max'])
				#amount = getModeValue(min, max, itemModDict[key].get('mode'))
				amount = RandDistribInt(itemModDict[key])
				#if key in ('fine-craftmanship', 'lightened', 'improved-db', 'improved-dr', 'adamantine', 'improved-damage', 'increased-to-hit'):
				#	print 'whoot, %s - %s: with vals %s, got result: %s' % (item['name'], key, itemModDict[key], amount)

				if ('improved' == key[:8]) or (key in kItemModifyKeys):
					if key in kItemModifyDamageKeys:
						stat = 'damage'
					elif ('lightened' == key) and (('weight' in item) and (item['weight'] != '-')):
						stat = 'weight'
					elif 'lightened' == key:
						stat = 'st'
					else:
						stat = key[9:]

					if stat == 'damage':
						thrustBonus, swingBonus = item[stat].split(',')
						if swingBonus[-1] == '*':
							# ok, this item has already been modified
							swingBonus = swingBonus[:-1]
						if thrustBonus != '-':
							thrustBonus = int(thrustBonus) + amount
						if swingBonus != '-':
							swingBonus = int(swingBonus) + amount
						item[stat] = '%s,%s*' % (thrustBonus, swingBonus)
					elif (stat == 'dr') and (stat in item) and ('/' in item[stat]):
						values = [str(int(v)+amount) for v in item[stat].split('/')]
						item[stat] = '/'.join(values) + '*'
					else:
						try:
							if stat in item:
								if stat == 'weight':
									weightFloat = float(item[stat][:item[stat].find('lbs')])
									weightMult = 0.75
									if amount == 2:
										weightMult = 0.50
									elif amount == 3:
										weightMult = 0.40
									item[stat] = str(round(weightFloat * weightMult, 2)) + 'lbs*'
								else:
									item[stat] = str(int(float(item[stat]) + amount)) + '*'
						except ValueError:
							# oh well, must have '-' value -- so pass?
							#item[stat] += '*'
							pass

				# filter out item modification if it does not meet item's minimum tech level (mintl)
				if ('tl' in item) and ('mintl' in itemModDict[key]) and int(item['tl']) < int(itemModDict[key]['mintl']):
					pass
				else:
					if amount:
						item['*mod' + str(itemNumber)] = key + ':' + str(amount)
					else:
						item['*mod' + str(itemNumber)] = key
					itemNumber += 1
		return item



def PrintWeaponsData(weapons):
	'sort hand weapons by damage and display...'
	fWeapons = []
	for weapon in weapons.mItemStats:
		if 'damage' in weapons.mItemStats[weapon]:
			if 'note' not in weapons.mItemStats[weapon]:
				note = ' none'
			else:
				note = weapons.mItemStats[weapon]['note']
			if 'req-mods' not in weapons.mItemStats[weapon]:
				reqmods = ' none'
			else:
				reqmods = weapons.mItemStats[weapon]['req-mods']
			if 'rch' not in weapons.mItemStats[weapon]:
				reach = 'r'
			else:
				reach = weapons.mItemStats[weapon]['rch']
			fWeapons.append((weapons.mItemStats[weapon]['damage'], weapons.mItemStats[weapon]['st'], weapons.mItemStats[weapon]['name'], reqmods, note, reach))

	fWeapons.sort()
	for weapon in fWeapons:
		damage, minStrength, name, reqmods, note, reach = weapon
		print '%-26s damage:%-6s st:%-6s reach:%-6s note:%s' % (name, damage, minStrength, reach, note)


def PrintArmorData(armorTypes):
	'sort armor by area, then db/dr and display...'
	armorList = []
	for armor in armorTypes.mItemStats:
		if 'note' not in armorTypes.mItemStats[armor]:
			note = ''
		else:
			note = armorTypes.mItemStats[armor]['note']

		# format dr
		dr = armorTypes.mItemStats[armor]['dr']
		if len(dr) == 1:
			dr = '0' + dr
		armorList.append(
			(armorTypes.mItemStats[armor]['area'].strip(),
			 armorTypes.mItemStats[armor]['db'], dr,
			 armorTypes.mItemStats[armor]['weight'], 
			 note, 
			 armorTypes.mItemStats[armor]['name']))

	armorList.sort()
	lastarea = ''
	for armor in armorList:
		area, db, dr, weight, note, name = armor
		if lastarea != area:
			print '-' * 110
		lastarea = area
		print '%-26s area:%-12s db:%-1s dr:%-3s weight:%-8s note:%s' % (name, area, db, dr, weight, note)


if __name__ == '__main__':
	from charactermaker import GetXMLData
	args = sys.argv
	if len(args) > 1:
		printTarget = args[1]
		xmlData = GetXMLData()
		attribdepmap, classes, genreMap, weapons, armorTypes, nameMap = xmlData
		if printTarget == 'armor':
			PrintArmorData(armorTypes)
		elif printTarget == 'weapons':
			PrintWeaponsData(weapons)
		else:
			print '%s  What?' % printTarget


"""
 *******************************************************************************
 * $Log: ClassData.py,v $
 * Revision 1.20  2004/02/22 18:04:45  andrew
 * changes to support dynamic NPC pics
 *
 * Revision 1.19  2003/12/28 20:55:14  andrew
 * item stats that don't get affected should not get '*' suffix
 *
 * Revision 1.18  2003/11/16 20:16:22  andrew
 * make updateDict, recursive method for parentDict inheritance
 *
 * Revision 1.17  2003/10/13 15:57:19  andrew
 * items should have lower chance of having multiple mods
 *
 * Revision 1.16  2003/10/13 15:22:26  andrew
 * items should have lower chance of having multiple mods
 *
 * Revision 1.15  2003/10/09 00:34:53  andrew
 * create static cache of commonly used sets
 *
 * Revision 1.14  2003/10/08 18:41:54  andrew
 * use new set types for sequences that need hash() function
 *
 * Revision 1.13  2003/10/04 16:12:38  andrew
 * remove trailing space on armor area
 *
 * Revision 1.12  2003/10/02 20:00:22  andrew
 * format and print armor/weapons based on main arg
 *
 * Revision 1.11  2003/09/23 17:21:16  andrew
 * some minor code cleanup
 *
 * Revision 1.10  2003/09/23 16:46:41  andrew
 * several issues addressed:
 *  - make classKeys tuple a constant
 *  - __repr__ just returns className
 *  - remove annoying debugging lines in inheritParents()
 *  - use list comprehensions for determining grandparents
 *
 * Revision 1.9  2003/09/15 23:43:11  andrew
 * attribmap can be based on dict type
 *
 * Revision 1.8  2003/09/05 18:38:32  andrew
 * randrange should really be RandInt
 *
 * Revision 1.7  2003/09/05 15:28:05  andrew
 * use new-style classes
 *
 * Revision 1.6  2003/08/20 19:09:57  ayinger
 * make attrib totals test easier to read
 *
 * Revision 1.5  2003/07/16 01:36:16  ayinger
 * refactor classType.mData inheritance population in classType.getData() --
 * this is effectively a contract post-condition: any time a client of classType calls
 * classType.getData(), he can be assured to receive data back that has been
 * populated with all ancestor data
 *
 * Revision 1.4  2003/07/15 00:23:08  ayinger
 * cleanup getModeValue() usage so we can optionally pass None as mode
 *
 * Revision 1.3  2003/07/14 20:02:50  ayinger
 * several performance enhancements and some re-factoring:
 *  - refactor getvalue methods to use new Util.GetModeValue()
 *  - ancestors array is now a dict, for faster lookup
 *  - include inheritParentsDepthFirst(), for historical posterity
 *  - remove dead, commented code
 *  - use dict for local data structures where appropriate, for faster lookup
 *  - use new tree-structured attribmap (used to be a flat dict)
 *
 * Revision 1.2  2003/07/11 20:51:27  ayinger
 * add RCS tags
 *
 *******************************************************************************
"""
