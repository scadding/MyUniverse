#!/bin/bash

# SectorMaker.cgi
# by Micki Kaufman (micki001@concentric.net)
# 
#   I was looking at my LBB collection, and got an idea, so voila!
# Presenting the Traveller Sector Guidebook .cgi script. Creates and returns 
# a high-resolution (vector-art) pdf containing the sector data and hexmaps, 
# listed subsector by subsector.
# 
#   Using sector tools, the sector data and individual subsector data is 
# created, and the PostScript hexmap is generated. Using ps2pdf, the PostScript 
# hexmap is converted to a pdf. Using enscript, the text sector data is 
# converted to a  PostScript file. Still working on page size. using pdftk, the 
# pdf's are overlaid and concatenated together. And then we serve it up!
#
# gensec3c code modifications
# binary modifications for Ubuntu
# script modifications for Ubuntu
# by Chris Moynihan (forthekill@gmail.com)

# REQUIREMENTS
#
#   - Apache Web Server
#   - Perl
#   - PDF-API2
#   - ps2pdf (aka GhostScript, this is built in on Mac OSX)
#   - enscript (This is built in on Mac OSX)
#   - pdftk (http://www.pdfhacks.com/pdftk/)

# INSTALLATION INSTRUCTIONS (Ubuntu 8.10, 9.04, 9.10)
# 
# # STEP ONE - INSTALL AND CONFIGURE APACHE HTTP SERVER
# ## 1A. Install the Ubuntu version of Apache HTTP Server
# ## # sudo apt-get install apache2
# ##
# ## 2A. Create the cgi-bin folder
# ## # sudo mkdir /var/www/cgi-bin
# ##
# ## 3A. Edit the /etc/apache2/sites-available/default and change the two occurances of "/usr/lib" in the cgi section to "/var/www"
# ##     Make sure you do this with proper access, or you won't be allowed to save the file.
# ## # 
# ## # .....
# ## #	ScriptAlias /cgi-bin/ /var/www/cgi-bin/
# ## #	<Directory "/var/www/cgi-bin">
# ## #		AllowOverride None
# ## #		Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
# ## #		Order allow,deny
# ## #		Allow from all
# ## #	</Directory>
# ## # .....
# ## # 
# 
# # STEP TWO - INSTALL PERL
# ## Install Perl 5
# ## # sudo apt-get install perl
#
# # STEP THREE - INSTALL PDFTK
# ## Install the Ubuntu version of pdftk (used for splitting, joining, compressing and overlaying pdf pages)
# ## # sudo apt-get install pdftk
#
# # STEP FOUR - INSTALL PDF-API2
# ## Install pdf-API2 (a perl resource for handling pdfs and fonts)
# ## # sudo apt-get install libpdf-api2-perl
#
# # STEP FIVE - INSTALL GHOSTSCRIPT (PS2PDF)
# ## Install ps2pdf
# ## # sudo apt-get install ghostscript
#
# # STEP SIX - CREATE THE OUTPUT AND TEMP FOLDERS
# ## Create folders for the script to use, one for the final output and one for temporary files
# ## # sudo mkdir /sectors
# ## # sudo mkdir /sectors/autogen
# ## # sudo mkdir /sectors/tmp
# ## # sudo chmod a=rwx /sectors/autogen
# ## # sudo chmod a=rwx /sectors/tmp
# 
# # STEP SEVEN - DOWNLOAD AND UNZIP THE SECTORMAKER ARCHIVE
# ## Unzip the SectorMaker files from the archive somewhere easily accessible. This will create a folder called SectorMaker in that location.
# ## # unzip SectorMaker.zip -d /var/tmp/
# ## # cd /var/tmp/SectorMaker
# 
# # STEP EIGHT - COPY TRAV FOLDER
# ## Copy the trav folder to the cgi-bin directory.
# ## # sudo cp -ar ./trav /var/www/cgi-bin/
#
# # STEP NINE - COPY SCRIPTS
# ## Copy the three scripts to the cgi-bin directory.
# ## # sudo cp SectorMaker.cgi SectorMaker.sh BatchSectorMaker.sh converter1.sh /var/www/cgi-bin
# ## # sudo cp *.pl /var/www/cgi-bin
# ## # sudo cp mapsub* /var/www/cgi-bin
#
# # STEP TEN - COPY CUSTOM COREFONTS
# ## Install the custom CoreFont fonts into the newly-installed pdf-api2 CoreFont folder.
# ## # cd install/pdf-api2
# ## # sudo cp universbold.pm univers.pm optima.pm /usr/share/perl5/PDF/API2/Resource/Font/CoreFont/
# ## # sudo cp CoreFont.pm /usr/share/perl5/PDF/API2/Resource/Font/
# 
# # STEP ELEVEN - INSTALL NEW FONTS
# ## Install the required TrueType fonts (a few styles of Univers and Optima) into the fonts folder.
# ## # sudo mkdir /usr/share/fonts/truetype/SectorMaker
# ## # cd /var/tmp/SectorMaker/install/ttf
# ## # sudo cp Univers* Optima* /usr/share/fonts/truetype/SectorMaker
# ## # sudo fc-cache -f -v
# 
# # STEP TWELVE - COPY CUSTOM BINARIES
# ## Install the custom SW4 Traveller binaries into your computer's '/usr/bin/' folder.
# ## # cd /var/tmp/SectorMaker/install/bin
# ## # sudo cp full-upp mapsub3 subsec2 gensec3 gensec3c /usr/bin/
#
# # STEP THIRTEEN - SET SCRIPT PERMISSIONS
# ## Move to the cgi-bin directory and set the script permissions properly.
# ## # cd /var/www/cgi-bin
# ## # sudo chmod ugo+x SectorMaker.cgi SectorMaker.sh BatchSectorMaker.sh
#
# # STEP FOURTEEN - INSTALL ENSCRIPT
# ## Install enscript.
# ## # sudo apt-get install enscript
#
# # STEP FIFTEEN - COPY CUSTOM ENSCRIPT FILES
# ## Install custom enscript files.
# ## # cd /var/tmp/SectorMaker/install/enscript
# ## # sudo cp enscript.cfg /etc/
# ## # sudo cp traveller*.hdr /usr/share/enscript/
#  
# # STEP SIXTEEN - FOR 64-bit OS ONLY
# ## Install ia32-libs for support for running the 32-bit binaries
# ## # sudo apt-get install ia32-libs

# USING THE SCRIPTS
# 
# ## COMMAND LINE USE
# ## To use the script from the command line, navigate to the /var/www/cgi-bin folder and use the following syntax:
#
# ./SectorMaker.sh {sectorname} {density} {tech} {desc} {{secfile}} {{regen}}
#  
# ## COMMAND LINE BATCH MODE
# ## To use the batch sector maker, navigate to the /var/www/cgi-bin folder and use the following syntax:
#
# ./BatchSectorMaker.sh {number of sectors} {tech} {desc}
#
#
# ## WEB BROWSER USE
# ## To use the script from a web browser, use the syntax for either of the two following URLs:
#
# http://[servername]/cgi-bin/SectorMaker.cgi/sectorname.pdf?{sectorname}+{density}+{tech}+{desc}+{{secfile}}+{{regen}}
# http://[servername]/cgi-bin/SectorMaker.cgi?{sectorname}+{density}+{tech}+{desc}+{{secfile}}+{{regen}}
#
# The first URL will return a file called sectorname.pdf to the browser.
# The second URL will return a file called SectorMaker.cgi to the browser, which is actually a PDF.
#
#
# ## PARAMETERS:
# 
#  {sectorname}
#    What you would like the name of the sector to be
# 
#  {density|zero,rift,sparse,scattered,dense, or Xx}
#    Determines the stellar density of the sector (avg. # of worlds per subsector)
#     zero = 0% (Will not randomly generate worlds, you must have a sectorname_names.txt file in /sectors/autogen for the world locations and names to use)
#     rift = 4%  (~1-10)
#     sparse = 16% (~5-20)
#     scattered = 33% (~19-35)
#     dense = 66% (~40-62)
#     Xx = Xx% density
# 
#  {tech|backwater,frontier,mature,cluster}
#    The general technology level of the sector, how well travelled it is
# 
#  {desc|1,2,3}
#    Level of description for generated worlds
#    1 = UPP and Animal
#    2 = UPP only
#    3 = None
# 
#  {secfile|sectorname.sec}
#    Pre-existing sector file name. Used if an existing set of files is available in /sectors/autogen
#    (sectorname.sec, sectorname_route.txt, sectorname_alliances.txt)
#    Using this parameter renders the density and tech parameters irrelevant, although in bash script you need to have placeholders
# 
#  {regen|1,2,3}
#    Pre-existing regeneration options
#    1 = Use pre-existing routes and borders (may generate additional routes and borders)
#    2 = Use pre-existing routes only
#    3 = Use pre-existing routes and borders only 

# Far Future Enterprises Fair Use Notice:
#
# The Traveller game in all forms is owned by Far Future Enterprises. Copyright 
# 1977 - 1998 Far Future Enterprises. Traveller is a registered trademark of Far 
# Future Enterprises.  Far Future permits web sites and fanzines for this game, 
# provided it contains this notice, that Far Future is notified, and subject to 
# a withdrawal of permission on 90 days notice.  The contents of this site are 
# for personal, non-commercial use only. Any use of Far Future Enterprises' 
# copyrighted material or trademarks anywhere on this web site and its files  
# should not be viewed as a challenge to those copyrights or trademarks. In 
# addition, any program/articles/file on this site cannot be republished or 
# distributed without the consent of the author who contributed it.   


#### First, here are the get_UPP, ?(random_Quote)? and parse_SectorDescription functions.

SCORING=1



### get_UPP (requires 'full-upp', installed in bin folder)

