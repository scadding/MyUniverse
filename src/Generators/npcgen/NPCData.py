#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $Id: NPCData.py 212 2009-09-03 22:26:41Z andrew $
 * $URL: http://ayinger.no-ip.info/repos/npcgen/trunk/NPCData.py $
 *
 * $Date: 2009-09-03 17:26:41 -0500 (Thu, 03 Sep 2009) $
 * $Author: andrew $
 *
 * Copyright (c) 2008  Andrew C. Yinger
 * All rights reserved.
 *
 * Description: 
 *
 * NPCData, used to generate 'real' npc data values and display them.
 *
 *******************************************************************************
"""

from ClassNames import GetFullName
from Utils import randint, getModeValue, GetInt, GetRootDir, RandDistribInt
from cPickle import dumps
from pprint import PrettyPrinter
from random import choice, shuffle, randint
from sets import Set, ImmutableSet
from time import time, strftime
from zlib import compress
import math, sys, os, codecs
import NPCConstants



# static data
kPageWidth = 88
kDwarfismModifier = 0.6
kGigantismModifier = 1.2
kNPCTemplateFileName = 'NPCTemplate.html'
kDateTimeFormat = "%Y-%m-%d %H:%M:%S"
kuBOM = unicode(codecs.BOM_UTF8, 'utf-8')
#print GetRootDir()

def GetNPC(xmlData, classAlias, power, id):
	'return a unique npc'
	attribmap, classes, genreMap, weapons, armor, nameMap = xmlData

	classType = GetClassType(classAlias, classes)
	if classType:
		return NPCBuilder(id, power, classType, classes, attribmap, weapons, armor, nameMap).getNPC()
	else:
		print "sorry, could not find non-abstract class alias or name '%s'" % (classAlias,)
		sys.exit(-1)


def GetClassType(classPointer, classes):
	'get a classType given a classAlias or name (must be non-abstract)'
	classType = None
	if classPointer in classes:
		classType = classes[classPointer]
	elif classPointer in ImmutableSet([classtype.getName() for classtype in classes.values()]):
		classType = [classtype for classtype in classes.values() if classtype.getName() == classPointer][0]
	if classType and not classType.isAbstract():
		return classType


class NPC(object):
	'a container for generated npc data'
	encoding = 'UTF-8'
	def __init__(self, data):
		self.mData = data
		self.title = None
		self.rank = None

	# convenience gettrs
	def __str__(self):
		return self.getTitle()
	def __rpr__(self):
		return self.getTitle()

	def getFullRankName(self):
		return u'%s%s' % (self.getRank(), self.mData['name'])
	def getRank(self):
		'return text rank from rank level'
		if self.rank is None:
			rank = ''
			for rankType, rankDict in (('paramilitary-rank', NPCConstants.kParaRankLevels), ('rank', NPCConstants.kRankLevels)):
				if (rankType in self.mData['advantages']) and (self.mData['advantages'][rankType] in rankDict):
					rank = choice(rankDict[self.mData['advantages'][rankType]]) + ' '
			self.rank = rank
		return self.rank
	def getName(self):
		return self.getFullRankName()
	def getTitle(self):
		if not self.title:
			selfstring = u''
			selfstring += '%s: %s%s' % (self.mData['class'], self.getRank(), self.mData['name'])
			self.title = selfstring
		return self.title

	def getDefenses(self):
		return (self.mData['combat']['dodge'], self.mData['combat']['parry'], self.mData['combat']['block'], self.mData['combat']['baseDr'])

	def getDamageSets(self):
		return (self.mData['combat']['baseDamTh'], self.mData['combat']['baseDamSw'], self.mData['combat']['punchDam'], self.mData['combat']['kickDam'],
			   self.mData['combat']['knifeDamTh'], self.mData['combat']['knifeDamSw'])

	def getStats(self):
		return self.mData['stats']
	def getAppearance(self):
		return self.mData['appearance']
	def getSkills(self):
		return self.mData['skills']
	def getAdvantages(self):
		return self.mData['advantages']
	def getDisadvantages(self):
		return self.mData['disadvantages']
	def getAttributes(self):
		return self.mData['attributes']

	def getImage(self):
		return self.mData['image']

	def getType(self):
		return self.mData['class']

	def getClass(self):
		return self.mData['class']

	def getNationality(self):
		return self.mData['nationality']

	def getTL(self):
		return self.mData['tech-level']

	def getTechLevel(self):
		return self.mData['tech-level']

	def getGenre(self):
		return self.mData['genre']

	def getPoints(self):
		return self.getTotalPoints()

	def getCost(self, key=None):
		if key:
			return self.mData['costs'][key]
		return self.getTotalPoints()

	def getGender(self):
		return self.mData['gender']

	def getSex(self):
		return self.mData['gender']

	def getDBData(self, binary=1):
		if binary:
			return compress(dumps(self, 2), 9)
		return compress(dumps(self, 2), 9).encode('hex')

	def getDBValues(self):
		tl = self.getTL()
		if (tl in ('*', '?')): tl = -1
		#print (strftime(kDateTimeFormat), self.getType(), self.getName(), self.getGender(), self.getNationality(), self.getTL(), self.getGenre(), self.getPoints(), self.getDBData(1), self.getGURPSVersion())
		return (strftime(kDateTimeFormat), self.getType(), self.getName(), self.getGender(), self.getNationality(), tl, self.getGenre(), self.getPoints(), self.getDBData(1), self.getGURPSVersion())

	def getGURPSVersion(self):
		return 4.0

	def getTotalPoints(self):
		return self.mData['costs']['total']

	def setGenInfo(self, selection, power, address):
		self.stime = strftime(kDateTimeFormat)
		self.selection = selection
		self.power = power
		self.address = address

	def getGenInfo(self):
		return (self.stime, self.selection.encode('UTF-8'), self.power, self.address)
		


class NPCBuilder(object):
	'a factory builder for making npcs'
	def __init__(self, id, power, classType, classes, attribmap, weapons, armor, nameMap):
		'populate mData for this npc'
		self.skillCosts = self.attributeCosts = self.statCosts = self.advantageCosts = self.disadvantageCosts = 0
		self.attribCostSet = [0, 0]
		self.rankName = ''
		classData = classType.getData(classes)
		nationality = choice(classData['class']['nationality'].split(', '))
		fullName, gender = GetFullName(nationality, int(classData['class']['gender-ratio']), nameMap)
		self.mData = {
						'class':		classData['class']['name'],
						'power':		power,
						'number':		(id + 1),
						'name':			fullName,
						'nationality':	nationality,
						'gender':		gender,
						'hair':			(),
						'eyes':			(),
						'age':			classData['class']['age'],
						'genre':		classData['class']['genre'],
						'tech-level':	self.getRandomDistribValue(classData['class']['tech-level'])
					 }
		# other locals
		itemPowerMod = self.getItemPowerModifier(classType.getItemModInc(), power)

		for trait in ('hair', 'eyes'):
			if trait in classData['class']:
				self.mData[trait] = classData['class'][trait].split(', ')

		# populate self.mData's (independent) statDict
		statDict = {}
		for attribKey in ('stats', 'advantages', 'disadvantages'):
			attribSet = classType.getAttribSet(attribKey)
			statDict[attribKey] = attribSet
		self.mData['image'] = self.getNPCImage(gender, classData, statDict)

		# apply power multiplier to stats
		if power != 1.0:
			#print power, int(round(1 * power))
			for attribKey in ('stats', 'advantages'):
				for key in statDict[attribKey]:
					origValue = statDict[attribKey][key]
					newValue = self.getPowerAdjustedValue(classData, attribKey, key, power, origValue)
					if newValue != origValue:
						statDict[attribKey][key] = newValue

		# check for disad exclusion-criteria
		keys = statDict['disadvantages'].keys()
		for disad in keys:
			if (disad in attribmap['skill-mods']) and ('exclusion-criteria' in attribmap['skill-mods'][disad]):
				for adDisad in attribmap['skill-mods'][disad]['exclusion-criteria'].split(', '):
					if (adDisad in statDict['advantages']) or (adDisad in statDict['disadvantages']):
						if statDict['disadvantages'].has_key(disad):
							del statDict['disadvantages'][disad]

		# now, populate self.mData's (dependant) raw data
		for attribKey in NPCConstants.kMainAttribKeyOrder:
			self.mData[attribKey] = {}

			if attribKey in NPCConstants.kMainAttributeKeys:
				self.mData[attribKey] = statDict[attribKey]
			elif attribKey == 'appearance':
				self.mData[attribKey]['height'], self.mData[attribKey]['weight'], self.mData[attribKey]['eyes'], self.mData[attribKey]['hair'], self.mData[attribKey]['age'] = self.getAppearance(statDict, gender)
				self.mData[attribKey]['sex'] = self.mData['gender']
				self.mData[attribKey]['nationality'] = self.mData['nationality']
				self.mData[attribKey]['genre'] = self.mData['genre']
				self.mData[attribKey]['tech-level'] = self.mData['tech-level']

			elif attribKey == 'quirks':
				quirkVerbs = classType.getAttribSet('quirk-verbs')
				quirkNouns = classType.getAttribSet('quirk-nouns')
				quirkObjects = quirkNouns.keys()
				#shuffle(quirkObjects)
				quirks = []
				for quirkVerb in quirkVerbs:
					quirkNoun = choice(quirkObjects)
					quirkObjects.remove(quirkNoun)
					quirks.append('%s %s' % (quirkVerb.replace('_', ' '), quirkNoun.replace('_', ' ')))
				quirks.sort()
				self.mData['quirks'] = quirks

			elif attribKey in NPCConstants.kSkillAttributeKeys:
				# populate self.mData with final values for these
				attribSet = classType.getAttribSet(attribKey, attribmap[attribKey])
				keys = attribSet.keys()
				#@
				#if attribKey == 'attributes': print attribSet
				for key in keys:
					value = attribSet[key]
					# increment basic skill costs before value add
					if (attribKey == 'skills') and (key not in self.mData[attribKey]):
						self.addSkillCost(attribmap, key, value)
					elif attribKey == 'attributes':
						cost = self.getStatCost(attribmap[attribKey], key, value)
						self.attributeCosts += cost
						if cost > 0:
							self.attribCostSet[0] += cost
						elif cost < 0:
							self.attribCostSet[1] += cost
					value += self.getStatValue(attribmap, attribKey, key, statDict)
					if not key in self.mData[attribKey]:
						self.mData[attribKey][key] = value
						# add any preReqs...
						if (attribKey == 'skills') and ('prereqs' in attribmap[attribKey][key]):
							self.addPreRequisite(classData, statDict, attribmap, power, key, key)
				# now reconcile move/speed
				if attribKey == 'attributes':
					##print 'speed increased by: %s' % attribSet['speed']
					speed = self.mData['attributes']['speed']; move = int(self.mData['attributes']['move'])
					if math.modf(speed)[0] == .75 and randint(0, 1):
						##print 'speed ended in .75, so bumping and adjusting costs...'
						self.mData['attributes']['speed'] += .25; speed += .25
						self.attributeCosts += 5
						self.attribCostSet[0] += 5
					if (int(speed) > move) and (attribSet['move'] != -1):
						##print 'move was %s, now setting to %s' % (move, speed)
						self.mData['attributes']['move'] = speed
					elif (int(speed) < move) and (attribSet['move'] != 1):
						##print 'move was %s, now setting to %s' % (move, speed)
						self.mData['attributes']['move'] = speed
					##print self.attribCostSet

			elif attribKey == 'maneuvers' and classType.hasAttribSet(attribKey):
				attribSet = classType.getAttribSet(attribKey, attribmap['skills'])
				keys = attribSet.keys()
				for key in keys:
					value = attribSet[key]
					if not self.mData['skills'].has_key(key):
						self.addSkillCost(attribmap, key, value)
					# add any maneuver prereqs...
					if 'prereqs' in attribmap['skills'][key]:
						self.addPreRequisite(classData, statDict, attribmap, power, key, key)
					# add stat (skill) value (note that we have to add skills to the statDict)
					statDict['skills'] = self.mData['skills']
					value += self.getStatValue(attribmap, 'skills', key, statDict)
					key = '*' + key
					if not self.mData['skills'].has_key(key):
						self.mData['skills'][key] = value

			elif (attribKey in NPCConstants.kItemAttributeKeys) and classType.hasAttribSet(attribKey):
				if attribKey == 'weapons':
					items = weapons
				elif attribKey == 'armor':
					items = armor
				attribSet = classType.getAttribSet(attribKey)
				for key in attribSet:
					# get a new item
					item = items.getItem(key, itemPowerMod)
					addItem = 1
					# make sure *this* NPC should get *this* item...
					if (attribKey == 'weapons') and ('skills' in item):
						addItem = 0
						for skillName in item['skills'].split(', '):
							if skillName in self.mData['skills']:
								addItem = 1
								break
						if addItem:
							skills = item['skills']
							for weaponKey in self.mData[attribKey]:
								if ('skills' in self.mData[attribKey][weaponKey]) and (self.mData[attribKey][weaponKey]['skills'] == skills):
									addItem = 0
									break
						else:
							pass
					elif (attribKey == 'armor'):
						area = item['area']
						for armorKey in self.mData[attribKey]:
							if self.mData[attribKey][armorKey]['area'] == area:
								addItem = 0
								break
					if addItem:
						for distribKey in item:
							if '~' in item[distribKey]:
								item[distribKey] = self.getRandomDistribValue(item[distribKey])
						self.mData[attribKey][key] = item

			elif attribKey == 'combat':
				# note: it is very important that combat get processed AFTER 'weapons' and 'skills'
				self.mData[attribKey]['dodge'], self.mData[attribKey]['parry'], self.mData[attribKey]['block'], self.mData[attribKey]['baseDr'] = self.getDefenses()
				baseDamTh, baseDamSw, punchDam, kickDam, knifeDamTh, knifeDamSw = self.getDamageSets()
				self.mData[attribKey]['baseDamTh'] = baseDamTh
				self.mData[attribKey]['baseDamSw'] = baseDamSw
				self.mData[attribKey]['punchDam'] = punchDam
				self.mData[attribKey]['kickDam'] = kickDam
				self.mData[attribKey]['knifeDamTh'] = knifeDamTh
				self.mData[attribKey]['knifeDamSw'] = knifeDamSw

		# calculate final points now that all preReqs have been traversed
		for attribKey in NPCConstants.kMainAttributeKeys:
			for key, value in self.mData[attribKey].items():
				if attribKey == 'stats':
					self.statCosts += self.getStatCost(NPCConstants.kBaseStatCosts, key, value)
				elif attribKey == 'advantages':
					self.advantageCosts += self.getStatCost(classData[attribKey], key, value)
				elif attribKey == 'disadvantages':
					self.disadvantageCosts += self.getStatCost(classData[attribKey], key, value)

		self.disadvantageCosts -= len(quirks)
		self.totalPoints = self.skillCosts + self.attributeCosts + self.advantageCosts + self.disadvantageCosts + self.statCosts
		self.mData['costs'] = {'total':self.totalPoints, 'skills':self.skillCosts, 'attributes':self.attributeCosts,
							   'advantages':self.attribCostSet[0] + self.advantageCosts, 'disadvantages':self.attribCostSet[1] + self.disadvantageCosts, 'stats':self.statCosts}

		# remove any unneccesary armor (NOTE: this is an expensive test!!!)
		removeArmorKeys = []
		for armorKey in self.mData['armor']:
			if self.mData['armor'][armorKey]['area'] == 'body ':
				bodyDr = GetInt(self.mData['armor'][armorKey]['dr'])
				for testKey in self.mData['armor']:
					if self.mData['armor'][testKey]['area'] == 'torso':
						testDr = GetInt(self.mData['armor'][testKey]['dr'])
						if (testDr <= bodyDr) or (8 > bodyDr > 4):
							removeArmorKeys.append(testKey)
						# break out now, since there can only be one 'torso' match
						break
				# break out now, since there can only be one 'body ' match
				break

		for removeArmorKey in removeArmorKeys:
			del self.mData['armor'][removeArmorKey]

		# calculate and set encumbrance
		self.mData['encumbrance'] = {}
		for itemTypeSet in (('armor', 'weight'), ('weapons', 'wt')):
			itemType, weightString = itemTypeSet
			weight = 0
			for item in self.mData[itemType]:
				if weightString in self.mData[itemType][item]:
					rawWeight   = self.mData[itemType][item][weightString]
					weightFloat = float(rawWeight[:rawWeight.find('lbs')])
					# armor special cases
					if itemType == 'armor':
						for disad, modifier in (('dwarfism', kDwarfismModifier), ('gigantism', kGigantismModifier)):
							if disad in self.mData['disadvantages']:
								weightFloat *= modifier
								self.mData[itemType][item][weightString] = str(round(weightFloat, 2)) + 'lbs'
					# weapon special cases
					else:
						if ('quantity' in self.mData[itemType][item]) and (int(self.mData[itemType][item]['quantity']) > 1):
							weightFloat *= int(self.mData[itemType][item]['quantity'])
						elif 'ammo' in self.mData[itemType][item]:
							weightFloat += (int(self.mData[itemType][item]['ammo']) * float(self.mData[itemType][item]['awt']))
					weight += weightFloat
			# set total weight for each type
			self.mData['encumbrance'][itemType] = weight

		miscWeight = 0 #(randint(2, 20)) * .1
		self.mData['encumbrance']['total'] = self.mData['encumbrance']['armor'] + self.mData['encumbrance']['weapons'] + miscWeight
		weightLevels = NPCConstants.kEncumbranceTable.keys()
		weightLevels.sort()
		# set defaults
		maxWeight = weightLevels[-1]
		encumbranceLevel = NPCConstants.kEncumbranceTable[weightLevels[-1]]
		# check for lowest ('lightest') match
		for weightLevel in weightLevels:
			maxWeight = int(self.mData['stats']['strength']) * weightLevel
			if self.mData['encumbrance']['total'] <= maxWeight:
				level, levelDesc = NPCConstants.kEncumbranceTable[weightLevel]
				if ('extra-encumbrance' in self.mData['advantages']) and (level > 1):
					lMaxWeight = weightLevels[weightLevels.index(weightLevel) - 1]
					encumbranceLevel = ((level - 1), (NPCConstants.kEncumbranceTable[lMaxWeight][1] + '*'))
				else:
					encumbranceLevel = (level, levelDesc)
				break
		# set final values in self.mData Map
		self.mData['encumbrance']['maxWeight'] = maxWeight
		self.mData['encumbrance']['level'] = encumbranceLevel


	def getStatCost(self, statmap, key, value=1):
		if type(statmap) == type(()):
			index = value - 1
			if key in ('strength', 'dexterity', 'intelligence', 'health'):
				index = value - 9
			if index > (len(statmap) - 1):
				increment = statmap[-1] - statmap[-2]
				cost = statmap[-1] + (increment * (index - (len(statmap) - 1)))
			elif index < 0:
				increment = statmap[0]
				cost = (increment * (10 - value))
			else:
				cost = statmap[index]
			if key in ('dexterity', 'intelligence'): cost *= 2
			#if key in ('strength', 'dexterity', 'intelligence', 'health'): print '%s costs %s' % (key, cost)
		else:
			try:
				costs = []
				if 'costs' in statmap[key]:
					costs = statmap[key]['costs'].split(', ')
				elif 'cost' in statmap[key]:
					cost = float(statmap[key]['cost'])
				else:
					print '! cannot find %s %s' % (key, value)
					cost = 1.333
				# check for multi-level costs
				if (len(costs) == 1):
					cost = abs(float(costs[0])) * value
				elif (len(costs) > 1):
					if 'zeromin' in statmap[key]:
						index = abs(value)
					else:
						index = abs(value) - 1
					cost = float(costs[index])
				##print '%s, at level %s, costs %s points' % (key, value, cost)
			except Exception, why:
				print why
				print key, value, costs
				cost = 1.333
		return cost


	def addSkillCost(self, attribmap, skillKey, value):
		stats = attribmap['skills'][skillKey]['stat'].split(', ')
		min = int(attribmap['skills'][skillKey]['min'])
		# remember: spell mins are off by 1 in the attribmap, because half-point may not be spent!
		if (skillKey[:5] == 'spell') and (skillKey != 'spell-throwing'):
			min -= 1;

		if ('intelligence' in stats) and (min > -4):
			statCostTable = NPCConstants.kMentalSkillCosts
		elif 'intelligence' in stats:
			statCostTable = NPCConstants.kMentalVHSkillCosts
		else:
			statCostTable = NPCConstants.kPhysicalSkillCosts
		index = value - min
		if index > (len(statCostTable) - 1):
			increment = statCostTable[-1] - statCostTable[-2]
			cost = statCostTable[-1] + (increment * (index - (len(statCostTable) - 1)))
		else:
			cost = statCostTable[index]
		#print 'skill %s, at stat plus %s, costs %s points' % (skillKey, value, cost)
		#if cost == .5:
		#	print 'skill %s costs %s, therefore i am removing...' % (skillKey, cost)
		#	del self.mData['skills'][skillKey]
		#else:
		self.skillCosts += cost


	def getItemPowerModifier(self, itemModInc, power):
		'return itemPowerMod, int'
		#if power > 1.0:
		itemPowerMod = int((power - 1) * 50) + itemModInc
		return itemPowerMod


	def getDamageSets(self):
		'return (baseDamTh, baseDamSw, punchDam, kickDam, knifeDamTh, knifeDamSw)'
		# calc base damage
		strength				= int(self.mData['stats']['strength'])
		if NPCConstants.kStrengthTable.has_key(strength):
			thrust, swing			= NPCConstants.kStrengthTable[strength]
		elif strength < 5:
			thrust, swing			= NPCConstants.kStrengthTable[5]
		elif strength > 26:
			thrust, swing			= NPCConstants.kStrengthTable[26]
			thrust					= (thrust[0], thrust[1] + ((strength - 26) / 2))
			swing					= (swing[0], swing[1] + (strength - 26))

		thrustDice, thrustMod	= thrust
		swingDice,  swingMod	= swing
		if thrustMod >= 0: thrustMod = '+' + str(thrustMod)
		if swingMod >= 0: swingMod = '+' + str(swingMod)

		# calc melee damage
		punchDice, punchMod = thrust
		knifeMod = punchMod
		knifeSwing = swing[1] - 2
		punchMod -= 2

		# change mods for skills/advantages		
		for unarmedSkill, divisor in NPCConstants.kUnarmedSkillDivisorSets:
			if unarmedSkill in self.mData['skills']:
				punchMod += int(self.mData['skills'][unarmedSkill] / divisor)
				break

		kickMod = punchMod + 2
		for adKey in ('claws', 'iron-hand'):
			if adKey in self.mData['advantages']:
				punchMod += 2
				break

		# final recalc. for punch damage
		punchDice = thrustDice
		while punchMod > 2:
			punchMod -= 4
			punchDice  += 1
		# final recalc. for kick damage
		kickDice = thrustDice
		while kickMod > 2:
			kickMod -= 4
			kickDice  += 1

		if punchMod >= 0: punchMod = '+' + str(punchMod)
		if kickMod >= 0: kickMod = '+' + str(kickMod)
		if knifeMod >= 0: knifeMod = '+' + str(knifeMod)
		if knifeSwing >= 0: knifeSwing = '+' + str(knifeSwing)

		return ((thrustDice, thrustMod), (swingDice,  swingMod), (punchDice, punchMod), (kickDice, kickMod), (thrustDice, knifeMod), (swingDice, knifeSwing))


	def getDefenses(self):
		'return (dodge, parry (unarmed and armed), block, base-dr)'
		# dodge
		dodge = int(self.mData['attributes']['speed']) + 3
		for advantage in NPCConstants.kEnhancedDodgeModifiers:
			if advantage in self.mData['advantages']:
				if self.mData['advantages'][advantage]:
					dodge += self.mData['advantages'][advantage]
				else:
					dodge += 1
		if dodge > 14:
			dodge = 14

		# parry ... find the best unarmed parry
		ratio = 0.5
		parrySkill = 'dexterity'
		skillMod = int(self.mData['stats'][parrySkill])
		parry = int(skillMod * ratio) + 1
		parrySkillValue = skillMod

		for skill in NPCConstants.kUnarmedParrySkills:
			if skill in self.mData['skills']:
				testSkillValue = int(self.mData['skills'][skill])
				testparry = int(testSkillValue * ratio) + 3
				if (testparry > parry) or ((testparry == parry) and (testSkillValue > parrySkillValue)):
					parry = testparry
					parrySkillValue = testSkillValue
					parrySkill = skill

		if (parrySkill == 'dexterity'): parry += 2

		# parry ... find the best weapon parry
		weaponSkill = ''
		weaponParry = weaponSkillValue = 0
		wSkillCache = Set()
		if 'weapons' in self.mData:
			weapons = self.mData['weapons'].keys()
			weapons.sort()
			for weapon in weapons:
				if (('skills' in self.mData['weapons'][weapon]) and \
				    ('damage' in self.mData['weapons'][weapon]) and \
				    ('ammo' not in self.mData['weapons'][weapon])):
					for skill in self.mData['weapons'][weapon]['skills'].split(', '):
						# note that we are using wSkillCache as cache here so we don't check same skill twice
						if (skill not in wSkillCache) and (skill[:3] != 'gun') and (skill[-4:] != 'wing'):
							wSkillCache.add(skill)
							if skill in self.mData['skills']:
								parryMult = 0.5; wparryBonus = 0
								if skill in ('staff',):
									wparryBonus = 2
								elif skill in ('whip', 'flail', 'kusari'):
									wparryBonus = -2
								testSkillValue = int(self.mData['skills'][skill])
								testValue = int(testSkillValue * parryMult) + 3 + wparryBonus
								if (skill == 'knife') and ('mangosh' not in self.mData['weapons']):
									testValue -= 1
								if (testValue > weaponParry) or ((testValue == weaponParry) and (testSkillValue > weaponSkillValue)):
									#print 'choosing %s parry: %s, because it is better than %s parry: %s' % \
									#		(skill, testValue, weaponSkill, weaponParry)
									weaponParry = testValue
									weaponSkillValue = testSkillValue
									weaponSkill = skill
		#print

		# mod the parryBonus for any appopriate advantages
		parryBonus = 0
		for advantage in NPCConstants.kEnhancedParryModifiers:
			if advantage in self.mData['advantages']:
				if self.mData['advantages'][advantage]:
					parryBonus += self.mData['advantages'][advantage]
				else:
					parryBonus += 1

		# block
		block = 0
		if 'shield' in self.mData['skills']:
			block = (self.mData['skills']['shield'] / 2) + 3
			for advantage in NPCConstants.kEnhancedBlockModifiers:
				if advantage in self.mData['advantages']:
					if self.mData['advantages'][advantage]:
						block += self.mData['advantages'][advantage]
					else:
						block += 1

		# base-dr
		baseDr = 0
		for advantage in NPCConstants.kBaseDRModifiers:
			if advantage in self.mData['advantages']:
				if self.mData['advantages'][advantage]:
					baseDr += self.mData['advantages'][advantage]
				else:
					baseDr += 1

		# format and return
		parry += parryBonus
		if weaponSkill:
			weaponParry += parryBonus
			parryText = '(%s)%s (%s)%s' % (weaponSkill, weaponParry, parrySkill, parry)
		else:
			parryText = '(%s)%s' % (parrySkill, parry)
		return (dodge, parryText, block, baseDr)


	def getAppearance(self, statDict, gender='male'):
		'return (height, weight, eyes, hair, age) in formatted string tuple'
		# set st
		st = self.getStat(['strength'], statDict)
		if st not in NPCConstants.kStrengthHeightMap:
			if st < 5:
				st = 5
			else:
				st = 26

		# get base height from st and randomize
		height			= NPCConstants.kStrengthHeightMap[st] + randint(-2, 2) + randint(-2, 2)
		if gender == 'female':
			height		-= 4

		# alter height for appropriate disads
		for disad, modifier in (('dwarfism', kDwarfismModifier), ('gigantism', kGigantismModifier)):
			if disad in statDict['disadvantages']:
				height *= modifier

		# now we can get weight based on height
		weight			= NPCConstants.kHeightWeightMap[int(height)] + randint(-8, 10) + randint(-6, 8) + randint(-4, 4)

		# alter weight/ height(stocky) for appropriate disads
		if statDict['disadvantages'].has_key('stocky'):
			if statDict['disadvantages']['stocky'] == -2:
				height		*= .76
				weight		*= 1.20
			else:
				height		*= .95
				weight		*= 1.10
		if statDict['disadvantages'].has_key('overweight'):
			weight		*= 1.30
		elif statDict['disadvantages'].has_key('fat'):
			if statDict['disadvantages']['fat'] == -1:
				weight	*= 1.50
			elif statDict['disadvantages']['fat'] == -2:
				weight	*= 2.0
		elif statDict['disadvantages'].has_key('skinny'):
			weight		*= (2/3.0)

		# put height and weight in pretty format
		feet, inches	= divmod(height, 12)
		height			= '%s\'%s"' % (int(feet), int(inches))
		weight			= '%slbs.' % int(weight)

		# determine hair/eyes
		if self.mData['hair']:
			hair = choice(self.mData['hair'])
		else:
			hair			= choice(NPCConstants.kHairColours)
		if self.mData['eyes']:
			eyes = choice(self.mData['eyes'])
		else:
			eyes			= choice(NPCConstants.kEyeColours)

		# figure age
		age = self.getRandomDistribValue(self.mData['age'])
		if statDict['disadvantages'].has_key('youth'):
			age = 18 + statDict['disadvantages']['youth']
		for rkey in ('rank', 'paramilitary-rank'):
			if (rkey in statDict['advantages']) and statDict['advantages'][rkey]:
				age += int(round((statDict['advantages'][rkey] ** 1.4) - 1))
		if (age > 68) and not \
		   (('longevity' in statDict['advantages']) or ('unliving' in statDict['disadvantages'])):
			age = self.getRandomDistribValue('18~24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40~60')

		# check bald / white gray
		if (hair in NPCConstants.kOldHair) and (age < 38):
			# just get hair again -- i guess it's possible for young person to have gray hair
			hair = choice(NPCConstants.kHairColours)
		if (hair in NPCConstants.kBaldHair) and \
		   ((gender == 'female') or (age < 30)):
			hair = 'brown'

		if 'albinism' in statDict['disadvantages']:
			hair = choice(('white', 'white-gray', 'white-blonde'))
			eyes = choice (('pink', 'white-blue', 'gray', 'yellow', 'light-blue'))

		result = []
		for trait in (height, weight, eyes, hair, age):
			if type(trait) == type(''): result.append(unicode(trait, 'utf-8'))
			else: result.append(trait)
		return tuple(result)


	def getRandomDistribValue(self, rawValue):
		'get a value from a string formatted for random distribution: e.g., 1~3,4~6'
		valueSet	= rawValue.split('~')
		if len(valueSet) == 3:
			min, mode, max = valueSet
			value = getModeValue(int(min), int(max), mode)
		elif len(valueSet) == 2:
			min, max = valueSet
			value = randint(int(min), int(max))
		elif len(valueSet) > 3:
			# treat as arbitrary list
			min = 1
			max = 3
			if valueSet[0].startswith('choose:'):
				bounds = [int(val) for val in valueSet[0][7:].split('-')]
				if len(bounds) == 2:
					min, max = bounds
				else:
					min = max = bounds[0]
				valueSet = valueSet[1:]
			shuffle(valueSet)
			value = ''
			values = randint(min, max)
			for i in range(values):
				if not value:
					value = ' ' + valueSet[i]
				else:
					value += ', ' + valueSet[i]
		else:
			value = valueSet[0]
		return value


	def getStatValue(self, attribmap, attribKey, key, statDict):
		'get the stat-added value for this npc attrib'
		attribTypeMap = attribmap[attribKey]
		value = 0
		if (key in attribTypeMap):
			#  and ('stat' in attribTypeMap[key]) .... can we just assume that only attribTypeMaps with 'stat' key are passed in??? 
			statList = attribTypeMap[key]['stat'].split(', ')
			value = self.getStat(statList, statDict, attribmap)
		return value


	def getStat(self, statList, statDict, attribmap={}):
		'get the statDict stat value -- recurse if dex/health'
		statVal = 0
		for stat in statList:
			if stat == 'dex/health':
				statVal = ((self.getStat(['dexterity'], statDict, attribmap) + self.getStat(['health'], statDict, attribmap)) / 4.0)
			elif stat == 'guns-int':
				statVal = self.getStat(['intelligence'], statDict, attribmap) - 10
				if statVal < 0:
					statVal = 0
				elif statVal > 2:
					statVal = 2
			elif stat == 'karate-parry':
				statVal = int(self.getStat(['karate'], statDict, attribmap) * (2/3.0))

			else:
				#if stat == 'animal-empathy': print attribmap['animal-empathy']
				for attribDictKey in ('stats', 'advantages', 'disadvantages', 'skills'):
					found = 0
					if statDict.has_key(attribDictKey):
						statAttribDict = statDict[attribDictKey]
						#if (attribDictKey is not 'skills') and attribDict.has_key(stat) and \
						if statAttribDict.has_key(stat) and \
						   (attribmap and attribmap['skill-mods'].has_key(stat) and attribmap['skill-mods'][stat].has_key('skill-mod')):
							# this must be skill-mod type
							skillMod = attribmap['skill-mods'][stat]['skill-mod']
							#if ('karate-parry' in statList): print skillMod, '!!'
							if skillMod == 'val':
								statVal += int(statAttribDict[stat])
							elif skillMod[:4] == 'mult':
								statVal += (int(skillMod[5:]) * int(statAttribDict[stat]))
							else:
								statVal += int(skillMod)
							found = 1; break
						elif statAttribDict.has_key(stat):
							#if attribDictKey != 'stats': print stat, 'implicit VAL found'
							statVal += statAttribDict[stat]
							found = 1; break

				#if not found:
				#	print stat, 'NOT found AT ALL!!!!!!!!'

		return statVal


	def getPowerAdjustedValue(self, classData, attribKey, key, power, origValue):
		value = int(round(origValue * power))
		# what if power > 1.25 and newValue still == 1??? 
		if (value in (0, 1)) and (power > 1.15) and (randint(1, 80) < int(power * 40)):
			if (value == 1) or ('max' in classData[attribKey][key]):
				value += 1
		if (attribKey == 'advantages') and ('max' in classData[attribKey][key]) and (value > int(classData[attribKey][key]['max'])):
			value = int(classData[attribKey][key]['max'])
		return value


	def addPreRequisite(self, classData, statDict, attribmap, power, key, topParentKey):
		'add preReq skill or advantage to self.mData'
		preReqSet = attribmap['skills'][key]['prereqs']

		for preReq in preReqSet.split(', '):
			optionalAdv		= ''
			preReqName		= preReq
			value			= 0
			minValue		= 0
			nameValueSet	= preReq.split(':')

			if len(nameValueSet) == 2:
				preReqName	= nameValueSet[0]
				valueSet	= nameValueSet[1].split('~')
				if preReqName.count('|'):
					optionSet = preReqName.split('|')
					optionalAdv, preReqName = optionSet

				if len(valueSet) == 3:
					min, mode, max = valueSet
					value = getModeValue(int(min), int(max), mode)
				elif len(valueSet) == 2:
					min, max = valueSet
					value = randint(int(min), int(max))
				elif valueSet[0][0] == '*':
					# min. value required for this preReq
					if preReqName[:5] == 'spell':
						#value = randint(-2, 1)
						value = RandDistribInt(min=-2, mean=-2, max=0, mode=-1)
					else:
						value = randint(-3, 2)
					minValue = int(valueSet[0][1:])
				else:
					# get static value
					value = int(valueSet[0])

			if optionalAdv and self.mData['advantages'].has_key(optionalAdv):
				# self.mData has an optional advantage, this is sufficient
				npcDataKey = 'advantages'
				minValue = 0
				preReqName = optionalAdv

			elif (preReqName in attribmap['skills']):
				# this preReq is a skill
				npcDataKey = 'skills'
				if (preReqName not in self.mData[npcDataKey]):
					self.addSkillCost(attribmap, preReqName, value)
				# go recursive for this preReq skill...
				if 'prereqs' in attribmap['skills'][preReqName]:
					self.addPreRequisite(classData, statDict, attribmap, power, preReqName, topParentKey)
				# increment value for stat values and verify minValue...
				value += self.getStatValue(attribmap, 'skills', preReqName, statDict)
			else:
				# ok, this preReq is an advantage  apply self.mData['power']
				npcDataKey = 'advantages'
				if power != 1.0:
					newValue = self.getPowerAdjustedValue(classData, npcDataKey, preReqName, power, value)
					if newValue != value:
						value = newValue

			# verify minValue
			if minValue     and (value < minValue):
				value		= minValue
			minValueReset	= minValue and self.mData[npcDataKey].has_key(preReqName) and (int(self.mData[npcDataKey][preReqName]) < minValue)

			# check to see if this preReq change affects existing skills
			if npcDataKey == 'advantages' and minValueReset:
				for skillName in self.mData['skills'].keys():
					localStatList = attribmap['skills'][skillName]['stat'].split(', ')
					if localStatList.count(preReqName):
						backValue = value
						# compute backValue delta
						backValue -= int(self.mData[npcDataKey][preReqName])
						self.mData['skills'][skillName] += backValue

			# finally,  add this preReq ...
			if (not self.mData[npcDataKey].has_key(preReqName)) or minValueReset:
				self.mData[npcDataKey][preReqName] = value


	def getNPCImage(self, gender, classData, statDict):
		imageKey = 'images-%s' % gender
		image = choice(classData['class'][imageKey].split(','))
		# override
		if 'dwarfism' in statDict['disadvantages']:
			image = '%s-short.gif' % gender
		return image


	def DisplayRaw(npc, mode=0):
		'print raw self.mData data'
		pp = PrettyPrinter(indent=4)
		pp.pprint(npc.mData)
	DisplayRaw = staticmethod(DisplayRaw)



	def DisplayText(npc, mode=0):
		if mode == 1:
			encoding = 'UTF-8'
		elif mode == 2:
			encoding = 'mac_latin2'
		else:
			encoding = 'cp437'
		print NPCBuilder.GetTextDisplay(npc, mode)
	DisplayText = staticmethod(DisplayText)

	def DisplayHTML(npc, mode=0):
		if mode == 1:
			encoding = 'UTF-8'
		elif mode == 2:
			encoding = 'mac_latin2'
		else:
			encoding = 'cp437'
		print NPCBuilder.GetHTMLDisplay(npc, mode)
	DisplayHTML = staticmethod(DisplayHTML)


	def GetTextDisplay(npc, mode):
		'print self.mData string; mode=0 is default, which is to DOS screen'
		doubleLine = u'_' * (kPageWidth)
		font = bold = color = blue = mblue = red = ''
		if mode:
			font = '[wxFont:Arial]'
			bold = '[wxBold:1]'
			blue = '[wxColor:BLUE]'
			mblue = '[wxColor:MEDIUM BLUE]'
			red  = '[wxColor:RED]'
		if npc.mData['number'] > 1:
			npcString = u'%s%s%s' % (bold, doubleLine, bold)
		else:
			npcString = u''
		npcString += '\n%s%s%s\t\t%s points\n\n' % (font, npc.getTitle(), font, npc.getTotalPoints())

		# populate keySets
		adKeys = npc.mData['advantages'].keys()
		adKeys.sort()
		disadKeys = npc.mData['disadvantages'].keys()
		disadKeys.sort()
		quirkKeys = [quirk for quirk in npc.mData['quirks']]
		keySets = (adKeys, disadKeys, quirkKeys)

		bestLength = 0
		for listSet in keySets:
			if bestLength < len(listSet):
				bestLength = len(listSet)
		for listSet in keySets:
			while bestLength > len(listSet):
				listSet.append(' ')

		# loop thru each attribKey and set in specific order
		odd = 0
		origkeys = []
		for attribKey in NPCConstants.kDisplayAttribKeyOrder:
			if odd:
				npcString += '\n'; odd = 0

			if attribKey == 'advantages/disadvantages/quirks':
				origkeys = [listItem for listSet in keySets for listItem in listSet]				
			elif attribKey == 'stats/secondary stats':
				origkeys = NPCConstants.kStatAttribKeyOrder
			elif attribKey == 'appearance':
				origkeys = NPCConstants.kAppearanceKeyOrder
			else:
				origkeys = npc.mData[attribKey].keys()
				origkeys.sort()

			# column-sort the keys...
			keys = []
			position = 0
			# make origkeys Even
			lenDivisor = 2
			if attribKey in NPCConstants.kThreeColumnHeadings:
				lenDivisor = 3
			if (len(origkeys) % lenDivisor) and attribKey not in NPCConstants.kItemAttributeKeys:
				origkeys.append('')
				if (len(origkeys) % lenDivisor):
					origkeys.append('')
			while position < (len(origkeys) / lenDivisor):
				keys.append(origkeys[position])
				keys.append(origkeys[(len(origkeys) / lenDivisor) + position])
				if lenDivisor == 3:
					keys.append(origkeys[(2 * (len(origkeys) / lenDivisor)) + position])
				position += 1

			# print spacer/set Title-headers
			if keys or origkeys:
				dashCharacter = '_'
				if attribKey in NPCConstants.kNonSkillAttributeKeys:
					titles = attribKey.split('/')
					firstSpacer		= dashCharacter * ((kPageWidth / 3) - (len(titles[0]) + 1))
					secondSpacer	= dashCharacter * (kPageWidth - (len(titles[1]) + 2 + (kPageWidth / 3)))
					if len(titles) == 3:
						secondSpacer	= dashCharacter * (kPageWidth - (len(titles[1]) + 2 + int(kPageWidth * (2/3.0))))
						thirdSpacer	    = dashCharacter * (kPageWidth - (len(titles[2]) + 3 + int(kPageWidth * (2/3.0))))
						heading = '%s %s %s %s %s %s' % (titles[0].upper(), firstSpacer, titles[1].upper(), secondSpacer, titles[2].upper(), thirdSpacer)
					else:
						heading = '%s %s %s %s' % (titles[0].upper(), firstSpacer, titles[1].upper(), secondSpacer)
					npcString += '\n%s%s%s\n' % (bold, heading, bold)
				elif attribKey != 'appearance':
					if attribKey == 'weapons':
						keyTitle = 'Weapons/Equipment'
					else:
						keyTitle = attribKey.upper()
					heading = keyTitle + ' ' + dashCharacter * (kPageWidth - (len(keyTitle) + 1))
					npcString += '\n%s%s%s\n' % (bold, heading, bold)

			if attribKey == 'combat':
				# encumbrance
				npcString +=  '-] encumbrance: %s [%s](%slbs)  carrying %slbs (%slbs armor, %slbs weapons)' % \
								(npc.mData['encumbrance']['level'][1], npc.mData['encumbrance']['level'][0], npc.mData['encumbrance']['maxWeight'],
								 npc.mData['encumbrance']['total'],    npc.mData['encumbrance']['armor'],    npc.mData['encumbrance']['weapons'])
				# defenses
				encDodge = npc.mData[attribKey]['dodge'] - npc.mData['encumbrance']['level'][0]
				npcString +=  '\n-] dodge [enc]: %s[%s]   block: %s  parry: %s   base-dr: %s' % \
				                (npc.mData[attribKey]['dodge'], encDodge, npc.mData[attribKey]['block'], npc.mData[attribKey]['parry'], npc.mData[attribKey]['baseDr'])
				# damage
				npcString +=  '\n-> damage: th %sd%s,' % npc.mData[attribKey]['baseDamTh']
				npcString +=  ' sw %sd%s' % npc.mData[attribKey]['baseDamSw']
				npcString +=  '      punch:  th %sd%s ' % npc.mData[attribKey]['punchDam']
				npcString +=  ' kick: th %sd%s \n' % npc.mData[attribKey]['kickDam']

			elif attribKey in NPCConstants.kItemAttributeKeys:
				# print item(s) stats, uniqueness
				for key in origkeys:
					item = npc.mData[attribKey][key]
					for wattrib in NPCConstants.kItemKeyOrder:
						if item.has_key(wattrib):
							value = item[wattrib]

							if wattrib == 'name':
								wName = item[wattrib]
								npcString +=  '%-26s' % wName
							elif wattrib in NPCConstants.kNewLineModItemKeys:
								if wattrib == '*mod0':
									init = '*'
								else:
									init = ' '
								npcString +=  '\n\t\t\t\t  %s%s' %  (init, value)
							elif wattrib[:4] == '*mod':
								npcString +=  ', %s' %  (value,)
							elif wattrib == 'damage':
								# calculate final damage for this npc's hand or range weapon with 'damage'
								special = ''
								color   = ''
								thrustBonus, swingBonus = value.split(',')
								if thrustBonus != '-':
									thrustBonus				= int(thrustBonus) + int(npc.mData['combat']['baseDamTh'][1])
								if swingBonus[-1] == '*':
														swingBonus = swingBonus[:-1]
														special = '*'
								if swingBonus != '-':
									swingBonus				= int(swingBonus)  + int(npc.mData['combat']['baseDamSw'][1])

								thrustDice				= int(npc.mData['combat']['baseDamTh'][0])
								while (thrustBonus != '-') and (thrustBonus > 2):
														thrustBonus -= 4
														thrustDice  += 1
								swingDice				= int(npc.mData['combat']['baseDamSw'][0])
								while (swingBonus != '-') and (swingBonus > 2):
														swingBonus -= 4
														swingDice  += 1

								if (thrustBonus != '-') and (thrustBonus >= 0):		thrustBonus = '+' + str(thrustBonus)
								if (swingBonus != '-')  and (swingBonus >= 0):		swingBonus = '+' + str(swingBonus)

								if thrustBonus == '-':
									thrustDice = ''
								else:
									thrustDice = '%sd' %  thrustDice
								if swingBonus == '-':
									swingDice = ''
								else:
									swingDice = '%sd' %  swingDice
								if mode and special:
									special = ''
									color = blue
								else:
									color = ''
								npcString += ' damage: th %s%s%s%s%s, sw %s%s%s%s%s ' %  (
																							color,
																							thrustDice,
																							thrustBonus,
																							special,
																							color,
																							color,
																							swingDice,
																							swingBonus,
																							special,
																							color
																						 )
							elif mode and (wattrib == 'st') and (GetInt(value) > npc.mData['stats']['strength']):
								npcString += ' %s:%s%s%s' % (wattrib, red, GetInt(value), red)
							else:
								if mode and value and (type(value) == type('')) and (value[-1] == '*'):
									value = value[:-1]
									color = blue
								else:
									color = ''
								npcString += ' %s:%s%s%s' % (wattrib, color, value, color)
					npcString += '\n'

			else:
				for key in keys:
					# set value
					value = ''
					if attribKey == 'advantages/disadvantages/quirks':
						if key in npc.mData['advantages']:
							value = npc.mData['advantages'][key]
						elif key in npc.mData['disadvantages']:
							value = abs(npc.mData['disadvantages'][key])
					elif attribKey == 'stats/secondary stats':
						if key in npc.mData['stats']:
							value = npc.mData['stats'][key]
						elif key in npc.mData['attributes']:
							value = npc.mData['attributes'][key]
					elif key in npc.mData[attribKey]:
						value = npc.mData[attribKey][key]

					# format for printing
					if not value:
						# HACK (for value: 0)
						if key in NPCConstants.kZeroValueAttributes:
							screenValue = key + ': 0'
						else:
							screenValue = key
					elif key == 'move':
						screenValue = '%s [enc]: %s[%s]' % (key, int(value), int(value - npc.mData['encumbrance']['level'][0]))
					elif mode and key in NPCConstants.kMainStatKeys:
						screenValue = '%s%s%s: %s' % (mblue, key.capitalize(), mblue, value)
					else:
						screenValue = key + ': ' + str(value)

					if attribKey in NPCConstants.kThreeColumnHeadings:
						if odd != 2:
							if mode and mblue in screenValue:
								npcString +=  '%-72s' % screenValue; odd += 1
							else:
								npcString +=  '%-30s' % screenValue; odd += 1
						else:
							npcString +=  screenValue + '\n'; odd = 0
					elif not odd:
						npcString +=  '%-30s' % screenValue; odd = 1
					else:
						npcString +=  screenValue + '\n'; odd = 0

		# screen encoding for cmd-prompt is "cp437", else "ISO-8859-1" 
		return npcString
	GetTextDisplay = staticmethod(GetTextDisplay)


	def GetWeaponDamage(npc, weapon, damageKey='damage'):
		# calculate final damage for this npc's hand or range weapon with 'damage'
		special = ''
		thrustBonus, swingBonus = weapon[damageKey].split(',')

		if thrustBonus != '-':
			thrustBonus				= int(thrustBonus) + int(npc.mData['combat']['baseDamTh'][1])
		if swingBonus[-1] == '*':
								swingBonus = swingBonus[:-1]
								special = '*'
		if swingBonus != '-':
			swingBonus				= int(swingBonus)  + int(npc.mData['combat']['baseDamSw'][1])

		thrustDice				= int(npc.mData['combat']['baseDamTh'][0])
		while (thrustBonus != '-') and (thrustBonus > 2):
								thrustBonus -= 4
								thrustDice  += 1
		swingDice				= int(npc.mData['combat']['baseDamSw'][0])
		while (swingBonus != '-') and (swingBonus > 2):
								swingBonus -= 4
								swingDice  += 1

		if (thrustBonus != '-') and (thrustBonus >= 0):		thrustBonus = '+' + str(thrustBonus)
		if (swingBonus != '-')  and (swingBonus >= 0):		swingBonus = '+' + str(swingBonus)

		if thrustBonus == '-':
			thrustDice = ''
		else:
			thrustDice = '%sd' %  thrustDice
		if swingBonus == '-':
			swingDice = ''
		else:
			swingDice = '%sd' %  swingDice

		# thrust
		thrustDamage = ''
		if (thrustBonus == '-') and ('rch' in weapon):
			thrustDamage = '-'
		elif (thrustBonus != '-'):
			thrustDamage = '%s%s%s' % (thrustDice, thrustBonus, special)
		# swing
		swingDamage = ''
		if (swingBonus == '-') and ('rch' in weapon):
			swingDamage = '-'
		elif (swingBonus != '-'):
			swingDamage = '%s%s%s' % (swingDice, swingBonus, special)

		# finalDamage
		finalDamage = ''
		if thrustDamage and swingDamage:
			finalDamage = '%s, %s' % (thrustDamage, swingDamage)
		elif thrustDamage:
			finalDamage = thrustDamage
		elif swingDamage:
			finalDamage = swingDamage

		return finalDamage
	GetWeaponDamage = staticmethod(GetWeaponDamage)


	def getBuilderTotalPoints(self):
		return self.totalPoints


	def GetHTMLDisplay(npc, useWebKitPaths=1, prependHTMLHeaders=0):
		# setup the locals we want in localDict
		if useWebKitPaths: localDict = {'PRC':'%', 'npc':''}
		else: localDict = {'PRC':'%', 'npc':npc.getDBData(0)} #---ode(npc.getDBData(), 'latin1')} # b2a_qp(
		localDict.update(NPCBuilder.GetAppearanceDict(npc))
		localDict.update(NPCBuilder.GetStatsAndAttributesDict(npc))
		localDict.update(NPCBuilder.GetSkillsDict(npc))
		advantages, ad = NPCBuilder.GetAdvantagesDict(npc)
		localDict.update(ad)
		disadvantages, disad = NPCBuilder.GetDisAdvantagesDict(npc)
		localDict.update(disad)
		armorTotal, adict = NPCBuilder.GetArmorDict(npc)
		localDict.update(adict)
		weaponTotal, wdict = NPCBuilder.GetWeaponsDict(npc)
		localDict.update(wdict)
		localDict.update(NPCBuilder.GetDamageSetsDict(npc))
		localDict.update(NPCBuilder.GetEncumbranceDict(npc))
		localDict.update(NPCBuilder.GetDefensesDict(npc))
		localDict.update(NPCBuilder.GetPointsDict(npc))
		# now do padding
		localDict.update(NPCBuilder.GetPaddingDict(npc, armorTotal, weaponTotal, advantages, disadvantages))

		# setup path and header specific stuff...
		rootDir = GetRootDir()
		if prependHTMLHeaders:
			styleLink = NPCConstants.kNPCGenCSSLink % rootDir
			if useWebKitPaths:
				styleLink = NPCConstants.kNPCGenCSSLink % 'http://ayinger.no-ip.info/images'
			templateString = unicode('', 'utf-8')
			# <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"/> <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">
			templateString += '%s\n<html>\n<head>\n  %s\n  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=UTF-8">\n</head>\n' % (NPCConstants.kNPCGenHTMLDocType, styleLink)
			localDict['webLink'] = unicode('<A HREF="http://ayinger.no-ip.info/NPCGen/NPCGen"><B>NPCGen online</B></A>', 'utf-8')
		else:
			templateString = unicode('', 'utf-8')
			localDict['webLink'] = unicode('<A HREF="http://ayinger.no-ip.info/NPCGen/NPCGenEmail"><B>Email this NPC</B></A>', 'utf-8')

		if useWebKitPaths:
			templateRootPath = './html/'
			localDict['imagePath'] ='"http://ayinger.no-ip.info/images/pics/%s"' % npc.getImage()
			localDict['gimagePath'] = unicode('"http://ayinger.no-ip.info/images/GURPS.gif"', 'utf-8')
		else:
			templateRootPath = rootDir + os.sep
			localDict['imagePath'] = '"' + rootDir + os.sep + 'images/pics' + os.sep + npc.getImage() + '"'
			localDict['gimagePath'] = unicode('"' + rootDir + os.sep + 'images' + os.sep + 'GURPS.gif' + '"', 'utf-8')

		# load string from template and replace namefile = codecs.open(fullPath, 'rU', 'utf-8')
		templateFile = codecs.open(templateRootPath + kNPCTemplateFileName, 'rU', 'utf-8')
		#print loads(decompress(localDict['npc'].encode('latin1')))
		templateString += templateFile.read()
		if templateString.startswith(kuBOM): templateString = templateString.lstrip(kuBOM)
		templateFile.close()
		#result = templateString % localDict
		#localDict['npc'] = ''
		#for key in localDict:
		#	#print type(key), type(localDict[key])
		#	if type(localDict[key]) == type(''): print 'key %s value %s is only string' % (key, localDict[key])
		result = templateString % localDict
		#print type(result)
		return result
	GetHTMLDisplay = staticmethod(GetHTMLDisplay)


	def GetPointsDict(npc):
		#advantageCosts = int(self.attribCostSet[0] + self.advantageCosts)
		#disadvantageCosts = int(self.attribCostSet[1] + self.disadvantageCosts)
		localDict = {'adPoints': int(npc.getCost('advantages')), 'disadPoints': int(npc.getCost('disadvantages')), 'totalPoints': npc.getCost(),
					 'statPoints': int(npc.getCost('stats')), 'skillPoints': npc.getCost('skills')}
		return localDict
	GetPointsDict = staticmethod(GetPointsDict)


	def GetDefensesDict(npc):
		dodge, parryText, block, baseDr = npc.getDefenses()
		parryTexts = parryText.split(' ')
		if len(parryTexts) == 1:
			weaponSkill = 'weapon'
			weaponParry = '0'
			parrySkill, parry        = parryTexts[0][1:].split(')')
		else:
			weaponSkill, weaponParry = parryTexts[0][1:].split(')')
			parrySkill, parry        = parryTexts[1][1:].split(')')
		localDict = {'dodge': dodge, 'weaponSkill': weaponSkill, 'weaponParry': weaponParry, 'parrySkill': parrySkill, 'parry': parry, 'block': block}
		localDict['encDodge'] = localDict['dodge'] - npc.mData['encumbrance']['level'][0]
		return localDict
	GetDefensesDict = staticmethod(GetDefensesDict)


	def GetDamageSetsDict(npc):
		thrustSet, swingSet, punchSet, kickSet, knifeth, knifesw = npc.getDamageSets()
		thrustDice, thrustMod = thrustSet
		swingDice, swingMod = swingSet
		punchDice, punchMod = punchSet
		kickDice, kickMod = kickSet
		damageSetString = unicode('Thrust: %sd%s<BR>Swing: %sd%s<BR><BR>Punch: %sd%s<BR>Kick: %sd%s', 'utf-8') % \
							(thrustDice, thrustMod, swingDice, swingMod, punchDice, punchMod, kickDice, kickMod)
		return {'DAMAGESET': damageSetString}
	GetDamageSetsDict = staticmethod(GetDamageSetsDict)


	def GetEncumbranceDict(npc):
		encumbranceString = '<CENTER><I><B>%s [%s]</B></I></CENTER><BR>(up to %slbs)<BR>&nbsp;&nbsp;%slbs armor<BR>&nbsp;&nbsp;%slbs wpns &amp; eqp<BR>&nbsp;&nbsp;%slbs total' % \
							(npc.mData['encumbrance']['level'][1], npc.mData['encumbrance']['level'][0], npc.mData['encumbrance']['maxWeight'],
							 npc.mData['encumbrance']['armor'],    npc.mData['encumbrance']['weapons'],  npc.mData['encumbrance']['total'])
		return {'ENCUMBRANCE': encumbranceString}
	GetEncumbranceDict = staticmethod(GetEncumbranceDict)


	def GetPaddingDict(npc, armorTotal, weaponTotal, advantages, disadvantages):
		equipPadding = ''
		weaponPadding = ''
		if armorTotal > weaponTotal:
			weaponPadding = '''
							<TR><TD WIDTH=40%>&nbsp;</TD> <TD><CENTER>&nbsp;</CENTER></TD> 
								<TD>&nbsp;</TD> <TD><CENTER>&nbsp;</CENTER></TD> <TD>&nbsp;</TD> <TD><CENTER>&nbsp;</CENTER></TD> <TD>&nbsp;</TD> <TD>&nbsp;</TD> </TR>'''
			weaponPadding *= int(armorTotal - weaponTotal)
		elif armorTotal < weaponTotal:
			equipPadding = '''
							<TR><TD WIDTH=40%>&nbsp;</TD> <TD>&nbsp;</TD> <TD><CENTER>&nbsp;</CENTER></TD> <TD>&nbsp;</TD></TR>'''
			equipPadding *= int(weaponTotal - armorTotal)

		# ad/disads		
		adPadding = ''
		disadPadding = ''
		if advantages > disadvantages:
			disadPadding = '''
				<TR><TD WIDTH=90% ALIGN=LEFT>&nbsp;</TD><TD WIDTH=10&nbsp; ALIGN=RIGHT>&nbsp;</TD></TR>'''
			disadPadding *= int(advantages - disadvantages)
		elif advantages < disadvantages:
			adPadding = '''
				<TR><TD WIDTH=90% ALIGN=LEFT>&nbsp;</TD><TD WIDTH=10&nbsp; ALIGN=RIGHT>&nbsp;</TD></TR>'''
			adPadding *= int(disadvantages - advantages)
		
		return {'EPADDING': equipPadding, 'WPADDING': weaponPadding, 'ADPADDING': adPadding, 'DISADPADDING': disadPadding}
	GetPaddingDict = staticmethod(GetPaddingDict)


	def GetArmorDict(npc):
		# equipment: gernades, power stones, potions, posions ... (quantity, doses, note st, dam)
		total = 0
		armorStatKeys = ('name', 'pd', 'dr', 'area', 'weight')
		armorString = ''
		armors = npc.mData['armor'].items()
		armors.sort()
		for armorKey, armor in armors:
			armor = armor.copy()
			if '*mod0' in armor:
				armor['name'] = "<FONT COLOR=#3333CC>%s</FONT>" % armor['name']
			else:
				armor['name'] = armor['name']
			total += 1
			armorString += '''
						<TR><TD WIDTH=45%s>%s</TD> <TD><CENTER>%s</CENTER></TD> <TD><CENTER>%s</CENTER></TD> <TD>%s</TD> <TD>%s</TD></TR>''' % \
						('%', armor['name'], armor['db'], armor['dr'], armor['area'], armor['weight'])
			if '*mod0' in armor:
				armorModString = ''
				for n in range(10):
					modKey = '*mod%d' % n
					if modKey in armor:
						total += 1
						#armorModString += '%s, ' % armor[modKey]
						armorModString += '%s<BR>&nbsp;&nbsp;&nbsp;' % armor[modKey]
				armorString += '''
						<TR><TD WIDTH=45%s>&nbsp;&nbsp;&nbsp;<FONT COLOR=#3333CC>%s</FONT></TD><TD></TD><TD></TD><TD></TD><TD></TD></TR>''' % ('%', armorModString[:-22])

		equipString = ''
		equip = npc.mData['weapons'].items()
		equip.sort()
		for equipKey, equipment in equip:
			equipment = equipment.copy()
			# little hack for determining equipment from weapons
			if ('ss' not in equipment) and ('rch' not in equipment):
				total += 1
				if 'quantity' not in equipment:
					equipment['quantity'] = '1'
				equipment['wt'] = '%slbs' % (float(equipment['wt'][:equipment['wt'].find('lbs')]) * int(equipment['quantity']))
				# format 'note'
				if 'doses' in equipment:
					equipment['dnote'] = equipment['doses']
				for key in ('st', 'dam'):
					if key in equipment:
						equipment['knote'] = '%s: %s' % (key, equipment[key])
						break
				if ('note' in equipment) and ('knote' in equipment):
					equipment['note'] = '%s, %s' % (equipment['knote'], equipment['note'])
				elif 'knote' in equipment:
					equipment['note'] = equipment['knote']
				elif 'dnote' in equipment:
					equipment['note'] = equipment['dnote']
				elif ('note' not in equipment):
					equipment['note'] = ''
				equipString += '''
							<TR><TD WIDTH=40%s>%s</TD> <TD>%s</TD> <TD><CENTER>%s</CENTER></TD> <TD>%s</TD></TR>''' % \
						('%', equipment['name'], equipment['note'], equipment['quantity'], equipment['wt'])

		return (total, {'ARMOR': armorString, 'EQUIPMENT': equipString})
	GetArmorDict = staticmethod(GetArmorDict)

						
	def GetWeaponsDict(npc):
		total = 0
		rangedWeaponStatKeys = ('name', 'rel', 'dam', 'ss', 'acc', 'rof', 'shots', 'st', 'rcl')
		weaponString = ''
		weaponStatKeys = ('name', 'dam', 'st', 'rch', 'note')
		rangedWeaponString = ''
		weapons = npc.mData['weapons'].items()
		weapons.sort()
		for weaponKey, weapon in weapons:
			weapon = weapon.copy()
			if '*mod0' in weapon:
				weapon['name'] = '<FONT COLOR=#3333CC>%s</FONT>' % weapon['name']
			if ('st' in weapon) and (GetInt(weapon['st']) > npc.mData['stats']['strength']):
				weapon['st'] = '<FONT COLOR=#CC0033>%s</FONT>' % weapon['st']

			if 'ss' in weapon:
				total += 1
				for rangedWeaponStatKey in rangedWeaponStatKeys:
					if rangedWeaponStatKey not in weapon:
						if (rangedWeaponStatKey == 'shots'):
							weapon['shots'] = ''
							for shotsOptionKey in ('ammo', 'quantity'):
								if shotsOptionKey in weapon:
									weapon['shots'] = weapon[shotsOptionKey]
									break
						elif (rangedWeaponStatKey == 'dam') and ('damage' in weapon):
							# get real damage for fantasy ranged weapons
							weapon['dam'] = NPCBuilder.GetWeaponDamage(npc, weapon)
						else:
							weapon[rangedWeaponStatKey] = ''
				# Hack? don't put dash in certain weapon 'dam's so that html display doesn't split on hyphen
				if ('-' in weapon['dam']) and (weapon['dam'][-1] in ('-', '+')):
					weapon['dam'] = weapon['dam'].replace('-', '&#8209;')
				rangedWeaponString += '''
							<TR><TD>%s</TD> <TD><CENTER>%s</CENTER></TD>
								<TD><CENTER>%s</CENTER></TD> <TD><CENTER>%s</CENTER></TD> <TD><CENTER>%s</CENTER></TD>
								<TD><CENTER>%s</CENTER></TD> <TD><CENTER>%s</CENTER></TD> <TD><CENTER>%s</CENTER></TD> </TR>''' % \
							(weapon['name'], weapon['rel'], weapon['dam'], weapon['acc'],
							      weapon['rof'], weapon['shots'], weapon['st'], weapon['rcl'])
				if '*mod0' in weapon:
					rangedWeaponModString = ''
					for n in range(10):
						modKey = '*mod%d' % n
						if modKey in weapon:
							total += 1
							rangedWeaponModString += '%s<BR>&nbsp;&nbsp;&nbsp;' % weapon[modKey]
					rangedWeaponString += '''
							<TR><TD WIDTH=40%s>&nbsp;&nbsp;&nbsp;<FONT COLOR=#3333CC>%s</FONT></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD></TR>''' % \
							('%', rangedWeaponModString[:-22])

			elif 'rch' in weapon:
				total += 1
				for weaponStatKey in weaponStatKeys:
					if weaponStatKey not in weapon:
						weapon[weaponStatKey] = ''
				weaponString += '''
						<TR><TD WIDTH=40%s>%s</TD> <TD><CENTER>%s</CENTER></TD> <TD><CENTER>%s</CENTER></TD> <TD>%s</TD> <TD>%s</TD> </TR>''' % \
							('%', weapon['name'], NPCBuilder.GetWeaponDamage(npc, weapon), weapon['st'], weapon['rch'], weapon['note'])
				if '*mod0' in weapon:
					weaponModString = ''
					for n in range(10):
						modKey = '*mod%d' % n
						if modKey in weapon:
							total += 1
							weaponModString += '%s<BR>&nbsp;&nbsp;&nbsp;' % weapon[modKey]
					weaponString += '''
							<TR><TD WIDTH=40%s>&nbsp;&nbsp;&nbsp;<FONT COLOR=#3333CC>%s</FONT></TD><TD></TD><TD></TD><TD></TD><TD></TD></TR>''' % ('%', weaponModString[:-22])
		return (total, {'RANGEDWEAPONS': rangedWeaponString, 'WEAPONS': weaponString})
	GetWeaponsDict = staticmethod(GetWeaponsDict)


	def GetStatsAndAttributesDict(npc):
		localDict = {}
		localDict.update(npc.mData['stats'])
		localDict.update(npc.mData['attributes'])
		localDict['move'] = int(npc.mData['attributes']['move'])
		localDict['encMove'] = localDict['move'] - npc.mData['encumbrance']['level'][0]
		return localDict
	GetStatsAndAttributesDict = staticmethod(GetStatsAndAttributesDict)


	def GetSkillsDict(npc):
		return NPCBuilder.GetKeyDict(npc, 'skills')
	GetSkillsDict = staticmethod(GetSkillsDict)


	def GetAdvantagesDict(npc):
		localDict = NPCBuilder.GetKeyDict(npc, 'advantages')
		total = 0
		for value in localDict.values():
			total += value.count('<TR>')
		return (total, localDict)
	GetAdvantagesDict = staticmethod(GetAdvantagesDict)


	def GetDisAdvantagesDict(npc):
		npc.mData['quirkSet'] = {}
		for quirk in npc.mData['quirks']:
			npc.mData['quirkSet']['quirk: ' + quirk] = 0
		localDict = NPCBuilder.GetKeyDict(npc, 'quirkSet')
		localDict.update(NPCBuilder.GetKeyDict(npc, 'disadvantages'))
		total = 0
		for value in localDict.values():
			total += value.count('<TR>')
		return (total, localDict)
	GetDisAdvantagesDict = staticmethod(GetDisAdvantagesDict)


	def GetKeyDict(npc, key):
		keyString = ''
		keys = npc.mData[key].keys()
		keys.sort()
		for keyName in keys:
			value = abs(npc.mData[key][keyName])
			if not value:
				value = ''
			# HACK (for value: 0)
			if (keyName in NPCConstants.kZeroValueAttributes) and not value:
				value = 0
			keyString += '''
				<TR><TD WIDTH=90%s ALIGN=LEFT>%s</TD><TD WIDTH=10%s ALIGN=RIGHT>%s</TD></TR>''' % ('%', keyName, '%', value)
		return {key.upper(): keyString}
	GetKeyDict = staticmethod(GetKeyDict)


	def GetAppearanceDict(npc):
		localDict = {}
		localDict['name'] = npc.getFullRankName()
		localDict['className'] = npc.mData['class']
		localDict.update(npc.mData['appearance'])
		return localDict
	GetAppearanceDict = staticmethod(GetAppearanceDict)


	def getNPC(self):
		npc = NPC(self.mData)
		# quick hack to preserve rank!
		npc.getRank()
		return npc
