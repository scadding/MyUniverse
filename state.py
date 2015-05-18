#!/usr/bin/python
# -*- coding: utf-8 -*-

# http://zetcode.com/db/sqlitepythontutorial/

import sqlite3 as lite
import sys

con = lite.connect('test.db')
cur = con.cursor()    
cur.execute("DROP TABLE IF EXISTS Objects")
cur.execute("DROP TABLE IF EXISTS Variables")
cur.execute("DROP TABLE IF EXISTS TableVariables")
cur.execute("DROP TABLE IF EXISTS TableLines")
cur.execute("DROP TABLE IF EXISTS Tables")

cur.execute("CREATE TABLE Objects(Id INTEGER PRIMARY KEY, Name TEXT, Type TEXT)")
cur.execute("CREATE TABLE Variables(Id INTEGER PRIMARY KEY, Object TEXT, Name TEXT, Value TEXT)")
cur.execute("CREATE TABLE TableVariables(Id INTEGER PRIMARY KEY, TableName TEXT, Name TEXT, Value TEXT)")
cur.execute("CREATE TABLE TableLines(Id INTEGER PRIMARY KEY, Sub TEXT, Roll INT, Line TEXT)")
cur.execute("CREATE TABLE Tables(Id INTEGER PRIMARY KEY, TableName TEXT, SubTableName TEXT, Length INT)")
