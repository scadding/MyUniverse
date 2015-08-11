#!/usr/bin/env python

import wx
import os
import glob
import imp
import  wx.html as  html
import wx.html2 as webview
import  wx.lib.wxpTag
import  images
import wx.aui
import codecs

from GeneratorPanel import GeneratorPanel
from Generators.TableGenerator import TableGenerator
from Generators.SectorGenerator import SectorGenerator
from Generators.SystemGenerator import SystemGenerator
from Generators.PlanetGenerator import PlanetGenerator
from Generators.CharacterGenerator import CharacterGenerator
from Generators.PlanetImageGenerator import PlanetImageGenerator

FRAMETB = True
TBFLAGS = ( wx.TB_HORIZONTAL
            | wx.NO_BORDER
            | wx.TB_FLAT
            #| wx.TB_TEXT
            #| wx.TB_HORZ_LAYOUT
            )

class TestSearchCtrl(wx.SearchCtrl):
    maxSearches = 5
    
    def __init__(self, parent, id=-1, value="",
                 pos=wx.DefaultPosition, size=wx.DefaultSize, style=0,
                 doSearch=None):
        style |= wx.TE_PROCESS_ENTER
        wx.SearchCtrl.__init__(self, parent, id, value, pos, size, style)
        self.Bind(wx.EVT_TEXT_ENTER, self.OnTextEntered)
        self.Bind(wx.EVT_MENU_RANGE, self.OnMenuItem, id=1, id2=self.maxSearches)
        self.doSearch = doSearch
        self.searches = []

    def OnTextEntered(self, evt):
        text = self.GetValue()
        if self.doSearch(text):
            self.searches.append(text)
            if len(self.searches) > self.maxSearches:
                del self.searches[0]
            self.SetMenu(self.MakeMenu())            
        self.SetValue("")

    def OnMenuItem(self, evt):
        text = self.searches[evt.GetId()-1]
        self.doSearch(text)
        
    def MakeMenu(self):
        menu = wx.Menu()
        item = menu.Append(-1, "Recent Searches")
        item.Enable(False)
        for idx, txt in enumerate(self.searches):
            menu.Append(1+idx, txt)
        return menu
    

