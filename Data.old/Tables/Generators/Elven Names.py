from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        roll = randint(1,6)

        if (roll <= 4):
            result = self.pre() + self.mid() + self.suf()
        elif (roll <= 5):
            result = self.pre() + self.mid() + self.mid() + self.suf()
        else:
            result = self.pre() + self.suf()
            
        result = result.capitalize() + ' ' + self.surnamepre().capitalize() + self.surnamesuf()
        
        return result

    def pre(self):
        elements = ["a","ae","am","amon","an","ar","ara","barad","baran","bel","beleg","dae","dol","don","dun","e","el","ga","galad","gil","gon","gul","i","isen","loth","mene","mith","mor","naz","nim","orod","thin"]
        return choice(elements)

    def mid(self):
        elements = ["be","dri","fa","la","len","ma","na","ran","ro","sil"]
        return choice(elements)

    def suf(self):
        elements = ["adan","amroth","cirith","del","dil","dir","dor","duin","dur","falas","gard","gol","gorn","groth","gul","ia","las","ost","rain","ras","reth","rie","riel","rion","rond","ros","roth","roth","ruin","thyryr"]
        return choice(elements)

    def surnamepre(self):
        elements = ["alder","arrow","ash","bark","battle","bear","beech","bell","birch","black","blue","bold","brave","briar","bright","clear","cloud","cold","cool","crystal","dark","deep","deer","dew","elden","elder","elm","even","ever","fair","fallen","fast","fell","fleet","foam","fog","follow","forest","frost","gem","glad","gold","golden","good","green","grey","grim","hard","hawk","hidden","high","hollow","ice","iron","jewel","keen","kind","larch","leaf","light","lone","lost","maple","marsh","mist","moon","morning","moss","mountian","nature","never","new","oak","oaken","old","pale","pine","pure","quick","quiet","rain","raven","red","rose","shadow","shimmer","shining","silent","silven","silver","snow","sodden","song","sparrow","spell","star","storm","strong","sudden","swift","tall","tree","true","vale","valley","web","wet","white","wild","wilder","wise","wonder","wren","yellow"]
        return choice(elements)

    def surnamesuf(self):
        elements = ["arrow","ash","blossom","bough","bow","brand","bright","brook","brow","cairn","call","cloud","crown","dale","dark","dawn","dell","dew","drake","drop","even","ever","eye","fall","feather","fern","field","fire","flower","fruit","glade","gleam","glen","glitter","gold","grass","green","hair","hand","heart","helm","lake","leaf","light","lock","lore","love","maiden","mane","marsh","meadow","meet","mist","moon","oak","owl","path","petal","pool","rain","raven","reed","road","rock","root","shield","shore","sight","silver","singer","sky","slope","song","spear","spell","spirit","staff","star","stone","storm","stream","strider","sun","sword","thorn","trail","tree","vale","walker","warden","watcher","water","white","wind","wing","wood"]
        return choice(elements)




#x = generator()
#print x.start()