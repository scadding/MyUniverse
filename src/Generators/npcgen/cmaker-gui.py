#! /usr/bin/env python

"""
 *******************************************************************************
 * 
 * $Id: cmaker-gui.py 213 2009-09-03 22:27:05Z andrew $
 * $URL: http://ayinger.no-ip.info/repos/npcgen/trunk/cmaker-gui.py $
 *
 * $Date: 2009-09-03 17:27:05 -0500 (Thu, 03 Sep 2009) $
 * $Author: andrew $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description:
 *
 * Source for NPCGen.exe 
 *
 * GUI Wrapper for charactermaker.py
 * 
 *******************************************************************************
""" 
 

import sys, os, string, cPickle, codecs, StringIO
import wx

from random import choice
from time import time
from zlib import compress, decompress
from pprint import PrettyPrinter
from traceback import format_exception
from xmlrpclib import ServerProxy, Binary
from xml.sax._exceptions import SAXParseException

from Utils import GetRootDir, GetElapsedTime, URLLibTransport, GetSimpleHTML
from parentsTree import CreateParentsTree
from UIClasses import UIMixIn, NPCStack, LogWindow, ErrorLog, SyncDialog, TargetPointsDialog
from NPCData import NPCBuilder

# load the main NPC Engine Functions
import charactermaker

args = sys.argv

import wx.html2 as iewin


# Constants
kDisplayHTML = 1
kHTMLEnabledPlatform = '__WXMSW__'
if wx.Platform == kHTMLEnabledPlatform:
    #from wx.Python.iewin import *
    import wx.lib.iewin as iewin
else:
    kDisplayHTML = 0

kCurrentDir                    = os.getcwd()
kRootDir                    = GetRootDir()

#ID_GENBUTTON                = 900

kDefaultWidth                = 840
kDefaultHeight                = 850

kSettingsFileName            = 'NPCGen.cfg'
kDefaultRPCServerURI        = 'https://ayinger.no-ip.info:9777'
kDefaultRPCServerProxy        = 'http://myproxy:8080'