function get_UPP()
{
full-upp $2 > /sectors/tmp/$1uppo.txt
cat /sectors/tmp/$1uppo.txt /var/www/cgi-bin/trav/ret.txt > /sectors/tmp/$1upp.txt
chmod ugo+w /sectors/tmp/$1upp.txt /sectors/tmp/$1uppo.txt
#echo "Content-Type: application/text" 
#echo " Filename: "$1upp.txt
echo ""
echo $1
echo "Name: "$4 > /sectors/tmp/$1name1.txt
sed '/_/ s/_/ /g' /sectors/tmp/$1name1.txt > /sectors/tmp/$1name.txt
cat < /sectors/tmp/$1name.txt
sed -n '/ '$3' /p' /sectors/tmp/*_polities.txt > /sectors/tmp/$1polityinter.txt
cat /var/www/cgi-bin/trav/Affiliation.txt /sectors/tmp/$1polityinter.txt > /sectors/tmp/$1polity.txt
cat < /sectors/tmp/$1polity.txt
#echo ""
echo "UPP: "$2
echo ""
cat < /sectors/tmp/$1upp.txt
rm -rf /sectors/tmp/$1upp.txt
rm -rf /sectors/tmp/$1polityinter.txt
rm -rf /sectors/tmp/$1polity.txt
rm -rf /sectors/tmp/*uppo.txt
rm -rf /sectors/tmp/$1name1.txt
rm -rf /sectors/tmp/$1name.txt
     return
     }




### get_More (grabs cargo and animal encounters)

function get_More() 
{
cd /var/www/cgi-bin/trav
perl shell.pl --tool=animal --extended --profile=$2 > /sectors/tmp/$1animo.txt
perl shell.pl --tool=bk7.trade.cargo --profile=$2 > /sectors/tmp/$1cargo.txt
cat /sectors/tmp/$1animo.txt /var/www/cgi-bin/trav/ret.txt > /sectors/tmp/$1anim.txt
chmod ugo+w /sectors/tmp/$1anim.txt /sectors/tmp/$1animo.txt
#echo "Content-Type: application/text" 
#echo " Filename: "$1anim.txt
#echo ""
echo "Generic Cargo:"
#echo ""
cat < /sectors/tmp/$1cargo.txt
#echo $1
echo ""
echo "Animal Encounters:"
#echo ""
cat < /sectors/tmp/$1anim.txt
rm -rf /sectors/tmp/$1anim.txt
rm -rf /sectors/tmp/$1animo.txt
cd /var/www/cgi-bin/
     return
     }





### get_SectorDescriptions grabs each of the planet's descriptions (using the get_UPP function on each) and then serves the whole thing up as a text file and pdf.

function get_SectorDescriptions()
{
needforanim=2


## Main Loop
if [ "$needforanim" -lt "$desc" ]; then 	# -n tests to see if the argument is non empty
     while read LINE
    do
          get_UPP $LINE $2 > "/sectors/tmp/$1det$LINE-a.txt"
#               echo "  .  $1 World debug - UPP description render complete @" $SECONDS "seconds.   "
               done < /sectors/tmp/$1
else
     while read LINE
    do
          get_UPP $LINE $2 > "/sectors/tmp/$1det$LINE-a.txt"
#               echo "  .  $1 World debug - UPP description render complete @" $SECONDS "seconds.   "
          get_More $LINE > "/sectors/tmp/$1det$LINE-banim.txt"
#                echo "  .  $1 World debug - cargo and animal encounters description render complete @" $SECONDS "seconds.   "
              done < /sectors/tmp/$1
          fi


# Combine all the descriptions into one text file.
#echo " Combine descriptions to one text file "
cat /var/www/cgi-bin/trav/pageblanker.txt /sectors/tmp/$2title.txt /var/www/cgi-bin/trav/descinfo.txt /var/www/cgi-bin/trav/ret.txt /sectors/tmp/$1det*.txt > /sectors/tmp/$1finaldet.txt
wait

## Assemble the pages into a description section pdf

# Create .ps files from each datafile
#echo " Create .ps files from each datafile "
enscript --word-wrap --margins=35:35:0:40 --fancy-header='travellerpagenums2' --header='|- $% -|' --header-font=Univers9 /sectors/tmp/$1finaldet.txt -f Courier6.5 --media=Traveller -q -p /sectors/tmp/$1out.ps
wait

# Create .pdf files from each datafile .ps
#echo " Create .pdf files from each datafile .ps "
ps2pdf /sectors/tmp/$1out.ps /sectors/tmp/$1piecetemp.pdf
wait

# Grab the pages, starting #36 (the prior 35 are blank to set up the numbering, since the pdftk program can't start the numbering at 36.
pdftk /sectors/tmp/$1piecetemp.pdf cat 38-end output /sectors/tmp/$1piecetempText.pdf
wait

# Grab the first page of the descriptions
pdftk /sectors/tmp/$1piecetempText.pdf cat 1-1 output /sectors/tmp/$1piecetempIntro.pdf
wait

# Place the 'World Descriptions' header on the first page, in keeping with the document's section headings.
pdftk /sectors/tmp/$1piecetempIntro.pdf background /var/www/cgi-bin/trav/Desc_Background.pdf output /sectors/tmp/$1pieceDescIntroWithBack.pdf
wait

# Grab the rest of the descriptions
pdftk /sectors/tmp/$1piecetempText.pdf cat 2-end output /sectors/tmp/$1pieceEnd.pdf
wait

# Join the first page of descriptions (now with header) and the rest of the descriptions
pdftk /sectors/tmp/$1pieceDescIntroWithBack.pdf /sectors/tmp/$1pieceEnd.pdf cat output /sectors/tmp/$1piece.pdf
wait

return
}




### get_Sectormap makes a one-page sector map, including borders.

function get_Sectormap()
{

# Wipe pesky underscores
sed '/_/ s/_/ /g' /sectors/tmp/$1secdata > /sectors/tmp/$1secdataZ
cp /sectors/tmp/$1secdataZ /sectors/tmp/$1_secdatapre

# Remove the first line so it doesn't get chunked
sed '1d' /sectors/tmp/$1_secdatapre > /sectors/tmp/$1_secdata

# Cut new chunks for sec2pdf-friendly format
cut -b20-23 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataID
cut -b19-19 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataSpace
cut -b1-14 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataName
cut -b25-34 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataUPP
cut -b35-52 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataTrade
cut -b53-55 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataPBG
cut -b56-58 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataEmpire
cut -b60-60 /sectors/tmp/$1_secdata > /sectors/tmp/$1_secdataTAS

# Combine chunks in sec2pdf-friendly format, with new heading for sec2pdf
paste -d '\0' /sectors/tmp/$1_secdataName /sectors/tmp/$1_secdataID /sectors/tmp/$1_secdataSpace /sectors/tmp/$1_secdataUPP /sectors/tmp/$1_secdataSpace /sectors/tmp/$1_secdataTrade /sectors/tmp/$1_secdataTAS /sectors/tmp/$1_secdataSpace /sectors/tmp/$1_secdataSpace /sectors/tmp/$1_secdataPBG /sectors/tmp/$1_secdataEmpire /sectors/tmp/$1_secdataSpace > /sectors/tmp/$1_newsecdata1
cat /var/www/cgi-bin/trav/SectorTop.txt /sectors/tmp/$1_newsecdata1 > /sectors/tmp/$1_newsecdata

# Grab pre-existing alliances
if [ "$2" != "" ]; then 	# -n tests to see if the argument is non empty
perl allygen2.pl -f 2 -r -o /sectors/tmp/$1_automsec2 /sectors/tmp/$1_newsecdata
#echo "  .  $1 Sector debug - Allygen generation complete @" $SECONDS "seconds.   "

# Strip various allygen items, based on param $6 ($3 here)

# Pre-existing sector file name (used if an existing set of .sec, route and alliance files is available in /sectors/autogen)
## Loop
if [ $3 = 3 ]; then
sed '/# Alliance:/d' /sectors/tmp/$1_automsec2.msec > /sectors/tmp/$1_automsec2a.msec
sed '/border/d' /sectors/tmp/$1_automsec2a.msec > /sectors/tmp/$1_automsec2b.msec
sed '/route/d' /sectors/tmp/$1_automsec2b.msec > /sectors/tmp/$1_automsec3.msec
cat /sectors/autogen/$1_route.txt /sectors/tmp/$1_automsec3.msec > /sectors/tmp/$1_automsec.msec
fi

if [ $3 = 2 ]; then
sed '/# Alliance:/d' /sectors/tmp/$1_automsec2.msec > /sectors/tmp/$1_automsec2a.msec
sed '/route/d' /sectors/tmp/$1_automsec2a.msec > /sectors/tmp/$1_automsec3.msec
cat /sectors/autogen/$1_route.txt /sectors/tmp/$1_automsec3.msec > /sectors/tmp/$1_automsec.msec
fi

if [ $3 = 1 ]; then
cat /sectors/autogen/$1_route.txt /sectors/tmp/$1_automsec2.msec > /sectors/tmp/$1_automsec.msec
fi

if [ $3 = 0 ]; then
cat /sectors/tmp/$1_automsec2.msec > /sectors/tmp/$1_automsec.msec
fi


# Generate Sector Map
#echo " Generate Sector Map "
perl sec2pdf.pl -s $1 -f 2 -b -a -h -B -pTraveller -t sector -m /sectors/tmp/$1_automsec.msec -d /sectors/tmp/$1_newsecdata -o /sectors/tmp/$1_sectormap1.pdf
#echo "  .  $1 Sector debug - Small Sectormap generation complete @" $SECONDS "seconds.   "

# Generate Subsector Maps (borders, routes and regions)
#echo " Generate Subsector Maps "
perl sec2pdf_borders.pl -f 2 -s $1 -a -B -pTraveller -m /sectors/tmp/$1_automsec.msec -d /sectors/tmp/$1_newsecdata -o /sectors/tmp/$1_subsectorborders.pdf
#echo "  .  $1 Sector debug - Subsector routes generation complete @" $SECONDS "seconds.   "

else
# Generate New Allegiances
#echo " Generate New Allegiances "
perl allygen2.pl -f 2 -i -r -o /sectors/tmp/$1_automsec /sectors/tmp/$1_newsecdata
#echo "  .  $1 Sector debug - Allygen generation complete @" $SECONDS "seconds.   "

# Generate Sector Map
perl sec2pdf.pl -s $1 -f 2 -b -a -B -h -pTraveller -t sector -m /sectors/tmp/$1_automsec.msec -d /sectors/tmp/$1_newsecdata -o /sectors/tmp/$1_sectormap1.pdf
#echo "  .  $1 Sector debug - Small Sectormap generation complete @" $SECONDS "seconds.   "

# Generate Sector Map
perl sec2pdf_borders.pl -f 2 -s $1 -a -B -pTraveller -m /sectors/tmp/$1_automsec.msec -d /sectors/tmp/$1_newsecdata -o /sectors/tmp/$1_subsectorborders.pdf
#echo "  .  $1 Sector debug - Subsector routes generation complete @" $SECONDS "seconds.   "
fi

# Combine the map with canon Classic Traveller background
#echo " Combine map with canon background "
pdftk /var/www/cgi-bin/trav/backgrounds/SectorMap_Background.pdf background /sectors/tmp/$1_sectormap1.pdf output /sectors/tmp/$1_sectormap2.pdf compress
wait

return
}




### get_Polity randomly generates names for each of the polities generated by allygen.pl

function get_Polity {
#echo " In get_Polity "
case $((RANDOM%211+1)) in
1)echo $1 "Accomodation (" $2 ")"
;;
2)echo $1 "Accord (" $2 ")"
;;
3)echo $1 "Accordate (" $2 ")"
;;
4)echo $1 "Accordment (" $2 ")"
;;
5)echo $1 "Affiliation (" $2 ")"
;;
6)echo $1 "Affiliate (" $2 ")"
;;
7)echo $1 "Affinity (" $2 ")"
;;
8)echo $1 "Aggregate (" $2 ")"
;;
9)echo $1 "Alliance (" $2 ")"
;;
10)echo $1 "Amalgam (" $2 ")"
;;
11)echo $1 "Amalgamate (" $2 ")"
;;
12)echo $1 "Amalgamation (" $2 ")"
;;
13)echo $1 "Arbitration (" $2 ")"
;;
14)echo $1 "Ascendancy (" $2 ")"
;;
15)echo $1 "Assemblage (" $2 ")"
;;
16)echo $1 "Assembly (" $2 ")"
;;
17)echo $1 "Association (" $2 ")"
;;
18)echo $1 "Authority (" $2 ")"
;;
19)echo $1 "Autocracy (" $2 ")"
;;
20)echo $1 "Birthright (" $2 ")"
;;
21)echo $1 "Biumvirate (" $2 ")"
;;
22)echo $1 "Brotherhood (" $2 ")"
;;
23)echo $1 "Cabal (" $2 ")"
;;
24)echo $1 "Cartel (" $2 ")"
;;
25)echo $1 "Circle (" $2 ")"
;;
26)echo $1 "Circuit (" $2 ")"
;;
27)echo $1 "Clanholds (" $2 ")"
;;
28)echo $1 "Clanworlds (" $2 ")"
;;
29)echo $1 "Coalition (" $2 ")"
;;
30)echo $1 "Colonies (" $2 ")"
;;
31)echo $1 "Combine (" $2 ")"
;;
32)echo $1 "Command (" $2 ")"
;;
33)echo $1 "Commission (" $2 ")"
;;
34)echo $1 "Commonality (" $2 ")"
;;
35)echo $1 "Commonwealth (" $2 ")"
;;
36)echo $1 "Commune (" $2 ")"
;;
37)echo $1 "Communion (" $2 ")"
;;
38)echo $1 "Compact (" $2 ")"
;;
39)echo $1 "Compromise (" $2 ")"
;;
40)echo $1 "Concord (" $2 ")"
;;
41)echo $1 "Concordate (" $2 ")"
;;
42)echo $1 "Concourse (" $2 ")"
;;
43)echo $1 "Concurrence (" $2 ")"
;;
44)echo $1 "Confed (" $2 ")"
;;
45)echo $1 "Confederacy (" $2 ")"
;;
46)echo $1 "Confederation (" $2 ")"
;;
47)echo $1 "Confines (" $2 ")"
;;
48)echo $1 "Conglomerate (" $2 ")"
;;
49)echo $1 "Congress (" $2 ")"
;;
50)echo $1 "Congruity (" $2 ")"
;;
51)echo $1 "Conjunction (" $2 ")"
;;
52)echo $1 "Consanguinity (" $2 ")"
;;
53)echo $1 "Consolidation (" $2 ")"
;;
54)echo $1 "Consortium (" $2 ")"
;;
55)echo $1 "Conspiracy (" $2 ")"
;;
56)echo $1 "Constellation (" $2 ")"
;;
57)echo $1 "Consulate (" $2 ")"
;;
58)echo $1 "Control (" $2 ")"
;;
59)echo $1 "Cooperate (" $2 ")"
;;
60)echo $1 "Cooperative (" $2 ")"
;;
61)echo $1 "Coordinate (" $2 ")"
;;
62)echo $1 "Corporate (" $2 ")"
;;
63)echo $1 "Council (" $2 ")"
;;
64)echo $1 "County (" $2 ")"
;;
65)echo $1 "Covenant (" $2 ")"
;;
66)echo $1 "Crusade (" $2 ")"
;;
67)echo $1 "Delegation (" $2 ")"
;;
68)echo $1 "Democracy (" $2 ")"
;;
69)echo $1 "Demos (" $2 ")"
;;
70)echo $1 "Dependency (" $2 ")"
;;
71)echo $1 "Desmense (" $2 ")"
;;
72)echo $1 "Dictatorship (" $2 ")"
;;
73)echo $1 "Directorship (" $2 ")"
;;
74)echo $1 "Disjuncture (" $2 ")"
;;
75)echo $1 "District (" $2 ")"
;;
76)echo $1 "Domain (" $2 ")"
;;
77)echo $1 "Dominance (" $2 ")"
;;
78)echo $1 "Dominate (" $2 ")"
;;
79)echo $1 "Dominion (" $2 ")"
;;
80)echo $1 "Duchy (" $2 ")"
;;
81)echo $1 "Dynasty (" $2 ")"
;;
82)echo $1 "Ecclesiasty (" $2 ")"
;;
83)echo $1 "Empire (" $2 ")"
;;
84)echo $1 "Enclave (" $2 ")"
;;
85)echo $1 "Exchange (" $2 ")"
;;
86)echo $1 "Expanse (" $2 ")"
;;
87)echo $1 "Extent (" $2 ")"
;;
88)echo $1 "Faction (" $2 ")"
;;
89)echo $1 "Factors (" $2 ")"
;;
90)echo $1 "Federacy (" $2 ")"
;;
91)echo $1 "Federate (" $2 ")"
;;
92)echo $1 "Federation (" $2 ")"
;;
93)echo $1 "Fraternity (" $2 ")"
;;
94)echo $1 "Free Worlds (" $2 ")"
;;
95)echo $1 "Fusion (" $2 ")"
;;
96)echo $1 "Group (" $2 ")"
;;
97)echo $1 "Guild (" $2 ")"
;;
98)echo $1 "Harmony (" $2 ")"
;;
99)echo $1 "Hegemony (" $2 ")"
;;
100)echo $1 "Hierate (" $2 ")"
;;
101)echo $1 "Indentureship (" $2 ")"
;;
102)echo $1 "Integration (" $2 ")"
;;
103)echo $1 "Interacterate (" $2 ")"
;;
104)echo $1 "Inquisition (" $2 ")"
;;
105)echo $1 "Judicature (" $2 ")"
;;
106)echo $1 "Judiciary (" $2 ")"
;;
107)echo $1 "Judiciate (" $2 ")"
;;
108)echo $1 "Junction (" $2 ")"
;;
109)echo $1 "Juncture (" $2 ")"
;;
110)echo $1 "Jurisdiction (" $2 ")"
;;
111)echo $1 "Kinship (" $2 ")"
;;
112)echo $1 "Kinworlds (" $2 ")"
;;
113)echo $1 "Kithworlds (" $2 ")"
;;
114)echo $1 "League (" $2 ")"
;;
115)echo $1 "Legacy (" $2 ")"
;;
116)echo $1 "Legate (" $2 ")"
;;
117)echo $1 "Legion (" $2 ")"
;;
118)echo $1 "Magistracy (" $2 ")"
;;
119)echo $1 "Magistrate (" $2 ")"
;;
120)echo $1 "Mandate (" $2 ")"
;;
121)echo $1 "Manifest (" $2 ")"
;;
122)echo $1 "Merger (" $2 ")"
;;
123)echo $1 "Ministry (" $2 ")"
;;
124)echo $1 "Monarchy (" $2 ")"
;;
125)echo $1 "Monopoly (" $2 ")"
;;
126)echo $1 "Mutuality (" $2 ")"
;;
127)echo $1 "Network (" $2 ")"
;;
128)echo $1 "Occupation (" $2 ")"
;;
129)echo $1 "Order (" $2 ")"
;;
130)echo $1 "Organization (" $2 ")"
;;
131)echo $1 "Outlands (" $2 ")"
;;
132)echo $1 "Outworlds (" $2 ")"
;;
133)echo $1 "Pact (" $2 ")"
;;
134)echo $1 "Partnership (" $2 ")"
;;
135)echo $1 "Patronage (" $2 ")"
;;
136)echo $1 "Polity (" $2 ")"
;;
137)echo $1 "Possessions (" $2 ")"
;;
138)echo $1 "Power (" $2 ")"
;;
139)echo $1 "Predominance (" $2 ")"
;;
140)echo $1 "Preeminence (" $2 ")"
;;
141)echo $1 "Prepotency (" $2 ")"
;;
142)echo $1 "Prerogative (" $2 ")"
;;
143)echo $1 "Presidency (" $2 ")"
;;
144)echo $1 "Primacy (" $2 ")"
;;
145)echo $1 "Principality (" $2 ")"
;;
146)echo $1 "Proletariat (" $2 ")"
;;
147)echo $1 "Protectorate (" $2 ")"
;;
148)echo $1 "Province (" $2 ")"
;;
149)echo $1 "Radius (" $2 ")"
;;
150)echo $1 "Range (" $2 ")"
;;
151)echo $1 "Reaches (" $2 ")"
;;
152)echo $1 "Realm (" $2 ")"
;;
153)echo $1 "Reconciliation (" $2 ")"
;;
154)echo $1 "Regency (" $2 ")"
;;
155)echo $1 "Regime (" $2 ")"
;;
156)echo $1 "Region (" $2 ")"
;;
157)echo $1 "Regnum (" $2 ")"
;;
158)echo $1 "Reign (" $2 ")"
;;
159)echo $1 "Republic (" $2 ")"
;;
160)echo $1 "Rimworlds (" $2 ")"
;;
161)echo $1 "Ring (" $2 ")"
;;
162)echo $1 "Scope (" $2 ")"
;;
163)echo $1 "Settlements (" $2 ")"
;;
164)echo $1 "Signatorate (" $2 ")"
;;
165)echo $1 "Signatory (" $2 ")"
;;
166)echo $1 "Society (" $2 ")"
;;
167)echo $1 "Sodality (" $2 ")"
;;
168)echo $1 "Sovereignty (" $2 ")"
;;
169)echo $1 "Sphere (" $2 ")"
;;
170)echo $1 "States (" $2 ")"
;;
171)echo $1 "Stronghold (" $2 ")"
;;
172)echo $1 "Subjugacy (" $2 ")"
;;
173)echo $1 "Subjugate (" $2 ")"
;;
174)echo $1 "Superiority (" $2 ")"
;;
175)echo $1 "Supremacy (" $2 ")"
;;
176)echo $1 "Suzerainty (" $2 ")"
;;
177)echo $1 "Syndicate (" $2 ")"
;;
178)echo $1 "Synthesis (" $2 ")"
;;
179)echo $1 "Sweep (" $2 ")"
;;
180)echo $1 "Technocracy (" $2 ")"
;;
181)echo $1 "Territories (" $2 ")"
;;
182)echo $1 "Theocracy (" $2 ")"
;;
183)echo $1 "Throne Worlds (" $2 ")"
;;
184)echo $1 "Trade Association (" $2 ")"
;;
185)echo $1 "Trade Coalition (" $2 ")"
;;
186)echo $1 "Trade Confederation (" $2 ")"
;;
187)echo $1 "Trade Federation (" $2 ")"
;;
188)echo $1 "Trade Union (" $2 ")"
;;
189)echo $1 "Transcendancy (" $2 ")"
;;
190)echo $1 "Treaty (" $2 ")"
;;
191)echo $1 "Tripartite (" $2 ")"
;;
192)echo $1 "Triumvirate (" $2 ")"
;;
193)echo $1 "Trust (" $2 ")"
;;
194)echo $1 "Tyranny (" $2 ")"
;;
195)echo $1 "Unanimity (" $2 ")"
;;
196)echo $1 "Unification (" $2 ")"
;;
197)echo $1 "Union (" $2 ")"
;;
198)echo $1 "Unity (" $2 ")"
;;
199)echo $1 "Worlds (" $2 ")"
;;
200)echo $1 "Zone (" $2 ")"
;;
201)echo "Grand Duchy of "$1 "(" $2 ")"
;;
202)echo "Great "$1 "(" $2 ")"
;;
203)echo "League of "$1 "Worlds (" $2 ")"
;;
204)echo "Lords of "$1 "(" $2 ")"
;;
205)echo "New Worlds of "$1 "(" $2 ")"
;;
206)echo "Stellar Kingdom of "$1 "(" $2 ")"
;;
207)echo "Unified Systems of "$1 "(" $2 ")"
;;
208)echo "Concordance of "$1 "Worlds (" $2 ")"
;;
209)echo "League of "$1 "Worlds (" $2 ")"
;;
210)echo "Glory of "$1 "(" $2 ")"
;;
211)echo "Planetary Alliance of "$1 "(" $2 ")"
;;
212)echo "Entreaty of "$1 "(" $2 ")"
;;
*)echo $1
;;
esac
return
}



### get_Polities: Grabs each polity

function get_Polities {
    while read LINE
    do
          get_Polity $LINE > "/sectors/tmp/$1polity$LINE.txt"
               done < /sectors/tmp/$1
}





#### Now the main body of the script.


### 1. GENERATE THE COVERS


## Generate the custom cover 

# Generate the text of the sector name
echo $1 > /sectors/tmp/$1seccoverA.txt
sed '/_/ s/_/ /g' /sectors/tmp/$1seccoverA.txt > /sectors/tmp/$1seccover.txt

# Add a line to the sectorname for processing
cat /var/www/cgi-bin/trav/covercat.txt /sectors/tmp/$1seccover.txt > /sectors/tmp/$1seccover2.txt

# LBB cover
enscript --word-wrap /sectors/tmp/$1seccover2.txt -r --margins=266:-300:-400:407 -B -f OptimaItalic32 --media=Letter -q -p /sectors/tmp/$1seccover1.ps

# Turn the generated text white for the cover
sed '/5 578 M/ s/5 578 M/5 578 M 1 setgray/g' /sectors/tmp/$1seccover1.ps > /sectors/tmp/$1seccover.ps 

# Make the generated text of the cover a pdf
ps2pdfwr /sectors/tmp/$1seccover.ps /sectors/tmp/$1seccover1.pdf

# Combine the generated name and the background front cover pdf into the new sector front cover.
pdftk /sectors/tmp/$1seccover1.pdf background /var/www/cgi-bin/trav/SectorData_Cover.pdf output /sectors/tmp/$1CoverInt.pdf
wait

#echo "  .  $1 Sector debug - cover complete @" $SECONDS "seconds. "

## Generate the back page

# Random Quote Grabber
./trav/randomquote.sh > /sectors/tmp/$1backquote.txt

# LBB Back
enscript /sectors/tmp/$1backquote.txt -r --word-wrap --margins=122:-100:364:40 -B -f OptimaItalic20 --media=Letter -q -p /sectors/tmp/$1Back1.ps

# Make the generated text of the back cover a pdf
sed '/5 567 M/ s/5 567 M/5 567 M 1 setgray/g' /sectors/tmp/$1Back1.ps > /sectors/tmp/$1CoverBack.ps 

# Make the generated text of the back cover a pdf
ps2pdfwr /sectors/tmp/$1CoverBack.ps /sectors/tmp/$1BackCovertemp.pdf

# Combine the generated text and the background back cover pdf into the new sector back cover.
pdftk /sectors/tmp/$1BackCovertemp.pdf background /sectors/tmp/$1CoverInt.pdf output /sectors/tmp/$1Cover.pdf
wait

#echo "  .  $1 Sector debug - back cover complete @" $SECONDS "seconds. "
#echo "1:$1 2:$2 3:$3 4:$4 5:$5 6:$6 "


### 2. GENERATE OR GRAB THE SECTOR'S DATA


## Generate the raw subsector data or import it

if [ "$5" != "" ]; then 
cp /sectors/autogen/$5 /sectors/tmp/$1secdata
sed q /sectors/tmp/$1secdata > /sectors/tmp/$1secdataorighead
sed '1d' /sectors/tmp/$1secdata > /sectors/tmp/$1secdataorig
NUM_WORLDS=`wc -l /sectors/tmp/$1secdataorig | awk '{ print $1 }'`

else 
# Old gensec3 call
# gensec3 $2 $3 > /sectors/tmp/$1secdataorig2

# Modified gensec4 call, allows for using the $1_names.txt file
gensec4 -d $2 -m $3 -s $1 -p /sectors/autogen/ -o 5 -u /sectors/tmp/$1secdataorig2

# Save the first line of the sector file ('Version' tag)
sed q /sectors/tmp/$1secdataorig2 > /sectors/tmp/$1secdataorighead

# Remove the first line for the text processing
sed '1d' /sectors/tmp/$1secdataorig2 > /sectors/tmp/$1secdataorig

#echo "  .  $1 Sector debug - data generation complete @" $SECONDS "seconds.   "

## Generate the sector's system names

# Count the number of worlds generated
NUM_WORLDS=`wc -l /sectors/tmp/$1secdataorig | awk '{ print $1 }'`

# Grab the generated datafile's contents preceding and following the name
cut -b20-80 /sectors/tmp/$1secdataorig > /sectors/tmp/$1secdataorigBack
cut -b1-19 /sectors/tmp/$1secdataorig > /sectors/tmp/$1secdataorigNames

# Run the random word generator
perl /var/www/cgi-bin/trav/lc -$NUM_WORLDS /var/www/cgi-bin/trav/namesmaster.txt > /sectors/tmp/$1namegrab1

# Grab the names from the file
sed -e :a -e 's/^.\{1,18\}$/& /;ta' /sectors/tmp/$1namegrab1 > /sectors/tmp/$1namegrab2

# Added these three lines for the $1_names.txt file to format the names properly
# Compare the randomly generated names to the predefined names and only use generated names to replace "Unnamed"
paste -d" " /sectors/tmp/$1secdataorigNames /sectors/tmp/$1namegrab2| awk '{if ($1=="Unnamed"){print $2}else{print $1}}' > /sectors/tmp/$1namegrab3
sed 's/$/                   /' /sectors/tmp/$1namegrab3 > /sectors/tmp/$1namegrab4
cut -b1-19 /sectors/tmp/$1namegrab4 > /sectors/tmp/$1namegrab5

# Combine the three files into a new secdata file with the names inside
# Old call
#paste -d '\0' /sectors/tmp/$1namegrab2 /sectors/tmp/$1secdataorigBack > /sectors/tmp/$1secdatainterim
# New call to accomodate the $1_names file
paste -d '\0' /sectors/tmp/$1namegrab5 /sectors/tmp/$1secdataorigBack > /sectors/tmp/$1secdatainterim

# Paste the header back onto the sector data
cat /sectors/tmp/$1secdataorighead /sectors/tmp/$1secdatainterim > /sectors/tmp/$1secdata

fi

#echo "  .  $1 Sector debug - word generation complete @" $SECONDS "seconds.   "


### 4. GENERATE THE SECTOR MAP

## If we're using a pre-existing sector file, don't change the alliances.

if [ "$5" != "" ]; then 

# Run the map with old alliances retained
get_Sectormap $1 2 $6
wait
sed -n '/route/p' /sectors/tmp/$1_automsec.msec > /sectors/tmp/$1_route.txt

else
# Run the map with new alliances
get_Sectormap $1
wait

#echo "  .  $1 Sector debug - sector map generation complete @" $SECONDS "seconds.   "


## If it's a fresh sector, go ahead and generate the names of the Alliances

# Grab a version of the msec file without 'border'
sed '/# Alliance: / s/# Alliance: / /g' /sectors/tmp/$1_automsec.msec > /sectors/tmp/$1_before.msec

#sed '/# Alliance: /d' /sectors/tmp/$1_automsec.msec > /sectors/tmp/$1_beforeZ.msec
sed '/border/d' /sectors/tmp/$1_automsec.msec > /sectors/tmp/$1_before1a.msec
sed -n '/route/p' /sectors/tmp/$1_before1a.msec > /sectors/tmp/$1_route.txt

sed '/route/d' /sectors/tmp/$1_before.msec > /sectors/tmp/$1_before1.msec
sed '/border/d' /sectors/tmp/$1_before1.msec > /sectors/tmp/$1_after1.msec

sed -n 'G; s/\n/&&/; /^\([ -~]*\n\).*\n\1/d; s/\n//; h; P' /sectors/tmp/$1_after1.msec > /sectors/tmp/$1_after.msec
# Count the alliances in the file
NUM_POLS=`wc -l /sectors/tmp/$1_after.msec | awk '{ print $1 }'`

# Generate the right number of random names
perl /var/www/cgi-bin/trav/lc -$NUM_POLS /var/www/cgi-bin/trav/namesmaster.txt > /sectors/tmp/politynames

# Put the codes with the random names and run the polity combiner
paste -d '\0' /sectors/tmp/politynames /sectors/tmp/$1_after.msec > /sectors/tmp/$1_combo1.msec
sed '/  / s/  / /g' /sectors/tmp/$1_combo1.msec > /sectors/tmp/$1_combo.msec
#echo " Call get_Polities: $1_combo "
get_Polities $1_combo.msec

# Put all the polity names together as the final 'alliances' text
#echo " Put polity names together "
cat /sectors/tmp/$1_combo.msecpolity*.txt > "/sectors/tmp/$1_polities.txt"
#echo " Copy polities to alliances "
cp /sectors/tmp/$1_polities.txt /sectors/tmp/$1_alliances.txt
wait

#echo "  .  $1 Sector debug - alliance generation complete @" $SECONDS "seconds.   "
fi


# NOTE: Need to add code to grab any spaces INSIDE the names and turn them into underscores. Right now it's manual.


## Grab the alliances (pre-existing or not) and parse them into the secdata file

# Save the first line of the sector file ('Version' tag)
#echo " Save the first line of the sector file "
sed q /sectors/tmp/$1secdata > /sectors/tmp/$1secdataorighead2

# Remove the first line for the text processing
#echo " Remove the first line for the text processing "
if [ "$5" != "" ]; then 
cp /sectors/autogen/$1_alliances.txt /sectors/tmp/$1secdatadesc_alliances.txt
wait
cp /sectors/autogen/$1_alliances.txt /sectors/tmp/$1_alliances.txt
wait
cp /sectors/autogen/$1_alliances.txt /sectors/tmp/$1_polities.txt
wait
cp /sectors/tmp/$1secdata /sectors/tmp/$1_automsec.sec
wait
else
sed '1,15d' /sectors/tmp/$1_automsec.sec > /sectors/tmp/$1_newsecdataProc
sed '1d' /sectors/tmp/$1secdata > /sectors/tmp/$1secdataProc

# Splice the new secdata file
#echo " Splice the new secdata file "
cut -b1-56 /sectors/tmp/$1secdataProc > /sectors/tmp/$1_secdataEmpirePre
cut -b60-64 /sectors/tmp/$1secdataProc > /sectors/tmp/$1_secdataEmpirePost
cut -b56-58 /sectors/tmp/$1_newsecdataProc > /sectors/tmp/$1_secdataNewEmpire

# Put it all together
#echo " Put it all together "
paste -d '\0' /sectors/tmp/$1_secdataEmpirePre /sectors/tmp/$1_secdataNewEmpire /sectors/tmp/$1_secdataEmpirePost > /sectors/tmp/$1secdatainterim2
cat /sectors/tmp/$1secdataorighead2 /sectors/tmp/$1secdatainterim2 > /sectors/tmp/$1secdata

# safety kludge for literal problem
#echo " Safety kludge for literal problem "
cp /sectors/tmp/$1_alliances.txt /sectors/tmp/$1secdatadesc_alliances.txt
wait

#echo "  .  $1 Sector debug - alliance parsing complete @" $SECONDS "seconds.   "
fi


### GENERATE THE SUBSECTOR MAPS AND DATA PAGES

#Create Title Text for Listings and Descriptions
#echo " Create Title Text for Listings "
echo ""$1"""" > "/sectors/tmp/$1titleA.txt"
sed '/_/ s/_/\\ /g' "/sectors/tmp/$1titleA.txt" > "/sectors/tmp/$1title.txt"

# Create Title Text for Sector Map
#echo " Create Title Text for Sector Map "
echo "Contains" $NUM_WORLDS" systems." > "/sectors/tmp/$1SectorMapsubtitle.txt"
#echo ""$1""" Sector (containing" $NUM_WORLDS" systems)" > /sectors/tmp/$1SectorMaptitleA.txt
#sed '/_/ s/_/ /g' /sectors/tmp/$1SectorMaptitleA.txt > /sectors/tmp/$1SectorMaptitle.txt

#cat /sectors/tmp/$1title.txt /sectors/tmp/$1SectorMapsubtitle.txt > /sectors/tmp/$1SectorMaptitle.txt
cat /sectors/tmp/$1title.txt /var/www/cgi-bin/trav/ret.txt /sectors/tmp/$1SectorMapsubtitle.txt > /sectors/tmp/$1SectorMaptitle.txt

## Big Per-Subsector Loop Starts
#echo " Big Per-Subsector Loop Starts "
whichborderpage=0
for x in A B C D E F G H I J K L M N O P; do

let "whichborderpage = whichborderpage +1"
# Generate this subsector's data file
#echo " Generate this subsector's data file "
subsec2 S=$x < /sectors/tmp/$1secdata > /sectors/tmp/$1subsec1$x
NUM_SUBSECWORLDS=`wc -l /sectors/tmp/$1subsec1$x | awk '{ print $1 }'`

#this is the line that is messing up with underscores
#sed '/_/ s/_/\\ /g' /sectors/tmp/$1subsec1$x > /sectors/tmp/$1subsec$x
#this is the test for fixing this, which seems to work
sed '/_/ s/_/ /g' /sectors/tmp/$1subsec1$x > /sectors/tmp/$1subsec$x
wait

# Create a duplicate
#echo " Create duplicate "
sed '' /sectors/tmp/$1subsec$x > /sectors/tmp/$1subsecdata$x

# Reorder the ID on the datafile for output only
#echo " Reorder ID "
cut -b20-24 /sectors/tmp/$1subsecdata$x > /sectors/tmp/$1secdatareorderID
cut -b1-19 /sectors/tmp/$1subsecdata$x > /sectors/tmp/$1secdatareorderName1
sed '/_/ s/_/ /g' /sectors/tmp/$1secdatareorderName1 > /sectors/tmp/$1secdatareorderName
cut -b25-81 /sectors/tmp/$1subsecdata$x > /sectors/tmp/$1subsecend$x
#echo " End reorder ID "


## ============= HERE'S THE SCORING ================= ##




## EXPERIMENTAL: Find the capital



function totalize() {
sum=0
i=1
while [ $i -lt 10 ]
do
  eval num=\$$i
  if [ "$num" = "" ]; then
    num=0
  fi
  sum=`expr $sum + $num`
  i=`expr $i + 1`
done
echo `expr "$sum" "+" "0" | awk '{ print $1 }'`
return
}




function tally() {
tallyline=10
filler="_"
while read LINE
do
let "tallyline = tallyline +1"
totalize $LINE > "/sectors/tmp/$2subsecdataItemTally$1_$tallyline"
done < /sectors/tmp/$3

cat /sectors/tmp/$2subsecdataItemTally$1* > /sectors/tmp/$2subsecdataFinalTally$1
return
}






function scorer() {
#echo " In function scorer "
cp /sectors/tmp/$1subsecdata$x /sectors/tmp/$1subsecdataInit$x
cut -b1-19 /sectors/tmp/$1subsecdataInit$x > /sectors/tmp/$1subsecdataScoreName$x
cut -b24-24 /sectors/tmp/$1subsecdataInit$x > /sectors/tmp/$1subsecdataScoreSpace$x
cut -b25-25 /sectors/tmp/$1subsecdataInit$x > /sectors/tmp/$1subsecdataScoreStarport$x
cut -b29-29 /sectors/tmp/$1subsecdataInit$x > /sectors/tmp/$1subsecdataScorePop$x
cut -b32-33 /sectors/tmp/$1subsecdataInit$x > /sectors/tmp/$1subsecdataScoreTech1$x
sed '/-/ s/-/_/g' /sectors/tmp/$1subsecdataScoreTech1$x > /sectors/tmp/$1subsecdataScoreTech$x
cut -b36-52 /sectors/tmp/$1subsecdataInit$x > /sectors/tmp/$1subsecdataScoreTrade$x
cut -b59-61 /sectors/tmp/$1subsecdataInit$x > /sectors/tmp/$1subsecdataScoreBases$x

paste -d '\0' /sectors/tmp/$1subsecdataScoreSpace$x /sectors/tmp/$1subsecdataScoreStarport$x /sectors/tmp/$1subsecdataScoreSpace$x /sectors/tmp/$1subsecdataScorePop$x /sectors/tmp/$1subsecdataScoreSpace$x /sectors/tmp/$1subsecdataScoreTech$x /sectors/tmp/$1subsecdataScoreSpace$x /sectors/tmp/$1subsecdataScoreTrade$x /sectors/tmp/$1subsecdataScoreBases$x > /sectors/tmp/$1subsecdataScore$x

#Tradeclass:
#Ri: +1
#Po: -2
#Hi: +5
#Ni: -1
#Lo: -2
#Ba: -5
#Fl: -1
#echo " Tradeclass "

sed '/Ri/ s/Ri/1/g' /sectors/tmp/$1subsecdataScore$x > /sectors/tmp/$1subsecdataScore1$x
sed '/Po/ s/Po/-2/g' /sectors/tmp/$1subsecdataScore1$x > /sectors/tmp/$1subsecdataScore2$x
sed '/Hi/ s/Hi/5/g' /sectors/tmp/$1subsecdataScore2$x > /sectors/tmp/$1subsecdataScore3$x
sed '/Ni/ s/Ni/-1/g' /sectors/tmp/$1subsecdataScore3$x > /sectors/tmp/$1subsecdataScore4$x
sed '/Lo/ s/Lo/-2/g' /sectors/tmp/$1subsecdataScore4$x > /sectors/tmp/$1subsecdataScore5$x
sed '/Ba/ s/Ba/-5/g' /sectors/tmp/$1subsecdataScore5$x > /sectors/tmp/$1subsecdataScore6$x
sed '/Fl/ s/Fl/-1/g' /sectors/tmp/$1subsecdataScore6$x > /sectors/tmp/$1subsecdataScore7$x

#Zones:
#Amber: -5
#Red: -20
#U: -20
#echo " Zones "

sed '/R$/ s/R$/-20/g' /sectors/tmp/$1subsecdataScore7$x > /sectors/tmp/$1subsecdataScore8$x
sed '/A$/ s/A$/-5/g' /sectors/tmp/$1subsecdataScore8$x > /sectors/tmp/$1subsecdataScore9$x

#StarPort:
#A: +10
#B: +5
#C: +2
#D: +1
#echo " Starport "

sed '/^ A/ s/^ A/10/g' /sectors/tmp/$1subsecdataScore9$x > /sectors/tmp/$1subsecdataScore10$x
sed '/^ B/ s/^ B/5/g' /sectors/tmp/$1subsecdataScore10$x > /sectors/tmp/$1subsecdataScore11$x
sed '/^ C/ s/^ C/2/g' /sectors/tmp/$1subsecdataScore11$x > /sectors/tmp/$1subsecdataScore12$x
sed '/^ D/ s/^ D/1/g' /sectors/tmp/$1subsecdataScore12$x > /sectors/tmp/$1subsecdataScore13$x
sed '/^ E/ s/^ E/0/g' /sectors/tmp/$1subsecdataScore13$x > /sectors/tmp/$1subsecdataScore13a$x
sed '/^ X/ s/^ X/0/g' /sectors/tmp/$1subsecdataScore13a$x > /sectors/tmp/$1subsecdataScore13b$x

#Population:
#Add population number
#echo " Population "

sed '/ A / s/ A / 10 /g' /sectors/tmp/$1subsecdataScore13b$x > /sectors/tmp/$1subsecdataScore14$x
sed '/ B / s/ B / 11 /g' /sectors/tmp/$1subsecdataScore14$x > /sectors/tmp/$1subsecdataScore15$x
sed '/ C / s/ C / 12 /g' /sectors/tmp/$1subsecdataScore15$x > /sectors/tmp/$1subsecdataScore16$x
sed '/ D / s/ D / 13 /g' /sectors/tmp/$1subsecdataScore16$x > /sectors/tmp/$1subsecdataScore17$x
sed '/ E / s/ E / 14 /g' /sectors/tmp/$1subsecdataScore17$x > /sectors/tmp/$1subsecdataScore18$x
sed '/ F / s/ F / 15 /g' /sectors/tmp/$1subsecdataScore18$x > /sectors/tmp/$1subsecdataScore19$x
sed '/ G / s/ G / 16 /g' /sectors/tmp/$1subsecdataScore19$x > /sectors/tmp/$1subsecdataScore20$x
sed '/ H / s/ H / 17 /g' /sectors/tmp/$1subsecdataScore20$x > /sectors/tmp/$1subsecdataScore21$x
sed '/ I / s/ I / 18 /g' /sectors/tmp/$1subsecdataScore21$x > /sectors/tmp/$1subsecdataScore22$x

#TechLevel:
#Add 2*techlevel
#echo " TechLevel "

sed '/_0 / s/_0 / 0 /g' /sectors/tmp/$1subsecdataScore22$x > /sectors/tmp/$1subsecdataScore22a$x
sed '/_1 / s/_1 / 2 /g' /sectors/tmp/$1subsecdataScore22a$x > /sectors/tmp/$1subsecdataScore22b$x
sed '/_2 / s/_2 / 4 /g' /sectors/tmp/$1subsecdataScore22b$x > /sectors/tmp/$1subsecdataScore22c$x
sed '/_3 / s/_3 / 6 /g' /sectors/tmp/$1subsecdataScore22c$x > /sectors/tmp/$1subsecdataScore22d$x
sed '/_4 / s/_4 / 8 /g' /sectors/tmp/$1subsecdataScore22d$x > /sectors/tmp/$1subsecdataScore22e$x
sed '/_5 / s/_5 / 10 /g' /sectors/tmp/$1subsecdataScore22e$x > /sectors/tmp/$1subsecdataScore22f$x
sed '/_6 / s/_6 / 12 /g' /sectors/tmp/$1subsecdataScore22f$x > /sectors/tmp/$1subsecdataScore22g$x
sed '/_7 / s/_7 / 14 /g' /sectors/tmp/$1subsecdataScore22g$x > /sectors/tmp/$1subsecdataScore22h$x
sed '/_8 / s/_8 / 16 /g' /sectors/tmp/$1subsecdataScore22h$x > /sectors/tmp/$1subsecdataScore22i$x
sed '/_9 / s/_9 / 18 /g' /sectors/tmp/$1subsecdataScore22i$x > /sectors/tmp/$1subsecdataScore22j$x
sed '/_A / s/_A / 20 /g' /sectors/tmp/$1subsecdataScore22j$x > /sectors/tmp/$1subsecdataScore22k$x
sed '/_B / s/_B / 22 /g' /sectors/tmp/$1subsecdataScore22k$x > /sectors/tmp/$1subsecdataScore22l$x
sed '/_C / s/_C / 24 /g' /sectors/tmp/$1subsecdataScore22l$x > /sectors/tmp/$1subsecdataScore22m$x
sed '/_D / s/_D / 26 /g' /sectors/tmp/$1subsecdataScore22m$x > /sectors/tmp/$1subsecdataScore22n$x
sed '/_E / s/_E / 28 /g' /sectors/tmp/$1subsecdataScore22n$x > /sectors/tmp/$1subsecdataScore22o$x
sed '/_F / s/_F / 30 /g' /sectors/tmp/$1subsecdataScore22o$x > /sectors/tmp/$1subsecdataScore22p$x
sed '/_G / s/_G / 32 /g' /sectors/tmp/$1subsecdataScore22p$x > /sectors/tmp/$1subsecdataScore22q$x
sed '/_H / s/_H / 34 /g' /sectors/tmp/$1subsecdataScore22q$x > /sectors/tmp/$1subsecdataScore22r$x
sed '/_I / s/_I / 36 /g' /sectors/tmp/$1subsecdataScore22r$x > /sectors/tmp/$1subsecdataScore23$x

#Bases:
#Z, Y, G, N, 2, H: +4
#
#Planet names:
#Provincial name: -100 (To avoid that a planet with a name that sounds like a small place becomes the capital)
#echo " Bases "
sed '/[A-Z][a-z]/ s/[A-Z][a-z]/ /g' /sectors/tmp/$1subsecdataScore23$x > /sectors/tmp/$1subsecdataScoreFinal1$x
sed '/  */ s/  */ /g' /sectors/tmp/$1subsecdataScoreFinal1$x > /sectors/tmp/$1subsecdataScoreFinal$x
#echo " Done bases "

return
}






