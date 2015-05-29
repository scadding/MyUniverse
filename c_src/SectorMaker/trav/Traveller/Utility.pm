package Traveller::Utility;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Utility.pm
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

Traveller::Utility - Utility functions.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2002 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller::Utility;

=head1 METHODS

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# =head2 range();
# 
# Given a number N, returns a number between zero and N-1.
# 
# =cut

my %ENHEX = (
	0	=> '0',
	1	=> '1',
	2	=> '2',
	3	=> '3',
	4	=> '4',
	5	=> '5',
	6	=> '6',
	7	=> '7',
	8	=> '8',
	9	=> '9',
	10	=> 'A',
	11	=> 'B',
	12	=> 'C',
	13	=> 'D',
	14	=> 'E',
	15	=> 'F',
	16	=> 'G',
	17	=> 'H',
	18	=> 'J',
	19	=> 'K',
	20	=> 'L',
	21	=> 'M',
	22	=> 'N',
	23	=> 'P',
	24	=> 'Q',
	25	=> 'R',
	26	=> 'S',
	27	=> 'T',
	28	=> 'U',
	29	=> 'V',
	30	=> 'W',
	31	=> 'X',
	32	=> 'Y',
	33	=> 'Z',
);

my %DEHEX = (
	'0'	=> 0,
	'1'	=> 1,
	'2'	=> 2,
	'3'	=> 3,
	'4'	=> 4,
	'5'	=> 5,
	'6'	=> 6,
	'7'	=> 7,
	'8'	=> 8,
	'9'	=> 9,
	'A'	=> 10,
	'B'	=> 11,
	'C'	=> 12,
	'D'	=> 13,
	'E'	=> 14,
	'F'	=> 15,
	'G'	=> 16,
	'H'	=> 17,
	'J'	=> 18,
	'K'	=> 19,
	'L'	=> 20,
	'M'	=> 21,
	'N'	=> 22,
	'P'	=> 23,
	'Q'	=> 24,
	'R'	=> 25,
	'S'	=> 26,
	'T'	=> 27,
	'U'	=> 28,
	'V'	=> 29,
	'W'	=> 30,
	'X'	=> 31,
	'Y'	=> 32,
	'Z'	=> 33,
);

sub enhex {
	return $ENHEX{scalar(shift)};
}

sub dehex {
	return $DEHEX{uc(scalar(shift))};
}

sub probability {
	my $hash = shift;

	my $key;

	my $n = 0;
	my $m = rand();
	foreach my $k (keys %$hash) {
		$key = $k;
		$n += $hash->{$k};
		last if $m < $n;
	}

	return $key;
}

=head1 SEE ALSO

Traveller.pm

=head1 AUTHOR

Paul Sanford Toney <signalgk@gmail.com>

=cut

1;
