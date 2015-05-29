#!/bin/bash

## Generate the raw subsector data or import it

if [ "$IMPORTSEC" != "" ]; then 
	cp $OUTPUT_DIR/$IMPORTSEC $TMP_DIR/${SECTOR_NAME}secdata
	sed q $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/secdataorighead
	sed '1d' $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/secdataorig
	NUM_WORLDS=`wc -l $TMP_DIR/secdataorig | awk '{ print $1 }'`

else 
	# Modified gensec4 call, allows for using the $1_names.txt file
	#echo gensec4 -d $DENSITY -m $MATURITY -s="$SECTOR_NAME" -p $OUTPUT_DIR/ -o 5 -u $TMP_DIR/secdataorig2
	gensec4 -d $DENSITY -m $MATURITY -s="$SECTOR_NAME" -p $OUTPUT_DIR/ -o 5 -u $TMP_DIR/secdataorig2

	# Save the first line of the sector file ('Version' tag)
	sed q $TMP_DIR/secdataorig2 > $TMP_DIR/secdataorighead

	# Remove the first line for the text processing
	sed '1d' $TMP_DIR/secdataorig2 > $TMP_DIR/secdataorig
	rm $TMP_DIR/secdataorig2

	#echo "  .  $1 Sector debug - data generation complete @" $SECONDS "seconds.   "

	## Generate the sector's system names

	# Count the number of worlds generated
	NUM_WORLDS=`wc -l $TMP_DIR/secdataorig | awk '{ print $1 }'`

	# Grab the generated datafile's contents preceding and following the name
	cut -b20-80 $TMP_DIR/secdataorig > $TMP_DIR/secdataorigBack
	cut -b1-19 $TMP_DIR/secdataorig > $TMP_DIR/secdataorigNames

	# Run the random word generator
	perl $TRAV_DIR/lc -$NUM_WORLDS $TRAV_DIR/namesmaster.txt > $TMP_DIR/${SECTOR_NAME}namegrab1

	# Grab the names from the file
	sed -e :a -e 's/^.\{1,18\}$/& /;ta' $TMP_DIR/${SECTOR_NAME}namegrab1 > $TMP_DIR/${SECTOR_NAME}namegrab2

	# Added these three lines for the $1_names.txt file to format the names properly
	# Compare the randomly generated names to the predefined names and only use generated names to replace "Unnamed"
	paste -d" " $TMP_DIR/secdataorigNames $TMP_DIR/${SECTOR_NAME}namegrab2| awk '{if ($1=="Unnamed"){print $2}else{print $1}}' > $TMP_DIR/${SECTOR_NAME}namegrab3
	sed 's/$/                   /' $TMP_DIR/${SECTOR_NAME}namegrab3 > $TMP_DIR/${SECTOR_NAME}namegrab4
	cut -b1-19 $TMP_DIR/${SECTOR_NAME}namegrab4 > $TMP_DIR/${SECTOR_NAME}namegrab5

	# Combine the three files into a new secdata file with the names inside
	paste -d '\0' $TMP_DIR/${SECTOR_NAME}namegrab5 $TMP_DIR/secdataorigBack > $TMP_DIR/${SECTOR_NAME}secdatainterim

	# Paste the header back onto the sector data
	cat $TMP_DIR/secdataorighead $TMP_DIR/${SECTOR_NAME}secdatainterim > $TMP_DIR/${SECTOR_NAME}secdata
fi



