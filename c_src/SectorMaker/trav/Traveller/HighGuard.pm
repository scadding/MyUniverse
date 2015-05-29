package Traveller::HighGuard;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			HighGuard.pm
#	author:			paul sanford toney <signalgk@gmail.com>
#
#	description:	
#
#	%license%
#
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use Traveller::Dice;

use Data::Dumper;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#     GAME TURN SEQUENCE
# 
# 1.  Battle Formation Step.
#     Both players determine their line of battle and reserve positions.
#     Craft are launched and recovered.
# 
# 2.  Initiative Determination Step.
#     Dice determine who has the initiative. The player with the
#     initiative is called the attacker for the turn.
# 
# 3.  Range Determination Step.
#     The attacker decides the range for the turn.
# 
# 4.  Pre-Combat Decision Step.
#     Each player decides for each ship:
#     a.  Whether to break off this turn;
#     b.  Whether to use emergency agility;
#     c.  If black globe will be on.
#     The defending player announces all such decisions before the
#     attacker.
# 
# 5.  Combat Step.
#     Players arrange their battle lines by size with largest ship
#     first. Attacker then presents his first ship as a target for the
#     combat procedure. Then the defender presents his first ship.
#     Combat continues with players alternating until all ships in both
#     battle lines have been presented as targets. For each ship, the
#     combat procedure is:
#     a.  Fire Allocation.
#         Firing player indicates which batteries will fire.
#     b.  Hit Procedure.
#         Firing player determines which batteries have scored hits.
#     c.  Defensive Fire.
#         Target uses its defensive batteries to prevent enemy weapons
#         from penetrating.
#     d.  Passive Defense.
#         Passive defensive screens must be penetrated.
#     e.  Damage Determination.
#         Batteries which hit and penetrate all defenses must determine
#         the damage they inflict. Damage is recorded but does not apply
#         until step 6.
#     f.  Fire procedure begins for the next ship.
# 
# 6.  Damage Step.
#     Damage inflicted during combat (5e) takes effect.
# 
# 7.  Breakthrough Step.
#     If one player's line of battle has been broken, ships in the line
#     of battle of the victorious player may fire again at any ships in
#     the enemy reserve position. Go through steps 5 and 6 again; ships
#     in the enemy reserve position may not fire except defensively.
# 
# 8.  Pursuit Step.
#     Ships which are breaking off may be pursued by enemy ships.
# 
# 9.  Terminal Step.
#     Planetary bombardment, refuelling, revival of the frozen watch,
#     and other non-battle operations are performed.



# BC-9514 Kinunir       BC-A2447G2-000510-50202-0  MCr 1336.63  1250 tons
#                        | ||||||| |||||| ||||| |
#            Type [ty] --+ ||||||| |||||| ||||| +- [fi] Fighter Squadrons
#         Tonnage [to] ----+|||||| |||||| ||||+--- [mi] Missiles
#   Configuration [co] -----+||||| |||||| |||+---- [me] Meson Gun
#            Jump [jn] ------+|||| |||||| ||+----- [pa] Particle Accelerator
#        Maneuver [mn] -------+||| |||||| |+------ [en] Energy Weapons
#      Powerplant [pn] --------+|| |||||| +------- [la] Lasers
#        Computer [cn] ---------+| |||||+--------- [re] Repulsors
#            Crew [cr] ----------+ ||||+---------- [bg] Force Field
#           Armor [ar] ------------+||+----------- [nd] Nuclear Damper
#     Sandcasters [sa] -------------++------------ [ms] Meson Screen

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my %TBL;

use constant DEBUGGING		=> 1;

