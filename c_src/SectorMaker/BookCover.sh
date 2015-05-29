#!/bin/bash

BOOK_NAME=$1
OUTFILE_NAME=$2

PWD=`pwd`
TMP_DIR=$PWD/tmp
TRAV_DIR=$PWD/trav

mkdir -p $TMP_DIR

## Generate the custom cover 

# Generate the text of the sector name
echo $BOOK_NAME > $TMP_DIR/seccoverA.txt
sed '/_/ s/_/ /g' $TMP_DIR/seccoverA.txt > $TMP_DIR/seccover.txt

# Add a line to the sectorname for processing
cat $TRAV_DIR/covercat.txt $TMP_DIR/seccover.txt > $TMP_DIR/seccover2.txt

# LBB cover
enscript --word-wrap $TMP_DIR/seccover2.txt -r --margins=266:-300:-400:407 -B -f OptimaItalic32 --media=Letter -q -p $TMP_DIR/seccover1.ps

# Turn the generated text white for the cover
sed '/5 578 M/ s/5 578 M/5 578 M 1 setgray/g' $TMP_DIR/seccover1.ps > $TMP_DIR/seccover.ps 

# Make the generated text of the cover a pdf
ps2pdfwr $TMP_DIR/seccover.ps $TMP_DIR/seccover1.pdf

# Combine the generated name and the background front cover pdf into the new sector front cover.
pdftk $TMP_DIR/seccover1.pdf background $TRAV_DIR/SectorData_Cover.pdf output $TMP_DIR/CoverInt.pdf
wait

#echo "  .  $1 Sector debug - cover complete @" $SECONDS "seconds. "

## Generate the back page

# Random Quote Grabber
./trav/randomquote.sh > $TMP_DIR/backquote.txt

# LBB Back
enscript $TMP_DIR/backquote.txt -r --word-wrap --margins=122:-100:364:40 -B -f OptimaItalic20 --media=Letter -q -p $TMP_DIR/Back1.ps

# Make the generated text of the back cover a pdf
sed '/5 567 M/ s/5 567 M/5 567 M 1 setgray/g' $TMP_DIR/Back1.ps > $TMP_DIR/CoverBack.ps 

# Make the generated text of the back cover a pdf
ps2pdfwr $TMP_DIR/CoverBack.ps $TMP_DIR/BackCovertemp.pdf

# Combine the generated text and the background back cover pdf into the new sector back cover.
pdftk $TMP_DIR/BackCovertemp.pdf background $TMP_DIR/CoverInt.pdf output $OUTFILE_NAME
wait

#echo "  .  $1 Sector debug - back cover complete @" $SECONDS "seconds. "
#echo "1:$1 2:$2 3:$3 4:$4 5:$5 6:$6 "

rm -f $TMP_DIR/seccover1.pdf $TMP_DIR/CoverInt.pdf
rm -f $TMP_DIR/Back1.ps $TMP_DIR/CoverBack.ps $TMP_DIR/seccover1.pdf $TMP_DIR/seccoverA.txt
rm -f $TMP_DIR/BackCovertemp.pdf $TMP_DIR/CoverInt.pdf $TMP_DIR/seccover1.ps $TMP_DIR/seccover.ps
rm -f $TMP_DIR/backquote.txt $TMP_DIR/seccover2.txt $TMP_DIR/seccover.txt

