#!/usr/bin/perl
# sec2pdf: turn SEC-format Traveller data into spiffy PDF maps
# (source released into the public domain by J Greely; knock
# yourselves out)
#
# $Id: sec2pdf,v 1.81 2005/03/20 18:38:20 jgreely Exp $
#

use strict;
use Getopt::Long;
use PDF::API2::Lite; # part of PDF::API2, available from CPAN

#hack to gain access to clipping regions (changed between versions...)
#
my $cliphack = 'gfx';
if ($PDF::API2::VERSION =~ /^1\./) {
	$cliphack = 'gfx';
}

Getopt::Long::Configure("Bundling");

#globals used to hold all the data
#
my %paper;
my %grid;
my @edgex;
my @edgey;
my %meta;
my @ally;
my @base;
my @filledregion;
my @strokedregion;
my @route;
my @label;
my %SystemData;

my %Style;
my %StyleFont;

#variables for command-line options
#
my $DEBUG = 0;
my $DoLandscape = 0;
my $PaperSize = '8.5x11';
my $GridType = '';
my $PrintBorder = 0;
my $PrintDivisions = 0;
my $MarginX = 0.625;
my $MarginY = .75;
my $PrintAll = 0;
my $PrintUWP = 0;
my $DoCaps = 0;
my $SectionC = 0;
my $SectionR = 0;
my $FirstHex = "0101";
my $FirstC = 0;
my $FirstR = 0;
my @SectorFile;
my @SectorBB = (999,999,-999,-999); #startcol,startrow,endcol,endrow
my $AutoScale = 0;
my @MetaFile;
my $SectorName = '';
my $Grayscale = 0;
my $OutputFile = 'grid.pdf';
my $StyleFile = undef; # defaults set in mainsetup()
my $NoClutter = 0;
my $MultiSector = 0;

mainsetup();

my $pdf = PDF::API2::Lite->new;

readstyles($StyleFile);
grayscalehack() if $Grayscale;

papersetup($DoLandscape);

#if we're not scaling based on the data file, do this now
gridsetup() unless $AutoScale;

my ($tmpX,$tmpY,$tmpH,$tmpW,$tmpE,$tmpS,$tmpC);
if ($MultiSector) {
	die "$0: must use --type sector with --multisector\n"
		if $grid{type} ne 'Sector';
	die "$0: must supply exactly four SEC files for --multisector\n"
		if $#SectorFile != 3;
	$Style{"map-text"} = "off";
	newpage();
	$tmpX = $grid{origX};
	$tmpY = $grid{origY};
	$grid{height} /= 2;
	$grid{width} /= 2;
	$grid{edge} /= 2;
	$grid{sin} /= 2;
	$grid{cos} /= 2;
	foreach my $i (0..5) {
		$edgex[$i] /= 2;
		$edgey[$i] /= 2;
	}
}
foreach my $i (0..$#SectorFile) {
	if ($MultiSector) {
		$grid{origX} = $tmpX;
		$grid{origY} = $tmpY;
		$grid{origY} += $grid{height} - $grid{cos} if $i < 2;
		$grid{origX} += $grid{width} - $grid{sin} if $i % 2;
	}
	@filledregion = ();
	@strokedregion = ();
	@ally = ();
	@base = ();
	@route = ();
	@label = ();
	%meta = ();
	%SystemData = ();
	$SectorName = undef;
	readmetafile($MetaFile[$i]) if $MetaFile[$i];
	readsectorfile($SectorFile[$i]) if $SectorFile[$i];
	gridsetup() if $AutoScale;
	
	if ($PrintAll) {
		my $tmp = 4/$grid{divnum} - 1;
		for my $offsetR (0..$tmp) {
			for my $offsetC (0..$tmp) {
				newpage() unless $MultiSector;
				completepage($offsetC,$offsetR);
			}
		}
	}else{
		$SectionC += $FirstC / $grid{cols};
		$SectionR += $FirstR / $grid{rows};
		newpage() unless $MultiSector;
		completepage($SectionC,$SectionR);
	}
}
if ($MultiSector) {
	setstyles("map-border");
	$pdf->rectxy($tmpX,$tmpY,$tmpX + $grid{width} * 2 - $grid{sin},
		$tmpY + $grid{height} * 2 - $grid{cos});
	$pdf->move($tmpX,$tmpY + $grid{height});
	$pdf->line($tmpX + $grid{width} * 2 - $grid{sin},
		$tmpY + $grid{height});
	$pdf->move($tmpX + $grid{width} - $grid{sin},$tmpY);
	$pdf->line($tmpX + $grid{width} - $grid{sin},
		$tmpY + $grid{height} * 2 - $grid{cos});
	$pdf->stroke;
}
$pdf->saveas($OutputFile);
exit 0;

sub newpage {
	$pdf->page($paper{$PaperSize}{x},$paper{$PaperSize}{y});

	if ($DEBUG) {
		# show paper boundaries
		$pdf->rect(0,0,$paper{$PaperSize}{x},$paper{$PaperSize}{y});
		$pdf->stroke;
	}
}

sub drawpage {
	my ($oc,$or) = @_;

	my $tmpy = $grid{origY} + $grid{height} + 6;
	styledtext("map-text-sector",1,$SectorName,$grid{origX},$tmpy);
	my $header;
	if ($grid{type} eq 'Quadrant') {
		#TODO: print quadrant names from .msec file
		$header = ("Alpha","Beta","Gamma","Delta")[$oc + 2 * $or]
			. ' Quadrant';
	}elsif ($grid{type} eq 'Subsector') {
		my $tmp = chr(65 + $oc + 4 * $or);
		if ($meta{$tmp}) {
			$header = $meta{$tmp} . " Subsector";
		}else{
			$header = "Subsector $tmp" ;
		}
	}
	styledtext("map-text-section",1,$header,
		$grid{origX} + $grid{width},$tmpy);

	for my $c (1..$grid{cols}) {
		for my $r (1..$grid{rows}) {
			drawhex($c,$r,$oc * $grid{cols},$or * $grid{rows});
		}
	}
	drawdividers($grid{divnum}) if $PrintDivisions;
	if ($PrintBorder) {
		drawfixup() unless $NoClutter > 1;
		drawborder();
	}
}

