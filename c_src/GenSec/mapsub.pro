%!PS-Adobe-1.0
% Traveller subsector map and mapping functions

%-----Constants-----
/cm { 72 mul 2.54 div } def     % cm >> points
/unit { 1 cm mul } def


/topOfPageX 4.5 cm def
/topOfPageY 25 cm def
/nameFontSize 9 def
/nameWidthLimit 1.9 unit def

/nameFont /Times-Roman findfont nameFontSize scalefont def
/nameCapitolFont /Times-Italic findfont nameFontSize scalefont def
/portFont /Helvetica findfont 10 scalefont def
/numberFont /Helvetica findfont 8 scalefont def

%-----Basic Functions-----
/hex {                             % - >> -
  gsave
  -1 unit 0 rmoveto
  currentpoint translate
  60 rotate
  5 {
    1 unit 0 lineto
    currentpoint translate
    -60 rotate
  } repeat
  closepath
  stroke
  grestore
} def

/rowOfHex {                       % x y >> -
  moveto
  10 {
    hex
    0 0 3 sqrt unit sub rmoveto
  } repeat
} def

/hexMap {                         % - >> -
  .3 setlinewidth
  /curX topOfPageX def
  /curY topOfPageY def
  4 {
    curX curY rowOfHex
    /curX curX 1.5 unit add def
    /curY curY 3 sqrt 2 div unit sub def
    curX curY rowOfHex
    /curX curX 1.5 unit add def
    /curY curY 3 sqrt 2 div unit add def
  } repeat
} def

/mapCoords {                   % HexX HexY >> -
  /actY exch 1 sub 10 mod 3 sqrt unit mul topOfPageY exch sub def
  dup 2 mod 0 eq {
    /actY actY 3 sqrt 2 div unit sub def    % displace column if HexX is even
  } if
  /actX exch 1 sub 8 mod 1.5 unit mul topOfPageX add def
} def

/getCoords {                   % - >> x y
  actX actY
} def

/liqPlnt {
  newpath
  getCoords .1 unit 0 360 arc fill
} def

/desPlnt {
  gsave
  newpath
  getCoords .1 unit 0 360 arc
  gsave
  1 setgray fill
  grestore
  .5 setlinewidth stroke
  grestore
} def

/asteroids {
  newpath
  getCoords .1 unit add .04 unit 0 360 arc fill
  getCoords exch .15 unit add exch .02 unit 0 360 arc fill
  getCoords .15 unit sub exch .1 unit add exch .03 unit 0 360 arc fill
  getCoords .04 unit sub exch .15 unit sub exch .05 unit 0 360 arc fill
  getCoords .05 unit sub .02 unit 0 360 arc fill
} def

/name {                   %  name >> -
  getCoords moveto
  nameFont setfont
  gsave
  0 -.55 unit rmoveto
  dup stringwidth pop
  dup dup nameWidthLimit gt
    {nameWidthLimit exch div 1 scale}
    {pop}
  ifelse
  neg 2 div 0 rmoveto
  gsave
  0 nameFontSize -6 div rmoveto
  dup stringwidth pop 0 rlineto
  0 nameFontSize rlineto
  dup stringwidth pop neg 0 rlineto
  closepath
  1 setgray fill
  grestore
  show
  grestore
} def

/nameCapitol {                   %  name >> -
  getCoords moveto
  nameCapitolFont setfont
  gsave
  0 -.55 unit rmoveto
  dup stringwidth pop
  dup dup nameWidthLimit gt
    {nameWidthLimit exch div 1 scale}
    {pop}
  ifelse
  neg 2 div 0 rmoveto
  gsave
  0 nameFontSize -6 div rmoveto
  dup stringwidth pop 0 rlineto
  0 nameFontSize rlineto
  dup stringwidth pop neg 0 rlineto
  closepath
  1 setgray fill
  grestore
  .25 setgray      %number of your own taste, 0 is black, 1 is white
  show
  grestore
} def

/starPort {                % code >> -
  getCoords moveto
  portFont setfont
  dup stringwidth pop 2 div 0 exch sub
    .2 unit rmoveto
  show
} def

/navalBase {               % - >> -
  gsave
  getCoords moveto
  -.5 unit .3 unit rmoveto
  currentpoint translate
  4 {
    .2 unit 0 lineto
    currentpoint translate
    -144 rotate
  } repeat
  closepath
  fill
  grestore
} def

/navalDepot {               % - >> -
  gsave
  getCoords moveto
  -.5 unit .3 unit rmoveto
  currentpoint translate
  3 {
    .15 unit 0 lineto
    currentpoint translate
    -90 rotate
  } repeat
  closepath
  fill
  grestore
} def

/scoutBase {
  gsave
  newpath
  getCoords moveto
  -.5 unit -.125 unit rmoveto
  currentpoint translate
  60 rotate
  .15 unit 0 lineto
  currentpoint translate
  -120 rotate
  .15 unit 0 lineto
  closepath
  fill
  grestore
} def

/scoutWayStation {
  gsave
  .5 setlinewidth
  newpath
  getCoords moveto
  -.5 unit -.125 unit rmoveto
  currentpoint translate
  60 rotate
  .15 unit 0 lineto
  currentpoint translate
  -120 rotate
  .15 unit 0 lineto
  closepath
  stroke
  grestore
} def

/gasGiant {
  gsave
  newpath
  getCoords moveto
  .4 unit .3 unit rmoveto
  currentpoint translate
  0 0 .05 unit 0 360 arc fill
  grestore
} def

/redZone {
  newpath
  .8 setgray
  getCoords 2 3 div unit 0 360 arc fill
  0 setgray
} def

/amberZone {
  gsave
  newpath
  .7 setlinewidth
  0 setgray
  getCoords 2 3 div unit 0 360 arc stroke
  grestore
} def

/hexNumber {                 % (XX YY) >> -
  getCoords moveto
  numberFont setfont
  dup stringwidth pop 2 div 0 exch sub
    .6 unit rmoveto
  show
} def

%-----Main-----
hexMap
% Other system specs  (start with zones)
