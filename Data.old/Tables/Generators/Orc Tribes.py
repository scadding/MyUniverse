from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''

        roll = randint(1,7)
        
        if (roll <= 4):
            result = self.adj() + ' ' + self.thing()
        elif (roll <= 6):
            result = self.adj() + ' ' + self.group()
        else:
            result = self.thing() + ' ' + self.group()    

        return result.title()
        
    def adj(self):
        elements = ["black", "bleeding", "blighted", "bloody", "blue", "broken", "burning", "chaos", "cracked", "crooked", "crushed", "dark", "dead", "death", "doom", "dread", "dripping", "evil", "flaming", "flayed", "gouged", "green", "grey", "grim", "jagged", "plague", "red", "rotten", "rotting", "scorched", "shadow", "shredded", "slashing", "storm", "swamp", "thorned", "torn", "vile", "yellow"]
        return choice(elements)

    def group(self):
        elements = ["bearers", "crushers", "destroyers", "hunters", "ones", "raiders", "slayers"]
        return choice(elements)

    def thing(self):
        elements = ["axe", "blade", "blood", "bone", "cave", "claw", "cleaver", "crow", "dagger", "eye", "fang", "fist", "flesh", "knife", "mace", "maggot", "moon", "rat", "rock", "rune", "scourge", "scream", "skull", "snake", "spear", "spike", "stone", "swamp", "talon", "thorn", "tongue", "wolf"]
        return choice(elements)




#x = generator()
#print x.start()
