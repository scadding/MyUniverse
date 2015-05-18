from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        roll = randint(1,4)
        if roll <= 3:
            groups = { 0: self.seg() + self.seg(),
                       1: self.pref() + self.seg(),
                       2: self.seg() + self.suff(),
                       3: self.pref() + self.suff(),
            }
            result = groups[randint(0,3)].capitalize()
        else:
            groups = { 0: self.seg() + self.seg() + self.seg(),
                       1: self.pref() + self.seg() + self.seg(),
                       2: self.seg() + self.seg() + self.suff(),
                       3: self.pref() + self.seg() + self.suff(),
            }
            result = groups[randint(0,3)].capitalize()
            
        if randint(1,2) ==1 :
            result = result + ' ' + self.lastpref().capitalize() + self.lastsuff()
            
        return result

    def seg(self):
        elements = ["ad","ag","am","ar","ash","at","az","bad","bag","bak","bakh","bam","bar","bash","bat","baz","bog","bol","bor","both","bub","buf","bug","buk","bul","bum","bur","bush","buz","dub","duf","dug","duk","dul","dum","dur","dush","duz","gad","gak","gakh","gam","gar","gash","gat","gaz","ghad","ghak","ghakh","gham","ghar","ghash","ghat","ghaz","ghob","ghol","ghor","ghoth","glad","glak","glakh","glam","glar","glash","glat","glaz","glim","glob","glor","gloth","glub","gluf","gluk","glum","glur","glush","gluz","gob","gol","gonk","gor","goth","grad","grak","grakh","gram","grash","grat","graz","grim","grish","grob","grol","grub","gruf","gruk","grul","grum","grush","gruz","gub","guk","gul","gum","gur","khad","khag","kham","khar","khash","khat","khaz","lad","lag","lak","lakh","lam","lar","lash","lat","laz","lim","lob","log","lonk","lor","loth","lub","luf","lug","luk","lum","lur","lush","luz","mad","mag","mak","makh","mar","mash","mat","mauh","maz","mob","mog","mol","monk","mor","moth","mub","muf","mug","muk","mul","mur","mush","muz","nad","nag","nakh","nam","nar","nash","nat","naz","og","ol","or","oth","rad","rag","rak","ram","ramph","rash","rat","raz","rim","rish","rob","rog","rol","roth","rub","ruf","rug","rush"]
        return choice(elements)

    def pref(self):
        elements = ["brag","cad","car","dor","far","fog","fr","grop","grud","hrad","hrod","ror","ruk","rul","rum","ruz","shad","shag","shak","shakh","sham","shar","shat","shaz","shel","shub","shuf","shug","shuk","shul","shum","shur","shuz","snad","snag","snak","snakh","snam","snar","snat","snaz","sor","star","thor","thul","uft","ug","ul","um","ur","ush","uz","war","yad","yag","yak","yakh","yam","yar","yarn","yash","yat","yaz"]
        return choice(elements)

    def suff(self):
        elements = ["ak","arg","dak","darg","dark","dash","drak","dush","ga","gar","gask","gor","gush","hack","hag","ich","jak","kak","kar","kor","lak","mak","nak","rack","rag","rake","urgh","urk","wack"]
        return choice(elements)

    def lastsuff(self):
        elements = ["basher","beater","belly","breaker","chewer","choker","crawler","crusher","cutter","dicer","dragger","drinker","eater","gobber","gobbler","hack","hammer","hater","juggler","kicker","killer","lover","maimer","mangler","masher","poker","prodder","punisher","raper","ripper","scarer","shaker","slammer","slasher","slicer","slobberer","slurper","smasher","stomper","thrasher","thrower","throttler","trembler","torturer","walker","wrecker"]
        return choice(elements)

    def lastpref(self):
        elements = ["arm","axe","ball","blood","bone","bone","cleaver","club","dagger","demon","devil","dryad","dwarf","ear","earth","elf","ettin","eye","face","finger","fist","flesh","foot","ghoul","giant","gnoll","gnome","goblin","hair","hammer","hand","head","heart","human","kidney","knife","kobold","leg","liver","man","moon","nail","nose","nose","ogre","rat","rock","skull","spear","spike","spleen","star","stone","sun","sword","throat","toe","tooth","troll","wolf"]
        return choice(elements)




#x = generator()
#print x.start()