#draw the two extra lines to make a bordered map look right
#assumes same graphics state as drawhex()
#
sub drawfixup {
	setstyles("hex");
	$pdf->move($grid{origX},$grid{origY});
	$pdf->line($grid{origX}+$grid{sin},$grid{origY}+$grid{cos});
	$pdf->move($grid{origX}+$grid{width},$grid{origY}+$grid{height});
	$pdf->line($grid{origX}+$grid{width}-$grid{sin},
		$grid{origY}+$grid{height}-$grid{cos});
	$pdf->stroke;
}

#draw subdivisions of a grid (for showing subsectors on a quadrant
#or sector map)
#
sub drawdividers {
	my ($count) = @_;
	setstyles("map-dividers");
	for my $i (0..$count-1) {
		my $x = 1.5 * $i * ($grid{cols}/$count) * $grid{edge};
		$pdf->move($grid{origX} + $x,$grid{origY});
		$pdf->line($grid{origX} + $x,$grid{origY} + $grid{height});
		$pdf->stroke;
	}
	#horizontal
	for my $i (1..$count) {
		my $y = (2 * $i * $grid{rows}/$count + 1) * $grid{cos};
		$pdf->move($grid{origX},$grid{origY} + $y);
		$pdf->line($grid{origX} + $grid{width},$grid{origY} + $y);
		$pdf->stroke;
	}
}

#draw background color for map
#
sub drawbackground {
#	my @tmp = setstyles("map-background");
#	return if $tmp[6] eq "none" and $tmp[7] eq "none";
#	$pdf->rect($grid{origX},$grid{origY},$grid{width},
#		$grid{height});
#	if ($tmp[6] ne "none" and $tmp[7] ne "none") {
#		$pdf->fillstroke;
#	}elsif ($tmp[6] ne "none") {
#		$pdf->fill;
#	}else{
#		$pdf->stroke;
#	}
}

#draw exterior border for grid
#
sub drawborder {
	my @tmp = setstyles("map-border");
	$pdf->rect($grid{origX},$grid{origY},$grid{width},
		$grid{height});
	if ($tmp[6] ne "none" and $tmp[7] ne "none") {
		$pdf->fillstroke;
	}elsif ($tmp[6] ne "none") {
		$pdf->fill;
	}else{
		$pdf->stroke;
			}
}

#draw a single hex cell and optionally label it with its
#map coordinates (with offset for printing different subsectors/quadrants)
#
sub drawhex {
	my ($col,$row,$startC,$startR) = @_;
	my ($x,$y) = center($col,$row);

	my @tmp = setstyles("hex");
	my ($fill,$stroke) = @tmp[6,7];

	# do these before I change x, y, col, and row
	my $hex = sprintf("%02d%02d",$col + $startC,$row + $startR);
	next if $NoClutter > 1 and ! defined $SystemData{$hex};
	$pdf->move($x+$edgex[0],$y+$edgey[0]);
	for my $side (1..5) {
		$pdf->line($x+$edgex[$side],$y+$edgey[$side]);
	}
	$pdf->close;
	if ($fill ne "none" and $stroke ne "none") {
		$pdf->fillstroke;
	}elsif ($fill ne "none") {
		$pdf->fill;
	}else{
		$pdf->stroke;
	}
}

#find the center of a hex by row and column
#
sub center {
	my ($col,$row) = @_;
	my ($x,$y);
	$x = $grid{origX} + $col * $grid{edge} + ($col - 1) * $grid{sin};
	my $ytmp = 2 * ($grid{rows} - $row) + 1 + ($col % 2);
	$y = $grid{origY} + $ytmp * $grid{cos};
	return ($x,$y);
}

#figure out how to fit the desired grid onto the page,
#including font size for labels
#
sub gridsetup {
	my $cos30 = 0.866025403784439; # cos(deg2rad(30))
	my $sin30 = 0.5; # sin(deg2rad(30))

	#lower left corner of grid, in points (72dpi)
	$grid{origX} = 72 * $MarginX;
	$grid{origY} = 72 * $MarginY;

	#number of rows & cols in grid, plus number of divisions and
	#count of rows/cols in complete sector (for -a)
	#
	if ($AutoScale) {
		# arbitrary, based on SEC file
		$grid{cols} = $SectorBB[2] - $SectorBB[0] + 1;
		$grid{cols}++ if $grid{cols} % 2 == 1;
		$grid{rows} = $SectorBB[3] - $SectorBB[1] + 1;
		die "$0: autoscaling failed. No data in SEC file?\n"
			unless $grid{cols} > 0 and $grid{rows} > 0;
		$grid{divnum} = 1;
		$grid{type} = "Auto";
	}elsif ($GridType =~ /^s/i) {
		# sector
		$grid{cols} = 32;
		$grid{rows} = 40;
		$grid{divnum} = 4;
		$grid{type} = 'Sector';
	} elsif ($GridType =~ /^q/i) {
		# quadrant
		$grid{cols} = 16;
		$grid{rows} = 20;
		$grid{divnum} = 2;
		$grid{type} = 'Quadrant';
	} elsif ($GridType =~ /^(\d+)x(\d+)$/) {
		$grid{cols} = $1;
		if ($grid{cols} % 2 == 1) {
			warn "$0: number of columns in map must be even!\n";
			$grid{cols}++;
		}
		$grid{rows} = $2;
		$grid{divnum} = 1;
		$grid{type} = 'Custom';
	} else {
		# subsector
		$grid{cols} = 8;
		$grid{rows} = 10;
		$grid{divnum} = 1;
		$grid{type} = 'Subsector';
	}
	if ($grid{cols} > $grid{rows}
		and $paper{$PaperSize}{y} > $paper{$PaperSize}{x}) {
		warn "$0: wide map on narrow page! try --landscape\n";
	}

	$grid{height} = $paper{$PaperSize}{y} - $grid{origY} * 2;
	$grid{height} -= $grid{origY} if activestyle("map-text");
	$grid{edge} = $grid{height}/($cos30 * (2 * $grid{rows} + 1));
	$grid{width} = $grid{edge} * $grid{cols} * 1.5 + $grid{edge}/2;

	#make it fit on the page
	my $tmp = ($paper{$PaperSize}{x} - 2 * $grid{origX}) / $grid{width};
	if ($tmp < 1) {
		$grid{height} *= $tmp;
		$grid{edge} *= $tmp;
		$grid{width} *= $tmp;
	}
	$grid{cos} = $cos30 * $grid{edge};
	$grid{sin} = $sin30 * $grid{edge};

	#while we're here, set up the offsets of hex-edge endpoints
	$edgex[0] = -1 * $grid{sin};
	$edgey[0] = -1 * $grid{cos};
	$edgex[1] = -1 * $grid{edge};
	$edgey[1] = 0;
	$edgex[2] = -1 * $grid{sin};
	$edgey[2] = $grid{cos};
	$edgex[3] = $grid{sin};
	$edgey[3] = $grid{cos};
	$edgex[4] = $grid{edge};
	$edgey[4] = 0;
	$edgex[5] = $grid{sin};
	$edgey[5] = -1 * $grid{cos};

	# figure out starting hex for non-standard maps.
	if ($FirstHex =~ /^(\d\d)(\d\d)$/) {
		$FirstC = $1 - 1;
		if ($FirstC % 2 == 1) {
			warn "$0: --firsthex must start in odd-numbered column!\n";
			$FirstC--;
		}
		$FirstR = $2 - 1;
	}
}