function grabCapital() {

# get the lowercase version of the subsector letter
LOWERSUB=`echo $x |tr "[A-Z]" "[a-z]" `
#echo $LOWERSUB

# find the line in the 'routes' file containing the subsector name, if it exists.
SUBSECNAMEFOUND=`sed -n "/^$LOWERSUB /p" /sectors/tmp/$1_automsec.msec | awk '{ print $2 " " $3 " " $4 " " $5 }'`
#echo $SUBSECNAMEFOUND

# if so, grab the name's contents and put it into 'CAPITALNAME'.
if [ "$SUBSECNAMEFOUND" != "" ]; then
echo $SUBSECNAMEFOUND > /sectors/tmp/$1_subsecname$x.txt
CAPITALNAME=`cat < /sectors/tmp/$1_subsecname$x.txt | awk '{ print $0 }'`

# If not, combine a new file with 'Ca' autogenerated.
else

cut -b1-19 /sectors/tmp/$1subsecdata$x > /sectors/tmp/$1subsecdataName$x
cut -b36-51 /sectors/tmp/$1subsecdata$x > /sectors/tmp/$1subsecdataTrade$x
paste -d '\0' /sectors/tmp/$1subsecdataTrade$x /sectors/tmp/$1subsecdataName$x > /sectors/tmp/$1subsecdataSum1$x
sed -n -e '/ Ca /{1!p;g;$!N;}' -e h /sectors/tmp/$1subsecdataSum1$x > /sectors/tmp/$1subsecdataSum2$x
cut -b17-60 /sectors/tmp/$1subsecdataSum2$x > /sectors/tmp/$1subsecdataSum3$x
sed 's/[ \t]*$//' /sectors/tmp/$1subsecdataSum3$x > /sectors/tmp/$1subsecdataSum4$x 
PRIORCAPITAL=`cat < /sectors/tmp/$1subsecdataSum4$x | awk '{ print $0 }'`

# If no line contains 'Ca'
if [ "$PRIORCAPITAL" != "" ]; then
CAPITALNAME=`cat < /sectors/tmp/$1subsecdataSum4$x | awk '{ print $1 }'`
else
CAPITALNAME=`cat < /sectors/tmp/$1subsecdataScorerSheetCapital$x | awk '{ print $2 }'`
WORLDLINE=`cat < /sectors/tmp/$1subsecdataScorerSheetCapital$x | awk '{ print $1 }'`

# Revise subsector data to include 'Capital'

#remove prior lines
#remove subsequent lines
#grab trade chunk
#replace chunk's last two digits with 'Ca'
#reduce chunk's white space to 1 space only
#recombine chunk into line
#recombine subsector data into new secfile

fi

fi

echo "The "$CAPITALNAME" subsector contains "$NUM_SUBSECWORLDS" worlds."  > /sectors/tmp/$1subsecdataSum$x
#, with a population of "$SUBSECPOP". The highest population is "$HIGHPOP", at "$HIGHPOPWORLD". The highest tech level is "$HIGHTEK", at "$HIGHTEK_WORLD"."
return
}




