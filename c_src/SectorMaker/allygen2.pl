#!/usr/bin/perl
# allygen: compute regional borders for a SEC-format Traveller data
# file, based on the "influence" of good starports, suitable for
# feeding into sec2pdf as a metadata-file. It can also completely
# ignore the existing allegiance data and create brand new allegiances,
# saving the results to a fresh SEC file.
#
# $Id: allygen,v 1.15 2004/11/09 03:36:15 jgreely Exp $
#
use strict;
use Getopt::Long;
Getopt::Long::Configure("Bundling");

my @Same;
my $NoAllies = "";
my $Ignore = 0;
my $Fudge = 0;
my $Routes = 0;
my $DEBUG = 0;
my $OutFile;
my $DoDOS = 0;

#TODO: add option & code for dumping a new SEC file when -i is used
#
GetOptions("same|s=s" => \@Same,"nonaligned|n=s" => \$NoAllies,
	"fudge|f=i" => \$Fudge, "ignore|i" => \$Ignore,"routes|r" => \$Routes,"dos|d" => \$DoDOS,
	"DEBUG|D" => \$DEBUG,"output|o=s" => \$OutFile) or die <<EOF;
usage: allygen [-di] [-o out] [-f N] [-n Na,...] [-s Im,...] secfile

        -d write output files as DOS text instead of Unix (CRLF)

      * -r ignore existing routes and create new ones
        -i ignore existing allegiances and create new ones.
        -f fudge-factor for scaling influence of systems on others (+-N).
        -n treat comma-separated list of allegiance codes as non-aligned
           (don't draw borders around them).
        -o save border data to out.msec; if -i is also specified, save
           a revised version of the input SEC file with new allegiances.
        -s treat comma-separated list of allegiance codes as the same
           when generating borders.
EOF

my $CR = $DoDOS ? "\r" : "";

my ($SectorFile) = @ARGV;
die "usage: $0 [...] sectorfile\n" unless -r $SectorFile;

if ($OutFile) {
	open(Meta,">$OutFile.msec") or die "$0: $OutFile.msec: $!\n";
	select(Meta);
}

my @S;
my %A;
my %Aname;
my $mod;
my %Aneighbor;
my @WorldAlly;
my %Port;
my %Pop;
my %TL;
my %Data;
my $I = 0;
my $pbgtest;
my $uwptest;

# record the "non-aligned" codes
#
my %NoAllies;
foreach (split(/,/,$NoAllies)) {
	$NoAllies{$_} = 1;
}

# collapse multiple allegiance codes to one
#
my %Same;
foreach (@Same) {
	my ($primary,@rest) = split(/,/);
	foreach my $i (@rest) {
		$Same{$i} = $primary;
	}
}

readsectorfile();

# for each populated system, in order by its starport, population, and
# tech level, claim it as part of the region, and then extend its
# influence outward based on starport quality.
#
foreach (sort bycoolness keys %Port) {
	my ($c,$r) = /^(..)(..)$/;

	if ($Ignore) {
		if (! $S[$c][$r]) {
			$I++;
			$S[$c][$r] = $I;
			$A{$I} = $I;
#			$Aname{$I} = sprintf("%s%s",unidigit(int($I/33)),
			$Aname{$I} = sprintf("%s%s",unidigit(int(rand($I % 92))),
				unidigit_lc(int($I % 33)));
		}
		$WorldAlly[$c][$r] = $S[$c][$r];
	}

	my $A = $A{$WorldAlly[$c][$r]};
	
	# ignore non-aligned worlds
	#
	if ($NoAllies{$WorldAlly[$c][$r]}) {
		$S[$c][$r] = 99999;
		next;
	}

	# I claim this planet in the name of Mars
	#
	$S[$c][$r] = $A;

	# what neighbors can I claim?
	# (E and X starports have no influence at all)
	#
	next if $Port{$c.$r} gt "D";
	my ($cn,$rn);

	# starting at distance 1, up to the limit of my influence,
	# claim unclaimed hexes
	#
	foreach my $dist (1..index("DCBA",$Port{$c.$r})+$Fudge) {
		#start in direction 4
		($cn,$rn) = neighbor($c,$r,$dist,4);
		$S[$cn][$rn] = $A unless $S[$cn][$rn];
		#for each direction
		for my $dir (0..5) {
			#claim $dist hexes in a line
			for my $i (1..$dist) {
				($cn,$rn) = neighbor($cn,$rn,1,$dir);
				# ignore out-sector hexes
				next if $cn < 1 or $cn > 32 or $rn < 1
					or $rn > 40;
				$S[$cn][$rn] = $A unless $S[$cn][$rn];
			}
		}
	}
}

# compute number of hexes in each allegiance
#
my %Count = ();;
foreach my $c (1..32) {
	foreach my $r (1..40) {
		next if $S[$c][$r] == 99999 or $S[$c][$r] == 0;
		$Count{$S[$c][$r]}++;
	}
}

# switch empty squares to largest allegiance of adjacent planet
#
foreach my $c (1..32) {
	foreach my $r (1..40) {
		next if $WorldAlly[$c][$r];
		my %tmp = ();
		for my $dir (0..5) {
			my ($cn,$rn) = neighbor($c,$r,1,$dir);
			next if $S[$cn][$rn] == 99999 or $S[$cn][$rn] == 0;
			$tmp{$S[$cn][$rn]} = 1 if $WorldAlly[$cn][$rn];
		}
		next unless keys %tmp;
		my $ally = 0;
		foreach (sort keys %tmp) {
			$ally = $_ if $Count{$_} >= $Count{$ally};
		}
		$S[$c][$r] = $ally;
		$Count{$S[$c][$r]}--;
		$Count{$ally}++;
	}
}

# repel borders! non-aligned worlds want elbow room!
#
foreach my $c (1..32) {
	foreach my $r (1..40) {
		next unless $WorldAlly[$c][$r] and $S[$c][$r] == 99999;
		for my $dir (0..5) {
			my ($cn,$rn) = neighbor($c,$r,1,$dir);
			$S[$cn][$rn] = 0 unless $WorldAlly[$cn][$rn];
		}
	}
}

	if ($Routes) {
# routermatic! - generate trade routes and like it
# vary the results based on starports. Yikes! Here we go.

# pair    J1   J2   J3   J4
# A-A     1    2    4    5
# A-B     1    3    4    5
# A-C     1    4    6    -
# A-D     1    5    -    -
# A-E     2    -    -    -
# B-B     1    3    4    6
# B-C     2    4    6    -
# B-D     3    6    -    -
# B-E     4    -    -    -
# C-C     3    6    -    -
# C-D     4    -    -    -
# C-E     4    -    -    -
# D-D     4    -    -    -
# D-E     5    -    -    -
# E-E     6    -    -    -

my $SECTOR_WIDTH=32;
my $SECTOR_HEIGHT=40;

sub even { return ( $_[0] % 2 ) == 0; }
sub odd  { return !even($_[0]);       }

sub div { return int( $_[0] / $_[1] ); }
sub mod { return int( $_[0] % $_[1] ); }

sub max { my $max = shift; while( @_ ) { my $test = shift; if( $test > $max ) { $max = $test; } } return $max; }

sub dist

{

	my($a,$b) = @_;



	my $a_x = div($a,100);

	my $a_y = mod($a,100);

	my $b_x = div($b,100);

	my $b_y = mod($b,100);



	my $dx = $b_x - $a_x;

	my $dy = $b_y - $a_y;



	my $adx = abs($dx);

	my $ody = $dy + div( $adx, 2 );



	if( odd($a_x) && even($b_x) ) { $ody += 1; }



	return max($adx - $ody, $ody, $adx);

}




sub reachable
{
	my( $hex, $jump ) = @_;
	my @results = ();

	my $x = div($hex,100);
	my $y = mod($hex,100);

	for( my $rx = $x - $jump; $rx <= $x + $jump; $rx++ )
	{
		for( my $ry = $y - $jump; $ry <= $y + $jump; $ry++ )
		{
			if( $rx >= 1 && $rx <= $SECTOR_WIDTH && $ry >= 1 && $ry <= $SECTOR_HEIGHT )
			{
				my $candidate = sprintf("%02d%02d", $rx, $ry );
				my $dist = dist( $hex, $candidate );
				if( $dist > 0 && $dist <= $jump )
				{
					push( @results, $candidate );
				}
			}
		}
	}

	return @results;	
}




foreach (sort bycoolness keys %Port) {
	my ($c,$r) = /^(..)(..)$/;
	next if $Port{$c.$r} gt "C";


	foreach my $dist (1..index("DCBA",$Port{$c.$r}))  {
#		for my $dir (0..5) {
#		#for each direction
#			my ($cn,$rn) = neighbor($c,$r,$dist,$dir);
#				# ignore out-sector hexes
#				next if $cn < 1 or $cn > 32 or $rn < 1
#					or $rn > 40;
#


foreach ( reachable( sprintf("%02d%02d", $c, $r ), $dist ) )
{
    my( $cn, $rn ) = /^(..)(..)$/;	next if $Port{$cn.$rn} gt "C";
	if ($WorldAlly[$cn][$rn]) {

$mod="99";

#if ($dist=1) {
#
## A
#
#if (($Port{$c.$r} gt "A") and ($Port{$cn.$rn} lt "A")) {
#$mod="0"
#};
#if (($Port{$c.$r} lt "A") and ($Port{$cn.$rn} lt "B")) {
#$mod="0"
#};
#if (($Port{$c.$r} lt "A") and ($Port{$cn.$rn} lt "C")) {
#$mod="0"
#};
#if (($Port{$c.$r} lt "A") and ($Port{$cn.$rn} lt "D")) {
#$mod="0"
#};
#if (($Port{$c.$r} lt "A") and ($Port{$cn.$rn} lt "E")) {
#$mod="1"
#};
#
## B
#
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "A")) {
#$mod="0"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "B")) {
#$mod="0"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "C")) {
#$mod="1"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "D")) {
#$mod="2"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "E")) {
#$mod="3
#"
#};
#
## C
#
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "A")) {
#$mod="0"
#};
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "B")) {
#$mod="1"
#};
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "C")) {
#$mod="2"
#};
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "D")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "E")) {
#$mod="3"
#};
#
## D
#
#if (($Port{$c.$r} = "D") and ($Port{$cn.$rn} = "A")) {
#$mod="1"
#};
#if (($Port{$c.$r} = "D") and ($Port{$cn.$rn} = "B")) {
#$mod="2"
#};
#if (($Port{$c.$r} = "D") and ($Port{$cn.$rn} = "C")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "D") and ($Port{$cn.$rn} = "D")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "D") and ($Port{$cn.$rn} = "E")) {
#$mod="4"
#};
#
## E
#
#if (($Port{$c.$r} = "E") and ($Port{$cn.$rn} = "A")) {
#$mod="1"
#};
#if (($Port{$c.$r} = "E") and ($Port{$cn.$rn} = "B")) {
#$mod="2"
#};
#if (($Port{$c.$r} = "E") and ($Port{$cn.$rn} = "C")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "E") and ($Port{$cn.$rn} = "D")) {
#$mod="4"
#};
#if (($Port{$c.$r} = "E") and ($Port{$cn.$rn} = "E")) {
#$mod="5"
#};
#};
#
#
## Distance 2
#
#if ($dist=2) {
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "A")) {
#$mod="1"
#};
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "B")) {
#$mod="2"
#};
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "C")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "D")) {
#$mod="4"
#};
#
## B
#
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "A")) {
#$mod="2"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "B")) {
#$mod="2"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "C")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "D")) {
#$mod="5"
#};
#
#};
#
## C
#
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "A")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "B")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "C")) {
#$mod="5"
#};
#
## D
#
#if (($Port{$c.$r} = "D") and ($Port{$cn.$rn} = "A")) {
#$mod="1"
#};
#if (($Port{$c.$r} = "D") and ($Port{$cn.$rn} = "B")) {
#$mod="2"
#};
#
## E
#
#};
#
#
#
#
#
#
## Distance 3
#
#if ($dist=3) {
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "A")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "B")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "C")) {
#$mod="5"
#};
#
## B
#
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "A")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "B")) {
#$mod="3"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "C")) {
#$mod="5"
#};
#
## C
#
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "A")) {
#$mod="5"
#};
#if (($Port{$c.$r} = "C") and ($Port{$cn.$rn} = "B")) {
#$mod="5"
#};
#
## D
#
## E
#
#};
#
#
#
#
#
## Distance 4
#
#if ($dist=4) {
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "A")) {
#$mod="4"
#};
#if (($Port{$c.$r} = "A") and ($Port{$cn.$rn} = "B")) 4
#$mod="4"
#};
#
## B
#
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "A")) {
#$mod="4"
#};
#if (($Port{$c.$r} = "B") and ($Port{$cn.$rn} = "B")) {
#$mod="5"
#};
#
## C
#
## D
#
## E
#
#};
#
#
#
if (((int(rand(100)+1) ) > 92)) {
#		$Aneighbor{$I} = sprintf("route %01d %01d %01d %02d%02d %02d%02d",$mod,$Port{$c.$r},$Port{[$cn].[$rn]},$c,$r,$cn,$rn,"$CR\n");			
		$Aneighbor{$I} = sprintf("route %02d%02d %02d%02d",$c,$r,$cn,$rn,"$CR\n");			
		print $Aneighbor{$I},"$CR\n";
		};
}






}
}

}

}
# the empires strike back! don't let non-aligned worlds
# break an allegiance in two
#
foreach my $c (1..32) {
	foreach my $r (1..40) {
		next if $WorldAlly[$c][$r];
		my %tmp;
		for my $dir (0..5) {
			my ($cn,$rn) = neighbor($c,$r,1,$dir);
			$tmp{$S[$cn][$rn]}++ if $WorldAlly[$cn][$rn]
				and $S[$cn][$rn] != 99999;
		}
		my $ally = (sort { $tmp{$b} <=> $tmp{$a} } keys %tmp)[0];
		next unless $tmp{$ally} >= 2;
		$S[$c][$r] = $ally;
	}
}

