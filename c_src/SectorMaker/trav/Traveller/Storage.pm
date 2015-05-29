package Traveller::Storage;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Storage.pm
#	author:			paul sanford toney <signalgk@gmail.com>
#
#	description:	
#
#	%license%
#
#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use strict;

use XML::SAX;
use XML::SAX::PurePerl;

use YAML;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#	xml

	package SAXHandler_Sector;
	use base qw(XML::SAX::Base);
	use Data::Dumper;

	sub new {
		my $pkg = shift;
		my %arg = @_;
		my $obj = bless {
			'sector'		=> undef,
			'subsector'		=> undef,
		}, $pkg;
		return $obj;
	}

	sub start_element {
		my ($self, $e) = @_;

		if ($e->{Name} eq 'sector') {
			$self->{'sector'} = Traveller::Sector->new;
			$self->{'sector'}->name($e->{Attributes}{'{}name'}{Value});
		}
	
		if ($e->{Name} eq 'subsector') {
			$self->{'subsector'} = Traveller::Subsector->new;
			$self->{'subsector'}->name($e->{Attributes}{'{}name'}{Value});
			$self->{'sector'}->subsector_at($e->{Attributes}{'{}quadrant'}{Value}, $self->{'subsector'});
		}
	
		if ($self->{'subsector'} && $e->{Name} eq 'system') {
			my $world = Traveller::World->new(
				NAME		=> $e->{Attributes}{'{}name'}{Value},
				LOCATION	=> $e->{Attributes}{'{}location'}{Value},
				PROFILE		=> $e->{Attributes}{'{}profile'}{Value},
			);
			$self->{'subsector'}->add_world($world);
		}
	}

	sub end_element {
		my ($self, $e) = @_;

		if ($e->{Name} eq 'subsector') {
			$self->{'subsector'} = undef;
		}
	}

	package Traveller::Storage;

sub sector_from_xml {

	my $handler = SAXHandler_Sector->new;
	my $parser = XML::SAX::ParserFactory->parser(
		Handler => $handler
	);

	$parser->parse_uri(shift);

	return $handler->{'sector'};
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#	yaml

sub sector_to_yaml {
	my $sector = shift;
	my $path = shift;
	YAML::DumpFile($path, $sector);
}

sub sector_from_yaml {
	my $path = shift;
	YAML::LoadFile($path);
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;