## Scoring loop

## Process:
# 1. see if there is a line in the msec file with a lowercase version of the subsector letter and a space at the beginning.
# 2. if so, use that line.
# 3. if not, enable the scoring loop.
# 4. In the scoring loop, if there is a world in the subsector with 'Ca', that's the subsector capital and therefore the name.
# 5. If not, determine the capital and modify the world's line in the .sec file.
#
# If the msec file has a line containing the lowercase variant of the subsector letter, followed by a space, use the rest of the line as the subsector name.
#


if [ $SCORING ]; then

if [ $NUM_SUBSECWORLDS -gt 0 ]; then
#echo " This subsector ( "$x" ) has "$NUM_SUBSECWORLDS" worlds."

scorer $1
wait

# Tally up the scores
#echo " Tally up the scores "
tally $x $1 $1subsecdataScoreFinal$x > /sectors/tmp/$1subsecdataTally$x
#echo " Done tallying "

# Include the names
#echo " Include the names "
paste -d '\0' /sectors/tmp/$1subsecdataFinalTally$x /sectors/tmp/$1subsecdataScoreSpace$x /sectors/tmp/$1subsecdataScoreName$x > /sectors/tmp/$1subsecdataScorerSheet$x
wait

# Sort subsector worlds by tally
#echo " Sort subsector worlds "
sort -n -r /sectors/tmp/$1subsecdataScorerSheet$x > /sectors/tmp/$1subsecdataScorerSheetSorted$x
sed q /sectors/tmp/$1subsecdataScorerSheetSorted$x > /sectors/tmp/$1subsecdataScorerSheetCapital$x

