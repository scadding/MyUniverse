



## If we're using a pre-existing sector file, don't change the alliances.

if [ "$5" != "" ]; then 

	# Run the map with old alliances retained
	get_Sectormap ${SECTOR_NAME} 2 $6
	wait
	sed -n '/route/p' $TMP_DIR/${SECTOR_NAME}_automsec.msec > $TMP_DIR/${SECTOR_NAME}_route.txt

else
	# Run the map with new alliances
	get_Sectormap ${SECTOR_NAME}
	wait

	#echo "  .  ${SECTOR_NAME} Sector debug - sector map generation complete @" $SECONDS "seconds.   "


	## If it's a fresh sector, go ahead and generate the names of the Alliances

	# Grab a version of the msec file without 'border'
	sed '/# Alliance: / s/# Alliance: / /g' $TMP_DIR/${SECTOR_NAME}_automsec.msec > $TMP_DIR/${SECTOR_NAME}_before.msec

	#sed '/# Alliance: /d' $TMP_DIR/${SECTOR_NAME}_automsec.msec > $TMP_DIR/${SECTOR_NAME}_beforeZ.msec
	sed '/border/d' $TMP_DIR/${SECTOR_NAME}_automsec.msec > $TMP_DIR/${SECTOR_NAME}_before1a.msec
	sed -n '/route/p' $TMP_DIR/${SECTOR_NAME}_before1a.msec > $TMP_DIR/${SECTOR_NAME}_route.txt

	sed '/route/d' $TMP_DIR/${SECTOR_NAME}_before.msec > $TMP_DIR/${SECTOR_NAME}_before1.msec
	sed '/border/d' $TMP_DIR/${SECTOR_NAME}_before1.msec > $TMP_DIR/${SECTOR_NAME}_after1.msec

	sed -n 'G; s/\n/&&/; /^\([ -~]*\n\).*\n\1/d; s/\n//; h; P' $TMP_DIR/${SECTOR_NAME}_after1.msec > $TMP_DIR/${SECTOR_NAME}_after.msec
	# Count the alliances in the file
	NUM_POLS=`wc -l $TMP_DIR/${SECTOR_NAME}_after.msec | awk '{ print $1 }'`

	# Generate the right number of random names
	perl $TRAV_DIR/lc -$NUM_POLS $TRAV_DIR/namesmaster.txt > $TMP_DIR/politynames

	# Put the codes with the random names and run the polity combiner
	paste -d '\0' $TMP_DIR/politynames $TMP_DIR/${SECTOR_NAME}_after.msec > $TMP_DIR/${SECTOR_NAME}_combo1.msec
	sed '/  / s/  / /g' $TMP_DIR/${SECTOR_NAME}_combo1.msec > $TMP_DIR/${SECTOR_NAME}_combo.msec
	#echo " Call get_Polities: ${SECTOR_NAME}_combo "
	get_Polities ${SECTOR_NAME}_combo.msec

	# Put all the polity names together as the final 'alliances' text
	#echo " Put polity names together "
	cat $TMP_DIR/${SECTOR_NAME}_combo.msecpolity*.txt > "$TMP_DIR/${SECTOR_NAME}_polities.txt"
	#echo " Copy polities to alliances "
	cp $TMP_DIR/${SECTOR_NAME}_polities.txt $TMP_DIR/${SECTOR_NAME}_alliances.txt
	wait

	#echo "  .  ${SECTOR_NAME} Sector debug - alliance generation complete @" $SECONDS "seconds.   "
fi

# NOTE: Need to add code to grab any spaces INSIDE the names and turn them into underscores. Right now it's manual.

## Grab the alliances (pre-existing or not) and parse them into the secdata file

# Save the first line of the sector file ('Version' tag)
#echo " Save the first line of the sector file "
sed q $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/secdataorighead2

# Remove the first line for the text processing
#echo " Remove the first line for the text processing "
if [ "$5" != "" ]; then 
	cp $OUTPUT_DIR/${SECTOR_NAME}_alliances.txt $TMP_DIR/${SECTOR_NAME}secdatadesc_alliances.txt
	wait
	cp $OUTPUT_DIR/${SECTOR_NAME}_alliances.txt $TMP_DIR/${SECTOR_NAME}_alliances.txt
	wait
	cp $OUTPUT_DIR/${SECTOR_NAME}_alliances.txt $TMP_DIR/${SECTOR_NAME}_polities.txt
	wait
	cp $TMP_DIR/${SECTOR_NAME}secdata $TMP_DIR/${SECTOR_NAME}_automsec.sec
	wait
else
	sed '1,15d' $TMP_DIR/${SECTOR_NAME}_automsec.sec > $TMP_DIR/${SECTOR_NAME}_newsecdataProc
	sed '1d' $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/${SECTOR_NAME}secdataProc

	# Splice the new secdata file
	#echo " Splice the new secdata file "
	cut -b1-56 $TMP_DIR/${SECTOR_NAME}secdataProc > $TMP_DIR/${SECTOR_NAME}_secdataEmpirePre
	cut -b60-64 $TMP_DIR/${SECTOR_NAME}secdataProc > $TMP_DIR/${SECTOR_NAME}_secdataEmpirePost
	cut -b56-58 $TMP_DIR/${SECTOR_NAME}_newsecdataProc > $TMP_DIR/${SECTOR_NAME}_secdataNewEmpire

	# Put it all together
	#echo " Put it all together "
	paste -d '\0' $TMP_DIR/${SECTOR_NAME}_secdataEmpirePre $TMP_DIR/${SECTOR_NAME}_secdataNewEmpire $TMP_DIR/${SECTOR_NAME}_secdataEmpirePost > $TMP_DIR/${SECTOR_NAME}secdatainterim2
	cat $TMP_DIR/secdataorighead2 $TMP_DIR/${SECTOR_NAME}secdatainterim2 > $TMP_DIR/${SECTOR_NAME}secdata

	# safety kludge for literal problem
	#echo " Safety kludge for literal problem "
	cp $TMP_DIR/${SECTOR_NAME}_alliances.txt $TMP_DIR/${SECTOR_NAME}secdatadesc_alliances.txt
	wait

	#echo "  .  ${SECTOR_NAME} Sector debug - alliance parsing complete @" $SECONDS "seconds.   "
fi



