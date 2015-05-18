#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import math
from lxml import etree

class svgXML:
	def __init__(self):
		page = etree.Element('svg')
		page.attrib['xmlns'] = "http://www.w3.org/2000/svg"
		page.attrib['version'] = "1.1"
		page.attrib['xlink'] = "http://www.w3.org/1999/xlink"
		page.attrib['width'] = "210mm"
		page.attrib['height'] = "297mm"
		page.attrib['viewBox'] = "-50.0 -50.0 4950.0 7214.80577065"
		doc = etree.ElementTree(page)
		print etree.tostring(page)
		#etree.dump(page)
		doc = etree.parse('t.html')
		root = doc.getroot()
		for i in root.attrib:
			print i
		#for i in root.iter():
			#print i
			#print i.tag
		defs = root.find('{http://www.w3.org/2000/svg}defs')
		etree.dump(defs)
		for i in defs.iter():
			#print i
			print i.tag
		style = defs.find('{http://www.w3.org/2000/svg}style')
		etree.dump(style)
		print etree.tostring(doc)

class svg:
	def __init__(self, sx = -50.000, sy = -50.000, w = 1400.000, h = 2041.858):
		self.displayWidth = '210mm'
		self.displayHeight = '297mm'
		self.viewOriginX = sx
		self.viewOriginY = sy
		self.viewWidth = w
		self.viewHeight = h
		self.description = 'Traveller Subsector'
		self.defs = list()
		self.styles = list()
		self.elements = list()
	def Header(self):
		header = '<?xml version="1.0" encoding="UTF-8" standalone="no"?>\n'
		header += '<!DOCTYPE html>\n'
		header += '<html>\n'
		header += '<body>\n'
		return header
	def SVGStart(self):
		s = '<svg xmlns="http://www.w3.org/2000/svg" version="1.1"\n'
		s += 'xmlns:xlink="http://www.w3.org/1999/xlink"\n'
		s += 'width="' + str(self.viewWidth / 2) + '" height="' + str(self.viewHeight / 2) + '"\n'
		s += 'viewBox="' + str(self.viewOriginX) + ' '
		s += str(self.viewOriginY) + ' '
		s += str(self.viewWidth + self.viewOriginX) + ' '
		s += str(self.viewHeight + self.viewOriginY) + '">'
		return s
	def Description(self):
		s = '<desc>'
		s += self.description
		s += '</desc>'
		return s
	def Defs(self):
		s = '<defs>\n'
		s += self.Style()
		for d in self.defs:
			s += d + '\n'
		s += '</defs>'
		return s
	def AddDef(self, d):
		self.defs.append(d)
	def AddStyle(self, s):
		self.styles.append(s)
	def Style(self):
		s = '<style type="text/css">\n'
		for style in self.styles:
			s += style + '\n'
		s += '</style>\n'
		return s
	def AddElement(self, g):
		self.elements.append(g)
	def Element(self):
		s = ''
		for style in self.elements:
			s += style + '\n'
		return s
	def Print(self):
		print self.Header()
		print self.SVGStart()
		print self.Description()
		print self.Defs()
		print "<g id='grid'>"
		print self.Element()
		print "</g>"
		print "</svg>"
	def Get(self):
		s = self.Header()
		s += self.SVGStart()
		s +=  self.Description()
		s +=  self.Defs()
		s +=  "<g id='grid'>"
		s +=  self.Element()
		s +=  "</g>"
		s +=  "</svg>"
		s +=  "</body>"
		s +=  "</html>"
		return s