# Run the main loop
#echo "Run the main loop "
grabCapital $1

# Create Subsector subtitle
echo $CAPITALNAME "Subsector" > /sectors/tmp/$1subsectitle$x.txt

# Put the heading on top of this subsector's data file (for printing)
paste -d '\0' /sectors/tmp/$1secdatareorderID /sectors/tmp/$1secdatareorderName /sectors/tmp/$1subsecend$x > /sectors/tmp/$1subsecReorder$x
cat -u /var/www/cgi-bin/trav/introspace.txt /sectors/tmp/$1title.txt /var/www/cgi-bin/trav/ret.txt /sectors/tmp/$1subsectitle$x.txt /var/www/cgi-bin/trav/subtitles/subtitle$x.txt /var/www/cgi-bin/trav/beginfile.txt /sectors/tmp/$1subsecReorder$x /var/www/cgi-bin/trav/ret.txt /sectors/tmp/$1subsecdataSum$x > /sectors/tmp/$1subsecOut$x

else


# Compile 'em without subsector scoring
paste -d '\0' /sectors/tmp/$1secdatareorderID /sectors/tmp/$1secdatareorderName /sectors/tmp/$1subsecend$x > /sectors/tmp/$1subsecReorder$x
cat -u /var/www/cgi-bin/trav/introspace.txt /sectors/tmp/$1title.txt /var/www/cgi-bin/trav/subtitles/subtitle$x.txt /var/www/cgi-bin/trav/beginfile.txt /sectors/tmp/$1subsecReorder$x /var/www/cgi-bin/trav/ret.txt > /sectors/tmp/$1subsecOut$x
wait
fi

