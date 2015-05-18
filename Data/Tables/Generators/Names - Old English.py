from random import choice
from random import randint

class generator:
    def version(self):
        return 1.0

    def start(self):
        result = self.prefix().capitalize() + self.suffix()
        return result
        
    def prefix(self):
        elements = ["aelf","aelf","aethel","aethel","badu","beo","blith","blith","bregu","bregu","ceol","ceol","coen","cene","cuth","cud","cwic","cwic","dryct","drynt","ead","aead","eald","ald","ealh","alh","earcon","ercon","earn","earn","ecg","ec","eofor","eofor","eorcon","eorcon","eormen","yrmen","folc","folc","ford","ford","fri","fri","gold","gold","grim","grim","haem","haem","haeth","haeth","heah","heah","healf","healf","hreth","hreth","hroth","hroth","huaet","huaet","hyg","hugu","iaru","iaru","leof","leof","maegen","maegen","oidil","oidil","ongen","ongen","os","os","rath","rath","saex","sax","sele","sele","tat","tat","theod","theod","til","til","torct","torct","trum","trum","tun","tun","waeg","waeg","wig","wig","wil","wil"]
        return choice(elements)

    def suffix(self):
        elements = ["bald","balth","beorht","berct","beorn","bern","brand","brand","brod","brord","burg","burth","cyni","cynne","degn","degn","ferth","ferth","flaed","fled","for","for","frit","frid","gar","gar","geld","geld","gifu","geofu","gisil","gisil","gunnr","gyd","haed","hathu","heard","hard","here","heri","helm","helm","hilde","hilde","hun","hun","lac","lac","laf","laf","lid","lid","lind","linda","maere","maere","man","mon","mund","mund","noth","noth","raed","red","refu","refu","ric","ric","sig","sige","stan","stan","swith","swid","theof","theof","theow","theow","thryth","thryd","wealch","walh","weald","wald","weard","ward","wic","wic","wict","wiht","wine","wini","wiw","wiu","wuda","widu","wulf","ulf","wyn","wynn"]
        return choice(elements)




#x = generator()
#print x.start()