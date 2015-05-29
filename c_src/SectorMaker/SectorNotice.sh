
# SectorMaker.sh
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
# ## # sudo mkdir $OUTPUT_DIR
# ## # sudo mkdir /sectors/tmp
# ## # sudo chmod a=rwx $OUTPUT_DIR
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
#     zero = 0% (Will not randomly generate worlds, you must have a sectorname_names.txt file in $OUTPUT_DIR for the world locations and names to use)
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
#    Pre-existing sector file name. Used if an existing set of files is available in $OUTPUT_DIR
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

