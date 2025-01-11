#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: XMLHandlers.py,v $
 * $Date: 2010-03-02 14:49:43 -0600 (Tue, 02 Mar 2010) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * All of the XMLHandlers, plus a single convenience method
 * to populate ClassData with XML data.
 *
 *******************************************************************************
"""

import os, os.path, sha
from sets import ImmutableSet
from xml.sax import handler, make_parser, parse, ContentHandler
from pprint import PrettyPrinter
from time import timezone

from ClassData import ClassType
from Utils import GetRootDir, kTimeZoneDifference


kServerTimeZoneOffset = 18000 # need this because winzip is annoying

# static paths
kRootDir                = GetRootDir()
kMetaConfigFilePath        = '%s%s%s%s%s' % (kRootDir, '/', 'cfg', '/', 'meta-config.xml')

# static tag Sets
kAttributeSetTypes    = ImmutableSet(('stats', 'attributes', 'advantages', 'disadvantages', 'quirk-verbs', 'quirk-nouns', 'skills', 'maneuvers', 'weapons', 'armor'))
kDependencyTags        = ImmutableSet(('attributes', 'skills', 'skill-mods'))
kMetaConfigKeyTags    = ImmutableSet(('class-file-sets', 'attribute-rules', 'weapons-data', 'armor-data', 'nationality-name-file-sets'))
kMetaConfigSetTags    = ImmutableSet(('class-file-sets', 'nationality-name-file-sets'))
kItemAttribTags        = ImmutableSet(('weapons', 'weapon-stats', 'weapon-mods', 'hand-weapon-mods', 'armor', 'armor-stats', 'armor-mods'))
kItemModTags        = ImmutableSet(('weapon-mods', 'armor-mods'))
kRangeTags            = ImmutableSet(('min', 'max'))


# custom xml handlers

class MetaConfigHandler(ContentHandler):
    'custom xml handler for meta-config file -- populates metaConfigMap'
    def __init__(self, metaConfigMap):
        self.mMetaConfigKey = ''
        self.mMetaConfigMap = metaConfigMap

    def startElement(self, tag, attrs):
        if tag in kMetaConfigKeyTags:
            self.mMetaConfigKey = tag
            if tag not in kMetaConfigSetTags:
                # this is some other (non-list) file type...
                localDict = {}
                for dataPair in attrs.items():
                    name, value = dataPair
                    localDict[name] = value

                self.mMetaConfigMap[tag] = localDict

        elif tag == 'files':
            # this is a set of class-data files
            localDict = {}
            for dataPair in attrs.items():
                name, value = dataPair
                if name == 'file-paths':
                    value = value.split(', ')
                localDict[name] = value

            if self.mMetaConfigKey in self.mMetaConfigMap:
                self.mMetaConfigMap[self.mMetaConfigKey].append(localDict)
            else:
                self.mMetaConfigMap[self.mMetaConfigKey] = [localDict]

        elif tag != 'xml-files':
            # this is a set of name data files
            localDict = {}
            for dataPair in attrs.items():
                name, value = dataPair
                if name == 'file-paths':
                    value = value.split(', ')
                localDict[name] = value

            if self.mMetaConfigKey not in self.mMetaConfigMap:
                self.mMetaConfigMap[self.mMetaConfigKey] = {}

            if tag in self.mMetaConfigMap[self.mMetaConfigKey]:
                self.mMetaConfigMap[self.mMetaConfigKey][tag].append(localDict)
            else:
                self.mMetaConfigMap[self.mMetaConfigKey][tag] = [localDict]


class ClassHandler(ContentHandler, handler.DTDHandler):
    'custom xml handler for character creation (automagically handles recursion, nesting, and multiple inhereitance of ClassTypes'
    def __init__(self, classes, genreMap, classFilePath):
        self.mClasses = classes
        self.mGenreMap = genreMap
        self.mClassFilePath = classFilePath
        self.mClassType = None
        self.mAttributeSetTag = ''
        self.mGenreKey = ''

    def startElement(self, tag, attrs):
        if tag == 'class':
            self.mClassType = ClassType(attrs.items(), self.mGenreKey, self.mClassFilePath)
            # add to classes maps
            self.mClasses[self.mClassType.getAlias()] = self.mClassType
            self.mGenreMap[self.mGenreKey].append(self.mClassType)
            self.mGenreMap['Random'].append(self.mClassType)
        elif tag in kAttributeSetTypes:
            # this is a set of attributes
            self.mAttributeSetTag = tag
            self.mClassType.addAttribSet(self.mAttributeSetTag, attrs.items())
        elif tag != 'classes':
            # this an attribute, the most primitive (at deepest xml level)
            self.mClassType.addAttrib(self.mAttributeSetTag, tag, attrs.items())
        else:
            # classes, top level definition
            for dataPair in attrs.items():
                name, value = dataPair
                if name == 'genre':
                    self.mGenreKey = value
                    if not self.mGenreKey in self.mGenreMap:
                        self.mGenreMap[self.mGenreKey] = []

    def skippedEntity(self, tag):
        print 'skippedEntity tag: %s' % tag

    def notationDecl(self, name, publicId, systemId):
        print 'notationDeclaration %s %s %s' % (name, publicId, systemId)

    def unparsedEntityDecl(self, name, publicId, systemId, ndata):
        print 'unparsedEntityDeclaration %s %s %s %s' % (name, publicId, systemId, ndata)


class AttributeDependencyHandler(ContentHandler):
    'custom xml handler for character creation'
    def __init__(self, attribDependencies):
        self.mAttributes = attribDependencies
        self.mAttributeSetTag = ''

    def startElement(self, tag, attrs):
        if (tag in kDependencyTags):
            # this is a set of attributes
            self.mAttributeSetTag = tag
            self.mAttributes.addAttribSet(tag, attrs.items())
        elif (tag != 'attribute-bases'):
            # this an attribute, the most primitive (at deepest xml level)
            self.mAttributes.addAttrib(self.mAttributeSetTag, tag, attrs.items())


class ItemHandler(ContentHandler):
    'custom xml handler for weapon/armor creation'
    def __init__(self, itemTable):
        self.mItemTable = itemTable
        self.mAttribute = ''
        self.mAttributeProperties = []
        self.mHandMod = 0

    def startElement(self, tag, attrs):
        if tag not in kItemAttribTags:
            # this an attribute, the most primitive (at deepest xml level)
            self.mAttribute = tag
            self.mAttributeProperties = attrs.items()
        elif tag in kItemModTags:
            self.mHandMod = 0
            for dataPair in attrs.items():
                name, value = dataPair
                if name == 'mod-inc':
                    self.mItemTable.mModInc = int(value)
        elif tag == 'hand-weapon-mods':
            self.mHandMod = 1

    def endElement(self, tag):
        # if at deepest level 
        if self.mAttribute:
            amount = None
            localDict = {}
            if self.mAttributeProperties:
                amount = min = max = wmod = 0
                for dataPair in self.mAttributeProperties:
                    name, value = dataPair
                    localDict[name] = value
                    if name in kRangeTags:
                        wmod = 1
                if wmod and self.mHandMod:
                    self.mItemTable.mHandItemMods[self.mAttribute] = localDict
                elif wmod:
                    self.mItemTable.mItemMods[self.mAttribute] = localDict
                else:
                    # item-stat
                    self.mItemTable.mItemStats[self.mAttribute] = localDict
            # clear mAttribute primitive and mAttributeProperties list
            self.mAttribute = ''
            self.mAttributeProperties = []



# static functions

def GetFilePathFromMap(configMap):
    'return a single path or list of paths from configMap passed in'
    attribFilePaths = []
    pathInfix = configMap['path-infix']

    if 'file-path' in configMap:
        basePaths  = [configMap['file-path']]
    else:
        # multi-path
        basePaths  = configMap['file-paths']

    for basePath in basePaths:
        attribFilePath = '%s%s%s%s%s' % (
                                            kRootDir,
                                            '/', pathInfix,
                                            '/', basePath
                                        )
        attribFilePaths.append(attribFilePath)

    if len(attribFilePaths) == 1:
        return attribFilePaths[0]
    else:
        return attribFilePaths


def GetShaSum(spath):
    shahex = ''
    if os.path.isfile(spath):
        sfile = file(spath, 'rb')
        contents = sfile.read()
        sfile.close()
        shaobj = sha.new(contents)
        shahex = shaobj.hexdigest()
    return shahex


def GetRepositoryDataMap():
    'return repositoryDateMap; used by rpc to compare file versions in client/server repositories'
    metaConfigMap = GetMetaConfigMap()
    metaNameKey = str(kMetaConfigFilePath[len(GetRootDir()) + 1:])
    repositoryDateMap = {metaNameKey: [os.path.getmtime(kMetaConfigFilePath) - kTimeZoneDifference, GetShaSum(kMetaConfigFilePath)]}
    for classFileSet in metaConfigMap['class-file-sets']:
        for classFilePath in GetFilePathFromMap(classFileSet):
            baseNameKey = str(classFilePath[len(GetRootDir()) + 1:])
            repositoryDateMap[baseNameKey] = [os.path.getmtime(classFilePath) - kTimeZoneDifference, GetShaSum(classFilePath)]
    for ruleType in ('attribute-rules', 'weapons-data', 'armor-data'):
        rulePath = GetFilePathFromMap(metaConfigMap[ruleType])
        baseNameKey = str(rulePath[len(GetRootDir()) + 1:])
        repositoryDateMap[baseNameKey] = [os.path.getmtime(rulePath) - kTimeZoneDifference, GetShaSum(rulePath)]

    nameMap = metaConfigMap['nationality-name-file-sets']
    for nationality in nameMap:
        for nameData in nameMap[nationality]:
            filePaths = nameData['file-paths']
            pathInfix = nameData['path-infix']
            for filePath in filePaths:
                baseNameKey = str(pathInfix + '/' + filePath)
                if baseNameKey not in repositoryDateMap:
                    repositoryDateMap[baseNameKey] = [os.path.getmtime(GetRootDir() + '/' + baseNameKey) - kTimeZoneDifference, GetShaSum(GetRootDir() + '/' + baseNameKey)]

    return repositoryDateMap


def GetMetaConfigMap():
    'load metaConfigMap by parsing the meta-config.xml, and return it'
    metaConfigMap = {}
    parse(kMetaConfigFilePath, MetaConfigHandler(metaConfigMap))
    return metaConfigMap


def ParseAllXML(attribmap, classes, genreMap, weapons, armor, nameMap):
    'parse all data in xml format ... populate the stuff passed in'
    # first parse metaconfig file
    metaConfigMap = GetMetaConfigMap()

    # populate nameMap metadata
    nameMap.update(metaConfigMap['nationality-name-file-sets'])

    # parse and populate attribmap 
    parse(GetFilePathFromMap(metaConfigMap['attribute-rules']), AttributeDependencyHandler(attribmap))

    # parse all class files; populate classes
    genreMap['Random'] = []
    for classFileSet in metaConfigMap['class-file-sets']:
        for classFilePath in GetFilePathFromMap(classFileSet):
            parse(classFilePath, ClassHandler(classes, genreMap, classFilePath))

    # parse weapons/armor and populate
    for itemPath, itemHandler in ((GetFilePathFromMap(metaConfigMap['weapons-data']), ItemHandler(weapons)),
                                  (GetFilePathFromMap(metaConfigMap['armor-data']),   ItemHandler(armor))):
        parse(itemPath, itemHandler)


if __name__ == '__main__':
    # first parse metaconfig file
    metaConfigMap = {}
    parse(kMetaConfigFilePath, MetaConfigHandler(metaConfigMap))

    # let's take a look
    pp = PrettyPrinter(indent=4)
    pp.pprint(GetRepositoryDateMap())

"""
 *******************************************************************************
 * $Log: XMLHandlers.py,v $
 * Revision 1.7  2004/08/21 22:20:44  andrew
 * fix winzip timezone bug
 *
 * Revision 1.6  2003/10/08 18:41:54  andrew
 * use new set types for sequences that need hash() function
 *
 * Revision 1.5  2003/09/16 00:41:03  andrew
 * cache all classTypes in 'Random' key (for now...)
 *
 * Revision 1.4  2003/07/14 22:53:10  ayinger
 * cleanup
 *
 * Revision 1.3  2003/07/14 22:32:52  ayinger
 * several performance enhancements and some re-factoring:
 *  - remove dead, commented code
 *  - use dict for static tag data structures where appropriate, for faster lookup
 *  - use new tree-structured attribmap (used to be a flat dict)
 *  - AttributeDependencyHandler populates attribmap as tree, instead of flat dict
 *
 * Revision 1.2  2003/07/11 20:51:28  ayinger
 * add RCS tags
 *
 *******************************************************************************
"""
