package Traveller::Animal;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Animal.pm
#	author:			paul sanford toney <signalgk@gmail.com>
#
#	description:	
#
# 		The Traveller game in all forms is owned by Far Future
# 	Enterprises. Copyright 1977 - 2005 Far Future Enterprises. Traveller
# 	is a registered trademark of Far Future Enterprises. Far Future
# 	permits web sites and fanzines for this game, provided it contains
# 	this notice, that Far Future is notified, and subject to a
# 	withdrawal of permission on 90 days notice. The contents of this
# 	site are for personal, non-commercial use only. Any use of Far
# 	Future Enterprises's copyrighted material or trademarks anywhere on
# 	this web site and its files should not be viewed as a challenge to
# 	those copyrights or trademarks. In addition, any
# 	program/articles/file on this site cannot be republished or
# 	distributed without the consent of the author who contributed it.
#
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use strict;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my %TBLTERRAIN;
my %TBLTYPE;
my %TBLATTRIBUTE;
my %TBLSIZE;
my %TBLBEHAVIOR;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub generate {
	my $self = shift;
	my %arg = (
		TERRAIN			=> 'clear',
		WORLD			=> undef,
		SIZE			=> undef,
		ATMO			=> undef,
		EXTENDED		=> undef,
		@_
	);

	my $size = $arg{SIZE} ? $arg{SIZE} : 0;
	$size = $arg{WORLD} ? $arg{WORLD}->{ATTR}{SIZE} : $size;

	my $atmo = $arg{ATMO} ? $arg{ATMO} : 0;
	$atmo = $arg{WORLD} ? $arg{WORLD}->{ATTR}{ATMO} : $atmo;

	my $hydr = $arg{HYDR} ? $arg{HYDR} : 0;
	$hydr = $arg{WORLD} ? $arg{WORLD}->{ATTR}{HYDR} : $hydr;

# 		if ($arg{WORLD}) {
# 		$arg{TERRAIN} = 'desert world' if $hydr ==  0;
# 		$arg{TERRAIN} = 'water world'  if $hydr == 10;
# 		}

 		if ($arg{WORLD}) {

# 		$arg{TERRAIN} = 'land' if $hydr < 3;
# 		$arg{TERRAIN} = 'all'  if $hydr > 2 && $hydr < 10;
# 		$arg{TERRAIN} = 'desert world' if $hydr ==  0;
# 		$arg{TERRAIN} = 'water world'  if $hydr == 10;
# 		$arg{TERRAIN} = 'underground' if $atmo <  2;
#

 		$arg{TERRAIN} = 'land' if $hydr < 5;
 		$arg{TERRAIN} = 'water'  if $hydr > 7 && $hydr < 10;
 		$arg{TERRAIN} = 'all'  if $hydr > 4 && $hydr < 8;
 		$arg{TERRAIN} = 'water world'  if $hydr == 10;
 		$arg{TERRAIN} = 'desert world' if $hydr ==  0;
 		$arg{TERRAIN} = 'underground' if $atmo <  2;
 		}
	$arg{TERRAIN} = lc($arg{TERRAIN});

#	if ($arg{TERRAIN} eq 'all') {
#		$arg{TERRAIN} = join(',', keys(%TBLTERRAIN));
#	}
#
#	if ($arg{TERRAIN} eq 'land') {
#		$arg{TERRAIN} = 'broken,clear,desert,forest,jungle,mountain,prairie,rough,ruins';
#	}
#
#	if ($arg{TERRAIN} eq 'water') {
#		$arg{TERRAIN} = 'beach,bottom,depths,marsh,river,sargasso,seacave,shallows,surface,swamp';
#	}
#
#	if ($arg{TERRAIN} eq 'underground') {
#		$arg{TERRAIN} = 'cave,chasm,crater';
#	}
#
#	if ($arg{TERRAIN} eq 'water world') {
#		$arg{TERRAIN} = 'beach,bottom,depths,sargasso,seacave,shallows,surface';
#	}
#
#	if ($arg{TERRAIN} eq 'desert world') {
#		$arg{TERRAIN} = 'broken,cave,chasm,clear,crater,desert,mountain,rough,ruins';
#	}
#


	if ($arg{TERRAIN} eq 'all') {
		$arg{TERRAIN} = join(',', keys(%TBLTERRAIN));
	}

	if ($arg{TERRAIN} eq 'land') {
		$arg{TERRAIN} = 'clear,desert,forest,jungle,mountain,prairie,rough';
	}

	if ($arg{TERRAIN} eq 'water') {
		$arg{TERRAIN} = 'bottom,depths,marsh,river,sargasso,seacave,shallows,surface,swamp';
	}

	if ($arg{TERRAIN} eq 'underground') {
		$arg{TERRAIN} = 'cave,chasm,crater';
	}

	if ($arg{TERRAIN} eq 'water world') {
		$arg{TERRAIN} = 'bottom,depths,sargasso,seacave,shallows,surface';
	}

	if ($arg{TERRAIN} eq 'desert world') {
		$arg{TERRAIN} = 'broken,cave,chasm,clear,crater,desert,mountain,rough';
	}


	my $area = $arg{WORLD} ? $arg{WORLD}->name . ' ' . $arg{WORLD}->profile : '';

	my $result = '';

	foreach my $t (split /,/, lc($arg{TERRAIN})) {

		$^A = '';
formline <<EO, "\u$t", $area;
TERRAIN: @<<<<<<<<<<<<<<<<<<<<<<<              @>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
Die Animal                Weight   Hits  Armor      Wounds & Weapons      Action
EO
		$result .= $^A;
		$^A = '';

		my $n = $arg{EXTENDED} ? 1 : 0;
		foreach my $m ($arg{EXTENDED} ? qw(S O S O H H H C E C C) : qw(S H H H O C)) {
			$result .= _generate_one(TERRAIN => $t, ROLL => $n += 1, KIND => $m, SIZE => $size, ATMO => $atmo);
		}
		
		$result .= "\n";
	}

	return $result;
}

