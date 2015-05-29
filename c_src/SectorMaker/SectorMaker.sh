#!/bin/bash

# no need to include, all copyright notices in this file.
#. ./SectorNotice.sh

#### First, here are the get_UPP, ?(random_Quote)? and parse_SectorDescription functions.

if [ -z $1 ]
then
    echo "Usage: `basename $0` (system name) (density) (maturity)"
	SECTOR_NAME="junk"
else
    SECTOR_NAME=$1
fi

if [ -z $2 ]
then
    echo "Usage: `basename $0` (system name) (density) (maturity)"
	DENSITY=50
else
    DENSITY=$2
fi

if [ -z $3 ]
then
	MATURITY=""
else
    MATURITY=$3
fi

if [ -z $4 ]
then
	DETAIL=0
else
    DETAIL=$4
fi

if [ -z $5 ]
then
	IMPORTSEC=""
else
    IMPORTSEC=$5
fi

PWD=`pwd`
TMP_DIR=$PWD/tmp/${SECTOR_NAME}
TRAV_DIR=$PWD/trav
OUTPUT_DIR=$PWD/output

mkdir -p $TMP_DIR

. ./SectorFunctions.sh

#### Now the main body of the script.


### 1. GENERATE THE COVERS

./BookCover.sh $SECTOR_NAME $TMP_DIR/Cover.pdf

### 2. GENERATE OR GRAB THE SECTOR'S DATA

. ./SectorGen.sh

### 4. GENERATE THE SECTOR MAP

. ./SectorGenMap.sh

### GENERATE THE SUBSECTOR MAPS AND DATA PAGES

#Create Title Text for Listings and Descriptions
#echo " Create Title Text for Listings "
echo ""${SECTOR_NAME}"""" > "$TMP_DIR/${SECTOR_NAME}titleA.txt"
sed '/_/ s/_/\\ /g' "$TMP_DIR/${SECTOR_NAME}titleA.txt" > "$TMP_DIR/${SECTOR_NAME}title.txt"

# Create Title Text for Sector Map
#echo " Create Title Text for Sector Map "
echo "Contains" $NUM_WORLDS" systems." > "$TMP_DIR/${SECTOR_NAME}SectorMapsubtitle.txt"
#echo ""${SECTOR_NAME}""" Sector (containing" $NUM_WORLDS" systems)" > $TMP_DIR/${SECTOR_NAME}SectorMaptitleA.txt
#sed '/_/ s/_/ /g' $TMP_DIR/${SECTOR_NAME}SectorMaptitleA.txt > $TMP_DIR/${SECTOR_NAME}SectorMaptitle.txt

#cat $TMP_DIR/${SECTOR_NAME}title.txt $TMP_DIR/${SECTOR_NAME}SectorMapsubtitle.txt > $TMP_DIR/${SECTOR_NAME}SectorMaptitle.txt
cat $TMP_DIR/${SECTOR_NAME}title.txt $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}SectorMapsubtitle.txt > $TMP_DIR/${SECTOR_NAME}SectorMaptitle.txt

## Big Per-Subsector Loop Starts
#echo " Big Per-Subsector Loop Starts "
whichborderpage=0
for x in A B C D E F G H I J K L M N O P; do
	. ./SubSectorGen.sh
done

# Then recombine subsectors into new secdatafile


### Assemble final .pdf containing cover and each subsector's map and data

# Sectormap Page

# Combine text for Sectormap
cat $TRAV_DIR/ret.txt $TRAV_DIR/ret.txt $TRAV_DIR/ret.txt $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}SectorMaptitle.txt > $TMP_DIR/${SECTOR_NAME}Maptitle.txt 

# Grab the Sector's title for the map page
enscript --word-wrap --margins=35:-100:0:40 --fancy-header='travellerpagenums' --header='' --header-font=Univers9 $TMP_DIR/${SECTOR_NAME}Maptitle.txt -f Univers8 --media=Traveller -q -p $TMP_DIR/${SECTOR_NAME}_SectorMapSectorTitle.ps
wait

# Make the generated text of the back cover a pdf
ps2pdfwr $TMP_DIR/${SECTOR_NAME}_SectorMapSectorTitle.ps $TMP_DIR/${SECTOR_NAME}_SectorMapSectorTitle.pdf

