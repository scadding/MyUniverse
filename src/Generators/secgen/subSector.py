#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from src.Generators.secgen.starSystem import *
from src.Generators.secgen.svg import *
import math

def rad2deg(radians):
    degrees = 360 * radians / (2.0 * math.pi)
    return degrees
def deg2rad(degrees):
    radians = (2.0 * math.pi) * degrees / 360
    return radians

class svgHexCellDecoration:
    def __init__(self):
        self.i = 0
        self.j = 0
        self.radius = 0.0
        self.angle = 0.0
        self.className = ''

class subSectorMap(svgHexMap):
    systems = list()
    decorations = dict()
    def __init__(self, t='sub'):
        self.sv = 1
        self.sh = 1
        if t == 'sub':
            self.sv = 10
            self.sh = 8
        else:
            self.sv = 40
            self.sh = 32
        svgHexMap.__init__(self, v=self.sv, h=self.sh)
        svgHexMap.Stage(self)
        if t != 'sub':
            svgHexMap.SubsectorBorder(self, 8, 10)
    def clearSystems(self):
        del self.systems[:]
    def addSystem(self, system):
        self.systems.append(system)
    def addDecorationType(self, classname, decoration):
        decorations[classname] = decoration
    def textDecoration(self, className, text, radius, angle, x, y):
        x1 = math.cos(deg2rad(angle)) * radius + x
        y1 = math.sin(deg2rad(angle)) * radius + y
        s = '<text class="' + className + '" x="' + str(x1) + '" y="' + str(y1) + '">' + text + '</text>'
        return s
    def circleDecoration(self, className, size, radius, angle, x, y):
        x1 = math.cos(deg2rad(angle)) * radius + x
        y1 = math.sin(deg2rad(angle)) * radius + y
        s = '<circle class="' + className + '" cx="' + str(x1) + '" cy="' + str(y1) + '" r="' + str(size) + '"/>'
        return s
    def getSystem(self, system):
        x = self.iToX(system.i() % self.sh, system.j() % self.sv)
        y = self.jToY(system.i() % self.sh, system.j() % self.sv)
        s = '<a xlink:href="' + system.url + '">'
        s += '<g id="' + system.name + '">'
        if system.Hydro() > 0:
            s += self.circleDecoration('', 11.0 + system.Size(), 0, 0, x, y)
        else:
            s += self.circleDecoration('hydro', 11.0 + system.Size(), 0, 0, x, y)
        if system.Amber():
            s += self.circleDecoration('code amber', 52.0, 0, 0, x, y)
        elif system.Red():
            s += self.circleDecoration('code red', 52.0, 0, 0, x, y)
        s += self.textDecoration('starport', system.Starport(), 30.0, 270.0, x, y)
        if system.Population() > 8:
            s += self.textDecoration('name', system.name.upper(), 45.0, 90.0, x, y)
        else:
            s += self.textDecoration('name', system.name, 45.0, 90.0, x, y)
        if system.Pirate():
            s += self.textDecoration('pirate base', '☠', 60.0, 5, x, y)
        if system.Consulate():
            s += self.textDecoration('consulate base', '■', 60.0, 145.0, x, y)
        if system.TAS():
            s += self.textDecoration('TAS base', '☼', 60.0, 175, x, y)
        if system.Research():
            s += self.textDecoration('research base', 'π', 60.0, 345, x, y)
        if system.Navy():
            s += self.textDecoration('naval base', '★', 60.0, 225, x, y)
        if system.Scout():
            s += self.textDecoration('scout base', '▲', 60.0, 200, x, y)
        if system.GasGiant():
            s += self.textDecoration('gasgiant base', '◉', 60.0, 315, x, y)
        s += '</g>'
        s += '</a>'
        return s
    def printMap(self):
        for i in range(self.horizontal):
            for j in range(self.vertical):
                self.AddElement(self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)].PrintGrid())                
        for s in self.systems:
            #i = s.i
            #j = s.j
            #self.AddElement(self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)].PrintGrid())
            self.AddElement(self.getSystem(s))
        self.Print()
    def getMap(self):
        for i in range(self.horizontal):
            for j in range(self.vertical):
                self.AddElement(self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)].PrintGrid())                
        for s in self.systems:
            #i = s.i
            #j = s.j
            #self.AddElement(self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)].PrintGrid())
            self.AddElement(self.getSystem(s))
        return self.Get()