fi


## ======================= END SCORING ======================= ##






# Change the version number to trip the logic of the mapper
sed '/3/ s/3/2/g' /sectors/tmp/$1secdataorighead > /sectors/tmp/$1secdataorighead2
wait

# Put the header on each subsector file
cat /sectors/tmp/$1secdataorighead2 /sectors/tmp/$1subsec$x > /sectors/tmp/$1subsecCalc$x
wait

# Map each subsector's data
mapsub3 -p < /sectors/tmp/$1subsecCalc$x > /sectors/tmp/$1subsec$x.ps

# Create .pdf file from this subsectors map's .ps file
/usr/bin/ps2pdfwr /sectors/tmp/$1subsec$x.ps /sectors/tmp/$1subsecinter$x.pdf
wait

# Create .ps file from this subsectors data's file
enscript /sectors/tmp/$1subsecOut$x --word-wrap --margins=40:20:40:40 -B -f Courier7.5 --media=Traveller -q -p /sectors/tmp/$1subsecdata$x.ps
wait

# Create .pdf file from this subsector data's .ps file
/usr/bin/ps2pdfwr /sectors/tmp/$1subsecdata$x.ps /sectors/tmp/$1subsecdatainter$x.pdf
wait

# Place the generated data on a background with header and page number
pdftk /sectors/tmp/$1subsecdatainter$x.pdf background /var/www/cgi-bin/trav/backgrounds/Data_Background$x.pdf output /sectors/tmp/$1subsecdata$x.pdf
wait

