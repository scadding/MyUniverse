#!/usr/bin/env python

import wx
import sys
from src import MyFrame
import inspect

def main(argv):
    app = wx.App(False)
    frame_1 = MyFrame.MyFrame(None, -1, "")
    #app.SetTopWindow(frame_1)
    frame_1.Show()
    app.MainLoop()