# un-align empty squares that are far away from their allied worlds
#
foreach my $c (1..32) {
	foreach my $r (1..40) {
		next if $WorldAlly[$c][$r];
		my $count = 0;
		my ($cn,$rn);
		foreach my $dist (1..2) {
			#start in direction 4
			($cn,$rn) = neighbor($c,$r,$dist,4);
			#for each direction
			for my $dir (0..5) {
				for my $i (1..$dist) {
					($cn,$rn) = neighbor($cn,$rn,1,$dir);
					next if $cn < 0 or $rn < 0;
					$count++ if $S[$c][$r] ==
						$A{$WorldAlly[$cn][$rn]};
				}
			}
		}
		$S[$c][$r] = 0 unless $count;
	}
}

# un-align empty hexes that have less than two adjacent friends
# (it's so nice we'll do it twice!)
#
foreach (1..2) {
	foreach my $c (1..32) {
		foreach my $r (1..40) {
			next if $WorldAlly[$c][$r];
			my $count = 0;
			for my $dir (0..5) {
				my ($cn,$rn) = neighbor($c,$r,1,$dir);
				# ignore out-sector hexes
				next if $cn < 1 or $cn > 32 or $rn < 1
					or $rn > 40;
				$count++ if $S[$cn][$rn] == $S[$c][$r];
			}
			$S[$c][$r] = 0 unless $count > 1;
		}
	}
}

