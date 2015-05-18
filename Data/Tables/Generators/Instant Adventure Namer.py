from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        groups = {1: self.action().capitalize() + ' of the ' + (self.monster() + ' ' + self.ruler()).title(),
                  2: (self.adjective() + ' ' + self.building()).title() + ' of ' + self.deity(),
                  3: (self.adjective() + ' ' + self.building()).title() + ' of ' + self.name(),
                  4: (self.adjective() + ' ' + self.place()).title() + ' of ' + self.name(),
                  5: self.adjective().capitalize() + ' ' + self.item().capitalize() + ' of ' + self.name(),
                  6: self.building().capitalize() + ' of ' + (self.adjective() + ' ' + self.element()).title(),
                  7: self.building().capitalize() + ' of ' + self.element().capitalize(),
                  8: self.building().capitalize() + ' of ' + self.name(),
                  9: self.building().capitalize() + ' of the ' + (self.creature() + ' ' + self.ruler()).title(),
                  10: self.building().capitalize() + ' of the ' + (self.color() + ' ' + self.ruler()).title(),
                  11: self.building().capitalize() + ' of the ' + self.monster().title(),
                  12: self.building().capitalize() + ' of the ' + (self.monster() + ' ' + self.ruler()).title(),
                  13: self.building().capitalize() + ' on ' + (self.creature() + ' ' + self.geography()).title(),
                  14: self.building().capitalize() + ' on the ' + self.region(),
                  15: self.building().capitalize() + 's of the ' + self.monster().title() + ' ' + self.ruler().capitalize() + 's',
                  16: (self.color() + ' ' + self.item() + ' ' + self.geography()).title(),
                  17: self.escape().capitalize() + ' from ' + self.name() + "'s " + self.building().capitalize(),
                  18: self.escape().capitalize() + ' from ' + self.name() + ' ' + self.building().capitalize(),
                  19: self.geography().title() + ' of ' + self.element().capitalize(),
                  20: self.geography().title() + ' of the ' + (self.monster() + ' ' + self.ruler()).title(),
                  21: self.item().capitalize() + ' of ' + self.deity(),
                  22: self.item().capitalize() + ' of ' + self.element().capitalize(),
                  23: self.journey().capitalize() + ' the ' + self.region(),
                  24: self.journey().capitalize() + ' ' + self.name() + ' ' + self.place().capitalize(),
                  25: (self.monster() + ' ' + self.place()).title() + ' of ' + self.name(),
                  26: self.name() + ' ' + self.building().capitalize(),
                  27: self.people().capitalize() + ' of the ' + (self.adjective() + ' ' + self.place()).title(),
                  28: self.people().capitalize() + ' of ' + self.element().capitalize(),
                  29: self.person().capitalize() + ' of ' + self.name() + ' ' + self.geography().title(),
                  30: self.place().capitalize() + ' of ' + self.name(),
                  31: self.place().capitalize() + ' of the ' + (self.color() + ' ' + self.item()).title(),
                  32: self.place().capitalize() + ' of the ' + self.monster().title(),
                  33: (self.person() + ' ' + self.place()).title() + ' of the ' + self.place().capitalize(),
                  34: self.quest() + ' ' + self.adjective().capitalize() + ' ' + self.item().capitalize(),
                  35: self.quest() + ' ' + self.adjective().capitalize() + ' ' + self.place().title(),
                  36: self.quest().replace(' the','') + ' ' + self.name() + "'s " + self.item().capitalize(),
                  37: self.ruler().capitalize() + ' of the ' + (self.monster() + ' ' + self.place()).title(),
                  38: self.secret().capitalize() + ' of the ' + (self.people() + ' ' + self.building()).title(),
                  39: self.secret().capitalize() + ' of the ' + self.adjective().capitalize() + ' ' + self.item().capitalize(),
                  40: self.secret().capitalize() + ' of ' + (self.item() + ' ' + self.geography()).title(),
                }
            
        result = groups[randint(1,40)]
        
        return result

    def action(self):
        elements = ["attack","fury","revenge","vengeance"]
        return choice(elements)

    def adjective(self):
        elements = ["#color#","dark","elemental","forbidden","forgotten","ghost","hidden","lost","nightmare","sinister","unknown","sunless"]
        result = choice(elements)

        if (result == "#color#"):
            result = self.color()
        
        return result

    def building(self):
        elements = ["abbey","citadel","forge","hall","keep","manor","maze","palace","shrine","steading","stockade","temple","tomb","tower","vault"]
        return choice(elements)

    def color(self):
        elements = ["black","blue","gold","green","red","silver","white"]
        return choice(elements)

    def creature(self):
        elements = ["ass","bear","beast","boar","bull","cat","chicken","chimera","cock","cow","crab","demon","devil","dog","dragon","drake","duck","eagle","falcon","fox","frog","gargoyle","goat","griffon","hare","hawk","hog","horse","hound","lamb","lion","lizard","mare","monster","owl","ox","phoenix","pig","pony","rat","raven","rooster","serpent","shadow","shark","sheep","siren","snake","stag","stallion","swine","unicorn","vulture","wolf","wyrm","wyvern"]
        return choice(elements)

    def deity(self):
        elements = ["Demogorgon","Freya","Odin","Orcus","Tharizdun"]
        return choice(elements)

    def element(self):
        elements = ["air","blood","chaos","darkness","death","desolation","doom","earth","evil","fire","fury","good","horrors","light","madness","order","power","water"]
        return choice(elements)

    def escape(self):
        elements = ["escape","flight"]
        return choice(elements)

    def geography(self):
        elements = ["canyon","desert","forest","glacial rift","hill","lake","mountain","river","sea"]
        return choice(elements)

    def item(self):
        elements = ["bones","crucible","gold","grimoire","helm","palm","pearl","plume","scepter","spear","sword","tome"]
        return choice(elements)

    def journey(self):
        elements = ["descent into","expedition to","journey into","voyage to"]
        return choice(elements)

    def monster(self):
        elements = ["#creature#","demon","drow","Fire Giant","Frost Giant","Hill Giant","kuo-toa","vampire"]
        result = choice(elements)
        
        if (result == "#creature#"):
            result = self.creature()

        return result

    def name(self):
        elements = ["Inverness","Saltmarsh","Tamoachan","Tegel","Tsojcanth","Amanhir","Anargon","Anargon","Bergoth","Bergoth","Cardandir","Cardandir","Earaldir","Earaldir","Eogard","Eogard","Estarion","Estarion","Haldor","Haldor","Hathhelm","Hathhelm","Heruos","Heruos","Lorlost","Lorlost","Lorwise","Lorwise","Marast","Marast","Morgil","Morgil","Morwyn","Morwyn","Pelgorn","Pelgorn","Saeron","Saeron","Sarathar","Sarathar","Sereden","Sereden","Tirith"]
        return choice(elements)

    def people(self):
        elements = ["cultists","druids","dwellers","followers","masters","slavers","warriors","wizards"]
        return choice(elements)

    def person(self):
        elements = ["necromancer","slave","sorcerer","thrall","warlock","witch"]
        return choice(elements)

    def place(self):
        elements = ["caverns","caves","city","lair","mazes","mines","oasis","pits","undercity","wastes"]
        return choice(elements)

    def quest(self):
        elements = ["In Search of the","Quest for the","Search for the"]
        return choice(elements)

    def region(self):
        elements = ["Barrier Peaks","Borderlands","Depths of the Earth"]
        return choice(elements)

    def ruler(self):
        elements = ["god","jarl","king","lord","master","queen"]
        return choice(elements)

    def secret(self):
        elements = ["mystery","riddle","secret"]
        return choice(elements)




#x = generator()
#print x.start()