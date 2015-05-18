from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        roll = randint(1,6)

        result = ''
        if (roll <= 2):
            result = self.aoran(self.metal()) + " " + self.blazon1() + " " + self.blazon2() + "on a " + self.color() + " field"
        elif (roll <= 4):
            result = self.aoran(self.color()) + " " + self.blazon1() + " " + self.blazon3() + "on a " + self.metal() + " field"
        elif (roll <= 5):
            result = self.numwords() + " " + self.metal() + " " + self.plural(self.blazon1()) + " " + self.blazon2() + "on a " + self.color() + " field"
        else:
            result = self.numwords() + " " + self.color() + " " + self.plural(self.blazon1()) + " " + self.blazon3() + "on a " + self.metal() + " field"
            
        return result

    def metal(self):
        elements = ["gold","silver"]
        return choice(elements)

    def color(self):
        elements = ["red","blue","green","black"]
        return choice(elements)

    def blazon1(self):
        elements = ["stripe","sword","tower","fist","skull","star","fleur-de-lys","[Creature]","[Creature]","[Creature]","crown","arrow","tree","cross","bar","shell","harp","ring","axe","ship","sun","moon"]
        result = choice(elements)

        if result == "[Creature]":
            result = self.creature()
            
        return result

    def creature(self):
        elements = entries=["bear","boar","bull","cock","devil","dragon","drake","eagle","falcon","fox","griffon","hare","hawk","horse","hound","lion","owl","raven","serpent","stag","unicorn","wolf","wyvern"]
        return choice(elements)

    def blazon2(self):
        groups = {  1: "and " + self.aoran(self.metal()) + " " + self.blazon1() + " ",
                    2: "and " + self.numwords() + " " + self.color() + " " + self.plural(self.blazon1()) + " ",
                    3: "",
        }
        return groups[randint(1,3)]

    def blazon3(self):
        groups = {  1: "and " + self.aoran(self.color()) + " " + self.blazon1() + " ",
                    2: "and " + self.numwords() + " " + self.color() + " " + self.plural(self.blazon1()) + " ",
                    3: "",
        }
        return groups[randint(1,3)]

    def numwords(self):
        elements = ["two","three","four"]
        return choice(elements)
        
    def aoran(self, txt):
        if (txt[0] in 'aeiou'):
            txt = 'an ' + txt
        else:
            txt = 'a ' + txt
        return txt
            
    def plural(self, txt):
        result = txt
        suffix = 's'

        if result[-1].lower() in 'sxz':
            suffix = 'es'
        if result[-2:].lower() == 'ff':
            result = result[0:-2]
            suffix = 'ves'
        if result[-1].lower() == 'f':
            result = result[0:-1]
            suffix = 'ves'
        if result[-2:].lower() == 'is':
            result = result[0:-2]
            suffix = 'es'
        if result[-2:].lower() in ('ch', 'sh'):
            suffix = 'es'
        if result[-1].lower() == 'o':
            if result[-2].lower() not in 'aeiou':
                suffix = 'es'
        if result[-1].lower() == 'y':
            if result[-2].lower() not in 'aeiou':
                result = result[0:-1]
                suffix = 'ies'
                
        result += suffix

        return result	





#x = generator()
#print x.start()