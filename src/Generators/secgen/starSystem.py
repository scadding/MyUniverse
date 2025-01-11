#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from subprocess import *

class starSystem:
    def __init__(self, s, version=0):
        self.name = ""
        self.upp = "0000000-0"
        self._i = 0
        self._j = 0
        #self.features = "PCTRNSG"
        self.features = "       "
        line = s.lstrip()
        if version == 0:
            self.processVersion0(line)
        elif version == 1:
            self.processVersion1(line)
        elif version == 2:
            self.processVersion2(line)
        self.url = 'genr://System' + ' x ' + str(self._i) + ' y ' + str(self._j) + ' z ' + str(self._k)
    def processVersion0(self, line):
        fields = line[:28].split(None, 3)
        self.name = fields[0]
        self._i = int(fields[1][:2]) - 1
        self._j = int(fields[1][2:]) - 1
        self.upp = fields[2]
        self.features = line[29:36]
        self.zone = ' '
        if len(line) > 52:
            self.zone = line[53]
    def processVersion1(self, line):
        fields = line[:33].split(None, 3)
        self.name = fields[0]
        self._i = int(fields[1][:2]) - 1
        self._j = int(fields[1][2:]) - 1
        self.upp = fields[2]
        self.zone = ' '
        if len(line) > 58:
            self.zone = line[59]
        t = line[34]
        if t == 'S':
            s = self.features[0:5]
            s += 'S'
            s += self.features[6]
            self.features = s
        elif t == 'N':
            s = self.features[0:4]
            s += 'N'
            s += self.features[5:7]
            self.features = s
        elif t == 'A':
            s = self.features[0:4]
            s += 'NS'
            s += self.features[6]
            self.features = s
        t = line[54]
        g = int(t)
        if g > 0:
            s = self.features[0:6]
            s += 'G'
            self.features = s
    def i(self):
        return self._i
    def j(self):
        return self._j
    def k(self):
        return self._k
    def processVersion2(self, line):
        gasgiant = False
        print(line)
        self.upp = line[:9]
        self._i = int(line[14:22])
        self._j = int(line[24:31])
        self._k = int(line[34:41])
        self.name = line[41:]
        t = line[10:13]
        #print self.name, line[10:13]
        args = list()
        args.append('./bin/sysgen2')
        args.append(str(self._i))
        args.append(str(self._j))
        args.append(str(self._k))

        #args.append('./bin/sysgen')
        #args.append('-d2')
        #args.append('-x')
        #args.append(str(self._i))
        #args.append('-y')
        #args.append(str(self._j))
        #args.append('-z')
        #args.append(str(self._k))
        o = Popen(args, stdout=PIPE).stdout.read().split(b'\n')
        for l in o:
            #print(str(l)[2:len(str(l)) - 1])
            if len(l):
                s = l.strip().split(b'\t')
                if s[0].isdigit():
                    if s[1][:15] == 'Small Gas Giant' or s[1][:15] == 'Large Gas Giant':
                        gasgiant = True
        self.features = '    '
        self.features += t[0]
        self.features += t[1]
        if gasgiant:
            self.features += 'G'
        else:
            self.features += ' '
        self.zone = ' '
    def Starport(self):
        return self.upp[0]
    def Size(self):
        return int(self.upp[1], 25)
    def Atmosphere(self):
        return int(self.upp[2], 25)
    def Hydro(self):
        return int(self.upp[3], 25)
    def Population(self):
        return int(self.upp[4], 25)
    def Government(self):
        return int(self.upp[5], 25)
    def Law(self):
        return int(self.upp[6], 25)
    def Tech(self):
        return int(self.upp[8], 25)
    def Amber(self):
        if self.zone == 'A':
            return True
        return False
    def Red(self):
        if self.zone == 'R':
            return True
        return False
    def Pirate(self):
        if self.features[0] == 'P':
            return True
        return False
    def Consulate(self):
        if self.features[1] == 'C':
            return True
        return False
    def TAS(self):
        if self.features[2] == 'T':
            return True
        return False
    def Research(self):
        if self.features[3] == 'R':
            return True
        return False
    def Navy(self):
        if self.features[4] == 'N':
            return True
        return False
    def Scout(self):
        if self.features[5] == 'S':
            return True
        return False
    def GasGiant(self):
        if self.features[6] == 'G':
            return True
        return False
