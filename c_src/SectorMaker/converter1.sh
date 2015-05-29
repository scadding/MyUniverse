#!/bin/bash
#clear
#Convert DAT files to usable format

cut -b1-14 /sectors/tmp/$1.sec > /sectors/tmp/$1_names1

#remove trailing spaces in names
sed 's/[ \t]*$//' /sectors/tmp/$1_names1 > /sectors/tmp/$1_names2

#convert spaces to underscores
sed 's/ /_/g' /sectors/tmp/$1_names2 > /sectors/tmp/$1_names3

#return names to proper width
sed -e :a -e 's/^.\{1,14\}$/& /;ta' /sectors/tmp/$1_names3 > /sectors/tmp/$1_names

cut -b15-18 /sectors/tmp/$1.sec > /sectors/tmp/$1_ID
cut -b19-19 /sectors/tmp/$1.sec > /sectors/tmp/$1_space
cut -b20-29 /sectors/tmp/$1.sec > /sectors/tmp/$1_Upp
cut -b31-32 /sectors/tmp/$1.sec > /sectors/tmp/$1_Base
cut -b33-46 /sectors/tmp/$1.sec > /sectors/tmp/$1_Trade
cut -b52-57 /sectors/tmp/$1.sec > /sectors/tmp/$1_PBG
cut -b49-49 /sectors/tmp/$1.sec > /sectors/tmp/$1_Zone
cut -b52-58 /sectors/tmp/$1.sec > /sectors/tmp/$1_End
paste -d '\0' /sectors/tmp/$1_names /sectors/tmp/$1_space /sectors/tmp/$1_space /sectors/tmp/$1_space /sectors/tmp/$1_space /sectors/tmp/$1_ID /sectors/tmp/$1_space /sectors/tmp/$1_Upp /sectors/tmp/$1_Base /sectors/tmp/$1_Trade /sectors/tmp/$1_space /sectors/tmp/$1_space /sectors/tmp/$1_PBG /sectors/tmp/$1_space /sectors/tmp/$1_Zone > /sectors/tmp/$1paste.sec
open /sectors/tmp/$1paste.sec 

#
#FarFrontiers
#
#cut -b61-61 FarFrontiersINTER.sec > FarFrontiers_space
#cut -b1-48 FarFrontiersINTER.sec > FarFrontiers_front
#cut -b74-75 FarFrontiersINTER.sec > FarFrontiers_tas
#cut -b76-81 FarFrontiersINTER.sec > FarFrontiers_back
#
#paste -d '\0' FarFrontiers_front FarFrontiers_tas FarFrontiers_space FarFrontiers_back > FarFrontiers_new
#
