#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from stat import *
import re
from pyparsing import *
import sys
from table import *
import collections

class node:
    matching = collections.OrderedDict()
    matching["\"", "\""] = "double quotes"
    matching["\'", "\'"] = "single quotes"
    matching['{{', '}}'] = "double bracket"
    matching["{", "}"] = "single bracket"
    matching['\[', '\]'] = "square bracket"
    matching['\(', '\)'] = "parenthesis"
    operator = dict()
    def __init__(self, exp):
        self.expression = exp
        self.children = list()
        print exp
        # do matching first
        for l, r in self.matching:
            matchDeclaration = re.compile('^(.*?)' + l + '([^' + l + '.]*?)' + r + '(.*)$')
            m = matchDeclaration.match(exp)
            if m:
                #print l, r
                self.children.append(node(m.group(1)))
                self.children.append(node(m.group(2)))
                self.children.append(node(m.group(3)))
        self.top = None
    def show(self):
        for i in self.children:
            print i.expression
    
if __name__ == "__main__":
    n = node("the (quick brown fox (\"jumped\") over the \"lazy\") dogs")
    n.show()
