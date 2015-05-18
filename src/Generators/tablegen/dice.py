from pyparsing import *

def convertNums(s):
    try:
        return int(s[0])
    except ValueError:
        return float(s[0])

LPAR,RPAR,POINT,COMMA = map(Suppress,"().,")
fnumber = Combine(Word(nums) + Optional("." + Optional(Word(nums))))
fnumber.setParseAction( convertNums )

fn = oneOf("minRoll maxRoll takeLowest takeHighest setMinimum")
dice = Combine(Optional(fnumber,default=1) + CaselessLiteral("d") + Word(nums))
args = Group(delimitedList(fnumber))
diceExpr = Group( dice + OneOrMore( Group(POINT + fn + LPAR + args + RPAR) ) )

atom = diceExpr | dice | fnumber
signop = oneOf('+ -')
multop = oneOf('* /')
plusop = oneOf('+ -')
fnop = Group(POINT + fn + LPAR + args + RPAR) 

expr = operatorPrecedence( atom,
    [(signop, 1, opAssoc.RIGHT),
     (multop, 2, opAssoc.LEFT),
     (plusop, 2, opAssoc.LEFT),]
    )

test = '2d6*3-5.5+4d6.minRoll(2).takeHighest(3)'
#~ test = 'D5+2d6*3-5.5+4d6.takeHighest(3)'
#~ test = 'D5+2d6*3-5.5+4d6'

results = expr.parseString(test)

def dice(s):
	results =  expr.parseString(s)
	return evalExpr(results)

from random import randint
def roll(dieStr):
    count,sides = map(int,dieStr.split("d"))
    ret = [ randint(1,sides) for _ in range(count) ]
    #print "Rolling", dieStr, "->", ret  # debugging statement, comment out to disable
    return ret

def evalAtom(a):
    if isinstance(a, ParseResults):
        return evalExpr(a)
    if isinstance(a, (int,float)):
        return a
    if isinstance(a, str):
        return sum(roll(a))

binops = {
    '+' : (lambda a,b : a+b),
    '-' : (lambda a,b : a-b),
    '*' : (lambda a,b : a*b),
    }
def evalExpr(a):
    if isinstance(a,ParseResults) and \
        len(a)>1 and \
        isinstance(a[1],ParseResults):
        return evalDiceExpr(a)
        
    accum = evalAtom(a[0])
    for op,elem in zip(a[1::2],a[2::2]):
        accum = binops[op](accum,evalAtom(elem))
    return accum

sortedRoll = lambda dstr,revflag=False : sorted(roll(dstr),reverse=revflag)
takeLowest = lambda d,n : isinstance(d,str) and sortedRoll(d)[:n] or sorted(d)[:n]
takeHighest = lambda d,n : isinstance(d,str) and sortedRoll(d,revflag=True)[:n] or sorted(d,reverse=True)[:n]
minRoll = lambda d,n : sorted([ sortedRoll(d) for _ in range(n)])[0]
maxRoll = lambda d,n : sorted([ sortedRoll(d) for _ in range(n)],reverse=True)[0]
def setMinimum(d, n):
	#print d, n
	r = roll(d)
	#print r
	while r[0] < n:
		r = roll(d)
	return r
	
fns = {
    'takeLowest' : takeLowest,
    'takeHighest' : takeHighest,
    'minRoll' : minRoll,
    'maxRoll' : maxRoll,
    'setMinimum' : setMinimum,
    }
def evalDiceExpr(a):
    dice = a[0]
    for mod in a[1:]:
        dice = fns[mod[0]](dice, mod[1][0])
    return sum(dice)

#print results
#print evalExpr(results)
