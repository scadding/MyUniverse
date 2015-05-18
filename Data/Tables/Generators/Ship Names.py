from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ''
        roll = randint(1,6)

        if (roll == 1):
            result = "The " + self.adj() + " " + self.noun()
        elif (roll == 2):
            result = "The " + self.adj() + " " + self.person(0)
        elif (roll == 3):
            result = "The " + self.person(0) + "'s " + self.noun()
        elif (roll <= 5):
            result = "The " + self.noun()
        else:
            result = self.person(1)
            gender = result[-1]
            result = result[:-2]
            if (gender == "m"):
                result = result + " " + self.nameM()
            else:
                result = result + " " + self.nameF()
            result = result  + "'s " + self.noun()
        
        return result
    
    def person(self,saveGender):
        result = ''
        
        elements = ["Angel!f","Bandit!m","Baroness!f","Baron!m","Contessa!f","Count!m","Dervish!m","Devil!m","Duchess!f","Duke!m","Goddess!f","Gypsy!f","Harlot!f","Herald!m","Huntress!f","King!m","Knight!m","Lady!f","Lord!m","Maiden!f","Mariner!m","Mistress!f","Mother!f","Paladin!m","Princess!f","Prince!m","Queen!f","Tyrant!m","Vagabond!m","Virgin!f","Wanderer!m","Wench!f"]
        result = choice(elements)
        
        if saveGender == 0:
            result = result[:-2]

        return result

    def noun(self):
        elements = ["Angel","Arrow","Banshee","Bell","Blade","Blessing","Blossom","Bounty","Breeze","Challenge","Charity","Claw","Crown","Cutlass","Dagger","Dawn","Death","Defender","Delight","Demon","Desire","Destiny","Devil","Diamond","Discovery","Dog","Dove","Dragon","Drake","Dream","Eagle","Echo","Endeavor","Explorer","Eye","Faith","Falcon","Flower","Fortress","Fortune","Freedom","Fury","Gem","Ghost","Glory","Grace","Griffin","Griffon","Harmony","Haven","Hawk","Heart","Hope","Horizon","Hound","Hunter","Hydra","Ivy","Journey","Lance","Legacy","Lightning","Lion","Locust","Luck","Maiden","Mermaid","Mist","Monkey","Moon","Nightengale","Odyssey","Omen","Passion","Pearl","Phantom","Pigeon","Plague","Raider","Ravager","Raven","Reaper","Reaver","Revenge","Rose","Salamander","Sanctuary","Scimitar","Scorpion","Scourge","Serpent","Shadow","Skull","Song","Spirit","Spring","Stag","Star","Storm","Sun","Sunrise","Swan","Sword","Tempest","Terror","Threat","Tiger","Trader","Trident","Triumph","Unicorn","Vanguard","Venture","Victory","Vindicator","Voyager","Walrus","Warrior","Wave","Whisper","Wind","Winter","Witch","Wolf","Wyvern","Zephyr"]
        return choice(elements)

    def adj(self):
        elements = ["Angry","Ardent","Baneful","Beautiful","Bitter","Black","Bleeding","Blessed","Bloody","Blue","Bonney","Bountiful","Brazen","Coral","Crimson","Dancing","Dark","Defiant","Diamond","Dread","Dreadful","Drowned","East","Eastern","Elysian","Emerald","Exultant","Far","Fearless","Flying","Gallant","Ghostly","Glorious","Golden","Green","Happy","Horrible","Imperial","Infernal","Intrepid","Invincible","Iron","Ivory","Jolly","Joyous","Lightning","Loyal","Lucky","Merciful","Merciless","Merry","Midnight","Mighty","Night","Nimble","North","Northern","Old","Opal","Perilous","Phantom","Proud","Red","Royal","Sable","Salty","Sea","Shining","Silent","Silver","Singing","South","Southern","Spectral","Stalwart","Sweet","Swift","Tenacious","Terrible","Undaunted","Valiant","Vengeful","Vermillion","Vigilant","West","Western","White","Wicked","Wild","Wrathful","Writhing"]
        return choice(elements)

    def nameF(self):
        elements = ["Angela","Amelia","Anne","Belinda","Cassandra","Catherine","Elizabeth","Helene","Horatia","Isabella","Jane","Louise","Lucinda","Maria","Mary","Matilda","Sarah","Vanessa"]
        return choice(elements)

    def nameM(self):
        elements = ["Bernard","Bradford","Charles","Edward","Garrick","George","Griffith","Henry","James","John","Julian","Patrick","Paul","Richard","Reuben","Roger","Rupert","Simon","Stephen","Stuart","Timothy","Thomas","Wesley","William"]
        return choice(elements)




#x = generator()
#print x.start()