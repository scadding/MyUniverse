
import os
import sys
from src.Generators.secgen import subSector
from subprocess import *
from src.Generators.PlanetImageGenerator import PlanetImageGenerator
import base64
import codecs

class Magic:
    Language = 1
    Exist = 2
    Detail = 8
    Map = 16
    def __init__(self, x, y, z, magic, offset = 0):
        self._x = (x & 0x07ff) << (32 - 11)
        self._y = (y & 0x07ff) << (32 - 11 - 11)
        self._z = z & 0x03ff
        self._seed = self._x | self._y | self._z
        print(self._seed)
        self.sRand(self._seed + magic)
        for i in range(offset):
            self.Rand()
    def sRand(self, seed):
        self._value = seed
    def D(self, n):
        retval = n
        for i in range(n):
            retval += (self.Rand() % 6)
        return retval
    def Rand(self, offset = 0):
        offset += 1
        for i in range(offset):
            l = self._value
            self._value = l * 1103515245 + 12345
        return ((self._value >> 16) & 0x7fff)

class systemHtml:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        self.system = starSystem(x, y, z)
        pass
    def head(self):
        s = '<head>'
        s += self.meta()
        s += self.style()
        s += self.link()
        s += '</head>'
        return s
    def meta(self):
        s = "<meta http-equiv=content-type content='text/html; charset=utf-8'>"
        s += "    <title>System 1582017981 - stargen 1582017981-0.934943</title>"
        s += "    <meta name='generator' content='stargen - $Revision: 1.43 $'>"
        return s
    def style(self):
        s = "<style type='text/css'>"
        s += "<!--"
        s += "table {border-color: #ffd;}"
        s += "-->"
        s += "</style>"
        return s
    def link(self):
        s = "<link rel='icon' type='image/png' href='../ref/favicon.png'>"
        return s
    def systemTable(self):
        s = "<table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='90%'>"
        # add system name
        s += "<tr><th colspan=2 bgcolor='#FFE6CC' align=center>"
        s += "<font size='+2' color='#000000'>"
        s += "<a href='"
        s += "../html/stargen-1582017981-0.934943.html"
        s += "'>"
        s += self.system.Name()
        s += "</a></font></th></tr>"
        s += "<tr bgcolor='#FFE6CC'><td colspan=2>"
        # build map
        s += '''
<table border=0 cellspacing=0 cellpadding=3 bgcolor='#000000' width='100%'>
<tr><td colspan=12 bgcolor='#CC9966' align=center>
    <font size='+1'  color='#000000'><b>'''
        s += str(len(self.system._primary._satelites))
        s += ''' Planets</b> </font>
</td></tr>
<tr valign=middle bgcolor='#000000'>
'''
        s += "<td valign='Top' halign='Left' bgcolor='#000000'><img alt='Sun' src='../html/Sun.png' width=60 height=60 border=0></td>"
        for i in self.system._primary._satelites:
            b = self.system._primary._satelites[i]
            s += "<td valign='Top' halign='Left' bgcolor='#000000' align=center><a href="
            s += "'genr://Image" + ' seed ' + str(b.GetSeed()) + " name " + b.Name() + "'"
            s += " title="
            s += "'#" + str(i) + " - "
            s += b.Name()
            s += ": "
            s += b.Type()
            s += "'>"
            s += b.GetImage()
            s += "</a>"
            for j in b._satelites:
                s += "<br><a href="
                s += "'genr://Image" + ' seed ' + str(b._satelites[j].GetSeed()) + " name " + b._satelites[j].Name() + "'"
                s += " title="
                s += "'#" + str(i) + "." + str(j) + " - "
                s += b._satelites[j].Name()
                s += ": "
                s += b._satelites[j].Type()
                s += "'>"
                s += b._satelites[j].GetImage()
                s += "</a>"
                for k in b._satelites[j]._satelites:
                    s += "<a href="
                    s += "'genr://Image" + ' seed ' + str(b._satelites[j]._satelites[k].GetSeed()) + " name " + b._satelites[j]._satelites[k].Name() + "'"
                    s += " title="
                    s += "'#" + str(i) + "." + str(j) + "." + str(k) + " - "
                    s += b._satelites[j]._satelites[k].Name()
                    s += ": "
                    s += b._satelites[j]._satelites[k].Type()
                    s += "'>"
                    s += b._satelites[j]._satelites[k].GetImage()
                    s += "</a>"
                s += "</a>"
            s += "</td>"
        s += "</tr></table>"
        s += '''
</td></tr>
<tr><td colspan=2 bgcolor='#CC9966' align=center><font size='+1' color='#000000'><b>Stellar characteristics</b></font></td></tr>
<tr><td>Stellar mass</td>
    <td>0.93 solar masses</td></tr>
<tr><td>Stellar luminosity</td>
    <td>0.72</td></tr>
<tr><td>Age</td>
    <td>1.691 billion years<br>(11.209 billion left on main sequence)<br><tr><td>Habitable ecosphere radius</td>
    <td>0.851 AU (0.693 - 1.229 AU)</td></tr>
'''
        s += "</table>"
        return s
    def systemDescriptionTable(self):
        pass
    def systemBodyTable(self):
        pass
        
    def body(self):
        s = "<body bgcolor='#FFCC99' text='#330000' link='#990000' vlink='#330000' alink='#FF0000'>"
        s += self.systemTable()
        #s += content
        s += "</body>"
        return s

    def __repr__(self):
        s = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"'
        s += '"http://www.w3.org/TR/html4/loose.dtd">'
        s += '<html>'
        s += self.head()
        s += self.body()
        s += '</html>'
        return s

