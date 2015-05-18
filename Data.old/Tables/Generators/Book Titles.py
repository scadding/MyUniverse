from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        roll = randint(1,10)
        groups = {  1: self.religion(),
                    2: self.travelling(),
                    3: self.biology(),
                    4: self.magic(),
                    5: self.craftwork(),
                    6: self.tales(),
                    7: self.art(),
                    8: self.historical(),
                    9: self.geography(),
                    10: self.linguistics(),
        }

        result = groups[roll]

        return result

    def religion(self):
        groups = {  1: "Life of Saint " + self.names(),
                    2: self.relthings().capitalize() + " to the " + self.domain().capitalize() + " " + self.rel().capitalize(),
                    3: self.adj().capitalize() + " " + self.things().capitalize() + " of the " + self.domain().capitalize() + " " + self.rel().capitalize(),
                    4: self.things().capitalize() + " of the " + self.domain().capitalize() + " " + self.rel().capitalize(),
        }
        return groups[randint(1,4)]

    def travelling(self):
        return self.travel().capitalize() + " of " + self.names()

    def biology(self):
        result = ''
        if randint(1,2) == 1:
            result = self.bio().capitalize() + " of the " + self.creature().capitalize()
        else:
            result = self.creature().capitalize() + " " + self.things().capitalize()
        return result

    def magic(self):
        groups = {  1: self.adj() + " " + self.arcana() + " " + self.things(),
                    2: self.arcana() + " " + self.things(),
                    3: self.adj() + " " + self.things() + " " + self.regarding() + " " + self.arcana(),
                    4: self.things() + " " + self.regarding() + " " + self.arcana(),
        }
        
        return groups[(randint(1,4))].title()
            
    def craftwork(self):
        return self.guide().capitalize() + " " + self.crafts().capitalize()

    def tales(self):
        groups = {  1: self.story().capitalize() + " of the " + self.adj2().capitalize() + " " + self.place().capitalize(),
                    2: self.story().capitalize() + " of the " + self.adj2().capitalize() + " " + self.person().capitalize(),
                    3: self.story().capitalize() + " of the " + self.adj3().capitalize() + " " + self.person().capitalize(),
        }
        return groups[randint(1,3)]

    def art(self):
        return self.poettype().capitalize() + " " + self.poetry().capitalize()        
        
    def historical(self):
        groups = {  1: self.history().capitalize() + " of the " + self.histtype().capitalize(),
                    2: self.adj().capitalize() + " " + self.things().capitalize() + " of the " + self.histtype().capitalize(),
                    3: self.things().capitalize() + " of the " + self.histtype().capitalize(),
                    4: self.fame().capitalize() + " " + self.people().capitalize() + " of the " + self.histtype().capitalize(),
        }
        return groups[randint(1,4)]

    def geography(self):
        groups = {  1: self.adj2().capitalize() + " " + self.geo().capitalize() + " Atlas",
                    2: self.geothings().capitalize() + " of the " + self.adj2().capitalize() + " " + self.geo().capitalize(),
                    3: self.activity().capitalize() + " in the " + self.geo().capitalize(),
        }
        return groups[randint(1,3)]

    def linguistics(self):
        result = ''
        roll = randint(1,2)
        if roll == 1:
            result = self.language().capitalize() + " - " + self.language().capitalize() + " Dictionary"
        else:
            result = self.dictionary().capitalize() + " of " + self.language().capitalize() + " " + self.words().capitalize()
            
        return result

    def things(self):
        elements = ["fables","legends","lore","mysteries","myths","powers","rites","rituals","sagas","secrets","strictures","tales","traditions"]
        return choice(elements)

    def relthings(self):
        elements = ["appeals","champions","hymns","litanies","orders","petitions","prayers","relics","saints","shrines","symbols","temples"]
        return choice(elements)

    def domain(self):
        elements = ["air","chaos","death","destruction","earth","evil","fire","good","knowledge","law","love","moon","nature","protection","strength","sun","travel","trickster","war"]
        return choice(elements)

    def rel(self):
        elements = ["cult","cults","deities","deity","divinities","divinity","faith","faiths","god","goddess","goddesses","gods","religion","religions"]
        return choice(elements)

    def crafts(self):
        elements = ["alchemy","assassination","baking","barrelmaking","basketmaking","brewing","carpentry","cobbling","cooking","dancing","fishing","fletching","gemcutting","herbalism","leatherworking","locksmithing","masonry","milling","painting","poisons","ropemaking","sculpting","sewing","smithing","tanning","weaving"]
        return choice(elements)

    def travel(self):
        elements = ["travels","journeys","voyages"]
        return choice(elements)

    def geo(self):
        elements = ["caverns","desert","forest","hills","moors","mountains","plains","swamp"]
        return choice(elements)

    def geothings(self):
        elements = ["animals","climate","hazards","herbs","history","monsters","people","riches","ruins","settlements","tales","weather"]
        return choice(elements)

    def dictionary(self):
        elements = ["dictionary","glossary","lexicon"]
        return choice(elements)

    def words(self):
        elements = ["letters","phrases","terms","words"]
        return choice(elements)

    def bio(self):
        elements = ["bestiary","biology","dietary habits","ecology","mating habits"]
        return choice(elements)

    def arcana(self):
        elements = ["abjuration","alchemy","conjuration","divination","evocation","illusion","magic","runecraft","thaumaturgy","transmutation","enchantment","necromancy","sorcery"]
        return choice(elements)

    def guide(self):
        elements = ["book of","compendium on","guide to","manual of","writings on"]
        return choice(elements)

    def regarding(self):
        elements = ["about","concerning","regarding"]
        return choice(elements)

    def history(self):
        elements = ["architecture","armies","art","beliefs","cities","cuisine","customs","dances","heraldry","history","music","languages","rulers","ships","trade","treasures","wars","weapons"]
        return choice(elements)

    def language(self):
        elements = ["abyssal","common","deep speech","draconic","dwarven","elven","giant","goblin","primordial","supernal"]
        return choice(elements)

    def story(self):
        elements = ["legend","stories","tales"]
        return choice(elements)

    def poetry(self):
        elements = ["ballads","idylls","poems","poetry","songs","verses"]
        return choice(elements)

    def poettype(self):
        elements = ["ancient","celestial","elven","erotic","hopeful","inspirational","merry","mournful","mystic","rousing"]
        return choice(elements)

    def histtype(self):
        result = ''
        elements = ["Ancient World","Barbarian Plains","Chaos Lands","Desert Tribes","Eastern Kingdoms","Empire","Island Kingdoms","Mountain Clans","Northern Cultures","Southern Regions","Western Realms","#NonHuman#"]
        result = choice(elements)
        
        if (result == "#NonHuman#"):
            result = self.nonhuman()

        return result

    def nonhuman(self):
        elements = ["Dragons","Dwarves","Elves","Giants","Gnomes","Orcs","Trolls","Underworld"]
        return choice(elements)

    def fame(self):
        elements = ["acclaimed","exalted","famous","infamous","legendary","notable","obscure","renowned"]
        return choice(elements)

    def people(self):
        elements = ["adventurers","champions","heroes","nobles","priests","sages","warriors","wizards"]
        return choice(elements)

    def person(self):
        elements = ["alchemist","angel","archer","bandit","banshee","bard","baron","beggar","brigand","champion","cleric","dancer","druid","drunkard","dryad","dwarf","elf","enchanter","enchantress","fisherman","fool","friar","gambler","giant","goblin","guard","gypsy","hag","hangman","harlot","harpy","hermit","hero","highwayman","horseman","hunter","huntress","jester","king","knave","knight","lady","lord","maiden","merchant","mermaid","minotaur","minstrel","mistress","monk","nomad","nymph","ogre","pilgrim","pirate","plowman","priest","priestess","prince","queen","ranger","rogue","sage","sailor","shepard","smith","sorcerer","squire","stranger","thief","traveler","troll","virgin","vixen","wanderer","warrior","watchman","wench","witch","wizard"]
        return choice(elements)

    def adj(self):
        elements = ["esoteric","false","forbidden","forgotten","hidden","lost","mystical","sacred","secret","unholy","unknown","veiled"]
        return choice(elements)

    def adj2(self):
        elements = ["ancient","black","blue","brass","brown","chaotic","copper","crimson","dark","dirty","dripping","enchanted","fearsome","frosty","frozen","glass","glorious","golden","great","green","grey","imperial","invisible","iron","ivory","jade","last","lawful","magical","majestic","mystic","odd","old","pale","red","royal","sacred","scarlet","secret","shining","silent","silver","singing","steel","stone","sturdy","twisted","whistling","white","wooden"]
        return choice(elements)

    def adj3(self):
        elements = ["adulterous","amorous","angry","arcane","avenging","bald","bearded","beefy","bitter","blind","bloated","boastful","brave","brawny","brazen","bumbling","burly","buxom","charming","clever","crazy","crusty","curious","dancing","dead","dreaming","drooling","drunken","eager","fallen","fat","fell","fiery","fighting","flying","foolish","foppish","gay","gentle","greedy","grim","handsome","hanging","happy","hearty","honest","horned","horny","howling","humble","hungry","jealous","jolly","lame","laughing","lazy","leaping","lucky","lusty","mad","merry","mighty","naughty","noble","prancing","proud","roaring","rowdy","rude","salty","savage","shady","slaughtered","sleeping","sneezing","stalwart","stout","thirsty","timid","towering","virtuous","vulgar","wandering","wanton","weary","wise","zealous"]
        return choice(elements)

    def activity(self):
        elements = ["fighting","hunting","mining","surviving","trave"]
        return choice(elements)

    def names(self):
        elements = ["Amanapa","Arannash","Aspin","Astarte","Avilion","Celindaria","Crystanor","Dalis","Denudiel","Derenome","Doniri","Dwinian","Egildas","Elargir","Eldraz","El-Wisbin","Eninope","Findrillian","Foobar","Fordulana","Hirondelle","Ignius","Isrid","Jinarf","Lagwyn","Loriel","Margwas","Melpomene","Meridian","Mitzi","Mortick","Mystrene","Nilf","Ornill","Pildar","Prodge","Quailandir","Ravaniof","Repetitios","Rhyidon","Rhyndis","Sidiar","Spiridon","Splinders","Spork","Syniara","Tiffany","Trisilyan","Valdison","Vermopolis","Vevrissan","Widgil","Wrastforth","Xandiri","Zwind"]
        return choice(elements)

    def creature(self):
        elements = ["ass","bear","beast","boar","bull","cat","chicken","chimera","cock","cow","crab","demon","devil","dog","dragon","drake","duck","eagle","falcon","fox","frog","gargoyle","goat","griffon","hare","hawk","hog","horse","hound","lamb","lion","lizard","mare","monster","owl","ox","phoenix","pig","pony","rat","raven","rooster","serpent","shadow","shark","sheep","siren","snake","stag","stallion","swine","unicorn","vulture","wolf","wyrm","wyvern"]
        return choice(elements)

    def place(self):
        elements = ["battlement","bridge","castle","cave","cove","crag","crypt","den","domain","dungeon","gate","hall","haven","hideout","hill","house","inn","keep","labyrinth","lair","landing","manor","nest","oasis","pit","rest","river","road","rookery","roost","sanctum","shrine","star","temple","throne","tomb","tower","valley","vault","wood"]
        return choice(elements)




#x = generator()
#print x.start()