package Traveller::Character;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Character.pm
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
use Traveller::Career;

use Text::Wrap;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

sub new {
	my $pkg = shift;
	my %arg = @_;
	my $obj = bless {
		ATTR		=> {
			'str'	=> $arg{STR}	|| 0,
			'dex'	=> $arg{DEX}	|| 0,
			'end'	=> $arg{END}	|| 0,
			'int'	=> $arg{INT}	|| 0,
			'edu'	=> $arg{EDU}	|| 0,
			'soc'	=> $arg{SOC}	|| 0,
						},
		NAME		=> undef,
		CAREER		=> undef,
		AGE			=> 18,
		RANK		=> 0,
		TERMS		=> 0,
		CREDITS		=> 0,
		SKILLS		=> {},
		POSSESSIONS	=> {},
		HISTORY		=> '',
		DECEASED	=> 0,
		%arg
	}, $pkg;
	return $obj;
}

sub name {
	my $self = shift;
	if (@_) { $self->{NAME} = shift; }
	return $self->{NAME};
}

sub career {
	my $self = shift;
	if (@_) { $self->{CAREER} = shift; }
	return $self->{CAREER};
}

sub age {
	my $self = shift;
	if (@_) { $self->{AGE} = shift; }
	return $self->{AGE};
}

sub rank {
	my $self = shift;
	if (@_) { $self->{RANK} = shift; }
	return $self->{RANK};
}

sub terms {
	my $self = shift;
	if (@_) { $self->{TERMS} = shift; }
	return $self->{TERMS};
}

sub credits {
	my $self = shift;
	if (@_) { $self->{CREDITS} = shift; }
	return $self->{CREDITS};
}

sub skills {
	my $self = shift;
	if (@_) { @{ $self->{SKILLS} } = @_; }
	return @{ $self->{SKILLS} };
}

sub possessions {
	my $self = shift;
	if(@_) { @{ $self->{POSSESSIONS} } = @_; }
	return @{ $self->{POSSESSIONS} };
}

sub attr {
	my $self = shift;
	my $attr = shift;
	if (@_) { my $m = shift; $self->{ATTR}{$attr} = $m > 15 ? 15 : $m; }
	return $self->{ATTR}{$attr};
}

sub profile {
	my $self = shift;
	my $s = sprintf(
		"%X%X%X%X%X%X",
		$self->{ATTR}{'str'},
		$self->{ATTR}{'dex'},
		$self->{ATTR}{'end'},
		$self->{ATTR}{'int'},
		$self->{ATTR}{'edu'},
		$self->{ATTR}{'soc'});
	return $s;
}

sub generate {
	my $self = shift;
	foreach my $a(keys %{ $self->{ATTR} }) {
		$self->{ATTR}{$a} = Traveller::Dice::roll(2)
			unless $self->{ATTR}{$a};
	}
}

sub serve {
	my $self = shift;
	my $career = shift;

	$career->serve($self);
}

sub add_skill {
	my $self = shift;
	my $s = shift;

	return unless($s);

	if ($s =~ /[+-]\d/) {
		my($m, $a) = split(/ /, $s);
		$a = lc(substr($a, 0, 3));
		$self->attr($a, $m + $self->attr($a));
	}
	else {
		$self->{SKILLS}{$s}++;
	}
}

sub add_benefit {
	my $self = shift;
	my $s = shift;

	return unless($s);

	if ($s =~ /[+-]/) {
		my($m, $a) = split(/ /, $s);
		$a = lc(substr($a, 0, 3));
		$self->attr($a, $m + $self->attr($a))
			unless $self->attr($a) >= 15;
	}
	else {
		return if (defined($self->{POSSESSIONS}{$s})
				&& ($s eq 'Scout Ship'
#				||	$s eq 'Free Trader'
				||	$s eq 'Lab Ship'
				||	$s eq 'Yacht'
				||	$s eq 'Corsair'
				||	$s eq 'Safari Ship'
				||	$s eq 'Seeker'
				||	$s eq 'Travellers\''
				||	$s eq 'Instruments'));
		$self->{POSSESSIONS}{$s}++;
	}
}

sub listing {
	my $self = shift;
	my $verbose = shift || undef;
	my $upp = $self->profile;
	my $skills = '';
	my $possessions = '';

	my $rank = $self->rank;
	my $age = $self->age;
	my $credits = $self->credits;
	my $terms = $self->terms;
	   $terms .= $self->terms == 1 ? " Term" : " Terms";

	$skills =
		join( ', ',
			map { "$_-$self->{SKILLS}{$_}" }
				sort keys %{ $self->{SKILLS} });

	$possessions =
		join( ', ',
			map { $self->{POSSESSIONS}{$_} > 1 ? "$_ x$self->{POSSESSIONS}{$_}" : "$_"}
				sort keys %{ $self->{POSSESSIONS} });

	my $name;

	if ($self->career) {
		$name .= $self->rank >= 5 ? 'Retired ' : ''; # Ex-
		$name .= ucfirst(lc($self->career));
	}

	my $title = Traveller::Career::rank($self->career, $self->rank);
	$name .= (' ' . $title) if ($title);
	$name .= (' ' . $self->name) if ($self->name);

	my $s = '';
	$s .= ($self->{HISTORY} . ">>>\n") if ($verbose);

	$s .= <<END_OF_LISTING;
$name $upp  Age $age  $terms  Cr $credits
END_OF_LISTING

# 	$s .= "$skills\n"      if $skills;
# 	$s .= "$possessions\n" if $possessions;

	$s .= (Text::Wrap::wrap("", "", ($skills)) . "\n")		if $skills;;
	$s .= (Text::Wrap::wrap("", "", ($possessions)) . "\n")	if $possessions;

	$s .= "** DECEASED **\n" if $self->{DECEASED};

	return $s;
}

sub report {
	my $self = shift;
	print $self->listing;
}

sub append_history {
	my $self = shift;
	$self->{HISTORY} .= shift;
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

$Text::Wrap::separator = ",\n";
$Text::Wrap::columns = 60;
$Text::Wrap::break = qr/,\s/;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;
