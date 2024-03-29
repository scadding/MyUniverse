#!/usr/bin/awk -f
# This is an awk script.  It should be installed with the execute bit set.
# The first line will magically cause awk to be invoked when the file is
# exec'ed.
#
# subsec S=<subsector> sectorfile
#
# subsec will extract one subsector out of a sector's worth of worlds.
#
# you must provide it with two arguments: S=<subsector>, where <subsector>
# is a letter from A through P, to indicate the subsector:
#
#	A B C D
#	E F G H
#	I J K L
#	M N O P
#
# and the name of the file containing the sector information.  If you
# wish the file to come from stdin, use a filename of "-".  Examples:
#
#	gensec > sector.sec
#	subsec S=A sector.sec
#	subsec S=H - <sector.sec
#
# Copyright 1989 James Perkins.  The holder of this file may share it
# with others, as long as it is not transferred for monetary gain.  This
# copyright notice must be retained on all shared copies.
#

BEGIN {
	# Assume this could be an old format file.  The hexid is in
	# column 1.  Make sure we initialize first time thru.
	hexidcol = 1;
	mustinit = 1;
}

mustinit == 1 {
	# was S initialized on the command line?
	if (S !~ /^[A-Pa-p]$/) {
		print "usage: subsec S=[A-P] sectorfile";
		exit;
	}
	# Translate the S variable into the starting x and y of the
	# subsector.
	if (S ~ /[AaEeIiMm]/) sx = 1;
	if (S ~ /[BbFfJjNn]/) sx = 9;
	if (S ~ /[CcGgKkOo]/) sx = 17;
	if (S ~ /[DdHhLlPp]/) sx = 25;
	if (S ~ /[AaBbCcDd]/) sy = 1;
	if (S ~ /[EeFfGgHh]/) sy = 11;
	if (S ~ /[IiJjKkLl]/) sy = 21;
	if (S ~ /[MmNnOoPp]/) sy = 31;
	mustinit = 0;
}

/^#Version: 2$/ {
	# This is a new format file.  Echo this to the output,
	# Set the hexid column to column 20, and go on to the next
	# line.
	print $0;
	hexidcol = 20;
	continue;
}

{
	x = substr($0, hexidcol, 2) + 0;
	y = substr($0, hexidcol + 2, 2) + 0;
	if (x >= sx && x < sx + 8) {
		if (y >= sy && y < sy + 10) {
			print $0;
		}
	}
}