#dump cheesy fills for testing
#
if ($DEBUG) {
	foreach my $c (1..32) {
		foreach my $r (1..40) {
			next if $S[$c][$r] == 99999;
			next if $S[$c][$r] == 0;
			printf("region %02d%02d %.4f$CR\n",$c,$r,
				0.9 - $S[$c][$r] / (1.75*$I)) if $S[$c][$r];
		}
	}
}

# hoo-boy, here we go: walk the outside edge of each connected
# region in the sector
#
foreach my $c (1..32) {
	my $current = 0;
	foreach my $r (1..40) {
		my $A = $S[$c][$r];
		next if abs($current) == abs($A);
		$current = $A;
		next if $A == 0 or $A == 99999;
		next if $A < 0;
		my @R = walkregion($c,$r,$A);
		print "# Alliance: ",$Aname{$A},"$CR\n",
			join(" ","border",@R),"$CR\n" if @R;
	}
}

# need to deal with completely-surrounded non-aligned worlds
# (not often needed, unless repel-borders is disabled)
# TODO: not quite good enough for alphacrucis.sec (use -D to see why...)
#
foreach my $c (1..32) {
	foreach my $r (1..40) {
		next unless $S[$c][$r] == 99999;
		my $count = 0;
		foreach my $dir (0..5) {
			my ($cn,$rn) = neighbor($c,$r,1,$dir);
			$count++ if $S[$cn][$rn] > 0;
		}
		next unless $count == 5;
		my @R = walkregion($c,$r,99999);
		print "# Non-aligned\n",join(" ","border",@R),"$CR\n" if @R;
	}
}

