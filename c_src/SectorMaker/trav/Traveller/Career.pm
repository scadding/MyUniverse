package Traveller::Career;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Career.pm
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

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my %TBL;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub new {
	my $pkg = shift;
	my %arg = @_;
	my $obj = bless {
		CHARACTER	=> undef,
		CAREER		=> undef,
		SKILLS		=> 0,
		BENEFITS	=> 0,
		AUTOSURVIVE	=> undef,
		TERMMIN		=> undef,
		TERMMAX		=> undef,
		%arg
	}, $pkg;
	return $obj;
}

sub character {
	my $self = shift;
	if (@_) { $self->{CHARACTER} = shift; }
	return $self->{CHARACTER};
}

sub career {
	my $self = shift;
	if (@_) { $self->{CAREER} = uc(shift); }
	return $self->{CAREER};
}

sub serve {
	my $self = shift;
	$self->character(shift);
	$self->character->career($self->career);

	my $continue = 1;
	my $deceased = 0;

	$self->{SKILLS}	= 0;
	$self->{BENEFITS} = 0;

	$self->adjust();

	$self->character->append_history($self->character->profile . "\n");
	$self->character->append_history("Start of career as " . ucfirst(lc($self->career)) . ".\n");

	$self->_promoted();

	while($continue) {
		$self->character->append_history("Start of term " . (1 + $self->character->terms) . ".\n");

		if ($self->survival()) {
			if (0 == $self->character->rank) {
				$self->commission();
			}
			
			if ($self->character->rank) {
				$self->promotion();
			}

			$self->term();
			$self->character->append_history("End of term " . $self->character->terms . ".\n");

			$continue = $self->reenlist();

			foreach my $a(keys %{ $self->character->{ATTR} }) {
				if ($self->character->attr($a) == 0) {
					$continue = 0;
					$self->character->append_history("Aging crisis.\n");
				}
			}
		}
		else {
			$deceased = 1;
			$continue = 0;
			$self->character->{DECEASED} = 1;
		}
	}

	$self->muster() unless($deceased);
}

sub adjust {
	my $self = shift;
	my $m;
	my $n;

	# xxx pst ugly hack

	if ($self->career eq 'NOBLE') {
		if ($self->character->attr('soc') < 10) {
			$m = Traveller::Dice::roll();
			$n = ($m <= 1) ? 12 :
				 ($m <= 3) ? 11 :
				 ($m <= 6) ? 10 : 10;
			$self->character->attr('soc', $n);
		}
	}

}

sub term {
	my $self = shift;

	$self->{SKILLS}++;
	$self->{BENEFITS}++;

	# xxx pst yuck
	$self->{SKILLS}++ if ($self->career eq 'SCOUT' && $self->character->terms > 0);

	$self->character->append_history("Eligible for $self->{SKILLS} skills:\n");

	while($self->{SKILLS}) {
		my $t = "TBL" . Traveller::Dice::range(3 +($self->character->attr('edu') >= 8 ? 1 : 0));
		my $r = Traveller::Dice::roll(1, 0, 1, 6);
		my $s = $TBL{$self->career}{$t}[$r] || '';
		$self->character->append_history("Skill: $s\n");
		$self->character->add_skill($s);
		$self->{SKILLS}--;
	}

	$self->character->age(4 + $self->character->age);
	$self->character->terms(1 + $self->character->terms);
	$self->aging();
}

sub muster {
	my $self = shift;
	my $m = 0;

	$self->{BENEFITS}++ if ($self->character->rank >= 5);
	$self->{BENEFITS}++ if ($self->character->rank >= 3);
	$self->{BENEFITS}++ if ($self->character->rank >= 1);

	$self->character->append_history("Muster out.\n");
	$self->character->append_history("Eligible for $self->{BENEFITS} benefits:\n");

	while($self->{BENEFITS}) {
		my $t = $m == 3                      ? 'BENEFIT' :
				Traveller::Dice::roll(1) > 3 ? 'BENEFIT' :
				'CASH';

		my $dm	= $t eq 'BENEFIT'
				? ($self->character->rank >= 5 ? 1 : 0)
				: (defined($self->character->{SKILLS}{'Gambling'}) ? 1 : 0);

		$m++ if ($t eq 'CASH');

		my $r = Traveller::Dice::roll(1, $dm, 1, 7) - 1;
		my $s = $TBL{$self->career}{$t}[$r];
		if ($s) {
			if ($t eq 'CASH') {
				$self->character->credits($self->character->credits + $s);
				$self->character->append_history("Credits: $s\n");
			}
			else {
				$self->character->add_benefit($s);
				$self->character->append_history("Benefit: $s\n");
			}
		}
		$self->{BENEFITS}--;
	}
}

