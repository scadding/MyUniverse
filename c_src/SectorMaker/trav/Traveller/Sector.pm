package Traveller::Sector;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Sector.pm
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

use Traveller::Dice;
use Traveller::Subsector;
use Traveller::World;
use Traveller::Name;

use POSIX qw(ceil);
use Text::Wrap;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub new {
	my $pkg = shift;
	my %arg = @_;
	my $obj = bless {
		'name'			=> 'Unnamed',
		'subsectors'	=> {},
		%arg
	}, $pkg;
	return $obj;
}

sub generate {
	my $self = shift;

	my %arg = (
		AUTONAME	=> 1,
		DENSITY		=> undef,
		@_
	);

	foreach my $a (Traveller::Sector::_quadrants()) {
		my $density = $arg{DENSITY}
					? $arg{DENSITY}
					: qw/rift sparse scattered standard dense/[Traveller::Dice::range(5)];
		my $s = Traveller::Subsector->new(QUADRANT => $a);
		$s->generate(DENSITY => $density, AUTONAME => 1);
		$self->{'subsectors'}{$a} = $s;
	}

	if ($arg{AUTONAME}) {
		my $sk = scalar(keys(%{ $self->{'subsectors'} }));
		my $k = (keys(%{ $self->{'subsectors'} }))[Traveller::Dice::range($sk)];
		my $w = $self->{'subsectors'}{$k};
		$self->name($w->name);
	}
}

sub name {
	my $self = shift;
	if (@_) { $self->{'name'} = shift; }
	return $self->{'name'};
}

sub subsectors {
	my $self = shift;
	if (@_) { $self->{'subsectors'} = shift; }
	return $self->{'subsectors'};
}

sub subsector_at {
	my $self = shift;
	my $quadrant = uc(shift);
	if (@_) { $self->{'subsectors'}{$quadrant} = shift; }
	return $self->{'subsectors'}{$quadrant};
}

sub listing {
	my $self = shift;

	foreach my $quadrant (Traveller::Sector::_quadrants()) {
		my $subsector = $self->subsector_at($quadrant);
		$subsector->hexmap();
	}

	print $self->dotmap();
}

sub dotmap {
	my $self = shift;

	my $result = '';

	foreach my $sy (1..4) {
		foreach my $sx (1..4) {
			my $a = chr(64 + (($sy * 4) + $sx - 4));
			$result .= "+-$a----$a-";
		}

		$result .= "+\n";

		foreach my $i (1..10) {
			foreach my $sx (1..4) {
				$result .= '|';

				my $quadrant = chr(64 + (($sy * 4) + $sx - 4));
				my $subsector = $self->subsector_at($quadrant);
				next unless $subsector;

				foreach my $k (1..8) {
					my $hex = sprintf("%02d%02d", $k, $i);
					my $world = $subsector->world_by_location($hex);
					if ($world) {
						$result .= '.';
					}
					else {
						$result .= ' ';
					}
				}
			}
			$result .= "|\n";
		}
	}

	foreach (1..4) {
		$result .= "+--------";
	}

	$result .= "+\n";
	return $result;
}

