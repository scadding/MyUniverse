
### get_UPP (requires 'full-upp', installed in bin folder)

function get_UPP()
{
	full-upp $2 > $TMP_DIR/$1uppo.txt
	cat $TMP_DIR/$1uppo.txt $TRAV_DIR/ret.txt > $TMP_DIR/$1upp.txt
	chmod ugo+w $TMP_DIR/$1upp.txt $TMP_DIR/$1uppo.txt
	#echo "Content-Type: application/text" 
	#echo " Filename: "$1upp.txt
	echo ""
	echo $1
	echo "Name: "$4 > $TMP_DIR/$1name1.txt
	sed '/_/ s/_/ /g' $TMP_DIR/$1name1.txt > $TMP_DIR/$1name.txt
	cat < $TMP_DIR/$1name.txt
	sed -n '/ '$3' /p' $TMP_DIR/*_polities.txt > $TMP_DIR/$1polityinter.txt
	cat $TRAV_DIR/Affiliation.txt $TMP_DIR/$1polityinter.txt > $TMP_DIR/$1polity.txt
	cat < $TMP_DIR/$1polity.txt
	#echo ""
	echo "UPP: "$2
	echo ""
	cat < $TMP_DIR/$1upp.txt
	rm -rf $TMP_DIR/$1upp.txt
	rm -rf $TMP_DIR/$1polityinter.txt
	rm -rf $TMP_DIR/$1polity.txt
	rm -rf $TMP_DIR/*uppo.txt
	rm -rf $TMP_DIR/$1name1.txt
	rm -rf $TMP_DIR/$1name.txt
	return
}




### get_More (grabs cargo and animal encounters)

function get_More() 
{
	cd ./trav
	perl shell.pl --tool=animal --extended --profile=$2 > $TMP_DIR/$1animo.txt
	perl shell.pl --tool=bk7.trade.cargo --profile=$2 > $TMP_DIR/$1cargo.txt
	cat $TMP_DIR/$1animo.txt $TRAV_DIR/ret.txt > $TMP_DIR/$1anim.txt
	chmod ugo+w $TMP_DIR/$1anim.txt $TMP_DIR/$1animo.txt
	#echo "Content-Type: application/text" 
	#echo " Filename: "$1anim.txt
	#echo ""
	echo "Generic Cargo:"
	#echo ""
	cat < $TMP_DIR/$1cargo.txt
	#echo $1
	echo ""
	echo "Animal Encounters:"
	#echo ""
	cat < $TMP_DIR/$1anim.txt
	rm -rf $TMP_DIR/$1anim.txt
	rm -rf $TMP_DIR/$1animo.txt
	cd ..
	return
}





### get_SectorDescriptions grabs each of the planet's descriptions (using the get_UPP function on each) and then serves the whole thing up as a text file and pdf.

function get_SectorDescriptions()
{
	needforanim=1


	## Main Loop
	if [ "$needforanim" -lt "$DETAIL" ]; then 	# -n tests to see if the argument is non empty
		while read LINE
		do
			get_UPP $LINE $2 > "$TMP_DIR/$1det$LINE-a.txt"
			#echo "  .  $1 World debug - UPP description render complete @" $SECONDS "seconds.   "
		done < $TMP_DIR/$1
	else
		while read LINE
		do
			get_UPP $LINE $2 > "$TMP_DIR/$1det$LINE-a.txt"
			#echo "  .  $1 World debug - UPP description render complete @" $SECONDS "seconds.   "
			get_More $LINE > "$TMP_DIR/$1det$LINE-banim.txt"
			#r=$((RANDOM%100000))
			#echo $r
			#/home/doug/planet/SectorMaker/planetgen/planet -S $r -o $OUTPUT_DIR/"$LINE" -C /home/doug/planet/SectorMaker/planetgen/Olsson.col -pi -g 20 -G 20 -i 0.00
			#echo "  .  $1 World debug - cargo and animal encounters description render complete @" $SECONDS "seconds.   "
		done < $TMP_DIR/$1
	fi


	# Combine all the descriptions into one text file.
	#echo " Combine descriptions to one text file "
	cat $TRAV_DIR/pageblanker.txt $TMP_DIR/$2title.txt $TRAV_DIR/descinfo.txt $TRAV_DIR/ret.txt $TMP_DIR/$1det*.txt > $TMP_DIR/$1finaldet.txt
	wait

	## Assemble the pages into a description section pdf

	# Create .ps files from each datafile
	#echo " Create .ps files from each datafile "
	enscript --word-wrap --margins=35:35:0:40 --fancy-header='travellerpagenums2' --header='|- $% -|' --header-font=Univers9 $TMP_DIR/$1finaldet.txt -f Courier6.5 --media=Traveller -q -p $TMP_DIR/$1out.ps
	wait

	# Create .pdf files from each datafile .ps
	#echo " Create .pdf files from each datafile .ps "
	ps2pdf $TMP_DIR/$1out.ps $TMP_DIR/$1piecetemp.pdf
	wait

	# Grab the pages, starting #36 (the prior 35 are blank to set up the numbering, since the pdftk program can't start the numbering at 36.
	pdftk $TMP_DIR/$1piecetemp.pdf cat 38-end output $TMP_DIR/$1piecetempText.pdf
	wait

	# Grab the first page of the descriptions
	pdftk $TMP_DIR/$1piecetempText.pdf cat 1-1 output $TMP_DIR/$1piecetempIntro.pdf
	wait

	# Place the 'World Descriptions' header on the first page, in keeping with the document's section headings.
	pdftk $TMP_DIR/$1piecetempIntro.pdf background $TRAV_DIR/Desc_Background.pdf output $TMP_DIR/$1pieceDescIntroWithBack.pdf
	wait

	# Grab the rest of the descriptions
	pdftk $TMP_DIR/$1piecetempText.pdf cat 2-end output $TMP_DIR/$1pieceEnd.pdf
	wait

	# Join the first page of descriptions (now with header) and the rest of the descriptions
	pdftk $TMP_DIR/$1pieceDescIntroWithBack.pdf $TMP_DIR/$1pieceEnd.pdf cat output $TMP_DIR/$1piece.pdf
	wait

	return
}




### get_Sectormap makes a one-page sector map, including borders.

function get_Sectormap()
{
	# Wipe pesky underscores
	sed '/_/ s/_/ /g' $TMP_DIR/${SECTOR_NAME}secdata > $TMP_DIR/${SECTOR_NAME}_secdatapre
	#rm $TMP_DIR/$1secdata

	# Remove the first line so it doesn't get chunked
	sed '1d' $TMP_DIR/${SECTOR_NAME}_secdatapre > $TMP_DIR/${SECTOR_NAME}_secdata
	rm $TMP_DIR/${SECTOR_NAME}_secdatapre

	# Cut new chunks for sec2pdf-friendly format
	cut -b20-23 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataID
	cut -b19-19 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataSpace
	cut -b1-14 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataName
	cut -b25-34 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataUPP
	cut -b35-52 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataTrade
	cut -b53-55 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataPBG
	cut -b56-58 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataEmpire
	cut -b60-60 $TMP_DIR/${SECTOR_NAME}_secdata > $TMP_DIR/${SECTOR_NAME}_secdataTAS

	# Combine chunks in sec2pdf-friendly format, with new heading for sec2pdf
	paste -d '\0' $TMP_DIR/${SECTOR_NAME}_secdataName $TMP_DIR/${SECTOR_NAME}_secdataID $TMP_DIR/${SECTOR_NAME}_secdataSpace $TMP_DIR/${SECTOR_NAME}_secdataUPP $TMP_DIR/${SECTOR_NAME}_secdataSpace $TMP_DIR/${SECTOR_NAME}_secdataTrade $TMP_DIR/${SECTOR_NAME}_secdataTAS $TMP_DIR/${SECTOR_NAME}_secdataSpace $TMP_DIR/${SECTOR_NAME}_secdataSpace $TMP_DIR/${SECTOR_NAME}_secdataPBG $TMP_DIR/${SECTOR_NAME}_secdataEmpire $TMP_DIR/${SECTOR_NAME}_secdataSpace > $TMP_DIR/${SECTOR_NAME}_newsecdata1

	rm $TMP_DIR/${SECTOR_NAME}_secdataName $TMP_DIR/${SECTOR_NAME}_secdataID $TMP_DIR/${SECTOR_NAME}_secdataSpace $TMP_DIR/${SECTOR_NAME}_secdataUPP $TMP_DIR/${SECTOR_NAME}_secdataTrade $TMP_DIR/${SECTOR_NAME}_secdataTAS $TMP_DIR/${SECTOR_NAME}_secdataPBG $TMP_DIR/${SECTOR_NAME}_secdataEmpire
	cat $TRAV_DIR/SectorTop.txt $TMP_DIR/${SECTOR_NAME}_newsecdata1 > $TMP_DIR/${SECTOR_NAME}_newsecdata
	rm $TMP_DIR/${SECTOR_NAME}_newsecdata1
	
	# Grab pre-existing alliances
	if [ "$2" != "" ]; then 	# -n tests to see if the argument is non empty
		perl allygen2.pl -f 2 -r -o $TMP_DIR/${SECTOR_NAME}_automsec2 $TMP_DIR/${SECTOR_NAME}_newsecdata
		#echo "  .  $1 Sector debug - Allygen generation complete @" $SECONDS "seconds.   "

		# Strip various allygen items, based on param $6 ($3 here)

		# Pre-existing sector file name (used if an existing set of .sec, route and alliance files is available in $OUTPUT_DIR)
		## Loop
		if [ $3 = 3 ]; then
			sed '/# Alliance:/d' $TMP_DIR/${SECTOR_NAME}_automsec2.msec > $TMP_DIR/${SECTOR_NAME}_automsec2a.msec
			sed '/border/d' $TMP_DIR/${SECTOR_NAME}_automsec2a.msec > $TMP_DIR/${SECTOR_NAME}_automsec2b.msec
			sed '/route/d' $TMP_DIR/${SECTOR_NAME}_automsec2b.msec > $TMP_DIR/${SECTOR_NAME}_automsec3.msec
			cat $OUTPUT_DIR/$1_route.txt $TMP_DIR/${SECTOR_NAME}_automsec3.msec > $TMP_DIR/${SECTOR_NAME}_automsec.msec
		elif [ $3 = 2 ]; then
			sed '/# Alliance:/d' $TMP_DIR/${SECTOR_NAME}_automsec2.msec > $TMP_DIR/${SECTOR_NAME}_automsec2a.msec
			sed '/route/d' $TMP_DIR/${SECTOR_NAME}_automsec2a.msec > $TMP_DIR/${SECTOR_NAME}_automsec3.msec
			cat $OUTPUT_DIR/$1_route.txt $TMP_DIR/${SECTOR_NAME}_automsec3.msec > $TMP_DIR/${SECTOR_NAME}_automsec.msec
		elif [ $3 = 1 ]; then
			cat $OUTPUT_DIR/$1_route.txt $TMP_DIR/${SECTOR_NAME}_automsec2.msec > $TMP_DIR/${SECTOR_NAME}_automsec.msec
		elif [ $3 = 0 ]; then
			cat $TMP_DIR/${SECTOR_NAME}_automsec2.msec > $TMP_DIR/${SECTOR_NAME}_automsec.msec
		fi


		# Generate Sector Map
		#echo " Generate Sector Map "
		perl sec2pdf.pl -s $1 -f 2 -b -a -h -B -pTraveller -t sector -m $TMP_DIR/${SECTOR_NAME}_automsec.msec -d $TMP_DIR/${SECTOR_NAME}_newsecdata -o $TMP_DIR/${SECTOR_NAME}_sectormap1.pdf
		#echo "  .  $1 Sector debug - Small Sectormap generation complete @" $SECONDS "seconds.   "

		# Generate Subsector Maps (borders, routes and regions)
		#echo " Generate Subsector Maps "
		perl sec2pdf_borders.pl -f 2 -s $1 -a -B -pTraveller -m $TMP_DIR/${SECTOR_NAME}_automsec.msec -d $TMP_DIR/${SECTOR_NAME}_newsecdata -o $TMP_DIR/${SECTOR_NAME}_subsectorborders.pdf
		#echo "  .  $1 Sector debug - Subsector routes generation complete @" $SECONDS "seconds.   "

	else
		# Generate New Allegiances
		#echo " Generate New Allegiances "
		perl allygen2.pl -f 2 -i -r -o $TMP_DIR/${SECTOR_NAME}_automsec $TMP_DIR/${SECTOR_NAME}_newsecdata
		#echo "  .  $1 Sector debug - Allygen generation complete @" $SECONDS "seconds.   "

		# Generate Sector Map
		perl sec2pdf.pl -s $1 -f 2 -b -a -B -h -pTraveller -t sector -m $TMP_DIR/${SECTOR_NAME}_automsec.msec -d $TMP_DIR/${SECTOR_NAME}_newsecdata -o $TMP_DIR/${SECTOR_NAME}_sectormap1.pdf
		#echo "  .  $1 Sector debug - Small Sectormap generation complete @" $SECONDS "seconds.   "

		# Generate Sector Map
		perl sec2pdf_borders.pl -f 2 -s $1 -a -B -pTraveller -m $TMP_DIR/${SECTOR_NAME}_automsec.msec -d $TMP_DIR/${SECTOR_NAME}_newsecdata -o $TMP_DIR/${SECTOR_NAME}_subsectorborders.pdf
		#echo "  .  $1 Sector debug - Subsector routes generation complete @" $SECONDS "seconds.   "
	fi

	# Combine the map with canon Classic Traveller background
	#echo " Combine map with canon background "
	pdftk $TRAV_DIR/backgrounds/SectorMap_Background.pdf background $TMP_DIR/${SECTOR_NAME}_sectormap1.pdf output $TMP_DIR/${SECTOR_NAME}_sectormap2.pdf compress
	rm $TMP_DIR/${SECTOR_NAME}_sectormap1.pdf
	rm $TMP_DIR/${SECTOR_NAME}_newsecdata
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
          get_Polity $LINE > "$TMP_DIR/$1polity$LINE.txt"
               done < $TMP_DIR/$1
}



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
		totalize $LINE > "$TMP_DIR/$2subsecdataItemTally$1_$tallyline"
	done < $TMP_DIR/$3

	cat $TMP_DIR/$2subsecdataItemTally$1* > $TMP_DIR/$2subsecdataFinalTally$1
	return
}






function scorer() {
	#echo " In function scorer "
	cp $TMP_DIR/$1subsecdata$x $TMP_DIR/$1subsecdataInit$x
	cut -b1-19 $TMP_DIR/$1subsecdataInit$x > $TMP_DIR/$1subsecdataScoreName$x
	cut -b24-24 $TMP_DIR/$1subsecdataInit$x > $TMP_DIR/$1subsecdataScoreSpace$x
	cut -b25-25 $TMP_DIR/$1subsecdataInit$x > $TMP_DIR/$1subsecdataScoreStarport$x
	cut -b29-29 $TMP_DIR/$1subsecdataInit$x > $TMP_DIR/$1subsecdataScorePop$x
	cut -b32-33 $TMP_DIR/$1subsecdataInit$x > $TMP_DIR/$1subsecdataScoreTech1$x
	sed '/-/ s/-/_/g' $TMP_DIR/$1subsecdataScoreTech1$x > $TMP_DIR/$1subsecdataScoreTech$x
	cut -b36-52 $TMP_DIR/$1subsecdataInit$x > $TMP_DIR/$1subsecdataScoreTrade$x
	cut -b59-61 $TMP_DIR/$1subsecdataInit$x > $TMP_DIR/$1subsecdataScoreBases$x

	paste -d '\0' $TMP_DIR/$1subsecdataScoreSpace$x $TMP_DIR/$1subsecdataScoreStarport$x $TMP_DIR/$1subsecdataScoreSpace$x $TMP_DIR/$1subsecdataScorePop$x $TMP_DIR/$1subsecdataScoreSpace$x $TMP_DIR/$1subsecdataScoreTech$x $TMP_DIR/$1subsecdataScoreSpace$x $TMP_DIR/$1subsecdataScoreTrade$x $TMP_DIR/$1subsecdataScoreBases$x > $TMP_DIR/$1subsecdataScore$x

	#Tradeclass:
	#Ri: +1
	#Po: -2
	#Hi: +5
	#Ni: -1
	#Lo: -2
	#Ba: -5
	#Fl: -1
	#echo " Tradeclass "

	sed '/Ri/ s/Ri/1/g' $TMP_DIR/$1subsecdataScore$x > $TMP_DIR/$1subsecdataScore1$x
	sed '/Po/ s/Po/-2/g' $TMP_DIR/$1subsecdataScore1$x > $TMP_DIR/$1subsecdataScore2$x
	sed '/Hi/ s/Hi/5/g' $TMP_DIR/$1subsecdataScore2$x > $TMP_DIR/$1subsecdataScore3$x
	sed '/Ni/ s/Ni/-1/g' $TMP_DIR/$1subsecdataScore3$x > $TMP_DIR/$1subsecdataScore4$x
	sed '/Lo/ s/Lo/-2/g' $TMP_DIR/$1subsecdataScore4$x > $TMP_DIR/$1subsecdataScore5$x
	sed '/Ba/ s/Ba/-5/g' $TMP_DIR/$1subsecdataScore5$x > $TMP_DIR/$1subsecdataScore6$x
	sed '/Fl/ s/Fl/-1/g' $TMP_DIR/$1subsecdataScore6$x > $TMP_DIR/$1subsecdataScore7$x

	#Zones:
	#Amber: -5
	#Red: -20
	#U: -20
	#echo " Zones "

	sed '/R$/ s/R$/-20/g' $TMP_DIR/$1subsecdataScore7$x > $TMP_DIR/$1subsecdataScore8$x
	sed '/A$/ s/A$/-5/g' $TMP_DIR/$1subsecdataScore8$x > $TMP_DIR/$1subsecdataScore9$x

	#StarPort:
	#A: +10
	#B: +5
	#C: +2
	#D: +1
	#echo " Starport "

	sed '/^ A/ s/^ A/10/g' $TMP_DIR/$1subsecdataScore9$x > $TMP_DIR/$1subsecdataScore10$x
	sed '/^ B/ s/^ B/5/g' $TMP_DIR/$1subsecdataScore10$x > $TMP_DIR/$1subsecdataScore11$x
	sed '/^ C/ s/^ C/2/g' $TMP_DIR/$1subsecdataScore11$x > $TMP_DIR/$1subsecdataScore12$x
	sed '/^ D/ s/^ D/1/g' $TMP_DIR/$1subsecdataScore12$x > $TMP_DIR/$1subsecdataScore13$x
	sed '/^ E/ s/^ E/0/g' $TMP_DIR/$1subsecdataScore13$x > $TMP_DIR/$1subsecdataScore13a$x
	sed '/^ X/ s/^ X/0/g' $TMP_DIR/$1subsecdataScore13a$x > $TMP_DIR/$1subsecdataScore13b$x

	#Population:
	#Add population number
	#echo " Population "

	sed '/ A / s/ A / 10 /g' $TMP_DIR/$1subsecdataScore13b$x > $TMP_DIR/$1subsecdataScore14$x
	sed '/ B / s/ B / 11 /g' $TMP_DIR/$1subsecdataScore14$x > $TMP_DIR/$1subsecdataScore15$x
	sed '/ C / s/ C / 12 /g' $TMP_DIR/$1subsecdataScore15$x > $TMP_DIR/$1subsecdataScore16$x
	sed '/ D / s/ D / 13 /g' $TMP_DIR/$1subsecdataScore16$x > $TMP_DIR/$1subsecdataScore17$x
	sed '/ E / s/ E / 14 /g' $TMP_DIR/$1subsecdataScore17$x > $TMP_DIR/$1subsecdataScore18$x
	sed '/ F / s/ F / 15 /g' $TMP_DIR/$1subsecdataScore18$x > $TMP_DIR/$1subsecdataScore19$x
	sed '/ G / s/ G / 16 /g' $TMP_DIR/$1subsecdataScore19$x > $TMP_DIR/$1subsecdataScore20$x
	sed '/ H / s/ H / 17 /g' $TMP_DIR/$1subsecdataScore20$x > $TMP_DIR/$1subsecdataScore21$x
	sed '/ I / s/ I / 18 /g' $TMP_DIR/$1subsecdataScore21$x > $TMP_DIR/$1subsecdataScore22$x

	#TechLevel:
	#Add 2*techlevel
	#echo " TechLevel "

	sed '/_0 / s/_0 / 0 /g' $TMP_DIR/$1subsecdataScore22$x > $TMP_DIR/$1subsecdataScore22a$x
	sed '/_1 / s/_1 / 2 /g' $TMP_DIR/$1subsecdataScore22a$x > $TMP_DIR/$1subsecdataScore22b$x
	sed '/_2 / s/_2 / 4 /g' $TMP_DIR/$1subsecdataScore22b$x > $TMP_DIR/$1subsecdataScore22c$x
	sed '/_3 / s/_3 / 6 /g' $TMP_DIR/$1subsecdataScore22c$x > $TMP_DIR/$1subsecdataScore22d$x
	sed '/_4 / s/_4 / 8 /g' $TMP_DIR/$1subsecdataScore22d$x > $TMP_DIR/$1subsecdataScore22e$x
	sed '/_5 / s/_5 / 10 /g' $TMP_DIR/$1subsecdataScore22e$x > $TMP_DIR/$1subsecdataScore22f$x
	sed '/_6 / s/_6 / 12 /g' $TMP_DIR/$1subsecdataScore22f$x > $TMP_DIR/$1subsecdataScore22g$x
	sed '/_7 / s/_7 / 14 /g' $TMP_DIR/$1subsecdataScore22g$x > $TMP_DIR/$1subsecdataScore22h$x
	sed '/_8 / s/_8 / 16 /g' $TMP_DIR/$1subsecdataScore22h$x > $TMP_DIR/$1subsecdataScore22i$x
	sed '/_9 / s/_9 / 18 /g' $TMP_DIR/$1subsecdataScore22i$x > $TMP_DIR/$1subsecdataScore22j$x
	sed '/_A / s/_A / 20 /g' $TMP_DIR/$1subsecdataScore22j$x > $TMP_DIR/$1subsecdataScore22k$x
	sed '/_B / s/_B / 22 /g' $TMP_DIR/$1subsecdataScore22k$x > $TMP_DIR/$1subsecdataScore22l$x
	sed '/_C / s/_C / 24 /g' $TMP_DIR/$1subsecdataScore22l$x > $TMP_DIR/$1subsecdataScore22m$x
	sed '/_D / s/_D / 26 /g' $TMP_DIR/$1subsecdataScore22m$x > $TMP_DIR/$1subsecdataScore22n$x
	sed '/_E / s/_E / 28 /g' $TMP_DIR/$1subsecdataScore22n$x > $TMP_DIR/$1subsecdataScore22o$x
	sed '/_F / s/_F / 30 /g' $TMP_DIR/$1subsecdataScore22o$x > $TMP_DIR/$1subsecdataScore22p$x
	sed '/_G / s/_G / 32 /g' $TMP_DIR/$1subsecdataScore22p$x > $TMP_DIR/$1subsecdataScore22q$x
	sed '/_H / s/_H / 34 /g' $TMP_DIR/$1subsecdataScore22q$x > $TMP_DIR/$1subsecdataScore22r$x
	sed '/_I / s/_I / 36 /g' $TMP_DIR/$1subsecdataScore22r$x > $TMP_DIR/$1subsecdataScore23$x

	#Bases:
	#Z, Y, G, N, 2, H: +4
	#
	#Planet names:
	#Provincial name: -100 (To avoid that a planet with a name that sounds like a small place becomes the capital)
	#echo " Bases "
	sed '/[A-Z][a-z]/ s/[A-Z][a-z]/ /g' $TMP_DIR/$1subsecdataScore23$x > $TMP_DIR/$1subsecdataScoreFinal1$x
	sed '/  */ s/  */ /g' $TMP_DIR/$1subsecdataScoreFinal1$x > $TMP_DIR/$1subsecdataScoreFinal$x
	#echo " Done bases "

	return
}