# Extract the proper subsector borders page
pdftk /sectors/tmp/$1_subsectorborders.pdf cat $whichborderpage output /sectors/tmp/$1subsecborders$x.pdf
wait

# Combine the borders and the previous pdf
pdftk /sectors/tmp/$1subsecinter$x.pdf background /sectors/tmp/$1subsecborders$x.pdf output /sectors/tmp/$1subsecinter2$x.pdf
wait

# Place the generated map on a background with header and page number
pdftk /var/www/cgi-bin/trav/backgrounds/Map_Background$x.pdf background /sectors/tmp/$1subsecinter2$x.pdf output /sectors/tmp/$1subsec$x.pdf
wait



#echo "  .  $1 Sector debug - " $x "subsector data / map parsing complete @" $SECONDS "seconds.   "

# Each subsector's loop ends here
done

# Then recombine subsectors into new secdatafile


### Assemble final .pdf containing cover and each subsector's map and data

# Sectormap Page

# Combine text for Sectormap
cat /var/www/cgi-bin/trav/ret.txt /var/www/cgi-bin/trav/ret.txt /var/www/cgi-bin/trav/ret.txt /var/www/cgi-bin/trav/ret.txt /sectors/tmp/$1SectorMaptitle.txt > /sectors/tmp/$1Maptitle.txt 

# Grab the Sector's title for the map page
enscript --word-wrap --margins=35:-100:0:40 --fancy-header='travellerpagenums' --header='' --header-font=Univers9 /sectors/tmp/$1Maptitle.txt -f Univers8 --media=Traveller -q -p /sectors/tmp/$1_SectorMapSectorTitle.ps
wait

# Make the generated text of the back cover a pdf
ps2pdfwr /sectors/tmp/$1_SectorMapSectorTitle.ps /sectors/tmp/$1_SectorMapSectorTitle.pdf

#place the map on a Classic Traveller background
pdftk /sectors/tmp/$1_SectorMapSectorTitle.pdf background /sectors/tmp/$1_sectormap2.pdf output /sectors/tmp/$1_sectormap.pdf
wait


# DEBUG - echo "  .  $1 Sector debug - sector map generation complete @" $SECONDS "seconds.   "



# Polities Page

# combine text
cat /var/www/cgi-bin/trav/ret.txt /var/www/cgi-bin/trav/ret.txt /var/www/cgi-bin/trav/ret.txt /var/www/cgi-bin/trav/ret.txt /sectors/tmp/$1title.txt /var/www/cgi-bin/trav/ret.txt /sectors/tmp/$1_alliances.txt > /sectors/tmp/$1_politiespage.txt
wait

#turn to ps file
enscript --word-wrap --margins=35:-100:0:40 --fancy-header='travellerpagenums' --header='' --header-font=Univers9 /sectors/tmp/$1_politiespage.txt -f Univers8 --media=Traveller -q -p /sectors/tmp/$1_politiespageout.ps
wait

# turn to pdf
ps2pdfwr /sectors/tmp/$1_politiespageout.ps /sectors/tmp/$1_politiespage1.pdf
wait

# join with background (header, page nums)
pdftk /sectors/tmp/$1_politiespage1.pdf background /var/www/cgi-bin/trav/backgrounds/SectorPolities_Background.pdf output /sectors/tmp/$1_politiespage.pdf
wait

