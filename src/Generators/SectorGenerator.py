
from secgen import *
from secgen.subSector import *
from subprocess import *

class SectorGenerator:
	def __init__(self):
		self.parameters = dict()
		self.parameters['x'] = '0'
		self.parameters['y'] = '0'
		self.parameters['z'] = '0'
		self.parameters['subsector'] = ['', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16']
		# order of the parameters to be displayed
		self.pList = ['x', 'y', 'z', 'subsector']
	def Update(self, p):
		pass
	def roll(self, p, numRolls):
		subSector = ""
		if p.has_key('subsector'):
			subSector = p['subsector']
		x = p['x']
		y = p['y']
		z = p['z']
		sx = int(x) * 32
		sy = int(y) * 40
		sz = int(z)
		ex = sx + 32
		ey = sy + 40
		ez = sz + 1
		t = ''
		if subSector == "":
			s = subSectorMap(t = 'sec')
			t = "Sector "
			print sx, sy, sz, ex, ey, ez
		else:
			s = subSectorMap(t = 'sub')
			sx = sx + (8 * ((int(subSector) - 1) % 4))
			ex = sx + 8
			sy = sy + (10 * ((int(subSector) - 1) / 4))
			ey = sy + 10
			t = "SubSector "
		t += x + " " + y + " " + z + " " + subSector
		s.clearSystems()
		for i in range(sx, ex):
			for j in range(sy, ey):
				for k in range(sz, ez):
					args = list()
					args.append('./bin/sysgen1')
					args.append(str(i))
					args.append(str(j))
					args.append(str(k))
					#print args
					o = Popen(args, stdout=PIPE).stdout.read().split('\n')
					for l in o:
						if len(l):
							s.addSystem(starSystem(l, version = 2))
		return t, s.getMap()
		
