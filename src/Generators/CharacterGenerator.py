
from npcgen.charactermaker import * 


class CharacterGenerator:
	def __init__(self):
		self.xmlData = GetXMLData()
		self.attribmap, self.classes, self.genreMap, self.weapons, self.armor, self.nameMap = self.xmlData
		self.parameters = dict()
		self.parameters['Seed'] = ['', '0']
		self.parameters['Power'] = '1.0'
		self.parameters['Genre'] = self.getGenreMap()
		self.parameters['Class'] = []
		self.pList = ['Seed', 'Power', 'Genre', 'Class']
	def getGenreMap(self):
		aliases = self.classes.keys()
		aliases.sort()
		genreNames = self.genreMap.keys()
		genreNames.sort()
		return genreNames
	def getAliasMap(self):
		aliases = self.classes.keys()
		aliases.sort()
		return aliases
	def getClassList(self, genre):
		classList = []
		for c in self.genreMap[genre]:
			if not c.isAbstract():
				classList.append(c.getName())
		classList.sort()
		return classList
	def Update(self, p):
		self.parameters['Class'] = self.getClassList(p['Genre'])
	def getAlias(self, p, c):
		for t in self.genreMap[p['Genre']]:
			if t.getName() == c:
				if t.isAbstract():
					return '*' + t.getAlias()
				return t.getAlias()
		return '*random'
	def roll(self, p, numRolls):
		stack = []
		content = GenerateHTMLString([self.getAlias(p, p['Class'])], self.xmlData, power=float(p['Power']), mode=1, stack = stack).encode('utf-8')
		name = ''
		for c in stack[0].getName():
			if ord(c) > 26:
				name = name + c
		return name,  content
		
		
if __name__ == '__main__':
	args = sys.argv
	if len(args) > 1:
		xmlData = GetXMLData()
		html = 0
		if args[1] == 'html':
			args = args[1:]
			html = 1

		if args[1] == 'alias':
			PrintAliasMap(xmlData)
		elif args[1].lower() in ('h', 'help', '/?', '/h', '-?', '-h', '--help'):
			PrintHelp(args[0])
		else:
			print args[1:]
			#print GenerateHTMLString(args[1:], xmlData, power=1, mode=1)
	else:
		PrintHelp(args[0]) 
