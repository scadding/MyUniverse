from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        roll = randint(1,2)
        if roll == 1:
            part1 = self.names().capitalize()
            part2 = part1
            
            while (part1 == part2):
                part2 = self.names()
            
            result = part1 + part2
            
        else:
            result = self.pre().capitalize() + self.suff()
        
        return result

    def names(self):
        elements = ["hawk","fair","spear","eagle","sea","dreamer","black","stone","singer","dark","fox","bow","blade","death","high","grey","changer","strong","wood","white","bear","star","sly","claw","sure","slayer","helm","staff","gold","wave","shadow","tiger","shield","brother","cleaver","sky","dancer","flame","bone","moon","lion","foam","red","horn","soul","silver","fang","fist","wolf","blood","bane","free","rune","wise","hammer","storm","piper","weaver","heart","wind"]
        return choice(elements)

    def pre(self):
        elements = ["bird","crystal","dove","earth","fast","flame","glitter","gold","golden","hawk","honor","ice","iron","lightning","maple","moon","oak","raven","river","shadow","silver","sly","small","snow","star","steel","stone","sun","swift","wild","willow","wind","winter"]
        return choice(elements)

    def suff(self):
        elements = ["bird","bones","caller","child","cloak","dancer","dove","eye","flower","fox","gold","hand","hawk","heart","lady","leaf","light","lover","raven","shadow","singer","song","foot","man","skin","strider","wanderer","whisper","will","wind"]
        return choice(elements)




#x = generator()
#print x.start()