function grabCapital() {

	# get the lowercase version of the subsector letter
	LOWERSUB=`echo $x |tr "[A-Z]" "[a-z]" `
	#echo $LOWERSUB

	# find the line in the 'routes' file containing the subsector name, if it exists.
	SUBSECNAMEFOUND=`sed -n "/^$LOWERSUB /p" $TMP_DIR/${SECTOR_NAME}_automsec.msec | awk '{ print $2 " " $3 " " $4 " " $5 }'`
	#echo $SUBSECNAMEFOUND

	# if so, grab the name's contents and put it into 'CAPITALNAME'.
	if [ "$SUBSECNAMEFOUND" != "" ]; then
		echo $SUBSECNAMEFOUND > $TMP_DIR/${SECTOR_NAME}_subsecname$x.txt
		CAPITALNAME=`cat < $TMP_DIR/${SECTOR_NAME}_subsecname$x.txt | awk '{ print $0 }'`

		# If not, combine a new file with 'Ca' autogenerated.
	else

		cut -b1-19 $TMP_DIR/$1subsecdata$x > $TMP_DIR/$1subsecdataName$x
		cut -b36-51 $TMP_DIR/$1subsecdata$x > $TMP_DIR/$1subsecdataTrade$x
		paste -d '\0' $TMP_DIR/$1subsecdataTrade$x $TMP_DIR/$1subsecdataName$x > $TMP_DIR/$1subsecdataSum1$x
		sed -n -e '/ Ca /{1!p;g;$!N;}' -e h $TMP_DIR/$1subsecdataSum1$x > $TMP_DIR/$1subsecdataSum2$x
		cut -b17-60 $TMP_DIR/$1subsecdataSum2$x > $TMP_DIR/$1subsecdataSum3$x
		sed 's/[ \t]*$//' $TMP_DIR/$1subsecdataSum3$x > $TMP_DIR/$1subsecdataSum4$x 
		PRIORCAPITAL=`cat < $TMP_DIR/$1subsecdataSum4$x | awk '{ print $0 }'`

		# If no line contains 'Ca'
		if [ "$PRIORCAPITAL" != "" ]; then
			CAPITALNAME=`cat < $TMP_DIR/$1subsecdataSum4$x | awk '{ print $1 }'`
		else
			CAPITALNAME=`cat < $TMP_DIR/$1subsecdataScorerSheetCapital$x | awk '{ print $2 }'`
			WORLDLINE=`cat < $TMP_DIR/$1subsecdataScorerSheetCapital$x | awk '{ print $1 }'`

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

	echo "The "$CAPITALNAME" subsector contains "$NUM_SUBSECWORLDS" worlds."  > $TMP_DIR/$1subsecdataSum$x
	#, with a population of "$SUBSECPOP". The highest population is "$HIGHPOP", at "$HIGHPOPWORLD". The highest tech level is "$HIGHTEK", at "$HIGHTEK_WORLD"."
	return
}


