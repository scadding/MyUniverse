from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        roll = randint(1,11)

        result = ''
        if (roll <= 2):
            result = self.adj().capitalize() + ' ' + self.noun().capitalize() + ' of ' + self.prefv().capitalize() + self.suff()
        elif (roll <= 5):
            result = self.adj().capitalize() + ' ' + self.noun().capitalize() + ' of ' + self.prefc().capitalize() + self.suff()
        elif (roll <= 7):
            result = self.prefc().capitalize()+'ian' + ' ' + self.noun().capitalize()
        elif (roll <= 8):
            result = self.prefc().capitalize()+'ian' + ' ' + self.adj().capitalize() + ' ' + self.noun().capitalize()
        elif (roll <= 10):
            result = self.prefc().capitalize() + self.suff()
        else:
            result = self.prefv().capitalize() + self.suff()
        return result

    def adj(self):
        elements = ["abhorrent","abominable","ancient","black","blasphemous","crawling","creeping","crypt","damned","dark","dead","deep","dimensional","dread","elder","eldritch","esoteric","evil","faceless","flying","formless","gaunt","hideous","horrific","living","loathsome","lurking","mad","mist","night","red","shadow","shambling","shunned","sightless","slithering","star","tomb","white","winged"]
        return choice(elements)

    def noun(self):
        elements = ["beasts","burrowers","children","crawlers","creatures","creepers","dead","devourers","dwellers","followers","hounds","horrors","hunters","ones","ooze","outsiders","polyps","prowlers","servants","shamblers","spawn","stalkers","things","walkers","worms","young"]
        return choice(elements)

    def prefv(self):
        elements = ["aza","bya","ctha","gha","ghata","golo","itha","kha","kla","nya","nyag","nyarla","tsa"]
        return choice(elements)

    def prefc(self):
        elements = ["ab","ash","ashtor","azar","azaz","byak","byal","chaug","cthon","cthug","cthul","dag","faug","ghatan","hal","hast","ith","ithaq","kyn","len","leng","nyar","nyog","r'y","shog","shub","soth","tsag","tsath","y'g","yith","yog"]
        return choice(elements)

    def suff(self):
        elements = ["ash","aten","azar","azash","ekesh","eth","gash","gha","gon","goth","gua","hotep","hoth","hu","ikesh","kesh","khee","lash","loth","lyeh","nac","nar","nash","oggua","on","onac","oth","othoa","rath","tha","thal","thoa","thotep","thoth","ugha","ulhu","urath","ze"]
        return choice(elements)




#x = generator()
#print x.start()