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

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use Traveller::Dice;
use Traveller::Utility;
use Traveller::Career;
use Traveller::Character;
use Traveller::World;
use Traveller::Sector;
use Traveller::Subsector;
use Traveller::Animal;
use Traveller::Trade;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head1 NAME

Traveller.pm - A perl module for Travller.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2003 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller;

=head1 DESCRIPTION

The Traveller.pm module is a gateway to a set of perl classes and tools for the Traveller roleplaying game, first published by Game Designers' Workshop in 1977 and recently reprinted as a series of collector's editions by Far Future Enterprises.

=head1 SEE ALSO

Blar blar blar.

=head1 AUTHOR

Paul Sanford Toney <signalgk@gmail.com>

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;