use constant RANGE_LONG		=> 'L';
use constant RANGE_SHORT	=> 'S';

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	package Traveller::HighGuard::Battle;

	sub new {
		my $pkg = shift;
		my $obj = bless {
			'comb'	=> [],
			'turn'	=> 0,
			'rang'	=> RANGE_LONG,
		}, $pkg;
		$obj->add_combatant(@_);
		return $obj;
	}

	sub add_combatant {
		my $self = shift;
		foreach my $comb (@_) {
			push @{ $self->{'comb'} }, $comb;
		}
	}

	sub turn {
		my $self = shift;

		$self->remark("start of turn " . ++$self->{'turn'});

		$self->step_1();	# 1.  Battle Formation Step.
		$self->step_2();	# 2.  Initiative Determination Step.
		$self->step_3();	# 3.  Range Determination Step.
	}

	sub step_1 {
		my $self = shift;

		#     Both players determine their line of battle and reserve positions.

		foreach my $comb (@{ $self->{'comb'} }) {
			$comb->{'doct'}->determine_formation();
		}

		#     Craft are launched and recovered.
		# xxx nope

		return;
	}

	sub step_2 {
		my $self = shift;
#		my @roll;

		#     Dice determine who has the initiative.
		# xxx nope; just picking the first combatant

#		foreach my $comb (@{ $self->{'comb'} }) {
#			my	$href = {};
#				$href->{'comb'} = $comb;
#				$href->{'roll'} = Traveller::Dice::roll(1);
#		}

		$self->{'atpl'} = @{ $self->{'comb'} }[0];
		$self->remark(' initiative to ' . $self->{'atpl'}->{'name'});

		return;
	}

	sub step_3 {
		my $self = shift;

		$self->{'rang'} = $self->{'atpl'}->{'doct'}->determine_range();
		$self->{'rang'} = RANGE_LONG unless $self->{'turn'} > 1;

		$self->remark(' range is ' . ($self->{'rang'} eq RANGE_LONG ? "long" : "short"));
		return;
	}

	sub remark {
		my $self = shift;
		print shift, "\n";
		return;
	}

	#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	package Traveller::HighGuard::Combatant;

	my $COUNT = 0;

	sub new {
	$COUNT++;
		my $pkg = shift;
		my %arg = @_;
		my $obj = bless {
			'name'	=> "Combatant $COUNT",
			'vess'	=> [],	# aref of refs to vessels
			'line'	=> [],
			'resv'	=> [],
			'doct'	=> undef,
			%arg
		}, $pkg;
		$obj->init();
		return $obj;
	}

	sub init {
		my $self = shift;

		$self->{'doct'} = Traveller::HighGuard::Doctrine->new($self);
		return;
	}

	sub add_vessel {
		my $self = shift;
		foreach my $vess (@_) {
			push @{ $self->{'vess'} }, $vess;
		}
	}

	sub least_agile {
		my $self = shift;
		my $ag = undef;

		foreach my $vess (@{ $self->{'vess'} }) {
			$ag = $vess->attr('ag') unless $ag;
			$ag = $vess->attr('ag') if $vess->attr('ag') > $ag;
		}

		return $ag;
	}

	sub vessels_in_line {
		my $self = shift;
		my $n = 0;

		foreach my $vess (@{ $self->{'line'} }) {
			$n++ if $vess->can_maneuver() && $vess->can_attack();
		}

		return $n;
	}

	#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	package Traveller::HighGuard::Doctrine;

	sub new {
		my $pkg = shift;
		my $obj = bless {
			'comb'	=> shift,
		}, $pkg;
		return $obj;
	}

	sub determine_formation {
		my $self = shift;

		# xxx nope; just shifting every vessel into line for now

		$self->{'comb'}->{'line'} = [];
		$self->{'comb'}->{'resv'} = [];

		foreach my $vess (@{ $self->{'comb'}->{'vess'} }) {
			push @{ $self->{'comb'}->{'line'} }, $vess;
		}

		# sort line by vessel size
#		$self->{'comb'}->{'line'} = sort {$a->attr('to') <=> $b->attr('to')} @{ $self->{'comb'}->{'line'} };

		return;
	}

	sub determine_range {
		my $self = shift;

		# xxx nope; just returning short range
		return RANGE_SHORT;
	}

	#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	package Traveller::HighGuard::Vessel;

