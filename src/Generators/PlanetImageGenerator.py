'''
Usage: planet [options]

options:
  -?                (or any illegal option) Output this text
  -s seed           Specifies seed as number between 0.0 and 1.0
  -w width          Specifies width in pixels, default = 800
  -h height         Specifies height in pixels, default = 600
  -m magnification  Specifies magnification, default = 1.0
  -o output_file    Specifies output file, default is standard output
  -l longitude      Specifies longitude of centre in degrees, default = 0.0
  -L latitude       Specifies latitude of centre in degrees, default = 0.0
  -g gridsize       Specifies vertical gridsize in degrees, default = 0.0 (no grid)
  -G gridsize       Specifies horisontal gridsize in degrees, default = 0.0 (no grid)
  -i init_alt       Specifies initial altitude (default = -0.02)
  -c                Colour depends on latitude (default: only altitude)
  -C file           Read colour definitions from file
  -O                Produce a black and white outline map
  -E                Trace the edges of land in black on colour map
  -B                Use ``bumpmap'' shading
  -b                Use ``bumpmap'' shading on land only
  -d                Use ``daylight'' shading
  -a angle            Angle of ``light'' in bumpmap shading
                    or longitude of sun in daylight shading
  -A latitude         Latitude of sun in daylight shading
  -P                Use PPM file format (default is BMP)
  -x                Use XPM file format (default is BMP)
  -V number         Distance contribution to variation (default = 0.03)
  -v number         Altitude contribution to variation (default = 0.4)
  -pprojection      Specifies projection: m = Mercator (default)
                                          p = Peters
                                          q = Square
                                          s = Stereographic
                                          o = Orthographic
                                          g = Gnomonic
                                          a = Area preserving azimuthal
                                          c = Conical (conformal)
                                          M = Mollweide
                                          S = Sinusoidal
                                          h = Heightfield
                                          f = Find match, see manual
'''

from subprocess import *
import base64

class PlanetImageGenerator:
    def __init__(self):
        self.parameters = dict()
        self.pList = []
        self.AddParameter('name', '')
        self.AddParameter('seed', '')
        self.AddParameter('altitude', '-0.02')
        self.AddParameter('longitude', '0.0')
        self.AddParameter('latitude', '0.0')
        self.AddParameter('height', '480')
        self.AddParameter('width', '640')
        self.AddParameter('magnification', '1.0')
        self.AddParameter('grid', '10')
        self.AddParameter('projection', {'Gnomonic': 'g', 'Mollweide': 'M', 'Square': 'q', 'Conical (conformal)': 'c', 'Area preserving azimuthal': 'a', 'Peters': 'p', 'Stereographic': 's', 'Orthographic': 'o', 'Mercator': 'm', 'Heightfield': 'h', 'Sinusoidal': 'S'})
        self.AddParameter('colormap', {'Olsson': './cfg/Olsson.col', 'wood': './cfg/wood.col', 'mars': './cfg/mars.col'})
    def AddParameter(self, parameter, value):
        self.parameters[parameter] = value
        self.pList.append(parameter)
    def Update(self, p):
        # will update the lists based on current parameters
        # this can be used to update lists based on selections
        pass
    def roll(self, p, numRolls):
        t = 'Planet Image'
        args = list()
        args.append('./bin/planet')
        if 'projection' in p:
            projection = self.parameters['projection'][p['projection']]
            args.append('-p' + projection)
        if 'colormap' in p:
            colormap = self.parameters['colormap'][p['colormap']]
            args.append('-C')
            args.append(colormap)
        else:
            args.append('-C')
            args.append('./cfg/Olsson.col')            
        if 'seed' in p:
            args.append('-s')
            args.append(p['seed'])
        if 'height' in p:
            args.append('-h')
            args.append(p['height'])
        if 'width' in p:
            args.append('-w')
            args.append(p['width'])
        if 'longitude' in p:
            args.append('-l')
            args.append(p['longitude'])
        if 'grid' in p:
            args.append('-g')
            args.append(p['grid'])
            args.append('-G')
            args.append(p['grid'])
        if 'latitude' in p:
            args.append('-L')
            args.append(p['latitude'])
        if 'magnification' in p:
            args.append('-m')
            args.append(p['magnification'])
        if 'altitude' in p:
            args.append('-i')
            args.append(p['altitude'])
        if 'name' in p:
            t = p['name']
        o = Popen(args, stdout=PIPE).stdout.read().split(b'\n')
        b = b''
        for l in o:
            b += l
        s = '<image src="data:image/bmp;base64,'
        s += str(base64.b64encode(b))[2:-1]
        s += '">'
        return t, s
        
