#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: parentsTree.py,v $
 * $Date: 2008-03-25 17:15:39 -0500 (Tue, 25 Mar 2008) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * ParentsTree UI support classes, for displaying NPC Template data in tree.
 *
 *******************************************************************************
"""

import os
import wx

from Utils import GetRootDir
from ClassData import ClassType
from NPCExceptions import InfiniteLoopError


class TreeClassData(wx.TreeItemData):
    'treeItem container for classType data'
    def __init__(self, classType):
        wx.TreeItemData.__init__(self, classType)

    def GetId(self):
        'get the unique id of the data in this treeItem ... basically the memory address of data'
        return id(self.GetData())


# static functions

def GetImageList():
    'static convenience function for getting all of the images to be rendered by the tree control'
    wx.Image_AddHandler(wx.PNGHandler())
    rootDir            = GetRootDir()

    imageList        = wx.ImageList(14, 16)
    imageDir        = rootDir + os.sep + 'images' + os.sep

    imageList.Add(wx.Bitmap(imageDir + 'NPC.png', wx.BITMAP_TYPE_PNG))
    imageList.Add(wx.Bitmap(imageDir + 'AFile.png', wx.BITMAP_TYPE_PNG))
    imageList.Add(wx.Bitmap(imageDir + 'File.png', wx.BITMAP_TYPE_PNG))
    imageList.Add(wx.Bitmap(imageDir + 'next.png', wx.BITMAP_TYPE_PNG))
    imageList.Add(wx.Bitmap(imageDir + 'BinaryNode.png', wx.BITMAP_TYPE_PNG))
    imageList.Add(wx.Bitmap(imageDir + 'BinaryData.png', wx.BITMAP_TYPE_PNG))
    imageList.Add(wx.Bitmap(imageDir + 'Branch.png', wx.BITMAP_TYPE_PNG))

    return imageList


def SortClassTypes(classTypes):
    'return sorted list of input classTypes'
    classTypeMap = {}

    for classType in classTypes:
        classTypeMap[classType.getName()] = classType

    classNames = classTypeMap.keys()
    classNames.sort()

    sortedClassTypes = []
    for className in classNames:
        sortedClassTypes.append(classTypeMap[className])

    return sortedClassTypes



class NPCTree(wx.TreeCtrl):
    'treeControl widget for displaying NPC template data'
    def __init__(self, parent, encoding, xmlData):
        wx.TreeCtrl.__init__(self, parent, -1, wx.DefaultPosition, wx.DefaultSize, wx.TR_HAS_BUTTONS)
        self.encoding = encoding
        wx.EVT_TREE_ITEM_EXPANDING(self, self.GetId(), self.onExpand)
        self.AssignImageList(GetImageList())
        self.populateRoot(xmlData)

    def populateRoot(self, xmlData):
        'populate all the root nodes in the tree'
        attribDepMap, classes, genreMap, weapons, armor, nameMap = xmlData
        self.classes = classes

        root = self.AddRoot("NPC Genres", 3)
        genres = genreMap.keys()
        genres.sort()
        for genre in genres:
            genreNode = self.AppendItem(root, genre, 6)

            classTypes = genreMap[genre]
            # FIX-ME: sort-hack
            for classType in SortClassTypes(classTypes):
                try:
                    # make sure we don't create tree node with infinite loop
                    classType.checkForInfiniteLoop(classes)
                    self.createRootClassType(genreNode, classType)
                except InfiniteLoopError, ex:
                    print 'Warning: infinite ancestor loop found in class %s' % ex.getClassName()

        self.Expand(root)


    def createRootClassType(self, parentNode, classType):
        'create an expandable root classType in the tree'
        classTypeNode = self.AppendItem(parentNode, classType.getName(), classType.isAbstract(), -1, TreeClassData(classType))
        dataNode = self.AppendItem(classTypeNode, '__data__', 2)


    def populateClassType(self, classTypeNode, classType):
        'populate all data for a given classType'
        # first, populate iData
        self.populateClassData(self.GetFirstChild(classTypeNode)[0], classType.iData)

        if classType.hasParents():
            # now, mData (only if parents? hmmm...)
            mdataNode = self.AppendItem(classTypeNode, '__inherited-data__', 2)
            self.populateClassData(mdataNode, classType.getData(self.classes))
            # add parents -- preserve parent inheritance order (do not sort)
            for parentClassType in classType.getParents(self.classes):
                self.createRootClassType(classTypeNode, parentClassType)

    def populateClassData(self, parentNode, dict):
        'recursively walk dict, appending name value pairs as nodes...'
        keys = dict.keys()
        keys.sort()

        for key in keys:
            if type(dict[key]) == type({}):
                dictNode = self.AppendItem(parentNode, key, 4)
                # go recursive
                self.populateClassData(dictNode, dict[key])
            else:
                keyPair = '%s: %s' % (key, dict[key])
                #if key == 'name':
                #    keyPair = keyPair.---de(self.---ding)
                dictNode = self.AppendItem(parentNode, keyPair, 5)


    def onExpand(self, event):
        nodeID = event.GetItem()
        classType = self.GetPyData(nodeID)
        dataNode, cookie = self.GetFirstChild(nodeID)
        if (classType and (classType.__class__ == ClassType)) and \
           (not self.ItemHasChildren(self.GetFirstChild(nodeID)[0])):
            self.populateClassType(nodeID, classType)


def CreateParentsTree(parent, encoding, xmlData):
    'anchor an NPCTree to parent frame and return it'
    tree = NPCTree(parent, encoding, xmlData)

    return tree

"""
 *******************************************************************************
 * $Log: parentsTree.py,v $
 * Revision 1.5  2003/08/01 02:05:28  ayinger
 * add doc strings
 *
 * Revision 1.4  2003/07/16 01:36:17  ayinger
 * refactor classType.mData inheritance population in classType.getData() --
 * this is effectively a contract post-condition: any time a client of classType calls
 * classType.getData(), he can be assured to receive data back that has been
 * populated with all ancestor data
 *
 * Revision 1.3  2003/07/14 22:29:27  ayinger
 * get use built-in method id() to get unique memory address
 *
 * Revision 1.2  2003/07/11 20:51:28  ayinger
 * add RCS tags
 *
 *******************************************************************************
"""