class starSystem:
    def __init__(self, x, y, z):
        args = list()
        self._bodies = dict()
        self._x = int(x)
        self._y = int(y)
        self._z = int(z)
        m = Magic(x, y, z, Magic.Map)
        args.append('./bin/sysgen2')
        args.append(str(x))
        args.append(str(y))
        args.append(str(z))
        o = Popen(args, stdout=PIPE).stdout.read().split(b'\n')
        systemName = str(o[0])[2:-1]
        location = str(o[1])[2:-1]
        primary = str(o[2])[2:-1]
        self._primary = body(primary, x, y, z)
        self._bodies[self._primary.Name()] = self._primary
        self._bodies[self._primary.Name()].Seed(m.Rand())
        previousLevel = 0
        current = dict()
        current[0] = self._primary
        for l in o[3:]:
            if len(l) == 0:
                break
            level = 0
            for c in l:
                if c == '\t':
                    level += 1
                else:
                    break
            l = l.strip()
            orbit = int(l[:l.find(b'\t') + 1])
            l = l[l.find(b'\t') + 1:]
            b = body(l, x, y, z)
            b.Seed(m.Rand())
            self._bodies[b.Name()] = b
            if level + 1 != previousLevel:
                previousLevel = level + 1
            current[previousLevel] = b
            current[previousLevel - 1].addOrbit(orbit, b)
            #print 'orbit = ' + str(orbit)
            #print 'level = ' + str(level)
            #print 'name = ' + b.Name()
            #print 'type = ' + b.Type()
            #print
        #print self._bodies
        #self._primary.Print()
    def Name(self):
        name = ''
        for n in self._bodies:
            if self._bodies[n].MainWorld():
                name += self._bodies[n].Name()
        return name

'''
        printf("Star %c%d %d       %s\n", s->upp.s.classification,
               s->upp.s.decimal, s->upp.s.size, s->name);
            printf("Large Gas Giant %s\n", s->name);
            printf("Small Gas Giant %s\n", s->name);
            printf("Ring            %s\n", s->name);
            printf("Asteroids       %s\n", s->name);
        printf("%c%1x%1x%1x%1x%1x%1x-%x %c%c%c  %c%s\n",
               s->upp.w.starport, s->upp.w.size,
               s->upp.w.atmos, s->upp.w.hydro, s->upp.w.pop, s->upp.w.gov, s->upp.w.law,
               s->upp.w.tech, (s->upp.w.bases&NAVAL_BASE)? 'N' : ' ',
               (s->upp.w.bases&SCOUT_BASE)? 'S' : ' ',
               (s->upp.w.bases&LOCAL_BASE)? 'M' : ' ',
               (s->type == T_MAIN) ? '*' : ' ',
               s->name);
'''

