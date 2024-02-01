import random
from subprocess import *
from table import *

def dice(number_of_dice, sides = 6, remove_lowest = 0):
    value = 0
    d = []
    for i in range(0,number_of_dice):
    	t = random.randint(1,6)
    	d.append(t)
    d.sort()
    d.reverse()
    for j in range(0, remove_lowest):
    	d .pop()
    for v in d:
    	value += v
    return value

class TravellerAttribute:
    def __init__(self, name, value = 0):
        self.name = name
        self.value = value
    def __repr__(self):
        return "Test()"
    def __str__(self):
        return self.name + " " + str(self.value)
    def UPPPart(self):
        s = '%01x' % self.value
        return s.upper()
    def roll(self):
        self.value = dice(3, remove_lowest = 1)

class TravellerCharacter:
	def __init__(self):
		self.age = 18
		self.term = 0
		self.rank = ''
		self.grade = 0
		self.commission = False
		self.resignation = ''
		self.attNames = ['Strength', 'Dexterity', 'Endurance', 'Intelligence',
						 'Education', 'Social']
		self.attributes = dict()
		self.history = ''
		for a in self.attNames:
			self.attributes[a] = TravellerAttribute(a)
		self.drafted = False
	def SetTableManager(self, tm):
		self.tm = tm
	def generate(self):
	    for a in self.attNames:
	        self.attributes[a].roll()
	    self.Enlist()
	    self.SetSex('')
	    self.SetName()
	    c = True
	    self.GetSkill()
	    while c:
	    	self.term += 1
	    	self.history += 'term: ' + str(self.term) + '<br>'
	    	self.Promotion()
	    	for i in range(3):
		    	self.GetSkill()
	    	c = self.RunTerm()
	    self.MusterOut()
	def Promotion(self):
		if self.drafted and self.term == 1:
			return
		if int(self.tm.run('Traveller', 'career', self.careerIndex, 3)) == 0:
			return
		if not self.commission:
			c = dice(2)
			dmName = self.career + ' Commission'
			c += self.GetDM(dmName)
			if c >= int(self.tm.run('Traveller', 'career', self.careerIndex, 3)):
				self.commission = True
				self.grade = 1
				self.rank = self.tm.run(self.career, 'Rank', self.grade)
				self.history += 'Commissioned: ' + self.rank + '<br>'
				self.AutoSkill(self.career + ' ' + self.rank)
				self.GetSkill()
			else:
				return
		promote = dice(2)
		dmName = self.career + ' Promotion'
		promote += self.GetDM(dmName)
		if promote >= int(self.tm.run('Traveller', 'career', self.careerIndex, 4)):
			self.grade += 1
			p = self.tm.run(self.career, 'Rank', self.grade)
			if p != '' and p != 'Error: Out of Range':
				self.rank = p
			self.history += 'Promoted to: ' + self.rank + '<br>'
			self.AutoSkill(self.career + ' ' + self.rank)
			self.GetSkill()
	def AutoSkill(self,condition):
		count = self.tm.getCount('Traveller', 'Auto Skill')
		for i in range(1, count + 1):
			c = self.tm.run('Traveller', 'Auto Skill', i, 0)
			if c == condition:
				skill = self.tm.run('Traveller', 'Auto Skill', i, 1)
				self.IncrementSkill(skill)
	def GetSkill(self):
		i = self.tm.get_random_index('Traveller', 'Skill Types')
		t = self.tm.run('Traveller', 'Skill Types', i, 0)
		a = self.tm.run('Traveller', 'Skill Types', i, 1)
		if a != '':
			v = self.tm.run('Traveller', 'Skill Types', i, 2)
			if self.attributes[a].value < int(v):
				self.GetSkill()
				return
		self.IncrementSkill(self.tm.run(self.career, t))
	def IncrementSkill(self, skill, inc = 1):
		if skill not in self.attributes:
			self.attributes[skill] = TravellerAttribute(skill)
		self.attributes[skill].value += inc
		self.history += '    ' + self.attributes[skill].name + '-' + str(self.attributes[skill].value) + '<br>'
	def MusterOut(self):
		self.history += 'Muster Out:<br>'
		for i in range(self.term):
			self.MusterBenefit()
	def MusterBenefit(self):
		roll = self.tm.get_random_index(self.career, 'Muster Benefit')
		if self.grade >= 5:
			roll += 1
		b = self.tm.run(self.career, 'Muster Benefit', roll, 0)
		inc = self.tm.run(self.career, 'Muster Benefit', roll, 1)
		i = 1
		if inc != '' and inc != 'Error: Out of Range':
			i = int(inc)
		if i == 0 and b in self.attributes:
			self.MusterBenefit()
			return
		if i == 0:
			i = 1
		if b != 'Error: Out of Range':
			self.IncrementSkill(b, i)
	def __str__(self):
		s = ''
		for a in self.attNames:
			s += self.attributes[a].UPPPart()
		return s
	def UPP(self):
		s = ''
		for a in self.attNames:
			s += self.attributes[a].UPPPart()
		return s
	def Name(self):
		return self.name
	def Sex(self):
		return self.sex
	def Career(self):
		return self.career
	def GetTitle(self):
		tableName = 'Noble Title ' + self.sex
		return self.tm.run('Traveller', tableName,
						   self.attributes['Social'].value)
	def SetSex(self, s = ''):
		if(s != ''):
			self.sex = s
		else:
			self.sex = self.tm.run('Traveller', 'sex')
	def SetCareer(self, c = ''):
		if(c != ''):
			self.career = c
		else:
			roll = self.tm.get_random_index('Traveller', 'career')
			self.career = self.tm.run('Traveller', 'career', roll, 0)
	def GetDM(self, s):
		dm = 0
		count = self.tm.getCount('Traveller', 'DM')
		for i in range(1, count + 1):
			if s == self.tm.run('Traveller', 'DM', i, 0):
				attribute = self.tm.run('Traveller', 'DM', i, 2)
				attValue = int(self.tm.run('Traveller', 'DM', i, 3))
				if self.attributes[attribute].value >= attValue:
					dm += int(self.tm.run('Traveller', 'DM', i, 1))
		return dm
	def Enlist(self):
		self.careerIndex = self.tm.get_random_index('Traveller', 'career')
		enlist = dice(2)
		dmName = self.tm.run('Traveller', 'career', self.careerIndex, 0) + ' Enlistment'
		enlist += self.GetDM(dmName)
		if enlist >= int(self.tm.run('Traveller', 'career', self.careerIndex, 1)):
			self.career = self.tm.run('Traveller', 'career', self.careerIndex)
		else:
			self.drafted = True
			self.DesiredCareer = self.tm.run('Traveller', 'career', self.careerIndex)
			self.careerIndex = self.tm.get_random_index('Traveller', 'career')
			self.career = self.tm.run('Traveller', 'career', self.careerIndex)
			self.history += 'Failed Enlistment in the ' +  self.DesiredCareer + '<br>'
			self.history += 'Drafted into the ' +  self.career + '<br>'
		self.AutoSkill(self.career)
	def RunTerm(self):
		# Survival
		retval = True
		survival = dice(2)
		dmName = self.career + ' Survival'
		survival += self.GetDM(dmName)
		if survival >= int(self.tm.run('Traveller', 'career', self.careerIndex, 2)):
			reenlist = dice(2)
			if reenlist >= int(self.tm.run('Traveller', 'career', self.careerIndex, 5)):
				if reenlist == 12:
					self.history += 'Forced Reenlistment<br>'
				else:
					if self.term > 7:
						self.resignation = 'Retired'
						self.history += 'Retired<br>'
						retval = False
			else:
				self.resignation = 'needs of the service'
				self.history += 'Discharged<br>'
				retval = False
			self.age += 4
		else:
			self.resignation = 'Medical Discharge'
			self.history += 'Medical Discharge<br>'
			retval = False
			self.age += dice(1) / 2
		return retval
	def Report(self):
		s = ''
		s += '<pre><br>'
		if self.GetTitle() != '':
			s += self.GetTitle() + '-'
		s += self.name
		if self.rank != '':
			s += ' - ' + self.rank
		s += '<br>'
		s += self.Career() + ' - ' + str(self.term) + '<br>'
		s += self.UPP() + '<br>'
		s += 'age: ' + str(self.age) + '<br>'
		s += 'History:<br>'
		s += self.history
		s += '</pre>'
		return s
	def SetName(self, n = ''):
		if(n != ''):
			self.name = n
		else:
			self.name = self.tm.run('American Census Names', self.sex.lower()).lstrip()
			self.name += ' '
			self.name += self.tm.run('American Census Names', 'surname').lstrip()

class generator:
	def version(self):
		return 2.0
	def start(self):
		#result = Popen("ls", stdout=PIPE, shell=True).stdout.read()
		f = TravellerCharacter()
		f.SetTableManager(self.tm)
		f.generate()
		return f.Report()
	def SetManager(self, tm):
		self.tm = tm

if __name__ == '__main__':
	#x = generator()
	#print x.start()
	for j in range(0, 100):
		f = TravellerCharacter()
		f.generate()
		print(f.UPP())

