from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        if randint(1,4) <= 3:
            result = self.twosyl()
        else:
            result = self.threesyl()
        return result.capitalize()

    def twosyl(self):
        groups = {  1: self.cc()+self.apos()+self.cc(),
                    2: self.cv()+self.cc(),
                    3: self.cv()+self.cv(),
                    4: self.vc()+self.apos()+self.cc(),
                    5: self.vc()+self.apos()+self.cv(),
                    6: self.vc()+self.vc(),
                    7: self.vv()+self.cc(),
                    8: self.vv()+self.cv(),
        }
        return groups[randint(1,8)]

    def threesyl(self):
        groups = {  1: self.cc()+self.apos()+self.cc()+self.apos()+self.cc(),
                    2: self.cv()+self.apos()+self.cc()+self.vc(),
                    3: self.cc()+self.vv()+self.cc(),
                    4: self.vc()+self.vv()+self.cv(),
                    5: self.cv()+self.cc()+self.vc(),
                    6: self.cv()+self.cv()+self.cv(),
                    7: self.vc()+self.vc()+self.vc(),
                    8: self.vv()+self.cc()+self.vc(),
                    9: self.vv()+self.cc()+self.vv(),
                    10: self.vv()+self.cv()+self.cv(),
                    11: self.vc()+self.cc()+self.vc(),
        }
        return groups[randint(1,11)]

    def apos(self):
        result = ''
        if randint(1,3) == 3:
            result = "'"
        return result

    def vc(self):
        elements = ["aal","al","an","ang","atl","ax","axl","az","il","ill","is","iss","it","ith","itz","iz","otl","ov","oxl","us","uss","uz"]
        return choice(elements)

    def vv(self):
        elements = ["a","aka","aza","i","ila","illa","iza","o","ova","ovi"]
        return choice(elements)

    def cv(self):
        elements = ["cha","chka","cho","chta","hexo","hu","ka","kaa","kha","khi","ki","kii","kra","kry","ky","kzo","la","pa","qua","que","ra","rasa"]
        return choice(elements)

    def cc(self):
        elements = ["ch","cotl","cus","haat","has","hass","haz","hex","his","hiss","huax","kaar","kar","khat","kil","kis","kys","lan","mas","mass","mis","miss","pec","poc","ptar","pter","ras","rep","rept","sal","sith","tak","tan","tax","tec","tek","tep","than","this","tis","tl","tlac","tlan","tlax","trax","tzun","val","var","vas","ven","vil","vis","xh","xlan","yxl","zaal","zah","zak","zal","zla"]
        return choice(elements)




#x = generator()
#print x.start()