if ($Ignore and $OutFile) {
	open(Out,">$OutFile.sec") or die "$0: $OutFile.sec: $!\n";
	print Out <<EOF;
SECTOR DATA$CR
$CR
The data in the sector text files is laid out in column format:$CR
$CR
 1-14: Name$CR
15-18: HexNbr$CR
20-28: UWP$CR
   31: Bases$CR
33-47: Codes & Comments$CR
   49: Zone$CR
52-54: PBG$CR
56-57: Allegiance$CR
59-74: Stellar Data$CR
$CR
....+....1....+....2....+....3....+....4....+....5....+....6....+....7....+....8$CR
EOF
	foreach (sort keys %Port) {
		my ($name,$uwp,$bases,$trade,$zone,$pbg,$ally,$stars)
			= split(/\t/,$Data{$_});
		my ($c,$r) = /^(..)(..)$/;
		$ally = $Aname{$WorldAlly[$c][$r]};
		printf Out ("%-14s%04d %-9s  %1s %-15s %1s  %-3s %-2s %s$CR\n",
			$name,$_,$uwp,$bases,$trade,$zone,$pbg,$ally,$stars);
	}
	close(Out);
}

if ($OutFile) {
	select(STDOUT);
	close(Meta);
}

exit 0;

# reproduce the walk-around-the-edges routine used in sec2pdf
#
sub walkregion {
	my ($c,$r,$ally) = @_;
	my @R = (sprintf("%02d%02d",$c,$r));
	#directions checked in starting hex
	my @checked = (1,1,1);
	my $checkfirst = 3;
	my $checklast = 5;
	my ($c1,$r1) = ($c,$r);
	my ($cn,$rn);
	until ($checked[4] and $checked[5]) {
		last if $c == $c1 and $r == $r1 and $checked[$checkfirst];
		$checklast = $checkfirst + 5;
		my $dir;
		for ($checkfirst..$checklast) {
			$dir = $_;
			if ($c == $c1 and $r == $r1) {
				if ($checked[$dir % 6]++) {
					($cn,$rn) = (0,0);
					last;
				}
			}
			($cn,$rn) = neighbor($c,$r,1,$dir % 6);
			next if $cn < 1 or $cn > 32 or $rn < 1 or $rn > 40;
			last if abs($S[$cn][$rn]) == $ally;
		}
		if (abs($S[$cn][$rn]) == $ally) {
			$S[$c][$r] = -1 * $ally; #been there!
			push(@R,sprintf("%02d%02d",$cn,$rn));
			$checkfirst = ($dir + 4) % 6;
			($c,$r) = ($cn,$rn);
		}else{
			#can't find a friend!
			last;
		}
	}
	# scan for bogon regions
	#   - invalid ones made when you leave concentric regions
	#   - empty ones made when you hit open space inside a region
	#
	my ($cs,$rs) = $R[0] =~ /^(..)(..)$/;
	my $pcount = 0;
	foreach (@R) {
		my ($ct,$rt) = /^(..)(..)$/;
		return () if $ct < $cs;
		return () if $ct == $cs and $rt < $rs;
		$pcount++ if $WorldAlly[$ct][$rt];
	}
	return () if @R < 4 and $pcount == 0;
	return @R;
}