#initialize the hash of paper sizes
#source:
#	http://partners.adobe.com/asn/developer/pdfs/tn/5003.PPD_Spec_v4.3.pdf
#
#
sub papersetup {
	%paper = (
		a0       => {x => 2384, y => 3370},
		a1       => {x => 1684, y => 2384},
		a2       => {x => 1191, y => 1684},
		a3       => {x =>  842, y => 1191},
		a4       => {x =>  595, y =>  842},
		a5       => {x =>  420, y =>  595},
		a6       => {x =>  297, y =>  420},
		b0       => {x => 2920, y => 4127},
		b1       => {x => 2064, y => 2920},
		b2       => {x => 1460, y => 2064},
		b3       => {x => 1032, y => 1460},
		b4       => {x =>  729, y => 1032},
		b5       => {x =>  516, y =>  729},
		b6       => {x =>  363, y =>  516},
		'36x48'  => {x => 2592, y => 3456},
		'24x36'  => {x => 1728, y => 2592},
		'18x24'  => {x => 1296, y => 1728},
		'11x17'  => {x =>  792, y => 1224},
		'8.5x14' => {x =>  612, y => 1008},
		'8.5x11' => {x =>  612, y =>  792},
		'Traveller' => {x =>  396, y =>  612},
		'4x6'    => {x =>  288, y =>  432},
	);
	#swap (x,y) if we're landscape-mode
	if ($_[0]) {
		foreach (values %paper) {
			my $tmp = $_->{x};
			$_->{x} = $_->{y};
			$_->{y} = $tmp;
		}
	}
	die "sec2pdf: unknown paper size '$PaperSize'\n"
		unless defined $paper{$PaperSize};
}

sub drawsystemdata {
	my ($oc,$or) = @_;
	#TODO: fix indentation (I wanted minimal diffs when testing...)
			styledcircle("jammy",0,1,1);
	foreach my $c (1..$grid{cols}) { foreach my $r (1..$grid{rows}) {
		my $hex = sprintf("%02d%02d",$c+$oc*$grid{cols},
			$r+$or*$grid{rows});
		my ($x,$y) = center($c,$r);

		if (defined $SystemData{$hex}) {
			styledtext("hex-label",0,$hex,$x,$y);
		}else{		
			styledtext("hex-label",0,$hex,$x,$y) unless $NoClutter;
			next;
		}

		my ($name,$xc,$xr,$uwp,$bases,$trade,$zone,$pbg,
			$ally,$stars) = split("\t",$SystemData{$hex});

		# mainworld
		if (substr($uwp,1,1) eq '0') {
#			# belt as mainworld
			my $tmprad = getstyle("system-belt-radius");
			my $tmppos = $grid{sin}/5;
			$pdf->circle($x,$y,$tmprad);
					$pdf->fill;
			$pdf->circle($x+$tmppos,$y+$tmppos,$tmprad*1.2);
					$pdf->fill;
			$pdf->circle($x-$tmppos/2,$y+$tmppos,$tmprad*0.9);
					$pdf->fill;
			$pdf->circle($x+$tmppos,$y-$tmppos,$tmprad*1.1);
					$pdf->fill;
			$pdf->circle($x-$tmppos,$y-$tmppos/2,$tmprad*0.8);
					$pdf->fill;
			$pdf->circle($x-$tmppos/3,$y-$tmppos,$tmprad*1.3);
				$pdf->fill;
#			otherstyledcircle("system-belt",0,$x-$tmppos,$y+$tmppos/3,$tmprad*0.7);
#					$pdf->fill;
		}elsif (substr($uwp,3,1) ne '0') {
			styledcircle("system-world-haswater",0,$x,$y);
		}else{
			styledcircle("system-world-nowater",0,$x,$y);
		}

		# zone
		styledarc("system-amber",0,$x,$y)
			if $zone eq "A";
		styledarc("system-red",0,$x,$y)
			if $zone eq "R";

		#system name
		$name = '' if $PrintUWP; # always print UWP+PBG
		if ($name && activestyle("system-name")) {
			my $tmp = "";
			if (substr($uwp,4,1) ge "9") {
				if ($DoCaps) {
					$name =~ tr/a-z/A-Z/;
				}else{
					$tmp = "-highpop";
				}
			}
			styledtext("system-name$tmp",0,$name,$x,$y);
			styledtext("system-uwp",0,$uwp,$x,$y);
		}else{
			styledtext("system-noname-uwp",0,$uwp,$x,$y);
			styledtext("system-noname-pbg",0,$pbg,$x,$y);
		}

		# starport
		styledtext("system-starport",0,substr($uwp,0,1),$x,$y);

		# bases
		# TODO: taking QLI's Gateway to Destiny as canon,
		# create standard icons for each type of base (or
		# just use the Symbol font...)
		# N - Navy, filled star
		# D - Navy Depot, filled square
		# S - Scout, filled triangle
		# W - Scout Way Station, triangle
		# A - N & S (per printed maps, text claims "B")
		# B - N & W (per printed maps)
		# R - Imperial Research Station (not used)
		# P - Prison Camp (not used)
		# (Spinward Marches seems to use G, M, and Z as well)
		styledtext("system-base",0,$bases,$x,$y)
			if ($bases) ;
#			if ($bases = "S") ne chr"85" ;
	
		# gas giants
		styledcircle("system-gasgiant",0,$x,$y)
			if substr($pbg,2,1) ne "0";

		# allegiance - skip blank
		styledtext("system-ally",0,$ally,$x,$y)
			if $ally and $ally ne "Xx";
	}}
}




