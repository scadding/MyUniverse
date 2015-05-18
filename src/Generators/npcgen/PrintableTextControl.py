#!/progra~1/python/python

# filename.py

'''
simple printable text control for wx.Python
prntxtwx.py
jjk  01/06/00  001  from txtviewwx.py 005
jjk  01/07/00  002  
jjk  01/12/00  003b documentation updates

see also demo_prntxtwx.py

latest version available at http://starship.python.net/crew/jjkunce

Tested only on WinNT.

*** USE AT YOUR OWN RISK ***


PrintableTextControl can be used wherever a wxTextCtrl might be used.

Differences between PrintableTextControl and wxTextCtrl:

	-The default style for PrintableTextControl is wxTE_MULTILINE
	since it doesn't usually make sense to print a single line control

Additional features in PrintableTextControl:
	
	-Setting Printer Margins. The margins set by the user are 
	values IN ADDITION to any hardware margins the printer sets.
	Margins may be set in inches or millimeters.
	Examples:
		myText.setPrintMarginsIn(upperLeft=(0.5,0.75), lowerRight=(0.5,0.5))
		myText.setPrintMarginsMM(upperLeft=(13,19), lowerRight=(13,13))
	also:
		ul, lr = myText.getPrintMarginsIn()
		ul, lr = myText.getPrintMarginsMM()
	

	-Setting the print line wrap options. Line wrap on print may be set to:
		-print line wrap OFF (PTC_LINEWRAP_OFF)
			Long lines will be truncated. 
		-print line wrap ON (PTC_LINEWRAP_OFF)
			Long lines will be broken at character before right margin, and continued  
			at left margin on next line.
		-print line wrap same as screen control (PTC_LINEWRAP_SAMEASCONTROL) [DEFAULT]
			Horizontal scroll bar = print line wrap off
	Example:
		myText.setPrintLinewrap(PTC_LINEWRAP_OFF)
	also:
		lw = myText.getPrintLinewrap()
		

	-Setting up the printing parameters. Display a Print Setup dialog and accept
	input from the user. Settings are remembered between invocations.
	Example:
		myText.OnPrintSetup(event)
		
		
	-Printing the contents of the control.
	Example:
		myText.OnDoPrint(event)
		
'''

import string

#from wxPython.wx import *
import wx

import sys

PTC_LINEWRAP_SAMEASCONTROL = -1
PTC_LINEWRAP_OFF = 0
PTC_LINEWRAP_ON = 1

