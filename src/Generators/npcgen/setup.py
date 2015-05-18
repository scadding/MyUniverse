#! /usr/bin/env python

"""
 *******************************************************************************
 * 
 * $RCSfile: ClassData.py,v $
 * $Date: 2008-03-25 17:15:39 -0500 (Tue, 25 Mar 2008) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: setup script for creating windows binary, via py2exe
 *
 *******************************************************************************
"""

import sys, os, re, glob
# setup.py
from distutils.core import setup
import py2exe
   
if len(sys.argv) < 2:
	sys.argv.append('py2exe')

# FIXME: dist should really be dist/NPCGen
setup(
		# The first three parameters are not required, if at least a
		# 'version' is given, then a versioninfo resource is built from
		# them and added to the executables.
		version = "2.5.1",
		description = "py2exe script for npcgen",
		name = "py2exe npcgen",

		# targets to build
		#windows = ["cmaker-gui.py"],
		console = ['charactermaker.py'],
		windows = [
			{
				"script": "cmaker-gui.py",
				"icon_resources": [(1, "images/favicon.ico")],
			}
		],
		data_files=[ ("", glob.glob("*.html")),
					("", glob.glob("*.css")),
					("", glob.glob("*.vbs")),
					("class-data", glob.glob("class-data/*.xml")),
					("names-data", glob.glob("names-data/*.txt")),
					("cfg", glob.glob("cfg/*.xml")),
					("sav", glob.glob("sav/*.html")),
					("images/pics", glob.glob("images/pics/*.gif")),
					("images", glob.glob("images/*.*"))],

		options = {'py2exe': {'packages': 'encodings', "dist_dir": "dist", "compressed":1, "optimize": 2}},
    )

if os.path.isfile('./dist/NPCGen.exe'):
	os.remove('./dist/NPCGen.exe')
os.rename('./dist/cmaker-gui.exe', './dist/NPCGen.exe')

"""
setup(console=["myscript.py"],
      data_files=[("bitmaps",
                   ["bm/large.gif", "bm/small.gif"]),
                  ("fonts",
                   glob.glob("fonts\\*.fnt"))])
"""

