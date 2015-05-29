package Traveller::Subsector;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Subsector.pm
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

use Traveller::Dice;
use Traveller::World;
use Traveller::Name;
use Traveller::Utility;

use Text::Wrap;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub new {
	my $pkg = shift;
	my %arg = @_;
	my $obj = bless {
		NAME			=> 'Unnamed',
		WORLDS			=> [],
		QUADRANT		=> 'A',
		%arg
	}, $pkg;
	return $obj;
}

sub generate {
	my $self = shift;
	my %arg = (
		DENSITY			=> 'standard',
		AUTONAME		=> 1,
		LANGUAGE		=> undef,
		@_
	);

	$arg{DENSITY} = lc($arg{DENSITY});
	$arg{LANGUAGE} = lc($arg{LANGUAGE});
	$arg{LANGUAGE} = undef if $arg{LANGUAGE} eq 'none';

	foreach my $m (1..8) {
		foreach my $n (1..10) {
			my $p =	0;
			SWITCH: {
				$p = Traveller::Dice::roll(2) >= 12 ? 1 : 0, last SWITCH if ($arg{DENSITY} eq 'rift');
				$p = Traveller::Dice::roll(1) >= 6  ? 1 : 0, last SWITCH if ($arg{DENSITY} eq 'sparse');
				$p = Traveller::Dice::roll(1) >= 5  ? 1 : 0, last SWITCH if ($arg{DENSITY} eq 'scattered');
				$p = Traveller::Dice::roll(1) >= 4  ? 1 : 0, last SWITCH if ($arg{DENSITY} eq 'standard');
				$p = Traveller::Dice::roll(1) >= 3  ? 1 : 0, last SWITCH if ($arg{DENSITY} eq 'dense');
			}
			
			if ($p) {
				my $world = Traveller::World->new(LOCATION => sprintf("%02d%02d", $m, $n));

				$world->generate;
				$world->name(ucfirst($self->generate_name($arg{LANGUAGE})));

				push(@{ $self->{WORLDS} }, $world);
			}
		}
	}
	
	if ($arg{AUTONAME}) {
		my $w = $self->{WORLDS}[Traveller::Dice::range(scalar($self->worlds))];
		$self->name($w->name) if ($w);
	}
}

sub generate_name {
	my $self = shift;
	my $language = shift;
# 	if (Traveller::Dice::range(60) == 0) {
# 		return	''
# 				. Traveller::Dice::range(9)
# 				. Traveller::Dice::range(9)
# 				. Traveller::Dice::range(9)
# 				. '-'
# 				. Traveller::Dice::range(9)
# 				. Traveller::Dice::range(9)
# 				. Traveller::Dice::range(9);
# 	}
# 	else {
		return $language ? Traveller::Name::generate(LANGUAGE => $language, SYLLABLES => (2 + Traveller::Dice::range(3))) : '';
# 	}
}

sub name {
	my $self = shift;
	if (@_) { $self->{NAME} = shift }
	return $self->{NAME};
}

sub worlds {
	my $self = shift;
	if (@_) { @{ $self->{WORLDS} } = @_ }
	return @{ $self->{WORLDS} };
}

sub worlds_ref {
	my $self = shift;
	return $self->{WORLDS};
}

sub add_world {
	my $self = shift;
	my $world = shift;

	push @{ $self->{WORLDS} }, $world;
}

sub report {
	my $self = shift;

	my %arg = (
		LISTING		=> 1,
		HEXMAP		=> 1,
		PAGINATE	=> undef,
		SUMMARY		=> 1,
		LEGEND		=> undef,
		@_
	);

	if ($arg{LISTING}) {
	print $self->listing(%arg);
	print "\x0C" if $arg{PAGINATE};
	}

	if ($arg{HEXMAP}) {
		print $self->hexmap;
	}
}

