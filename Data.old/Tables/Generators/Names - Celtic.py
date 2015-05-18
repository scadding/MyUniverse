from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0
    
    def start(self):
        groups = {0: self.prefM() + self.midM() + self.suffM(),
                  1: self.prefF() + self.midF() + self.suffF() + " (f.)",
        }

        result = groups[randint(0,1)]
        
        return result
        
    def prefM(self):
        elements = ["Ad","Addr","Aeth","Air","Al","An","Ans","Ar","Bed","Bl","Bow","C","Cad","Cal","Car","Ced","Con","Cor","Cul","D","Dar","Der","Dev","Don","Dr","Dunh","Er","Ev","Ew","Ferg","Fin","G","Gl","Gw","Hal","Inn","K","Kel","Ken","L","M","Mer","Mor","Nol","Or","Ow","P","Pw","Qu","R","Rh","S","Sher","Sl","T","Tr","V","Yr"]
        return choice(elements)

    def midM(self):
        elements = ["a","ae","e","eo","i","o","u","y"]
        return choice(elements)
        
    def suffM(self):
        elements = ["air","al","am","an","ant","awn","bad","bryn","c","ce","cyn","dan","dd","ddry","ddyn","der","doc","don","dric","dry","dyn","ell","en","ey","gan","gar","gda","gh","git","gus","gwyn","gyle","hern","ine","lan","len","lin","llyn","loch","lyn","mac","man","myr","n","nall","nan","ney","nnyn","nry","nvan","nyc","r","ran","rcyn","ric","rol","roy","rraent","rry","rth","ryn","s","son","thur","tur","vin","well","wn","wyl","wyr"]
        return choice(elements)
        
    def prefF(self):
        elements = ["A","Al","Ar","Arl","Be","Birg","Br","C","Cl","Cord","D","D","Dag","De","Dor","El","Fi","Gw","Is","J","L","M","Mer","Morr","N","R","Row","S","Wyn","Ys"]
        return choice(elements)

    def midF(self):
        elements = ["a","ae","e","ea","i","o","u","y","w"]
        return choice(elements)
        
    def suffF(self):
        elements = ["cla","da","dra","ena","eve","gan","ghid","git","ld","lia","ll","lla","lona","lyan","lyra","n","na","ne","neve","nn","noic","ra","rdre","ria","ryan","ryla","ssa","t","tha","vona","vyan","wen"]
        return choice(elements)

#x = generator()
#print x.start()