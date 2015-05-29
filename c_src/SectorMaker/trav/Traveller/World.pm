package Traveller::World;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			World.pm
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
#	changes:		apr 13 05 pst fixed is_in function
#
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use strict;

=head1 NAME

Traveller::World - A class representing Traveller worlds.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2002 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller::World;
 $w = Traveller::World->new();
 $w->generate();
 print $w->listing();

=head1 DESCRIPTION

The Traveller::World class is used for representing worlds in the
Traveller game, and can also be used to generate worlds according to the
method presented in Traveller Book 3, Worlds and Adventures.

=head1 SEE ALSO

Blah blah blah.

=head1 METHODS

=cut

use Traveller::Dice;
use Traveller::Utility;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my %TBL;

my %SYSTEMTBL = (
	2	=> ['A',0,0,1],
	3	=> ['A',0,0,1],
	4	=> ['A',0,0,1],
	5	=> ['B',0,0,1],
	6	=> ['B',0,0,1],
	7	=> ['C',0,1,1],
	8	=> ['C',1,1,1],
	9	=> ['D',1,1,1],
	10	=> ['E',1,1,0],
	11	=> ['E',1,1,0],
	12	=> ['X',1,1,0],
);

my %TECHLVLTBL = (
	0	=> '0,2,1,0,0,1',
	1	=> '0,2,1,0,1,0',
	2	=> '0,1,1,0,1,0',
	3	=> '0,1,1,0,1,0',
	4	=> '0,1,0,0,1,0',
	5	=> '0,0,0,0,1,1',
	6	=> '0,0,0,0,0,0',
	7	=> '0,0,0,0,0,0',
	8	=> '0,0,0,0,0,0',
	9	=> '0,0,0,1,2,0',
	10	=> '6,0,1,2,4,0',
	11	=> '4,0,1,0,0,0',
	12	=> '2,0,1,0,0,0',
	13	=> '0,0,1,0,0,-2',
	14	=> '0,0,1,0,0,0',
	15	=> '0,0,0,0,0,0',
		'A'	=> '6,0,1,2,4,0',
		'B'	=> '4,0,1,0,0,0',
		'C'	=> '2,0,1,0,0,0',
		'D'	=> '0,0,1,0,0,-2',
		'E'	=> '0,0,1,0,0,0',
		'F'	=> '0,0,0,0,0,0',
	'X'	=> '-4,0,0,0,0,0',
);

my %NUMTBL = (
	'GNUM'	=> [0,0,1,1,2,2,3,3,4,4,4,5,5],
	'BNUM'	=> [0,0,1,1,1,1,1,1,2,2,2,2,2,3],
);

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head2 $obj = Traveller::World->new();

Constructor for the World. Returns a reference to a Traveller::World object.

=cut

sub new {
	my $pkg = shift;
	my %arg = @_;
	my $obj = bless {
		ATTR			=> {
			PORT			=> 0,
			SIZE			=> 0,
			ATMO			=> 0,
			HYDR			=> 0,
			POPU			=> 0,
			GOVT			=> 0,
			LAWL			=> 0,
			TECH			=> 0},
		NAME			=> '',
		LOCATION		=> '0000',
		NAVAL			=> '',
		SCOUT			=> '',
		GIANT			=> '',
		PNUM			=> 0,
		BNUM			=> 0,
		GNUM			=> 0,
		CAPITOL			=> 0,
		ZONE			=> '',
		ALLEGIANCE		=> '',
		%arg
	}, $pkg;
	$obj->profile($arg{PROFILE}) if $arg{PROFILE};
	return $obj;
}

=head2 $obj->name();

Access the name of the world.

=cut

sub name {
	my $self = shift;
	if (@_) { $self->{NAME} = shift }
	return $self->{NAME};
}

=head2 $obj->location();

Access the hex location of the world within its subsector.

=cut

sub location {
	my $self = shift;
	if (@_) { $self->{LOCATION} = shift }
	return $self->{LOCATION};
}

=head2 $obj->attr();

Access an attribute of the world.

=cut

sub attr {
	my $self = shift;
	my $attr = shift;
	if (@_) { $self->{ATTR}{$attr} = shift; }
	return $self->{ATTR}{$attr};
}

sub naval {
	my $self = shift;
	if (@_) { $self->{NAVAL} = shift; }
	return $self->{NAVAL};
}

