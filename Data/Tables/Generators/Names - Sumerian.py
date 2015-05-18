from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        groups = {1: self.vcvc()+self.vcvc(),
                  2: self.vcvc()+self.mix(),
                  3: self.vcvc()+self.vc(), 
                  4: self.vcvc()+self.vcv(), 
                  5: self.ccvc()+self.vcvc(), 
                  6: self.ccvc()+self.mix(), 
                  7: self.ccvc()+self.vc(), 
                  8: self.ccvc()+self.vccv(), 
                  9: self.ccvc()+self.vcv(), 
                  10: self.cv()+self.cv(), 
                  11: self.cv()+self.cvcc(), 
                  12: self.cvc()+self.vcvc(), 
                  13: self.cvc()+self.mix(), 
                  14: self.cvc()+self.ccvc(), 
                  15: self.cvc()+self.cvcc(), 
                  16: self.cvc()+self.vc(), 
                  17: self.cvc()+self.vccv(), 
                  18: self.cvc()+self.vcv(), 
                  19: self.cvcc()+self.vcvc(), 
                  20: self.cvcc()+self.vc(), 
                  21: self.cvcc()+self.vcv(), 
                  22: self.cvcv()+self.cv(), 
                  23: self.cvcv()+self.cvcv(), 
                  24: self.vc()+self.vcvc(), 
                  25: self.vc()+self.vc(), 
                  26: self.vc()+self.vccv(), 
                  27: self.vc()+self.vcv(), 
                  28: self.vccv()+self.ccvc(), 
                  29: self.vccv()+self.cv(), 
                  30: self.vccv()+self.cvcc(), 
                  31: self.vccv()+self.cvcv(), 
                  32: self.vccv()+self.cvc(), 
                  33: self.vcv()+self.ccvc(), 
                  34: self.vcv()+self.cv(), 
                  35: self.vcv()+self.cvc(), 
                  36: self.vcv()+self.cvcc(), 
                  37: self.vcv()+self.cvcv(),
        }

        result = groups[randint(1,37)]

        if (result[len(result)-1] == "'"):
                result = result[:-1]

        return result.capitalize()

    def vcvc(self):
        elements = ["agam","agar","akan","akar","alan","alim","amar","asar","azag","ebir","eden","edim","edin","egir","elam","enar","enir","epir","eren","erib","erim","erin","esan","esig","esir","ezem","ezen","idig","idim","imin","inar","inim","isim","itid","itim","itud","ubil","ubur","udug","udun","ugar","ugun","ukum","ukur","ulul","umab","umum","umun","unug","unum","urim","urin","urum","usan","usar","usug","utab","utug","utul","uzug"]
        return choice(elements)

    def mix(self):
        elements = ["lu-e","ku-a","sha","she"]
        return choice(elements)

    def ccvc(self):
        elements = ["shab","shag","shak","shal","sham","shap","shar","shed","sheg","shem","shen","sher","shes","shib","shid","shig","shim","ship","shir","shit","shub","shud","shug","shul","shum","shun","shur","skim"]
        return choice(elements)

    def cv(self):
        elements = ["ba","da","ga","ha","ia","ka","la","ma","na","pa","ra","sa","ta","za","be","de","ge","he","ke","le","me","ne","pe","re","se","te","ze","bi","di","ei","gi","ki","li","mi","ni","pi","ri","si","ti","zi","bu","du","du'","gu","hu","ku","lu","mu","nu","pu","ru","su","tu","zu"]
        return choice(elements)

    def cvc(self):
        elements = ["bab","bad","bag","bak","bal","ban","bap","bar","baz","ben","bid","bil","bin","bir","biz","bol","bub","bud","bul","bum","bun","bup","bur","buz","dab","dag","dak","dal","dam","dan","dar","del","dib","did","dig","dil","dim","din","dir","dub","dug","duh","duk","dul","dum","dun","dur","egi","esh","esi","ezi","gab","gag","gak","gal","gam","gan","gar","gaz","gen","gib","gid","gig","gik","gil","gim","gin","gir","git","giz","gub","gud","gug","guk","gul","gum","gun","gur","guz","har","hin","hub","hul","hun","hur","kab","kad","kak","kal","kam","kan","kar","kas","kid","kik","kil","kim","kin","kir","kis","kud","kug","kuk","kul","kum","kun","kur","kus","lab","lad","lag","lak","lal","lal'","lam","lan","lib","lid","lil","lim","lub","lud","lug","lul","lum","lun","lus","mab","mad","man","mar","mel","men","mer","mes","mid","min","mir","mud","mug","muk","mul","mun","mur","nad","nag","nal","nam","nan","nar","nen","ner","nib","nid","nig","nim","nin","nir","nis","niz","nud","nug","nun","nus","nuz","pab","pad","pag","pan","pap","par","pil","pin","pir","pun","rab","ran","rap","rib","rig","rim","rin","rub","rud","rug","rum","run","sad","sag","sal","sam","san","sar","sed","seg","ses","shi","sid","sig","sik","sil","sim","sin","sir","sis","sub","sud","sug","suh","suk","sul","sum","sun","sur","sus","tab","tag","tak","tal","tam","tan","tar","ten","tib","til","tim","tin","tir","tub","tud","tug","tuk","tul","tum","tun","tur","ush","zab","zag","zal","zan","zar","zeb","zer","zib","zid","zig","zil","ziz","zub","zud","zum","zur"]
        return choice(elements)

    def cvcc(self):
        elements = ["bash","gash","kash","mash","rash","desh","kesh","mesh","nesh","pesh","tesh","dish","kish","mish","nish","pish","bush","kush","lush","mush","push","rush","sush","tush"]
        return choice(elements)

    def cvcv(self):
        elements = ["bada","bala","bala'","bara","bira","buru","dala","dara","deri","dida","didi","dili","diri","dumu","duru","dutu","gaba","gada","gaga","gage","gala","gana","gara","gazi","geme","gidi","gili","giri","gisu","guda","gula","gunu","guru","inda","kala","kara","kili","kiri","kisi","kuba","kuru","lama","lima","limi","liri","liru","luki","luma","lusu","mabi","mana","medu","meli","meze","mina","munu","muru","nala","nana","naza","nele","neme","niga","nigu","nili","ninu","niri","nisi","nita","pala","pana","para","rutu","sagi","sesi","sila","sipa","suku","suru","taka","tena","tuku","tuma","tumu","zubu"]
        return choice(elements)

    def vc(self):
        elements = ["ab","ad","ag","ak","al","am","an","ar","as","az","eb","ed","eg","el","em","en","er","ib","id","ig","ik","il","im","in","ir","is","it","iz","or","uh","ub","ud","ug","uk","ul","um","un","ur","us","ut","uz"]
        return choice(elements)

    def vccv(self):
        elements = ["ebla","emma","esha","imma","ugra","asha","iani","ishi","ashe","eshe","ensi","illu","ummu","urdu","urgu","ushu","ussu"]
        return choice(elements)

    def vcv(self):
        elements = ["aba","ada","aga","aka","ala","ama","ana","ara","asa","dra","eda","era","ida","ila","ima","ira","uga","una","die","due","eme","azu","eru","iku","itu","mau","pau","riu","shu","udu","ugu","uku","ulu","umu","unu","uru","usu","utu","uzu","uri","abi","ali","ash","asi","ibi","idi","igi","ili","imi","isi","iti","izi"]
        return choice(elements)




#x = generator()
#print x.start()