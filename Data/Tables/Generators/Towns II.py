from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = ""
        
        roll = randint(1,4)
        if roll == 1:
            result = self.affix() + " " + self.prefix() + self.suffix()
        else:
            result = self.prefix() + self.suffix()
            
        return result.title()
    
    def affix(self):
        elements = ["east","fort","high","lake","low","mount","new","north","old","port","south","west"]
        return choice(elements)

    def prefix(self):
        elements = ["abbey","abbots","aber","ad","amber","ard","arrow","ash","ashen","autumn","ballin","bards","barren","barrow","battle","bear","beech","bel","bell","bella","ben","berry","birch","black","blue","bright","bryn","bur","caer","cam","carn","carrick","cart","castle","cedar","cherry","cold","cooper","copper","coving","crick","crook","crystal","dark","dart","day","deer","den","diamond","dragon","dun","eagle","eal","east","edin","elk","elm","emerald","fair","far","fir","fitz","free","glass","glen","gold","grand","great","green","grey","hack","hagers","hamp","har","hart","hav","hazel","high","hodge","horn","hunt","inns","inver","iron","ivy","jack","kent","key","kill","king","kings","knights","lake","lis","loch","long","low","lyn","mal","man","maple","may","mill","miller","monk","nether","new","night","nor","north","oak","odd","old","pen","pine","port","queens","ram","rath","red","rich","ross","royal","ruby","rush","scot","silver","south","spring","stark","stock","summer","sun","tower","tre","way","well","west","white","wick","win","winter","wood","york"]
        return choice(elements)

    def suffix(self):
        elements = ["abbey","ant","arbor","bank","bluff","bourne","bridge","brook","burg","bury","by","caster","chester","cord","cot","crest","dale","dell","den","don","down","dun","end","ey","fall","field","ford","fort","gard","garth","gate","glen","green","grove","ham","haven","head","hill","hold","hurst","ia","ing","ington","keep","lake","lan","land","ley","lin","march","mark","marsh","mead","mere","minster","mond","mont","moor","more","mount","mouth","park","point","port","ridge","river","shire","stead","stock","ston","ter","ton","town","tree","vale","view","ville","wall","water","way","well","wich","wick","wood","worth"]
        return choice(elements)




#x = generator()
#print x.start()