package Traveller::Starchart;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Starchart.pm
#	author:			paul sanford toney <signalgk@gmail.com>
#
#	description:	
#
#	%license%
#
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head1 NAME

Traveller::Starchart - A module for handling a collection of sectors.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2002 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller::Starchart;
 
 my	$starchart = Traveller::Starchart->new;
	$starchart->sector_at(0, 0, Traveller::Sector->new)

=head1 DESCRIPTION

The Traveller::Starchart module... .

=head1 SEE ALSO

Blar blar blar.

=head1 METHODS

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use strict;

use Traveller::Sector;
use Traveller::Subsector;
use Traveller::World;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

package Traveller::Starchart;

sub new {
	my $pkg = shift;
	my $obj = bless {
		'sectors'	=> {},
	}, $pkg;
	return $obj;
}

sub sector_at {
	my $self = shift;
	my $x = shift;
	my $y = shift;
	if (@_) { $self->{'sectors'}{"$x,$y"} = shift; }
	return $self->{'sectors'}{"$x,$y"};
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub neighborhood {
	my	$location = shift;
	my	$distance = shift;
	my	$inclusive = shift || undef;

	my	$hx = substr($location, 0, 2);
	my	$hy = substr($location, 2, 2);

	my	$maxy = $distance * 2 + 1;

	for (my $curx = ($hx - $distance); $curx <= ($hx + $distance); $curx++) {
		my	$ry = $maxy - abs($curx - $hx);

		my	$upperlimy;
		my	$lowerlimy;

		if ($hx % 2) {
			if ($curx % 2) {
				$upperlimy = $hy - (0.5 * $ry) + 0.5;
				$lowerlimy = $hy + (0.5 * $ry) - 0.5;
			}
			else {
				$upperlimy = $hy - (0.5 * $ry);
				$lowerlimy = $maxy % 2
								? $hy + (0.5 * $ry) - 1.0
								: $hy + (0.5 * $ry);
			}
		}
		else {
			if ($curx % 2) {
				$upperlimy = $maxy % 2
								? $hy - (0.5 * $ry) + 1.0
								: $hy - (0.5 * $ry);
				$lowerlimy = $hy + (0.5 * $ry);
			}
			else {
				$upperlimy = $hy - (0.5 * $ry) + 0.5;
				$lowerlimy = $hy + (0.5 * $ry) - 0.5;
			}
		}

		for (my $cury = $upperlimy; $cury <= $lowerlimy; $cury++) {
			my	$resx = $curx;
			my	$resy = $cury;

			next unless	$inclusive
					||	(
							($resx == ($hx - $distance)) 
						||	($resx == ($hx + $distance))
						||	(
								($resx > ($hx-$distance))
							&&	($resx < ($hx+$distance)))
						&&	(
								($resy == $upperlimy)
							||	($resy == $lowerlimy)));

			$resx += 32 if $resx < 0;
			$resy += 40 if $resy < 0;

			$resx = $resx % 32;
			$resy = $resy % 40;
			$resx = 32 unless $resx % 32;
			$resy = 40 unless $resy % 40;

#			printf("%02d%02d\n", $resx, $resy);
		}
	}
}

sub location_to_global {
	my	$location	= shift;
	my	$sector_x	= shift;	# x offset of sector
	my	$sector_y	= shift;	# y offset of sector

	my	$system_x	= 0;		# x offset of system within sector
	my	$system_y	= 0;		# y offset of system within sector

	($system_x, $system_y) = ($1, $2) if $location =~ /(\d\d)(\d\d)/;

	my	$xabs = 62832 + (32 * $sector_x) + $system_x;
	my	$yabs = 10000 + (40 * $sector_y) + $system_y - 40;

	return ($xabs, $yabs);
	return "ring $yabs/ray $xabs";
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;
