from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0
    
    def start(self):
        result = ''
        roll = randint(1,7)
        
        if roll <= 4:
            result = (self.prefix() + self.suffix()).capitalize()
        elif roll <= 6:
            result = (self.prefix() + self.suffix()).capitalize() + '-' + (self.prefix() + self.suffix()).capitalize()
        else:
            result = self.prefix().capitalize()
            roll2 = randint(1,3)
            
            for j in range(roll2):
                result = result + self.middle()
            
            result = result + self.suffix()
        
        return result
        
    def prefix(self):
        elements = ["ab","adj","ah","aman","amen","amun","ani","ankh","ap","apo","aset","ash","bak","bas","bes","bith","dja","dje","en","esem","geb","gen","hap","hat","hath","hen","henut","hep","heq","her","het","ib","id","im","in","ip","is","iset","it","iu","iy","ka","kar","kem","kha","khe","kho","khu","lux","maa","meh","mem","mer","mut","na","nakh","nan","neb","nef","nefer","nes","nub","pa","pad","pak","pas","pen","pesh","ptah","ra","ram","ren","sa","sat","seb","sel","sen","set","shed","shem","shep","sob","sot","sut","tab","tah","takh","tef","tet","tut","wer"]
        return choice(elements)
        
    def middle(self):
        elements = ["u","akh","ana","athor","bash","em","es","fer","hen","het","hir","ho","iam","ian","ju","ke","ket","khar","khe","khef","khen","khm","kop","mos","nakh","nakt","ned","nem","neph","net","no","on","pakh","pat","ren","sat","sen","shep","ten","tep","tim","tut"]
        return choice(elements)

    def suffix(self):
        elements = ["aat","akht","ankh","ar","asht","ast","aten","ath","ayt","bet","daa","det","dis","eb","ebi","eni","epes","eru","hat","he","heb","hef","her","heri","het","his","hor","imen","is","khem","kis","ma","mat","mhat","mut","nen","nut","or","or","osh","pet","pet","phis","ptah","ra","ru","sa","shef","sut","tari","ten","tep","thys","tis","tit","to","tra","u"]
        return choice(elements)


#x = generator()
#print x.start()