#place the map on a Classic Traveller background
pdftk $TMP_DIR/${SECTOR_NAME}_SectorMapSectorTitle.pdf background $TMP_DIR/${SECTOR_NAME}_sectormap2.pdf output $TMP_DIR/${SECTOR_NAME}_sectormap.pdf
wait


# DEBUG - echo "  .  ${SECTOR_NAME} Sector debug - sector map generation complete @" $SECONDS "seconds.   "



# Polities Page

# combine text
cat $TRAV_DIR/ret.txt $TRAV_DIR/ret.txt $TRAV_DIR/ret.txt $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}title.txt $TRAV_DIR/ret.txt $TMP_DIR/${SECTOR_NAME}_alliances.txt > $TMP_DIR/${SECTOR_NAME}_politiespage.txt
wait

#turn to ps file
enscript --word-wrap --margins=35:-100:0:40 --fancy-header='travellerpagenums' --header='' --header-font=Univers9 $TMP_DIR/${SECTOR_NAME}_politiespage.txt -f Univers8 --media=Traveller -q -p $TMP_DIR/${SECTOR_NAME}_politiespageout.ps
wait

# turn to pdf
ps2pdfwr $TMP_DIR/${SECTOR_NAME}_politiespageout.ps $TMP_DIR/${SECTOR_NAME}_politiespage1.pdf
wait

# join with background (header, page nums)
pdftk $TMP_DIR/${SECTOR_NAME}_politiespage1.pdf background $TRAV_DIR/backgrounds/SectorPolities_Background.pdf output $TMP_DIR/${SECTOR_NAME}_politiespage.pdf
wait

#echo "  .  ${SECTOR_NAME} Sector debug - polities generation complete @" $SECONDS "seconds.   "



## Decide - descriptions or no?
# Evaluate whether the fourth argument is higher than 1 (to bypass descriptions)

needfordesc=3

let "DETAIL = DETAIL + 2"

if [ "$needfordesc" -lt "$DETAIL" ]; then 	# -n tests to see if the argument is non empty
	## BRING IT TOGETHER WITHOUT DESCRIPTIONS
	# Bring it together with the right TOC (no 'world descriptions' line)
	pdftk $TMP_DIR/Cover.pdf $TRAV_DIR/SectorData_IntNoDesc.pdf $TMP_DIR/${SECTOR_NAME}subsecdataA.pdf $TMP_DIR/${SECTOR_NAME}subsecA.pdf $TMP_DIR/${SECTOR_NAME}subsecdataB.pdf $TMP_DIR/${SECTOR_NAME}subsecB.pdf $TMP_DIR/${SECTOR_NAME}subsecdataC.pdf $TMP_DIR/${SECTOR_NAME}subsecC.pdf $TMP_DIR/${SECTOR_NAME}subsecdataD.pdf $TMP_DIR/${SECTOR_NAME}subsecD.pdf $TMP_DIR/${SECTOR_NAME}subsecdataE.pdf $TMP_DIR/${SECTOR_NAME}subsecE.pdf $TMP_DIR/${SECTOR_NAME}subsecdataF.pdf $TMP_DIR/${SECTOR_NAME}subsecF.pdf $TMP_DIR/${SECTOR_NAME}subsecdataG.pdf $TMP_DIR/${SECTOR_NAME}subsecG.pdf $TMP_DIR/${SECTOR_NAME}subsecdataH.pdf $TMP_DIR/${SECTOR_NAME}subsecH.pdf $TMP_DIR/${SECTOR_NAME}subsecdataI.pdf $TMP_DIR/${SECTOR_NAME}subsecI.pdf $TMP_DIR/${SECTOR_NAME}subsecdataJ.pdf $TMP_DIR/${SECTOR_NAME}subsecJ.pdf $TMP_DIR/${SECTOR_NAME}subsecdataK.pdf $TMP_DIR/${SECTOR_NAME}subsecK.pdf $TMP_DIR/${SECTOR_NAME}subsecdataL.pdf $TMP_DIR/${SECTOR_NAME}subsecL.pdf $TMP_DIR/${SECTOR_NAME}subsecdataM.pdf $TMP_DIR/${SECTOR_NAME}subsecM.pdf $TMP_DIR/${SECTOR_NAME}subsecdataN.pdf $TMP_DIR/${SECTOR_NAME}subsecN.pdf $TMP_DIR/${SECTOR_NAME}subsecdataO.pdf $TMP_DIR/${SECTOR_NAME}subsecO.pdf $TMP_DIR/${SECTOR_NAME}subsecdataP.pdf $TMP_DIR/${SECTOR_NAME}subsecP.pdf cat output $TMP_DIR/${SECTOR_NAME}pre.pdf
	wait
	#echo "  .  ${SECTOR_NAME} Sector debug - initial pdf document combination (without descs) complete @" $SECONDS "seconds.   "

	# Combine the subsector pages into the final pdf and compress
	#pdftk $TMP_DIR/${SECTOR_NAME}pre.pdf $TMP_DIR/${SECTOR_NAME}Back.pdf cat output $TMP_DIR/${SECTOR_NAME}.pdf compress
	pdftk $TMP_DIR/${SECTOR_NAME}pre.pdf $TMP_DIR/${SECTOR_NAME}_sectormap.pdf $TMP_DIR/${SECTOR_NAME}_politiespage.pdf cat output $TMP_DIR/${SECTOR_NAME}.pdf compress
	wait

	# Back up the relevant files in the survey archives
	mv $TMP_DIR/${SECTOR_NAME}.pdf $OUTPUT_DIR/${SECTOR_NAME}.pdf
	mv $TMP_DIR/${SECTOR_NAME}secdata $OUTPUT_DIR/${SECTOR_NAME}.sec
	wait
