package Traveller::Dice;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Dice.pm
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

=head1 NAME

Traveller::Dice - Functions for generating random numbers.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2002 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller::Dice;
 
 my $n;
 
 $n = Traveller::Dice::roll();				# rolls 1d
 $n = Traveller::Dice::roll(1);				# same thing
 $n = Traveller::Dice::roll(2);				# rolls 2d
 $n = Traveller::Dice::roll(2, 2);			# rolls 2d+2
 $n = Traveller::Dice::roll(2, 2, 2, 12);	# rolls 2d+2 but makes sure
 											  the result is between two
 											  and twelve, inclusive 
 $n = Traveller::Dice::range(3);			# returns a number between
 											  zero and two

=head1 DESCRIPTION

The Traveller::Dice package is used for simulating die rolls and
generating random numbers.

=head1 METHODS

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head2 range();

Given a number N, returns a number between zero and N-1.

=cut

sub range {
	my $m = shift || 0;
	return int(rand($m));
}

=head2 roll();

Simulates die rolls. The first argument is the number of dice to use;
default is 1. The second argument is the die modifier, applied to the
total; default is 0. The final two optional arguments are treated as
minimum and maximum values for the final result.

=cut

sub roll {
	my $d = shift || 1;
	my $m = shift || 0;
	my $min = shift;
	my $max = shift;

	my $r = 0;

	return 0
		unless $d > 0;

	for (my $k = 0; $k < $d; $k++) {
		$r += (1 + range(6));
	}

	$r += $m;

	if (defined($min)) {
		$r = $r >= $min ? $r : $min;
	}

	if (defined($max)) {
		$r = $r <= $max ? $r : $max;
	}

	return int($r);
}

=head1 SEE ALSO

Traveller.pm

=head1 AUTHOR

Paul Sanford Toney <signalgk@gmail.com>

=cut

1;