#echo "  .  $1 Sector debug - polities generation complete @" $SECONDS "seconds.   "



## Decide - descriptions or no?
# Evaluate whether the fourth argument is higher than 1 (to bypass descriptions)

needfordesc=3
desc=$4

let "desc = desc + 1"

if [ "$needfordesc" -lt "$desc" ]; then 	# -n tests to see if the argument is non empty


## BRING IT TOGETHER WITHOUT DESCRIPTIONS

# Bring it together with the right TOC (no 'world descriptions' line)
pdftk /sectors/tmp/$1Cover.pdf /var/www/cgi-bin/trav/SectorData_IntNoDesc.pdf /sectors/tmp/$1subsecdataA.pdf /sectors/tmp/$1subsecA.pdf /sectors/tmp/$1subsecdataB.pdf /sectors/tmp/$1subsecB.pdf /sectors/tmp/$1subsecdataC.pdf /sectors/tmp/$1subsecC.pdf /sectors/tmp/$1subsecdataD.pdf /sectors/tmp/$1subsecD.pdf /sectors/tmp/$1subsecdataE.pdf /sectors/tmp/$1subsecE.pdf /sectors/tmp/$1subsecdataF.pdf /sectors/tmp/$1subsecF.pdf /sectors/tmp/$1subsecdataG.pdf /sectors/tmp/$1subsecG.pdf /sectors/tmp/$1subsecdataH.pdf /sectors/tmp/$1subsecH.pdf /sectors/tmp/$1subsecdataI.pdf /sectors/tmp/$1subsecI.pdf /sectors/tmp/$1subsecdataJ.pdf /sectors/tmp/$1subsecJ.pdf /sectors/tmp/$1subsecdataK.pdf /sectors/tmp/$1subsecK.pdf /sectors/tmp/$1subsecdataL.pdf /sectors/tmp/$1subsecL.pdf /sectors/tmp/$1subsecdataM.pdf /sectors/tmp/$1subsecM.pdf /sectors/tmp/$1subsecdataN.pdf /sectors/tmp/$1subsecN.pdf /sectors/tmp/$1subsecdataO.pdf /sectors/tmp/$1subsecO.pdf /sectors/tmp/$1subsecdataP.pdf /sectors/tmp/$1subsecP.pdf cat output /sectors/tmp/$1pre.pdf
wait
#echo "  .  $1 Sector debug - initial pdf document combination (without descs) complete @" $SECONDS "seconds.   "

# Combine the subsector pages into the final pdf and compress
#pdftk /sectors/tmp/$1pre.pdf /sectors/tmp/$1Back.pdf cat output /sectors/tmp/$1.pdf compress
pdftk /sectors/tmp/$1pre.pdf /sectors/tmp/$1_sectormap.pdf /sectors/tmp/$1_politiespage.pdf cat output /sectors/tmp/$1.pdf compress
wait

# Back up the relevant files in the survey archives
cp /sectors/tmp/$1.pdf /sectors/autogen/$1.pdf
cp /sectors/tmp/$1secdata /sectors/autogen/$1.sec
wait

#echo "  .  $1 Sector debug - final document combination (without descs) complete @" $SECONDS "seconds.   "
else 


## PARSE THE UPP'S OF EACH WORLD INTO TEXTUAL DESCRIPTIONS

# Cut and recombine the ID, UPP, NAME and Polity fields into a new list
cut -b20-24 /sectors/tmp/$1secdata > /sectors/tmp/$1secdatadescreformID
cut -b25-34 /sectors/tmp/$1secdata > /sectors/tmp/$1secdatadescreformSpaceUPP
cut -b1-18 /sectors/tmp/$1secdata > /sectors/tmp/$1secdatadescreformName
cut -b57-59 /sectors/tmp/$1secdata > /sectors/tmp/$1secdatadescreformEmpire

paste -d '\0' /sectors/tmp/$1secdatadescreformID /sectors/tmp/$1secdatadescreformSpaceUPP /sectors/tmp/$1secdatadescreformEmpire /sectors/tmp/$1secdatadescreformName > /sectors/tmp/$1secdatareform
#sed -e 's/ /\+/' /sectors/tmp/$1secdatareform > /sectors/tmp/$1secdatareform2
#sed -e 's/-//' /sectors/tmp/$1secdatareform2 > /sectors/tmp/$1secdatareform3
#sed -e 's/-//' /sectors/tmp/$1secdatareform > /sectors/tmp/$1secdatareform4
#sed -e 's/ /\+/' /sectors/tmp/$1secdatareform3 > /sectors/tmp/$1secdataform4
#sed '1d' /sectors/tmp/$1secdataform4 > /sectors/tmp/$1secdatadesc
sed '1d' /sectors/tmp/$1secdatareform > /sectors/tmp/$1secdatadesc

# Run the Descriptions generator
get_SectorDescriptions $1secdatadesc $1
wait

## BRING IT TOGETHER WITH TEXTUAL DESCRIPTIONS

# Bring it together with the right TOC (w/ 'world descriptions' line)
pdftk /sectors/tmp/$1Cover.pdf /var/www/cgi-bin/trav/SectorData_IntDesc.pdf /sectors/tmp/$1subsecdataA.pdf /sectors/tmp/$1subsecA.pdf /sectors/tmp/$1subsecdataB.pdf /sectors/tmp/$1subsecB.pdf /sectors/tmp/$1subsecdataC.pdf /sectors/tmp/$1subsecC.pdf /sectors/tmp/$1subsecdataD.pdf /sectors/tmp/$1subsecD.pdf /sectors/tmp/$1subsecdataE.pdf /sectors/tmp/$1subsecE.pdf /sectors/tmp/$1subsecdataF.pdf /sectors/tmp/$1subsecF.pdf /sectors/tmp/$1subsecdataG.pdf /sectors/tmp/$1subsecG.pdf /sectors/tmp/$1subsecdataH.pdf /sectors/tmp/$1subsecH.pdf /sectors/tmp/$1subsecdataI.pdf /sectors/tmp/$1subsecI.pdf /sectors/tmp/$1subsecdataJ.pdf /sectors/tmp/$1subsecJ.pdf /sectors/tmp/$1subsecdataK.pdf /sectors/tmp/$1subsecK.pdf /sectors/tmp/$1subsecdataL.pdf /sectors/tmp/$1subsecL.pdf /sectors/tmp/$1subsecdataM.pdf /sectors/tmp/$1subsecM.pdf /sectors/tmp/$1subsecdataN.pdf /sectors/tmp/$1subsecN.pdf /sectors/tmp/$1subsecdataO.pdf /sectors/tmp/$1subsecO.pdf /sectors/tmp/$1subsecdataP.pdf /sectors/tmp/$1subsecP.pdf cat output /sectors/tmp/$1pre.pdf
wait
#echo "  .  $1 Sector debug - initial pdf document combination (with descs) complete @" $SECONDS "seconds.   "

# Combine the subsector pages and descriptions pages, plus covers into the final pdf and compress
#pdftk /sectors/tmp/$1pre.pdf /sectors/tmp/$1secdatadescpiece.pdf /sectors/tmp/$1Back.pdf cat output /sectors/tmp/$1.pdf compress
pdftk /sectors/tmp/$1pre.pdf /sectors/tmp/$1_sectormap.pdf /sectors/tmp/$1_politiespage.pdf /sectors/tmp/$1secdatadescpiece.pdf cat output /sectors/tmp/$1.pdf compress
wait

# Trim the description file's beginning section
sed '1,2596d' /sectors/tmp/$1secdatadescfinaldet.txt > /sectors/tmp/$1secdatadescfinaldet1.txt
cat /sectors/tmp/$1title.txt /sectors/tmp/$1secdatadescfinaldet1.txt > /sectors/tmp/$1_desc.txt
# Back up the relevant files in the survey archives
cp /sectors/tmp/$1.pdf /sectors/autogen/$1.pdf
cp /sectors/tmp/$1secdata /sectors/autogen/$1.sec
cp /sectors/tmp/$1_desc.txt /sectors/autogen/$1_desc.txt
wait

#echo "  .  $1 Sector debug - document combination (with descs) complete @" $SECONDS "seconds.   "
fi

### BACK THE REST UP

# Obsolete backups
#cp /sectors/tmp/$1_sectormap.pdf /sectors/autogen/$1_sectormap.pdf
#cp /sectors/tmp/$1_subsectorborders.pdf /sectors/autogen/$1_subsectorborders.pdf
#pdftk /sectors/tmp/$1.pdf cat 2-end output /sectors/tmp/$1nocover.pdf

# Optional (?): Save file versions for 'sec2pdf' backwards compatibility
cp /sectors/tmp/$1_polities.txt /sectors/autogen/$1_alliances.txt
#sed '/# Alliance: /d' /sectors/tmp/$1_automsec.msec > /sectors/tmp/$1route1.txt
#sed '/border/d' /sectors/tmp/$1route1.txt > /sectors/autogen/$1_route.txt
cp /sectors/tmp/$1_automsec.msec /sectors/autogen/$1_route.txt
#cp /sectors/tmp/$1_automsec.sec /sectors/autogen/$1_automsec.sec

#echo "  .  $1 Sector debug - backups and cleanups complete @" $SECONDS "seconds.   "

# Provide statistics
#echo "  .  $1 containing" $NUM_WORLDS" worlds ("$2"%) took" $SECONDS "seconds to generate.   "

### And Serve

# CGI for presenting a pdf
echo "Content-type: application/pdf" 
echo "Filename: "$1.pdf
echo "" 
cat < /sectors/tmp/$1.pdf


#Clean up all traces
rm -rf /sectors/tmp/*$1*
rm -rf /sectors/tmp/*.txt
rm -rf /sectors/tmp/*anim*
rm -rf /sectors/tmp/*cargo*
rm -rf /sectors/tmp/*polity*
