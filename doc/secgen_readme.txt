SecGen v0.2 Alpha
=================
By Omer Golan-Joel


1. What is SecGen?
==================
SecGen is a Python script generating a Classic Traveller sector (or subsector or
quadrant). The result is saved as a SEC file compatible with several
Traveller-related programs created in the past by other people.


2. Why did I write it?
======================
Traveller worlf generation is very easy to automate, so it is both easy to 
automate (saving a lot of time when building a Traveller setting) and a good 
excersize in programming. Given my limited free time, generating a Traveller
sector "by hand" takes about a month; with this simple script, the "raw" data
can be generated in sectonds, ready to be modified to suit the setting's needs.


3. Installation
===============
SecGen is a Python script and thus requires Python 2.6 to be installed on your
computer. You can get Python for free from its official site:
http://www.python.org/


4. Usage
========
Simply run secgen.py by double-clicking on it.


5. Configuration
================
At this stage, SecGen lacks a graphical user interface (GUI). However, it is
possible to modify its function using five text parameters in the sec_cfg.txt
configuration file. It can be modified by editing in any text editor. Each
option depends on the word or words in its row.

sec_cfg.txt has four rows:
--------------------------
1) The first row controls the amount of realism in the generated sector. If it is
   set to "canon" (without the ""), then only canonical Traveller rules will be
   used. If it is set to "real", a few rules will be modified (see below) to reduce
   the amount of nonsensical results.

2) The second row controls the size of the space generated. "sector" generates a
   sector (16 subsectors); "quadrant" generates a quadrant (4 subsectors);
   "subsector" generates a single subsector.

3) The third row determines habitation. "inhabited" generates (mostly) populated
   worlds (as per standard Traveller rukes); "empty" generates only non-inhabited
   worlds.

4) The fourth row determines the sector's name (appearing at the top of the SEC file)

5) The fifth row determines the output sector's filename.


6. Realism (rules modification)
===============================
Using the "real" configuration option, the following modifications will be applied:

-Atmospheres 1 or A+ can only exist on worlds of Size 3+.
-Atmospheres 2-5 can only exist on worlds of Size 4+.
-Atmospheres 6-9 can only exist on worlds of Size 5+.
-If the Population is zero, the Starport is set to X and the Government, 
 Law Level and Population Multiplier are set to 0; bases are never present with 
 such a low population.
- Barren worlds do not recieve the Lo and Ni trade codes.
- Worlds with Atmospheres 4, 7 and 9 must have at least TL5.
- Worlds with Atmospheres 3- must have at least TL7.
- Worlds with Atmospheres A or B must have at least TL8.
- Worlds with Atmosphere C must have at least TL9.
- Worlds with Atmospheres D+ must have at least TL7.
- Worlds with Starport A must have at least a Population of 5 and TL9.
- Worlds with Starport A must have at least a Population of 3 and TL7.
- Stars are generated using Constantine Thomas' rules. For detailed rules look here:
  http://www.evildrganymede.net/rpg/world/new_rsgt.pdf.


7. Rules Used
=============
UWP generation uses Classic Traveller's Book 3 world generation. Trade codes are
based on the Classic Traveller Book 7 (Merchant Prince) rules. PBG codes are
based on Megatraveller's Referee's Book. Stars are generated according to Classic
Traveller's Book 6 (Scouts). The optional realism rules are my house-rules, with
the exception of the stellar generation rules, which were created by Constantine
Thomas.


8. Version History
==================
v0.2 Alpha
----------
- Added Constantine Thomas' stellar generation rules as part of the "realistic" option.
- Slight adjustments done to the Legal Stuff section.

v0.1 Alpha
----------
- First Release - script only, no GUI.


9. Contacting Me
================
I can be reached at golan2072@gmail.com. Comments, suggestions and bug reports will
be welcome :)


10. Legal Stuff
===============
The script itself is free to use, modify and distribute, as long as it is used for a
non-commercial (and free of charge) purpose. The realistic stellar generation rules 
are copyrighted by Constantine Thomas. The Traveller rules used in it are copyrighted 
by Far Future Enterprises (see below) and thus must be accompanied by the following notice:

The Traveller game in all forms is owned by Far Future Enterprises. Copyright
1977 - 2008 Far Future Enterprises. Traveller is a registered trademark of Far Future
Enterprises. Far Future permits web sites and fanzines for this game, provided it
contains this notice, that Far Future is notified, and subject to a withdrawal of
permission on 90 days notice. The contents of this site are for personal, non-commercial
use only. Any use of Far Future Enterprises's copyrighted material or trademarks anywhere
on this web site and its files should not be viewed as a challenge to those copyrights.