class NPCFrame(wx.Frame, UIMixIn):
    'main NPCFrame class'
    def __init__(self, parent, ID, title):
        wx.Frame.__init__(self, parent, ID, title, wx.DefaultPosition, wx.Size(kDefaultWidth, kDefaultHeight))

        # cached data
        self.lastPointTargetIndex = 10
        self.target = 150
        self.precision = 5
        self.displayHTML = kDisplayHTML
        self.npcStack = NPCStack()
        self.currentNPCIndex = None

        self.parentsTreeFrame = None
        self.parentsTreeFrameSize = ()
        self.lastStatus = ''

        self.sSettingsFileName = kRootDir + os.sep + 'cfg' + os.sep + kSettingsFileName
        self.positionTable = {
                                'size':                (),
                                'position':            (),
                                'number':             0,
                                'power':            50,
                                'selection':        '*Random Random',
                                'parentsTreeSize':    (),
                                'uploadPassword':    '',
                                'RPCServerURI':     kDefaultRPCServerURI,
                                'RPCServerProxy':   kDefaultRPCServerProxy,
                                'useRPCProxy':        0
                             }

        # mode
        self.encoding = 'UTF-8'
        if os.name == 'posix':
            self.mode = 2
            self.encoding = 'mac_latin2'

        # set icon, populate frame and then load default settings...
        self.SetIcon(wx.Icon(kRootDir + '/images/favicon.ico', wx.BITMAP_TYPE_ICO))
        self.populateFrame()
        autosync = 0
        if os.path.isfile(self.sSettingsFileName):
            self.loadDefaultSettings()
        else:
            autosync = 1

        wx.EVT_CLOSE(self, self.quit)
        self.Show(1)

        if autosync:
            self.onSync()
            #dialog.onConnect()
            #dialog.onDownload()


    def populateFrame(self):
        self.createFrameMenu()
        self.createStatusBar()
        self.createToolBar()
        if self.displayHTML:
            self.createIEWindow()
        else:
            self.createLogWindow()


    def createIEWindow(self):
        #iewin.IEHtmlWindow
        self.logWindow = iewin.IEHtmlWindow(self, -1, style=wx.NO_FULL_REPAINT_ON_RESIZE)
        #self.logWindow.SetCharset('ISO-8859-1')
        self.logWindow.write = self.writeLog
        self.logWindow.writeOne = self.writeOne
        self.logWindow.WriteText = self.writeLog
        self.createErrorLog()

    def writeLog(self, *args):
        logString = unicode('', 'utf8')
        for arg in args:
            if (type(arg) == type(u'')): logString += arg
            else: logString += str(arg)
        self.logWindow.LoadString(logString)

    def writeOne(self, stuff):
        self.logWindow.LoadString(stuff)
        #self.logWindow.LoadStream(StringIO.StringIO(stuff.encode(self.encoding)))  

    def createLogWindow(self):
        'create the logWindow and errorLog wrapper class; redirect stdout and stderr'
        self.logWindow = LogWindow(self, -1, '')
        self.logWindow.LoadString = self.logWindow.write
        self.logWindow.writeOne = self.logWindow.write
        self.createErrorLog()
        """self.errorLog = ErrorLog(self.logWindow)
        # redirect
        sys.stdout = self.logWindow
        sys.stderr = self.errorLog"""

    def createErrorLog(self):
        self.errorLog = ErrorLog(self.logWindow)
        # redirect
        #sys.stderr = self.errorLog

    def writeError(self, text, display):
        'wrapper for controlling error output'
        #self.logWindow.writeToErrorLog(text, display)
        self.logWindow.LoadString(text)

    def logCurrentExceptionTrace(self):
        'log the current exception stack trace to the error log'
        exceptionTrace = string.join(format_exception(sys.exc_type, sys.exc_value, sys.exc_traceback), '')
        #self.writeError(exceptionTrace, display=0)
        self.errorLog.writeToErrorLog(exceptionTrace, display=0)


    def createToolBar(self):
        'create the main action toolbar'
        captionFont        = wx.Font(6, -1, wx.NORMAL, wx.NORMAL, 0, faceName='Arial')
        imageDir        = kRootDir + os.sep + 'images' + os.sep
        """if os.name == 'posix':
            genNextPNG    = wx.Bitmap(imageDir + 'GenNext.png', wx.BITMAP_TYPE_PNG) 
        else:
            genNextPNG    = WriteBitMapCaption('GO', wx.Bitmap(imageDir + 'GenNext.png', wx.BITMAP_TYPE_PNG), captionFont, margins=(1,1))"""
        previousPNG        = wx.Bitmap(imageDir + 'previousButton.png',    wx.BITMAP_TYPE_PNG)
        nextPNG            = wx.Bitmap(imageDir + 'nextButton.png',        wx.BITMAP_TYPE_PNG)
        historyPNG        = wx.Bitmap(imageDir + 'Refresh.png',            wx.BITMAP_TYPE_PNG)
        newPNG            = wx.Bitmap(imageDir + 'new.png',                wx.BITMAP_TYPE_PNG)
        newTargetPNG    = wx.Bitmap(imageDir + 'newG.png',                wx.BITMAP_TYPE_PNG)
        saveAsPNG        = wx.Bitmap(imageDir + 'SaveAs.png',            wx.BITMAP_TYPE_PNG)
        saveDBPNG        = wx.Bitmap(imageDir + 'db.png',                wx.BITMAP_TYPE_PNG)
        printPNG        = wx.Bitmap(imageDir + 'Print.png',                wx.BITMAP_TYPE_PNG)
        reloadPNG        = wx.Bitmap(imageDir + 'Reload.png',            wx.BITMAP_TYPE_PNG)
        treePNG            = wx.Bitmap(imageDir + 'Branch.png',            wx.BITMAP_TYPE_PNG)
        syncPNG            = wx.Bitmap(imageDir + 'sync.png',                wx.BITMAP_TYPE_PNG)
        openPNG            = wx.Bitmap(imageDir + 'open.png',                wx.BITMAP_TYPE_PNG)

        self.toolBar = self.CreateToolBar(wx.NO_BORDER|wx.TB_FLAT|wx.TB_HORIZONTAL) #wx.NO_BORDER
        #self.toolBar.SetToolBitmapSize((18, 16))

        # main action tools (create combo and spin first, add below)
        self.characterType = wx.ComboBox(
                                            self.toolBar, -1,
                                            '*Random Random',
                                            wx.Point(0, 25), wx.Size(280, -1),
                                            self.getClassNames(self.getXMLData()),
                                            wx.CB_READONLY
                                       )
        self.characterType.SetSelection(0)
        self.characterType.SetToolTipString('Choose NPC Class Type')
        wx.EVT_COMBOBOX(self, self.characterType.GetId(), self.onSelect)
        self.quantity = wx.SpinCtrl(self.toolBar, -1, "1", wx.Point(0, 60), wx.Size(48, -1), wx.SP_ARROW_KEYS, 1, 999, 1, "npc #")
        self.quantity.SetToolTipString('Increment NPC Quantity')
        wx.EVT_SPINCTRL(self, self.quantity.GetId(), self.onSelect)

        #tb.AddCheckTool(50, images.getTog1Bitmap(), shortHelp="Toggle this") toggle

        # navigate buttons
        #self.toolBar.AddLabelTool(890, "labelback", previousPNG, wx.NullBitmap, wx.ITEM_NORMAL, 'Back', 'Display previous NPC')
        self.Bind(wx.EVT_TOOL, self.displayPreviousNPC, self.toolBar.AddSimpleTool(890, previousPNG, 'Back', 'Display previous NPC'))
        self.Bind(wx.EVT_TOOL, self.onShowNPCStack, self.toolBar.AddSimpleTool(891, historyPNG, 'History', 'Display all NPCs'))
        self.Bind(wx.EVT_TOOL, self.displayNextNPC, self.toolBar.AddSimpleTool(892, nextPNG, 'Forward', 'Display next NPC'))
        for id in range(890, 893):
            self.toolBar.EnableTool(id, 0)

        # save and print button
        self.toolBar.AddSeparator()
        self.Bind(wx.EVT_TOOL, self.onOpen, self.toolBar.AddSimpleTool(-1, openPNG, 'Open', 'Open saved NPC...'))
        self.Bind(wx.EVT_TOOL, self.onSaveAs, self.toolBar.AddSimpleTool(901, saveAsPNG, 'SaveAs', 'Save the Current non-empty Screen to a Text File...'))
        self.Bind(wx.EVT_TOOL, self.onSaveToDB, self.toolBar.AddSimpleTool(902, saveDBPNG, 'Save to DB', 'Save the Current non-empty Screen to remote Database'))
        self.Bind(wx.EVT_TOOL, self.onPrint, self.toolBar.AddSimpleTool(899, printPNG, 'Print', 'Print the Current Screen'))
        for id in range(901, 903):
            self.toolBar.EnableTool(id, 0)

        # new gen buttons
        self.toolBar.AddSeparator()
        tool = self.toolBar.AddSimpleTool(-1, newPNG, 'Generate New NPC(s)')#,
                                                    #'Generate %s New "%s" NPC(s)' % (self.quantity.GetValue(), self.characterType.GetValue()))
        self.Bind(wx.EVT_TOOL, self.onGen, tool)
        self.genId = tool.GetId()
        self.Bind(wx.EVT_TOOL, self.onGenTargetPoints, self.toolBar.AddSimpleTool(-1, newTargetPNG, 'Generate New NPC with Points'))#, 'Generate New NPC with Target Point Value'))

        # add combo box (290) and quantity spin
        self.toolBar.AddControl(self.characterType)
        self.toolBar.AddControl(wx.StaticText(self.toolBar, -1, ' quantity: ', style=wx.ALIGN_CENTRE))
        self.toolBar.AddControl(self.quantity)

        # slider
        self.toolBar.AddControl(wx.StaticText(self.toolBar, -1, ' relative power: ', style=wx.ALIGN_CENTRE))
        # parent, id, pos, size, style, validator, name
        #self.slider = wx.Slider(self.toolBar, -1, 50, 0, 100, wx.Point(0, 60), wx.Size(160, -1), wx.SL_HORIZONTAL) # | wx.SL_AUTOTICKS) # | wx.SL_LABELS | wx.SL_TOP)
        #self.slider.SetTickFreq(10, 1)
        #self.slider.SetPageSize(10)
        self.slider = wx.SpinCtrl(self.toolBar, -1, "50", wx.Point(0, 60), wx.Size(46, -1), wx.SP_ARROW_KEYS, 1, 100, 50, "npc power")
        self.slider.SetToolTipString('Relative Power')
        self.toolBar.AddControl(self.slider)

        # basic action tools
        self.toolBar.AddSeparator()
        self.Bind(wx.EVT_TOOL, self.onReload, self.toolBar.AddSimpleTool(-1, reloadPNG, 'Reload XML Data', 'Dynamically reload all NPC Templates from XML'))
        self.Bind(wx.EVT_TOOL, self.showParentsTreeFrame, self.toolBar.AddSimpleTool(-1, treePNG, 'Display Template Data...', 'Display all Template Data in Relational Tree'))
        self.Bind(wx.EVT_TOOL, self.onSync, self.toolBar.AddSimpleTool(-1, syncPNG, 'Sync. Data...', 'Synchronize Local XML Files with Remote Server'))

        # finally, realize (pack everything together and display)
        #self.SetToolBar(self.toolBar)
        self.toolBar.Realize()


    def createFrameMenu(self):
        'create menuBar Items...'
        # fileMenu
        fileMenu = wx.Menu()
        self.Bind(wx.EVT_MENU, self.onOpen, fileMenu.Append(-1, '&Open...\tCtrl-O', 'Open'))
        self.Bind(wx.EVT_MENU, self.onSaveAs, fileMenu.Append(-1, '&Save As...\tCtrl-S', 'Save the Current Screen to Text File'))
        self.Bind(wx.EVT_MENU, self.onSaveToDB, fileMenu.Append(-1, "Save to &DB...\tCtrl-D", "Save the Current Screen to remote DB"))
        self.Bind(wx.EVT_MENU, self.onPrint, fileMenu.Append(-1, "&Print...\tCtrl-P", "Print the Current Screen"))
        fileMenu.AppendSeparator()
        self.Bind(wx.EVT_MENU, self.quit, fileMenu.Append(-1, "&Exit\tAlt-X", "Exit the program."))

        # actionMenu
        actionMenu = wx.Menu()
        self.htmlMode = wx.MenuItem(actionMenu, -1, "&Switch HTML Mode\tAlt-S", "Switch to HTML or Simple Text", wx.ITEM_CHECK)
        self.Bind(wx.EVT_MENU, self.onLogWindowSwitch, self.htmlMode)
        actionMenu.AppendItem(self.htmlMode)
        actionMenu.AppendSeparator()

        self.Bind(wx.EVT_MENU, self.onGen, actionMenu.Append(-1, '&Generate NPC(s)\tCtrl-G', 'Generate NPC(s) defined in dropdown'))
        self.Bind(wx.EVT_MENU, self.onGenTargetPoints, actionMenu.Append(-1, "&Generate NPC(s) with Point Value...\tAlt-G", "Generate NPC(s) with a Target Point Value"))
        self.Bind(wx.EVT_MENU, self.displayNextNPC, actionMenu.Append(-1, "Go &Forward\tCtrl-F", "Display next NPC in buffer"))
        self.Bind(wx.EVT_MENU, self.displayPreviousNPC, actionMenu.Append(-1, "Go &Back\tCtrl-B", "Display previous NPC in buffer"))
        actionMenu.AppendSeparator()
        self.Bind(wx.EVT_MENU, self.onShowNPCStack, actionMenu.Append(-1, "S&how NPCs\tCtrl-H", "Display all NPCs currently in buffer"))

        # NPCDataMenu
        NPCDataMenu = wx.Menu()
        self.Bind(wx.EVT_MENU, self.showParentsTreeFrame, NPCDataMenu.Append(-1, "&Display Template Data...\tAlt-D", "Display all Template Data in Relational Tree"))
        NPCDataMenu.AppendSeparator()

        self.Bind(wx.EVT_MENU, self.onReload, NPCDataMenu.Append(-1, "&Reload Data\tCtrl-R", "Reload ALL Template Data from Local XML Files"))
        self.Bind(wx.EVT_MENU, self.onSync, NPCDataMenu.Append(-1, "S&ynchronize Data...\tCtrl-Y", "Synchronize Local XML Files with Remote Server"))
        self.Bind(wx.EVT_MENU, self.onLoadFromServer, NPCDataMenu.Append(-1, "&Load Data from Server...\tCtrl-L", "Load ALL Template Data from Remote Server"))

        # helpMenu
        helpMenu = wx.Menu()
        self.Bind(wx.EVT_MENU, self.onMoreHelp, helpMenu.Append(-1,  "&Help...", "More help about this program"))
        self.Bind(wx.EVT_MENU, self.onAbout, helpMenu.Append(-1, "&About GURPS NPC Generator...", "More information about this program"))

        # create Parent menuBar
        menuBar = wx.MenuBar()
        menuBar.Append(fileMenu, "&File")
        menuBar.Append(actionMenu, "&Action")
        menuBar.Append(NPCDataMenu, "&NPC Data")
        menuBar.Append(helpMenu, "&Help")
        self.SetMenuBar(menuBar)

        if kDisplayHTML:
            self.htmlMode.Check(1)


    def createStatusBar(self):
        self.CreateStatusBar()
        self.SetStatusText("Status: ")


    ################################## MENU-BAR EVENT-INVOKED FUNCTIONS ####################
    def onSelect(self, event=None):
        'reset the help text for the gen. button on any select-type events'
        #print 'in on select'
        genre, displayName = self.getShortClassName(self.characterType.GetValue())
        if genre:
            displayName = genre
        self.toolBar.SetToolShortHelp(self.genId, 'Generate New %s NPC(s)...' % displayName)
        #if self.lastStatus: self.toolBar.SetToolLongHelp(self.genId, self.lastStatus)
        #self.toolBar.SetToolLongHelp(self.genId, 'Generate %s New "%s" NPC(s)' % (self.quantity.GetValue(), displayName))


    def onAbout(self, event):
        'talk about myself'
        number = '$Rev: 213 $'
        rev = number[6:-2]

        helpDialogue = wx.MessageDialog(self,
            "GURPS NPC Generator\n"
            "\n"
            "version " + "4.0." + rev + " \n"
            "\n\nit's about family ...\n\n"
            "by andrew yinger",
            "About GURPS NPC Generator", wx.OK | wx.ICON_INFORMATION)

        helpDialogue.ShowModal()
        helpDialogue.Destroy()


    def onMoreHelp(self, event):
        'more help'
        helpFilePath = GetRootDir() + os.sep + 'README.html'
        if self.displayHTML:
            url = 'file:///' + helpFilePath.replace('\\', '/')
            self.logWindow.LoadUrl(url)
        else:
            helpDialogue = wx.MessageDialog(self,
                kHelpText +
                "\n"
                "\nFor more help, read: " + helpFilePath + "   \n\n",
                "GURPS NPC Generator Help", wx.OK | wx.ICON_QUESTION)

            helpDialogue.ShowModal()
            helpDialogue.Destroy()


    def onClear(self, event=None):
        'clear the mofo screen'
        self.logWindow.Clear()
        if self.displayHTML:
            self.logWindow.write('')
        self.toolBar.EnableTool(901, 0)
        self.toolBar.EnableTool(902, 0)


    def onSaveToDB(self, event=None, npc=None):
        if not npc:
            npc = self.npcStack.getCurrent()
        if npc:
            style = wx.OK | wx.CANCEL | wx.CENTRE
            serverDialog = wx.TextEntryDialog(self, 'Server Address:', 'Store NPC on remote NPC Server', self.positionTable['RPCServerURI'], style=style)
            if (not event) or (serverDialog.ShowModal() == wx.ID_OK):
                startTime = time()
                serverURI = serverDialog.GetValue()
                #hmmm... 
                self.positionTable['RPCServerURI'] = serverURI
                # use URLLibTransport so we can use http proxy on this as well 
                server = ServerProxy(serverURI, transport=URLLibTransport())
                self.SetStatusText("Sending NPC to: %s...." % serverURI)
                npcData = Binary(npc.getDBData(1))
                result = server.storeNPC(npcData, 2.0)
                if result != -1:
                    self.SetStatusText("NPC Data sent to %s: %s" % (serverURI, GetElapsedTime(startTime)))
                else:
                    self.SetStatusText("NPC Data already on Server!")
            serverDialog.Destroy()


    def onOpen(self, event):
        'Open User saved NPC (html only)'
        wildCardFlags            = 'HTML files (*.html)|*.html'
        defaultDir                = kRootDir + os.sep + 'sav'
        dlg = wx.FileDialog(self, 'Choose a file', defaultDir, '', wildCardFlags, wx.OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            fileName = dlg.GetPath()
            npcFile = codecs.open(fileName, 'rb', 'utf-8')
            npcData = npcFile.read()
            marker = '##!!##'
            npcRaw = npcData[npcData.find(marker)+len(marker):npcData.rfind(marker)]
            try:
                npc = cPickle.loads(decompress(npcRaw.decode('hex')))
            except TypeError:
                # k, try old way, using base64...
                npc = cPickle.loads(decompress(npcRaw.decode('base64')))
            self.npcStack.append(npc)
            self.displayNPC(npc)
            # renable relevant buttons/tools
            for id in (890, 891, 892, 901, 902):
                self.toolBar.EnableTool(id, 1)
        dlg.Destroy()


    def onSaveAs(self, event):
        'Write out self.logWindow contents to User def. File'
        if wx.Platform == kHTMLEnabledPlatform:
            wildCardFlags        = 'HTML files (*.html)|*.html|Text files (*.txt)|*.txt|All files (*.*)|*.*'
            if not self.displayHTML:
                wildCardFlags    = 'Text files (*.txt)|*.txt|HTML files (*.html)|*.html|All files (*.*)|*.*'
        else:
            wildCardFlags        = 'Text files (*.txt)|*.txt|All files (*.*)|*.*'
        defaultDir                = kRootDir + os.sep + 'sav'
        style                    = wx.SAVE|wx.OVERWRITE_PROMPT
        npc                        = self.npcStack.getCurrent()
        if npc:
            saveName = npc.mData['class']

            dlg = wx.FileDialog(self, 'Choose a file', defaultDir, saveName, wildCardFlags, style)
            if dlg.ShowModal() == wx.ID_OK:
                fileName = dlg.GetPath()
                saveFile = codecs.open(fileName, 'w', 'utf-8')
                # determine saveType, i.e., if npcString will be html or not
                if fileName.endswith('html'):
                    npcString = NPCBuilder.GetHTMLDisplay(npc, 0, 1)
                else:
                    npcString = NPCBuilder.GetTextDisplay(npc, 0)
                saveFile.write(npcString)
                saveFile.close()
                self.SetStatusText("%s Saved." % fileName)
            dlg.Destroy()


    def onReload(self, event):
        'reload all the xmldata'
        self.SetStatusText('**Please Wait**  Reloading Data from XML ...')
        try:
            self.loadDataInteral(reload=1)
            self.GetToolBar().Refresh()
            self.displayNPC(self.npcStack.getCurrent())
            self.SetStatusText('All XML Data has been reloaded')
        except SAXParseException, why:
            self.logWindow.LoadString(str(why))
            self.SetStatusText('XML Data had Errors, FAILED to Reload')


    def loadDataInteral(self, reload):
        'interal method for loading xml data into various UI elements (such as combobox)'
        xmlData = self.getXMLData(reload)
        if self.parentsTreeFrame and self.parentsTreeFrame.IsShown():
            self.onCloseParentTreeWindow()
            self.showParentsTreeFrame(event=None, reload=0)

        # purge the Names cache
        charactermaker.EmptyNamesCacheMain()

        # reload cache and all the class names for the combox too
        selection = self.characterType.GetValue()
        self.characterType.Clear()

        for name in self.getClassNames(xmlData):
            self.characterType.Append(name)
        index = self.characterType.FindString(selection)
        if index == -1:
            index = 0
        self.characterType.SetSelection(index)


    def onLogWindowSwitch(self, event=None):
        if wx.Platform == kHTMLEnabledPlatform:
            size = self.GetSizeTuple()
            position = self.GetPositionTuple()
            #print dir(self.logWindow)
            #self.logWindow.Clear()
            self.logWindow.Destroy()

            if not self.displayHTML:
                self.createIEWindow()
                self.displayHTML = 1
            else:
                self.createLogWindow()
                self.displayHTML = 1

            self.Fit()
            self.SetSize(size)
            self.SetPosition(position)
            self.GetToolBar().Refresh()
            self.displayNPC(self.npcStack.getCurrent())
        else:
            self.logWindow.Clear()
            warningMessage = 'Platform %s does not support IE HTML' % wx.Platform
            self.logWindow.write(warningMessage)
            self.SetStatusText(warningMessage)
            self.htmlMode.Check(0)


    def onGen(self, event=None, pointTarget=0):
        'gen the mofo npcs already...'
        try:
            shortClassName                = self.lastShortClassName
            number                        = self.quantity.GetValue()
            #number = 1000
            longClassName                = self.characterType.GetValue()
            if longClassName:
                genre, shortClassName    = self.getShortClassName(longClassName)
                xmlData                    = self.getXMLData()
                alias                    = self.getClassAliasMap(xmlData)[shortClassName]
                power                    = self.getRelativePower(self.slider.GetValue())

                # get statusName
                if (int(number) > 1) and genre:
                    statusName = genre.capitalize()
                else:
                    statusName = shortClassName

                # start times and set status
                startTime = time()
                self.SetStatusText('**Please Wait**  Creating %s %s NPC(s) ...' % (number, statusName))

                if longClassName[0] == '*':
                    # ok, this is a set of randoms...
                    for i in range(int(number)):
                        if not i:
                            randomAlias = alias
                        else:
                            randomAlias = self.getClassAliasMap(xmlData)[self.getShortClassName(longClassName)[1]]
                        if pointTarget:
                            self.generateNPCWithTargetPoints(i, xmlData, randomAlias, power, i)
                        else:
                            charactermaker.Generate((randomAlias, 1), xmlData, power, self.mode, startPos=i, stack=self.npcStack, display=0)
                else:
                    if pointTarget:
                        for i in range(int(number)):
                            self.generateNPCWithTargetPoints(i, xmlData, alias, power, i)
                    else:
                        charactermaker.Generate((alias, number), xmlData, power, self.mode, stack=self.npcStack, display=0)

                # display each generated
                #for npc in self.npcStack[-number:]:
                    #self.onSaveToDB(npc=npc)
                    #self.displayNPC(npc)
                self.displayNPC(self.npcStack[-1])

                # renable relevant buttons/tools
                for id in (890, 891, 892, 901, 902):
                    self.toolBar.EnableTool(id, 1)

                # finish status
                #self.SetStatusText('')
                self.lastStatus = 'Finished creating %s %s NPC(s) in %s' % (number, statusName, GetElapsedTime(startTime))
                self.SetStatusText(self.lastStatus)
                #print 'finished creating...'
            else:
                self.SetStatusText('No target Selected')
        except Exception, why:
            self.logWindow.WriteText('\n\nFailed to generate NPC [%s] %s: %s' % (genre, shortClassName, str(why)))
            self.SetStatusText('Failed to generate NPC')
            self.logCurrentExceptionTrace()


    def onGenTargetPoints(self, event):
        'sync data with server'
        longClassName = self.characterType.GetValue()[1:]
        if ']' in longClassName:
            longClassName = longClassName[longClassName.find(']')+3:]
        dialog = TargetPointsDialog(self, -1, 'Generate %s %s NPC(s) -- Select approximate Target Point Value:    ' % (self.quantity.GetValue(), longClassName), self.target, self.precision)
        if dialog.ShowModal() == wx.ID_OK:
            self.target = dialog.target
            self.precision = dialog.precision
            self.onGen(pointTarget=1)
        dialog.Destroy()

    def generateNPCWithTargetPoints(self, npcNumber, xmlData, alias, power, start):
        'generate an npc with a point target'
        minPower = 0.1
        maxPower = 2.0
        minTarget = self.target - self.precision
        maxTarget = self.target + self.precision
        n = 1
        while (1):
            self.SetStatusText('NPC: %s, Attempt number: %s ...' % (npcNumber + 1, n))
            npc = charactermaker.GetCharacters(xmlData, alias, power, start=0, total=1)[0]
            if minTarget < npc.getTotalPoints() < maxTarget:
                self.npcStack.append(npc)
                break
            elif npc.getTotalPoints() < self.target:
                power += .05
                if power > maxPower: power = maxPower
            elif npc.getTotalPoints() > self.target:
                power -= .05
                if power < minPower: power = minPower
            self.slider.SetValue(self.getNPCPower(npc))
            n += 1


    def displayPreviousNPC(self, event=None):
        'display the previous npc in the stack'
        npc = self.npcStack.previous()
        if npc:
            self.toolBar.EnableTool(892, 1)
            self.displayNPC(npc)
        else:
            self.toolBar.EnableTool(890, 0)

    def displayNextNPC(self, event=None):
        'display the next npc in the stack'
        npc = self.npcStack.next()
        if npc:
            self.toolBar.EnableTool(890, 1)
            self.displayNPC(npc)
        else:
            self.toolBar.EnableTool(892, 0)

    def displayNPC(self, npc):
        'display an npc'
        if npc:
            if not self.displayHTML:
                self.onClear()
                # renable relevant buttons/tools
                self.toolBar.EnableTool(901, 1)
                self.toolBar.EnableTool(902, 1)
            #npc.display(mode=1)
            if not self.displayHTML:
                npcString = NPCBuilder.GetHTMLDisplay(npc, useWebKitPaths=0, prependHTMLHeaders=1)
            else:
                npcString = NPCBuilder.GetTextDisplay(npc, mode=1)
            self.logWindow.writeOne(npcString)
            self.slider.SetValue(self.getNPCPower(npc))
            self.SetStatusText('%s Selected.' % self.getSimpleNPCTitle(npc))
            #print 'display npc slected?'
            #print '-' * 40

    def getSimpleNPCTitle(self, npc):
        return ('%s: %s' % (npc.getFullRankName(), npc.mData['class']))

    def onShowNPCStack(self, event=None):
        'display a single choice dialog of all available npcs in buffer'
        dlg = wx.SingleChoiceDialog(self, 'Choose an NPC and click OK to display:', 'NPCs available', 
                                   [self.getSimpleNPCTitle(npc) for npc in self.npcStack], wx.CHOICEDLG_STYLE)
        if self.npcStack.getCurrentIndex():
            dlg.SetSelection(self.npcStack.getCurrentIndex())
        if dlg.ShowModal() == wx.ID_OK:
            currentNPC = self.npcStack.getCurrent()
            npc = self.npcStack.get(dlg.GetSelection())
            #if npc and (npc != currentNPC):
            self.displayNPC(npc)
            self.SetStatusText('%s Selected.' % dlg.GetStringSelection())
        dlg.Destroy()


    def onPrint(self, event):
        'print, no setup'
        if self.displayHTML:
            printDialogue = wx.MessageDialog(self, 'To print HTML, Right-Click screen and Select Print', 'Print Help', wx.OK | wx.ICON_INFORMATION)
            printDialogue.ShowModal()
            printDialogue.Destroy()
        else:
            #self.logWindow.OnDoPrint(event)
            htmlString = GetSimpleHTML(self.npcStack.getCurrent().getDisplay(mode=0), self.encoding)
            htmlprint = HtmlEasyPrinting()
            htmlprint.PrintText(htmlString)
            #htmlprint.PreviewText(htmlString)


    def persistView(self):
        'Size and Position (test for invalid size, dont set if invalid)'
        sizeTuple = self.GetSizeTuple()
        if sizeTuple != (160, 24):
            self.positionTable['size'] = sizeTuple
            self.positionTable['position'] = self.GetPositionTuple()
            self.positionTable['number'] = self.quantity.GetValue()
            self.positionTable['power'] = self.slider.GetValue()
            self.positionTable['selection'] = self.characterType.GetSelection()
        
        if self.parentsTreeFrameSize:
            self.positionTable['parentsTreeSize'] = self.parentsTreeFrameSize


    def saveDefaultView(self, event):
        'persist positionTable'
        self.persistView()

        try:
            persistentFile = open(self.sSettingsFileName, 'wb')
            cPickle.dump(self.positionTable, persistentFile, 1)
            persistentFile.close()
            self.SetStatusText("%s Saved." % self.sSettingsFileName)
        except (OSError, IOError), why:
            self.SetStatusText("%s NOT Saved.  File System is not writeable!" % self.sSettingsFileName)


    def loadDefaultSettings(self):
        'load default settings from settings file'
        persistentFile = open(self.sSettingsFileName, 'rb')
        positionTable = cPickle.load(persistentFile)
        persistentFile.close()

        self.loadView(positionTable)
        self.SetStatusText("%s Loaded." % self.sSettingsFileName)


    def loadView(self, positionTable):
        'load the view stuff from a default settings file'
        if positionTable['size']:
            self.SetSize(positionTable['size'])
        if positionTable['position']:
            self.SetPosition(positionTable['position'])
        if positionTable.has_key('selection'):
            self.characterType.SetSelection(positionTable['selection'])
        if positionTable.has_key('number'):
            self.quantity.SetValue(int(positionTable['number']))
        if positionTable.has_key('power'):
            self.slider.SetValue(int(positionTable['power']))
        if positionTable.has_key('parentsTreeSize') and positionTable['parentsTreeSize']:
            self.parentsTreeFrameSize = positionTable['parentsTreeSize']
        if 'uploadPassword' not in positionTable:
            positionTable['uploadPassword'] = ''
        if 'RPCServerURI' not in positionTable:
            positionTable['RPCServerURI'] = kDefaultRPCServerURI
        if 'RPCServerProxy' not in positionTable:
            positionTable['RPCServerProxy'] = kDefaultRPCServerProxy
        if 'useRPCProxy' not in positionTable:
            positionTable['useRPCProxy'] = 0
        self.positionTable = positionTable
        self.onSelect()


    def showParentsTreeFrame(self, event, reload=0):
        'return a local pointer to parentsTreeFrame, wx.MiniFrame'
        if not self.parentsTreeFrame or not self.parentsTreeFrame.IsShown():
            self.parentsTreeFrame = wx.MiniFrame(self, -1, 'NPC Parents Tree', wx.DefaultPosition, wx.DefaultSize,
                                    wx.DEFAULT_FRAME_STYLE | wx.TINY_CAPTION_HORIZ)
            wx.EVT_CLOSE(self.parentsTreeFrame, self.onCloseParentTreeWindow)

            self.parentsTreeFrame.Show(1)
            self.parentsTreeFrame.tree = CreateParentsTree(self.parentsTreeFrame, self.encoding, self.getXMLData(reload))
            self.setParentsTreeFrameSize()


    def setParentsTreeFrameSize(self):
        'hmm, something is wrong with using wx.DefaultSize for children of wx.MiniFrame...'
        if self.parentsTreeFrame:
            if self.parentsTreeFrameSize:
                size = self.parentsTreeFrameSize
            else:
                size = wx.Size(500, 500)
            self.parentsTreeFrame.SetSize(size)


    def onCloseParentTreeWindow(self, event=None):
        self.parentsTreeFrameSize = self.parentsTreeFrame.GetSizeTuple()
        self.parentsTreeFrame.Show(0)
        self.parentsTreeFrame.Destroy()


    def onSync(self, event=None):
        'sync data with server'
        dialog = SyncDialog(self, -1, "Synchronize Data with Server")
        if event:
            dialog.ShowModal()
        else:
            #pass
            dialog.autoDownload()


    def onLoadFromServer(self, event):
        'load template data (into self.xmlCache) directly from server -- do not store locally'
        style = wx.OK | wx.CANCEL | wx.CENTRE
        serverDialog = wx.TextEntryDialog(self, 'Server Address:', 'Load Template Data from RPC Server', self.positionTable['RPCServerURI'], style=style)
        if serverDialog.ShowModal() == wx.ID_OK:
            startTime = time()
            serverURI = serverDialog.GetValue()
            #hmmm... 
            self.positionTable['RPCServerURI'] = serverURI
            # use URLLibTransport so we can use http proxy on this as well
            server = ServerProxy(serverURI, transport=URLLibTransport())
            self.SetStatusText("Loading Data from: %s...." % serverURI)
            self.xmlCache = cPickle.loads(decompress(server.getTemplateDataFromServer().data))
            self.loadDataInteral(reload=0)
            self.SetStatusText("Loading Name Cache from: %s...." % serverURI)
            charactermaker.ReplaceNamesCacheMain(cPickle.loads(decompress(server.getFullNamesCache().data)))
            self.SetStatusText("Template Data loaded from %s: %s" % (serverURI, GetElapsedTime(startTime)))

        serverDialog.Destroy()


    def quit(self, event):
        'exit'
        self.saveDefaultView(None)
        self.Destroy()



kHelpText = '''GURPS NPC Generator is tool, or game aid, typically used by a Game Master,
for quickly generating NPCs (Non-Player Characters) for the GURPS role-playing
system.  It can be used create multiple NPCs (their names, appearance, stats, 
skills, and even weapons and armor) of varying power for many different genres.  
The NPC Generator closely follows the GURPS rules for creating all character 
types and their attributes, including their appearance, gender, nationality, 
stats, advantages, disadvantages, quirks and skills.'''


if __name__ == '__main__':
    if len(args) > 1:
        charactermaker.Main(args)
    else:
        app = wx.PySimpleApp()
        frame = NPCFrame(None, -1, 'GURPS NPC Generator')
        app.MainLoop()