class planetMap(svgHexMap):
    systems = list()
    decorations = dict()
    def __init__(self, mapdata):
        svgHexMap.__init__(self, h = 30, v = 19, t = 's')
        svgHexMap.Stage(self)
        self.mapdata = mapdata
    def printMap1(self):
        for i in range(self.horizontal):
            for j in range(self.vertical):
                if j < 6:
                    l = range(0, 34, 6)
                    t = range(-1 * ((j + 1)/ 2), (j / 2) + 1)
                    z = list()
                    for a in l:
                        for b in t:
                            z.append(a + b)
                    if i in z:
                        self.AddElement(self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)].PrintGrid())
                elif j < 12:
                    self.AddElement(self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)].PrintGrid())
                else:
                    l = range(2, 34, 6)
                    j1 = 17 - j
                    t = range(-1 * ((j1)/ 2), ((j1 + 1) / 2) + 1)
                    z = list()
                    for a in l:
                        for b in t:
                            z.append(a + b)
                    if i in z:
                        self.AddElement(self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)].PrintGrid())
        self.Print()
    def printMap(self):
        i = 0
        l = list()
        for line in self.mapdata:
            i += 1
            s = ''
            if i < 6:
                j = 6 - i
                for x in range(j):
                    s = s + '  '
            if i &0x1:
                l.append(line[6:].rstrip() + s + line[:6].strip())
            else:
                l.append(line[7:].rstrip() + s + line[:7].strip())
        j = 0
        for line in l:
            if len(line) == 0:
                continue
            j += 1
            x = 0
            for i in range(0, 60, 2):
                x += 1
                s = line[i:i+2]
                if len(s) == 0 or s == '  ':
                    continue
                print(s)
                if s == 'WW':
                    self.AddElement(self.CellsBlue[str(x).zfill(2) + str(j).zfill(2)].PrintGrid())
                elif s == 'LL':
                    self.AddElement(self.CellsGreen[str(x).zfill(2) + str(j).zfill(2)].PrintGrid())
                elif s == 'll':
                    self.AddElement(self.CellsBrown[str(x).zfill(2) + str(j).zfill(2)].PrintGrid())
                else:
                    self.AddElement(self.Cells[str(x).zfill(2) + str(j).zfill(2)].PrintGrid())
        self.Print()
mapdata = '''
      WW          WW          WW          WW          WW     
     WWWW        WWWW        WWWW        WWWW        WWWW    
    WWWWWW      WWWWWW      WWWWWW      WWWWWW      WWWWWW   
   WWWWWWWW    WWWWWWWW    WWWWWWLL    LLLLLLWW    WWWWWWWW  
  wwLLWWWWWW  WWWWWWWWWW  WWWWWWLLLL  LLLLLLLLLL  LLLLLLLLww 
 LLLLLLWWWWWWWWWWWWWWWWwwwwLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
wwLLwwLLWWWWWWWWWWWWWWLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLww
 LLLLLLLLWWWWWWWWWWWWLLLLLLLLLLLLLLLLLLLLLLLLLLWWLLLLLLLLLLLLLL
  LLLLLLLLWWWWWWWWWWLLLLLLLLLLLLwwLLLLLLLLLLLLLLLLwwLLLLLLLLLLLL
   LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLwwLLLLWWLLLLLLLLLLLL
    LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLWWLLLLLL
     LLLLLLLLLLWWLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLWWLLLLLLLLLL
      LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
       LLLLLLLLLLLLLLLLLLWWLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLwwLLLLLL
        LLLLLLWWLL  LLLLLLLLLL  LLLLLLLLLL  LLWWLLLLLL  LLLLLLLLLL 
         LLLLLLLL    LLLLLLLL    LLLLLLLL    LLLLLLLL    LLwwLLLL  
          LLLLLL      LLLLLL      LLWWLL      LLLLLL      LLLLLL   
           LLLL        LLLL        LLLL        LLLL        LLLL    
            LL          LL          LL          LL          LL     
'''

if __name__ == '__main__':
    p = planetMap(mapdata)
    p.printMap()
    #for line in data1.split('\n'):
        #print line
    for line in data2.split('\n')[2:]:
        print(line)
    i = 0
    l = list()
    for line in data2.split('\n')[2:]:
        i += 1
        s = ''
        if i < 6:
            j = 6 - i
            for x in range(j):
                s = s + '  '
        if i &0x1:
            l.append(line[6:].rstrip() + s + line[:6].strip())
        else:
            l.append(line[7:].rstrip() + s + line[:7].strip())
    #for line in data3.split('\n'):
        #print line
    j = 0
    for line in l:
        if len(line) == 0:
            continue;
        j += 1
        print(line)
        x = 0
        for i in range(0, 60, 2):
            x += 1
            s = line[i:i+2]
            if len(s) == 0 or s == '  ':
                continue
            print(str(x).zfill(2) + str(j).zfill(2), '\'' + s + '\'')