class MyFrame(wx.Frame):
    def __init__(self, *args, **kwds):
        self.ctrl = False
        self.h = dict()
        self.timer = None
        kwds["style"] = wx.DEFAULT_FRAME_STYLE
        wx.Frame.__init__(self, *args, **kwds)
        
        self.MenuBar()
        self.ToolBar()
        
        self.CreateStatusBar()
                
        self.notebook_2 = wx.aui.AuiNotebook(self)
        self.generators = dict()
        self.generators['Character'] = GeneratorPanel(self.notebook_2, CharacterGenerator())
        self.generators['Table'] = GeneratorPanel(self.notebook_2, TableGenerator())
        self.generators['Sector'] = GeneratorPanel(self.notebook_2, SectorGenerator())
        self.generators['System'] = GeneratorPanel(self.notebook_2, SystemGenerator())
        self.generators['Image'] = GeneratorPanel(self.notebook_2, PlanetImageGenerator())
        self.generators['Planet'] = GeneratorPanel(self.notebook_2, PlanetGenerator())
        
        self.notebook_1 = wx.aui.AuiNotebook(self)
                        
        self.txtResults = wx.TextCtrl(self, -1, "", style=wx.TE_MULTILINE)
        # Events
        self.Bind(wx.aui.EVT_AUINOTEBOOK_PAGE_CLOSE, self.OnPaneClosing, self.notebook_1)

        self.__set_properties()
        # end wxGlade
        self.__do_layout()
        self.Populate('about', '')
        
    def __set_properties(self):
        self.SetTitle("RPG Generator")
        self.SetSize((1000, 600))
        self.cboRolls.SetSelection(0)

    def __do_layout(self):
        sizer_2 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_7 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_2.Add(sizer_7, 2, wx.EXPAND, 0)
        
        self.txtResults.Show(False)
                        
        for t in self.generators:
            self.generators[t].do_layout()
        
        for t in self.generators:
            self.notebook_2.AddPage(self.generators[t], t)

        sizer_2.Add(self.notebook_1, 7, wx.ALIGN_RIGHT | wx.EXPAND, 0)
        sizer_7.Add(self.notebook_2, 3, wx.ALIGN_RIGHT | wx.EXPAND, 0)

        self.SetSizer(sizer_2)
        self.Layout()
    
    def ToolBar(self):
        tb = self.CreateToolBar( TBFLAGS )
        tsize = (24,24)

        new_bmp =  wx.ArtProvider.GetBitmap(wx.ART_NEW, wx.ART_TOOLBAR, tsize)
        open_bmp = wx.ArtProvider.GetBitmap(wx.ART_FILE_OPEN, wx.ART_TOOLBAR, tsize)
        copy_bmp = wx.ArtProvider.GetBitmap(wx.ART_COPY, wx.ART_TOOLBAR, tsize)
        paste_bmp= wx.ArtProvider.GetBitmap(wx.ART_PASTE, wx.ART_TOOLBAR, tsize)

        tb.SetToolBitmapSize(tsize)

        #tb.AddSimpleTool(10, new_bmp, "New", "Long help for 'New'")
        tb.AddLabelTool(10, "New", new_bmp, shortHelp="New", longHelp="Long help for 'New'")
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=10)
        self.Bind(wx.EVT_TOOL_RCLICKED, self.OnToolRClick, id=10)

        #tb.AddSimpleTool(20, open_bmp, "Open", "Long help for 'Open'")
        tb.AddLabelTool(20, "Open", open_bmp, shortHelp="Open", longHelp="Long help for 'Open'")
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=20)
        self.Bind(wx.EVT_TOOL_RCLICKED, self.OnToolRClick, id=20)

        tb.AddSeparator()
        tb.AddSimpleTool(30, copy_bmp, "Copy", "Long help for 'Copy'")
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=30)
        self.Bind(wx.EVT_TOOL_RCLICKED, self.OnToolRClick, id=30)

        tb.AddSimpleTool(40, paste_bmp, "Paste", "Long help for 'Paste'")
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=40)
        self.Bind(wx.EVT_TOOL_RCLICKED, self.OnToolRClick, id=40)

        tb.AddSeparator()

        #tool = tb.AddCheckTool(50, images.Tog1.GetBitmap(), shortHelp="Toggle this")
        tool = tb.AddCheckLabelTool(50, "Checkable", images.Tog1.GetBitmap(),
                                    shortHelp="Toggle this")
        self.Bind(wx.EVT_TOOL, self.OnToolClick, id=50)

        self.Bind(wx.EVT_TOOL_ENTER, self.OnToolEnter)
        self.Bind(wx.EVT_TOOL_RCLICKED, self.OnToolRClick) # Match all
        self.Bind(wx.EVT_TIMER, self.OnClearSB)

        tb.AddSeparator()
        cbID = wx.NewId()

        tb.AddControl(
            wx.ComboBox(
                tb, cbID, "", choices=["", "This", "is a", "wx.ComboBox"],
                size=(150,-1), style=wx.CB_DROPDOWN
                ))
        self.Bind(wx.EVT_COMBOBOX, self.OnCombo, id=cbID)
        tb.AddSeparator()
        
        btnRoll = wx.Button(tb, -1, "Roll", style = wx.BU_EXACTFIT)
        tb.AddControl(btnRoll)
        self.Bind(wx.EVT_BUTTON, self.OnRoll, btnRoll)
        self.cboRolls = wx.ComboBox(tb, -1, choices=["1", "5", "10", "15", "20", "25", "100", "1000"], style=wx.CB_DROPDOWN)
        tb.AddControl(self.cboRolls)

        tb.AddStretchableSpace()
        search = TestSearchCtrl(tb, size=(150,-1), doSearch=self.DoSearch)
        tb.AddControl(search)

        # Final thing to do for a toolbar is call the Realize() method. This
        # causes it to render (more or less, that is).
        tb.Realize()
    
    def DoSearch(self,  text):
        # called by TestSearchCtrl
        self.SetStatusText("Searching for: " + text)
        #("DoSearch: %s\n" % text)
        # return true to tell the search ctrl to remember the text
        return True


    def OnToolClick(self, event):
        #print("tool %s clicked\n" % event.GetId())
        #tb = self.GetToolBar()
        tb = event.GetEventObject()
        tb.EnableTool(10, not tb.GetToolEnabled(10))

    def OnToolRClick(self, event):
        print("tool %s right-clicked\n" % event.GetId())

    def OnCombo(self, event):
        print("combobox item selected: %s\n" % event.GetString())

    def OnToolEnter(self, event):
        #('OnToolEnter: %s, %s\n' % (event.GetId(), event.GetInt()))

        if self.timer is None:
            self.timer = wx.Timer(self)

        if self.timer.IsRunning():
            self.timer.Stop()

        self.timer.Start(2000)
        event.Skip()
    def OnClearSB(self, event):  # called for the timer event handler
        self.SetStatusText("")
        self.timer.Stop()
        self.timer = None
        
    def MenuBar(self):
        # Menu Bar
        self.frame_1_menubar = wx.MenuBar()
        wxglade_file_menu = wx.Menu()
        wxglade_edit_menu = wx.Menu()
        wxglade_generate_menu = wx.Menu()
        self.menuExit = wxglade_file_menu.Append(wx.ID_EXIT, "&Quit", "", wx.ITEM_NORMAL)
        self.menuCopy = wxglade_edit_menu.Append(wx.ID_COPY, "&Copy\tCtrl+C", "", wx.ITEM_NORMAL)
        self.menuGen = wxglade_generate_menu.Append(wx.NewId(), "&Roll\tCtrl+R", "", wx.ITEM_NORMAL)
        self.frame_1_menubar.Append(wxglade_file_menu, "&File")
        self.frame_1_menubar.Append(wxglade_edit_menu, "&Edit")
        self.frame_1_menubar.Append(wxglade_generate_menu, "&Generate")
        self.SetMenuBar(self.frame_1_menubar)
        # Menu Bar end
        self.Bind(wx.EVT_MENU, self.OnExit, self.menuExit)
        self.Bind(wx.EVT_MENU, self.OnRoll, self.menuGen)
        self.Bind(wx.EVT_MENU, self.OnCopy, self.menuCopy)

    def OnExit(self, e):
        self.Close(True)
    
    def OnRoll(self, e):
        numRolls = int(self.cboRolls.GetStrings()[self.cboRolls.GetSelection()])
        current = self.notebook_2.GetCurrentPage()
        name = self.notebook_2.GetPageText(self.notebook_2.GetPageIndex(current))
        t, r = current.Roll(numRolls)
        self.Populate(t, r)
        
    def Populate(self, name, content=u'', file=''):
        if self.h.has_key(name):
            html = self.h[name]
            for i in range(self.notebook_1.GetPageCount()):
                if self.notebook_1.GetPageText(i) == name:
                    self.notebook_1.SetSelection(i)
                    break
        else:
            sizer_7 = wx.BoxSizer(wx.HORIZONTAL)
            notebook_pane = wx.Panel(self.notebook_1, wx.ID_ANY)
            #html = MyHtmlWindow(notebook_pane, -1)
            html = webview.WebView.New(notebook_pane)
            sizer_7.Add(html, 10, wx.EXPAND, 0)
            notebook_pane.SetSizer(sizer_7)
            self.notebook_1.AddPage(notebook_pane, name)
            self.notebook_1.SetSelection(self.notebook_1.GetPageCount() - 1)
            self.h[name] = html
            self.Bind(webview.EVT_WEBVIEW_NAVIGATING, self.OnNavigate, html)

        if file != '':
            html.SetPage("", name)
            html.LoadURL(file)
            self.Layout()
        else:
            f = codecs.open(name + '.html', 'w', "utf-8")
            u = ''
            if content.__class__.__name__ == "unicode":
                u = content
            else:
                u = unicode(content, "utf-8")
            f.write(u)
            f.close()
            html.SetPage("", name)
            path = os.getcwd()
            url = "file://" + path + "/" + name + ".html"
            html.LoadURL(url)
            self.Layout()
            #os.remove(name + '.html')

    def OnPaneClosing(self, event):
        current = event.GetSelection()
        name = self.notebook_1.GetPageText(current)
        if self.h.has_key(name):
            del self.h[name]
        # 'closing ' + name
        
    def OnNavigate(self, event):
        url = event.GetURL()
        if url[0:4] == "genr":
            s = url.replace('%20', ' ')[7:]
            #print s
            i = s.find(' ')
            g = s[:i]
            s = s[i+1:]
            #print g
            p = dict()
            while s != '':
                i = s.find(' ')
                a = s[:i]
                s = s[i+1:]
                i = s.find(' ')
                if i > 0:
                    v = s[:i]
                    s = s[i+1:]
                else:
                    v = s
                    s = ''
                p[a] = v
            event.Veto()
            t, r = self.generators[g].generator.roll(p, 1)
            self.Populate(t, r)

    def OnCopy(self, e):
        t = self.html.SelectionToText()
        clipdata = wx.TextDataObject()
        clipdata.SetText("Hi folks!")
        wx.TheClipboard.Open()
        wx.TheClipboard.SetData(clipdata)
        wx.TheClipboard.Close()