#location of neighbor at a certain distance, in a certain direction
#directions are numbered starting with 0 (LL) and going clockwise
#
sub neighbor {
	my ($c,$r,$dist,$dir) = @_;
	my ($c2,$r2);
	#expanded for clarity
	if ($dir == 0) {
		$c2 = $c - $dist;
		$r2 = $r + int(($dist + 1 - $c % 2)/2);
	}elsif ($dir == 1) {
		$c2 = $c - $dist;
		$r2 = $r - int(($dist + $c % 2)/2);
	}elsif ($dir == 2) {
		$c2 = $c;
		$r2 = $r - $dist;
	}elsif ($dir == 3) {
		$c2 = $c + $dist;
		$r2 = $r - int(($dist + $c % 2)/2);
	}elsif ($dir == 4) {
		$c2 = $c + $dist;
		$r2 = $r + int(($dist + 1 - $c % 2)/2);
	}elsif ($dir == 5) {
		$c2 = $c;
		$r2 = $r + $dist;
	}else{
		return undef;
	}
	return ($c2,$r2);
}

sub bycoolness {
	#lower is better
	return $Port{$a} cmp $Port{$b} if $Port{$a} ne $Port{$b};
	#higher is better
	return $TL{$b} <=> $TL{$a} if $TL{$b} != $TL{$a};
	#higher is better
	return $Pop{$a} <=> $Pop{$b} if $Pop{$a} != $Pop{$b};
}

