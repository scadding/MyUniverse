#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $Id: UIClasses.py 214 2009-09-03 22:27:49Z andrew $
 * $URL: http://ayinger.no-ip.info/repos/npcgen/trunk/UIClasses.py $
 *
 * $Date: 2009-09-03 17:27:49 -0500 (Thu, 03 Sep 2009) $
 * $Author: andrew $
 *
 * Copyright (c) 2008  Andrew C. Yinger
 * All rights reserved.
 *
 * Description:
 *
 *   Helper classes for main UI frame in cmaker-gui.py
 *
 *******************************************************************************
"""

import string, sys, os, re
import sha

from random import choice
from traceback import format_exception
#from wx.Python.wx. import *
import wx
from xmlrpclib import ServerProxy, Binary, DateTime
from xml.parsers.expat import ExpatError
from zlib import compress, decompress
from time import sleep, time
from threading import Event, currentThread, Thread
from xml.sax._exceptions import SAXParseException
from collections import deque

from Utils import GetRootDir, URLLibTransport, SingletonMetaClass, kTimeZoneDifference
from PrintableTextControl import PrintableTextControl
from ClassNames import EmptyNamesCache
from Utils import GetElapsedTime, ReadWriteLock, GetSimpleHTML
from NPCConstants import kNPCGenCSSLink, kNPCGenHTMLDocType

# load the main NPC Engine Functions
import charactermaker



# Constants
kErrorLogFilePath            = '%s%s%s' % (GetRootDir(), os.sep, 'error.log')
kCustomTextRegex            = re.compile(r'(\[wx..+?:.+?\])(.+?)\1', re.M)
kMaxNPCStackSize            = 500
kMaxRecentNPCs                = 50
kDataRepositoryCheckInterval = 5 #FIX-ME: make this configurable?

# Font Constants
if os.name == 'posix':
    kFontSize = 11
else:
    kFontSize = 8


class RecentNPCs(deque):
    __metaclass__=SingletonMetaClass
    def __init__(self):
        self.poo = 1
    def add(self, npc):
        self.appendleft(npc)
        if len(self) > kMaxRecentNPCs: self.pop()


class DataRepositoryChecker:
    __metaclass__=SingletonMetaClass
    def __init__(self, ui):
        self.ui = ui
        self.currentDataRepositoryMap = charactermaker.GetRepositoryDataMapMain()
        # init. all of the event monitors
        self.dataLock = ReadWriteLock()

        # load the xml data for the first time
        self.getXMLData(1)

        # start the dataRepositoryChecker
        checker = Thread(target=self.checkThread)
        checker.setDaemon(1)
        checker.start()


    def getXMLData(self, reload=0):
        'when it is safe (i.e., no loaders are going) get the latest xmlData cache'
        if not reload:
            # we are just reading, check the event monitor...
            self.dataLock.acquireRead()

        if reload or not self.xmlCache:
            print '%-7s%s' % ('', '-] (Re)Loading XML Data...')
            try:
                self.xmlCache = charactermaker.GetXMLData()
                ui = self.ui
                ui.genreNatCache = {}
                ui.classNamesGenreMap = {}
                ui.classAliasMapCache = {}
                ui.dropDownListCache = []
            except SAXParseException, why:
                print str(why)
                print 'XML Data had Errors, FAILED to Reload'
            print '%-7s%s' % ('', '-] Done Loading XML Data.')

        if not reload:
            # done.  reset the event monitor
            self.dataLock.releaseRead()

        return self.xmlCache


    def checkThread(self):
        'check thread'
        while 1:
            sleep(kDataRepositoryCheckInterval)
            self.checkDataRespository()

    def checkDataRespository(self):
        'compare currentDataRepositoryMap with repository as it looks right now'
        start = time()

        lastRepository = self.currentDataRepositoryMap
        currentRepository = charactermaker.GetRepositoryDataMapMain()
        changedFiles = []
        for filePath, lastVersion in lastRepository.items():
            if type(lastVersion) == type([]): lastVersion = lastVersion[0]
            currentVersion = currentRepository[filePath]
            if type(currentVersion) == type([]): currentVersion = currentVersion[0]
            if (filePath in currentRepository) and (lastVersion < currentVersion):
                changedFiles.append(filePath.lower())

        # check changedFiles, signal loader Event if changes found...
        if changedFiles:
            print '%-7s%s' % ('', '-] Changes found in data repository...')
            # check the data lock
            self.dataLock.acquireWrite()

            # figure out what stuff to reload...
            for fileName in changedFiles:
                if fileName.endswith('.xml'):
                    print '%-9s%s %s: %s' % ('', '-]', 'new .xml files found', str(changedFiles))
                    self.getXMLData(1)
                    break
            for fileName in changedFiles:
                if fileName.endswith('.txt'):
                    print '%-9s%s %s: %s' % ('', '-]', 'new .txt files found', str(changedFiles))
                    EmptyNamesCache()
                    break
            self.currentDataRepositoryMap = currentRepository

            # done.  reset the event monitors
            self.dataLock.releaseWrite()
            print '%-7s%s: [%s]\n' % ('', '-] Finished updating data repository', GetElapsedTime(start))


class UIMixIn:
    'generic ui attributes and methods'
    mode = 1
    #encoding = 'ISO-8859-1'
    dropDownListCache = []
    xmlCache = ()
    genreNatCache = {}
    classNamesGenreMap = {}
    classAliasMapCache = {}
    lastShortClassName = ''

    def __init__(self):
        self.encoding = 'UTF-8'

    def getXMLData(self, reload=0):
        if reload or not self.xmlCache:
            #self.genreNatCache = {}
            self.xmlCache = charactermaker.GetXMLData()
            self.classNamesGenreMap = {}
            self.classAliasMapCache = {}
            self.dropDownListCache = []

        return self.xmlCache


    def getRelativePower(self, power):
        return round((((int(power) - 50) * .01) + 1), 2)

    def getNPCPower(self, npc):
        return int(round((npc.mData['power'] - 1) * 100) + 50)

    def getGenreNames(self):
        if not self.xmlCache:
            self.xmlCache = charactermaker.GetXMLData()
        attribmap, classes, genreMap, weapons, armor, nameMap = self.xmlCache
        names = []
        genres = genreMap.keys()
        genres.sort()
        for genre in genres:
            names.append(genre)

        # don't show 'Random'
        return names[1:]


    def getAllGenreNationalities(self, genre):
        if genre not in self.genreNatCache:
            if not self.xmlCache: self.xmlCache = charactermaker.GetXMLData()
            #self.mClasses[self.mClassType.getAlias()] = self.mClassType
            #self.mGenreMap[self.mGenreKey].append(self.mClassType)
            #classData = classType.getData(classes)
            #nationality = choice(classData['class']['nationality'].split(', '))
            attribmap, classes, genreMap, weapons, armor, nameMap = self.xmlCache
            gclasses = genreMap[genre]
            nationalities = set()
            for gclass in gclasses:
                classData = gclass.getData(classes)
                if 'nationality' in classData['class'] and not gclass.isAbstract():
                    nationalities.update(classData['class']['nationality'].split(', '))
            self.genreNatCache[genre] = nationalities
        return self.genreNatCache[genre]


    def getNationalities(self, wildcards=False, genre=None):
        # refactor?
        if not self.xmlCache:
            self.xmlCache = charactermaker.GetXMLData()
        attribmap, classes, genreMap, weapons, armor, nameMap = self.xmlCache
        names = []
        values = nameMap.keys()
        if genre and genre in genreMap: values = list(self.getAllGenreNationalities(genre))
        if wildcards:
            #if genre != 'exotic': values.append('exotic')
            if genre == 'fantasy': values.extend(['elf*', 'orc*', 'dwarf*'])
        values.sort()
        for value in values:
            names.append(value)

        return names


    def getTypeNames(self, genre=None):
        # fix me: put in UIClasses
        if not self.xmlCache:
            self.xmlCache = charactermaker.GetXMLData()
        attribmap, classes, genreMap, weapons, armor, nameMap = self.xmlCache
        classNames = []
        genres = genreMap.keys()
        if genre and genre in genreMap: genres = [genre]
        for genre in genres:
            if genre != 'Random':
                classTypes = genreMap[genre]
                for classType in classTypes:
                    if not classType.isAbstract():
                        classNames.append(classType.getName())
        classNames.sort()

        return classNames

    
    def getClassNames(self, xmlData, reload=0):
        'return array of class names to use in comboBox'
        if reload or not self.dropDownListCache:
            attribmap, classes, genreMap, weapons, armor, nameMap = xmlData
            classNames = []
            genres = genreMap.keys()
            genres.sort()

            for genre in genres:
                # append random genres!!!
                randomName        = '*Random %s' % genre
                classNames.append(randomName)

                if genre != 'Random':
                    classTypes = genreMap[genre]
                    for classType in classTypes:
                        if not classType.isAbstract():
                            # qualify the name so that it can be properly displayed
                            name            = classType.getName()
                            qualifiedName    = '[%s]  %s' % (genre, name)
                            classNames.append(qualifiedName)

            classNames.sort()

            # update the cache
            self.dropDownListCache = classNames

        return self.dropDownListCache

    
    def getRandomShortClassName(self, xmlData, genreFilter):
        'get a random short class name based on genreFilter!!!'
        attribmap, classes, genreMap, weapons, armor, nameMap = xmlData
        filteredClassNames = []
        genreName = 'Random'

        # string hack: get genre name from Random prefix
        if genreFilter[1:7] == 'Random':
            genreName = genreFilter[8:]

        # populate filteredClassNames
        if genreName not in self.classNamesGenreMap:
            classTypes = genreMap[genreName]

            for classType in classTypes:
                if not classType.isAbstract():
                    className = classType.getName()
                    filteredClassNames.append(className)

            # stick in cache so we don't have to do this again
            self.classNamesGenreMap[genreName] = filteredClassNames
        else:
            filteredClassNames = self.classNamesGenreMap[genreName]

        return (genreName, choice(filteredClassNames))


    def getClassAliasMap(self, xmlData, reload=0):
        'get the classAliasMap from the cache'
        if reload or not self.classAliasMapCache:
            classAliasMap = {}
            attribmap, classes, genreMap, weapons, armor, nameMap = xmlData
            aliases = classes.keys()

            for alias in aliases:
                classType = classes[alias]
                className = classType.getName()
                classAliasMap[className] = alias

            # update the cache
            self.classAliasMapCache = classAliasMap

        return self.classAliasMapCache


    def getShortClassName(self, longClassName):
        'get short version of className'
        if longClassName[0] == '*':
            genreName, shortClassName = self.getRandomShortClassName(self.getXMLData(), longClassName)
        else:
            genreName = None
            shortClassName    = longClassName[longClassName.index(']') + 3:]

        # cache this puppy
        self.lastShortClassName = shortClassName

        return (genreName, shortClassName)



class NPCStack(list):
    'custom npc stack class'
    def __init__(self):
        self.index = None

    def append(self, npc):
        self[len(self):len(self)] = [npc]
        if len(self) > kMaxNPCStackSize:
            self.pop(0)
        self.index = len(self) - 1

    def previous(self):
        if self.index is not None:
            self.index -= 1
            if self.index < 0:
                # to make this circular: self.index = len(self) - 1
                self.index = 0
            else:
                return self[self.index]

    def next(self):
        if self.index is not None:
            self.index += 1
            if self.index >= len(self):
                # to make this circular: self.index = 0
                self.index = len(self) - 1
            else:
                return self[self.index]

    def get(self, index):
        if 0 <= index < len(self):
            self.index = index
            return self[index]

    def getCurrentIndex(self):
        if self.index is None:
            return -1
        return self.index

    def getCurrent(self):
        if self.index is not None:
            return self[self.index]



class LogWindow(PrintableTextControl):
    'custom logwindow class; can be substituted for sys.stdout'
    def __init__(self, parent, ID, title):
        self.kDefaultFont                = wx.Font(kFontSize, -1, wx.NORMAL, wx.NORMAL, 0, faceName='Courier New')
        self.kBoldFont                    = wx.Font(kFontSize, -1, wx.NORMAL, wx.BOLD,   0, faceName='Courier New')
        PrintableTextControl.__init__(self, parent, ID, title, style=wx.TE_MULTILINE|wx.VSCROLL|wx.TE_RICH)
        self.SetFont(self.kDefaultFont)

    def writeToErrorLog(self, text, display=1):
        'write (append) directly to the errorlog file and close it'
        try:
            if display:
                self.WriteText(text)
            errorlog = file(kErrorLogFilePath, 'a')
            errorlog.write(text)
            errorlog.close()
        except (OSError, IOError), why:
            self.WriteText(str(why) + '\n')

    def write(self, text):
        'write text to logWindow, formatting text if custom font markup is found'
        matchObj = kCustomTextRegex.search(text)
        while matchObj:
            preMatch = text[:matchObj.start(1)]
            self.SetDefaultStyle(wx.TextAttr(wx.BLACK, font=self.kDefaultFont))
            self.WriteText(preMatch)

            afterMatch = text[matchObj.end(1):matchObj.end(2)]
            attrib, value = matchObj.group(1)[1:-1].split(':')
            if attrib == 'wx.Color':
                self.SetDefaultStyle(wx.TextAttr(wx.NamedColour(value), font=self.kBoldFont))
            elif attrib == 'wx.Bold':
                self.SetDefaultStyle(wx.TextAttr(wx.BLACK, font=self.kBoldFont))
            elif attrib == 'wx.Font':
                font = wx.Font(11, -1, wx.NORMAL, wx.BOLD, 0, faceName=value)
                self.SetDefaultStyle(wx.TextAttr(wx.NamedColour('MEDIUM BLUE'), font=font))
            self.WriteText(afterMatch)
            text = text[matchObj.end():]
            matchObj = kCustomTextRegex.search(text)
        else:
            self.SetDefaultStyle(wx.TextAttr(wx.BLACK, font=self.kDefaultFont))
            self.WriteText(text)

class ErrorLog:
    'wrapper errolog class'
    def __init__(self, logwindow):
        self.logWindow = logwindow

    def write(self, text):
        if 'DeprecationWarning' not in text:
            self.writeToErrorLog(text)

    def writeToErrorLog(self, text, display=1):
        'write (append) directly to the errorlog file and close it'
        try:
            if display:
                #print '!', text
                self.logWindow.LoadString(text)
            errorlog = file(kErrorLogFilePath, 'a')
            errorlog.write(text)
            errorlog.close()
        except (OSError, IOError), why:
            self.logWindow.LoadString(str(why) + '\n')



class TargetPointsDialog(wx.Dialog):
    'TargetPoints Dialog, with ability to control precision'
    def __init__(self, parent, id, title, target, precision):
        wx.Dialog.__init__(self, parent, id, title, size=wx.Size(550, 230), style=wx.CAPTION | wx.SYSTEM_MENU | wx.THICK_FRAME | wx.CLOSE_BOX)
        self.parent = parent
        self.target = target
        self.precision = precision
        # buttons
        ok = wx.Button(self, wx.ID_OK, " OK ", pos=wx.Point(210, 136))
        wx.EVT_BUTTON(self, ok.GetId(), self.OnOK)
        wx.Button(self, wx.ID_CANCEL, " Cancel ", pos=wx.Point(380, 136))

        # sliders
        wx.StaticText(self, -1, label='Target Point Value:', pos=wx.Point(6, 12))
        self.targetSlider = wx.Slider(self, -1, target, 0, 900, wx.Point(102, 10), wx.Size(425, -1), wx.SL_HORIZONTAL|wx.SL_AUTOTICKS|wx.SL_LABELS)
        self.targetSlider.SetTickFreq(25, 1)
        self.targetSlider.SetPageSize(50)

        wx.StaticText(self, -1, label='Precision:', pos=wx.Point(6, 122))
        self.precisionSlider = wx.Slider(self, -1, precision, 1, 20, wx.Point(102, 72), wx.Size(-1, -1), wx.SL_VERTICAL|wx.SL_AUTOTICKS|wx.SL_LABELS)

    def OnOK(self, event):
        self.target = self.targetSlider.GetValue()
        self.precision = self.precisionSlider.GetValue()
        self.EndModal(wx.ID_OK)


class SyncDialog(wx.Dialog):
    'UI Dialog widget for syncronizing repository data'
    def __init__(self, parent, id, title):
        width = 488; height = 398
        wx.Dialog.__init__(self, parent, id, title, size=wx.Size(width, height), style=wx.CAPTION | wx.SYSTEM_MENU | wx.CLOSE_BOX)
        #self.SetSizeHints(width, height, width, height)
        self.parent = parent
        connect = wx.Button(self, -1, label='Connect', pos=wx.Point(18, 10))
        wx.EVT_BUTTON(self, connect.GetId(), self.onConnect)
        # connection info
        self.useProxy     = wx.CheckBox(self, -1, 'http proxy:', pos=wx.Point(20, 38))
        self.serverField  = wx.TextCtrl(self, -1, self.parent.positionTable['RPCServerURI'],   pos=wx.Point(100, 12), size=(220, -1))
        self.proxyField   = wx.TextCtrl(self, -1, self.parent.positionTable['RPCServerProxy'], pos=wx.Point(100, 34), size=(120, -1))
        self.useProxy.SetValue(self.parent.positionTable['useRPCProxy'])
        if not self.parent.positionTable['useRPCProxy']:
            self.proxyField.Enable(0)
        wx.EVT_CHECKBOX(self, self.useProxy.GetId(), self.onCheckProxy)
        # rbuttons
        wx.StaticText(self, -1, label='Synchronize', pos=wx.Point(360, 12))
        wx.RadioButton(self, -1, "Data", (340, 26), style=wx.RB_GROUP)
        self.syncCodeRB = wx.RadioButton(self, -1, "Code", (388, 26))
        if not os.path.basename(sys.argv[0]).lower() in ('cmaker-gui.py', 'cmaker~1.py'):
            self.syncCodeRB.Enable(0)

        wx.StaticText(self, -1, label='New Server Files', pos=wx.Point(8, 60))
        self.newServerFiles = wx.ListBox(self, -1, wx.Point(8, 78), wx.Size(190, 150), [], wx.LB_EXTENDED | wx.LB_SORT)
        # wx.LB_MULTIPLE
        wx.StaticText(self, -1, label='New Client Files', pos=wx.Point(278, 60))
        self.newClientFiles = wx.ListBox(self, -1, wx.Point(280, 78), wx.Size(190, 150), [], wx.LB_EXTENDED | wx.LB_SORT)

        # download/upload
        self.downloadButton = wx.Button(self, -1, label='Download->', pos=wx.Point(200, 100))
        wx.EVT_BUTTON(self, self.downloadButton.GetId(), self.onDownload)
        self.downloadButton.Enable(0)
        self.uploadButton = wx.Button(self, -1, label='  <- Upload  ', pos=wx.Point(200, 140))
        wx.EVT_BUTTON(self, self.uploadButton.GetId(), self.onUpload)
        self.uploadButton.Enable(0)

        # logWindow
        style = wx.TE_MULTILINE|wx.VSCROLL|wx.TE_READONLY # wx.TE_RICH|
        self.logWindow = wx.TextCtrl(self, -1, '', style=style, pos=wx.Point(8, 240), size=wx.Size(464, 120))
        self.logWindow.SetBackgroundColour(wx.NamedColour('LIGHT GREY'))


    def autoDownload(self):
        self.Show(1)
        # aparently, we need to call update, in order to force re-painting of this dialog since we didn't use ShowModal()
        self.Update()
        self.logWindow.WriteText('initializing NPCGenerator for first time, auto-syncing files...\n\n')
        self.onConnect()
        self.onDownload()
        self.Destroy()


    def onCheckProxy(self, event):
        'reset the self.proxyField Enabled, based on the useProxy check box'
        self.proxyField.Enable(self.useProxy.GetValue())

    
    def clearLog(self):
        'nasty hack'
        self.logWindow.SetFocus()
        self.logWindow.Clear()
        self.logWindow.WriteText('\n'*12)
        self.logWindow.Clear()
        #self.logWindow.Refresh()


    def onConnect(self, event=None):
        self.diffMap = {}
        if event:
            self.clearLog()
        self.downloadButton.Enable(0)
        self.uploadButton.Enable(0)
        try:
            # connect...
            urlTransport = URLLibTransport()
            if self.useProxy.GetValue():
                urlTransport = URLLibTransport(self.proxyField.GetValue())
            self.server = ServerProxy(self.serverField.GetValue(), transport=urlTransport)
            self.logWindow.WriteText('connecting to %s...\n' % self.serverField.GetValue())
            if self.syncCodeRB.GetValue():
                # sync code
                self.diffMap = self.server.getRepositoryCodeDiffs(charactermaker.GetRepositoryCodeMap())
            else:
                self.diffMap = self.server.getRepositoryDataDiffs(charactermaker.GetRepositoryDataMapMain(), event==None)
            # load any new server files
            self.logWindow.WriteText('checking remote files...\n')
            self.newServerFiles.Clear()
            newServerFiles = self.diffMap['new-server']
            if newServerFiles:
                self.downloadButton.Enable(1)
                for baseFilePath in newServerFiles:
                    self.newServerFiles.Append(baseFilePath)
                for i in range(self.newServerFiles.GetCount()):
                    self.newServerFiles.SetSelection(i)
            # load any new client files
            self.logWindow.WriteText('checking client files...\n')
            self.newClientFiles.Clear()
            newClientFiles = self.diffMap['new-client']
            if newClientFiles:
                self.uploadButton.Enable(1)
                for baseFilePath in newClientFiles:
                    self.newClientFiles.Append(baseFilePath)
            # done -- persist the last connect URI
            if not (self.diffMap['new-server'] or self.diffMap['new-client']):
                self.logWindow.WriteText('No new files detected.\n')
            self.logWindow.WriteText('Done.\n\n')
            self.parent.positionTable['RPCServerURI'] = self.serverField.GetValue()
            self.parent.positionTable['useRPCProxy']  = self.useProxy.GetValue()
            if self.useProxy.GetValue():
                self.parent.positionTable['RPCServerProxy'] = self.proxyField.GetValue()
        except ExpatError, why:
            self.logWindow.WriteText('Received unparseable HTTP error from proxy: %s' % str(why))
            self.logCurrentExceptionTrace()
        except IOError, why:
            self.logWindow.WriteText('Failed to connect: %s' % str(why))
        except Exception, why:
            self.logWindow.WriteText('Failed to connect: %s' % str(why))
            self.logCurrentExceptionTrace()
            # raise sys.exc_info()[1]


    def logCurrentExceptionTrace(self):
        "log the current exception stack trace to the error log of this dialogue's parent"
        exceptionTrace = string.join(format_exception(sys.exc_type, sys.exc_value, sys.exc_traceback), '')
        self.parent.writeError(exceptionTrace, display=0)


    def onDownload(self, event=None):
        if event:
            self.clearLog()
        serverFiles = self.newServerFiles.GetSelections()
        if serverFiles:
            try:
                serverFilePaths = []
                for serverFileIndex in serverFiles:
                    serverFilePaths.append(self.newServerFiles.GetString(serverFileIndex))
                for serverFilePath in serverFilePaths:
                    self.logWindow.WriteText('retrieving %s from server...\n' % serverFilePath)
                    dirname, basename = (os.path.dirname(serverFilePath), os.path.basename(serverFilePath))
                    fullDirPath = '%s%s%s' % (GetRootDir(), os.sep, dirname)
                    fullPath = '%s%s%s' % (GetRootDir(), os.sep, serverFilePath)
                    if event:
                        fileData = self.server.getZippedFileData(serverFilePath)
                        if not os.path.isdir(fullDirPath):
                            self.logWindow.WriteText('creating dir: %s...\n' % fullDirPath)
                            os.makedirs(fullDirPath)
                        if os.path.isfile(fullPath):
                            self.logWindow.WriteText('overwriting: %s...\n' % fullPath)
                        else:
                            self.logWindow.WriteText('creating: %s...\n' % fullPath)
                        localFile  = file(fullPath, 'wb')
                        localFile.write(decompress(fileData.data))
                        localFile.close()
                    # if no event, only update file mtime
                    mTime = self.diffMap['new-server'][serverFilePath] + kTimeZoneDifference
                    self.logWindow.WriteText('resetting file time: %s...\n' % mTime)
                    os.utime(fullPath, (mTime, mTime))
                # clear serverFilePaths (have to do this after all serverFiles have been processed!!!!)
                for serverFilePath in serverFilePaths:
                    self.newServerFiles.Delete(self.newServerFiles.FindString(serverFilePath))
                if not self.newServerFiles.GetCount():
                    self.downloadButton.Enable(0)
                # out of loop: reload code --or-- xml now
                if self.syncCodeRB.GetValue():
                    if os.path.basename(sys.argv[0]) == 'cmaker-gui.py':
                        self.logWindow.WriteText('reloading main engine module...\n')
                        reload(charactermaker)
                else:
                    self.logWindow.WriteText('reloading local xml data...\n')
                    self.parent.onReload(event)
                self.logWindow.WriteText('Done.\n\n')
            except Exception, why:
                self.logWindow.WriteText('Failed download: %s' % str(why))
                self.logCurrentExceptionTrace()
        else:
            self.logWindow.WriteText('No server files are selected.\n')


    def onUpload(self, event):
        self.clearLog()
        clientFiles = self.newClientFiles.GetSelections()
        if clientFiles:
            try:
                clientFilePaths = []
                for clientFileIndex in clientFiles:
                    clientFilePaths.append(self.newClientFiles.GetString(clientFileIndex))
                for clientFilePath in clientFilePaths:
                    self.logWindow.WriteText('sending %s to server...\n' % clientFilePath)
                    fullPath = '%s%s%s' % (GetRootDir(), os.sep, clientFilePath)
                    repFile  = file(fullPath, 'rb')
                    fileData = repFile.read()
                    repFile.close()
                    failureMessage = self.server.sendZippedFileData(
                                                                        Binary(self.getUploadPassword()),
                                                                        clientFilePath,
                                                                        self.diffMap['new-client'][clientFilePath],
                                                                        Binary(compress(fileData, 9))
                                                                   )
                    if failureMessage:
                        raise Exception(failureMessage)
                # clear clientFilePaths (have to do this after all clientFiles have been processed!!!!)
                for clientFilePath in clientFilePaths:
                    self.newClientFiles.Delete(self.newClientFiles.FindString(clientFilePath))
                if not self.newClientFiles.GetCount():
                    self.uploadButton.Enable(0)
                # out of loop: done
                self.logWindow.WriteText('Done.\n\n')
            except Exception, why:
                # HACK: assume it was a password failure, so reset stored password
                self.parent.positionTable['uploadPassword'] = ''
                self.logWindow.WriteText('Failed upload: %s' % str(why))
                self.logCurrentExceptionTrace()
        else:
            self.logWindow.WriteText('No client files are selected.\n')


    def getUploadPassword(self, reload=0):
        'upload requires password, so prompt user with dialogue if password has not already been (successfully) entered before'
        if reload:
            storedPassword = ''
        else:
            storedPassword = self.parent.positionTable['uploadPassword']
        if not storedPassword:
            style = wx.OK | wx.CANCEL | wx.CENTRE | wx.TE_PASSWORD
            passwordDialog = wx.TextEntryDialog(self, 'Please enter password:', 'Upload Password Required', '', style=style)
            if passwordDialog.ShowModal() == wx.ID_OK:
                textPassword = passwordDialog.GetValue()
                storedPassword = sha.new(textPassword).digest()
                self.parent.positionTable['uploadPassword'] = storedPassword
            passwordDialog.Destroy()
        return storedPassword