sub listing {
	my $self = shift;

	my %arg = (
		SUMMARY		=> 1,
		LEGEND		=> undef,
		RELATIVE	=> undef,
		PBG			=> undef,
		@_
	);

	my $tw = 0;
	my $tp = 0;
	my $hp = '0';
	my $ht = '0';
	my @hpa = ();
	my @hta = ();

	$tw = scalar($self->worlds);

	my $s = <<END_OF_LISTING;
$self->{NAME} Subsector
----------------------------------------------------------------
World Name       Hex  Profile      Remarks
----------------------------------------------------------------
END_OF_LISTING

	my $quadrant = $arg{RELATIVE} ? $self->{QUADRANT} : undef;

	foreach my $w ($self->worlds) {
		$s .= $w->listing(REMARK => 1, QUADRANT => $quadrant, PBG => $arg{PBG});
		
		$tp += $w->{ATTR}{POPU} > 3
			? 10 ** $w->{ATTR}{POPU}
			: 0;
		
		if ($w->{ATTR}{POPU} > $hp) {
			@hpa = ();
			push @hpa, ($w->name ? ($w->name . ' ' . $w->location) : $w->location);
			$hp = $w->{ATTR}{POPU};
		}
		elsif ($w->{ATTR}{POPU} == $hp) {
			push @hpa, ($w->name ? ($w->name . ' ' . $w->location) : $w->location);
		}

		if ($w->{ATTR}{TECH} > $ht) {
			@hta = ();
			push @hta, ($w->name ? ($w->name . ' ' . $w->location) : $w->location);
			$ht = $w->{ATTR}{TECH};
		}
		elsif ($w->{ATTR}{TECH} == $ht) {
			push @hta, ($w->name ? ($w->name . ' ' . $w->location) : $w->location);
		}
	}

	if ($arg{LEGEND}) {
		$s .= <<END_OF_LEGEND;
----------------------|||||||-|-||---------------------------|-|
       Starport Type -+|||||| | ||              Travel Zone -+ |
      Planetary Size --+||||| | ||          Gas Giant Present -+
Planetary Atmosphere ---+|||| | |+- Scout Base present
        Hydrographic ----+||| | +-- Naval Base present
          Population -----+|| |
          Government ------+| |
           Law Level -------+ |
          Tech Level ---------+
END_OF_LEGEND
	}

	if ($arg{SUMMARY}) {
		my $n = $self->name ? ('The ' . $self->name) : 'This';
		my $hpx = commify_series(@hpa);
		my $htx = commify_series(@hta);

		$hp = sprintf("%X", $hp);
		$ht = sprintf("%X", $ht);
		
		$tp =	($tp > 10**9)	? sprintf("%.3f billion",  $tp / 10**9) :
				($tp > 10**6)	? sprintf("%.3f million",  $tp / 10**6) :
				($tp > 10**3)	? sprintf("%.3f thousand", $tp / 10**3)
								: $tp;

		my @lines;
		push @lines, "$n subsector contains $tw worlds";
		push @lines, "with a total population of approximately $tp.";
		push @lines, "The highest population is $hp at $hpx;";
		push @lines, "the highest tech level is $ht at $htx.";

		$Text::Wrap::columns = 64;
		my $w = Text::Wrap::wrap("", "", @lines);

		$s .= <<END_OF_SUMMARY;
----------------------------------------------------------------
$w
END_OF_SUMMARY
	}

	$s .= <<END_OF_FOOTER;
----------------------------------------------------------------
END_OF_FOOTER

	return $s;
}

sub world_by_location {
	my $self = shift;
	my $location = shift;
	foreach my $w ($self->worlds) {
		if (local_hex($w->location) eq local_hex($location)) {
			return $w;
		}
	}

	return undef;
}

sub world_by_name {
	my $self = shift;
	my $name = shift;

	foreach my $w ($self->worlds) {
		if ($w->name =~ /$name/i) {
			return $w;
		}
	}

	return undef;
}

sub worlds_by {
	my $self = shift;
	my $test = shift; # code reference

	my @worlds;

	foreach my $w ($self->worlds) {
		push @worlds, $w if ($test->($w));
	}

	return @worlds;
}

sub world_listing {
	my $self = shift;
	my $location = shift;

	my $world = world_by_location($location);
	my %m;

	foreach my $m (-6..6) {
		my $location = $world->location;
		foreach my $n (-6..6) {
		}
	}
}

