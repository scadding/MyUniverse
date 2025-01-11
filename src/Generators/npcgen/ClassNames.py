#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: ClassNames.py,v $
 * $Date: 2008-03-25 17:15:39 -0500 (Tue, 25 Mar 2008) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * Static, nationality/gender-based name generator.
 *
 *******************************************************************************
"""

import sys, os, codecs
from random import choice

from src.Generators.npcgen.ExoticNames import GetExoticName
from src.Generators.npcgen.Utils import GetRootDir, RandInt

# static stuff
kRootDir                        = GetRootDir()
kNamesCache                        = {}
kuBOM                            = str


def GetName(type, nationality, gender, nameMap):
    'try to retrieve an NPC name from the kNamesCache'
    # get filePath from nameMap (based on type and gender match)
    for nameData in nameMap[nationality]:
        if (nameData['type'] == type) and (nameData['gender'] == gender):
            filePaths = nameData['file-paths']
            pathInfix = nameData['path-infix']
            break

    # ensure relevant filePaths:[data] are loaded into the cache
    PopulateNamesCacheFromFilePaths(filePaths, pathInfix, kNamesCache)

    # ensure non-empty name and return
    name = unicode('', 'utf-8')
    while not name:
        name = choice(kNamesCache[choice(filePaths)])
    return name


def EmptyNamesCache(cache=kNamesCache):
    'purge the names cache (used on full reload)'
    cache.clear()


def ReplaceNamesCache(serverCache):
    'replace the static cache; used when loading data straight from an RPC server'
    kNamesCache.clear()
    kNamesCache.update(serverCache)


def PopulateNamesCacheFromFilePaths(filePaths, pathInfix, cache):
    'system function: given a set of filePaths, populate the cache provided'
    for filePath in filePaths:
        if filePath not in cache:
            fullPath = '%s%s%s%s%s' % (kRootDir, os.sep, pathInfix, os.sep, filePath)
            namefile = codecs.open(fullPath, 'rU', 'utf-8')
            names = []
            for i, namePair in enumerate(namefile):
                if (i == 0) and namePair.startswith(kuBOM): namePair = namePair.lstrip(kuBOM)
                for name in namePair[:-1].split(', '):
                    names.append(name)
            cache[filePath] = names


def GetFullNamesCache(nameMap):
    'return lNamesCache; used by rpc server to return a full cache of ALL available names (on the server) to the client'
    lNamesCache = {}
    for nationality in nameMap:
        for nameData in nameMap[nationality]:
            filePaths = nameData['file-paths']
            pathInfix = nameData['path-infix']
            PopulateNamesCacheFromFilePaths(filePaths, pathInfix, lNamesCache)
    return lNamesCache


def GetFullName(nationality, genderRatio, nameMap):
    'return a random first and last name based on nationality and gender'
    gender = 'male'
    if RandInt(1, 100) > genderRatio:
        gender = 'female'

    if nationality == 'exotic':
        fullName = GetExoticName()
    else:
        fullName = '%s %s' % (GetName('first', nationality.lower(), gender, nameMap), GetName('last',  nationality.lower(), gender, nameMap))

    return (fullName, gender)



# "ISO-8859-1"  cp437 cp850 cp1252
if __name__ == '__main__':
    from charactermaker import GetXMLData
    nameMap = GetXMLData()[-1]
    args = sys.argv
    encoding = 'cp437'
    if len(args) > 1:
        total = int(args[1])
    else:
        total = 4
    for nationality in ('spanish',):
        #'english', 'various', 'american', 'chinese', 'spanish'):
        print
        for i in range(total):
            name, gender = GetFullName(nationality, 50, nameMap)
            print('%-12s: %s [%s]' % (nationality, name.encode(encoding, 'replace'), gender))
            #print name.encode(encoding), gender
            #print name, gender