class PrintableTextControl(wx.TextCtrl):
	'''a text control that can be printed
	jjk  01/06/00'''
	
	MMperIN = 25.4
	
	def __init__(self, parent, id=-1, value='', pos=wx.Point(0, 0), size=wx.DefaultSize, style=wx.TE_MULTILINE, name='text'):    # | wx.HSCROLL
		'''jjk  01/06/00'''
		wx.TextCtrl.__init__(self, parent, id, value, pos, size, style, name=name)
		self.printData = wx.PrintData()
		self.printData.SetPaperId(wx.PAPER_LETTER)
		self.setPrintMarginsIn()
		self.setPrintLinewrap()

	def setPrintMarginsIn(self, upperLeft=(0.5,0.75), lowerRight=(0.5,0.5)):
		'''public: set the printing margins in inches
		jjk  01/07/00'''
		self.printMarginsIn = [upperLeft, lowerRight]

	def getPrintMarginsIn(self):
		'''public: answer the printing margins in inches
		jjk  01/11/00'''
		return(self.printMarginsIn)

	def setPrintMarginsMM(self, upperLeft=(13,19), lowerRight=(13,13)):
		'''public: set the printing margins in millimeters
		jjk  01/11/00'''
		ulin = (upperLeft[0]/self.MMperIN, upperLeft[1]/self.MMperIN)
		lrin = (lowerRight[0]/self.MMperIN, lowerRight[1]/self.MMperIN)
		self.setPrintMarginsIn(ulin, lrin)

	def getPrintMarginsMM(self):
		'''public: set the printing margins in millimeters
		jjk  01/11/00'''
		ulmm = (self.PrintMarginsIn[0][0]*self.MMperIN, self.PrintMarginsIn[0][1]*self.MMperIN)
		lrmm = (self.PrintMarginsIn[1][0]*self.MMperIN, self.PrintMarginsIn[1][1]*self.MMperIN)
		return(ulmm,lrmm)

	def setPrintLinewrap(self, linewrapCode=PTC_LINEWRAP_SAMEASCONTROL):
		'''public: set the line wrap behavior for printing 
		jjk  01/07/00'''
		self.printLinewrap = linewrapCode

	def getPrintLinewrap(self):
		'''public: answer the print line wrap code
		jjk  01/11/00'''
		return(self.printLinewrap)

	def OnPrintSetup(self, event):
		'''public: call this to set up printing
		jjk  01/06/00'''
		printerDialog = wx.PrintDialog(self)
		printerDialog.GetPrintDialogData().SetPrintData(self.printData)
		printerDialog.GetPrintDialogData().SetSetupDialog(true)
		printerDialog.ShowModal();
		self.printData = printerDialog.GetPrintDialogData().GetPrintData()
		printerDialog.Destroy()

	def OnDoPrint(self, event):
		'''public: call this to print the contents of the receiver
		jjk  01/02/00'''
		pdd = wx.PrintDialogData()
		pdd.SetPrintData(self.printData)
		printer = wx.Printer(pdd)
		printout = TextControlPrintout(self)
		if not printer.Print(self, printout):
			pass
			# wx.MessageBox("There was a problem printing.\nPerhaps your current printer is not set correctly?", "Printing", wx.OK)
		else:
			self.printData = printer.GetPrintDialogData().GetPrintData()
		printout.Destroy()

	def doPrinting(self, dc, nextLineToPrint=0, printerPixelsPerInch=(300,300)):
		'''system: called by TextControlPrintout to print the next page
		jjk  01/07/99'''
		dc.BeginDrawing()
		dc.SetFont(self.GetFont())

		(ulx,uly),(lrx,lry) = self.computePrintMargins(dc, printerPixelsPerInch)
		wrapLinesFlag = self.wrapPrintLines()

		x1 = ulx
		y1 = uly
		allLinesPrinted = 0
		while (1):
			if (nextLineToPrint>=self.GetNumberOfLines()):
				allLinesPrinted = 1
				break
			line = self.GetLineText(nextLineToPrint)
			lines, lineExtents, allLinesHeight = self.wrapLine(dc, line, ulx, lrx, wrapLinesFlag)
			if (y1+allLinesHeight>lry):
				break
			for i1 in range(len(lines)):
				y1 = y1 + lineExtents[i1][1]
				dc.DrawText(lines[i1], x1, y1)
			nextLineToPrint = nextLineToPrint + 1
		dc.EndDrawing()
		return(nextLineToPrint, allLinesPrinted)
			
	def computePrintMargins(self, dc, printerPixelsPerInch=(300,300)):
		'''private: answer the margins converted to scaled print pixels
		jjk  01/07/99'''
		ulx = dc.DeviceToLogicalX(self.printMarginsIn[0][0]*printerPixelsPerInch[0])
		uly = dc.DeviceToLogicalY(self.printMarginsIn[0][1]*printerPixelsPerInch[1])
		pwid, phgt = dc.GetSizeTuple()
		lrx = dc.DeviceToLogicalX(pwid-(self.printMarginsIn[1][0]*printerPixelsPerInch[0]))
		lry = dc.DeviceToLogicalX(phgt-(self.printMarginsIn[1][1]*printerPixelsPerInch[1]))
		return((ulx,uly),(lrx,lry))

	def wrapPrintLines(self):
		'''private: answer true if lines are to be wrapped on printout
		jjk  01/07/99'''
		if (self.printLinewrap==PTC_LINEWRAP_OFF):
			wrapLines=0
		elif (self.printLinewrap==PTC_LINEWRAP_ON):
			wrapLines=1
		else:
			if (self.GetWindowStyleFlag()&wx.HSCROLL):
				wrapLines=0
			else:
				wrapLines=1
		return(wrapLines)

	def wrapLine(self, dc, line, leftMargin=0.0, rightMargin=0.0, wrapLinesFlag=1):
		'''private: break up line as needed for current margins and linewrap settings.
		answer a list of lines, a list of the line text extents, and the combined height of all the lines
		jjk  01/07/99'''
		if (len(line)==0):
			lines = [line]
			lext = dc.GetTextExtent('X')
			lineExtents = [lext]
			allLinesHeight = lext[1]
		else:
			maxLineWidth = rightMargin - leftMargin
			lines = []
			lineExtents = []
			allLinesHeight = 0
			while(len(line)>0):
				llen = len(line)+1
				while(1):
					lext = dc.GetTextExtent(line[:llen])
					if (lext[0] < maxLineWidth):
						lines.append(line[:llen])
						lineExtents.append(lext)
						allLinesHeight = allLinesHeight + lext[1]
						line = line[llen:]
						break
					llen = llen-1    #not very smart - only one character at a time
				if (not wrapLinesFlag):
					break
		return(lines, lineExtents, allLinesHeight)
		


class TextControlPrintout(wx.Printout):
	'''system private class: a wx.Printout for instances of class PrintableTextControl
	jjk  01/06/00'''

	def __init__(self, canvas):
		'''jjk  01/07/00'''
		wx.Printout.__init__(self)
		self.canvas = canvas

	def HasPage(self, page):
		'''jjk  01/06/00'''
		return(not self.allLinesPrinted)

	def GetPageInfo(self):
		'''jjk  01/06/00'''
		return (0, 1, 1, 32000)    #min pg, max pg, pg from, pg to
		
	def OnPreparePrinting(self):
		'''jjk  01/06/00'''
		self.nextLineToPrint = 0
		self.allLinesPrinted = 0
		self.base_OnPreparePrinting()

	def OnBeginPrinting(self):
		'''jjk  01/07/00'''
		dc = self.GetDC()
		if (self.IsPreview()):
			self.pixelsPerInch = self.GetPPIScreen()
		else:
			self.pixelsPerInch = self.GetPPIPrinter()
		te1 = dc.GetTextExtent('X') 
		te2 = self.canvas.GetTextExtent('X')
		self.printUserScale = float(te1[1]) / te2[1]
		self.base_OnBeginPrinting()
		
	def OnPrintPage(self, page):
		'''jjk  01/07/00'''
		dc = self.GetDC()
		dc.SetUserScale(self.printUserScale, self.printUserScale)
		self.nextLineToPrint, self.allLinesPrinted = self.canvas.doPrinting(dc, self.nextLineToPrint, self.pixelsPerInch)
		return true



