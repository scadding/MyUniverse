package Traveller::WorldExtended;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			WorldExtended.pm
#	author:			paul sanford toney <signalgk@gmail.com>
#
#	description:	
#
#	%license%
#
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use strict;

=head1 NAME

Traveller::WorldExtended - A class representing Traveller worlds.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2002 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller::WorldExtended;
 $w = Traveller::WorldExtended->new();
 $w->generate();
 print $w->listing();

=head1 DESCRIPTION

The Traveller::WorldExtended class is used for representing worlds in the
Traveller game, and can also be used to generate worlds according to the
method presented in Traveller Book 6, Scouts.

=head1 SEE ALSO

Blah blaj blah.

=head1 METHODS

=cut

use Traveller::Dice;
use Traveller::World;

use Data::Dumper;
#$Data::Dumper::Terse = 1;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my %TBL;

use constant FLAG_GASGIANT		=> '@';
use constant FLAG_PLANETOIDBELT	=> ':';
use constant FLAG_PLANET		=> '+';

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	package Traveller::WorldExtended::Star;
	
	sub new {
		my $pkg = shift;
		my %arg = @_;
		my $obj = bless {
			'type'	=> $arg{TYPE} || undef,	# star type
			'size'	=> $arg{SIZE} || undef,	# star size
			'decl'	=> $arg{DECL} || undef,	# star decimal classification

			'orbn'	=> undef,					# hash or orbit zones

			'coor'	=> undef,					# orbit number for companion star
			'orbd'	=> undef,					# orbit distance for far companion star

			'gaqu'	=> 0,						# gas giant quantity
			'plqu'	=> 0,						# planetoid belt quantity
			
		}, $pkg;
		return $obj;
	}

	sub generate {
		my $self = shift;
		my $main = shift;
		my %arg = (
			PRIMARY		=> 0,
			TYPE_DM		=> 0,
			SIZE_DM		=> 0,
			INDEX		=> 0,
			@_
		);
		my $m;
		my $roll_prty;
		my $roll_prsi;
		
		#	determine size and type

		if ($arg{PRIMARY}) {
			$m = ($main->attr('popu') >= 8 || ($main->attr('atmo') >= 4 && $main->attr('popu') <= 9)) ? 4: 0;
			$roll_prty = Traveller::Dice::roll(2, $m, 0, 12);
			$roll_prsi = Traveller::Dice::roll(2, $m, 0, 12);
		}
		else {
			$roll_prty = Traveller::Dice::roll(2, $arg{TYPE_DM}, 0, 12);
			$roll_prsi = Traveller::Dice::roll(2, $arg{SIZE_DM}, 0, 12);

			$m = $arg{INDEX} == 3 ? 4 : 0;
			$self->{'coor'} = $TBL{'coor'}[Traveller::Dice::roll(2, $m, 0, 12)];

			if ($self->{'coor'} =~ /\+/) {
				my ($m, $d) = split /\+/, $self->{'coor'};
				$self->{'coor'} = Traveller::Dice::roll($d, $m);
			}
		}

		$self->{'type'} = $TBL{'prty'}[$roll_prty];
		$self->{'size'} = $TBL{'prsi'}[$roll_prsi];
		$self->{'decl'} = Traveller::Dice::roll() <= 3 ? 0 : 5;

		$self->{'size'} = 'v' if (
			($self->{'size'} =~ /iv/ && (($self->{'type'} =~ /k/ && $self->{'decl'} >= 5) || ($self->{'type'} =~ /m/)))
		||	($self->{'size'} =~ /vi/ && (($self->{'type'} =~ /f/ && $self->{'decl'} <= 4) || ($self->{'type'} =~ /[ba]/))));

		#	number of orbits available for star

		SWITCH: {
			if ($self->{'size'} eq 'iii')	{ $m  = 4; last SWITCH; }
			if ($self->{'size'} eq 'ia')	{ $m  = 8; last SWITCH; }
			if ($self->{'size'} eq 'ib')	{ $m  = 8; last SWITCH; }
			if ($self->{'size'} eq 'ii')	{ $m  = 8; last SWITCH; }
		}

		SWITCH: {
			if ($self->{'type'} eq 'm')		{ $m -= 4; last SWITCH; }
			if ($self->{'type'} eq 'k')		{ $m -= 2; last SWITCH; }
		}

		$self->{'maor'} = Traveller::Dice::roll(2, $m, 0);
		$self->{'maor'} = 0 if (defined $self->{'coor'} && $self->{'coor'} =~ /c/);

		$self->{'orbn'} = {};

		#	unavailable, inner, habitable, and outer zones

		my $n = 0;
		foreach my $zone (Traveller::WorldExtended::get_zones($self->size(), $self->type(), $self->decl())) {
			$self->orbit($n, Traveller::WorldExtended::new_orbit($zone)) if $zone;
			$n++;
		}

		if (defined($self->{'coor'})) {

			SWITCH: {
				if ($self->{'coor'} =~ /c/) {	# close companion
					$self->{'orbn'} = {};		# no orbits
					last SWITCH;
				}

				if ($self->{'coor'} =~ /f/) {	# far companion
												# determine distance
					# xxx determine if far companion has binary companion
					$self->{'orbd'} = 1000 * Traveller::Dice::roll(1);
					last SWITCH;
				}
				
				# normal companion
				#	determine orbit
				if ($self->{'coor'} =~ /\+/) {
					my ($m, $d) = split(/\+/, $self->{'coor'});
					$self->{'coor'} = Traveller::Dice::roll($d, $m);
				}
				#	no orbits
				$self->{'orbn'} = {};
			}
		}
		
		return ($roll_prty, $roll_prsi);
	}

	sub attr {
		my $self = shift;
		return uc($self->{'type'} . $self->{'decl'} . ' ' . $self->{'size'});
	}

	sub orbit {
		my $self = shift;
		my $n = shift;

		$self->{'orbn'}{$n} = shift if (@_);
		return $self->{'orbn'}{$n};
	}

	sub size {
		my $self = shift;
	
		$self->{'size'} = shift if (@_);
		return $self->{'size'};
	}
	
	sub type {
		my $self = shift;
	
		$self->{'type'} = shift if (@_);
		return $self->{'type'};
	}
	
	sub decl {
		my $self = shift;
	
		$self->{'decl'} = shift if (@_);
		return $self->{'decl'};
	}
	
	sub orbit_zone {
		my $self = shift;
		my $n = shift;

		$self->{'orbn'}{$n}{'zone'} = shift if (@_);
		return $self->{'orbn'}{$n}{'zone'};
	}

	sub orbit_flag {
		my $self = shift;
		my $n = shift;

		$self->{'orbn'}{$n}{'flag'} = shift if (@_);
		return $self->{'orbn'}{$n}{'flag'};
	}

	sub orbit_body {
		my $self = shift;
		my $n = shift;

		$self->{'orbn'}{$n}{'body'} = shift if (@_);
		return $self->{'orbn'}{$n}{'body'};
	}

	sub orbit_iter {
		my $self = shift;
		return sort { $a <=> $b } keys %{ $self->{'orbn'} };
	}

	sub has_orbits {
		my $self = shift;
		return scalar(keys %{ $self->{'orbn'} });
	}

	sub num_orbits_available {
		my $self = shift;
		my $type = shift || undef;
		my $n = 0;

		foreach my $r ($self->orbit_iter()) {

			next unless ($self->orbit_flag($r) =~ /[iho]/);

			if ($type) {
				$n++ if ($self->orbit_flag($r) =~ /[$type]/);
			}
			else {
				$n++;
			}
		}

		return $n;
	}

	sub orbits_of_type {
		my $self = shift;
		my $type = shift || '';
		my @orbn;

		foreach my $r ($self->orbit_iter()) {
			push @orbn, $r if ($self->orbit_flag($r) =~ /[$type]/);
		}

		return @orbn;
	}

	sub pick_avail_orbit {
		my $self = shift;
		my $type = shift || 'iho';
	
		my $n = $self->num_orbits_available($type);
	#print "[pick_avail_orbit] $n orbits available for $type\n";
		return undef unless $n;
	
		my $m = 1 + Traveller::Dice::range($n);
		my $p = undef;
	
	#print "[pick_avail_orbit] \$m is $m\n";
		foreach my $r ($self->orbit_iter()) {
	
			if ($self->orbit_flag($r) !~ /[iho]/)	{ next; } #print "[pick_avail_orbit] skipped $r (unavailable orbit)\n"; next; }
			if ($self->orbit_flag($r) !~ /[$type]/)	{ next; } #{ print "[pick_avail_orbit] skipped $r (orbit type mismatch)\n"; next; }
	
			$p = $r;
	#print "[pick_avail_orbit] ...$p\n";
			$m--;
			last unless $m > 0;
		}
	#print "[pick_avail_orbit] pick $p for $type\n" if defined $p;
	#print "[pick_avail_orbit] pick failed for $type\n" unless defined $p;
		return $p;
	}

	sub dump_orbits {
		my $self = shift;
		my @orbs;

		return unless $self->has_orbits();

		print "+---------------------+\n";

		foreach my $r ($self->orbit_iter()) {
			printf("%5.2f %s %s\n", $r, $self->orbit_zone($r), $self->orbit_flag($r));
		}

		print "  orbits available: " . $self->num_orbits_available() . "\n";
		print "i orbits available: " . $self->num_orbits_available('i') . "\n";
		print "h orbits available: " . $self->num_orbits_available('h') . "\n";
		print "o orbits available: " . $self->num_orbits_available('o') . "\n";

		print "   zone boundary .: " . Traveller::WorldExtended::zone_boundary($self, '.') . "\n";
		print "   zone boundary -: " . Traveller::WorldExtended::zone_boundary($self, '-') . "\n";
		print "   zone boundary i: " . Traveller::WorldExtended::zone_boundary($self, 'i') . "\n";
		print "   zone boundary h: " . Traveller::WorldExtended::zone_boundary($self, 'h') . "\n";
		print "   zone boundary o: " . Traveller::WorldExtended::zone_boundary($self, 'o') . "\n";

		print "+---------------------+\n";
	}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	package Traveller::WorldExtended::World;
	
	sub new {
		my $pkg = shift;
		my %arg = @_;
		my $obj = bless {
			'name'	=>	'',
			'attr'	=> {
				'size'	=> 0,
				'atmo'	=> 0,
				'hydr'	=> 0,
				'popu'	=> 0,
				'subg'	=> 0,
				'subl'	=> 0,
				'subt'	=> 0,
				'subp'	=> 0,
				'subf'	=> [],
				},
			'type'	=> $arg{TYPE} || 'planet',
			'orbs'	=> {},
			'main'	=> 0
		}, $pkg;
		return $obj;
	}
	
	sub generate {
		my $self = shift;
		my $stty = shift;
		my $zone = shift;
		my $orbn = shift;
		my $main = shift || undef;

		my $m;

		# generate world within system

		#	orbit location

		$self->{'zone'} = $zone;	# orbit zone
		$self->{'orbn'} = $orbn;	# orbit number
		$self->{'orbd'} = 0;		# orbit distance

		return unless $self->{'type'} eq 'planet';

		#	size
		
		$m = -2;
		$m -= 5 if ($orbn == 0);
		$m -= 4 if ($orbn == 1);
		$m -= 2 if ($orbn == 2);
		$m -= 2 if ($stty eq 'm');
	
		$self->{'attr'}{'size'} = Traveller::Dice::roll(2, $m, 0, 10);
	
		#	atmosphere
		
		$m = -7 + $self->{'attr'}{'size'};
		$m -= 2 if ($zone eq 'i');
		$m -= 4 if ($zone eq 'o');
	
		$self->{'attr'}{'atmo'} = Traveller::Dice::roll(2, $m, 0, 15);
	
		if ($self->{'attr'}{'size'} == 0) {
			$self->{'attr'}{'atmo'} = 0;
		}
	
		# xxx need to handle exotic atmo case
	
		#	hydrographics
		
		$m = -7 + $self->{'attr'}{'size'};
		$m -= 2 if ($zone eq 'o');
		$m -= 4 if ($self->{'attr'}{'atmo'} <= 1
				|| 	$self->{'attr'}{'atmo'} >= 10);
	
		$self->{'attr'}{'hydr'} = Traveller::Dice::roll(2, $m, 0, 10);
	
		if ($self->{'attr'}{'size'} <= 1) {
			$self->{'attr'}{'hydr'} = 0;
		}
	
		#	population
	
		$m = -2;
		$m -= 5 if ($zone eq 'i');
		$m -= 3 if ($zone eq 'o');
		$m -= 2 if ($self->{'attr'}{'atmo'} != 0
				&&  $self->{'attr'}{'atmo'} != 5
				&&  $self->{'attr'}{'atmo'} != 6
				&&  $self->{'attr'}{'atmo'} != 8);
	
		$self->{'attr'}{'popu'} = Traveller::Dice::roll(2, $m, 0, 10);
	
		if ($main) {
			$self->{'attr'}{'popu'} = $main->attr('popu') - 1
				if $self->{'attr'}{'popu'} >= $main->attr('popu');
		}

		$self->{'attr'}{'size'} = int($self->{'attr'}{'size'}); # !!! hack
	}

	sub attr {
		my $self = shift;
		my $attr = shift;
		if (@_) { $self->{'attr'}{$attr} = shift; }
		return $self->{'attr'}{$attr};
	}

	sub profile {
		my $self = shift;
		return Traveller::WorldExtended::demangled_profile($self->{'attr'});
	}

	sub listing {
		my $self = shift;

		my $or = '';
		my $na = '';
		my $at = '';
		my $re = '';
		my $zo = '';

		$or = $self->{'orbn'};

		SWITCH: {
			if ($self->{'type'} =~ /[large|small] gas giant/) {
				$na = $self->{'name'} || 'Unnamed';
				$at = '';
				$re = ucfirst($self->{'type'});
				last SWITCH;
			}
			if ($self->{'type'} =~ /planetoid belt/) {
				$na = $self->{'name'} || 'Unnamed';
				$at = '';
				$re = ucfirst($self->{'type'});
				last SWITCH;
			}
			if ($self->{'type'} =~ /planet/) {	
				$na = $self->{'name'} || 'World';
				$at = $self->profile();
				$re = $self->{'main'} ? join ' ', Traveller::World::classification($self->{'attr'}) : '';
				last SWITCH;
			}
		}

		$zo = $self->{'zone'} =~ /h/ ? '*' : '';

		$^A = '';
formline <<EOL, $zo, $or, $na, $at, $re;
@@##.#         @<<<<<<<<<<<<< @<<<<<<<< @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
EOL
		my $listing .= $^A;
	
		return $listing;
	}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	package Traveller::WorldExtended::Satellite;
	
	sub new {
		my $pkg = shift;
		my %arg = @_;
		my $obj = bless {
			'attr'	=> {
				'size'	=> 0,
				'atmo'	=> 0,
				'hydr'	=> 0,
				'popu'	=> 0,
				'subg'	=> 0,
				'subl'	=> 0,
				'subt'	=> 0,
				'subp'	=> 0,
				'subf'	=> [],
				},
			'orbn'	=> '',		# orbit number
			'orbd'	=> '',		# orbit distance
		}, $pkg;
		return $obj;
	}
	
	sub generate {
		my $self = shift;
		my $wrld = shift;
		my $zone = shift;
		my $orbn = shift;
		my $main = shift || undef;

#print $wrld->attr(), " in orbit $orbn zone $zone\n";

		my ($d, $m);

		# generate satellite for world

		#	size

		SWITCH: {
			if ($wrld->{'type'} =~ /small gas giant/)	{ $self->{'attr'}{'size'} = Traveller::Dice::roll(2, -6); last SWITCH;}
			if ($wrld->{'type'} =~ /large gas giant/)	{ $self->{'attr'}{'size'} = Traveller::Dice::roll(2, -4); last SWITCH;}
			$self->{'attr'}{'size'} = int($wrld->{'attr'}{'size'} - Traveller::Dice::roll(1));
		}

		if ($self->{'attr'}{'size'} == 0) {
			$self->{'attr'}{'size'} = 'r';
		}
		elsif ($self->{'attr'}{'size'} < 0) {
			$self->{'attr'}{'size'} = 's';
		}

		#	orbit location

		$self->{'orbn'} = $orbn;
		$self->{'orbd'} = $orbn; # xxx fix this

		#	atmosphere
		
		$m = -7;
		$m += $self->{'attr'}{'size'} unless $self->{'attr'}{'size'} =~ /[sr]/i;
		$m -= 4 if ($zone =~ /[io]/);
	
		$self->{'attr'}{'atmo'} = Traveller::Dice::roll(2, $m, 0, 15);

		if ($self->{'attr'}{'size'} =~ /[sr]/i) {
			$self->{'attr'}{'atmo'} = 0;
		}
		elsif ($self->{'attr'}{'size'} <= 1) {
			$self->{'attr'}{'atmo'} = 0;
		}
	
		# xxx need to handle exotic atmo case
	
		#	hydrographics
		
		$m = -7;
		$m += $self->{'attr'}{'size'} unless $self->{'attr'}{'size'} =~ /[sr]/i;
		$m -= 4 if ($zone =~ /o/);
		$m -= 4 if ($self->{'attr'}{'atmo'} <= 1
				|| 	$self->{'attr'}{'atmo'} >= 10);
	
		$self->{'attr'}{'hydr'} = Traveller::Dice::roll(2, $m, 0, 10);
	
		if ($zone =~ /i/) {
			$self->{'attr'}{'hydr'} = 0;
		}

		if ($self->{'attr'}{'size'} =~ /[sr]/i) {
			$self->{'attr'}{'atmo'} = 0;
		}
		elsif ($self->{'attr'}{'size'} <= 1) {
			$self->{'attr'}{'hydr'} = 0;
		}
	
		#	population
	
		$m = -2;
		$m -= 5 if ($zone =~ /i/);
		$m -= 4 if ($zone =~ /i/);

		if ($self->{'attr'}{'size'} =~ /[sr]/i) {
			;
		}
		elsif ($self->{'attr'}{'size'} <= 4) {
			$m -= 4;
		}

		$self->{'attr'}{'popu'} = Traveller::Dice::roll(2, $m, 0, 10);
	
		if ($self->{'attr'}{'atmo'} != 0
		&&  $self->{'attr'}{'atmo'} != 5
		&&  $self->{'attr'}{'atmo'} != 6
		&&  $self->{'attr'}{'atmo'} != 8) {
			$self->{'attr'}{'popu'} = 0;
		}

		if ($self->{'attr'}{'size'} =~ /[r]/i) {
			$self->{'attr'}{'popu'} = 0;
		}

		if ($main) {
			$self->{'attr'}{'popu'} = $main->attr('popu') - 1
				if ($self->{'attr'}{'popu'} >= $main->attr('popu'));
		}

		$self->{'attr'}{'size'} = int($self->{'attr'}{'size'})
			unless $self->{'attr'}{'size'} =~ /[sr]/;
	}

	sub attr {
		my $self = shift;
		my $attr = shift;
		if (@_) { $self->{'attr'}{$attr} = shift; }
		return $self->{'attr'}{$attr};
	}

	sub profile {
		my $self = shift;
		return Traveller::WorldExtended::demangled_profile($self->{'attr'});
	}

	sub listing {
		my $self = shift;

		my $or = $self->{'orbd'};
		my $na = $self->{'name'} || 'Satellite';
		my $at = $self->profile();
		my $re = join ' ', Traveller::WorldExtended::facilities($self->{'attr'});

		$^A = '';
formline <<EOL, $or, $na, $at, $re;
    @>>>>>     @<<<<<<<<<<<<< @<<<<<<<< @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
EOL
		my $listing .= $^A;
	
		return $listing;
	}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

