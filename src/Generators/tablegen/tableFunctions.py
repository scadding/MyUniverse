# -*- coding: utf-8 -*-

import inflect
from src.Generators.tablegen.dice import dice as roll
from src.Generators.tablegen.eval import evalString

def eval(l):
    #print 'eval ', l
    if len(l) == 1:
        return str(evalString(l[0]))
    else:
        print("Bad Parameter Count 'eval'")
    return ''

def ucfirst(l):
    if len(l) == 1:
        return str.capitalize(l[0])
    else:
        print("Bad Parameter Count 'ucfirst'")
    return ''

def lc(l):
    if len(l) == 1:
        return str.lower(l[0])
    else:
        print("Bad Parameter Count 'lc'")
    return ''

def plural(l):
    p = inflect.engine()
    if len(l) == 1:
        return p.plural(l[0])
    else:
        print("Bad Parameter Count 'plural'")
    return ''
    
def article(l):
    p = inflect.engine()
    if len(l) == 1:
        return p.an(l[0])
    else:
        print("Bad Parameter Count 'article'")
    return ''
    
def ia(l):
    p = inflect.engine()
    if len(l) == 1:
        return p.inflect(l[0])
    else:
        print("Bad Parameter Count 'ia'")
    return ''

def dice(l):
    s = str(roll(l[0].encode('ascii', 'ignore')))
    #print 'dice ', l[0], s
    return s

def junk(l):
    return 'junk'