class svgHexMap(svg):
	def __init__(self, size = 100.000, t = 'f', v = 10, h = 8):
		self.size = size
		self.type = t
		self.vertical = v
		self.horizontal = h
		self.Cells = dict()
		self.CellsBlue = dict()
		self.CellsGreen = dict()
		self.CellsBrown = dict()
		if t == 'f':
			self.height = math.sqrt(math.pow(self.size, 2) - math.pow((self.size / 2.0), 2)) * 2.0
			self.width = size * 2.0
		else:
			self.height = size * 2.0
			self.width = math.sqrt(math.pow(self.size, 2) - math.pow((self.size / 2.0), 2)) * 2.0
		self.x = self.MapTopX() - 50.0
		self.y = self.MapTopY() - 50.0
		self.w = self.MapBottomX() + 150.0
		self.h = self.MapBottomY() + 250.0
		svg.__init__(self, sx = self.x, sy = self.y, w = self.w, h = self.h)
		self.borderWidth = 10
		self.legendText = '◉ gas giant – ■ imperial consulate – ☼ TAS – ▲ scout base – ★ navy base – π research base – ☠ pirate base – '
		self.legendText += '<tspan class="comm">▮</tspan> communication – <tspan class="trade">▮</tspan> trade'
		self.northText = 'coreward'
		self.southText = 'rimward'
		self.eastText = 'trailing'
		self.westText = 'spinward'
	def MiddleX(self):
		return (self.MapBottomX() - self.MapTopX()) / 2.0
	def MiddleY(self):
		return (self.MapBottomY() - self.MapTopY()) / 2.0
	def MapTopX(self):
		return self.iToX(0, 0) - (self.width / 2.0)
	def MapBottomX(self):
		if self.type == 'f':
			return self.iToX(self.horizontal - 1, self.vertical - 1) + (self.width / 2.0)
		else:
			return self.iToX(self.horizontal - 1, self.vertical - 1) + (self.width) 
	def MapTopY(self):
		return self.jToY(0, 0) - (self.height / 2.0)
	def MapBottomY(self):
		return self.jToY(self.horizontal - 1, self.vertical - 1) + (self.height / 2.0)
	def Stage(self):
		for l in open('cfg/style.txt'):
			self.AddStyle(l.strip())
		for c in ['', 'blue', 'green', 'brown']:
			s = '<polygon id="hex' + c + '" points="'
			if self.type == 'f':
				s += str(self.size * -1.0) + ',' + str(0.000) + ' '
				s += str(self.size * -0.5) + ',' + str(self.height / 2.0) + ' '
				s += str(self.size * 0.5) + ',' + str(self.height / 2.0) + ' '
				s += str(self.size) + ',' + str(0.000) + ' '
				s += str(self.size * 0.5) + ',' + str(self.height / 2.0 * -1) + ' '
				s += str(self.size * -0.5) + ',' + str(self.height / 2.0 * -1) + '" />'
			else:
				s += str(0.000) + ',' + str(self.size * -1.0) + ' '
				s += str(self.width / 2.0) + ',' + str(self.size * -0.5) + ' '
				s += str(self.width / 2.0) + ',' + str(self.size * 0.5) + ' '
				s += str(0.000) + ',' + str(self.size) + ' '
				s += str(self.width / 2.0 * -1) + ',' + str(self.size * 0.5) + ' '
				s += str(self.width / 2.0 * -1) + ',' + str(self.size * -0.5) + '" />'
			self.AddDef(s)
		if self.borderWidth > 0:
			s = '<rect fill="white" stroke="black" stroke-width="' + str(self.borderWidth)
			s += '" id="frame" x="' + str(self.viewOriginX) + '" y="' + str(self.viewOriginY)
			s += '" width="' + str(self.viewWidth + self.viewOriginX)
			s += '" height="' + str(self.viewHeight + self.viewOriginY) + '" />'
			self.AddElement(s)
		s = '<rect fill="white" stroke="black" stroke-width="3"'
		s += ' id="frame" x="' + str(self.MapTopX()) + '" y="' + str(self.MapTopY())
		s += '" width="' + str(self.MapBottomX() - self.MapTopX())
		s += '" height="' + str(self.MapBottomY() - self.MapTopY()) + '" />'
		self.AddElement(s)
		self.Legend()
		self.createGrid()
	def SubsectorBorder(self, i, j):
		for x in range(i, self.horizontal, i):
			m = (self.iToX(x, 0) + self.iToX(x - 1, 0)) / 2.0
			s = '<line stroke="black" stroke-width="3" x1="' + str(m) + '" '
			s += 'y1="' + str(self.MapTopY()) + '" '
			s += 'x2="' + str(m) + '" '
			s += 'y2="' + str(self.MapBottomY()) + '" />'
			self.AddElement(s)
		for y in range(j, self.vertical, j):
			n = (self.jToY(0, y - 1) + self.jToY(1, y)) / 2.0
			s = '<line stroke="black" stroke-width="3" x1="' + str(self.MapTopX()) + '" '
			s += 'y1="' + str(n) + '" '
			s += 'x2="' + str(self.MapBottomX()) + '" '
			s += 'y2="' + str(n) + '" />'
			self.AddElement(s)
	def Legend(self):
		# legend
		s = '<g id="legend">'
		s += '<text class="legend" style="text-anchor: middle" x="' + str(self.MiddleX())
		s += '" y="' + str(self.y + self.h - 75.0) + '">'
		s += self.legendText
		s += '</text>'
		s += '<text class="direction" style="text-anchor: middle" x="' + str(self.MiddleX())
		s += '" y="' + str(self.MapTopY() - 10.0) + '">'
		s += self.northText
		s += '</text>'
		s += '<text style="text-anchor: middle" transform="translate(' + str(self.MapBottomX() + 10.0)
		s += ',' + str(self.MiddleY()) + ') rotate(90)" class="direction">'
		s += self.eastText
		s += '</text>'
		s += '<text class="direction" style="text-anchor: middle" x="' + str(self.MiddleX())
		s += '" y="' + str(self.MapBottomY() + 30.0) + '">'
		s += self.southText
		s += '</text>'
		s += '<text style="text-anchor: middle" transform="translate(' + str(self.MapTopX() - 10.0)
		s += ',' + str(self.MiddleY()) + ') rotate(-90)" class="direction">'
		s += self.westText
		s += '</text>'
		s += '</g>'
		self.AddElement(s)
	def iToX(self, i, j):
		if self.type == 'f':
			return i * (self.size * 1.5) + self.size
		else:
			if j & 0x1:
				return i * self.width + self.width
			return i * self.width + self.width / 2.0
	def jToY(self, i, j):
		if self.type == 'f':
			if i & 0x1:
				return j * self.height + self.height
			return j * self.height + self.height / 2.0
		else:
			return j * (self.size * 1.5) + self.size
	def createGrid(self):
		for i in range(self.horizontal):
			for j in range(self.vertical):
				x = self.iToX(i, j)
				y = self.jToY(i, j)
				self.Cells[str(i + 1).zfill(2) + str(j + 1).zfill(2)] = svgHexCell(i, j, x, y)
				self.CellsBlue[str(i + 1).zfill(2) + str(j + 1).zfill(2)] = svgHexCell(i, j, x, y, 'blue')
				self.CellsGreen[str(i + 1).zfill(2) + str(j + 1).zfill(2)] = svgHexCell(i, j, x, y, 'green')
				self.CellsBrown[str(i + 1).zfill(2) + str(j + 1).zfill(2)] = svgHexCell(i, j, x, y, 'brown')
	def addGrid(self):
		for i in self.Cells:
			self.AddElement(self.Cells[i].PrintGrid())

class svgHexCell:
	def __init__(self, i, j, x, y, color = ''):
		self.i = i
		self.j = j
		self.x = x
		self.y = y
		self.size = 100.00
		self.visible = True
		loc = str(i + 1).zfill(2) + str(j + 1).zfill(2)
		self.grid = '<use xlink:href="#hex' + color + '" x="' + str(x) + '" y="' + str(y) + '" />\n'
		self.coord = '<text class="coordinates" x="' + str(x) + '" y="' + str(y - 60.0) + '">' + loc + '</text>\n'
	def PrintGrid(self):
		if self.visible:
			return self.grid + self.coord
		else:
			return ''

