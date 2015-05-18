from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        return self.truename().capitalize() + ' (' + self.usename() + ')'
        
    def truename(self):
        result = ''
        roll = randint(1,100)
        num = 0
        
        if (roll <= 8):
            num = 1
        elif (roll <= 18):
            num = 2
        elif (roll <= 29):
            num = 3
        elif (roll <= 42):
            num = 4
        elif (roll <= 56):
            num = 5
        elif (roll <= 71):
            num = 6
        elif (roll <= 79):
            num = 7
        elif (roll <= 86):
            num = 8
        elif (roll <= 92):
            num = 9
        elif (roll <= 96):
            num = 10
        elif (roll <= 99):
            num = 11
        else:
            num = 12

        for x in range(num):
            result = result + self.tnelements()
            
        return result

    def tnelements(self):
        groups = {  1: self.tn1(),
                    2: self.tn2(),
                    3: self.tn3(),
                    4: self.tn4(),
                    5: self.tn5(),
                    6: self.tn6(),
        }
        return groups[randint(1,6)]

    def tn1(self):
        elements = ["a","er","fol","n'n","thl","an","e","il","rh","yy"]
        return choice(elements)

    def tn2(self):
        elements = ["cog","kw","mm","th","ak","du","i","ow","y","ch"]
        return choice(elements)

    def tn3(self):
        elements = ["fl","pp","sh","ae","dh","hl","oo","uu","io","ff"]
        return choice(elements)

    def tn4(self):
        elements = ["ll","z","abl","dh","hh","oe","ul","bh","eu","ks"]
        return choice(elements)

    def tn5(self):
        elements = ["ss","aa","de","gz","oa","u","ar","eo","ir","q'"]
        return choice(elements)

    def tn6(self):
        elements = ["cc","da","g'g","o","tl","ao","ee","il","ph","zh"]
        return choice(elements)

    def usename(self):
        return (self.name() + ' ' + self.name()).title()
        
    def name(self):
        return self.unelement() + self.unelement()
        
    def unelement(self):
        groups = {  1: self.un1(),
                    2: self.un2(),
                    3: self.un3(),
                    4: self.un4(),
                    5: self.un5(),
                    6: self.un6(),
                    7: self.un7(),
                    8: self.un8(),
                    9: self.un9(),
                    10: self.un10(),
        }
        return groups[randint(1,10)]

    def un1(self):
        elements = ["blue","suck","chew","sword","canker","rut","mark","grab","vile","worm","maim","doom","break","rheum","lewd","fiend","spurt","spot","howl","bite"]
        return choice(elements)

    def un2(self):
        elements = ["dangle","thigh","dog","wrack","eat","skull","red","gristle","whip","blech","moulder","foul","dread","scum","plague","gnash","throb","warp","lust","claw"]
        return choice(elements)

    def un3(self):
        elements = ["dreg","bane","gibber","cackle","fester","spike","spider","helm","blunt","bog","pinch","grin","eye","smut","slake","grasp","war","bend","man","filth"]
        return choice(elements)

    def un4(self):
        elements = ["fondle","carnal","gnaw","fang","flux","tremble","thrash","loon","drink","buttock","scratch","loose","thrust","tear","squeeze","hack","beetle","blight","ooze","glutton"]
        return choice(elements)

    def un5(self):
        elements = ["grind","fiddle","grope","hammer","glut","vomit","bag","pest","gall","crush","slobber","putrid","fury","twist","whine","lick","craze","bowel","rotten","kill"]
        return choice(elements)

    def un6(self):
        elements = ["grunt","hot","maul","mildew","hate","wind","blade","puke","gross","fire","spew","slob","grue","water","ash","nibble","flesh","clap","sinew","pain"]
        return choice(elements)

    def un7(self):
        elements = ["mad","mucus","offal","rot","ichor","brute","cold","rip","maggot","froth","stare","sting","heart","blast","beast","pierce","gore","fat","slug","scrape"]
        return choice(elements)

    def un8(self):
        elements = ["pox","sate","pus","toad","leper","dung","death","sharp","rabid","gobble","wart","wither","loath","cut","chaos","reap","lip","flush","spoor","spine"]
        return choice(elements)

    def un9(self):
        elements = ["quiver","vex","spasm","bile","mire","glop","face","spume","sore","grim","wobble","axe","mange","foam","crab","scab","pile","fume","venom","wail"]
        return choice(elements)

    def un10(self):
        elements = ["slash","bubo","spittle","blister","rend","gut","fist","sweat","taint","liver","blood","black","quake","green","drool","spite","sin","gob","wight","burble"]
        return choice(elements)




#x = generator()
#print x.start()