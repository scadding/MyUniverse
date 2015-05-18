from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        roll = randint(1,7)
        result = ''

        if (roll == 1):
            result = self.group().capitalize() + ' of the ' + self.adj().capitalize() + ' '
            if (randint(1,2)== 1):
                result = result + self.obj().capitalize()
            else:
                result = result + self.objplural().capitalize()
        elif (roll == 2):
            result = self.adj().capitalize() + ' ' + self.objplural().capitalize()
        elif (roll == 3):
            result = self.adj().capitalize() + ' ' + self.obj().capitalize() + ' ' + self.group().capitalize()
        elif (roll == 4):           
            result = self.group().capitalize() + ' of the ' + self.obj().capitalize()
        elif (roll == 5):            
            result = self.obj().capitalize() + ' ' + self.group().capitalize()
        elif (roll == 6):            
            result = self.adj().capitalize() + ' ' + self.group().capitalize()
        else:
            result = self.adj().capitalize() + ' ' + self.group().capitalize() + ' of the '
            if (randint(1,2)== 1):
                result = result + self.obj().capitalize()
            else:
                result = result + self.objplural().capitalize()
                
        return result

    def adj(self):
        elements = ["accursed","arcane","astral","azure","benevolent","black","blessed","blue","brass","brave","bronze","burning","celestial","cerulean","chromatic","copper","courageous","crimson","crystal","cursed","dark","diamond","divine","emerald","eternal","ethereal","fallen","glass","glorious","gold","golden","green","grey","hidden","holy","imperial","infernal","invincible","iron","jade","just","legendary","lost","magic","magical","malevolent","merciful","mighty","mystic","mystical","oak","obsidian","opal","orange","pure","purple","red","royal","ruby","sacred","sapphire","scarlet","secret","silver","steel","stone","unholy","veiled","white","yellow"]
        return choice(elements)

    def obj(self):
        elements = ["arrow","axe","bat","bear","blade","bull","castle","chalice","city","cloud","crescent","cross","crow","crown","cup","dagger","dark","dawn","dragon","drake","dusk","eagle","eye","falcon","fist","flame","forest","forge","fox","gate","gauntlet","gem","griffin","hand","hart","hawk","heart","helm","hill","horn","hound","king","knife","knight","light","lion","lotus","moon","mountain","night","ocean","queen","raven","ring","river","rose","rune","scorpion","scroll","sea","serpent","shadow","shard","shield","skull","spider","staff","stag","star","storm","sun","sword","tome","tower","town","truth","village","wall","wind","wizard","wolf","word","wyrm","wyvern"]
        return choice(elements)

    def objplural(self):
        result = self.obj()

	pluralform = {"city": "cities",
		    "cross": "crosses",
		    "fox": "foxes",
		    "knife": "knives",
		    "lotus": "lotuses",
		    "staff": "staves",
		    "wolf": "wolves",
		    }
	
	if result in pluralform:
	    result = pluralform[result]
	else:
	    result = result + "s"

        return result

    def group(self):
        elements = ["alliance","band","brotherhood","brothers","cabal","circle","clan","compact","company","coven","covenant","cult","defenders","fellowship","fighters","guardians","guards","guild","heroes","horde","house","hunters","keepers","knights","masters","order","protectors","raiders","regulars","ring","seekers","servants","sisterhood","sisters","society","tribunal","trinity","wanderers","warriors","watchers","way"]
        return choice(elements)




#x = generator()
#print x.start()