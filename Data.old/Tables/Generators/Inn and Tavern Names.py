from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        roll = randint(1,2)
        result = "The " + self.name()
        if (roll == 2):
            result = result + " " + self.bldg()        
        return result.title().replace("'S","'s").replace("And","and")
            
    def name(self):
        result = ""
        
        groups = {1: self.adj() + " " + self.creature(),
                  2: self.adj() + " " + self.creature(),
                  3: self.adj2() + " " + self.creature(),
                  4: self.adj() + " " + self.item(),
                  5: self.adj3() + " " + self.item(),
                  6: self.adj() + " " + self.person(),
                  7: self.adj2() + " " + self.person(),
                  8: self.adj() + " " + self.place(),
                  9: self.creature() + " and " + self.creature(),
                  10: self.creature() + "'s " + self.item(),
                  11: self.creature() + "'s " + self.item2(),
                  12: self.creature() + "'s " + self.place(),
                  13: self.item() + " and " + self.item(),
                  14: self.person() + " and " + self.person(),
                  15: self.person()+ "'s " + self.item(),
                  16: self.person() + "'s " + self.place(),
        }

        result = groups[randint(1,16)]
        return result

    def place(self):
        elements = entries=["battlement","bridge","castle","cave","cove","crag","crypt","den","domain","dungeon","gate","hall","haven","hideout","hill","house","inn","keep","labyrinth","lair","landing","manor","nest","oasis","pit","rest","river","road","rookery","roost","sanctum","shrine","star","temple","throne","tomb","tower","valley","vault","wood"]
        return choice(elements)

    def bldg(self):
        elements = entries=["alehouse", "inn", "pub", "tavern"]
        return choice(elements)

    def adj(self):
        elements = entries=["ancient","black","blue","brass","brown","chaotic","copper","crimson","dark","dirty","dripping","enchanted","fearsome","frosty","frozen","glass","glorious","golden","great","green","grey","imperial","invisible","iron","ivory","jade","last","lawful","magical","majestic","mystic","odd","old","pale","red","royal","sacred","scarlet","secret","shining","silent","silver","singing","steel","stone","sturdy","twisted","whistling","white","wooden"]
        return choice(elements)

    def creature(self):
        elements = entries=["ass","bear","beast","boar","bull","cat","chicken","chimera","cock","cow","crab","demon","devil","dog","dragon","drake","duck","eagle","falcon","fox","frog","gargoyle","goat","griffon","hare","hawk","hog","horse","hound","lamb","lion","lizard","mare","monster","owl","ox","phoenix","pig","pony","rat","raven","rooster","serpent","shadow","shark","sheep","siren","snake","stag","stallion","swine","unicorn","vulture","wolf","wyrm","wyvern"]
        return choice(elements)

    def person(self):
        elements = entries=["alchemist","angel","archer","bandit","banshee","bard","baron","beggar","brigand","champion","cleric","dancer","druid","drunkard","dryad","dwarf","elf","enchanter","enchantress","fisherman","fool","friar","gambler","giant","goblin","guard","gypsy","hag","hangman","harlot","harpy","hermit","hero","highwayman","horseman","hunter","huntress","jester","king","knave","knight","lady","lord","maiden","merchant","mermaid","minotaur","minstrel","mistress","monk","nomad","nymph","ogre","pilgrim","pirate","plowman","priest","priestess","prince","queen","ranger","rogue","sage","sailor","shepard","smith","sorcerer","squire","stranger","thief","traveler","troll","virgin","vixen","wanderer","warrior","watchman","wench","witch","wizard"]
        return choice(elements)

    def adj3(self):
        elements = entries=["battered","broken","cracked","dusty","empty","heavy","rusty","satin","shattered","shimmering","shiny","stolen","tarnished","weathered"]
        return choice(elements)

    def item2(self):
        elements = entries=["beak","bite","claw","fang","scales","snout","tail","talon","wing"]
        return choice(elements)

    def adj2(self):
        elements = entries=["adulterous","amorous","angry","arcane","avenging","bald","bearded","beefy","bitter","blind","bloated","boastful","brave","brawny","brazen","bumbling","burly","buxom","charming","clever","crazy","crusty","curious","dancing","dead","dreaming","drooling","drunken","eager","fallen","fat","fell","fiery","fighting","flying","foolish","foppish","gay","gentle","greedy","grim","handsome","hanging","happy","hearty","honest","horned","horny","howling","humble","hungry","jealous","jolly","lame","laughing","lazy","leaping","lucky","lusty","mad","merry","mighty","naughty","noble","prancing","proud","roaring","rowdy","rude","salty","savage","shady","slaughtered","sleeping","sneezing","stalwart","stout","thirsty","timid","towering","virtuous","vulgar","wandering","wanton","weary","wise","zealous"]
        return choice(elements)

    def item(self):
        elements = entries=["anchor","anvil","arrow","axe","banner","barrel","bell","blade","bodice","bone","boot","bottle","bow","buckle","candle","cap","cauldron","cave","chalice","cloak","coin","crest","crown","crystal","cup","dagger","dress","eye","fist","flame","flask","flute","folly","garter","gate","gauntlet","gem","glass","glove","goblet","hammer","harp","head","helm","helmet","hood","horn","jewel","keg","kettle","key","kiss","lamp","lantern","mask","mirror","moon","mug","portal","pride","quest","ring","rose","rune","secret","shield","sigil","skirt","skull","song","soup","spear","spell","spike","staff","stone","sword","tale","tankard","thorn","throne","tongue","tooth","torch","treasure","trinket","trophy","wagon","wheel","whip","whistle"]
        return choice(elements)




#x = generator()
#print x.start()