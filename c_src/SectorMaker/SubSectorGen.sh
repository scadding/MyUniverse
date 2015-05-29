#!/bin/bash

let "whichborderpage = whichborderpage +1"
# Generate this subsector's data file
#echo " Generate this subsector's data file "
#echo subsec2 S=$x  $TMP_DIR/${SECTOR_NAME}secdata  $TMP_DIR/${SECTOR_NAME}subsec1$x
subsec2 S=$x < $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/${SECTOR_NAME}subsec1$x
NUM_SUBSECWORLDS=`wc -l $TMP_DIR/${SECTOR_NAME}subsec1$x | awk '{ print $1 }'`

#this is the line that is messing up with underscores
#sed '/_/ s/_/\\ /g' $TMP_DIR/${SECTOR_NAME}subsec1$x > $TMP_DIR/${SECTOR_NAME}subsec$x
#this is the test for fixing this, which seems to work
sed '/_/ s/_/ /g' $TMP_DIR/${SECTOR_NAME}subsec1$x > $TMP_DIR/${SECTOR_NAME}subsec$x
wait

# Create a duplicate
#echo " Create duplicate "
sed '' $TMP_DIR/${SECTOR_NAME}subsec$x > $TMP_DIR/${SECTOR_NAME}subsecdata$x

# Reorder the ID on the datafile for output only
#echo " Reorder ID "
cut -b20-24 $TMP_DIR/${SECTOR_NAME}subsecdata$x > $TMP_DIR/${SECTOR_NAME}secdatareorderID
cut -b1-19 $TMP_DIR/${SECTOR_NAME}subsecdata$x > $TMP_DIR/${SECTOR_NAME}secdatareorderName1
sed '/_/ s/_/ /g' $TMP_DIR/${SECTOR_NAME}secdatareorderName1 > $TMP_DIR/${SECTOR_NAME}secdatareorderName
cut -b25-81 $TMP_DIR/${SECTOR_NAME}subsecdata$x > $TMP_DIR/${SECTOR_NAME}subsecend$x
#echo " End reorder ID "

## EXPERIMENTAL: Find the capital
## Scoring loop

## Process:
# 1. see if there is a line in the msec file with a lowercase version of the subsector letter and a space at the beginning.
# 2. if so, use that line.
# 3. if not, enable the scoring loop.
# 4. In the scoring loop, if there is a world in the subsector with 'Ca', that's the subsector capital and therefore the name.
# 5. If not, determine the capital and modify the world's line in the .sec file.
#
# If the msec file has a line containing the lowercase variant of the subsector letter, followed by a space, use the rest of the line as the subsector name.
SCORING=1
if [ $SCORING ]; then
	if [ $NUM_SUBSECWORLDS -gt 0 ]; then
		echo " This subsector ( "$x" ) has "$NUM_SUBSECWORLDS" worlds."

		scorer ${SECTOR_NAME}
		wait

		# Tally up the scores
		#echo " Tally up the scores "
		tally $x ${SECTOR_NAME} ${SECTOR_NAME}subsecdataScoreFinal$x > $TMP_DIR/${SECTOR_NAME}subsecdataTally$x
		#echo " Done tallying "

		# Include the names
		#echo " Include the names "
		paste -d '\0' $TMP_DIR/${SECTOR_NAME}subsecdataFinalTally$x $TMP_DIR/${SECTOR_NAME}subsecdataScoreSpace$x $TMP_DIR/${SECTOR_NAME}subsecdataScoreName$x > $TMP_DIR/${SECTOR_NAME}subsecdataScorerSheet$x
		wait
		rm $TMP_DIR/${SECTOR_NAME}subsecdataFinalTally$x $TMP_DIR/${SECTOR_NAME}subsecdataScoreSpace$x $TMP_DIR/${SECTOR_NAME}subsecdataScoreName$x

		# Sort subsector worlds by tally
		#echo " Sort subsector worlds "
		sort -n -r $TMP_DIR/${SECTOR_NAME}subsecdataScorerSheet$x > $TMP_DIR/${SECTOR_NAME}subsecdataScorerSheetSorted$x
		sed q $TMP_DIR/${SECTOR_NAME}subsecdataScorerSheetSorted$x > $TMP_DIR/${SECTOR_NAME}subsecdataScorerSheetCapital$x

		# Run the main loop
		#echo "Run the main loop "
		grabCapital ${SECTOR_NAME}

		# Create Subsector subtitle
		echo $CAPITALNAME "Subsector" > $TMP_DIR/${SECTOR_NAME}subsectitle$x.txt

		# Put the heading on top of this subsector's data file (for printing)
		paste -d '\0' $TMP_DIR/${SECTOR_NAME}secdatareorderID $TMP_DIR/${SECTOR_NAME}secdatareorderName $TMP_DIR/${SECTOR_NAME}subsecend$x > $TMP_DIR/${SECTOR_NAME}subsecReorder$x
		cat -u $TMP_DIR/${SECTOR_NAME}title.txt $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}subsectitle$x.txt $TRAV_DIR/subtitles/subtitle$x.txt $TRAV_DIR/beginfile.txt $TMP_DIR/${SECTOR_NAME}subsecReorder$x $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}subsecdataSum$x > $TMP_DIR/${SECTOR_NAME}subsecOut$x
		# cat -u $TRAV_DIR/introspace.txt $TMP_DIR/${SECTOR_NAME}title.txt $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}subsectitle$x.txt $TRAV_DIR/subtitles/subtitle$x.txt $TRAV_DIR/beginfile.txt $TMP_DIR/${SECTOR_NAME}subsecReorder$x $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}subsecdataSum$x > $TMP_DIR/${SECTOR_NAME}subsecOut$x
	else

		# Compile 'em without subsector scoring
		paste -d '\0' $TMP_DIR/${SECTOR_NAME}secdatareorderID $TMP_DIR/${SECTOR_NAME}secdatareorderName $TMP_DIR/${SECTOR_NAME}subsecend$x > $TMP_DIR/${SECTOR_NAME}subsecReorder$x
		cat -u $TMP_DIR/${SECTOR_NAME}title.txt $TRAV_DIR/subtitles/subtitle$x.txt $TRAV_DIR/beginfile.txt $TMP_DIR/${SECTOR_NAME}subsecReorder$x $TRAV_DIR/ret.txt > $TMP_DIR/${SECTOR_NAME}subsecOut$x
		#cat -u $TRAV_DIR/introspace.txt $TMP_DIR/${SECTOR_NAME}title.txt $TRAV_DIR/subtitles/subtitle$x.txt $TRAV_DIR/beginfile.txt $TMP_DIR/${SECTOR_NAME}subsecReorder$x $TRAV_DIR/ret.txt > $TMP_DIR/${SECTOR_NAME}subsecOut$x
		wait
	fi
