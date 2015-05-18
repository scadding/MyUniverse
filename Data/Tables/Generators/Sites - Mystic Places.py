from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        groups = { 0: self.place() + ' of ' + self.thing(),
                   1: self.adj() + ' ' + self.place(),
                   2: self.adj() + ' ' + self.place() + ' of ' + self.thing(),
        }
        result = groups[randint(0,2)]
        return result

    def adj(self):
        elements = ["Bright","Burning","Dead","Eternal","Forgotten","Golden","Living","Lost","Shining","Silver","Sunken","Undying"]
        return choice(elements)

    def place(self):
        elements = ["Castle","Cave","Chasm","City","Coast","Desert","Field","Forest","Fortress","Fountain","Garden","Grove","Hamlet","Hill","Isle","Jungle","Lake","Mansion","Marsh","Mesa","Mine","Mire","Mountain","Pillar","River","Ruins","Sea","Shores","Shrine","Source","Spire","Stone","Swamp","Temple","Tower","Valley"]
        return choice(elements)

    def thing(self):
        elements = ["the Abyss","Blood","Bronze","Dawn","the Dead","Doom","Dragons","Dread","Dust","Eternal Peril","Fire","Flame","Frost","the Gods","Gold","Heaven","Ice","the Inferno","Iron","Lightning","Maps Edge","Mists","Mithril","No Return","Runes","Scales","the Seven","Shadow","Silver","Skulls","the Sky","Stone","Swords","Thunder","Wind","Youth"]
        return choice(elements)




#x = generator()
#print x.start()