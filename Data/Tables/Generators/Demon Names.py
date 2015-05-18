from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        return self.syllable().capitalize() + self.segments()
        
    def segments(self):
        roll = randint(1,7)

        result = ''
        if (roll <= 4):
            result = self.build_name() + self.build_name()
        elif (roll <= 6):
            result = self.build_name() + self.build_name() + self.build_name()
        else:
            result = self.build_name() + self.build_name() + self.build_name() + self.build_name()
        return result

    def build_name(self):
        roll = randint(1,7)

        result = ''
        if (roll <= 2):
            result = "'" + self.syllable()
        elif (roll <= 3):
            result = "-" + self.syllable()
        else:
            result = self.syllable()
        return result

    def syllable(self):
        elements = ["slag","el","nagir","girruk","zug","tha","meg","skal","deth","esh","uth","sshul","zan","zhel","khs","chal","vush","chlu","varn","elts","ch","aa","ugg","ksin","ven","phod","sagy","aak","qek","zohr","kkis","eg","zan","gkil","aezg","gluU"]
        return choice(elements)




#x = generator()
#print x.start()