sub readsectorfile {
	open(In,$SectorFile) or die "$0: $SectorFile: $!\n";
	#fast parse of traditional .SEC file
	while (<In>) {
		last if /^\.\.\./;
	}
	while (<In>) {
		next if /^\s+$/;
		my ($name,$c,$r,$uwp,$bases,$trade,$zone,$pbg,$ally,$stars) =
			/^(.{14})(.{2})(.{2}) (.{9})  (.) (.{15}) (.)  (.{3}) (.{2}) (.*)$/;
		$name =~ s/\s+$//;
		$stars =~ s/\s+$//;

		# preserve original data in case we're writing it back out
		#
		$Data{$c.$r} = join("\t",$name,$uwp,$bases,$trade,$zone,
			$pbg,$ally,$stars);

		# record allegiance if we're not ignoring it
		if (! $Ignore) {
			# collapse allegiances that should share borders
			$ally = $Same{$ally} if $Same{$ally};
			if (! defined $A{$ally}) {
				$I++;
				$A{$ally} = $I;
				$Aname{$I} = $ally;
			}
			$WorldAlly[$c][$r] = $ally;
		}
		$Port{$c.$r} = substr($uwp,0,1);
		$Pop{$c.$r} = 10 ^ uni2dec(substr($uwp,4,1)) * substr($pbg,0,1);
		$TL{$c.$r} = uni2dec(substr($uwp,8,1));
		close (MYFILE);
	}
	close(In);
}

sub uni2dec {
	return undef unless length($_[0]) == 1;
	return index("0123456789ABCDEFGHJKLMNPQRSTUVWXY",$_[0]);
}

sub unidigit {
	return substr("ABCDEFGHJKLMNPQRSTUVWXYZabcdefghjkmnpqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZabcdefghjkmnpqrstuvwxyz",$_[0],1);
}

sub unidigit_lc {
	return substr("0123456789abcdefghjklmnpqrstuvwxy",$_[0],1);
}
