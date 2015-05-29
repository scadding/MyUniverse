#!/bin/bash

# the first parameter is the number of sectors to generate

PWD=`pwd`
BSM_TMP_DIR=$PWD/tmp
TRAV_DIR=$PWD/trav

function worldroller()
{
	
	while read LINE
	do
		DENSITY=$(( $RANDOM % 50 + 50 ))
		echo " Generating Sector \"$LINE\""
		echo " Sector Density \"$DENSITY\""
		./SectorMaker.sh $LINE $DENSITY $2 $3 $4
		wait
	done < $1
}


#Create Sector Names

#run the name generator
echo " "
#echo perl $TRAV_DIR/lc -$1 $TRAV_DIR/namesmaster.txt  $BSM_TMP_DIR/sectornamesA
perl $TRAV_DIR/lc -$1 $TRAV_DIR/namesmaster.txt > $BSM_TMP_DIR/sectornamesA
sed 's/$/_Sector/g' $BSM_TMP_DIR/sectornamesA > $BSM_TMP_DIR/sectornamesB
rm $BSM_TMP_DIR/sectornamesA
sed 's/ _Sector/_Sector/g' $BSM_TMP_DIR/sectornamesB > $BSM_TMP_DIR/sectornames
rm $BSM_TMP_DIR/sectornamesB
worldroller $BSM_TMP_DIR/sectornames $2 $3 $4
rm $BSM_TMP_DIR/sectornames

echo "  o . . . . . . . . . . . . . . . . . . . . . . . . ."
echo "  .  Master sector generation process complete. "
echo "  .  Generation of "$1" sectors took" $SECONDS "seconds."
echo "  o . . . . . . . . . . . . . . . . . . . . . . . . ."