else 
	## PARSE THE UPP'S OF EACH WORLD INTO TEXTUAL DESCRIPTIONS
	# Cut and recombine the ID, UPP, NAME and Polity fields into a new list
	cut -b20-24 $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/${SECTOR_NAME}secdatadescreformID
	cut -b25-34 $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/${SECTOR_NAME}secdatadescreformSpaceUPP
	cut -b1-18 $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/${SECTOR_NAME}secdatadescreformName
	cut -b57-59 $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/${SECTOR_NAME}secdatadescreformEmpire

	paste -d '\0' $TMP_DIR/${SECTOR_NAME}secdatadescreformID $TMP_DIR/${SECTOR_NAME}secdatadescreformSpaceUPP $TMP_DIR/${SECTOR_NAME}secdatadescreformEmpire $TMP_DIR/${SECTOR_NAME}secdatadescreformName > $TMP_DIR/${SECTOR_NAME}secdatareform
	#sed -e 's/ /\+/' $TMP_DIR/${SECTOR_NAME}secdatareform > $TMP_DIR/${SECTOR_NAME}secdatareform2
	#sed -e 's/-//' $TMP_DIR/${SECTOR_NAME}secdatareform2 > $TMP_DIR/${SECTOR_NAME}secdatareform3
	#sed -e 's/-//' $TMP_DIR/${SECTOR_NAME}secdatareform > $TMP_DIR/${SECTOR_NAME}secdatareform4
	#sed -e 's/ /\+/' $TMP_DIR/${SECTOR_NAME}secdatareform3 > $TMP_DIR/${SECTOR_NAME}secdataform4
	#sed '1d' $TMP_DIR/${SECTOR_NAME}secdataform4 > $TMP_DIR/${SECTOR_NAME}secdatadesc
	sed '1d' $TMP_DIR/${SECTOR_NAME}secdatareform > $TMP_DIR/${SECTOR_NAME}secdatadesc

	# Run the Descriptions generator

	get_SectorDescriptions ${SECTOR_NAME}secdatadesc ${SECTOR_NAME}
	wait

	## BRING IT TOGETHER WITH TEXTUAL DESCRIPTIONS

	# Bring it together with the right TOC (w/ 'world descriptions' line)
	pdftk $TMP_DIR/Cover.pdf $TRAV_DIR/SectorData_IntDesc.pdf $TMP_DIR/${SECTOR_NAME}subsecdataA.pdf $TMP_DIR/${SECTOR_NAME}subsecA.pdf $TMP_DIR/${SECTOR_NAME}subsecdataB.pdf $TMP_DIR/${SECTOR_NAME}subsecB.pdf $TMP_DIR/${SECTOR_NAME}subsecdataC.pdf $TMP_DIR/${SECTOR_NAME}subsecC.pdf $TMP_DIR/${SECTOR_NAME}subsecdataD.pdf $TMP_DIR/${SECTOR_NAME}subsecD.pdf $TMP_DIR/${SECTOR_NAME}subsecdataE.pdf $TMP_DIR/${SECTOR_NAME}subsecE.pdf $TMP_DIR/${SECTOR_NAME}subsecdataF.pdf $TMP_DIR/${SECTOR_NAME}subsecF.pdf $TMP_DIR/${SECTOR_NAME}subsecdataG.pdf $TMP_DIR/${SECTOR_NAME}subsecG.pdf $TMP_DIR/${SECTOR_NAME}subsecdataH.pdf $TMP_DIR/${SECTOR_NAME}subsecH.pdf $TMP_DIR/${SECTOR_NAME}subsecdataI.pdf $TMP_DIR/${SECTOR_NAME}subsecI.pdf $TMP_DIR/${SECTOR_NAME}subsecdataJ.pdf $TMP_DIR/${SECTOR_NAME}subsecJ.pdf $TMP_DIR/${SECTOR_NAME}subsecdataK.pdf $TMP_DIR/${SECTOR_NAME}subsecK.pdf $TMP_DIR/${SECTOR_NAME}subsecdataL.pdf $TMP_DIR/${SECTOR_NAME}subsecL.pdf $TMP_DIR/${SECTOR_NAME}subsecdataM.pdf $TMP_DIR/${SECTOR_NAME}subsecM.pdf $TMP_DIR/${SECTOR_NAME}subsecdataN.pdf $TMP_DIR/${SECTOR_NAME}subsecN.pdf $TMP_DIR/${SECTOR_NAME}subsecdataO.pdf $TMP_DIR/${SECTOR_NAME}subsecO.pdf $TMP_DIR/${SECTOR_NAME}subsecdataP.pdf $TMP_DIR/${SECTOR_NAME}subsecP.pdf cat output $TMP_DIR/${SECTOR_NAME}pre.pdf
	wait

	pdftk $TMP_DIR/${SECTOR_NAME}pre.pdf $TMP_DIR/${SECTOR_NAME}_sectormap.pdf $TMP_DIR/${SECTOR_NAME}_politiespage.pdf $TMP_DIR/${SECTOR_NAME}secdatadescpiece.pdf cat output $TMP_DIR/${SECTOR_NAME}.pdf compress
	wait

	# Trim the description file's beginning section
	sed '1,2596d' $TMP_DIR/${SECTOR_NAME}secdatadescfinaldet.txt > $TMP_DIR/${SECTOR_NAME}secdatadescfinaldet1.txt
	cat $TMP_DIR/${SECTOR_NAME}title.txt $TMP_DIR/${SECTOR_NAME}secdatadescfinaldet1.txt > $TMP_DIR/${SECTOR_NAME}_desc.txt
	# Back up the relevant files in the survey archives
	mv $TMP_DIR/${SECTOR_NAME}.pdf $OUTPUT_DIR/${SECTOR_NAME}.pdf
	mv $TMP_DIR/${SECTOR_NAME}secdata $OUTPUT_DIR/${SECTOR_NAME}.sec
	mv $TMP_DIR/${SECTOR_NAME}_desc.txt $OUTPUT_DIR/${SECTOR_NAME}_desc.txt
	wait
fi

### BACK THE REST UP
mv $TMP_DIR/${SECTOR_NAME}_polities.txt $OUTPUT_DIR/${SECTOR_NAME}_alliances.txt
mv $TMP_DIR/${SECTOR_NAME}_automsec.msec $OUTPUT_DIR/${SECTOR_NAME}_route.txt

# Provide statistics
echo "  .  ${SECTOR_NAME} containing" $NUM_WORLDS" worlds ("$DENSITY"%) took" $SECONDS "seconds to generate.   "

#Clean up all traces
rm -rf $TMP_DIR