sub enlist {
	my $self = shift;
	my $b = $self->test($TBL{$self->career}{'ROLL'}[0], 'E');
	
	return $b;
}

sub survival {
	my $self = shift;
	my $b = $self->{AUTOSURVIVE} || $self->test($TBL{$self->career}{'ROLL'}[1], 'S');

	if (!$b) {
		$self->character->append_history("Survival: failed.\n");
	}

	return $b;
}

sub commission {
	my $self = shift;
	return undef unless($TBL{$self->career}{'ROLL'}[2]);

	my $b = $self->character->rank
		? 0
		: $self->test($TBL{$self->career}{'ROLL'}[2], 'C');
	
	if ($b) {
		$self->character->rank(1 + $self->character->rank);
		$self->_promoted();
		if ($TBL{$self->career}{RANK}[$self->character->rank]) {
			$self->character->append_history(sprintf("Received commission as %s.\n", $TBL{$self->career}{RANK}[$self->character->rank]));
		}
	}

	return $b;
}

sub promotion {
	my $self = shift;
	return undef unless($TBL{$self->career}{'ROLL'}[3]);
	my $b = $self->test($TBL{$self->career}{'ROLL'}[3], 'P');

	if ($b) {
		$self->character->rank(1 + $self->character->rank);
		$self->_promoted();
		if ($TBL{$self->career}{RANK}[$self->character->rank]) {
			$self->character->append_history(sprintf("Received promotion to %s.\n", $TBL{$self->career}{RANK}[$self->character->rank]));
		}
	}

	return $b
}

sub _promoted {
	my $self = shift;
	$self->{SKILLS}++;

	$self->character->rank(6)
		if $self->character->rank > 6;

	$self->character->add_skill($TBL{$self->career}{'AUTO'}[$self->character->rank]);
}

sub reenlist {
	my $self = shift;
	my $r = Traveller::Dice::roll(2);
	my $t = $TBL{$self->career}{'ROLL'}[4];
	$self->_test('R', $t, $r, 0, $r);
	my $b = $r == 12
			? 1
			: $t > 0 ? ($r > $t) : (($r < $t));
	if ($r == 12) {
		$self->character->append_history("Reenlist: mandatory.\n");
	}
	elsif (($self->{TERMMAX}) && ($self->character->terms >= $self->{TERMMAX})) {
		$b = 0;
		$self->character->append_history("Reenlist: exit career.\n");
	}
		elsif (($self->{TERMMIN}) && ($self->character->terms < $self->{TERMMIN})) {
			$b = 1;
			$self->character->append_history("Reenlist: succeeded.\n");
		}
	elsif ($self->character->terms >= 7) {
		$b = 0;
		$self->character->append_history("Reenlist: retirement.\n");
	}
	elsif ($b) {
		$self->character->append_history("Reenlist: succeeded.\n");
	}
	else {
		$self->character->append_history("Reenlist: failed.\n");
	}

	return $b;
}

sub aging {
	my $self = shift;
	my %agetbl =(
		'str'	=> ';-1,8;-1,9;-2,9',
		'dex'	=> ';-1,7;-1,8;-2,9',
		'end'	=> ';-1,8;-1,9;-2,9',
		'int'	=> ';;;-1,9',
	);

	my $x = $self->character->terms / 4; $x = 3 if ($x > 3);
	
	foreach my $a(keys %agetbl) {
		my $test =(split /;/, $agetbl{$a})[$x];
		if ($test) {
			my($p, $n) = split /,/, $test;
			my $r = Traveller::Dice::roll(2);
			$self->_test('A', $n, $r, 0, $r);
			if ($r < $n) {
				$self->character->attr($a, $p + $self->character->attr($a));
				$self->character->attr($a, 0) unless $self->character->attr($a) >= 0;
				$self->character->append_history("Aging: $p " . ($a eq 'str' ? 'Strength': $a eq 'end' ? 'Endurance': $a eq 'dex' ? 'Dexterity': 'Intelligence') . "\n");
			}
		}
	}
}