#BC-9514 Kinunir       BC-A2447G2-000510-50202-0  MCr 1336.63     1250 tons
#     batteries bearing                  2 2 2                     Crew=45.
#             batteries                  2 2 2                       TL=15.
#Passengers=0. Low=0. Cargo=63. Fuel=587.5. EP=87.5. Agility=1. Marines=35.

	sub new {
		my $pkg = shift;
		my $obj = bless {
			'code'	=> undef,
			'name'	=> undef,
			'attr'	=> {
				'ty' => 0, # attributes from universal ship profile
				'to' => 0, # listed in order of appearance in usp
				'co' => 0,
				'jn' => 0,
				'mn' => 0,
				'pn' => 0,
				'cn' => 0,
				'cr' => 0,
				'ar' => 0,
				'sa' => 0,
				'ms' => 0,
				'nd' => 0,
				'bg' => 0,
				're' => 0,
				'la' => 0,
				'en' => 0,
				'pa' => 0,
				'me' => 0,
				'mi' => 0,
				'fi' => 0,
				'ep' => 0, # attributes from elsewhere
				'ag' => 0,
			},
			'stat'	=> {},
			'vess'	=> {},
		}, $pkg;
		$obj->init(@_);
		return $obj;
	}

	sub init {
		my $self = shift;
		my $data = shift;

		my ($c, $n, $a) = $data =~ m/(.+)\s+(\w+)\s+(..-.......-......-.....-.)/;
		$self->{'code'} = $c;
		$self->{'name'} = $n;

		my ($ty,
			$to,
			$co,
			$jn,
			$mn,
			$pn,
			$cn,
			$cr,
			$ar,
			$sa,
			$ms,
			$nd,
			$bg,
			$re,
			$la,
			$en,
			$pa,
			$me,
			$mi,
			$fi) = $data =~ m/(..)-(.)(.)(.)(.)(.)(.)(.)-(.)(.)(.)(.)(.)(.)-(.)(.)(.)(.)(.)-(.)/;

		$self->attr('ty', $ty);
		$self->attr('to', $TBL{'cnvt'}{$to});
		$self->attr('co', $TBL{'cnvt'}{$co});
		$self->attr('jn', $TBL{'cnvt'}{$jn});
		$self->attr('mn', $TBL{'cnvt'}{$mn});
		$self->attr('pn', $TBL{'cnvt'}{$pn});
		$self->attr('cn', $TBL{'cnvt'}{$cn});
		$self->attr('cr', $TBL{'cnvt'}{$cr});
		$self->attr('ar', $TBL{'cnvt'}{$ar});
		$self->attr('sa', $TBL{'cnvt'}{$sa});
		$self->attr('ms', $TBL{'cnvt'}{$ms});
		$self->attr('nd', $TBL{'cnvt'}{$nd});
		$self->attr('bg', $TBL{'cnvt'}{$bg});
		$self->attr('re', $TBL{'cnvt'}{$re});
		$self->attr('la', $TBL{'cnvt'}{$la});
		$self->attr('en', $TBL{'cnvt'}{$en});
		$self->attr('pa', $TBL{'cnvt'}{$pa});
		$self->attr('me', $TBL{'cnvt'}{$me});
		$self->attr('mi', $TBL{'cnvt'}{$mi});
		$self->attr('fi', $TBL{'cnvt'}{$fi});
	}

	sub attr {
		my $self = shift;
		my $attr = shift;
		if (@_) { $self->{'attr'}{$attr} = shift; }
		return $self->{'attr'}{$attr};
	}

	sub profile {
		my $self = shift;
		return '';
	}

	sub can_maneuver {
		my $self = shift;

		# xxx nope
		return 1;
	}

	sub can_attack {
		my $self = shift;

		# xxx nope
		return 1;
	}

	sub listing {
		my $self = shift;

		my %arg = (
			LEGEND	=> undef,
			@_
		);

		return '';
	}

	sub report {
		my $self = shift;
		print $self->listing(@_);
	}

	package Traveller::HighGuard;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub attack {
	my	$attacker = shift;		# ref to attacking vessel
	my	$defender = shift;		# ref to defending vessel

	my	$at_sys = shift;		# attacking system

	my	$range = shift;			# range L or S

	my	$m	= 0;				# dm to hit

		# + relative computer size, all systems
		$m += $attacker->attr('cn') - $defender->attr('cn');

		# - target agility rating, all systems
		$m -= $defender->attr('ag');

		# xxx need to know system subtype (la, en)
		#$m -= 1 if ($range eq RANGE_LONG && $subtype eq 'la');

		# + target size modifiers, all systems

		SWITCH: {
			if ($defender->attr('sz') ==  0)	{ $m -= 2; last SWITCH;}
			if ($defender->attr('sz') >=  1
			&&	$defender->attr('sz') <= 10)	{ $m -= 1; last SWITCH;}
			if ($defender->attr('sz') >= 21
			&&	$defender->attr('sz') <= 24)	{ $m += 1; last SWITCH;}
			if ($defender->attr('sz') >= 25)	{ $m += 2; last SWITCH;}
		}

		# missile at short range
		$m -= 1 if $at_sys eq 'mi';

		# meson at short range
		$m += 2 if $at_sys eq 'me';

	# hit, roll, result
	my	$n = $TBL{$at_sys. 'th'}[$attacker->attr($at_sys)];
	my	$r = Traveller::Dice::roll(2);
	my	$b = $r + $m >= $n;

		remark(sprintf("hit %s %s at range %s: [%2d + %2d >= %2d]: %s",
						$at_sys,
						$attacker->attr($at_sys),
						$range,
						$r, $m, $n,
						qw/failure success/[$b]));

	return $b;
}

