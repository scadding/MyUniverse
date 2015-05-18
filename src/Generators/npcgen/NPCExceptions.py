#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: NPCExceptions.py,v $
 * $Date: 2004-11-28 12:50:39 -0600 (Sun, 28 Nov 2004) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * NPCException classes.
 *
 *******************************************************************************
"""

class NPCError(Exception):
	def __init__(self, message):
		self.message = message

	def __str__(self):
		return repr(self.message)

class InfiniteLoopError(NPCError):
	def __init__(self, message, className):
		NPCError.__init__(self, message)
		self.className = className

	def getClassName(self):
		return self.className


"""
 *******************************************************************************
 * $Log: NPCExceptions.py,v $
 * Revision 1.2  2003/07/11 20:51:28  ayinger
 * add RCS tags
 *
 *******************************************************************************
"""
