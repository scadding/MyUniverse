#!/bin/bash


filename=pulp.txt
outfile=pulp.tab
prepositions="in\|at\|on\|under\|near\|across"
regex="^The heroes must \([a-z]*\) \([-a-zA-Z ']*\?\) \($prepositions\) \([a-zA-Z ']*\)[ ,]*but have to contend with \([- a-zA-Z]*\)[ ,]*and opposition from \(.*\) trying to \([a-z]*\) \(.*$\)"

rm -f $outfile

echo >> $outfile
echo ";Start" >> $outfile
echo "1, The heroes must [action] [target] [preposition] [location], \
but have to contend with [contention], and opposition from [opposition] \
trying to [opposing action] [opposing target]" >> $outfile

echo >> $outfile
echo ";action" >> $outfile
cat $filename | sed "s/$regex/1, \1/" | sort | uniq >> $outfile

echo >> $outfile
echo ";target" >> $outfile
cat $filename | sed "s/$regex/1, \2/" | sort | uniq >> $outfile

echo >> $outfile
echo ";preposition" >> $outfile
cat $filename | sed "s/$regex/1, \3/" | sort | uniq >> $outfile

echo >> $outfile
echo ";location" >> $outfile
cat $filename | sed "s/$regex/1, \4/" | sort | uniq >> $outfile

echo >> $outfile
echo ";contention" >> $outfile
cat $filename | sed "s/$regex/1, \5/" | sort | uniq >> $outfile

echo >> $outfile
echo ";opposition" >> $outfile
cat $filename | sed "s/$regex/1, \6/" | sort | uniq >> $outfile

echo >> $outfile
echo ";opposing action" >> $outfile
cat $filename | sed "s/$regex/1, \7/" | sort | uniq >> $outfile

echo >> $outfile
echo ";opposing target" >> $outfile
cat $filename | sed "s/$regex/1, \8/" | sort | uniq >> $outfile
