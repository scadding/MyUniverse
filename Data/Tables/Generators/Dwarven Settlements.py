from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0
    
    def start(self):
        result = ''
        roll = randint(1,10)
        
        if roll <=4:
            result = 'The ' + self.community() + ' of ' + self.part_1() + self.part_2()
        else:
            result = self.part_1() + self.part_2() + ' ' + self.community()
        
        return result
        
    def community(self):
        elements = ["City","Town","Village","Mines","Deep","Delving"]
        return choice(elements)
        
    def part_1(self):
        elements = ["Stone","Granite","Deep","Iron","Rock","Silver","Crystal"]
        return choice(elements)

    def part_2(self):
        elements = ["hold","home","delve","deep","mine","fast"]
        return choice(elements)


#x = generator()
#print x.start()