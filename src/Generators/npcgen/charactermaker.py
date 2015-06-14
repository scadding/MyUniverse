#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: charactermaker.py,v $
 * $Date: 2009-08-12 10:45:10 -0500 (Wed, 12 Aug 2009) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * Main engine; entry point for command-line.
 *
 *******************************************************************************
"""

import sys, os, sha
from pprint import PrettyPrinter
from random import choice
from os.path import basename

from ClassData import ClassType, ItemTable, AttributeDependencyMap
from NPCData import GetNPC, NPCBuilder
from XMLHandlers import ParseAllXML
from Utils import GetRootDir, kTimeZoneDifference

# import functions to wrap
from ClassNames import EmptyNamesCache, ReplaceNamesCache
from XMLHandlers import GetRepositoryDataMap

 
# static stuff 
kCycleCacheSize = 20
kRootDir = GetRootDir()
kNPCImagesDir = 'images/pics'
kNPCImagesPath = '%s/%s' % (kRootDir, kNPCImagesDir)
kExcludedCodeFiles = {}


def EmptyNamesCacheMain():
    EmptyNamesCache()

def ReplaceNamesCacheMain(cache):
    ReplaceNamesCache(cache)


def GetRepositoryDataMapMain():
    'return map of data:mdate'
    dataMap = GetRepositoryDataMap()
    for fileName in os.listdir(kNPCImagesPath):
        if fileName.lower().endswith('gif'):
            fullPath = '%s%s%s' % (kNPCImagesPath, '/', fileName)
            filePath = '%s%s%s' % (kNPCImagesDir, '/', fileName)
            dataMap[filePath] = [os.path.getmtime(fullPath) - kTimeZoneDifference, GetShaSum(fullPath)]
    #print dataMap
    return dataMap



def GetShaSum(spath):
    shahex = ''
    if os.path.isfile(spath):
        sfile = file(spath, 'rb')
        contents = sfile.read()
        sfile.close()
        shaobj = sha.new(contents)
        shahex = shaobj.hexdigest()
    return shahex


def GetRepositoryCodeMap():
    'return map of codefile.py:mdate'
    codeMap = {}
    for fileName in os.listdir(kRootDir):
        lowerfile = fileName.lower()
        if (lowerfile.endswith('py') or lowerfile.endswith('html') or lowerfile.endswith('psp')) and \
           (lowerfile not in kExcludedCodeFiles):
            fullPath = '%s%s%s' % (kRootDir, '/', fileName)
            codeMap[fileName] = [os.path.getmtime(fullPath) - kTimeZoneDifference, GetShaSum(fullPath)]
    return codeMap


def GetXMLData(reload=0):
    attribmap        = AttributeDependencyMap()
    weapons            = ItemTable()
    armor            = ItemTable()
    classes            = {}
    genreMap        = {}
    nameMap            = {}

    # parse all class files; populate classes, weapons
    ParseAllXML(attribmap, classes, genreMap, weapons, armor, nameMap)
    data = (attribmap, classes, genreMap, weapons, armor, nameMap)

    return data


def GetCharacters(xmlData, classAlias, power, start, total):
    'return n unique character(s) from static xmlData'
    characters = []

    # process total # i
    for i in range(start, total):
        # initialize character and add to set of characters
        if classAlias[0] == '*':
            alias = GetRandomClassAlias(xmlData, classAlias)[1]
        else:
            alias = classAlias
        npc = GetNPC(xmlData, alias, power, i)
        characters.append(npc)

    return characters
        

def PrintHelp(appPath, exitCode=-1):
    print '\n Usage:    %s [alias|"name"|*genre] [#] [a2] [#2] [a(n)...] [#(n)...]\n' % basename(appPath)
    print '\n   Example:    %s as 1' % basename(appPath)
    print '\n   Example:    %s "American Soldier" 1' % basename(appPath)
    print '\n   Example:    %s *fantasy 100' % basename(appPath)
    print '\n   Example:    %s pb' % basename(appPath)
    print '\n   Example:    %s ss 5 sso 2' % basename(appPath)
    print '\n\n   To see list of aliases:  %s alias' % basename(appPath)
    print '\n   GUI:    NPCGen.exe'
    # sys.exit(exitCode)


def GetRandomClassAlias(xmlData, genreFilter):
    'get a random short class alias based on genreFilter!!!'
    attribmap, classes, genreMap, weapons, armor, nameMap = xmlData
    filteredClasses = []
    genreName = 'Random'

    if genreFilter == '*random':
        genres = genreMap.keys()
    else:
        genreName = genreFilter[1:]
        genres = [genreName]

    # populate filteredClasses looping thru genres 
    for genre in genres:
        classTypes = genreMap[genre]

        for classType in classTypes:
            if not classType.isAbstract():
                classAlias = classType.getAlias()
                filteredClasses.append(classAlias)

    return (genreName, choice(filteredClasses))


def Generate(args, xmlData, power=1, mode=0, startPos=0, stack=[], display=1, html=0):
    'main function to control cycles of character gen.'
    classAlias = args[0]
    if type(classAlias) != type(u''): classAlias = unicode(args[0], 'latin1')
    try:
        total = int(args[1])
    except (ValueError, IndexError):
        total = 1

    # get total number of cycles
    cycles, remainder = divmod(total, kCycleCacheSize)

    runningTotal = startPos
    for i in range(cycles + 1):
        # get characters for this cycle
        characters = []
        if cycles and i != cycles:
            characters = GetCharacters(xmlData, classAlias, power, runningTotal, (runningTotal + kCycleCacheSize))
            runningTotal += kCycleCacheSize
        elif (i == cycles) and remainder:
            characters = GetCharacters(xmlData, classAlias, power, runningTotal, (runningTotal + remainder))

        # print every character in this cycle or remainder
        for character in characters:
            stack.append(character)
            if display:
                if html == 0:
                    NPCBuilder.DisplayText(character, mode)
                else:
                    print NPCBulder.GetHTMLDisplay(character)
                    #NPCBuilder.DisplayHTML(character, mode)

    if len(args) > 2:
        Generate(args[2:], xmlData, power, mode)

def GenerateHTMLString(args, xmlData, power=1, mode=0, startPos=0, stack=[]):
    'main function to control cycles of character gen.'
    classAlias = args[0]
    if type(classAlias) != type(u''): classAlias = unicode(args[0], 'latin1')
    try:
        total = int(args[1])
    except (ValueError, IndexError):
        total = 1

    # get total number of cycles
    cycles, remainder = divmod(total, kCycleCacheSize)

    runningTotal = startPos
    for i in range(cycles + 1):
        # get characters for this cycle
        characters = []
        if cycles and i != cycles:
            characters = GetCharacters(xmlData, classAlias, power, runningTotal, (runningTotal + kCycleCacheSize))
            runningTotal += kCycleCacheSize
        elif (i == cycles) and remainder:
            characters = GetCharacters(xmlData, classAlias, power, runningTotal, (runningTotal + remainder))

        # print every character in this cycle or remainder
        for character in characters:
            stack.append(character)
            return NPCBuilder.GetHTMLDisplay(character)



def PrintAliasMap(xmlData):
    'print a classAliasMap'
    attribmap, classes, genreMap, weapons, armor, nameMap = xmlData
    aliases = classes.keys()
    aliases.sort()

    for alias in aliases:
        classType = classes[alias]
        if not classType.isAbstract():
            className = classType.getName()
            screenString = '%-9s: %s' % (alias, className)
            # cp437 mac_latin2
            print screenString.encode('cp437')

    genreNames = genreMap.keys()
    genreNames.sort()
    print '\n*random          : Completely Random NPC'
    for genreName in genreNames:
        print '*%-16s: Random %s NPC' % (genreName, genreName)



def Main(args):
    # load the xmlData ONCE
    xmlData = GetXMLData()
    html = 0
    if args[1] == 'html':
        args = args[1:]
        html = 1

    if args[1] == 'alias':
        PrintAliasMap(xmlData)
    elif args[1].lower() in ('h', 'help', '/?', '/h', '-?', '-h', '--help'):
        PrintHelp(args[0])
    else:
        Generate(args[1:], xmlData, power=1, mode=0, html=html)

if __name__ == '__main__':
    args = sys.argv
    if len(args) > 1:
        Main(args)
    else:
        PrintHelp(args[0]) 

"""
 *******************************************************************************
 * $Log: charactermaker.py,v $
 * Revision 1.14  2004/08/21 22:20:44  andrew
 * fix winzip timezone bug
 *
 * Revision 1.13  2004/02/22 18:33:23  andrew
 * fix bug: only look for 'gif' images when syncing
 *
 * Revision 1.12  2004/02/22 18:04:45  andrew
 * changes to support dynamic NPC pics
 *
 * Revision 1.11  2004/01/12 02:33:16  andrew
 * refactor to support new iewin version
 *
 * Revision 1.10  2003/10/21 16:20:27  andrew
 * changes to support rich HTML display via PSP servlet
 *
 * Revision 1.9  2003/10/16 17:13:48  andrew
 * refactor a bit to support NPCGen servlet
 *
 * Revision 1.8  2003/10/06 15:22:13  andrew
 * new npc stack, so we can traverse a buffer of npc's
 *
 * Revision 1.7  2003/09/15 23:43:31  andrew
 * attribmap can be based on dict type
 *
 * Revision 1.6  2003/07/14 18:46:09  ayinger
 * use dict for excluded code files for faster lookup
 *
 * Revision 1.5  2003/07/11 23:53:46  ayinger
 * add support for passing in complete class names from CLI
 *
 * Revision 1.4  2003/07/11 20:51:27  ayinger
 * add RCS tags
 *
 *******************************************************************************
"""