sub scout {
	my $self = shift;
	if (@_) { $self->{SCOUT} = shift; }
	return $self->{SCOUT};
}

sub giant {
	my $self = shift;
	if (@_) { $self->{GIANT} = shift; }
	return $self->{GIANT};
}

sub pnum {
	my $self = shift;
	if (@_) { $self->{PNUM} = shift; }
	return $self->{PNUM};
}

sub bnum {
	my $self = shift;
	if (@_) { $self->{BNUM} = shift; }
	return $self->{BNUM};
}

sub gnum {
	my $self = shift;
	if (@_) { $self->{GNUM} = shift; }
	return $self->{GNUM};
}

sub zone {
	my $self = shift;
	if (@_) { $self->{ZONE} = shift; }
	return $self->{ZONE};
}

sub allegiance {
	my $self = shift;
	if (@_) { $self->{ALLEGIANCE} = shift; }
	return $self->{ALLEGIANCE};
}

=head2 $obj->generate();

Generates a world using the method presented in Traveller Book 3.

=cut

sub generate {
	my $self = shift;
	my $m = 0;

	# basic star system generation checklist

	# 1. [determine system presence]
	# 2. check system contents table

	# 2a. find starport type

	$self->{ATTR}{PORT} = $SYSTEMTBL{Traveller::Dice::roll(2)}[0];
	
	# 2b. check for naval base

	$self->{NAVAL} = $self->{ATTR}{PORT} =~ /[AB]/
		? $SYSTEMTBL{Traveller::Dice::roll(2)}[1]
		: 0;

	# 2c. check for scout base

	$m =	$self->{ATTR}{PORT} eq 'A' && -3
		||	$self->{ATTR}{PORT} eq 'B' && -2
		||	$self->{ATTR}{PORT} eq 'C' && -1
		||	0;

	$self->{SCOUT} = $self->{ATTR}{PORT} =~ /[ABCD]/
		? $SYSTEMTBL{Traveller::Dice::roll(2, $m, 2, 12)}[2]
		: 0;

	# 2c. check for gas giant

	$self->{GIANT} = $SYSTEMTBL{Traveller::Dice::roll(2)}[3];

	# 3. [name main world]
	# 4. decide if travel zone coded
	#    xxx pst this is arbitrary

	$self->{ZONE} = Traveller::Dice::roll(2) == 12
					  ? ('', 'A', 'R')[Traveller::Dice::roll(1, -4, 0, 2)]
					  : '';

	# 5. generate main world upp

	# 5a. [note starport type]
	# 5b. main world size: 2d-2

	$self->{ATTR}{SIZE} = Traveller::Dice::roll(2, -2);

	# 5c. main world atmosphere: 2d-7 + size
	#     if size 0, then atmo 0

	$m = -7;
	$m += $self->{ATTR}{SIZE};
	$self->{ATTR}{ATMO} = $self->{ATTR}{SIZE} > 0
		? Traveller::Dice::roll(2, $m, 0, 12)
		: 0;

	# 5d. main world hydrographics: 2d-7 + size
	#     if size 1-, then hydr 0
	#     if atmo 1-, then dm -4
	#     if atmo A+, then dm -4

	$m = -7;
	$m += $self->{ATTR}{SIZE};
	$m += $self->{ATTR}{ATMO} <=  1 ? -4 : 0;
	$m += $self->{ATTR}{ATMO} >= 10 ? -4 : 0;
	$self->{ATTR}{HYDR} = $self->{ATTR}{SIZE} > 1
		? Traveller::Dice::roll(2, $m, 0, 10)
		: 0;

	# 5e. population: 2d-2

	$self->{ATTR}{POPU} = Traveller::Dice::roll(2, -2);

	# 5f. government: 2d-7 + popu

	$m = -7;
	$m += $self->{ATTR}{POPU};
	$self->{ATTR}{GOVT} = Traveller::Dice::roll(2, $m, 0, 14);

	# 5g. law level: 2d-7 + govt

	$m = -7;
	$m += $self->{ATTR}{GOVT};
	$self->{ATTR}{LAWL} = Traveller::Dice::roll(2, $m, 0, 10);

	# 5h. technological level: 1d + dms from the tech level table

	$m = 0;
	$m += (split(/,/, $TECHLVLTBL{$self->{ATTR}{PORT}}))[0];
	$m += (split(/,/, $TECHLVLTBL{$self->{ATTR}{SIZE}}))[1];
	$m += (split(/,/, $TECHLVLTBL{$self->{ATTR}{ATMO}}))[2];
	$m += (split(/,/, $TECHLVLTBL{$self->{ATTR}{HYDR}}))[3];
	$m += (split(/,/, $TECHLVLTBL{$self->{ATTR}{POPU}}))[4];
	$m += (split(/,/, $TECHLVLTBL{$self->{ATTR}{GOVT}}))[5];
	$self->{ATTR}{TECH} = Traveller::Dice::roll(1, $m, 0, 15);

	# mt values

	# p value

	if (Traveller::Dice::roll() % 2) {
		do {
			$self->{PNUM} = Traveller::Dice::roll(1, -1);
		} while (0 >= $self->{PNUM} && $self->{PNUM} <= 4);
	}
	else {
		do {
			$self->{PNUM} = Traveller::Dice::roll(1,  4);
		} while (5 >= $self->{PNUM} && $self->{PNUM} <= 9);
	}

	# b value

	if (Traveller::Dice::roll(2) >= 8) {
		$self->{BNUM} = $NUMTBL{BNUM}[Traveller::Dice::roll(2)];
	}
	else {
		$self->{BNUM} = 0;
	}

	# g value

	if (Traveller::Dice::roll(2) >= 5) {
		$self->{GNUM} = $NUMTBL{GNUM}[Traveller::Dice::roll(2)];
	}
	else {
		$self->{GNUM} = 0;
	}

}