class body:
    def __init__(self, l, x, y, z):
        self._x = x
        self._y = y
        self._z = z
        self._line = str(l)[2:-1]
        self._orbit = 0
        self._satelites = dict()
        self._type = self._line[:15].strip()
        m = self._line[15]
        if m == '*':
            self._mainWorld = True
        else:
            self._mainWorld = False
        self._name = self._line[16:].strip()
        self._seed = 0
    def Name(self):
        return self._name
    def GetSeed(self):
        return self._seed
    def Seed(self, s):
        self._seed = s
    def Type(self):
        return self._type
    def MainWorld(self):
        return self._mainWorld
    def addOrbit(self, o, b):
        self._orbit = o
        self._satelites[len(self._satelites)] = b
    def Print(self):
        #print(self._orbit, self._name)
        for o in self._satelites:
            #print('\t', o)
            self._satelites[o].Print()
    def GetImage(self):
        size = 32
        magnification = '1.0'
        if self._type == 'Asteroids':
            s = "<img alt='Asteroids' src='../html/Asteroids.png' width=20 height=20 border=0>"
        elif self._type == 'Large Gas Giant':
            s = "<img alt='Jupiter' src='../html/Jupiter.png' width=32 height=32 border=0>"
        elif self._type == 'Small Gas Giant':
            s = "<img alt='Neptune' src='../html/Neptune.png' width=32 height=32 border=0>"
        elif self._type == 'Ring':
            s = "<img alt='Ring' src='../html/Ring.png' width=20 height=20 border=0>"
        elif self._type[:4] == 'Star':
            s = "<img alt='Sun' src='../html/Sun.png' width=40 height=40 border=0>"
        else:
            magnification = str(int(self._type[1], 16) / 10.0 + .1)
            if int(self._type[1], 16) > 10:
                magnification = '1.0'
                size += int(self._type[1], 16) - 10
            args = list()
            args.append('./bin/planet')
            args.append('-C')
            args.append('./cfg/Olsson.col')
            args.append('-po')
            args.append('-m')
            args.append(magnification)
            args.append('-h')
            args.append(str(size))
            args.append('-w')
            args.append(str(size))
            args.append('-s')
            args.append(str(self._seed))
            args.append('-i')
            args.append('-0.02')
            o = Popen(args, stdout=PIPE).stdout.read().split(b'\n')
            b = b''
            for l in o:
                b += l
            s = '<image src="data:image/bmp;base64,'
            s += str(base64.b64encode(b))[2:-1]
            s += '">'
        return s

class SystemGenerator:
    def __init__(self):
        self.parameters = dict()
        self.parameters['x'] = '4'
        self.parameters['y'] = '7'
        self.parameters['z'] = '9'
        # order of the parameters to be displayed
        self.pList = ['x', 'y', 'z']
    def Update(self, p):
        pass
    def roll(self, p, numRolls):
        systemName, page = self.generate(p)
        filename = "tmp/" + str(systemName) + ".html"
        f = codecs.open(filename, "w", "utf-8")
        f.write(str(page))
        f.close()
        print(systemName, filename)
        return systemName, filename
    def generate(self, p):
        s = ''
        x = p['x']
        y = p['y']
        z = p['z']
        args = list()
        args.append('./bin/sysgen2')
        args.append(x)
        args.append(y)
        args.append(z)
        o = Popen(args, stdout=PIPE).stdout.read().split(b'\n')
        s += '<pre>\n'
        systemName = o[0]
        location = o[1]
        primary = o[2]
        page = systemHtml(int(x), int(y), int(z))
        return systemName, page

        
def main(argv):
    if len(argv) < 4:
        print('x y z')
        return
    sys = starSystem(argv[1], argv[2], argv[3])
    
if __name__ == "__main__":
    main(sys.argv)