sub defend {
	my	$attacker = shift;		# ref to attacking vessel
	my	$defender = shift;		# ref to defending vessel

	my	$at_sys = shift;		# attacking system
	my	$de_sys = shift;		# defending system

	my	$m = 0;					# dm to penetrate

		# + relative computer size, all systems
		$m += $attacker->attr('cn') - $defender->attr('cn');

		# xxx need to know system subtype (la, en)

	# attack penetrates defense if defensive factor is zero
	return 1 unless $defender->attr($de_sys);

	# attack penetrates defense if defensive factor is zero
	# xxx do this

	# xxx m += 2 if en
	#$m += 2 if $subtype eq 'en';

	# hit, roll, result
	my	$n = $TBL{$at_sys . $de_sys}{$defender->attr($de_sys)}[$attacker->attr($at_sys)];
	my	$r = Traveller::Dice::roll(2, $m);
	my	$b = $r >= $n;

		remark(sprintf("pen %s %s v %s %s: [%2d + %2d >= %2d]: %s",
						$at_sys,
						$attacker->attr($at_sys),
						$de_sys,
						$defender->attr($de_sys),
						$r, $m, $n,
						qw/failure success/[$b]));

	return $b;
}

sub damage {
	my	$attacker = shift;		# ref to attacking vessel
	my	$defender = shift;		# ref to defending vessel

	my	$at_sys = shift;		# attacking system
}