=head2 $obj->profile();

Returns the Universal Planetary Profile (UPP) of a world.

=cut

sub profile {
	my $self = shift;
	if (@_) {
		my @profile = split(//, shift);
		$self->{ATTR}{PORT} = $profile[0];
		$self->{ATTR}{SIZE} = Traveller::Utility::dehex($profile[1]);
		$self->{ATTR}{ATMO} = Traveller::Utility::dehex($profile[2]);
		$self->{ATTR}{HYDR} = Traveller::Utility::dehex($profile[3]);
		$self->{ATTR}{POPU} = Traveller::Utility::dehex($profile[4]);
		$self->{ATTR}{GOVT} = Traveller::Utility::dehex($profile[5]);
		$self->{ATTR}{LAWL} = Traveller::Utility::dehex($profile[6]);
		$self->{ATTR}{TECH} = Traveller::Utility::dehex($profile[8]);
	}
	my $s = sprintf(
		"%s%X%X%X%X%X%X-%s",
		$self->{ATTR}{PORT},
		$self->{ATTR}{SIZE},
		$self->{ATTR}{ATMO},
		$self->{ATTR}{HYDR},
		$self->{ATTR}{POPU},
		$self->{ATTR}{GOVT},
		$self->{ATTR}{LAWL},
		Traveller::Utility::enhex($self->{ATTR}{TECH}),
		);
	return $s;
}

=head2 $obj->listing();

Returns the canonical representation of a world as a string.

=cut

sub listing {
	my $self = shift;

	my %arg = (
		LEGEND		=> undef,
		REMARK		=> undef,
		QUADRANT	=> undef,
		PBG			=> undef,
		@_
	);

	my $profile	= $self->profile();
	my $n		= $self->{NAVAL} ? 'N' : '';
	my $s		= $self->{SCOUT} ? 'S' : '';
	my $g		= $self->{GIANT} ? 'G' : '';
	my $t		= $arg{REMARK} ? join ' ', Traveller::World::classification($self->{ATTR}) : '';
	my $name	= $self->{ATTR}{POPU} >= 9 ? uc($self->{NAME}) : $self->{NAME};
	my $location	= $arg{QUADRANT}
					? Traveller::World::location_in_sector($self->{LOCATION}, $arg{QUADRANT})
					: $self->{LOCATION};

#	$name = '. . . . . . . . .' unless $name;

$^A = '';

if ($arg{PBG}) {
	my $pbg		= $self->{PNUM} . $self->{BNUM} . $self->{GNUM};
	my $base	= $self->{NAVAL} && $self->{SCOUT} ? 'A'
				: $self->{NAVAL} ? 'N'
				: $self->{SCOUT} ? 'S'
				: ' ';
	my $aleg	= '  ';
formline <<END_OF_LISTING, $name, $location, $profile, $base, $t, $self->{ZONE}, $pbg, $aleg;
@<<<<<<<<<<<<<@<<< @<<<<<<<<  @ @<<<<<<<<<<<<<<<@  @<< @<..
END_OF_LISTING
}
else {
formline <<END_OF_LISTING, $name, $location, $profile, $n, $s, $t, $self->{ZONE}, $g;
@<<<<<<<<<<<<<<< @<<< @<<<<<<<< @@ @<<<<<<<<<<<<<<<<<<<<<<<< @ @
END_OF_LISTING
}

	my $listing = $^A;

	if ($arg{LEGEND}) {
	
		my $port	= $self->{ATTR}{PORT}
					? 'Class ' . $self->{ATTR}{PORT} . ' starport'
					: 'No starport';

		my $size	= $self->{ATTR}{SIZE}
					? (1600 * $self->{ATTR}{SIZE}) . 'km'
					: 'Asteroid/planetoid belt';

		my $atmo	= $TBL{ATMO}[$self->{ATTR}{ATMO}] . ' atmosphere';

		my $hydr	= $self->{ATTR}{HYDR} == 0
					? 'No free standing water; desert'
					: $self->{ATTR}{HYDR} == 10
					? 'No land masses; water world'
					: ($self->{ATTR}{HYDR} * 10) . '% water';

		my @a = qw(inhabitants thousands millions billions);
		my @b = qw(none tens hundreds);
		my $popu	= $self->{ATTR}{POPU} == 0
					? 'No inhabitants'
					: $self->{ATTR}{POPU} % 3 == 0
					? ucfirst($a[$self->{ATTR}{POPU} / 3])
						. ' of inhabitants'
					: ucfirst($b[$self->{ATTR}{POPU} % 3])
						. ' of ' . $a[$self->{ATTR}{POPU} / 3];

		my $govt	= $TBL{GOVT}[$self->{ATTR}{GOVT}];

$listing .= <<END_OF_LEGEND;
----------------------|||||||-|---------------------------------
       Starport Type -+|||||| | $port
      Planetary Size --+||||| | $size
Planetary Atmosphere ---+|||| | $atmo
        Hydrographic ----+||| | $hydr
          Population -----+|| | $popu
          Government ------+| | $govt
           Law Level -------+ |
          Tech Level ---------+
END_OF_LEGEND
	}

	return $listing;
}

sub report {
	my $self = shift;
	print $self->listing(@_);
}

sub distance_to {
	my $self = shift;
	my $world = shift;

	return Traveller::World::distance_between($self->location, $world->location);
}

sub location_in_subsector {
	my $location = shift;

	my	$x = substr($location, 0, 2);
		$x = $x %  8;
		$x =  8 unless $x;
	my	$y = substr($location, 2, 2);
		$y = $y % 10;
		$y = 10 unless $y;

	return sprintf("%02d%02d", $x, $y);
}

sub location_in_sector {
	my $location = shift;
	my $quad = ord(uc(shift)) - 65;

	$location = location_in_subsector($location);

	my	$x = substr($location, 0, 2);
		$x += ($quad % 4) * 8;
	my	$y = substr($location, 2, 2);
		$y += int($quad / 4) * 10;

	return sprintf("%02d%02d", $x, $y);
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head2 classification();

Given a reference to a world object, returns the trade classifications for that world as an array.

=cut

sub is_ag {
	my $attr = shift;
	return	hex($attr->{ATMO}) >= 4 && hex($attr->{ATMO}) <= 9
		&&	hex($attr->{HYDR}) >= 4 && hex($attr->{HYDR}) <= 8
		&&	hex($attr->{POPU}) >= 5 && hex($attr->{POPU}) <= 7;
}

sub is_na {
	my $attr = shift;
	return	hex($attr->{ATMO}) <= 3
		&&	hex($attr->{HYDR}) <= 3
		&&	hex($attr->{POPU}) >= 6;
}

sub is_in {
	my $attr = shift;
	return	hex($attr->{POPU}) >= 9
		&& (hex($attr->{ATMO}) == 0
		||	hex($attr->{ATMO}) == 1
		||	hex($attr->{ATMO}) == 2
		||	hex($attr->{ATMO}) == 4
		||	hex($attr->{ATMO}) == 7
		||	hex($attr->{ATMO}) == 9);
}

sub is_ni {
	my $attr = shift;
	return	hex($attr->{POPU}) <= 6;
}

sub is_ri {
	my $attr = shift;
	return	(hex($attr->{ATMO}) == 6 || hex($attr->{ATMO}) == 8)
		&&	hex($attr->{POPU}) >= 6 && hex($attr->{POPU}) <= 8
		&&	hex($attr->{GOVT}) >= 4 && hex($attr->{GOVT}) <= 9;
}

sub is_po {
	my $attr = shift;
	return	hex($attr->{ATMO}) >= 2 && hex($attr->{ATMO}) <= 5
		&&	hex($attr->{HYDR}) <= 3;
}

sub is_hi {
	my $attr = shift;
	return	hex($attr->{POPU}) >= 9;
}

sub is_lo {
	my $attr = shift;
	return	hex($attr->{POPU}) <= 4;
}

sub is_wa {
	my $attr = shift;
	return	hex($attr->{HYDR}) == 10;
}

sub is_de {
	my $attr = shift;
	return	hex($attr->{HYDR}) == 0
		&&	hex($attr->{ATMO}) >= 2;
}

sub is_va {
	my $attr = shift;
	return	hex($attr->{ATMO}) == 0;
}

sub is_as {
	my $attr = shift;
	return	$attr->{SIZE} =~ /[rs]/i
		||	hex($attr->{SIZE}) == 0;
}

sub is_ic {
	my $attr = shift;
	return	hex($attr->{ATMO}) <= 1
		&&	hex($attr->{HYDR}) >= 1;
}

sub is_ba {
	my $attr = shift;
	return	hex($attr->{POPU})
		+	hex($attr->{GOVT})
		+	hex($attr->{LAWL}) == 0;
}

sub is_fl {
	my $attr = shift;
	return	hex($attr->{ATMO}) >= 10
		&&	hex($attr->{HYDR}) >= 1;
}

sub classification {
	my $attr = shift;
	my @remarks;

	push @remarks, 'Ag' if is_ag($attr);
	push @remarks, 'Na' if is_na($attr);
	push @remarks, 'In' if is_in($attr);
	push @remarks, 'Ni' if is_ni($attr);
	push @remarks, 'Ri' if is_ri($attr);
	push @remarks, 'Po' if is_po($attr);
	push @remarks, 'Hi' if is_hi($attr);
	push @remarks, 'Lo' if is_lo($attr);
	push @remarks, 'Wa' if is_wa($attr);
	push @remarks, 'De' if is_de($attr);
	push @remarks, 'Va' if is_va($attr);
	push @remarks, 'As' if is_as($attr);
	push @remarks, 'Ic' if is_ic($attr);
	push @remarks, 'Ba' if is_ba($attr);
	push @remarks, 'Fl' if is_fl($attr);

	return sort @remarks;
}

sub distance_between {
   my ($h1, $h2) = @_;

   my ($x1, $y1) = ($1, $2) if $h1 =~ /(\d\d)(\d\d)/;
   my ($x2, $y2) = ($1, $2) if $h2 =~ /(\d\d)(\d\d)/;
   my $a1 = ($y1 + int($x1/2));
   my $a2 = ($y2 + int($x2/2));
   my $d1 = abs($a1 - $a2);
   my $d2 = abs($x1 - $x2);
   my $d3 = abs(($a1 - $x1) - ($a2 - $x2));
   return (sort {$b <=> $a} ($d1, $d2, $d3))[0];
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head1 AUTHOR

Paul Sanford Toney <signalgk@gmail.com>

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#  package startup code

foreach (1..2) {
	my $line = <Traveller::World::DATA>;
	chomp $line;
	my ($a, $b) = split /\|/, $line;
	my @f = split /;/, $b;
	$TBL{$a} = [ @f ];
}

1;

__DATA__
ATMO|No;Trace;Very thin, tainted;Very thin;Thin, tainted;Thin;Standard;Standard, tainted;Dense;Dense, tainted;Exotic;Corrosive;Insidious;Dense, high;Ellipsoid;Thin, low
GOVT|No government;Company/corporation;Participating democracy;Self-perpetuating oligarchy;Representative democracy;Feudal technocracy;Captive government;Balkanized;Civil service bureaucracy;Impersonal bureaucracy;Charismatic dictator;Non-charismatic dictator;Charismatic oligarchy;Religious dictatorship
# __DATA__
