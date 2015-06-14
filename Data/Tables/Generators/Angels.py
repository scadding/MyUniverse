from random import choice

class generator:
    def version(self):
        return 1.0
    
    def start(self):
        result = self.prefix() + self.suffix()
        return result.capitalize()
        
    def prefix(self):
        elements = ["a", "aba", "ach", "acha", "ad", "adon", "af", "ag", "ahad", "akat", "ali", "ama", "amab", "amal", "amat", "amb", "amit", "an", "anaf", "anan", "anon", "anth", "ar", "ara", "arath", "ard", "arm", "armis", "as", "asa", "ash", "ataph", "atha", "att", "au", "auph", "az", "aza", "azr", "ba", "bab", "bah", "bahr", "bar", "bara", "barb", "barc", "bard", "bed", "beda", "behe", "behem", "bel", "beth", "bethe", "cab", "cadm", "cal", "cam", "camb", "caph", "cas", "cast", "char", "charb", "chas", "cher", "cheru", "cora", "cruc", "dag", "dal", "dan", "din", "don", "duc", "elim", "eloh", "elom", "ened", "esc", "ez", "eze", "fam", "frac", "gab", "gal", "gala", "gam", "gar", "garr", "gas", "gazard", "ge", "gedar", "ghedor", "gon", "grad", "hab", "had", "hada", "hadak", "hakam", "hal", "halud", "ham", "hama", "han", "hana", "har", "harr", "hars", "has", "hati", "hiz", "hus", "iad", "ici", "id", "im", "in", "ir", "ira", "is", "ish", "isha", "ishi", "ism", "ith", "ithu", "ja", "jan", "jehu", "jehud", "jhud", "joph", "kab", "kad", "kam", "ke", "kem", "keph", "khe", "khem", "ky", "kyr", "lamad", "lamar", "lamed", "lar", "laz", "machi", "machid", "machr", "maha", "mahar", "mal", "mala", "malad", "mat", "me", "mel", "mela", "mem", "meta", "mi", "mich", "mid", "mo", "mor", "mora", "nach", "naha", "nan", "naoph", "nar", "nem", "nema", "nemi", "nith", "omn", "oph", "pag", "pas", "path", "pha", "phal", "phan", "phar", "qa", "qad", "qal", "qam", "qan", "ra", "rac", "rach", "ram", "raph", "ras", "re", "reg", "rha", "rham", "rig", "sac", "sam", "sama", "samar", "samm", "san", "saph", "sar", "sav", "sava", "savu", "seda", "sedak", "sede", "sek", "seke", "sen", "ser", "sera", "seraph", "sha", "shak", "shal", "shar", "shat", "she", "soph", "su", "sun", "sur", "tah", "taha", "tahar", "tam", "tar", "tem", "temp", "tha", "thar", "the", "ther", "tub", "tur", "tzad", "tzaph", "tzed", "tzeph", "u", "uba", "uma", "ur", "uv", "uz", "ve", "ver", "verc", "yah", "yaha", "yas", "yash", "yep", "yeph", "za", "zac", "zach", "zad", "zah", "zaha", "zahar", "zak", "zal", "zam", "zaph", "zav", "zec", "zech", "zeph", "zeth", "zoph", "zu"]
        return choice(elements)
        
    def suffix(self):
        elements = ["ach", "ael", "arel", "az", "bel", "don", "el", "has", "hiel", "him", "iah", "iel", "im", "kiel", "las", "liel", "lim", "lym", "mon", "nach", "niel", "nim", "phas", "phlas", "quel", "quiel", "riel", "ros", "tiel", "tron", "uel", "viel", "yah", "ziel"]
        return choice(elements)

#x = generator()
#print x.start()