sub test {
	my $self = shift;
	my $data = shift;
	my $test = shift;

	my $dm = 0;
	my($n, $mods) = split /\?/, $data;

	if ($n eq 'term') {
		$n = 1 + $self->character->terms;
	}

	if ($mods) {
		foreach my $d(split /;/, $mods) {
			my($a, $v, $m) = split /:/, $d;
			if ($self->{CHARACTER}->attr($a) >= $v) { $dm += $m; }
		}
	}

	my $r = Traveller::Dice::roll(2, $dm);

	$self->_test($test, $n, $r - $dm, $dm, $r);

	return $r >= $n;
}

sub _test {
my $self = shift;
$_[1] = 0 unless $_[1];
my	$e = '+'; $e = '-' if ($_[1] < 0);
$self->character->append_history(sprintf("[%s %2d$e =>(%2d +%d) = %2d]\n", $_[0], abs($_[1]), $_[2], $_[3], $_[4]));
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#  static methods

sub draft {
	my $character = shift || undef;
	my $choice = shift || undef;
	my $suited = shift || undef;

	return undef unless $character;

	# get list of applicable careers

	my @list;

	if (not defined($choice)) {
		@list = keys (%TBL);
	}
	elsif ($choice eq 'ANY') {
		@list = keys (%TBL);
	}
	elsif ($choice eq 'BOOK1') {
		@list = grep( $TBL{$_}{'BASE'} == 1, keys %TBL);
	}
	elsif ($choice eq 'SUPP4') {
		@list = grep( $TBL{$_}{'BASE'} == 0, keys %TBL);
	}
	else {
		@list = ($choice);
	}

	# return any one career from list

	return $list[Traveller::Dice::range(1 + $#list)] unless $suited;

	# choose career most suited for character

	my %list;

	#  marshall dms

	foreach my $career (@list) {

		my $dm = 0;

		$dm = 12 - $TBL{$career}{'ROLL'}[4];		# adjust for ease of reenlistment

		foreach my $k (0, 1, 3) {
			my($n, $mods) = split /\?/, $TBL{$career}{'ROLL'}[$k];

			$dm += 12 - $n if $k == 0 && $n;		# adjust for ease of enlistment

			if ($mods) {
				foreach my $d(split /;/, $mods) {
					my($a, $v, $m) = split /:|\?/, $d;

					$m += $v if $k == 0;			# adjust for ease of enlistment
					$m *=  2 if $k == 1;			# adjust for ease of survival
					$m *=  1 if $k == 3;			# adjust for ease of promotion

					$dm += $m
						if ($v > 0
							? $character->attr($a) >= $v
							: $character->attr($a) <= $v);
				}
			}
		}

		$list{$career} = $dm;
	}

	my $total = 0;

	foreach my $k (keys (%list)) {
		$total += $list{$k};
	}

	foreach my $k (keys (%list)) {
		$list{$k} = $list{$k} / $total;
	}

	# return any one career from list

	return Traveller::Utility::probability(\%list);
}

sub rank {
	my $career = shift;
	my $rank = shift;
	return $TBL{$career}{RANK}[$rank];
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#  package startup code

foreach(1..18) {
	my $h = {};
	my $s;
	
	$s = <Traveller::Career::DATA>;
	chomp($s);

	foreach(1..10) {
		my $line = <Traveller::Career::DATA>;
		chomp($line);
		my($a, $b) = split /\|/, $line;
		if ($a ne 'BASE') {
			my @f = split /,/, $b; $#f = 6;
			$h->{$a} = [ @f ];
		}
		else {
			$h->{$a} = $b;
		}
	}

	$TBL{$s} = $h;
}

1;

__DATA__
NAVY
BASE|1
ROLL|8?int:8:1;edu:9:2,5?int:7:2,10?soc:9:1,8?edu:8:1,6
AUTO|,,,,,+1 Social,+1 Social
RANK|,Ensign,Lieutenant,Lt Cmdr,Commander,Captain,Admiral
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Intelligence,+1 Education,+1 Social Standing
TBL1|,Ships' Boat,Vacc Suit,Forward Observer,Gunnery,Blade Combat,Gun Combat
TBL2|,Vacc Suit,Mechanical,Electronic,Engineering,Gunnery,Jack-o-T
TBL3|,Medical,Navigation,Engineering,Computer,Pilot,Administration
BENEFIT|Low Passage,+1 Intelligence,+2 Education,Blade,Travellers',High Passage,+2 Social Standing
CASH|1000,5000,5000,10000,20000,50000,50000
MARINE
BASE|1
ROLL|9?int:8:1;str:8:2,6?end:8:2,9?edu:7:1,9?soc:8:1,6
AUTO|Cutlass,Revolver
RANK|,Lieutenant,Captain,Force Commander,Lieutenant Colonel,Colonel,Brigadier
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,Gambling,Brawling,Blade Combat
TBL1|,Vehicle,Vacc Suit,Blade Combat,Gun Combat,Blade Combat,Gun Combat
TBL2|,Vehicle,Mechanical,Electronic,Tactics,Blade Combat,Gun Combat
TBL3|,Medical,Tactics,Tactics,Computer,Leader,Administration
BENEFIT|Low Passage,+2 Intelligence,+1 Education,Blade,Travellers',High Passage,+2 Social Standing
CASH|2000,5000,5000,10000,20000,30000,40000
ARMY
BASE|1
ROLL|5?dex:6:1;end:5:2,5?edu:6:2,5?end:7:1,6?edu:7:1,7
AUTO|Rifle,SMG
RANK|,Lieutenant,Captain,Major,Lt Colonel,Colonel,General
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,Gambling,+1 Education,Brawling
TBL1|,Vehicle,Air/Raft,Gun Combat,Forward Observer,Blade Combat,Gun Combat
TBL2|,Vehicle,Mechanical,Electronic,Tactics,Blade Combat,Gun Combat
TBL3|,Medical,Tactics,Tactics,Computer,Leader,Administration
BENEFIT|Low Passage,+1 Intelligence,+1 Education,Gun,High Passage,Middle Passage,+1 Social Standing
CASH|2000,5000,10000,10000,10000,20000,30000
SCOUT
BASE|1
ROLL|7?int:6:1;str:8:2,7?end:9:2,,,3
AUTO|Pilot
RANK|,,,,,,
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Intelligence,+1 Education,Gun Combat
TBL1|,Vehicle,Vacc Suit,Mechanical,Navigation,Electronic,Jack-o-T
TBL2|,Vehicle,Mechanical,Electronic,Jack-o-T,Gunnery,Medical
TBL3|,Medical,Navigation,Engineering,Computer,Pilot,Jack-o-T
BENEFIT|Low Passage,+2 Intelligence,+2 Education,Blade,Gun,Scout Ship,
CASH|20000,20000,30000,30000,50000,50000,50000
MERCHANT
BASE|1
ROLL|7?str:7:1;int:6:2,5?int:7:2,4?int:6:1,10?int:9:1,4
AUTO|,,,,Pilot
RANK|,4th Officer,3rd Officer,2nd Officer,1st Officer,Captain,Captain
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Strength,Blade Combat,Bribery
TBL1|,Vehicle,Vacc Suit,Jack-o-T,Steward,Electronic,Gun Combat
TBL2|,Streetwise,Mechanical,Electronic,Navigation,Gunnery,Medical
TBL3|,Medical,Navigation,Engineering,Computer,Pilot,Administration
BENEFIT|Low Passage,+1 Intelligence,+1 Education,Gun,Blade,Low Passage,Free Trader
CASH|1000,5000,10000,20000,30000,40000,40000
OTHER
BASE|1
ROLL|3,5?int:9:2,,,5
AUTO|
RANK|,,,,,,
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,Blade Combat,Brawling,-1 Social
TBL1|,Vehicle,Gambling,Brawling,Bribery,Blade Combat,Gun Combat
TBL2|,Streetwise,Mechanical,Electronic,Gambling,Brawling,Forgery
TBL3|,Medical,Forgery,Electronic,Computer,Streetwise,Jack-o-T
BENEFIT|Low Passage,+1 Intelligence,+1 Education,Gun,High Passage,,
CASH|1000,5000,10000,10000,10000,50000,100000
PIRATE
BASE|0
ROLL|7?soc:-7:1;end:9:2,6?int:8:2,9?str:10:1,8?int:9:1,7
AUTO|Brawling,,,,Pilot
RANK|,Henchman,Corporal,Sergeant,Lieutenant,Leader,Leader
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,Gambling,Brawling,Blade Combat
TBL1|,Blade Combat,Vacc Suit,Gun Combat,Gunnery,Zero-G Combat,Gun Combat
TBL2|,Streetwise,Gunnery,Engineering,Ship Tactics,Tactics,Mechanical
TBL3|,Navigation,Pilot,Forgery,Computer,Leader,Electronic
BENEFIT|Low Passage,+1 Intelligence,Weapon,,-1 Social,Middle Passage,Corsair
CASH|0,0,1000,10000,50000,50000,50000
BELTER
BASE|0
ROLL|8?dex:9:1;int:6:2,9,term,,7
AUTO|Vacc Suit
RANK|,,,,,,
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,Gambling,Brawling,Vacc Suit
TBL1|,Vacc Suit,Vacc Suit,Prospecting,Forward Observer,Prospecting,Ship's Boat
TBL2|,Ship's Boat,Electronic,Prospecting,Mechanical,Prospecting,Instruction
TBL3|,Navigation,Medical,Pilot,Computer,Engineering,Jack-o-T
BENEFIT|Low Passage,+1 Intelligence,Weapon,High Passage,Travellers',Seeker,
CASH|0,0,1000,10000,100000,100000,100000
SAILOR
BASE|0
ROLL|6?end:10:1;str:8:2,5?end:8:2,5?int:9:1,6?edu:8:1,6
AUTO|
RANK|,Ensign,Lieutenant,Lieutenant Commander,Commander,Captain,Admiral
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,Gambling,Brawling,Carousing
TBL1|,Gun Combat,Communications,Forward Observer,Vehicle,Vehicle,Battle Dress
TBL2|,Water Craft,Electronic,Mechanical,Gravitics,Navigation,Demolition
TBL3|,Medical,Vehicle,Streetwise,Computer,Administration,Jack-o-T
BENEFIT|Low Passage,+1 Education,Weapon,Weapon,High Passage,High Passage,+1 Social
CASH|2000,5000,10000,10000,10000,20000,30000
DIPLOMAT
BASE|0
ROLL|8?edu:8:1;soc:9:2,3?end:8:2,5?int:8:1,10?soc:10:1,5
AUTO|Liason
RANK|,3rd Secretary,2nd Secretary,1st Secretary,Counselor,Minister,Ambassador
TBL0|,+1 Strength,+1 Education,+1 Intelligence,Blade Combat,Gun Combat,Carousing
TBL1|,+1 Intelligence,Vacc Suit,Vehicle,Vehicle,Gambling,Computer
TBL2|,Forgery,Streetwise,Interrogation,Recruiting,Instruction,Administration
TBL3|,Liason,Liason,Administration,Computer,+1 Social,Jack-o-T
BENEFIT|Low Passage,+1 Intelligence,+2 Education,Weapon,+1 Social,High Passage,Travellers'
CASH|10000,10000,10000,20000,50000,60000,70000
DOCTOR
BASE|0
ROLL|9?int:8:1;dex:9:2,3?int:8:2,,,4
AUTO|Medical
RANK|,,,,,,
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Intelligence,+1 Education,+1 Social
TBL1|,+1 Dexterity,Electronic,Medical,Streetwise,Medical,Blade Combat
TBL2|,Medical,Medical,Mechanical,Electronic,Computer,Administration
TBL3|,Medical,Medical,Administration,Computer,+1 Intelligence,+1 Education
BENEFIT|Low Passage,+1 Education,+1 Education,Weapon,Instruments,Middle Passage,
CASH|20000,20000,20000,30000,40000,60000,100000
FLYER
BASE|0
ROLL|6?str:7:1;dex:9:2,5?dex:8:2,5?edu:8:1,8?edu:8:1,6
AUTO|Air Craft
RANK|,Pilot,Flight Leader,Squadron Leader,Staff Major,Group Leader,Air Marshal
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,Gambling,Brawling,Carousing
TBL1|,Brawling,Vacc Suit,Gun Combat,Vehicle,Vehicle,Vehicle
TBL2|,Air Craft,Mechanical,Electronic,Gravitics,Gun Combat,Survival
TBL3|,Medical,Leader,Pilot,Computer,Administration,Jack-o-T
BENEFIT|Low Passage,+1 Education,Weapon,Weapon,High Passage,Middle Passage,+1 Social
CASH|2000,5000,10000,10000,10000,20000,30000
BARBARIAN
BASE|0
ROLL|5?end:9:1;str:10:2,6?str:8:2,6?str:10:1,9?int:6:1,6
AUTO|Sword,,Blade Combat,,,Leader
RANK|,,Warrior,Warrior,Warrior,Chief,Chief
TBL0|,+1 Strength,+1 Strength,+1 Strength,Carousing,+1 Dexterity,+1 Endurance
TBL1|,Brawling,Blade Combat,Blade Combat,Bow Combat,Bow Combat,Gun Combat
TBL2|,Blade Combat,Mechanical,Survival,Recon,Streetwise,Bow Combat
TBL3|,Medical,Interrogation,Tactics,Leader,Instruction,Jack-o-T
BENEFIT|Low Passage,Blade,Blade,Blade,,High Passage,High Passage
CASH|0,0,1000,2000,3000,4000,5000
BUREAUCRAT
BASE|0
ROLL|5?edu:8:1;str:-8:2,4?edu:10:2,6?soc:9:1,7?int:9:1,3
AUTO|
RANK|,Clerk,Supervisor,Assistant Manager,Manager,Executive,Director
TBL0|,+1 Endurance,+1 Education,+1 Intelligence,Brawling,Carousing,+1 Dexterity
TBL1|,Gun Combat,Vehicle,Blade Combat,Instruction,Vehicle,+1 Education
TBL2|,Recruiting,Vehicle,Liaison,Interrogation,Administration,Administration
TBL3|,Administration,Administration,Computer,Administration,Jack-o-T,Leader
BENEFIT|Low Passage,Middle Passage,,Watch,,High Passage,+1 Social
CASH|0,0,10000,10000,40000,40000,80000
ROGUE
BASE|0
ROLL|6?soc:-8:1;end:7:2,6?int:9:2,,,5
AUTO|Streetwise
RANK|,,,,,,
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Intelligence,Brawling,Carousing
TBL1|,Blade Combat,Gun Combat,Demolition,Vehicle,+1 Education,Vehicle
TBL2|,Streetwise,Forgery,Bribery,Carousing,Liaison,Ship Tactics
TBL3|,Medical,Bribery,Forgery,Computer,Leader,Jack-o-T
BENEFIT|Low Passage,+1 Social Standing,Gun,Blade,High Passage,Travellers',
CASH|0,0,10000,10000,50000,100000,100000
NOBLE
BASE|0
ROLL|,3,5?edu:9:1,12?int:10:1,4
AUTO|
RANK|,Knight,Baron,Marquis,Count,Duke,Duke
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Intelligence,Carousing,Brawling
TBL1|,Gun Combat,Blade Combat,Hunting,Vehicle,Bribery,+1 Dexterity
TBL2|,Pilot,Ship's Boat,Vehicle,Navigation,Engineering,Leader
TBL3|,Medical,Computer,Administration,Liaison,Leader,Jack-o-T
BENEFIT|High Passage,High Passage,Gun,Blade,Travellers',Yacht,
CASH|10000,50000,50000,100000,100000,100000,200000
SCIENTIST
BASE|0
ROLL|6?int:9:1;edu:10:2,5?edu:9:2,,,5
AUTO|Computer
RANK|,,,,,,
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Intelligence,+1 Education,Carousing
TBL1|,Gun Combat,Blade Combat,Vehicle,Jack-o-T,Navigation,Survival
TBL2|,Mechanical,Electronic,Gravitics,Computer,+1 Intelligence,+1 Education
TBL3|,Medical,Computer,Administration,Leader,+1 Intelligence,Jack-o-T
BENEFIT|Low Passage,Middle Passage,High Passage,+1 Social Standing,Gun,Lab Ship,
CASH|1000,2000,5000,10000,20000,30000,40000
HUNTER
BASE|0
ROLL|9?dex:10:1;end:9:2,6?str:10:2,,,5
AUTO|Hunting
RANK|,,,,,,
TBL0|,+1 Strength,+1 Dexterity,+1 Endurance,+1 Intelligence,Gun Combat,Blade Combat
TBL1|,Gun Combat,Blade Combat,Survival,Hunting,Vehicle,Hunting
TBL2|,Mechanical,Electronic,Gravitics,Computer,Hunting,Administration
TBL3|,Medical,Computer,Hunting,Leader,Survival,Administration
BENEFIT|Low Passage,High Passage,Weapon,Weapon,Weapon,Safari Ship,
CASH|1000,1000,5000,5000,10000,100000,100000
Blade|Dagger,Blade,Foil,Sword,Cutlass,Broadsword,Bayonet,Spear,Halberd,Pike,Cudgel
Gun|Body Pistol,Auto Pistol,Revolver,Carbine,Rifle,Auto Rifle,Shotgun,SMG,Laser Carbine,Laser Rifle
Vehicle|Ground Car,Water Craft,Winged Craft,Hover Craft,Grav Belt
# __DATA__