sub report {
	my $self = shift;

	my %arg = (
		PAGINATE	=> undef,
		LEGEND		=> undef,
		SUMMARY		=> undef,
		LISTING		=> undef,
		HEXMAP		=> undef,
		RELATIVE	=> undef,
		PBG			=> undef,
		@_
	);

	print "\x0C" if $arg{PAGINATE};

	my $ts	= 0;
	my $tp	= 0;

	foreach my $quadrant (Traveller::Sector::_quadrants()) {
		my $subsector = $self->subsector_at($quadrant);
		next unless $subsector;

		foreach my $world ($subsector->worlds) {
			$ts++;
			$tp += $world->{ATTR}{POPU} > 3
				? 10 ** $world->{ATTR}{POPU}
				: 0;
		}
	}
	
	$tp =	($tp > 10**9)	? sprintf("%.3f billion",  $tp / 10**9) :
			($tp > 10**6)	? sprintf("%.3f million",  $tp / 10**6) :
			($tp > 10**3)	? sprintf("%.3f thousand", $tp / 10**3)
							: $tp;

	my $one	= $self->dotmap();
	my $two	= <<EOT;
SECTOR NAME:\r
${\uc($self->name)}\r
--------------------------------\r
The ${\$self->name} sector contains $ts systems with a total population of $tp inhabitants.\r
--------------------------------\r
SUBSECTORS:\r
EOT

	foreach my $quadrant (Traveller::Sector::_quadrants()) {
		my $subsector = $self->subsector_at($quadrant);
		next unless $subsector;

		$two .= "$quadrant ${\$subsector->name}\r";
	}

	$two .= "--------------------------------\r";

#format STDOUT =
#^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<~~^<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#$one,                                   $two
#.
#
#write;
	print "\x0C" if $arg{PAGINATE};

	foreach my $quadrant (Traveller::Sector::_quadrants()) {
		my $subsector = $self->subsector_at($quadrant);
		next unless $subsector;

		if ($arg{LISTING}) {
			print $subsector->listing(
				PBG => $arg{PBG},
				LEGEND => $arg{LEGEND},
				SUMMARY => $arg{SUMMARY},
				RELATIVE => $arg{RELATIVE},
				);
			print "\x0C" if $arg{PAGINATE};
		}

		if ($arg{HEXMAP}) {
			print $subsector->hexmap();
			print "\x0C" if $arg{PAGINATE};
		}
	}
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub import_sec {
	my $self = shift;
	my $path = shift;
	my $base = $path;

	my $c = chop $base;
	$base =~ s/.+$c//;

	open(FILE, "<$path$base.dat") or die "Unable to open sector data file ($base.dat): $!";

	while (<FILE>) {
		chomp;
		my $d = substr($_, 0, 1);
		SWITCH: {
			$self->import_sec_n($_), last SWITCH if ($d eq 'N');
			$self->import_sec_s($_), last SWITCH if ($d eq 'S');
		}
	}

	close(FILE);

	open(FILE, "<$path$base.sec") or die "Unable to open sector data file ($base.sec): $!";

	while (<FILE>) {
	last if (/\.\.\.\.\+/);
	}

	while (<FILE>) {
	my	$name		= substr($_, 0,  14);
		$name	=~ s/\s+$//;
	my	$hex		= substr($_, 14, 4);
	my	$uwp		= substr($_, 19, 9);
	my	$b			= substr($_, 30, 1);
	my	$naval		= $b =~ /A/ ? 'N'
					: $b =~ /N/ ? 'N'
					: '';
	my	$scout		= $b =~ /A/ ? 'S'
					: $b =~ /S/ ? 'S'
					: '';
	my	$remark		= substr($_, 32, 15);
		$remark	=~ s/\s+$//;
	my	$pbg		= substr($_, 51, 3);
	my 	$giant		= substr($pbg, 2, 1);
		$giant	= $giant ? 1 : 0;
	my	$zone		= substr($_, 48, 1);
	my	$world		= Traveller::World->new (
						NAME		=> $name,
						LOCATION	=> $hex,
						UWP			=> $uwp,
						NAVAL		=> $naval,
						SCOUT		=> $scout,
						GIANT		=> $giant,
						REMARK		=> $remark,
						ZONE		=> $zone);

	my	$x			= substr($hex, 0, 2);
	my	$y			= substr($hex, 2, 2);
	my	$quadrant	= chr(64 + POSIX::ceil($x / 8) + 4 * (POSIX::ceil($y / 10) - 1));
#print "$hex -> ", Traveller::Subsector::local_hex($hex), "\n";

	push(@{ $self->subsector_at($quadrant)->{WORLDS} }, $world);
	}

	close(FILE);
}

sub import_sec_n {
	my $self = shift;
	my $line = shift;

	$line =~ s/N://;

	$self->name($line);
}

sub import_sec_s {
	my $self = shift;
	my $line = shift;

    if ($line =~ /S:(.):(.+)/) {
		my $subsector = Traveller::Subsector->new(QUADRANT => uc($1), NAME => $2);
		$self->subsector_at($1) = $subsector;
    }
}

sub _quadrants {
	return ('A' .. 'P');
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;