sub hexmap {
	my $self = shift;
	my $result = '';

	$result .= "   _______             _______             _______             _______\n";

	foreach my $m (1..11) {
		my $n = 0;
		my $k = 0;

		foreach $k (1..3) {
			foreach $n (1..8) {
				my $location = sprintf("%02d%02d", $n, $m);
				my $hex = '';
				if (!($n % 2)) {
					$hex = sprintf("%02d%02d", $n, $m - 1);
				}
				else {
					$hex = sprintf("%02d%02d", $n, $m);
				}
				my $w = $self->world_by_location($hex);

				my $star = '     ';
				if ($w) {
					if ($w->{ATTR}{SIZE} > 0) {
						$star = ($w->{ATTR}{HYDR} > 0) ? ' @ ' : ' O ';
					}
					else {
						$star = ':::';
					}
					
					if ($w->{ZONE} =~ /r/i) {
						$star = "[$star]";
					}
					elsif ($w->{ZONE} =~ /a/i) {
						$star = "($star)";
					}
					else {
						$star = " $star ";
					}
				}
				my $name = '         ';
				if ($w) {
					$name = substr($w->name, 0, 9);
					if ($w->{ATTR}{POPU} >= 9) {
						$name = uc($name);
					}
					$^A = '';
formline <<EO, $name;
@|||||||| x
EO
					$name = $^A;
					chomp $name;
					chop $name;
					chop $name;
					$^A = '';
				}
				my $port = $w ? $w->{ATTR}{PORT} : ' ';
				my $nav	=  $w ? ($w->{NAVAL} ? '*' : ' ') : ' ';
				my $sco	=  $w ? ($w->{SCOUT} ? '^' : ' ') : ' ';
				my $gas	=  $w ? ($w->{GIANT} ? 'o' : ' ') : ' ';

				if ($k == 1) {
					if ($n % 2) {
						if ($n == 1) { $result .= "  "; }
						if ($m == 11) {
							$location = "    ";
							if ($n != 1) {
								$result .= "/";
							}
							else {
								$result .= " ";
							}
						}
						else {
							$result .= "/";
						}
						$result .= "$location   \\";
					}
					else {
						$result .= "           ";
						if ($n == 8 && $m != 1) { $result .= "/"; }
					}
				}
				elsif ($k == 2) {
					if ($n % 2) {
						if ($n == 1) { $result .= " "; }
						if ($m == 11) {
							if ($n != 1) {
								$result .= "/";
							}
							else {
								$result .= " ";
							}
						}
						else {
							$result .= "/";
						}
						if ($m < 11) {
							$result .= " $nav  $port  $gas \\";
						}
						else {
							$result .= "         \\";
						}
					}
					else {
						if ($m > 1) {
							$result .= "$name";
						}
						else {
							$result .= "         ";
						}
						if ($n == 8 && $m != 1) { $result .= "/"; }
					}
				}
				elsif ($k == 3) {
					if ($n % 2) {
						if ($m == 11) {
							$location = "    ";
							if ($n != 1) {
								$result .= "/";
							}
							else {
								$result .= " ";
							}
						}
						else {
							$result .= "/";
						}
						if ($m < 11) {
						$result .= " $sco $star   \\";
						}
						else {
							$result .= "           \\";
						}
					}
					else {
						$result .= "_______";
						if ($n == 8 && $m != 1) { $result .= "/"; }
					}
				}
			}
	
			$result .= "\n";
		}

		last if ($m >= 11);

		foreach $k (1..3) {
			foreach $n (1..8) {
				my $location = sprintf("%02d%02d", $n, $m);
				my $w = $self->world_by_location($location);

				my $star = '     ';
				if ($w) {
					if ($w->{ATTR}{SIZE} > 0) {
						$star = ($w->{ATTR}{HYDR} > 0) ? ' @ ' : ' O ';
					}
					else {
						$star = ':::';
					}
					
					if ($w->{ZONE} =~ /r/i) {
						$star = "[$star]";
					}
					elsif ($w->{ZONE} =~ /a/i) {
						$star = "($star)";
					}
					else {
						$star = " $star ";
					}
				}
				my $name = '         ';
				if ($w) {
					$name = substr($w->name, 0, 9);
					if ($w->{ATTR}{POPU} >= 9) {
						$name = uc($name);
					}
					$^A = '';
formline <<EO, $name;
@|||||||| x
EO
					$name = $^A;
					chomp $name;
					chop $name;
					chop $name;
					$^A = '';
				}
				my $port = $w ? $w->{ATTR}{PORT} : ' ';
				my $nav	=  $w ? ($w->{NAVAL} ? '*' : ' ') : ' ';
				my $sco	=  $w ? ($w->{SCOUT} ? '^' : ' ') : ' ';
				my $gas	=  $w ? ($w->{GIANT} ? 'o' : ' ') : ' ';

				if ($k == 1) {
					if ($n % 2) {
						$result .= "\\           /";
					}
					else {
						$result .= "$location   ";
						if ($n == 8) { $result .= "\\"; }
					}
				}
				elsif ($k == 2) {
					if ($n % 2) {
						$result .= " \\$name/";
					}
					else {
						$result .= " $nav  $port  $gas";
						if ($n == 8) { $result .= " \\"; }
					}
				}
				elsif ($k == 3) {
					if ($n % 2) {
						$result .= "  \\_______/";
					}
					else {
						$result .= " $sco $star ";
						if ($n == 8) { $result .= "  \\"; }
					}
				}
			}
	
			$result .= "\n";
		}
	}

	return $result;
}

