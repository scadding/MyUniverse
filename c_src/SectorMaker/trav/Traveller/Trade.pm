package Traveller::Trade;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Trade.pm
#	author:			paul sanford toney <signalgk@gmail.com>
#
#	description:	
#
#	%license%
#
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head1 NAME

Traveller::Trade - A module for handling Traveller trade, according to
Book 7: Merchant Prince.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2002 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller::Trade;

=head1 DESCRIPTION

The Traveller::Trade module can... .

=head1 SEE ALSO

Blar blar blar.

=head1 METHODS

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use strict;

use Traveller::World;
use Traveller::Utility;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my @VALUE;
my %CARGO;
my %ALIEN;
my %TBL1B1;
my %TBL1B3;
my %TBL2B1;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

package Traveller::Trade::Cargo;

use Data::Dumper;

sub new {
	my $pkg = shift;
	my $class = bless {
		PORT	=> undef,
		TECH	=> undef,
		CLAS	=> undef,
		COST	=> undef,
		@_
	}, $pkg;
	return $class;
}

sub generate {
	my $self = shift;
	my $w = shift;

	# 1A:	find source world trade data

	# 1A1:	trade classification

	my @tc = Traveller::World::classification($w->{ATTR});

	# 1A2:	starport type

	my $sp = $w->{ATTR}{PORT};

	# 1A3:	tech level

	my $tl = $w->{ATTR}{TECH};
	
	# 1B:	find cost of goods

	my $bc = 4000;

	# 1B1:	trade price modifiers
	foreach (@tc) {
		$bc += 1000 * $TBL1B1{$_};
	}

	# 1B2:	tech level modifiers
	$bc += 100 * $tl;

	# 1B3:	starport modifiers
	$bc += 1000 * $TBL1B3{$sp};

	$self->{PORT}	= $sp;
	$self->{TECH}	= $tl;
	$self->{CLAS}	= \@tc;
	$self->{COST}	= $bc;

	return $self;
}

sub value {
	my $self = shift;
	my $w = shift;
	my $s = shift || undef;

	# 2A:	find market world trade data

	# 2A1:	trade classification

	my @tc = Traveller::World::classification($w->{ATTR});

	# 2A2:	starport type

	my $sp = $w->{ATTR}{PORT};

	# 2A3:	tech level

	my $tl = $w->{ATTR}{TECH};

	# 2B:	find price for goods

	my $bp = 5000;

	# 2B1:	trade price modifiers

	if ($self->{CLAS}) {
		foreach my $stc (sort @{ $self->{CLAS} }) {
			foreach my $mtc (sort @tc) {
				if ($TBL2B1{$stc}{$mtc}) {
					my $mod = 1000 * $TBL2B1{$stc}{$mtc};
					$bp += $mod;
				}
			}
		}
	}

	# 2B2:	tech level modifiers

	$bp += (((Traveller::Utility::dehex($self->{TECH}) - $tl) * 10) / 100) * $bp;

	# 2B3:	select broker

	if ($s) {
	if ($$s{'Broker'}) {
		my $fee = (($$s{'Broker'} * 5) / 100) * $bp;
		$bp -=$fee;
#print"Brokerage fee: Cr$fee\n";
	}
	}

	return $bp;
}

sub listing {
	my $self = shift;
	return sprintf("%s-%X %s Cr%d",
					$self->{PORT},
					$self->{TECH},
					join(" ", @{ $self->{CLAS} }),
					$self->{COST});
}

package Traveller::Trade;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#foreach my $k ('major', 'minor', 'incidental') {
#	my $m = $CARGO{$k}[$w->attr('popu')];
#	next unless $m;
#	my $r = Traveller::Dice::roll (1, $m, 0);
#	print "$r $k cargos\n";
#
#	foreach my $j (1..$r) {
#
#		my $dt = ($k eq 'major') ? 10 :
#				 ($k eq 'minor') ?  5 : 1;
#		$dt *= Traveller::Dice::roll (3);
#
#		print "$j: $dt tons\n";
#
#	}
#}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head1 AUTHOR

Paul Sanford Toney <signalgk@gmail.com>

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#  package startup code

{
	my $line = <Traveller::Trade::DATA>;
	chomp $line;
	@VALUE = split /,/, $line;
}

foreach (1..3) {
	my $line = <Traveller::Trade::DATA>;
	chomp $line;
	my ($a, $b) = split /\|/, $line;
	my @f = split /,/, $b;
	$CARGO{$a} = [ @f ];
}

foreach (1..8) {
	my $line = <Traveller::Trade::DATA>;
	chomp $line;
	my ($a, $b) = split /\|/, $line;
	my %f;
	foreach (split /;/, $b) {
		my ($c, $d) = split /,/;
		$f{$c} = $d;
	}
	$ALIEN{$a} = { %f };
}

{
	my $line = <Traveller::Trade::DATA>;
	chomp $line;
	my @tuple = split /;/, $line;
	foreach my $t (@tuple) {
		my ($a, $b) = split /,/, $t;
		$TBL1B1{$a} = $b;
	}
}

{
	my $line = <Traveller::Trade::DATA>;
	chomp $line;
	my @tuple = split /;/, $line;
	foreach my $t (@tuple) {
		my ($a, $b) = split /,/, $t;
		$TBL1B3{$a} = $b;
	}
}

foreach (1..15) {
	my $line = <Traveller::Trade::DATA>;
	chomp $line;
	my ($a, $b) = split /\|/, $line;
	my %f;
	foreach (split /;/, $b) {
		my ($c, $d) = split /,/;
		$f{$c} = $d;
	}
	$TBL2B1{$a} = { %f };
}

1;

__DATA__
,,40,50,70,80,90,100,110,120,130,150,170,200,300,400
major|,-4,-2,-1,0,1,2,3,4,5,6
minor|,-4,-1,0,1,2,3,4,5,6,7
incidental|,,,,,,-3,-3,-2,-2,-1
As|Kk,-2;Va,1
Dr|Zh,2
Hv|As,1;Im,-2
Im|Zh,-1
Kk|Va,-2
So|Hv,1;Im,-1
Va|Kk,-4
Zh|As,1;Dr,1;Im,-2
Ag,-1;As,-1;Ba,+1;De,+1;Fl,+1;Hi,-1;Ic,0;In,-1;Lo,+1;Na,0;Ni,+1;Po,+1;Ri,+1;Va,+1;Wa,0
A,-1;B,0;C,1;D,2;E,3;X,5
Ag|Ag,1;As,1;De,1;Hi,1;In,1;Lo,1;Na,1;Ri,1
As|As,1;In,1;Na,1;Ri,1;Va,1
Ba|Ba,1;In,1
De|De,1;Na,1
Fl|Fl,1;In,1
Hi|Hi,1;Lo,1;Ri,1
Ic|In,1
In|Ag,1;As,1;De,1;Fl,1;Hi,1;In,1;Ni,1;Po,1;Ri,1;Va,1
Lo|In,1;Ri,1
Na|As,1;De,1;Va,1
Ni|In,1;Ni,-1
Po|Po,-1
Ri|Ag,1;De,1;Hi,1;In,1;Na,1;Ri,1
Va|As,1;In,1;Va,1
Wa|In,1;Ri,1;Wa,1
# __DATA__
