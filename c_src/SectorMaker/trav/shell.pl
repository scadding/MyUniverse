#!/usr/bin/perl -w

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Traveller.pm
#	author:			paul sanford toney <signalgk@gmail.com>
#
#	description:	traveller perl module
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
use warnings;

use Getopt::Long;

use Traveller;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my %OPTS = (
	"tool"		=> undef,

	# generic options

	"profile"	=> undef,

	# animal options

	"terrain"	=> "clear",
	"extended"	=> undef,

	# trade options

	"cargo"		=> undef,
);

GetOptions(
	"tool=s"	=> \$OPTS{"tool"},

	"profile=s"	=> \$OPTS{"profile"},
	"terrain=s"	=> \$OPTS{"terrain"},
	"extended!"	=> \$OPTS{"extended"},

	"cargo=s"	=> \$OPTS{"cargo"},
);

# foreach my $k (keys %OPTS) {
# 	print "SHELL: $k = " . $OPTS{$k} . "\n";
# }

my %TOOL = (
	"animal"			=> sub {

		my $profile = Traveller::World->new(
			PROFILE => $OPTS{"profile"}
		);

		print Traveller::Animal->generate(
			WORLD		=> $profile,
			TERRAIN		=> $OPTS{"terrain"},
			EXTENDED	=> $OPTS{"extended"},
		);
	},

	"bk7.trade.cargo"	=> sub {

		if (!defined($OPTS{"profile"})) {
			print "Sourceworld not provided.\n";
			return;
		}

		my $profile = Traveller::World->new(
			PROFILE => $OPTS{"profile"}
		);

		my $cargo = Traveller::Trade::Cargo->new;
		$cargo->generate($profile);

		print $cargo->listing, "\n";
	},

	"bk7.trade.value"	=> sub {

		if (!defined($OPTS{"profile"})) {
			print "Marketworld not provided.\n";
			return;
		}
		if (!defined($OPTS{"cargo"})) {
			print "Cargo identity not provided.\n";
			return;
		}

		my $profile = Traveller::World->new(
			PROFILE => $OPTS{"profile"}
		);

		$OPTS{"cargo"} =~ /(.)-(.) (.+) Cr([0-9,]+)/i;
		my @foo = split(" ", $3);
		my $cargo = Traveller::Trade::Cargo->new(
			PORT	=> $1,
			TECH	=> $2,
			CLAS	=> \@foo,
			COST	=> $4,
		);
		print "Value Cr", $cargo->value($profile), "\n";
	},

);

if ($TOOL{$OPTS{"tool"}}) {
	$TOOL{$OPTS{"tool"}}->();
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

exit(0);
