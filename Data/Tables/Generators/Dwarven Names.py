from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        fname = ''
        lname = ''
        fem = 0
        
        if (randint(1,2) == 1):
            fname = self.pre() + self.sufm()
        else:
            fname = self.pre() + self.suff()
            fem = 1
            
        if (randint(1,3) == 1):
            father = self.pre() + self.sufm()
            if (fem == 0):
                lname = father + "son"
            else:
                lname = father + "sdottir"
        else:
            lname = self.clanpre().capitalize() + self.clansuff()
            
        result = fname + " " + lname
        if (fem == 1):
            result = result + " (f.)"

        return result

    def pre(self):
        elements = ["Ag","Al","Ald","Alf","Ar","Arn","Art","As","Ath","Athran","Aud","Bal","Bala","Bar","Bara","Bel","Bela","Belf","Ber","Bif","Bof","Bok","Bol","Bom","Bor","Bra","Brott","Brun","Bryn","Bur","D","Da","Dag","Dam","Dar","Dor","Dora","Drok","Drong","Dur","Dwal","Eb","Ein","El","Ela","Elan","Elda","Fa","Faf","Far","Fara","Fil","Fim","Fima","Firen","Fo","For","Fros","Fur","Fura","Ga","Gar","Gil","Gim","Gir","Glam","Gol","Gollen","Gor","Got","Gota","Grim","Gro","Grun","Gunn","Gus","Gut","Ha","Had","Hak","Haka","Hal","Han","Har","Has","Hega","Hel","Hun","Hur","Ing","Jar","Kad","Kar","Kata","Kaz","Kaza","Kha","Khar","Kol","Krag","Kur","Lar","Logaz","Lok","Lun","Mag","Mel","Mo","Mola","Mor","Mora","Na","Nar","No","Nola","Nor","Noran","Nord","Nun","Nyr","Oda","Oka","Ol","Olf","Olla","Osk","Oth","Othra","Rag","Rak","Ro","Rog","Ror","Roran","Run","Rur","Sa","Sig","Ska","Skaf","Skalf","Skalla","Skar","Skeg","Skor","Skora","Snor","Snora","Sven","Tak","Thar","Tho","Thor","Thora","Thra","Thro","Thron","Thrun","Thura","Ulf","Ulla","Un","Utha","Val","Vala","Var","Vara","Ver","Ves","Yng","Zak","Zaka","Zakan","Zam","Zar","Zara","Zarna"]
        return choice(elements)

    def sufm(self):
        elements = ["ain","ald","ar","ard","arr","bin","bon","bor","born","brun","bur","dalf","dan","dar","den","di","dil","din","dir","dok","dor","dran","drin","ed","end","endd","fin","finn","fur","gan","gar","gard","gi","gil","gin","gir","gni","gon","gor","grim","grin","grir","grom","grond","groth","grum","grund","grunt","gui","gun","gund","hall","hel","hild","hor","ic","ik","in","ir","is","jald","ki","kil","kin","krag","kri","krin","li","lik","lin","ling","linn","lir","lok","lum","lun","mad","min","mir","mund","nar","ni","nin","nir","nus","odd","oin","olf","or","rag","ran","rand","rek","ri","rig","rik","rin","rir","run","sil","sin","skin","sur","tan","thor","ti","tin","tok","trek","trok","ur","urd","vald","vard","vir","zin","zor"]
        return choice(elements)

    def suff(self):
        elements = ["a","asi","bera","bina","bora","cla","dda","dila","dina","dis","dokina","dora","drid","drinella","era","fina","fya","ga","gana","gara","gella","gerd","gina","gona","gora","grid","grimella","grina","groma","gromina","grondella","grotha","gruma","grunda","gruntina","gula","gundina","gunella","hild","i","ia","il","ilda","ima","ja","kina","kragella","krina","kya","la","likina","lina","loda","loka","luna","mina","mira","na","nala","nina","nira","nya","ona","ra","ragina","rasa","rid","riga","rika","rina","runa","runella","sa","sif","skina","skinella","tina","toka","trekella","trekina","troka","vild","yra","zina","zora"]
        return choice(elements)

    def clanpre(self):
        elements = ["ale","anvil","armor","axe","beard","black","cavern","earth","flame","foe","forge","goblin","gold","granite","grudge","hammer","iron","oath","orc","ore","red","ring","rock","shield","silver","steel","stone","tunnel","troll"]
        return choice(elements)

    def clansuff(self):
        elements = ["arm","axe","back","bane","beard","bearer","beater","bender","binder","breaker","crusher","cutter","delver","fist","forge","hammer","head","hearth","keg","killer","maker","master","render","shaker","slayer","smith","splitter","worker"]
        return choice(elements)


#x = generator()
#print x.start()