sub remark {
	print shift, "\n";
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub munge_lookup {
	my $line = <Traveller::HighGuard::DATA>;
	chomp $line;
	my ($a, $b) = split /\|/, $line;
	my @f = split /,/, $b;
	my $href = {};
	for (my $n = 0; $n <= $#f; $n++) {
		$href->{$f[$n]} = $n;
	}
	$TBL{$a} = $href;
	return;
}

sub munge_series {
	my $line = <Traveller::HighGuard::DATA>;
	chomp $line;
	my ($a, $b) = split /\|/, $line;
	my @f = split /,/, $b;
	$TBL{$a} = \@f;
	return;
}

sub munge_table {
	my $m = shift || 9;
	my $href = {};
	my $name = undef;

	foreach my $n (1..$m) {
		my $line = <Traveller::HighGuard::DATA>;
		chomp $line;
		my ($a, $b) = split /\|/, $line;
		my @f = split /,/, $b;
		$href->{$n} = \@f;
		$name = $a unless $name;
	}
	$TBL{$name} = $href;
	return;
}

munge_lookup();	# cnvt
munge_series(); # mith
munge_table();	# misa ...
munge_table();	# mibe ...
munge_table();	# mire ...
munge_table();	# mind ...
munge_series(); # beth
munge_table();	# besa ...
munge_series(); # meth
munge_table();	# mesc ...
munge_table();	# meco ...
munge_series(); # path

#0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,J,K,L,M,N,P,Q,R,S,T
#0,1,2,3,4,5,6,7,8,9,111213141516171819202122232425262728
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;

__DATA__
cnvt|0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,J,K,L,M,N,P,Q,R,S,T
mith|,6,6,5,5,4,4,3,3,2
misa|,5,4,3,2,1,0,0,0,0
misa|,6,5,4,3,2,1,0,0,0
misa|,7,6,5,4,3,2,1,0,0
misa|,8,7,6,5,4,3,2,1,0
misa|,9,8,7,6,5,4,3,2,1
misa|,10,9,8,7,6,5,4,3,2
misa|,11,10,9,8,7,6,5,4,3
misa|,12,11,10,9,8,7,6,5,4
misa|,13,12,11,10,9,8,7,6,5
mibe|,5,4,3,2,1,0,0,0,0
mibe|,6,5,4,3,2,1,0,0,0
mibe|,7,6,5,4,3,2,1,0,0
mibe|,8,7,6,5,4,3,2,1,0
mibe|,9,8,7,6,5,4,3,2,1
mibe|,10,9,8,7,6,5,4,3,2
mibe|,11,10,9,8,7,6,5,4,3
mibe|,12,11,10,9,8,7,6,5,4
mibe|,13,12,11,10,9,8,7,6,5
mire|,15,14,13,12,11,10,9,8,7
mire|,16,15,14,13,12,11,10,9,8
mire|,17,16,15,14,13,12,11,10,9
mire|,18,17,16,15,14,13,12,11,10
mire|,19,18,17,16,15,14,13,12,11
mire|,20,19,18,17,16,15,14,13,12
mire|,21,20,19,18,17,16,15,14,13
mire|,22,21,20,19,18,17,16,15,14
mire|,23,22,21,20,19,18,17,16,15
mind|,10,9,8,7,6,5,4,3,2
mind|,11,10,9,8,7,6,5,4,3
mind|,12,11,10,9,8,7,6,5,4
mind|,13,12,11,10,9,8,7,6,5
mind|,14,13,12,11,10,9,8,7,6
mind|,15,14,13,12,11,10,9,8,7
mind|,16,15,14,13,12,11,10,9,8
mind|,17,16,15,14,13,12,11,10,9
mind|,18,17,16,15,14,13,12,11,10
beth|,8,7,7,6,6,5,5,4,4
besa|,6,5,4,3,2,1,0,0,0
besa|,7,6,5,4,3,2,1,0,0
besa|,8,7,6,5,4,3,2,1,0
besa|,9,8,7,6,5,4,3,2,1
besa|,10,9,8,7,6,5,4,3,2
besa|,11,10,9,8,7,6,5,4,3
besa|,12,11,10,9,8,7,6,5,4
besa|,13,12,11,10,9,8,7,6,5
besa|,14,13,12,11,10,9,8,7,6
meth|,9,9,8,7,7,7,6,6,6,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
mesc|,16,15,15,14,14,13,13,12,12,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1
mesc|,16,15,15,14,14,13,13,12,12,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1
mesc|,17,16,16,15,15,14,14,13,13,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2
mesc|,17,16,16,15,15,14,14,13,13,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2
mesc|,18,17,17,16,16,15,15,14,14,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3
mesc|,18,17,17,16,16,15,15,14,14,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4,3
mesc|,19,18,18,17,17,16,16,15,15,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4,4
mesc|,19,18,18,17,17,16,16,15,15,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5,4
mesc|,20,19,19,18,18,17,17,16,16,13,13,12,12,11,11,10,10,9,9,8,8,7,7,6,6,5,5
meco|,12,12,12,11,11,11,10,10,10,8,8,8,7,7,7,6,6,6,5,5,5,4,4,4,3,3,3
meco|,11,11,10,10,10,9,9,9,8,7,7,6,6,6,5,5,5,4,4,4,3,3,3,2,2,2,1
meco|,10,9,9,9,8,8,8,7,7,6,5,5,5,4,4,4,3,3,3,2,2,2,1,1,1,0,0
meco|,7,7,6,6,6,5,5,5,4,3,3,2,2,2,1,1,1,0,0,0,0,0,0,0,0,0,0
meco|,6,5,5,5,4,4,4,3,3,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
meco|,8,8,8,7,7,7,6,6,6,5,5,5,4,4,4,3,3,2,1,1,1,0,0,0,0,0,0
meco|,15,15,14,14,14,13,13,13,12,11,11,10,10,10,9,9,9,8,8,8,7,7,7,6,6,6,5
meco|,4,4,4,3,3,3,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
meco|,14,13,13,13,12,12,12,11,11,10,9,9,9,8,8,8,7,7,7,6,6,6,5,5,5,4,4
path|,9,8,7,7,6,6,5,5,4,3,3,3,3,3,2,2,2,2,2,1,1,1,1,1,0,0,0
sedt|,,^crit,^iedt,^iedt,^iedt,mn-2,fu-3,we-3,mn-1,fu-2,we-2,mn-1,,fu-1,we-1,we-1,fu-1,we-1,we-1,fu-1,we-1,we-1
radt|,,^crit,cr-1,cn-4,cr-1,cn-3,cr-1,cn-2,cn-2,we-4,cn-2,we-3,cn-1,we-2,we-1,we-1,we-1,we-1,we-1
iedt|,,,^crit,^crit,^crit,fe-x,cn-2,sc-3,jn-2,pn-2,cr-1,cn-1,sc-2,jn-1,pn-1,cn-1,sc-1,jn-1,pn-1,sc-1,jn-1,pn-1
# __DATA__