sub import_sec {
	my $self = shift;
	my $file = shift;

	if (open(FILE, "<$file")) {

		while (<FILE>) {
	
			next if (/#/);
			next if (/\$/);

			if (/^[a-z0-9]/io) {
			my $name			= substr($_, 0,  14);
			   $name =~ s/([a-z0-9-]+).+/$1/io;
			
			my $location		= local_hex(substr($_, 14, 4));
			my $profile			= substr($_, 19, 9);
			my $b				= substr($_, 30, 1);
			my $naval			= $b =~ /A/ ? 'N'
								: $b =~ /N/ ? 'N'
								: '';
			my $scout			= $b =~ /A/ ? 'S'
								: $b =~ /S/ ? 'S'
								: '';
			my $remark			= substr($_, 32, 15);
			my $zone			= substr($_, 48, 1);
			my $pbg				= substr($_, 51, 3);
			my $giant			= $pbg =~ /\d\d[1-9]/io;

			my $world			= Traveller::World->new(
									NAME		=> $name,
									LOCATION	=> $location,
									PROFILE		=> $profile,
									NAVAL		=> $naval,
									SCOUT		=> $scout,
									ZONE		=> $zone,
									GIANT		=> $giant,
								);

			push(@{ $self->{WORLDS} }, $world);
			}
			
			if (/\@SUB-SECTOR:\s+([a-zA-Z0-9]+)\s+SECTOR:\s+([a-zA-Z0-9]+)/io) {
				$self->name($1);
			}

		}
	
		close(FILE);
	
	}
	else {
 		return $!;
	}
	
	return undef;
}

sub export_sec {
	my $self = shift;
	my $result = '';

	$result .= <<EO_HEAD;
$self->{NAME} Subsector

The data in the sector text files is laid out in column format:

 1-14: Name
15-18: HexNbr
20-28: UWP
   31: Bases
33-47: Codes & Comments
   49: Zone
52-54: PBG
56-57: Allegiance
59-74: Stellar Data
....+....1....+....2....+....3....+....4....+....5....+....6....+....7....+....8
EO_HEAD

	foreach my $w ($self->worlds) {

	my $name			= $w->name;
	my $hex				= $w->location;
	my $profile			= $w->profile;
	my $b				= ''
						. ($w->{NAVAL} ? 'N' : '')
						. ($w->{SCOUT} ? 'S' : '');
	my $remark			= join(' ', Traveller::World::classification($w->{ATTR}));
	my $zone			= $w->{ZONE};
	my $pbg				= '00' . ($w->{GIANT} ? '1' : '0');

	$^A = '';
formline <<EO, $name, $hex, $profile, $b, $remark, $zone, $pbg;
@<<<<<<<<<<<< @<<< @<<<<<<<< @> @<<<<<<<<<<<<<< @  @<<
EO
	$result .= $^A;
	$^A = '';

	}
	
	return $result;
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub local_hex {
	my $hex = shift;
	my $x	= substr($hex, 0, 2);
	my $y	= substr($hex, 2, 2);
	$x = $x %  8 ||  8;
	$y = $y % 10 || 10;

	return sprintf("%02d%02d", $x, $y);
}

sub commify_series {
	(@_ == 0) ? ''                                        :
	(@_ == 1) ? $_[0]                                     :
	(@_ == 2) ? join(" and ", @_)                         :
				join(", ", @_[0 .. ($#_-1)], "and $_[-1]");
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;
