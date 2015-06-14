#!/usr/bin/env python

import os, sys
from stat import *
import re
import random
import imp
import csv
from optparse import OptionParser
import tableFunctions
from pyparsing import *

rand = random

def walktree(top, callback, load=False):
    for f in os.listdir(top):
        pathname = os.path.join(top, f)
        mode = os.stat(pathname).st_mode
        if S_ISDIR(mode):
            # It's a directory, recurse into it
            walktree(pathname, callback, load)
        elif S_ISREG(mode):
            # It's a file, call the callback function
            callback(pathname, load)
        else:
            # Unknown file type, print a message
            print 'Skipping %s' % pathname


class table:
    def __init__(self, tablename, continuous, csvflag = False):
        self.values = dict()
        self.index = 0
        self.tablename = tablename
        self.continuous = continuous
        self.csvflag = csvflag
    def add(self, i, v):
        if i == 0:
            return
        if self.continuous:
            self.index = i
        else:
            self.index += i
        if self.csvflag == False:
            if not self.values.get(self.index):
                self.values[self.index] = dict()
            self.values[self.index][0] = v
        else:
            for row in csv.reader([v]):
                self.values[self.index] = row
    def getvaluedict(self, index):
        if index > self.index:
            return 'Error: Out of Range'
        d = index
        while self.values.get(d) == None:
            d = d + 1
        return self.values[d]
    def rolldict(self, roll = -1):
        if self.index == 0:
            return ''
        if roll == -1:
            roll = self.getRandomIndex()
        return self.getvaluedict(roll)
    def getvalue(self, index, column = 0):
        if index > self.index:
            return 'Error: Out of Range'
        if column > 0 and not self.csvflag:
            return 'Error: Out of Range'
        d = index
        while self.values.get(d) == None:
            d = d + 1
        if len(self.values[d]) < (column + 1):
            return ''
        return self.values[d][column]
    def roll(self, column = 0, roll = -1):
        if self.index == 0:
            return ''
        if roll == -1:
            roll = self.getRandomIndex()
        return self.getvalue(roll, column)
    def getRandomIndex(self):
        return rand.randrange(self.index) + 1
    def getCount(self):
        return self.index

class tableFile:
    comment = re.compile('^\s*#.*$')
    whitespace = re.compile('^\s*$')
    tabledeclaration = re.compile('^\s*:([!,/\'\w \+-]*)$')
    #tabledeclaration = re.compile('^\s*:(.*)$')
    tabledeclarationalt = re.compile('^\s*;([!,/\'\w \+-]*)$')
    #tabledeclarationalt = re.compile('^\s*;(.*)$')
    tabledeclarationcsv = re.compile('^\s*@([!,/\'\w \+-]*)$')
    # need to set multiline
    tableline = re.compile('^\s*(\d*)\s*,(.*)')
    tablelinealt = re.compile('^\s*(\d*)-(\d*)\s*,(.*)')
    continuation = re.compile('^_(.*)$')
    variabledeclaration = re.compile('^\s*%(.*)%\s*=\s*(.*)$')
    parameterdeclaration = re.compile('^\s*@.*$')
    pragmadeclaration = re.compile('^/.*$')
    namespec = re.compile('^[/\w _~,-]*/(.*)\.tab$')
    currentstack = dict()
    filename = ''
    def __init__(self, filename):
        self.table = dict()
        self.filename = filename
        self.tablename = ''
        self.stack = dict()
        current = ''
        previous = ''
        m = self.namespec.match(filename)
        if m:
            self.name = m.group(1)
            for l in open(filename):
                current = l.strip()
                m7 = self.continuation.match(current)
                if m7:
                    x = m7.group(1)
                    previous = previous + ' ' + x
                    continue
                self.addTableLine(previous)
                previous = current
            self.addTableLine(previous)
    def addTableLine(self, line):
        m1 = self.comment.match(line)
        m2 = self.whitespace.match(line)
        m3 = self.tabledeclaration.match(line)
        m4 = self.tabledeclarationalt.match(line)
        m5 = self.tabledeclarationcsv.match(line)
        m6 = self.tableline.match(line)
        m7 = self.tablelinealt.match(line)
        m8 = self.variabledeclaration.match(line)
        m9 = self.parameterdeclaration.match(line)
        m10 = self.pragmadeclaration.match(line)
        x = ''
        if m1: #comment
            x = m1.group(0)
        elif m2: #whitespace
            x = m2.group(0)
        elif m3: # Table declaration
            self.tablename = m3.group(1)
            self.table[self.tablename] = table(self.tablename, True)
        elif m4: # Alternate Table Declaration
            self.tablename = m4.group(1)
            self.table[self.tablename] = table(self.tablename, False)
        elif m5: # Csv table declaration
            self.tablename = m5.group(1)
            self.table[self.tablename] = table(self.tablename, False, True)
        elif m6: #table line
            self.table[self.tablename].add(int(m6.group(1)), m6.group(2))
        elif m7: #alternate table line
            d = int(m7.group(2))
            self.table[self.tablename].add(d, m7.group(3))
        elif m8: # variable declaration
            if m8.group(1) == "template":
                self.template(m8.group(2))
            else:
                self.stack[m8.group(1)] = m8.group(2)
        elif m9: #parameter declaration
            x = m9.group(0)
        elif m10: #pragma declaration
            x = m10.group(0)
        else:
            print >> sys.stderr, 'Error: unidentified line ' + self.filename + ' - ' + line
    def template(self, template):
        templateFile = os.path.dirname(self.filename) + '/' + template + '.tml'
        self.stack[template] = ''
        for l in open(templateFile):
            #print l
            self.stack[template] = self.stack[template] + l
        print self.tablename
    def run(self, t = 'Start', roll = -1, column = 0):
        if self.table.get(t):
            return self.table[t].roll(column = column, roll = roll)
        print >> sys.stderr, 'Error: *** No [' + t + '] Table***'
        return ''
    def hasStart(self):
        if self.table.get(t):
            return True
        return False
    def rundict(self, t = 'Start', roll = -1):
        if self.table.get(t):
            return self.table[t].rolldict(roll = roll)
        print >> sys.stderr, 'Error: *** No [' + t + '] Table***'
        return ''
    def start(self):
        self.currentstack = dict()
        return self.run('Start')
    def getBaseVariable(self, var):
        if var in self.stack:
            return self.stack[var]
        return ""
    def getVariable(self, var):
        if var in self.currentstack:
            return self.currentstack[var]
        return ""
    def setVariable(self, var, val):
        self.currentstack[var] = val
    def getRandomIndex(self, t = 'Start'):
        if self.table.get(t):
            return self.table[t].getRandomIndex()
        return -1
    def getCount(self, t = 'Start'):
        if self.table.get(t):
            return self.table[t].getCount()
        return 0