# slurp in a file of keyword/value pairs that flesh out the
# map beyond the level of what's legal in SEC format.
# NOTE: domain, alpha, beta, gamma, and delta don't print yet,
# and ally, base, border, region, and route don't even exist in the code.
#
# Valid keys and their values:
#	domain - the name of the domain this sector is part of
#	sector - the name of this sector
#	alpha, beta, gamma, delta - the names of the four quadrants
#	a ... p - the names of the sixteen subsectors
#	NNNN - the complete name of the system in hex NNNN (SEC 14-char limit)
#	ally - two-letter allegiance code and full name
#		ally As Aslan
#		ally Dr Droyne
#		ally Im Imperial
#		ally Na Non-aligned
#	base - base code, ZapfDingbats ASCII code, color, and full name
#		base N 43 black Navy Base
#		base D 81 red Navy Depot
#		base S 86 darkgreen Scout Base
#		base W 89 darkgray Way Station
#		base A NS
#		base B NW
#		base R - - Research Station
#		base P - - Prison Camp
#	border - series of hexes that should have a border drawn around them,
#		with an optional color at the end of the list. The list
#		*must* be generated by the following procedure:
#
#		Draw a line through each hex inside the region that is
#		along the outer border, starting at the lowest-
#		numbered hex in the lowest-numbered column and going
#		clockwise around the region until you're back at the
#		start, recording the hex numbers as you go, including
#		duplicates. The list will always end with the starting
#		hex.
#	region - same as a border, but filled with color instead.
#	route - for drawing trade/xboat routes between systems. Format is:
#		route [n n] AAAA [n n] BBBB [color]
#		route 0101 0306 (short route, same sector, default color)
#		route 0212 -1 0 3214 red (short route out-sector to left, red)
#		route -1 -1 3240 1 1 0101 lime (huge out-sector route, silly)
#	label - for adding arbitrary labels on the map, centered in a hex
#		label 0406 Here There Be Dragons
#
sub readmetafile {
	open(In,$_[0]) or die "$0: $_[0]: $!\n";
	my ($key,$value);
	while (<In>) {
		next if /^\s*$/;
		next if /^\s*#/;
		chomp;
		tr/\015//d;
		if (/^\s+/) {
			#indented? must be a continuation of last line
			#(valid only for regions and borders)
			s/^\s+//;
			if ($key eq "REGION") {
				$filledregion[$#filledregion] .= " $_";
			}elsif ($key eq "BORDER") {
				$strokedregion[$#strokedregion] .= " $_";
			}
			next;
		}
		($key,$value) = split(' ',$_,2);
		$key =~ tr/a-z/A-Z/;
		if ($key eq "ALLY") {
			push(@ally,$value);
		}elsif ($key eq "BASE") {
			push(@base,$value);
		}elsif ($key eq "REGION") {
			push(@filledregion,$value);
		}elsif ($key eq "BORDER") {
			push(@strokedregion,$value);
		}elsif ($key eq "ROUTE") {
			push(@route,$value);
		}elsif ($key eq "LABEL") {
			push(@label,$value);
		}else{
			$meta{$key} = $value;
		}
	}
	close(In);
	if ($meta{SECTOR} && $SectorName eq '') {
		$SectorName = $meta{SECTOR};
	}
}

#clean up the top-level flow by burying a bunch of initialization
#way down here...
#
sub mainsetup {
	GetOptions("all|a" => \$PrintAll,"border|b" => \$PrintBorder,
		"boundaries|B" => \$PrintDivisions, "type|t=s" => \$GridType,
		"paper|p=s" => \$PaperSize, "data|d=s" => \@SectorFile,
		"sector|s=s" => \$SectorName, "output|o=s" => \$OutputFile,
		"DEBUG|D+" => \$DEBUG, "metadata|m=s" => \@MetaFile,
		"uwpprint|u" => \$PrintUWP, "highlight|h" => \$DoCaps,
		"cleanup|c+" => \$NoClutter, "grayscale|g" => \$Grayscale,
		"landscape|l" => \$DoLandscape,"firsthex|f=s" => \$FirstHex,
		"auto|A" => \$AutoScale, "style|S=s" => \$StyleFile,
		"multisector|M" => \$MultiSector)
			or die <<EOF;
usage: sec2pdf [-aAbBcghlrRu] [-t sector/quadrant] [-p papersize] [-s secname]
               [-f NNNN] [-d datafile] [-m metafile] [-o outputfile] [col row]
        -a generate entire sector of quadrant/subsector pages
        -A autoscale single-page map based on data in SEC file
        -b print border around edge of each map
        -B print subsector boundaries on sector/quadrant maps
        -c cleanup map (leaves out empty hex labels; "-c -c" for really clean)
        -d data file in standard SEC format
        -f hex to start the map in, column must be odd. Not compatible with -a.
        -g print in grayscale instead of color
        -h highlight hi-pop worlds with the traditional CAPS instead of bold
        -l rotate the paper to landscape (wide) mode
        -m file containing metadata (sector & subsector names, year, longnames)
        -M multi-sector map; draw all sectors on same page. Currently
           limited to exactly 4 sectors at sector level (domain map).
        -o outputfile (.pdf appended if not present)
        -p paper size (4x6,8.5x11,8.5x14,11x17,18x24,24x36,36x48,a0-a6,b0-b6)
        -s name of sector (map title)
	-S file containing map style definitions
        -t what type of map to print (subsector is default). Can now be an
           arbitrary grid size ("-t 4x7"), number of columns must be even.
           Doesn't work well with -a.
        -u always print UWP+PBG instead of system name

        [col row] selects a specific quadrant/subsector to print
        (top left is "1 1", the default)

        metadata file format is simple keyword/value, one per line:
            "sector foo","year 993","a subname" ... "p subname",
            "2135 Cantrell's Grant" (for names longer than 14 characters)
EOF
	if ($ARGV[1]) {
		$SectionC = shift(@ARGV) - 1;
		$SectionR = shift(@ARGV) - 1;
	}
	die "$0: -A and -s are incompatible with multi-sector printing\n"
		if $#SectorFile > 0 and ($AutoScale or $SectorName);
	foreach my $i (0..$#SectorFile) {
		die "$0: can't find sector data file '$SectorFile[$i]'\n"
			unless -r $SectorFile[$i];
		my $tmp = $SectorFile[$i];
		if (! $MetaFile[$i]) {
			$tmp =~ s/\.sec$/.msec/;
			$MetaFile[$i] = $tmp if -r $tmp;
		}
	}
	$OutputFile .= ".pdf" unless $OutputFile =~ /\.pdf$/i;

	# set default layout styles
	$StyleFile = <<'EOF' unless defined $StyleFile;
#
# default style file for sec2pdf
#
# How does this work?
#     1) There are 8 different types of data currently used
#        (pos, fill, stroke, line, font, size, align, and radius).
#     2) All the drawing routines call setstyles($stylename,$dontscale),
#        which grovels over the data in this file, looking for a match.
#     3) If $dontscale is 1, then pos, line, size, and radius will be
#        measured in points (1/72 inch), otherwise they'll be scaled to
#        be percentages of the radius of a hex, with (0,0) at the center
#        of the hex. A circle of radius 100 would then touch all six
#        vertices.
#     4) setstyles() searches through the style's "parents" to implement
#        simple inheritance.
#     5) If it can't find a matching value, it falls back to default-$type
#     6) A value of "none" is valid for fill and stroke.
#     7) A special value of "off" means that printing of this data
#        should be suppressed. So, if you wanted to turn off the
#        printing of UWP above system name, "system-uwp off" would
#        do it. Note that "system off" will turn off everything under
#        system-*.
#
#     For example, if the style name is "system-name-highpop-font",
#     the first matching key in the following list will be used:
#        system-name-highpop-font
#        system-name-font
#        system-font
#        default-font
# 
default-pos 0 0
default-fill white
default-stroke black
default-line 2.5
default-font Univers
default-size 32
default-align center
default-radius 20

#these should be unscaled
#(not fully integrated in the code yet)
map-text-fill black
map-text-sector-align left
map-text-section-align right

map-dividers-stroke none
map-dividers-line off

map-border-stroke none
map-border-fill none
map-border-line 0

map-background off
#-fill off
map-background-stroke none

map-route-line 20
map-route-stroke gray

map-region-line 12
map-region-filled-fill lightgray
map-region-filled-stroke none
map-region-stroked-fill none
map-region-stroked-stroke silver

#this feature is currently a bit fragile in the code, so all of
#the keys *must* be present, and the first two must *not* be
#changed.
#
map-label-align center
map-label-pos 0 12
map-label-fill darkgray
map-label-large-size 80
map-label-size 65
map-label-small-size 50

hex-line 1
hex-fill none
hex-stroke 0.3
hex-label-font Univers
hex-label-fill black
hex-label-size 23
hex-label-pos 0 70

system-fill black
system-size 18

system-name-pos 0 -64
system-name-size 32
system-name-fill black

#sample of how to disable printing specific data
system-uwp off
#system-uwp-font Univers
#system-uwp-size 25
#system-uwp-pos 0 -65

#used when there's no name for a system
system-noname-font Univers
system-noname-size 18
system-noname-uwp-pos 0 -67
system-noname-pbg-pos 0 -93

system-world-radius 18
system-world-stroke 1
system-world-haswater-fill black
system-world-haswater-stroke black
system-world-nowater-fill white
system-world-nowater-stroke black

jammy-radius .1
jammy-stroke none
jammy-fill none
jammy-line 0

system-belt-fill black
system-belt-stroke none
system-belt-radius 3.5
system-belt-line 0

system-base-font Univers-Bold
system-base-pos -26 26
system-base-align right

system-starport-pos 0 35
system-starport-size 35
system-starport-font Univers-Bold

system-gasgiant-pos 36 36
system-gasgiant-stroke 0
system-gasgiant-radius 6

system-ally-size 1
system-ally-align right
system-ally-pos 85 -43

system-amber-radius 75
system-amber-stroke gray
system-amber-fill none
system-amber-line 3

system-red-radius 75
system-red-stroke black
system-red-fill none
system-red-line 3
EOF
}

sub readstyles {
	my($styles) = @_;
	my @In;
	if (-f $styles) {
		open(In,$styles) or die "$0: $styles: $!\n";
		@In = <In>;
		close(In);
	}else{
		@In = split(/\n/,$styles);
	}
	foreach (@In) {
		next if /^\s*$/;
		next if /^\s*#/;
		my($key,$value) = split(' ',$_,2);
		$key =~ tr/A-Z/a-z/;
		chomp($Style{$key} = $value);
	}
}

#scan through all of the parents of a style declaration, looking
#to see if printing has been turned off at that level
#
sub activestyle {
	my ($style) = @_;
	my @c = split(/-/,$style);
	my @p = ();
	foreach (@c) {
		push(@p,$_);
		return 0 if $Style{join("-",@p)} =~ /^off$/i;
	}
	return 1;
}

#return a single raw style, without inheritance
#
sub getstyle {
	my ($style,$dontscale) = @_;
	my $scale;
	if (defined ($Style{scale})) {
		$scale = $Style{scale};
	}else{
		#figure out the scaling factor
		my $cos30 = 0.866025403784439; # cos(deg2rad(30))
		$scale = $dontscale ? 1 : $cos30 * $grid{edge} / 100;
	}

	return defined($Style{$style}) ? $Style{$style} * $scale : undef;
}

# setstyles walks the style tree for the requested style,
# applying values in order. Returns (x,y,radius,font,size,align), the
# ones whose usage will vary depending on what kind of object
# you're drawing. align: 0=left,1=center,2=right
#
# TODO: allow additional arguments to override styles, like so:
# setstyles($style,$dontscale,$dontsave, fill => "blue", align => "center");
# All the styledfoo() functions should also accept these arguments
# and pass them through. this will drastically simplify the code for
# drawlabels() and others. It also just makes sense.
#
sub setstyles {
	my($style,$dontscale,$dontsave) = @_;
	my @c = split(/-/,$style);
	my @tc = ();
	no strict "refs";

	my $scale;
	if (defined ($Style{scale})) {
		$scale = $Style{scale};
	}else{
		#figure out the scaling factor
		my $cos30 = 0.866025403784439; # cos(deg2rad(30))
		$scale = $dontscale ? 1 : $cos30 * $grid{edge} / 100;
	}

	#computed style
	my %CS = (); 

	#read in defaults
	foreach (qw/fill stroke line radius font size align/) {
		$CS{$_} = $Style{"default-$_"} if defined $Style{"default-$_"};
	}
	if (defined $Style{"default-pos"}) {
		($CS{x},$CS{y}) = split(' ',$Style{"default-pos"});
	}

	#inherit styles from parents
	foreach (@c) {
		push(@tc,$_);
		my $tmp = join("-",@tc);
		foreach (qw/fill stroke line radius font size align/) {
			$CS{$_} = $Style{"$tmp-$_"}
				if defined $Style{"$tmp-$_"};
		}
		if (defined $Style{"$tmp-pos"}) {
			($CS{x},$CS{y}) = split(' ',$Style{"$tmp-pos"});
		}
	}

	#print computed style
	print <<EOF if $DEBUG > 1;
	COMPUTED STYLE($style) [scale=$scale]
	fill $CS{fill}, stroke $CS{stroke}, line $CS{line},
	radius $CS{radius}, font $CS{font}, size $CS{size},
	align $CS{align}, pos $CS{x} $CS{y}
EOF

	#apply (fill,stroke,line) that differ from the current state
	#HACK: ignore state if $dontsave is set. Why not? Because
	#it means that we're being called from inside of a clipping
	#region, and that has to be wrapped in a savestate/restorestate
	#pair.
	#
	if (!$dontsave) {
		$pdf->fillcolor($CS{fill}) unless $CS{fill} eq "none";
		$pdf->strokecolor($CS{stroke}) unless $CS{stroke} eq "none";
		$pdf->linewidth($CS{line} * $scale);
	}else{
		if ($CS{fill} ne "none"
			and $CS{fill} ne $Style{"current-fill"}) {
			$pdf->fillcolor($CS{fill});
			$Style{"current-fill"} = $CS{fill};
		}
		if ($CS{stroke} ne "none"
			and $CS{stroke} ne $Style{"current-stroke"}) {
			$pdf->strokecolor($CS{stroke});
			$Style{"current-stroke"} = $CS{stroke};
		}
		if ($CS{line} ne $Style{"current-line"}) {
			$pdf->linewidth($CS{line} * $scale);
			$Style{"current-line"} = $CS{line};
		}
	}

	if ($CS{align} eq "center") {
		$CS{align} = 1;
	}elsif ($CS{align} eq "right") {
		$CS{align} = 2;
	}else{
		$CS{align} = 0;
	}
	return ($CS{x}*$scale,$CS{y}*$scale,$CS{radius}*$scale,$CS{font},
		$CS{size}*$scale,$CS{align},$CS{fill},$CS{stroke});
}

sub styledtext {
	my ($style,$dontscale,$text,$x,$y) = @_;
	return unless activestyle($style);
	my ($sx,$sy,$radius,$font,$size,$align) = setstyles($style,
		$dontscale);
	if (! defined($StyleFont{$font})) {
		$StyleFont{$font} = $pdf->corefont($font);
		die "$0: core font '$font' not found\n"
			unless defined $StyleFont{$font};
	}
	$pdf->print($StyleFont{$font},$size,$x+$sx,$y+$sy,0,$align,$text);
}

sub styledcircle {
	my ($style,$dontscale,$x,$y,$r) = @_;
	return unless activestyle($style);
	my ($sx,$sy,$radius,$font,$size,$align,
		$fill,$stroke) = setstyles($style,$dontscale);
	$pdf->circle($x+$sx,$y+$sy,$r || $radius);
	if ($fill ne "none" and $stroke ne "none") {
		$pdf->fillstroke;
	}elsif ($fill ne "none") {
		$pdf->fill;
	}else{
		$pdf->stroke;
	}
}

sub otherstyledcircle {
	my ($style,$dontscale,$x,$y,$r) = @_;
	return unless activestyle($style);
	my ($sx,$sy,$radius,$font,$size,$align,
		$fill,$stroke) = setstyles($style,$dontscale);
	$pdf->circle($x+$sx,$y+$sy,$r || $radius);
	if ($fill ne "none" and $stroke ne "none") {
		$pdf->stroke;
	}elsif ($fill ne "none") {
		$pdf->stroke;
	}else{
		$pdf->stroke;
	}
}
sub styledarc {
	my ($style,$dontscale,$x,$y) = @_;
	return unless activestyle($style);
	my ($sx,$sy,$radius,$font,$size,$align,
		$fill,$stroke) = setstyles($style,$dontscale);
	$pdf->arc($x+$sx,$y+$sy,$radius,$radius,200,-20,1);
	if ($fill ne "none" and $stroke ne "none") {
		$pdf->stroke;
	}elsif ($fill ne "none") {
		$pdf->stroke;
	}else{
		$pdf->stroke;
	}
}

sub readsectorfile {
	open(In,$_[0]) or die "$0: $_[0]: $!\n";
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

		#compute data bounding box
		$SectorBB[0] = $c if $c < $SectorBB[0];
		$SectorBB[1] = $r if $r < $SectorBB[1];
		$SectorBB[2] = $c if $c > $SectorBB[2];
		$SectorBB[3] = $r if $r > $SectorBB[3];
		#substitute full name for short version
		$name = $meta{$c.$r} if $meta{$c.$r};
		$SystemData{$c.$r} = join("\t",$name,$c,$r,$uwp,$bases,
			$trade,$zone,$pbg,$ally,$stars);
	}
	close(In);
	#adjust data bounding box to print correctly
	$SectorBB[0]-- if $SectorBB[0] % 2 == 0;
	$FirstHex = sprintf("%02d%02d",$SectorBB[0],$SectorBB[1])
		if $AutoScale;
}

sub drawlabels {
	my ($oc,$or) = @_;

	#"label hex[,option...] string"
	#options are: high,low,left,right,subsec,quadrant,sector,custom,
	#large,small,(colorname)
	foreach (@label) {
		my ($c,$r,$options,$text) = /^(..)(..)[,\/]?([\S]+)?\s+(.*)$/;

		# adjust endpoints to be relative to the current page
		$c -= $grid{cols} * $oc;
		$r -= $grid{rows} * $or;

		# skip labels that aren't on this page
		next if $c > $grid{cols} or $c < 1;
		next if $r > $grid{rows} or $r < 1;

		# find the coordinates to print the string at
		my ($x,$y) = center($c,$r);

		my $align = undef;
		my $size = undef;
		my $color = undef;
		my $dontprint = 0;
		foreach (split(/,/,$options)) {
			if (/^low$/i) {
				$y -= $grid{edge} * 0.85;
			}elsif (/^\+(\d+)$/i) {
				$y += $grid{edge} * $1 / 100;
			}elsif (/^-(\d+)$/i) {
				$y -= $grid{edge} * $1 / 100;
			}elsif (/^left$/i) {
				$align="left";
				$x -= $grid{edge} * 0.48;
			}elsif (/^right$/i) {
				$align="right";
				$x += $grid{edge} * 0.48;
			}elsif (/^large$/i) {
				$size = "large";
			}elsif (/^small$/i) {
				$size = "small";
			}elsif (/^subsec/i) {
				$dontprint++ unless $grid{type} eq 'Subsector';
			}elsif (/^quad/i) {
				$dontprint++ unless $grid{type} eq 'Quadrant';
			}elsif (/^sect/i) {
				$dontprint++ unless $grid{type} eq 'Sector';
			}elsif (/^custom/i) {
				$dontprint++ unless $grid{type} eq 'Custom';
			}elsif (length($_) > 0) {
				$color = $_;
			}
		}
		unless ($dontprint) {
			#TODO: see the todo note in setstyles()
			my $tmpcolor = $Style{"map-label-fill"};
			my $tmpsize = $Style{"map-label-size"};
			if (defined $color and ! $Grayscale) {
				$Style{"map-label-fill"} = $color;
			}
			$Style{"map-label-align"} = $align if $align;
			$Style{"map-label-size"} =
				$Style{"map-label-$size-size"} if $size;
			styledtext("map-label",0,$text,$x,$y);
			$Style{"map-label-fill"} = $tmpcolor;
			$Style{"map-label-align"} = "center";
			$Style{"map-label-size"} = $tmpsize;
		}
	}
}

#data format is [n n] fromhex [n n] tohex [color]
sub drawroutes {
	#TODO: cache this to make it less expensive, since
	#we scan the route list N times
	my ($oc,$or) = @_;

	#clip to page (must save graphics state here!)
	$pdf->savestate;
	$pdf->rectxy($grid{origX},$grid{origY},$grid{origX} + $grid{width},
		$grid{origY} + $grid{height});
#	$pdf->{$cliphack}->clip;
#	$pdf->{$cliphack}->endpath;
			styledcircle("jammy",0,1,1);

	foreach (@route) {
		my @tmp = split;
		my $r1oc = shift(@tmp) if $tmp[0] =~ /^[-+]?[01]$/;
		my $r1or = shift(@tmp) if $tmp[0] =~ /^[-+]?[01]$/;
		my $r1 = shift(@tmp);
		my $r1c = substr($r1,0,2) + $r1oc * 32;
		my $r1r = substr($r1,2,2) + $r1or * 40; 
		my $r2oc = shift(@tmp) if $tmp[0] =~ /^[-+]?[01]$/;
		my $r2or = shift(@tmp) if $tmp[0] =~ /^[-+]?[01]$/;
		my $r2 = shift(@tmp);
		my $r2c = substr($r2,0,2) + $r2oc * 32;
		my $r2r = substr($r2,2,2) + $r2or * 40; 
		my $color = shift(@tmp); #optional

		# adjust endpoints to be relative to the current page
		$r1c -= $grid{cols} * $oc;
		$r1r -= $grid{rows} * $or;
		$r2c -= $grid{cols} * $oc;
		$r2r -= $grid{rows} * $or;

		# skip routes that could never cross this page
		next if $r1c > $grid{cols}+1 and $r2c > $grid{cols}+1;
		next if $r1c < 0 and $r2c < 0;
		next if $r1r > $grid{rows}+1 and $r2r > $grid{rows}+1;
		next if $r1r < 0 and $r2r < 0;

		# find the coordinates of the endpoint hexes
		my ($x1,$y1) = center($r1c,$r1r);
		my ($x2,$y2) = center($r2c,$r2r);

		if (defined $color and ! $Grayscale) {
			my $tmp = $Style{"map-route-stroke"};
			$Style{"map-route-stroke"} = $color;
			setstyles("map-route",0,1);
			$Style{"map-route-stroke"} = $tmp;
		}else{
			setstyles("map-route",0,1);
		}
		$pdf->move($x1,$y1);
		$pdf->line($x2,$y2);
		$pdf->stroke;
	}
	$pdf->restorestate;
}


# How regions work: The Gory Details
#
# Filled and stroked regions are exactly the same thing, a closed
# path that runs along the edge of a bunch of hexes. The only
# differences are what you set your styles to and whether you call
# $pdf->fill or $pdf->stroke at the end.
#
# With that out of the way, how do we know where to draw the path
# to make it continuous and closed?
#
# First, tell the user *precisely* how to describe a region:
#	Draw a line through each hex inside the region that is along the
#	outer border, starting at the lowest-numbered hex in the
#	lowest-numbered column and going clockwise around the region until
#	you're back at the start, recording the hex numbers as you go,
#	including duplicates. The list will always end with the starting
#	hex.
#
# This procedure gives us a guaranteed starting place for the path.
# By definition, the hexes to the Lower Left (0), Upper Left (1), and
# Above (2) the starting hex cannot be inside the region, so we can
# always draw along those edges, in that order. The next border hex
# must be one of Upper Right (3), Lower Right (4), or Under (5).
#
# If the next border hex is Upper Right (3), then from its point of
# view, the hex in Lower Left (0) is guaranteed to be inside, and
# it should begin checking at Upper Left (1), and draw around clockwise
# until it finds another border hex.
#
# This is a lot easier to follow with pictures, so I'll have to draw
# one and distribute it with the script. Users should be able to get
# by with the text instructions, fortunately. :-)
#
# The "neighbor numbers" work out nicely, such that to your neighbor
# in N, you are the neighbor in (N + 3) % 6, and he should test his
# neighbors starting with (N + 4) % 6, in increasing order until he
# hits his first neighbor (which will always be the next hex on the
# list).
#
# For the starting hex only, we have to keep a bit of extra state,
# to keep the path from overlapping itself. If we're not visiting
# it for the first time, we will stop testing neighbors at (5), no
# matter where we start, because (0) has already been drawn.

sub drawregions {
	#TODO: cache this to make it less expensive
	my ($oc,$or,$type) = @_;

	#clip to page (must save graphics state here!)
	$pdf->savestate;
	$pdf->rectxy($grid{origX},$grid{origY},$grid{origX} + $grid{width},
		$grid{origY} + $grid{height});
#	$pdf->{$cliphack}->clip;
#	$pdf->{$cliphack}->endpath;
			styledcircle("jammy",0,1,1);
			styledcircle("map-region-$type",0,1,1);

	foreach ($type eq "stroked" ? @strokedregion : @filledregion) {
		my (@c,@r) = ();
		my @tmp = split;
		my $color = ($tmp[$#tmp] =~ /^\d{4}$/) ? undef: pop(@tmp);
		my ($bbc1,$bbc2,$bbr1,$bbr2) = (999,-999,999,-999);
		my %onborder = ();
		foreach (@tmp) {
			my ($c,$r) = /^(\d\d)(\d\d)$/;

			#relativize position to this page
			$c -= $grid{cols} * $oc;
			$r -= $grid{rows} * $or;

			$onborder{$c,$r}=1;
			push(@c,$c);
			push(@r,$r);

			#track bounding box
			$bbc1 = $c if $c < $bbc1;
			$bbr1 = $r if $r < $bbr1;
			$bbc2 = $c if $c > $bbc2;
			$bbr2 = $r if $r > $bbr2;
		}
		#TODO: skip if the region doesn't cross this page

		my $checkfirst = 0;
		my $checklast = 5;
		my ($c1,$r1) = ();
		for my $i (0..$#c) {
			my ($c,$r) = ($c[$i],$r[$i]);
			$checklast = $checkfirst + 5;
			if (defined $c1 and $c1 == $c and $r1 == $r) {
				#I'm in the starting hex, and I've been
				#there before, so stop testing at neighbor
				#5, no matter what
				$checklast = 5;
				#degenerate case, entering for third time
				last if $checkfirst < 3;
			}elsif (! defined $c1) {
				#I'm in the starting hex for the first time
				$c1 = $c;
				$r1 = $r;
				my ($x,$y) = center($c,$r);
				$pdf->move($x+$edgex[0],$y+$edgey[0]);
			}

			my ($x,$y) = center($c,$r);
			my $i;
			foreach ($checkfirst..$checklast) {
				$i = $_;
				my ($nc,$nr) = hexneighbor($c,$r,$i % 6);
				last if $onborder{$nc,$nr};
				$pdf->line($x+$edgex[($i+1) % 6],$y + $edgey[($i + 1)%6]);
			}
			$i %= 6;
			# $i is the direction to the next border hex,
			# and when we get there, we'll have come from
			# $i + 3, so we start checking with $i + 4.
			$checkfirst = ($i + 4) % 6;
		}
		if (defined $color and ! $Grayscale) {
			my $saved = $type eq "stroked" ? "stroke" : "fill";
			my $tmp = $Style{"map-region-$type-$saved"};
			$Style{"map-region-$type-$saved"} = $color;
			setstyles("map-region-$type",0,1);
			$Style{"map-region-$type-$saved"} = $tmp;
		}else{
			setstyles("map-region-$type",0,1);
		}
		$pdf->close;
		if ($type eq "stroked") {
			$pdf->stroke;
		}else{
			$pdf->fill;
		}
	}
	$pdf->restorestate;
}

#return col,row of a neighboring hex (0..5, starting LL and going clockwise)
sub hexneighbor {
	my ($c,$r,$num) = @_;
	if ($num == 0) {
		$r += 1 - $c-- % 2;
	}elsif ($num == 1) {
		$r -= $c-- % 2;
	}elsif ($num == 2) {
		$r--;
	}elsif ($num == 3) {
		$r -= $c++ % 2;
	}elsif ($num == 4) {
		$r += 1 - $c++ % 2;
	}elsif ($num == 5) {
		$r++;
	}
	return ($c,$r);
}

sub grayscalehack {
	#ditch the colors and make things stand out more
	#TODO: figure out how to gray-ize the route colors
#	$Style{"system-amber-stroke"} = 0.5;
#	$Style{"system-red-stroke"} = $Style{"default-stroke"};
#	$Style{"system-world-haswater-fill"} = $Style{"hex-label-fill"};
#	$Style{"system-world-nowater-fill"} = $Style{"map-background-fill"};
#	$Style{"system-world-nowater-stroke"} = $Style{"default-stroke"};
#	$Style{"system-world-nowater-line"} = "5";
#	$Style{"system-world-nowater-radius"} = "30";
#	$Style{"map-label-fill"} = "black";
	#mostly-vain attempt to make -g work for white-on-black maps...
	#TODO: make this less of a gross hack
#	$Style{"map-route-stroke"} = "slategray"
#		unless $Style{"map-route-stroke"} =~ /^(.*gr[ae]y|[0-9.])$/;
#	$Style{"map-region-stroked-stroke"} = "darkslategray"
#		unless $Style{"map-region-stroked-stroke"} =~ /^(.*gr[ae]y|[0-9.])$/;
#	$Style{"map-region-filled-fill"} = "lightgray"
#		unless $Style{"map-region-filled-fill"} =~ /^(.*gr[ae]y|[0-9.])$/;
}

sub completepage {
	my ($C,$R) = @_;
	drawbackground();
	drawregions($C,$R,"filled")
		if @filledregion;
	drawregions($C,$R,"stroked")
		if @strokedregion;
	drawroutes($C,$R);
	drawlabels($C,$R);
	drawsystemdata($C,$R) if keys %SystemData > 0;
	drawpage($C,$R);
	# draw this last so it ends up on top...
}
