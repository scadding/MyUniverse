#!/usr/bin/env python

import wx
import wx.stc as stc
import sys



class Log(stc.StyledTextCtrl):
    """
    Subclass the StyledTextCtrl to provide  additions
    and initializations to make it useful as a log window.

    """
    def __init__(self, parent, style=wx.SIMPLE_BORDER):
        """
        Constructor
        
        """
        stc.StyledTextCtrl.__init__(self, parent, style=style, )
        self._styles = [None]*32
        self._free = 1
        sys.stdout = self
        
    def getStyle(self, c='black'):
        """
        Returns a style for a given colour if one exists.  If no style
        exists for the colour, make a new style.
        
        If we run out of styles, (only 32 allowed here) we go to the top
        of the list and reuse previous styles.

        """
        free = self._free
        if c:
            c = c.lower()
        else:
            c = 'black'
        
        try:
            style = self._styles.index(c)
            return style
            
        except ValueError:
            style = free
            self._styles[style] = c
            self.StyleSetForeground(style, wx.Colour(c))

            free += 1
            if free >31:
                free = 0
            self._free = free
            return style

    def write(self, text, c=None):
        """
        Add the text to the end of the control using colour c which
        should be suitable for feeding directly to wx.NamedColour.
        s
        'text' should be a unicode string or contain only ascii data.
        """
        self.SetCurrentPos(self.GetTextLength())
        style = self.getStyle(c)
        lenText = len(text.encode('utf8'))
        end = self.GetLength()
        self.SetEditable(True)
        self.AddText(text)
        self.SetEditable(False)
        self.StartStyling(end)
        self.SetStyling(lenText, style)
        self.EnsureCaretVisible()
        

    __call__ = write