class tableMgr:
    tfile = dict()
    tfilename = dict()
    tgroup = set()
    tgenre = set()
    group = dict()
    def setSeed(self, seed):
        rand.seed(seed)
    def addfile(self, filename, load=False):
        basename = os.path.basename(filename)
        if basename.startswith("_"):
            return
        group = os.path.basename(os.path.dirname(filename))
        name = os.path.splitext(basename)[0]
        extension = os.path.splitext(basename)[1]
        if not(extension == '.py' or extension == '.tab' or extension == '.tsc'):
            return
        self.tfilename[name] = filename
        self.tgroup.add(group)
        if not self.group.get(group):
            self.group[group] = set()
        self.group[group].add(name)
        if load:
            self.loadtable(name)
        return
    def loadtable(self, tablename):
        extension = os.path.splitext(self.tfilename[tablename])[1]
        if extension == '.tab' or extension == '.tsc':
            self.tfile[tablename] = tableFile(self.tfilename[tablename])
        elif extension == '.py':
            x = imp.load_source('generator' ,self.tfilename[tablename])
            self.tfile[tablename] = x.generator()
            if(self.tfile[tablename].version() > 1.0):
                self.tfile[tablename].SetManager(self)
    def checkload(self, tablename):
        if not self.tfile.get(tablename):
            if self.tfilename.get(tablename):
                self.loadtable(tablename)
            else:
                print >> sys.stderr, 'Error: *** Table \'' + tablename + '\' Not found ***'
    def filename(self, tablename):
        return self.tfilename[tablename]
    def groups(self):
        return self.tgroup
    def genre(self):
        return self.tgenre
    def subVariable(self, table, exp):
        print 'sv ', exp
        r1 = re.compile('(.*)%(.*?)%(.*)')
        m = r1.match(exp)
        while m:
            var = m.group(2)
            if self.tfile[table].getVariable(var) == "":
                v = self.tfile[table].getBaseVariable(var)
                self.tfile[table].setVariable(var, self.parse(table, v))
            exp = m.group(1) + self.tfile[table].getVariable(var) + m.group(3)
            m = r1.match(exp)
        return exp
    def subFunction(self, table, exp):
        r1 = re.compile('(.*?){+(.*?)(\:|[|])(.*?)}+(.*)')
        m = r1.match(exp)
        r = exp
        if m != None:
            r = m.group(1)
            l = list()
            f = m.group(2)
            p = m.group(4)
            for i in p.rsplit(','):
                l.append(self.parse(table, i))
            r = r + getattr(tableFunctions, f)(l)
            r = r + m.group(5)
        return r
    def logic(self, table, exp):
        r1 = re.compile('(.*?){+(.*?)(\:|[|])(.*?)}+(.*)')
        m = r1.match(exp)
        r = exp
        if m != None:
            r = m.group(1)
            l = list()
            f = m.group(2)
            p = m.group(4)
            l = p.rsplit(',')
            if f == "for":
                start = int(self.parse(table, l[0]))
                stop = int(self.parse(table, l[1]))
                for x in range(start, stop):
                    r = r + self.parse(table, l[2])
            elif f == "if":
                logic = list()
                logic.append(self.parse(table, l[0]))
                if tableFunctions.eval(logic) == "True":
                    r = r + self.parse(table, l[1])
            else:
                return exp
            r = r + m.group(5)
            return r
        return exp
    def nestedExpr(self, opener, closer):
        content = (Combine(OneOrMore(~Literal(opener) + ~Literal(closer) + 
                   CharsNotIn('\n\r',exact=1))
                   ).setParseAction(lambda t:t[0].strip()))
        ret = Forward()
        ret <<= Group( Suppress(opener) + ZeroOrMore( ret | content )  + Suppress(closer))
        return ret
    def newparse(self, table, exp):
        print '\t', exp
        ret = ''
        last = 0
        nestedItems = self.nestedExpr("{{", "}}")
        for t, s, e in nestedItems.scanString(exp):
            ret = ret + exp[last:s]
            print 'ret', ret
            last = e
            print '{{}}', '\t', t, s, e, t.__class__.__name__
            for i in t:
                print i
                self.handleBrace(table, i)
        ret = ret + exp[last:]
        ret = self.subVariable(table, ret)
        print 'ret2', ret
        return ''
        nestedItems = nestedExpr("[", "]")
        for t, s, e in nestedItems.scanString(exp):
            print '[]', exp, '\t', t, s, e
        q = QuotedString('%', escChar='\\', multiline=False, unquoteResults=True, endQuoteChar=None)
        for t, s, e in q.scanString(exp):
            print 'a%\t', t, s, e
    def parseList(self, l):
        n = None
        for i in l:
            if i.__class__.__name__ == 'ParseResults':
                s = '{{' + self.listToString(i) + '}}'
                if n == None:
                    n = list()
                    n.append(s)
                else:
                    n[-1] = n[-1] + s
            else:
                t = i.rsplit(',')
                if n == None:
                    n = t
                else:
                    n[-1] = n[-1] + t[0]
                    n.extend(t[1:])
        return n
    def listToString(self, l):
        s = ''
        for i in l:
            if i.__class__.__name__ == 'ParseResults':
                s = '{{' + self.listToString(i) + '}}'
            else:
                s = s + i
        return s
    def handleBrace(self, table, l):
        n = list()
        s = ''
        r1 = re.compile('(.*?)(\:|[|]|~)(.*)')
        m = r1.match(l[0])
        if m == None:
            print 'malformed function'
            return ''
        f = m.group(1)
        l[0] = m.group(3)
        n = self.parseList(l)
        print f, n, l
        if f == "for":
            start = int(self.parse(table, n[0]))
            stop = int(self.parse(table, n[1]))
            for x in range(start, stop):
                s = s + self.parse(table, n[2])
        elif f == "if":
            logic = list()
            logic.append(self.parse(table, n[0]))
            if tableFunctions.eval(logic) == "True":
                s = s + self.parse(table, n[1])
        else:
            p = list()
            for i in n:
                p.append(self.parse(table, i))
            s = getattr(tableFunctions, f)(p)
        print '----', n[-1], s
        return s
    def parse(self, table, exp):
        exp = self.logic(table, exp)
        exp = self.subFunction(table, exp)
        exp = self.subVariable(table, exp)
        r1 = re.compile('(\[.*?\])')
        r2 = re.compile('\[(.*?)\]')
        rv = ''
        l = r1.split(exp)
        for segment in l:
            m = r2.match(segment)
            if m:
                exp = m.group(1)
                rv += self.parse(table, self.parseTable(table, exp))
            else:
                rv += segment
        return rv
    def parseTable(self, table, exp):
        roll = -1
        column = 0
        r1 = re.compile('([\w -]*)\.([\w -]*)')
        r2 = re.compile('(.*)@([0-9]+)(.*)')
        r3 = re.compile('(.*)\((.*?)\)(.*)')
        m = r2.match(exp)
        if m:
            exp = m.group(1) + m.group(3)
            column = int(m.group(2))
        m = r3.match(exp)
        if m:
            exp = m.group(1) + m.group(3)
            roll = int(self.parse(table, m.group(2)))
        m = r1.match(exp)
        if m:
            table = m.group(1)
            subtable = m.group(2)
        else:
            subtable = exp
        r2 = re.compile('(\+([0-9]+))')
        self.checkload(table)
        s = self.parse(table, self.tfile[table].run(subtable, roll, column))
        return s
    def roll(self, table):
        self.checkload(table)
        s = self.tfile[table].start()
        s = self.parse(table, s)
        return s
    def run(self, table, sub='Start', roll = -1, column = 0):
        self.checkload(table)
        s = self.tfile[table].run(sub, roll, column)
        s = self.parse(table, s)
        return s
    def rundict(self, table, sub, roll = -1):
        self.checkload(table)
        s = self.tfile[table].rundict(sub, roll)
        return s
    def getRandomIndex(self, table, sub = "Start"):
        self.checkload(table)
        return self.tfile[table].getRandomIndex(sub)
    def getCount(self, table, sub = "Start"):
        self.checkload(table)
        return self.tfile[table].getCount(sub)
    def test(self, count=10, dirname='./test', table=''):
        d = os.path.dirname(dirname)
        if not os.path.exists(dirname):
            os.makedirs(dirname)
        if table == '':
            for t in self.tfile:
                #if isinstance(self.tfile[t], tableFile):
                #    if not self.tfile[t].hasStart():
                #        continue
                print 'table - ' + t
                filename = dirname + '/' + t + '.txt'
                f = None
                for j in range(count):
                    s = self.roll(t)
                    print '\t' + s
                    if s == '':
                        continue
                    if f == None:
                        f = open(filename, 'w')
                    f.write(s + '\n')
        else:
            print 'table - ' + table
            filename = dirname + '/' + table + '.txt'
            f = None
            for j in range(count):
                s = self.roll(table)
                if s == '':
                    continue
                if f == None:
                    f = open(filename, 'w')
                f.write(s + '\n')
            
            
