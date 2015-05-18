from random import choice

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''

        result = self.pre() + self.suf()

        return result

    def pre(self):
        elements = ["Agh","Alim","Aquil","Ar","Arg","As","Bel","Boss","Bry","Cim","Cor","Dar","Er","Gal","Gun","Hal","Hyp","Hyr","Ian","Ir","Kass","Kesh","Khaur","Khaw","Khem","Khit","Khor","Khor","Khor","Kor","Kord","Koth","Kush","Kuth","Larsh","Lux","Mes","Nem","Num","Oph","Pte","Punt","Sam","Shad","Sham","Shang","Styg","Sul","Sukh","Tan","Taur","Tur","Tyb","Van","Vendh","Vil","Xa","Xuch","Xuth","Zam","Zarkh","Zing"]
        return choice(elements)

    def suf(self):
        elements = ["a","ai","aja","al","ali","an","ane","ar","ara","as","ava","borea","boula","chem","der","e","eba","ed","en","eria","er","es","far","gal","gard","heim","i","ia","ia","in","ir","ish","ism","istan","jun","kan","land","mer","met","og","on","onia","or","ora","org","os","ot","otl","par","pur","ra","san","shem","tana","the","thia","thun","tia","uk","ul","un","ur","us","ver","ya","yet","zar"]
        return choice(elements)




#x = generator()
#print x.start()