content = '''
<p>

<br clear=all>

<table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='90%'>
<tr><th colspan=7 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planetary Overview</font></th></tr>

<tr align=center>
    <th>#</th><th colspan=3>Type</th><th>Dist.</th><th>Mass</th><th>Radius</th>
</tr>
<tr align=right>
    <td><a href='#1'>1</a></td>
    <td align=center><img alt='Rock' src='../ref/Rock.gif'></td>
    <td colspan=2>Rock</td>
    <td>  0.391  AU</td>
    <td>   0.283 EM</td>
    <td>   0.661 ER</td></tr>
<tr align=right>
    <td><a href='#2'>2</a></td>
    <td align=center><img alt='Rock' src='../ref/Rock.gif'></td>
    <td colspan=2>Rock</td>
    <td>  0.523  AU</td>
    <td>   0.113 EM</td>
    <td>   0.488 ER</td></tr>
<tr align=right>
    <td><a href='#3'>3</a></td>
    <td align=center><img alt='Rock' src='../ref/Rock.gif'></td>
    <td colspan=2>Rock</td>
    <td>  0.593  AU</td>
    <td>   0.083 EM</td>
    <td>   0.441 ER</td></tr>
<tr align=right>
    <td><a href='#4'>4</a></td>
    <td align=center><img alt='Terrestrial' src='../ref/Terrestrial.gif'></td>
    <td colspan=2>Terrestrial</td>
    <td>  0.973  AU</td>
    <td>   1.258 EM</td>
    <td>   1.077 ER</td></tr>
<tr align=right>
    <td></td>
    <td align=center><a href='#4.1'>4.1</a></td>
    <td align=center><img alt='Terrestrial' src='../ref/Terrestrial.gif'></td>
    <td>Terrestrial</td>
    <td>  0.973  AU</td>
    <td>   1.194 EM</td>
    <td>   1.059 ER</td></tr>
<tr align=right>
    <td><a href='#5'>5</a></td>
    <td align=center><img alt='Jovian' src='../ref/Jovian.gif'></td>
    <td colspan=2>Jovian</td>
    <td>  2.796  AU</td>
    <td> 163.812 EM</td>
    <td>   8.115 ER</td></tr>
<tr align=right>
    <td></td>
    <td align=center><a href='#5.1'>5.1</a></td>
    <td align=center><img alt='Ice' src='../ref/Ice.gif'></td>
    <td>Ice</td>
    <td>  2.796  AU</td>
    <td>   0.758 EM</td>
    <td>   0.914 ER</td></tr>
<tr align=right>
    <td><a href='#6'>6</a></td>
    <td align=center><img alt='Jovian' src='../ref/Jovian.gif'></td>
    <td colspan=2>Jovian</td>
    <td>  7.328  AU</td>
    <td> 165.720 EM</td>
    <td>   8.823 ER</td></tr>
<tr align=right>
    <td><a href='#7'>7</a></td>
    <td align=center><img alt='Martian' src='../ref/Martian.gif'></td>
    <td colspan=2>Martian</td>
    <td> 11.688  AU</td>
    <td>   0.574 EM</td>
    <td>   1.112 ER</td></tr>
<tr align=right>
    <td><a href='#8'>8</a></td>
    <td align=center><img alt='Jovian' src='../ref/Jovian.gif'></td>
    <td colspan=2>Jovian</td>
    <td> 16.236  AU</td>
    <td>  85.440 EM</td>
    <td>   7.771 ER</td></tr>
<tr align=right>
    <td><a href='#9'>9</a></td>
    <td align=center><img alt='Sub-Jovian' src='../ref/Sub-Jovian.gif'></td>
    <td colspan=2>Sub-Jovian</td>
    <td> 34.166  AU</td>
    <td>   8.643 EM</td>
    <td>   4.238 ER</td></tr>
<tr align=right>
    <td></td>
    <td align=center><a href='#9.1'>9.1</a></td>
    <td align=center><img alt='Rock' src='../ref/Rock.gif'></td>
    <td>Rock</td>
    <td> 34.166  AU</td>
    <td>   0.010 EM</td>
    <td>   0.295 ER</td></tr>
<tr align=right>
    <td><a href='#10'>10</a></td>
    <td align=center><img alt='Rock' src='../ref/Rock.gif'></td>
    <td colspan=2>Rock</td>
    <td> 48.224  AU</td>
    <td>   0.046 EM</td>
    <td>   0.484 ER</td></tr>
<tr align=right>
    <td></td>
    <td align=center><a href='#10.1'>10.1</a></td>
    <td align=center><img alt='Rock' src='../ref/Rock.gif'></td>
    <td>Rock</td>
    <td> 48.224  AU</td>
    <td>   0.011 EM</td>
    <td>   0.305 ER</td></tr>
</table>
<br clear=all>
<p>
<a name='1'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #1 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Rock' src='../ref/Rock.gif' align=middle width=20 height=20>Rock<br>Resonant Spin Locked
<br>Low-G, Hot, Airless, 1-Face</td></tr>
<tr><th>Distance from primary star</th><td>5.9E+07 KM</td><td>0.391 AU</td></tr>
<tr><th>Mass</th><td>1.7E+24Kg</td><td>0.283 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>635.0 cm/sec<sup>2</sup></td><td>0.65 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>    0 millibars</td><td>0.000 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>122.8&deg; Celcius<br>253.1&deg; Fahrenheit</td><td rowspan=2 valign=top>+108.8&deg; C Earth temperature<br>+195.9&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>-201.8&deg; C<br>-331.3&deg; F</td><td> - </td><td>309.9&deg; C<br>589.8&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-212.3&deg; C<br>-350.1&deg; F</td><td> - </td><td>313.6&deg; C<br>596.5&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>4215.4 Km</td><td>0.66 Earth radii</td></tr>
<tr><th>Density</th><td>5.39 grams/cc</td><td>0.98 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.127</td><td></td></tr>
<tr><th>Escape Velocity</th><td> 7.3 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td>68.0 and above</td><td>SO<sub><small>3</small></sub>, Kr, Xe</td></tr>
<tr><th>Axial tilt</th><td>24&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.07</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>6030.82&deg; K</td><td>+4757.82&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>92.40 Earth days</td><td>1.29 Local days</td></tr>
<tr><th>Length of day</th><td>1718.29 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>-273.1&deg; Celcius<br>-459.7&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 0.0</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='2'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #2 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Rock' src='../ref/Rock.gif' align=middle width=20 height=20>Rock<br>Tidally Locked 1 Face
<br>Low-G, Hot, Airless, 1-Face</td></tr>
<tr><th>Distance from primary star</th><td>7.8E+07 KM</td><td>0.523 AU</td></tr>
<tr><th>Mass</th><td>6.8E+23Kg</td><td>0.113 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>466.1 cm/sec<sup>2</sup></td><td>0.48 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>    0 millibars</td><td>0.000 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td> 69.4&deg; Celcius<br>156.9&deg; Fahrenheit</td><td rowspan=2 valign=top>+55.4&deg; C Earth temperature<br>+99.7&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-227.8&deg; C<br>-378.1&deg; F</td><td> - </td><td>247.0&deg; C<br>476.7&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>3115.6 Km</td><td>0.49 Earth radii</td></tr>
<tr><th>Density</th><td>5.35 grams/cc</td><td>0.97 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.024</td><td></td></tr>
<tr><th>Escape Velocity</th><td> 5.4 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td>70.1 and above</td><td>SO<sub><small>3</small></sub>, Kr, Xe</td></tr>
<tr><th>Axial tilt</th><td>16&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.07</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>3375.91&deg; K</td><td>+2102.91&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>142.78 Earth days</td><td>1.00 Local days</td></tr>
<tr><th>Length of day</th><td>3426.68 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>-273.1&deg; Celcius<br>-459.7&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 0.0</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='3'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #3 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Rock' src='../ref/Rock.gif' align=middle width=20 height=20>Rock<br>Tidally Locked 1 Face
<br>Low-G, Hot, Airless, 1-Face</td></tr>
<tr><th>Distance from primary star</th><td>8.9E+07 KM</td><td>0.593 AU</td></tr>
<tr><th>Mass</th><td>5E+23Kg</td><td>0.083 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>420.0 cm/sec<sup>2</sup></td><td>0.43 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>    0 millibars</td><td>0.000 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td> 48.5&deg; Celcius<br>119.4&deg; Fahrenheit</td><td rowspan=2 valign=top>+34.5&deg; C Earth temperature<br>+62.2&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-230.7&deg; C<br>-383.3&deg; F</td><td> - </td><td>220.8&deg; C<br>429.4&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>2811.7 Km</td><td>0.44 Earth radii</td></tr>
<tr><th>Density</th><td>5.34 grams/cc</td><td>0.97 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.062</td><td></td></tr>
<tr><th>Escape Velocity</th><td> 4.9 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td>67.1 and above</td><td>SO<sub><small>3</small></sub>, Kr, Xe</td></tr>
<tr><th>Axial tilt</th><td>17&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.07</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>2627.04&deg; K</td><td>+1354.04&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>172.33 Earth days</td><td>1.00 Local days</td></tr>
<tr><th>Length of day</th><td>4135.87 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>-273.1&deg; Celcius<br>-459.7&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 0.0</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='4'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #4 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Terrestrial' src='../ref/Terrestrial.gif' align=middle width=20 height=20>Terrestrial<br>Cold, Wet</td></tr>
<tr><th>Distance from primary star</th><td>1.5E+08 KM</td><td>0.973 AU</td></tr>
<tr><th>Mass</th><td>7.5E+24Kg</td><td>1.258 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>1062.1 cm/sec<sup>2</sup></td><td>1.08 Earth gees</td></tr>
<tr><th>Surface pressure</th><td> 1714 millibars</td><td>1.692 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>  9.0&deg; Celcius<br> 48.2&deg; Fahrenheit</td><td rowspan=2 valign=top>-5.0&deg; C Earth temperature<br>-9.0&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>  5.6&deg; C<br> 42.0&deg; F</td><td> - </td><td> 12.2&deg; C<br> 53.9&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-11.9&deg; C<br> 10.5&deg; F</td><td> - </td><td> 28.7&deg; C<br> 83.7&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>6872.1 Km</td><td>1.1 Earth radii</td></tr>
<tr><th>Density</th><td>5.53 grams/cc</td><td>1 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.007</td><td></td></tr>
<tr><th>Escape Velocity</th><td>12.1 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 4.0 and above</td><td>N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O, Ne, N<sub><small>2</small></sub>, CO...</td></tr>
<tr><th>Axial tilt</th><td>29&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.24</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>974.88&deg; K</td><td>-298.12&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>362.44 Earth days</td><td>581.82 Local days</td></tr>
<tr><th>Length of day</th><td>14.95 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>115.3&deg; Celcius<br>239.6&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td>82.3</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td>40.3</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 4.0</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='4.1'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='90%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Moon #4.1 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Terrestrial' src='../ref/Terrestrial.gif' align=middle width=20 height=20>Terrestrial<br>Cold, Wet, Few clouds</td></tr>
<tr><th>Distance from primary star</th><td>1.5E+08 KM</td><td>0.973 AU</td></tr>
<tr><th>Mass</th><td>7.1E+24Kg</td><td>1.194 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>1042.9 cm/sec<sup>2</sup></td><td>1.06 Earth gees</td></tr>
<tr><th>Surface pressure</th><td> 1545 millibars</td><td>1.525 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>  8.3&deg; Celcius<br> 46.9&deg; Fahrenheit</td><td rowspan=2 valign=top>-5.7&deg; C Earth temperature<br>-10.3&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>  4.7&deg; C<br> 40.4&deg; F</td><td> - </td><td> 11.7&deg; C<br> 53.0&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-14.1&deg; C<br>  6.6&deg; F</td><td> - </td><td> 29.4&deg; C<br> 85.0&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>6757.3 Km</td><td>1.1 Earth radii</td></tr>
<tr><th>Density</th><td>5.52 grams/cc</td><td>1 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.007</td><td></td></tr>
<tr><th>Escape Velocity</th><td>11.9 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 4.2 and above</td><td>N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O, Ne, N<sub><small>2</small></sub>, CO...</td></tr>
<tr><th>Axial tilt</th><td>19&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.23</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>974.88&deg; K</td><td>-298.12&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>362.44 Earth days</td><td>576.12 Local days</td></tr>
<tr><th>Length of day</th><td>15.10 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>112.2&deg; Celcius<br>234.0&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td>80.8</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td>37.6</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 4.4</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='5'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #5 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Jovian' src='../ref/Jovian.gif' align=middle width=20 height=20>Jovian</td></tr>
<tr><th>Distance from primary star</th><td>4.2E+08 KM</td><td>2.796 AU</td></tr>
<tr><th>Mass</th><td>9.8E+26Kg</td><td>163.812 Earth masses<br>22.207 Earth masses dust<br>141.606 Earth masses gas<br></td></tr>
<tr><th>Equatorial radius</th><td>51755.5 Km</td><td>8.1 Earth radii</td></tr>
<tr><th>Density</th><td>1.69 grams/cc</td><td>0.31 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.112</td><td></td></tr>
<tr><th>Escape Velocity</th><td>50.2 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.0 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>29&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.48</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>118.03&deg; K</td><td>-1154.97&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>1765.40 Earth days</td><td>5119.43 Local days<br>4.83 Earth years</td></tr>
<tr><th>Length of day</th><td>8.28 hours</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='5.1'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='90%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Moon #5.1 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Ice' src='../ref/Ice.gif' align=middle width=20 height=20>Ice<br>Cold, Icy, Arid, Cloudless, Normal atmosphere</td></tr>
<tr><th>Distance from primary star</th><td>4.2E+08 KM</td><td>2.796 AU</td></tr>
<tr><th>Mass</th><td>4.5E+24Kg</td><td>0.758 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>890.3 cm/sec<sup>2</sup></td><td>0.91 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>  623 millibars</td><td>0.615 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>-148.0&deg; Celcius<br>-234.4&deg; Fahrenheit</td><td rowspan=2 valign=top>-162.0&deg; C Earth temperature<br>-291.6&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>-150.6&deg; C<br>-239.1&deg; F</td><td> - </td><td>-145.5&deg; C<br>-229.9&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-169.5&deg; C<br>-273.1&deg; F</td><td> - </td><td>-128.3&deg; C<br>-199.0&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>5828.0 Km</td><td>0.91 Earth radii</td></tr>
<tr><th>Density</th><td>5.47 grams/cc</td><td>0.99 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.112</td><td></td></tr>
<tr><th>Escape Velocity</th><td>10.2 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.7 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>37&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.70</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>118.03&deg; K</td><td>-1154.97&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>1765.86 Earth days</td><td>2638.50 Local days<br>4.83 Earth years</td></tr>
<tr><th>Length of day</th><td>16.06 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>87.3&deg; Celcius<br>189.1&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td>100.0</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='6'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #6 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Jovian' src='../ref/Jovian.gif' align=middle width=20 height=20>Jovian</td></tr>
<tr><th>Distance from primary star</th><td>1.1E+09 KM</td><td>7.328 AU</td></tr>
<tr><th>Mass</th><td>9.9E+26Kg</td><td>165.720 Earth masses<br>16.360 Earth masses dust<br>149.361 Earth masses gas<br></td></tr>
<tr><th>Equatorial radius</th><td>56272.5 Km</td><td>8.8 Earth radii</td></tr>
<tr><th>Density</th><td>1.33 grams/cc</td><td>0.24 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.071</td><td></td></tr>
<tr><th>Escape Velocity</th><td>48.5 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.0 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>23&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.50</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td> 17.18&deg; K</td><td>-1255.82&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>7491.23 Earth days</td><td>20095.82 Local days<br>20.51 Earth years</td></tr>
<tr><th>Length of day</th><td>8.95 hours</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='7'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #7 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Martian' src='../ref/Martian.gif' align=middle width=20 height=20>Martian<br>Low-G, Cold, Icy, Arid, Cloudless, Thin atmosphere</td></tr>
<tr><th>Distance from primary star</th><td>1.7E+09 KM</td><td>11.688 AU</td></tr>
<tr><th>Mass</th><td>3.4E+24Kg</td><td>0.574 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>454.7 cm/sec<sup>2</sup></td><td>0.46 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>  191 millibars</td><td>0.188 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>-200.7&deg; Celcius<br>-329.3&deg; Fahrenheit</td><td rowspan=2 valign=top>-214.7&deg; C Earth temperature<br>-386.5&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>-204.1&deg; C<br>-335.5&deg; F</td><td> - </td><td>-197.5&deg; C<br>-323.5&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-222.1&deg; C<br>-367.8&deg; F</td><td> - </td><td>-181.3&deg; C<br>-294.3&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>7093.4 Km</td><td>1.1 Earth radii</td></tr>
<tr><th>Density</th><td>2.29 grams/cc</td><td>0.42 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.031</td><td></td></tr>
<tr><th>Escape Velocity</th><td> 8.0 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.1 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>32&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.31</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>  6.75&deg; K</td><td>-1266.25&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>15094.48 Earth days</td><td>16117.56 Local days<br>41.33 Earth years</td></tr>
<tr><th>Length of day</th><td>22.48 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>59.2&deg; Celcius<br>138.6&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td>28.3</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='8'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #8 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Jovian' src='../ref/Jovian.gif' align=middle width=20 height=20>Jovian</td></tr>
<tr><th>Distance from primary star</th><td>2.4E+09 KM</td><td>16.236 AU</td></tr>
<tr><th>Mass</th><td>5.1E+26Kg</td><td>85.440 Earth masses<br>8.818 Earth masses dust<br>76.622 Earth masses gas<br></td></tr>
<tr><th>Equatorial radius</th><td>49564.6 Km</td><td>7.8 Earth radii</td></tr>
<tr><th>Density</th><td>1.00 grams/cc</td><td>0.18 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.125</td><td></td></tr>
<tr><th>Escape Velocity</th><td>37.1 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.0 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>44&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.47</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>  3.50&deg; K</td><td>-1269.50&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>24709.24 Earth days</td><td>54035.62 Local days<br>67.65 Earth years</td></tr>
<tr><th>Length of day</th><td>10.97 hours</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='9'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #9 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Sub-Jovian' src='../ref/Sub-Jovian.gif' align=middle width=20 height=20>Sub-Jovian</td></tr>
<tr><th>Distance from primary star</th><td>5.1E+09 KM</td><td>34.166 AU</td></tr>
<tr><th>Mass</th><td>5.2E+25Kg</td><td>8.643 Earth masses<br>1.918 Earth masses dust<br>6.725 Earth masses gas<br></td></tr>
<tr><th>Equatorial radius</th><td>27032.5 Km</td><td>4.2 Earth radii</td></tr>
<tr><th>Density</th><td>0.62 grams/cc</td><td>0.11 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.171</td><td></td></tr>
<tr><th>Escape Velocity</th><td>16.0 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.0 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>63&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.47</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>  0.79&deg; K</td><td>-1272.21&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>75436.84 Earth days</td><td>96202.00 Local days<br>206.53 Earth years</td></tr>
<tr><th>Length of day</th><td>18.82 hours</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='9.1'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='90%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Moon #9.1 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Rock' src='../ref/Rock.gif' align=middle width=20 height=20>Rock<br>Low-G, Cold, Airless</td></tr>
<tr><th>Distance from primary star</th><td>5.1E+09 KM</td><td>34.166 AU</td></tr>
<tr><th>Mass</th><td>6.2E+22Kg</td><td>0.010 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>116.4 cm/sec<sup>2</sup></td><td>0.12 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>    0 millibars</td><td>0.000 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>-231.7&deg; Celcius<br>-385.1&deg; Fahrenheit</td><td rowspan=2 valign=top>-245.7&deg; C Earth temperature<br>-442.3&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>-239.1&deg; C<br>-398.3&deg; F</td><td> - </td><td>-224.2&deg; C<br>-371.5&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-251.8&deg; C<br>-421.3&deg; F</td><td> - </td><td>-211.2&deg; C<br>-348.1&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>1878.7 Km</td><td>0.29 Earth radii</td></tr>
<tr><th>Density</th><td>2.22 grams/cc</td><td>0.4 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.171</td><td></td></tr>
<tr><th>Escape Velocity</th><td> 2.1 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.2 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>62&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.15</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>  0.79&deg; K</td><td>-1272.21&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>75437.88 Earth days</td><td>40763.89 Local days<br>206.53 Earth years</td></tr>
<tr><th>Length of day</th><td>44.41 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>-98.6&deg; Celcius<br>-145.5&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 0.0</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='10'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='95%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Planet #10 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Rock' src='../ref/Rock.gif' align=middle width=20 height=20>Rock<br>Low-G, Cold, Airless</td></tr>
<tr><th>Distance from primary star</th><td>7.2E+09 KM</td><td>48.224 AU</td></tr>
<tr><th>Mass</th><td>2.7E+23Kg</td><td>0.046 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>192.0 cm/sec<sup>2</sup></td><td>0.20 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>    0 millibars</td><td>0.000 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>-238.2&deg; Celcius<br>-396.8&deg; Fahrenheit</td><td rowspan=2 valign=top>-252.2&deg; C Earth temperature<br>-454.0&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>-243.2&deg; C<br>-405.8&deg; F</td><td> - </td><td>-233.1&deg; C<br>-387.6&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-254.2&deg; C<br>-425.5&deg; F</td><td> - </td><td>-222.1&deg; C<br>-367.8&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>3084.2 Km</td><td>0.48 Earth radii</td></tr>
<tr><th>Density</th><td>2.23 grams/cc</td><td>0.4 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.037</td><td></td></tr>
<tr><th>Escape Velocity</th><td> 3.4 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.0 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>62&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.15</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>  0.40&deg; K</td><td>-1272.60&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>126500.77 Earth days</td><td>87770.03 Local days<br>346.33 Earth years</td></tr>
<tr><th>Length of day</th><td>34.59 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>-78.6&deg; Celcius<br>-109.4&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 0.0</td><td></td></tr>
</table>

<p>
<br>

<p>
<a name='10.1'></a><table border=3 cellspacing=2 cellpadding=2 align=center bgcolor='#FFE6CC' width='90%'>
<colgroup span=1 align=left valign=middle><colgroup span=2 align=left valign=middle><tr><th colspan=3 bgcolor='#CC9966' align=center>
<font size='+2' color='#000000'>Moon #10.1 Statistics</font></th></tr>
<tr><th>Planet type</th><td colspan=2><img alt='Rock' src='../ref/Rock.gif' align=middle width=20 height=20>Rock<br>Low-G, Cold, Airless</td></tr>
<tr><th>Distance from primary star</th><td>7.2E+09 KM</td><td>48.224 AU</td></tr>
<tr><th>Mass</th><td>6.8E+22Kg</td><td>0.011 Earth masses<br></td></tr>
<tr><th>Surface gravity</th><td>120.5 cm/sec<sup>2</sup></td><td>0.12 Earth gees</td></tr>
<tr><th>Surface pressure</th><td>    0 millibars</td><td>0.000 Earth atmospheres</td></tr>
<tr><th>Surface temperature</th><td>-238.3&deg; Celcius<br>-396.9&deg; Fahrenheit</td><td rowspan=2 valign=top>-252.3&deg; C Earth temperature<br>-454.1&deg; F Earth temperature</td></tr>
<tr><th>Normal temperature range</th><td><center><table>
    <tr><th>Night</th><th></th><th>Day</th></tr>
    <tr><td>-244.3&deg; C<br>-407.8&deg; F</td><td> - </td><td>-232.0&deg; C<br>-385.6&deg; F</td></tr>
    <tr><th>Min</th><th></th><th>Max</th></tr>
    <tr><td>-258.0&deg; C<br>-432.5&deg; F</td><td> - </td><td>-217.1&deg; C<br>-358.7&deg; F</td></tr>
</table></center></td></tr>
<tr><th>Equatorial radius</th><td>1943.4 Km</td><td>0.3 Earth radii</td></tr>
<tr><th>Density</th><td>2.22 grams/cc</td><td>0.4 Earth densities</td></tr>
<tr><th>Eccentricity of orbit</th><td>0.037</td><td></td></tr>
<tr><th>Escape Velocity</th><td> 2.2 Km/sec</td><td></td></tr>
<tr><th>Molecular weight retained</th><td> 0.1 and above</td><td>H, H<sub><small>2</small></sub>, He, N, O, CH<sub><small>4</small></sub>, NH<sub><small>3</small></sub>, H<sub><small>2</small></sub>O...</td></tr>
<tr><th>Axial tilt</th><td>36&deg;</td><td></td></tr>
<tr><th>Planetary albedo</th><td> 0.15</td><td></td></tr>
<tr><th>Exospheric Temperature</th><td>  0.40&deg; K</td><td>-1272.60&deg; C Earth temperature</td></tr>
<tr><th>Length of year</th><td>126500.78 Earth days</td><td>69529.96 Local days<br>346.33 Earth years</td></tr>
<tr><th>Length of day</th><td>43.66 hours</td><td></td></tr>
<tr><th>Boiling point of water</th><td>-97.4&deg; Celcius<br>-143.3&deg; Fahrenheit</td><td></td></tr>
<tr><th>Hydrosphere percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Cloud cover percentage</th><td> 0.0</td><td></td></tr>
<tr><th>Ice cover percentage</th><td> 0.0</td><td></td></tr>
</table>

<p>
<br>

<p>

<center>
This page was created by <a href='http://www.eldacur.com/~brons/NerdCorner/StarGen/StarGen.html'>StarGen</a>.
</center>
<p>
'''