sub _generate_one {
	my %arg = @_;
	my $terrain		= $arg{TERRAIN};
	my $roll		= $arg{ROLL}      || 1;
	my $kind		= $arg{KIND};
	my $size		= hex($arg{SIZE}) || 0;
	my $atmo		= hex($arg{ATMO}) || 0;

	if ($kind eq 'E') {
		return sprintf(" %2d  Special event to be administered by the referee.\n", $roll);
	}

	my $type_dm		= 0;
	my $size_dm_1	= 0;

	($type_dm, $size_dm_1) = split(/,/, $TBLTERRAIN{$terrain});
	my $type = $TBLTYPE{$kind}[Traveller::Dice::roll(2, $type_dm, 0, 13)];
	my $number = 1;

	if ($type =~ /([0-9]),/) {
		$number = Traveller::Dice::roll($1);
		$type =~ s/[0-9],//;
	}

	my $attr_dm = 0;
	
	$attr_dm += $size >= 9               ? -1 :
				$size == 5 || $size == 4 ?  1 :
				$size <= 3               ?  2 : 0;

	$attr_dm += $atmo >= 8               ?  1 :
				$atmo <= 5               ? -1 : 0;

	my ($attribute, $size_dm_2) = $TBLATTRIBUTE{$terrain}
		? $TBLATTRIBUTE{$terrain}[Traveller::Dice::roll(2, $attr_dm, 2, 12)]
		: $TBLATTRIBUTE{'other'}[Traveller::Dice::roll(2, $attr_dm, 2, 12)];

	my $special = '';
	SWITCH_ATTRIBUTE: {
		$special = 'Amphibian ',  last SWITCH_ATTRIBUTE if ($attribute eq 'a');
		$special = 'Flying ',     last SWITCH_ATTRIBUTE if ($attribute eq 'f');
		$special = 'Swimming ',   last SWITCH_ATTRIBUTE if ($attribute eq 's');
		$special = 'Triphibian ', last SWITCH_ATTRIBUTE if ($attribute eq 't');
	}

	$size_dm_2 = $size_dm_2 ? $size_dm_2 : 0;
	
	my $size_dm = $attribute eq 'f' ? $size_dm_2 : $size_dm_1 + $size_dm_2;

	$size_dm += $size >= 8               ? -1 :
				$size <= 5               ?  1 : 0;

	my $r = 0;
REROLL_SIZE: while ($r = Traveller::Dice::roll(2, $size_dm, 1, 19)) {
		$size_dm += 6, last REROLL_SIZE unless ($r == 13);
	}

	my $weight	= $TBLSIZE{'weight'}[$r];
	my $hits	= $TBLSIZE{'hits'}[$r];
	my $wounds	= $TBLSIZE{'wounds'}[$r];

	my ($hitsa, $hitsb) = split /,/, $hits;
	$hitsa = Traveller::Dice::roll($hitsa);
	$hitsb = Traveller::Dice::roll($hitsb);
	$hits = "$hitsa/$hitsb";

	$wounds = $wounds || '';

	my $weapon_dm = 0;
	SWITCH_WEAPON: {
		$weapon_dm =  8, last SWITCH_WEAPON if ($kind eq 'C');
		$weapon_dm =  4, last SWITCH_WEAPON if ($kind eq 'O');
		$weapon_dm = -3, last SWITCH_WEAPON if ($kind eq 'H');
	}

	$r = Traveller::Dice::roll(2, $weapon_dm, 1, 19);
	my $weapon = $TBLSIZE{'weapons'}[$r];

	my $armor_dm = 0;
	SWITCH_ARMOR: {
		$armor_dm = -1, last SWITCH_ARMOR if ($kind eq 'C');
		$armor_dm =  1, last SWITCH_ARMOR if ($kind eq 'S');
		$armor_dm =  2, last SWITCH_ARMOR if ($kind eq 'H');
	}

	my $armor = '';
	if ($attribute ne 't' && $attribute ne 'f') {
		REROLL_ARMOR: while ($r = Traveller::Dice::roll(2, $armor_dm, 1, 19)) {
			$armor = $TBLSIZE{'armor'}[$r];
			$armor_dm += 6, last REROLL_ARMOR unless ($armor eq 'reroll');
		}
	}
	$armor = $armor || 'none';

	my ($attack, $flee, $speed);
	my ($a, $b);
	
	$a = $TBLBEHAVIOR{$type}[0];
	$attack = $a =~ /[PSM]/
		? 0
		: Traveller::Dice::roll(1, $a);
	
	$a = $TBLBEHAVIOR{$type}[1];
	$flee = $a =~ /S/
		? 0
		: Traveller::Dice::roll(1, $a);
	
	($a, $b) = split /,/, $TBLBEHAVIOR{$type}[2];
	$speed = Traveller::Dice::roll(1, -$a, $b);

	my $afs = sprintf("A%dF%dS%d", $attack, $flee, $speed);

	$^A = '';
formline <<EO, $roll, $number, "$special$type", $weight, $hits, $armor, $wounds, $weapon, $afs;
 @> @> @<<<<<<<<<<<<<<< @>>>>>kg  @>>>>  @<<<<<<<< @> @<<<<<<<<<<<<<<<<<< @<<<<<
EO
	my $result = $^A;
	$^A = '';
	
	return $result;
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

while (<Traveller::Animal::DATA>) {
	last if /^$/; chomp;
	my ($t, $m) = split /\|/;
	$TBLTERRAIN{$t} = $m;
}

while (<Traveller::Animal::DATA>) {
	last if /^$/; chomp;
	my ($a, $b) = split /\|/;
	my @f = split /;/, $b;
	$TBLTYPE{$a} = [ @f ];
}

while (<Traveller::Animal::DATA>) {
	last if /^$/; chomp;
	my ($a, $b) = split /\|/;
	my @f = split /;/, $b;
	$TBLATTRIBUTE{$a} = [ @f ];
}

while (<Traveller::Animal::DATA>) {
	last if /^$/; chomp;
	my ($a, $b) = split /\|/;
	my @f = split /;/, $b;
	$TBLSIZE{$a} = [ @f ];
}

while (<Traveller::Animal::DATA>) {
	last if /^$/; chomp;
	my ($a, $b) = split /\|/;
	my @f = split /;/, $b;
	$TBLBEHAVIOR{$a} = [ @f ];
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;

__DATA__
clear|3,0
prairie|4,0
rough|0,0
broken|-3,-3
mountain|0,0
forest|-4,-4
jungle|-3,-2
river|1,1
swamp|-2,4
marsh|0,-1
desert|3,-3
beach|3,2
surface|2,3
shallows|2,2
depths|2,4
bottom|-4,0
seacave|-2,0
sargasso|-4,-2
ruins|-3,0
cave|-4,1
chasm|-1,-3
crater|0,-1

H|1,Filter;Filter;Filter;Intermittent;Intermittent;1,Intermittent;Intermittent;Grazer;1,Grazer;2,Grazer;3,Grazer;2,Grazer;4,Grazer;5,Grazer
O|Gatherer;Gatherer;Eater;Gatherer;2,Eater;Gatherer;Hunter;1,Hunter;Hunter;Gatherer;1,Eater;1,Hunter;Gatherer;Gatherer
C|Siren;Pouncer;Siren;Pouncer;1,Killer;Trapper;Pouncer;Chaser;3,Chaser;Chaser;Killer;2,Chaser;Siren;1,Chaser
S|1,Carrion-eater;2,Carrion-eater;1,Reducer;1,Hijacker;2,Carrion-eater;1,Intimidator;Reducer;1,Carrion-eater;3,Reducer;Hijacker;2,Intimidator;1,Reducer;Hijacker;1,Intimidator

beach|;;s,1;a,2;a,2;,0;,0;,0;,0;,0;,0;f,-6;f,-5
marsh|;;s,-6;a,2;a,1;,0;,0;,0;,0;,0;,0;f,-6;f,-5
river|;;s,1;a,1;,0;,0;,0;,0;,0;,0;,0;f,-6;f,-5
sea|;;s,2;s,2;s,2;a,2;a,0;s,1;s,-1;t,-7;t,-6;f,-6;f,-5
swamp|;;s,-3;a,1;a,1;,0;,0;,0;,0;,0;,0;f,-6;f,-5
other|;;,0;,0;,0;,0;,0;,0;,0;,0;f,-6;f,-5;f,-3

weight|;1;3;6;12;25;50;100;200;400;800;1600;3200;reroll;6000;12000;24000;30000;36000;40000;44000
hits|;1,0;1,1;1,2;2,2;3,2;4,2;5,2;5,3;6,3;7,3;8,3;8,4;reroll;9,4;10,5;12,6;14,7;15,7;16,8;17,9;
wounds|;-2;-2;-1;;;;;1;2;3;4;5;reroll;x2;x2;x3;x4;x4;x5;x6
weapons|;hooves, horns;horns;hooves, teeth;hooves;horns, teeth;thrasher;claws, teeth;teeth;claws;claws;thrasher;claws, teeth;claws +1;stinger;claws +1, teeth +1;teeth +1;as blade;as pike;as broadsword;as body pistol
armor|;reroll;;;jack;;;;;;jack;;reroll;mesh +1;cloth +1;mesh;cloth;battle +4;reflec;ablat;battle

Filter|P;2;5,0
Intermittent|3;3;4,1
Grazer|2;0;2,2
Gatherer|3;2;3,1
Hunter|0;2;4,1
Eater|0;3;3,1
Pouncer|S;S;4,1
Chaser|M;3;2,2
Trapper|S;2;5,0
Siren|S;3;4,0
Killer|0;3;3,1
Hijacker|1;2;4,1
Intimidator|2;1;4,1
Carrion-eater|3;2;3,1
Reducer|3;2;4,1

# __DATA__