def testcallback(option, opt, value, parser, *args, **kwargs):
    #print option
    #print opt
    #print value
    #print parser.largs
    #print parser.rargs
    #print parser.values
    #print args
    #print kwargs
    print parser.values.datadir
    t = tableMgr()
    walktree(parser.values.datadir, t.addfile, load=True)
    t.test()
    sys.exit

def allcallback(option, opt, value, parser, *args, **kwargs):
    print parser.values.datadir
    t = tableMgr()
    walktree(parser.values.datadir, t.addfile, load=True)
    t.test()
    sys.exit

if __name__ == '__main__':
    parser = OptionParser()
    parser.add_option("-d", "--data", type="string", dest="datadir",
                    help="Data Directory", metavar="Data", default="Data")
    parser.add_option("-a", "--all", action="callback", callback=allcallback)
    parser.add_option("-t", "--test", action="callback", callback=testcallback)
    #parser.add_option("-p", "--planet", action="callback", nargs=3, type=int, callback=planetcallback)
    #parser.add_option("-s", "--sector", action="callback", nargs=3, type=int, callback=sectorcallback)
    #parser.add_option("-y", "--system", action="callback", nargs=3, type=int, callback=systemcallback)
    #parser.add_option("-u", "--subsector", action="callback", nargs=4, type=int, callback=subsectorcallback)
    #parser.add_option("-q", "--quiet",
    #                action="store_false", dest="verbose", default=True,
    #                help="don't print status messages to stdout")
    #parser.add_option("-a", "--amount", action="store",
    #                  type="string", dest="amount",
    #                  metavar='Charge Amount')
    #parser.add_option("-u", "--currency", action="store",
    #                  type="string", dest="currency",
    #                  metavar='Currency', default='jpy')
    #
    #parser.add_option("-p", "--period", action="store",
    #                  type="string", dest="period",
    #                  metavar='Period', default='month')
    #parser.add_option("-d", "--description", action="store",
    #                  type="string", dest="description",
    #                  metavar='Description')

    (options, args) = parser.parse_args()
    sys.exit
    t = tableMgr()
    for p in sys.argv:
        if p == sys.argv[0]:
            continue
        walktree(p, t.addfile, load=True)
    t.test()

