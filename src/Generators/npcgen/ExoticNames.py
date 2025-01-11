#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: ExoticNames.py,v $
 * $Date: 2008-03-25 17:15:39 -0500 (Tue, 25 Mar 2008) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * Custom 'yithogu-like' exotic name generator.
 *
 *******************************************************************************
"""

from random import choice

from src.Generators.npcgen.Utils import RandInt


# constants
kSyllableRange = frozenset((1, 2, 3))

kVowells = ('a', 'e', 'i', 'o', 'u', 'y', 'ae', 'ei', 'ou', 'au', 'ui', 'uo', 'oo', 'ee', 'ea', 'ie', 'ye', 'uy', 'ua', 'ue',
            'a', 'e', 'i', 'o', 'u', 'e', 'e', 'e', 'a', 'i', 'i', 'i')

kConsonants = ('b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z', 'qu', "t'", "c'", "k'", "r'")

kCommonConsonants = ('yt', 'b', 'c', 'd', 'f', 'g', 'k', 'n', 'p', 'q', 't', 'v', "c'", "t'")

kConsonantAddons = ('h', 'l', 'r', 's')

kSyllableMap =    {
                    6    :    (kVowells,),
                    16    :    (kConsonants, kVowells),
                    23    :    (kConsonants, kConsonantAddons, kVowells),
                    24    :    (kConsonants, kVowells, (' ',)),
                    29    :    (kConsonants, kConsonantAddons, kVowells, (' ',)),
                    42    :    (kCommonConsonants, kVowells),
                    52    :    (kCommonConsonants, kConsonantAddons, kVowells),
                    53    :    (kConsonants, kVowells, ('-',))
                }

def GetSyllable():
    roll = RandInt(1, 53)
    # increment roll until it is a dictionary key
    while not roll in kSyllableMap:
        roll += 1

    syllable = ''
    for each in kSyllableMap[roll]:
        syllable += choice(each)

    return syllable


def GetName():
    'just return a name element'
    syllables = RandInt(0, 2) + RandInt(1, 2)
    if syllables not in kSyllableRange:
        syllables = RandInt(0, 2) + RandInt(0, 1) + RandInt(1, 2)

    name = ''
    for i in range(syllables):
        name += GetSyllable()

    nameSet = name.split(' ')

    result = ''
    for name in nameSet:
        if name:
            result += name.capitalize() + ' '

    if result[-2] == '-':
        result = result[:-2]
    else:
        result = result[:-1]

    return unicode(result, 'UTF-8')


def GetExoticName():
    'return fullname, ok, fine, great'
    firstName = GetName()
    lastName  = GetName()
    if len(lastName) == 1:
        lastName  = GetName()
    return '%s %s' % (firstName, lastName)


if __name__ == '__main__':
    for n in range(24):
        print(GetExoticName())

"""
 *******************************************************************************
 * $Log: ExoticNames.py,v $
 * Revision 1.5  2003/10/09 00:36:59  andrew
 * create static cache of commonly used sets
 *
 * Revision 1.4  2003/10/08 18:41:54  andrew
 * use new set types for sequences that need hash() function
 *
 * Revision 1.3  2003/09/05 18:38:32  andrew
 * randrange should really be RandInt
 *
 * Revision 1.2  2003/07/11 20:51:28  ayinger
 * add RCS tags
 *
 *******************************************************************************
"""