fi

# Change the version number to trip the logic of the mapper
sed '/3/ s/3/2/g' $TMP_DIR/secdataorighead > $TMP_DIR/secdataorighead2
wait

# Put the header on each subsector file
cat $TMP_DIR/secdataorighead2 $TMP_DIR/${SECTOR_NAME}subsec$x > $TMP_DIR/${SECTOR_NAME}subsecCalc$x
wait
#rm $TMP_DIR/${SECTOR_NAME}subsec$x

# Map each subsector's data
mapsub3 -p < $TMP_DIR/${SECTOR_NAME}subsecCalc$x > $TMP_DIR/${SECTOR_NAME}subsec$x.ps
#rm $TMP_DIR/${SECTOR_NAME}subsecCalc$x

# Create .pdf file from this subsectors map's .ps file
ps2pdfwr $TMP_DIR/${SECTOR_NAME}subsec$x.ps $TMP_DIR/${SECTOR_NAME}subsecinter$x.pdf
wait
#rm $TMP_DIR/${SECTOR_NAME}subsec$x.ps

##############################################################################
# SubSector Data
##############################################################################

# Create .ps file from this subsectors data's file
enscript $TMP_DIR/${SECTOR_NAME}subsecOut$x --word-wrap --margins=40:20:60:40 -B -f Courier7.5 --media=Traveller -q -p $TMP_DIR/${SECTOR_NAME}subsecdata$x.ps
wait
# rm $TMP_DIR/${SECTOR_NAME}subsecOut$x

# Create .pdf file from this subsector data's .ps file
ps2pdfwr $TMP_DIR/${SECTOR_NAME}subsecdata$x.ps $TMP_DIR/${SECTOR_NAME}subsecdatainter$x.pdf
wait
# rm $TMP_DIR/${SECTOR_NAME}subsecdata$x.ps

# Place the generated data on a background with header and page number
pdftk $TMP_DIR/${SECTOR_NAME}subsecdatainter$x.pdf background $TRAV_DIR/backgrounds/Data_Background$x.pdf output $TMP_DIR/${SECTOR_NAME}subsecdata$x.pdf
wait
# rm $TMP_DIR/${SECTOR_NAME}subsecdatainter$x.pdf

##############################################################################
# Borders
##############################################################################

# Extract the proper subsector borders page
pdftk $TMP_DIR/${SECTOR_NAME}_subsectorborders.pdf cat $whichborderpage output $TMP_DIR/${SECTOR_NAME}subsecborders$x.pdf
wait

# Combine the borders and the previous pdf
pdftk $TMP_DIR/${SECTOR_NAME}subsecinter$x.pdf background $TMP_DIR/${SECTOR_NAME}subsecborders$x.pdf output $TMP_DIR/${SECTOR_NAME}subsecinter2$x.pdf
wait
rm $TMP_DIR/${SECTOR_NAME}subsecinter$x.pdf $TMP_DIR/${SECTOR_NAME}subsecborders$x.pdf

# Place the generated map on a background with header and page number
pdftk $TRAV_DIR/backgrounds/Map_Background$x.pdf background $TMP_DIR/${SECTOR_NAME}subsecinter2$x.pdf output $TMP_DIR/${SECTOR_NAME}subsec$x.pdf
wait
#rm $TMP_DIR/${SECTOR_NAME}subsecinter2$x.pdf

#echo "  .  ${SECTOR_NAME} Sector debug - " $x "subsector data / map parsing complete @" $SECONDS "seconds.   "


