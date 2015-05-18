from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = self.prefix() + self.suffix()
        return result.title()

    def prefix(self):
        elements = ["abbots","ald","alder","ale","altar","amaranth","anchor","ancient","ape","arch","archers","ard","arden","argent","arrach","arrow","ash","ashen","aulden","avens","ax","axe","axe","bad","bald","barb","bards","barrel","barren","barrow","battle","bottle","bay","bear","beard","beast","beer","berry","birch","bitter","black","blacken","blessed","blood","blue","boar","bold","bolt","bone","bottle","boulder","bow","bowyers","bracken","brand","bridge","bright","broken","bronze","brown","buck","bull","burr","by","cairn","candle","carn","castle","cave","cavern","cat","cell","chapel","claw","clear","cloud","cloudy","cold","copper","court","crag","crown","cruel","crypt","crystal","damp","dank","dark","dawn","dead","death","deeping","deer","demon","devil","dim","dire","doe","dog","dour","dove","dragon","dragons","drip","dry","duck","dun","dungeon","dusk","dust","dwarf","dwarven","dying","east","eld","elder","elf","elk","elm","elven","emerald","enchanted","eternal","ettin","ever","faerie","fair","fairy","fall","fang","fell","fey","flame","flat","fog","forest","four","fox","free","frog","frost","game","gargoyles","gem","ghast","ghost","ghoul","glacier","glad","glade","glass","glen","glitter","giants","gilt","goblin","gods","gold","gore","grand","great","green","grey","grim","grim","gryphons","guardian","guild","hag","hale","hallow","hammer","hard","hawks","haze","heather","hell","helm","helms","high","hill","hoar","hobbits","hock","holy","horn","horse","howling","hundred","hydra","ice","icy","ild","in","inns","iron","ivy","jade","jewel","jewelled","juniper","just","keep","key","kings","knife","knights","lake","lakes","lance","leaf","leech","lich","lichen","lightning","lions","little","lizard","lone","lonely","long","lords","lore","loud","lurk","lynx","mace","mad","mage","maid","maiden","mere","mid","middle","mill","mirk","mist","misty","mold","morning","moss","mossy","mould","mound","mud","muck","murk","murky","narrow","necromancers","night","nine","noble","nor","north","northron","nymph","oak","oaken","ochre","ogre","old","olden","ooze","orc","owl","ox","pack","pale","pine","placid","priest","prince","pure","queens","queer","quiet","rain","rainy","ranger","ravens","red","ring","river","roaring","rogues","rope","rot","rotten","round","ruby","rush","rust","rye","sabre","sacred","salt","sand","satyr","savage","scarlet","scrub","scyth","sea","serpent","serpents","shade","shadow","shaft","shield","short","silent","silven","silver","skel","skull","slate","sleeping","slime","small","smoke","snake","snow","snowy","sorcerers","sorrow","south","southern","sparrow","spear","spectre","spell","spider","stag","staff","star","stave","still","stirge","stone","stone","stones","storm","stormy","stout","strangle","straw","sullen","sun","sunder","sweet","sword","tame","thief","theives","three","toad","torch","tower","travellers","tree","troll","trolls","true","two","ul","umber","under","unholy","valley","victors","vile","vine","waggon","wander","wanderers","wardens","warder","warriors","wasp","wax","weasel","web","wedge","well","wench","were","west","westron","wet","white","wierd","wild","wilder","willow","wind","winder","windy","winter","wise","witch","withered","wizards","woe","wolf","wood","worm","wraith","wyrm","wyrms","yellow","yew"]
        return choice(elements)

    def suffix(self):
        elements = ["castle"," castle"," citadel"," cities","city"," city","cross","gard","guard","ford","fort"," fort","ham","haven"," haven","inn"," inn","keep"," keep","port"," port","ton","tower"," tower"," towers","town"," town"]
        return choice(elements)

#x = generator()
#print x.start()