package Traveller::WorldExtended;

=head2 $obj = Traveller::WorldExtended->new();

Constructor for the World. Returns a reference to a Traveller::WorldExtended object.

=cut

sub new {
	my $pkg = shift;
	my %arg = @_;
	my $obj = bless {
		'main'		=> $arg{MAIN} || undef,
		'verbose'	=> 0,
	}, $pkg;
	return $obj;
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub generate {
	my $self = shift;
	my %arg = (
		VERBOSE	=> 0,
		@_,
	);

	$self->{'verbose'} = $arg{VERBOSE};

	if (!$self->{'main'}) {
		$self->{'main'} = Traveller::World->new();
		$self->{'main'}->generate();
	}

	$self->generate_system();
	$self->generate_worlds();
}

sub generate_system {
	my $self = shift;
	my $m = 0;

	# determine star system details

	#	system nature

	$m = 0;
	$self->{'bana'} = $TBL{'bana'}[Traveller::Dice::roll(2, $m, 0, 12)];

	$self->remark("basic nature is " . qw/solitary binary trinary/[$self->{'bana'} - 1]);

	#	primary star type and size

	my ($type_dm, $size_dm);

	foreach (1..1) {
		my	$star = Traveller::WorldExtended::Star->new();
			($type_dm, $size_dm) = $star->generate($self->{'main'}, PRIMARY => 1);

		push (@{ $self->{'star'} }, $star);

		$self->remark("primary is $star->{'type'}$star->{'decl'} $star->{'size'} $star->{'maor'} orbits");
	}

	#	companion start type, size and orbit

	foreach (2..$self->{'bana'}) {
		my	$star = Traveller::WorldExtended::Star->new();
			$star->generate($self->{'main'}, INDEX => $_, TYPE_DM => $type_dm, SIZE_DM => $size_dm);

		push (@{ $self->{'star'} }, $star);

		$self->remark(qw/binary trinary/[$_ - 2] . " companion is $star->{'type'}$star->{'decl'} $star->{'size'} in orbit $star->{'coor'} $star->{'maor'} orbits");
	}

	my @comp_orbn;

	foreach my $star (@{ $self->{'star'} }) {
		push @comp_orbn, $star->{'coor'} if (defined $star->{'coor'} && $star->{'coor'} =~ /\d+/);
	}

	foreach my $star (@{ $self->{'star'} }) {

		foreach my $co (@comp_orbn) {
			my $n;
			my $m = ($star->orbit_iter())[0];

			$star->orbit_flag($co, '-');

			$n = int($co/2);
			foreach my $orbn ($star->orbit_iter()) {
				last if $orbn > $m;
				$star->orbit_flag($orbn, '-') if ($orbn > $n && $orbn < $co);
			}
			
			$n = $co + 2;
			foreach my $orbn ($star->orbit_iter()) {
				last if $orbn > $m;
				$star->orbit_flag($orbn, '-') if ($orbn < $n && $orbn > $co);
			}
		}

		last;
	}

	#	captured planets and empty orbits

	foreach my $star (@{ $self->{'star'} }) {

		my $n;

		next unless $star->has_orbits();

		SWITCH: {
			if ($star->{'type'} eq 'a')		{ $m = 1; last SWITCH; }
			if ($star->{'type'} eq 'b')		{ $m = 1; last SWITCH; }
			$m = 0;
		}

		$star->{'capr'} = $TBL{'capr'}[Traveller::Dice::roll(1, $m, 1, 6)];
		$star->{'caqu'} = $star->{'capr'} ? $TBL{'caqu'}[Traveller::Dice::roll(1, $m - 1, 0, 5)] : 0;

		$star->{'empr'} = $TBL{'empr'}[Traveller::Dice::roll(1, $m, 1, 6)];
		$star->{'emqu'} = $star->{'empr'} ? $TBL{'emqu'}[Traveller::Dice::roll(1, $m - 1, 0, 5)] : 0;

		$self->remark("$star->{'caqu'} captured orbits");

		foreach (1..$star->{'caqu'}) {
			# xxx can captured orbits be placed
			# beyond natural orbits and max orbits
			my $r;
			
			do {
				my	$n = Traveller::Dice::roll(2);
				my	$d = Traveller::Dice::roll(2, -7);
					$r = $n + (0.1 * $d);
			} while ($star->orbit($r));

			$star->orbit($r, Traveller::WorldExtended::new_orbit('o')); #xxx remove this line if not

			foreach my $z (qw/o h i - ./) {
				if ($r > Traveller::WorldExtended::zone_boundary($star, $z)) {
					last;
				}
				$star->orbit_zone($r, $z);
			}
			
			$self->remark(" captured orbit at $r");
		}

		$self->remark("$star->{'emqu'} empty orbits");

		if ($star->{'emqu'}) {
			my $emqu = $star->{'emqu'};				# empty orbits called for
			my $n = $star->num_orbits_available();	# number of available orbits

			$emqu = $n if ($emqu > $n);

			foreach (1..$emqu) {
				my $r = 1 + Traveller::Dice::range($n);

				foreach my $orbn ($star->orbit_iter()) {
					$r-- unless ($star->orbit($orbn));
				}

				$star->orbit_flag($r, '-');
				$n--;

				$self->remark(" empty orbit at $r");
			}
		}

	}

	#	presence, quantity, location of gas giants

	foreach my $star (@{ $self->{'star'} }) {
	
		next unless $star->has_orbits();

		$star->{'gaqu'} = ($TBL{'gapr'}[Traveller::Dice::roll(2)]) ? $TBL{'gaqu'}[Traveller::Dice::roll(2)] : 0;
	
		if ($star->{'gaqu'}) {
			my $n = $star->num_orbits_available();
			$star->{'gaqu'} = $star->{'gaqu'} > $n ? $n : $star->{'gaqu'};
			$star->{'gaqu'} = $n if ($star->{'gaqu'} > $n);
		}
	
		$self->remark("$star->{'gaqu'} gas giants");
	}

	foreach my $star (@{ $self->{'star'} }) {

		next unless $star->has_orbits();

		foreach (1..$star->{'gaqu'}) {
			my	$p = $star->pick_avail_orbit('ho');
				$p = $star->pick_avail_orbit('iho') unless defined $p;
			last unless defined $p;

			my	$t = Traveller::Dice::roll(1) >= 4 ? 'small gas giant' : 'large gas giant';
			my	$w = Traveller::WorldExtended::World->new(TYPE => $t);
				$w->generate($star->type(), $star->orbit_zone($p), $p);

			$star->orbit_flag($p, FLAG_GASGIANT);
			$star->orbit_body($p, $w);

			$self->remark(" $t at $p");
		}
	}

	#	presence, quantity, location of planetoid belts

	foreach my $star (@{ $self->{'star'} }) {

		next unless $star->has_orbits();

		$m = 0 - $star->{'gaqu'};
		$star->{'plqu'} = ($TBL{'plpr'}[Traveller::Dice::roll(2, $m)]) ? $TBL{'plqu'}[Traveller::Dice::roll(2, $m)] : 0;
	
		# xxx not sure about application of dm
	
		if ($star->{'plqu'}) {
			my $n = $star->num_orbits_available() - 2;
			$n = 0 unless $n > 0;
			$star->{'plqu'} = $n if ($star->{'plqu'} > $n);
		}
	
		$self->remark("$star->{'plqu'} planetoid belts");
	}

	foreach my $star (@{ $self->{'star'} }) {

		next unless $star->has_orbits();

		my @orbn = $star->orbits_of_type(FLAG_GASGIANT);

		foreach (1..$star->{'plqu'}) {
			my $p;

			foreach my $r (@orbn) {
#print "+ examining orbit $r\n";
				foreach my $n ($star->orbit_iter()) {
					next if $n >= $r;
#print "  $n $star->{'orbn'}{$n}\n";
# xxx placement of planetoid belts tricky
#					if ($star->{'orbn'}{$n} =~ /[iho]/) {
#						$p = $r;
##print "  $p found\n";
#						last;
#					}
				}
				last if $p;
			}

			if (!$p) {
				$p = $star->pick_avail_orbit('iho');
#print "  picking any orbit: $p\n" if $p;
			}

			if ($p) {
				my	$t = 'planetoid belt';
				my	$w = Traveller::WorldExtended::World->new(TYPE => $t);
					$w->generate($star->type(), $star->orbit_zone($p), $p);
				
				$star->orbit_flag($p, FLAG_PLANETOIDBELT);
				$star->orbit_body($p, $w);
	
				$self->remark(" $t at $p");
			}
		}
	}

	#	location of main world
	# xxx
	foreach my $star (@{ $self->{'star'} }) {
		last unless $self->{'main'};

		my $r;

		$r = Traveller::WorldExtended::zone_boundary($star, 'h');

		# xxx but if no habitable zone ... ?

		if ($self->{'main'}->attr('size') == 0
		||	$self->{'main'}->attr('atmo') <= 1
		||	$self->{'main'}->attr('atmo') >= 10) {
			$r = $star->pick_avail_orbit();
		}

		# xxx if gas giant in habitable, main world is satellite

		my	$w = Traveller::WorldExtended::World->new();
			$w->attr('port', $self->{'main'}->attr('port'));
			$w->attr('size', $self->{'main'}->attr('size'));
			$w->attr('atmo', $self->{'main'}->attr('atmo'));
			$w->attr('hydr', $self->{'main'}->attr('hydr'));
			$w->attr('popu', $self->{'main'}->attr('popu'));
			$w->attr('govt', $self->{'main'}->attr('govt'));
			$w->attr('lawl', $self->{'main'}->attr('lawl'));
			$w->attr('tech', $self->{'main'}->attr('tech'));

			$w->{'name'} = $self->{'main'}->name();
			$w->{'zone'} = 'h';
			$w->{'orbn'} = $r;
			$w->{'main'} = 1;

		$star->orbit_flag($r, FLAG_PLANET);
		$star->orbit_body($r, $w);

		$self->remark(" main world at $r");

		last;
	}

}

sub generate_worlds {
	my $self = shift;

	#	generate worlds within system

	foreach my $star (@{ $self->{'star'} }) {
		foreach my $r ($star->orbit_iter()) {
			next unless $star->orbit_flag($r) =~ /[iho]/; # xxx make into a function?

			my	$w = Traveller::WorldExtended::World->new();
				$w->generate ($star->type(), $star->orbit_zone($r), $r, $self->{'main'});

			$star->orbit_flag($r, FLAG_PLANET);
			$star->orbit_body($r, $w);

			$self->remark(" planet at $r");
		}
	}

	#	generate satellites for each planet or gas giant within system

	foreach my $star (@{ $self->{'star'} }) {
		foreach my $r ($star->orbit_iter()) {
			next unless $star->orbit_flag($r) =~ /[+@]/;

			my $w = $star->orbit_body($r);
			my $n;

			SWITCH: {
				if ($w->{'type'} =~ /planetoid belt/)	{ $n = 0; last SWITCH; }
				if ($w->{'type'} =~ /planet/)			{ $n = Traveller::Dice::roll(1, -3, 0); last SWITCH; }
				if ($w->{'type'} =~ /small gas giant/)	{ $n = Traveller::Dice::roll(2, -4, 0); last SWITCH; }
				if ($w->{'type'} =~ /large gas giant/)	{ $n = Traveller::Dice::roll(2,  0, 0); last SWITCH; }
			}

			foreach (1..$n) {
				$self->remark(" satellite at $_");
				my	$s = Traveller::WorldExtended::Satellite->new();
					$s->generate ($w, $star->orbit_zone($r), $_, $self->{'main'});

				$w->{'orbs'}->{$_} = $s;
			}
		}
	}

	#	determine additional planet and satellite characteristics

	foreach my $star (@{ $self->{'star'} }) {
		foreach my $rw ($star->orbit_iter()) {
			my $w = $star->orbit_body($rw);
			next unless $w;

			Traveller::WorldExtended::generate_subordinates($self->{'main'}, $star->orbit_zone($rw), $w->{'attr'})
				unless $w->{'main'};

			foreach my $rs (sort { $a <=> $b } keys %{ $w->{'orbs'} }) {
				my $s = $w->{'orbs'}{$rs};
				Traveller::WorldExtended::generate_subordinates($self->{'main'}, $star->orbit_zone($rw), $s->{'attr'});
			}
		}
	}

}

sub listing {
	my $self = shift;
	my %arg = (
		SUMMARY		=> 1,
		LEGEND		=> 0,
		@_
	);

	my $listing = '';

	$listing =<<EOL;
------------------------------------------------------------------------
Orbit          Name           UPP       Remarks
------------------------------------------------------------------------
EOL

#	my %comp_orbs;
#	foreach my $star (@{ $self->{'star'} }) {
#		if (defined($star->{'coor'}) && $star->{'coor'} !~ /cf/) {
#			$comp_orbs{$star->{'coor'}} = $star;
#		}
#	}

	my $m = 0;
	foreach my $star (@{ $self->{'star'} }) {
		next if ($star->{'coor'} && $star->{'coor'} !~ /f/);

		my $or = $m ? 'Companion' : 'Primary';
		my $na = $star->{'name'} || 'Unnamed';
		my $at = $star->attr();
		my $re = '';
		my $zo = '';

#		if ($m == 0) {
#			if ($self->{'star'}[1]->{'type'} eq 'c') {
#print "CLOSE COMPANION";
#			}
#		}

$^A = '';
formline <<EOL, $or, $na, $at, $re;
@<<<<<<<<<<<<< @<<<<<<<<<<<<< @<<<<<<<< @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
EOL
$listing .= $^A;

		foreach my $rw ($star->orbit_iter()) {
			SWITCH: {
				if ($star->orbit_flag($rw) =~ /\./) {
					next;
				}
				if ($star->orbit_flag($rw) =~ /-/) {
$^A = '';
formline <<EOL, $zo, $rw, 'Empty orbit', '', '';
@@##.#          @<<<<<<<<<<<<< @<<<<<<<< @<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
EOL
$listing .= $^A;
					next;
				}

				my $w = $star->orbit_body($rw);
				$listing .= $w->listing();

				foreach my $rs (sort { $a <=> $b } keys %{ $w->{'orbs'} }) {
					my $s = $w->{'orbs'}{$rs};
	 				$listing .= $s->listing ();
				}
			}
		}

		$m++;
	}

	if ($arg{LEGEND}) {
		$listing .=<<EOL;
------------------------------|||||||-|---------------------------------
			   Starport Type -+|||||| |
			  Planetary Size --+||||| |
		Planetary Atmosphere ---+|||| |
				Hydrographic ----+||| |
				  Population -----+|| |
				  Government ------+| |
				   Law Level -------+ |
				  Tech Level ---------+
EOL
	}
	else {
		$listing .=<<EOL;
------------------------------------------------------------------------
EOL
	}

	return $listing;
}

sub report {
	my $self = shift;
	print $self->listing (@_);
}

sub remark {
	my $self = shift;
	my $s = shift;
	print "$s\n" if $self->{'verbose'};
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#  static methods

sub get_zones {
	my $size = shift;
	my $type = shift;
	my $decl = shift;
	return @{ $TBL{$size eq 'd' ? "$size,$type" : "$size,$type$decl"} };
}

sub new_orbit {
	my $zone = shift;
	my $flag = shift || $zone;
	my $body = shift || undef;
	
	my $href = {
		'zone'	=> $zone,
		'flag'	=> $flag,
		'body'	=> $body,
	};
	
	return $href;
}

sub generate_subordinates {
	my $main = shift;	# ref to main world object
	my $zone = shift;	# zone classification
	my $attr = shift;	# ref to attr hash for world or satellite
	my $m;

	#	subordinate government

	$m = $main->attr('popu') >= 7 ? 2 : 0;
	$attr->{'subg'} = Traveller::Dice::roll(1, $m, 1, 6);
	$attr->{'subg'} = 6 if ($main->attr('govt') == 6);
	$attr->{'subg'} = 0 if ($main->attr('popu') == 0);
	$attr->{'subg'} = int($TBL{'subg'}[$attr->{'subg'} - 1]);

	# 	subordinate law level

	$attr->{'subl'} = Traveller::Dice::roll(1, -3) + $main->attr('lawl');
	$attr->{'subl'} = 0 unless $attr->{'subl'} >= 0;

	# 	subordinate facilities

	$attr->{'subf'} = [];

	# 		farming

	push @{ $attr->{'subf'} }, 'f'
		if ($zone =~ /h/i
		&&	$attr->{'atmo'} <= 9
		&&	$attr->{'popu'} >= 2
		&&	$attr->{'hydr'} >= 4
		&&	$attr->{'hydr'} <= 8);

	# 		mining

	push @{ $attr->{'subf'} }, 'm'
		if ($attr->{'popu'} >= 2
		&&	$main->attr('popu') >= 9
		&&	($main->attr('atmo') == 0
		  || $main->attr('atmo') == 1
		  || $main->attr('atmo') == 2
		  || $main->attr('atmo') == 4
		  || $main->attr('atmo') == 7
		  || $main->attr('atmo') == 9));

	# 		colony

	push @{ $attr->{'subf'} }, 'c'
		if ($attr->{'subg'} == 6
		&&	$attr->{'popu'} >= 5);

	# 		research laboratory

	$m = $main->attr('tech') >= 10 ? 2: 0;
	
	push @{ $attr->{'subf'} }, 'r'
		if (Traveller::Dice::roll(2, $m) >= 11
		&&	$main->attr('tech') >= 9
		&&	$main->attr('popu') >  0);

	# 		military base

	$m = $main->attr('popu') >= 8 ? 1: 0;
	$m+= $main->attr('popu') == $attr->{'atmo'} ? 2: 0;
	$m+= $main->scout() || $main->naval() ? 1 : 0;

	push @{ $attr->{'subf'} }, 'b'
		if (Traveller::Dice::roll(2, $m) >= 12
		&&	$main->attr('popu') > 0
		&&	!Traveller::World::is_po($main->{'attr'}));

	# 	subordinate tech level

	$attr->{'subt'} = $main->attr('tech') - 1;
	$attr->{'subt'} = 7 if ($attr->{'subt'} < 7 && ($attr->{'atmo'} != 5 || $attr->{'atmo'} != 6 || $attr->{'atmo'} != 8));

	# 	spaceport

	SWITCH: {
		if ($attr->{'popu'} >= 6)	{ $m =  2; last SWITCH;}
		if ($attr->{'popu'} == 1)	{ $m = -2; last SWITCH;}
		if ($attr->{'popu'} == 0)	{ $m = -3; last SWITCH;}
	}

	$attr->{'subp'} = $TBL{'subp'}[Traveller::Dice::roll(1, $m, 1, 6) - 1];
}

sub facilities {
	my $attr = shift;	# ref to attr hash for world or satellite
	my @list;
	
	foreach my $f (@{ $attr->{'subf'} }) {
		SWITCH: {
			if ($f =~ /f/)	{ push @list, 'Farming'; last SWITCH; }
			if ($f =~ /m/)	{ push @list, 'Mining'; last SWITCH; }
			if ($f =~ /c/)	{ push @list, 'Colony'; last SWITCH; }
			if ($f =~ /r/)	{ push @list, 'Research laboratory'; last SWITCH; }
			if ($f =~ /b/)	{ push @list, 'Military base'; last SWITCH; }
		}
	}

	return @list;
}

sub zone_boundary {
	my $star = shift;
	my $type = shift;

	# get natural orbit zones for star
	my @orbn = Traveller::WorldExtended::get_zones($star->size(), $star->type(), $star->decl());
	my $n;
	my $m;

	# find first $type
	for ($n = 0; $n <= $#orbn; $n++) {
		$m = $n;
		last if ($orbn[$n] eq $type);
	}

	# find last $type
	for ($n = $m; $n <= $#orbn; $n++) {
		last if ($orbn[$n] ne $type);
		$m = $n;
	}

	$m = -1 unless ($orbn[$m] eq $type);
	return $m;
}

sub demangled_profile {
	my $attr = shift;

	$attr->{'size'} = sprintf("%X", $attr->{'size'}) unless ($attr->{'size'} =~ /[rs]/i);
	$attr->{'size'} = uc($attr->{'size'});

	$attr->{'subp'} = uc($attr->{'subp'});

	return sprintf (
		"%s%s%X%X%X%X%X-%X",
		$attr->{'subp'},
		$attr->{'size'},
		$attr->{'atmo'},
		$attr->{'hydr'},
		$attr->{'popu'},
		$attr->{'subg'},
		$attr->{'subl'},
		$attr->{'subt'});
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head1 AUTHOR

Paul Sanford Toney <signalgk@gmail.com>

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#  package startup code

while (<Traveller::WorldExtended::DATA>) {
	last if /^\#/;
	chomp;
	my ($a, $b) = split /\|/;
	my @f = split /,/, $b;
	$TBL{$a} = [ @f ];
}

1;

__DATA__
bana|1,1,1,1,1,1,1,1,2,2,2,2,3
prty|b,b,a,m,m,m,m,m,k,g,f,f,f
prsi|ia,ib,ii,iii,iv,v,v,v,v,v,v,vi,d
coty|,b,a,f,f,g,g,k,k,m,m,m,m
cosi|ia,ib,ii,iii,iv,d,d,v,v,vi,d,d,d
coor|c,c,c,c,1,2,3,4+1,5+1,6+1,7+1,8+1,f
gapr|,1,1,1,1,1,1,1,1,1,0,0,0
gaqu|,1,1,1,2,2,3,3,4,4,4,5,5
plpr|1,1,1,1,1,1,1,0,0,0,0,0,0
plqu|3,2,2,2,2,2,2,1,1,1,1,1,1
sori|1,1,1,2,2,3
socl|,3,4,5,6,7,8,9,10,11,12,13
sofa|,15,20,25,30,35,40,45,50,55,60,65
soex|,75,100,125,150,175,200,225,250,275,300,325
capr|0,0,0,0,1,1
caqu|1,1,2,2,3,3
empr|0,0,0,0,1,1
emqu|1,1,2,3,3,3
subg|0,1,2,3,6,6
subp|y,y,h,g,g,f
ia,b0|,-,-,-,-,-,-,-,i,i,i,i,i,h,o
ia,b5|,-,-,-,-,-,-,i,i,i,i,i,h,o,o
ia,a0|,.,-,-,-,-,-,i,i,i,i,i,h,o,o
ia,a5|,.,-,-,-,-,-,i,i,i,i,i,h,o,o
ia,f0|,.,.,-,-,-,i,i,i,i,i,i,h,o,o
ia,f5|,.,.,-,-,-,i,i,i,i,i,h,o,o,o
ia,g0|,.,.,.,-,-,-,i,i,i,i,i,h,o,o
ia,g5|,.,.,.,.,-,-,i,i,i,i,i,h,o,o
ia,k0|,.,.,.,.,.,-,i,i,i,i,i,h,o,o
ia,k5|,.,.,.,.,.,-,i,i,i,i,i,h,o,o
ia,m0|,.,.,.,.,.,.,i,i,i,i,i,h,o,o
ia,m5|,.,.,.,.,.,.,.,i,i,i,i,h,o,o
ia,m9|,.,.,.,.,.,.,.,i,i,i,i,h,o,o
ib,b0|,-,-,-,-,-,-,-,i,i,i,i,i,h,o
ib,b5|,-,-,-,-,-,i,i,i,i,i,h,o,o,o
ib,a0|,-,-,-,-,i,i,i,i,i,i,h,o,o,o
ib,a5|,-,-,-,-,i,i,i,i,i,h,o,o,o,o
ib,f0|,-,-,-,-,i,i,i,i,i,h,o,o,o,o
ib,f5|,-,-,-,i,i,i,i,i,i,h,o,o,o,o
ib,g0|,-,-,-,i,i,i,i,i,i,h,o,o,o,o
ib,g5|,.,-,-,-,i,i,i,i,i,h,o,o,o,o
ib,k0|,.,.,.,-,i,i,i,i,i,h,o,o,o,o
ib,k5|,.,.,.,.,-,i,i,i,i,i,h,o,o,o
ib,m0|,.,.,.,.,.,i,i,i,i,i,h,o,o,o
ib,m5|,.,.,.,.,.,.,i,i,i,i,i,h,o,o
ib,m9|,.,.,.,.,.,.,.,i,i,i,i,h,o,o
ii,b0|,-,-,-,-,-,-,i,i,i,i,i,h,o
ii,b5|,-,-,-,-,i,i,i,i,i,i,h,o,o
ii,a0|,-,-,i,i,i,i,i,i,h,o,o,o,o
ii,a5|,-,i,i,i,i,i,i,h,o,o,o,o,o
ii,f0|,-,i,i,i,i,i,i,h,o,o,o,o,o
ii,f5|,-,i,i,i,i,i,i,h,o,o,o,o,o
ii,g0|,-,i,i,i,i,i,i,h,o,o,o,o,o
ii,g5|,-,i,i,i,i,i,i,h,o,o,o,o,o
ii,k0|,-,i,i,i,i,i,i,i,h,o,o,o,o
ii,k5|,.,-,i,i,i,i,i,i,h,o,o,o,o
ii,m0|,.,.,.,i,i,i,i,i,i,h,o,o,o
ii,m5|,.,.,.,.,.,i,i,i,i,i,h,o,o
ii,m9|,.,.,.,.,.,i,i,i,i,i,h,o,o
iii,b0|,-,-,-,-,-,-,i,i,i,i,i,h,o
iii,b5|,-,-,-,-,i,i,i,i,i,h,o,o,o
iii,a0|,i,i,i,i,i,i,i,h,o,o,o,o,o
iii,a5|,i,i,i,i,i,i,h,o,o,o,o,o,o
iii,f0|,i,i,i,i,i,h,o,o,o,o,o,o,o
iii,f5|,i,i,i,i,i,h,o,o,o,o,o,o,o
iii,g0|,i,i,i,i,i,h,o,o,o,o,o,o,o
iii,g5|,i,i,i,i,i,i,h,o,o,o,o,o,o
iii,k0|,i,i,i,i,i,i,h,o,o,o,o,o,o
iii,k5|,i,i,i,i,i,i,i,h,o,o,o,o,o
iii,m0|,-,i,i,i,i,i,i,h,o,o,o,o,o
iii,m5|,.,.,.,i,i,i,i,i,h,o,o,o,o
iii,m9|,.,.,.,.,i,i,i,i,h,o,o,o,o
iv,b0|-,-,-,-,-,-,-,i,i,i,i,i,h,o
iv,b5|-,-,-,i,i,i,i,i,i,h,o,o,o,o
iv,a0|-,i,i,i,i,i,i,h,o,o,o,o,o,o
iv,a5|i,i,i,i,i,i,h,o,o,o,o,o,o,o
iv,f0|i,i,i,i,i,i,h,o,o,o,o,o,o,o
iv,f5|i,i,i,i,i,h,o,o,o,o,o,o,o,o
iv,g0|i,i,i,i,i,h,o,o,o,o,o,o,o,o
iv,g5|i,i,i,i,i,h,o,o,o,o,o,o,o,o
iv,k0|i,i,i,i,h,o,o,o,o,o,o,o,o,o
v,b0|-,-,-,-,-,-,i,i,i,i,i,i,h,o
v,b5|-,-,-,i,i,i,i,i,i,h,o,o,o,o
v,a0|i,i,i,i,i,i,i,h,o,o,o,o,o,o
v,a5|i,i,i,i,i,i,h,o,o,o,o,o,o,o
v,f0|i,i,i,i,i,h,o,o,o,o,o,o,o,o
v,f5|i,i,i,i,h,o,o,o,o,o,o,o,o,o
v,g0|i,i,i,h,o,o,o,o,o,o,o,o,o,o
v,g5|i,i,h,o,o,o,o,o,o,o,o,o,o,o
v,k0|i,i,h,o,o,o,o,o,o,o,o,o,o,o
v,k5|h,o,o,o,o,o,o,o,o,o,o,o,o,o
v,m0|h,o,o,o,o,o,o,o,o,o,o,o,o,o
v,m5|o,o,o,o,o,o,o,o,o,o,o,o,o,o
v,m9|o,o,o,o,o,o,o,o,o,o,o,o,o,o
vi,f5|i,i,i,h,o
vi,g0|i,i,h,o,o
vi,g5|i,h,o,o,o
vi,k0|i,h,o,o,o
vi,k5|o,o,o,o,o
vi,m0|o,o,o,o,o
vi,m5|o,o,o,o,o
vi,m9|o,o,o,o,o
d,b|h,o,o,o,o
d,a|o,o,o,o,o
d,f|o,o,o,o,o
d,g|o,o,o,o,o
d,k|o,o,o,o,o
d,m|o,o,o,o,o
# __DATA__
