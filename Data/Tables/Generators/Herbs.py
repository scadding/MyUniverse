from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        roll = randint(1,14)        
        
        if (roll <= 5):
            result = self.pref() + self.appsuff()
        elif (roll <= 7):
            result = self.color() + " " + self.pref() + self.appsuff()
        elif (roll <= 9):
            result = self.color() + " " + self.misc()
        elif (roll <= 10):
            result = self.color() + self.suff()
        elif (roll <= 11):
            result = "St. " + self.name() + "'s " + self.misc()
        elif (roll <= 12):
            result = self.creature() + "'s " + self.misc()
        elif (roll <= 13):
            result = self.thing() + "'s " + self.misc()
        else:
            result = self.adj() + " " + self.misc()
            
        return result.title().replace("'S","'s")

    def appsuff(self):
        result = ''
        roll = randint(1,6)
        if roll <= 5:
            result = self.suff()
        else:
            result = self.suff() + self.suff()
        return result

    def adj(self):
        elements = ["bitter","burning","dwarf","fire","ice","master","night","old","snow","sweet","wild"]
        return choice(elements)

    def thing(self):
        elements = ["angel","bishop","corpse","demon","elf","hag","hangman","king","knight","lady","maiden","mountain","oak","queen","shepard","spring","virgin","winter","witch"]
        return choice(elements)

    def color(self):
        elements = ["black","blue","gold","green","purple","red","silver","white","yellow"]
        return choice(elements)

    def creature(self):
        elements = ["adder","boar","bull","cock","demon","devil","dragon","drake","eagle","falcon","fox","goat","hare","hawk","hind","hound","lion","owl","ox","ram","rat","raven","serpent","shadow","snake","stag","swine","wolf","wyrm","wyvern"]
        return choice(elements)

    def misc(self):
        elements = ["bane","beard","blood","bone","clover","crown","eye","feather","foot","fruit","grass","heart","leaf","nut","pepper","root","rose","seal","seed","spike","tail","thorn","tongue","weed","wort"]
        return choice(elements)

    def suff(self):
        elements = ["aas","aca","acca","acea","ach","age","ali","am","amom","amon","ana","ander","any","ard","bate","bena","berry","cana","cena","cess","cia","der","dock","drake","el","ell","eric","few","frey","gal","gana","gol","gon","gul","ian","iar","ica","icle","il","ilia","in","ind","indes","iper","ite","ley","lic","many","mile","mond","mony","mus","na","nel","net","nis","oe","ome","ony","ory","osa","osia","per","pias","ram","rans","rant","ranth","ray","rel","rind","rome","ron","rosia","row","rue","sali","se","seng","shade","sley","spur","tany","thal","ther","thin","url","us","usar","vil","vir","way","wort"]
        return choice(elements)

    def pref(self):
        elements = ["ab","aca","acon","ad","ado","ag","agri","al","aleth","alka","ama","amb","ani","ans","ar","aric","asara","ath","ati","bal","bar","bas","bel","bil","bor","bry","bur","caf","caff","cal","cala","cara","card","cel","chamo","cher","cinn","col","com","cori","cum","cur","dil","dit","echin","ed","el","eld","els","fen","fetha","fumi","gar","gin","hol","jan","jin","jinn","jun","kal","kel","kla","lar","lor","lot","lung","man","mari","marjer","mug","mull","nap","night","ore","par","pe","peri","ras","sad","saff","san","sav","sen","sera","sor","stra","tama","tamar","tarra","teph","thu","tum","val","valer","ver","wan","win","yar","zal","zu"]
        return choice(elements)

    def name(self):
        elements = ["Amanapa","Arannash","Aspin","Astarte","Avilion","Celindaria","Crystanor","Dalis","Denudiel","Derenome","Doniri","Dwinian","Egildas","Elargir","Eldraz","El-Wisbin","Eninope","Findrillian","Foobar","Fordulana","Hirondelle","Ignius","Isrid","Jinarf","Lagwyn","Loriel","Margwas","Melpomene","Meridian","Mitzi","Mortick","Mystrene","Nilf","Ornill","Pildar","Prodge","Quailandir","Ravaniof","Repetitios","Rhyidon","Rhyndis","Sidiar","Spiridon","Splinders","Spork","Syniara","Tiffany","Trisilyan","Valdison","Vermopolis","Vevrissan","Widgil","Wrastforth","Xandiri","Zwind"]
        return choice(elements)




#x = generator()
#print x.start()