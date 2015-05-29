package Traveller::Name;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# _______ ______ _______ ___ ___ _______ _____   _____   _______ ______ 
#|_     _|   __ |   _   |   |   |    ___|     |_|     |_|    ___|   __ \
#  |   | |      |       |   |   |    ___|       |       |    ___|      <
#  |___| |___|__|___|___|\_____/|_______|_______|_______|_______|___|__|
#
#	file:			Name.pm
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

Traveller::Name - A module for generating names for Traveller.

The Traveller game in all forms is owned by Far Future Enterprises.
Copyright 1977 - 2003 Far Future Enterprises. Traveller is a registered
trademark of Far Future Enterprises.

=head1 SYNOPSIS

 use Traveller::Name;
 print Traveller::Name::generate ();
 print Traveller::Name::generate ( LANGUAGE => 'vilani', SYLLABLES => 3);

=head1 DESCRIPTION

The Traveller::Name module can generate names for Vilani, Zhodani, Aslan,
Droyne, K'Kree and Vargr languages, according to the system presented in
the Alien Modules and the Journal of the Travellers' Aid Society Issue x.

=head1 SEE ALSO

Traveller.pm

=head1 METHODS

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

use strict;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

my %MAP;

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

=head2 generate ( LANGUAGE => 'vilani', SYLLABLES => 3 );

Generate a name. Optionally accepts a language and number of syllables;
if not provided, defaults to a random language and between two and six
syllables.

=cut

sub generate {

	my %arg = (
		LANGUAGE	=> '',
		SYLLABLES	=> Traveller::Dice::roll (1, 0, 2),
		@_,
	);

	$arg{LANGUAGE} = lc($arg{LANGUAGE});

	if ($arg{LANGUAGE} eq 'any' || $arg{LANGUAGE} eq '') {
		$arg{LANGUAGE} = (keys %MAP)[Traveller::Dice::range (scalar keys %MAP)]
	}

	my $w	= ''; # the word to return
	my $s	= ''; # the syllable pattern
	my $sp	= ''; # the previous syllable pattern
	my $a	= ''; # the generated syllable
	my $ap	= ''; # the previous generated syllable

	while ($arg{SYLLABLES}--) {

		$s = syllable ($arg{LANGUAGE}, $sp);

		goto DONE unless $s;

		do {
			$a = syllable_fill ($arg{LANGUAGE}, $s);
		} until ($a = syllable_check ($arg{LANGUAGE}, $s, $sp, $ap, $a));

		$sp = $s;
		$ap = $a;

		$w .= $a;
		
		goto DONE if ($arg{LANGUAGE} eq 'kkree' && $s eq 'cvc');
	}

DONE:

	return $w;
}

sub syllable {
	my $l = shift; # language to use
	my $p = shift; # previous syllable

	my $t = 0;
	
	if ($p) {
		if 		($l eq 'aslan') {
			if ($p eq 'cv' || $p eq 'v') {
				$t = 0;
			}
			if ($p eq 'vc' || $p eq 'cvc') {
				$t = 1;
			}
		}
		elsif	($l eq 'droyne') {
			if ($p eq 'cv' || $p eq 'v') {
				$t = 0;
			}
			if ($p eq 'vc' || $p eq 'cvc') {
				$t = 1;
			}
		}
		elsif	($l eq 'kkree') {
			if ($p eq 'vc') {
				$t = 1;
			}
			if ($p eq 'cv' || $p eq 'v') {
				$t = 2;
			}
		}
		elsif	($l eq 'vargr') {
			if ($p eq 'vc' || $p eq 'cvc') {
				$t = 0;
			}
			if ($p eq 'cv' || $p eq 'v') {
				$t = 1;
			}
		}
		elsif	($l eq 'vilani') {
		}
		elsif	($l eq 'zhodani') {
			if ($p eq 'cv' || $p eq 'v') {
				$t = 0;
			}
			if ($p eq 'vc' || $p eq 'cvc') {
				$t = 1;
			}
		}
	}

	return $MAP{$l}{"TABLE$t"}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
}

sub syllable_fill {
	my $l = shift; # language to use
	my $s = shift; # syllable pattern

	my $a = '';

	if		($s eq 'v') {
		$a .= $MAP{$l}{'vo'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
	}
	elsif	($s eq 'vc') {
		$a .= $MAP{$l}{'vo'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
		$a .= $MAP{$l}{'ic'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
	}
	elsif	($s eq 'cv') {
		$a .= $MAP{$l}{'ic'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
		$a .= $MAP{$l}{'vo'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
	}
	elsif	($s eq 'cvc') {
		$a .= $MAP{$l}{'ic'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
		$a .= $MAP{$l}{'vo'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
		$a .= $MAP{$l}{'fc'}[(Traveller::Dice::roll (1) * Traveller::Dice::roll (1) * Traveller::Dice::roll (1)) - 1];
	}

	return $a;
}

sub syllable_check {
	my $l	= shift; # language to use
	my $s	= shift; # syllable pattern
	my $sp	= shift; # previous syllable pattern
	my $ap	= shift; # previous generated syllable
	my $a	= shift; # accumulated syllable

	if 		($l eq 'aslan') {
		return 0 if (($s eq 'cv' || $s eq 'cvc') && ($sp eq 'vc' || $sp eq 'cvc'));
		return 0 if (($sp eq 'v') && ($ap eq $a));
	}
	elsif	($l eq 'droyne') {
		if ($ap =~ /([aeiouy])/ && $a =~ /$1./) {
			$a =~ /.(.)/;
			$a = $ap . $1;
			return $a;
		}
	}
	elsif	($l eq 'kkree') {
		return 0 if (($s eq 'cv' || $s eq 'cvc') && ($sp eq 'vc' || $sp eq 'cvc'));
		return 0 if (($s eq 'v'  || $s eq 'vc' ) && ($sp eq 'cv' || $sp eq 'v'  ));
	}
	elsif	($l eq 'vargr') {
		return 0 if (($s eq 'v'  || $s eq 'vc' ) && ($sp eq 'cv' || $sp eq 'v'  ));
	}
	elsif	($l eq 'vilani') {
	}
	elsif	($l eq 'zhodani') {
		return 0 if ($ap =~ /[aeiouy]{3}/ && $a =~ /[aeiouy]{3}/);
		return 'r' if ($a eq 'rrr');
	}

	return $a;
}

=head1 AUTHOR

Paul Sanford Toney <signalgk@gmail.com>

=cut

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
while (defined (my $key = <Traveller::Name::DATA>)) {
	my (%ht);

	chomp ($key);

	foreach my $k (0..2) {
		my $s = <Traveller::Name::DATA>;
		chomp ($s);
		my @a = split (/,/, $s);
		$ht{"TABLE$k"} = [ @a ];
	}

	foreach my $k (qw(ic vo fc)) {
		my $s = <Traveller::Name::DATA>;
		chomp ($s);
		my @a = split (/,/, $s);
		$ht{$k} = [ @a ];
	}

	$MAP{$key} = { %ht };
}

#  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

1;

__DATA__
zhodani
v,v,v,cv,cv,cv,vc,vc,vc,vc,vc,vc,vc,vc,vc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc
v,v,v,v,v,v,cv,cv,cv,cv,cv,cv,vc,vc,vc,vc,vc,vc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc

b,b,b,b,b,bl,bl,bl,br,br,br,br,br,ch,ch,ch,ch,ch,ch,ch,ch,ch,ch,ch,ch,cht,cht,cht,cht,cht,cht,cht,d,d,d,d,d,d,d,d,d,dl,dl,dl,dl,dl,dl,dl,dr,dr,dr,dr,dr,f,f,f,f,f,fl,fl,fl,fr,fr,fr,j,j,j,j,j,j,j,jd,jd,jd,jd,jd,k,k,k,k,k,kl,kl,kr,kr,l,l,l,m,m,n,n,n,n,n,n,n,n,p,p,p,p,p,p,p,pl,pl,pl,pl,pl,pl,pl,pr,pr,pr,q,q,ql,ql,qr,qr,r,r,r,r,r,s,s,s,s,s,s,s,sh,sh,sh,sh,sh,sh,sh,sht,sht,sht,sht,sht,sht,sht,t,t,t,t,t,st,st,st,st,st,st,st,tl,tl,tl,tl,tl,tl,tl,tl,tl,tl,ts,ts,ts,v,v,v,v,v,vl,vl,vr,vr,y,y,y,z,z,z,z,z,zd,zd,zd,zd,zd,zd,zd,zd,zd,zd,zh,zh,zh,zh,zh,zh,zh,zhd,zhd,zhd,zhd,zhd,zhd,zhd,zhd,zhd,zhd
a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ia,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,ie,o,o,o,o,o,o,o,o,o,o,o,o,o,o,r,r,r,r,r,r
b,b,bl,bl,bl,bl,bl,bl,bl,br,br,br,br,br,br,br,ch,ch,ch,ch,ch,d,d,d,d,dl,dl,dl,dl,dl,dl,dl,dr,dr,dr,dr,dr,dr,dr,f,f,f,f,f,fl,fl,fl,fl,fl,fr,fr,fr,fr,fr,j,j,j,j,k,k,kl,kl,kl,kl,kr,kr,l,l,l,l,l,l,l,l,l,l,l,l,m,m,n,n,nch,nch,nch,nch,nch,nch,nch,nj,nj,nj,nj,nj,ns,ns,ns,ns,ns,nsh,nsh,nsh,nsh,nsh,nsh,nsh,nt,nt,nt,nt,nts,nts,nts,nts,nz,nz,nz,nz,nz,nzh,nzh,nzh,nzh,nzh,nzh,nzh,p,p,pl,pl,pl,pl,pl,pl,pl,pr,pr,pr,pr,pr,pr,pr,q,q,ql,ql,qr,qr,r,r,r,r,r,sh,sh,sh,sh,sh,sh,sh,t,t,t,t,ts,ts,ts,ts,ts,ts,ts,tl,tl,tl,tl,tl,tl,tl,tl,tl,v,v,v,v,v,vl,vl,vl,vl,vr,vr,vr,vr,vr,z,z,z,z,z,z,z,z,z,zh,zh,zh,zh,zh,zh,zh,',',',',','
aslan
v,v,v,v,v,v,v,v,v,v,v,v,v,cv,cv,cv,cv,cv,cv,cv,cv,cv,vc,vc,vc,vc,vc,vc,vc,vc,cvc,cvc,cvc,cvc,cvc,cvc
v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc

f,f,f,f,f,f,f,f,f,f,f,f,ft,ft,ft,ft,ft,ft,ft,ft,ft,ft,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,hf,hf,hf,hf,hf,hk,hk,hk,hk,hk,hk,hk,hk,hk,hk,hk,hk,hl,hl,hl,hl,hl,hl,hl,hl,hr,hr,hr,hr,hr,hr,hr,ht,ht,ht,ht,ht,ht,ht,ht,ht,ht,ht,ht,hw,hw,hw,hw,hw,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kht,kht,kht,kht,kht,kht,kht,kht,kht,kht,kht,kt,kt,kt,kt,kt,kt,kt,kt,kt,kt,l,l,l,l,l,r,r,r,r,r,r,r,s,s,s,s,s,s,s,s,s,s,st,st,st,st,st,st,st,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,tl,tl,tl,tl,tl,tr,tr,tr,tr,tr,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w
a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,ai,ai,ai,ai,ai,ai,ai,ai,ai,ai,ai,ao,ao,ao,ao,ao,ao,ao,ao,au,au,au,au,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ea,ei,ei,ei,ei,ei,ei,ei,ei,ei,ei,ei,ei,ei,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,iy,iy,iy,iy,iy,iy,iy,iy,iy,iy,iy,iy,o,o,o,o,o,o,o,o,oa,oa,oa,oa,oi,oi,oi,oi,oi,oi,oi,oi,ou,ou,ou,ou,ou,u,u,u,u,ua,ua,ua,ua,ui,ui,ui,ui,ya,ya,ya,ya,ya,ya,ya,ya,ye,ye,ye,ye,ye,ye,ye,ye,yo,yo,yo,yo,yu,yu,yu,yu
h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,h,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,lr,lr,lr,lr,lr,lr,lr,lr,lr,lr,lr,lr,lr,lr,l,l,l,l,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,rl,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,w,',',',',',',',',',',',',','
kkree
v,v,v,v,v,v,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,vc,vc,vc,vc,vc,vc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc
v,v,v,v,v,v,v,v,v,v,v,v,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc
cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc
b,b,g,g,g,g,g,g,gh,gh,gh,gh,gh,gh,gh,gh,gh,gh,gh,gh,gh,gn,gn,gn,gn,gn,gn,gn,gn,gn,gr,gr,gr,gr,gz,gz,hk,hk,hk,hk,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kt,kt,kt,l,l,l,l,l,l,l,l,l,l,l,l,m,m,m,m,mb,mb,mb,n,n,n,n,n,n,n,n,n,n,p,p,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,rr,rr,rr,rr,rr,r,rr,rr,rr,rr,rr,rr,rr,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,tr,tr,tr,tr,x,x,x,x,x,x,x,x,x,xk,xk,xk,xk,xk,xk
a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,aa,aa,aa,aa,aa,aa,aa,aa,aa,aa,e,e,e,e,e,e,e,e,e,e,e,e,e,e,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,ee,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,o,o,o,o,o,oo,oo,oo,oo,oo,oo,oo,oo,oo,oo,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,uu,uu,uu,uu,uu,uu,uu,uu,uu,uu,',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',!,!,!,!,!,!,!,!,!,!,!,!,!,!,!!,!!,!!,!!,!!,!!,!',!',!'
b,b,b,b,b,g,g,g,g,g,g,g,g,g,g,gh,gh,gh,gh,gh,gr,gr,gr,gr,gr,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,kr,l,l,l,l,l,l,l,l,l,l,m,m,m,m,m,n,n,n,n,n,n,n,n,n,n,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,p,p,p,p,p,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,t,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,xk,xk,xk,xk,xk,xk
vargr
v,v,v,v,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,cv,cv,cv,cv,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc
cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc

d,d,d,d,d,d,d,d,d,dh,dh,dh,dh,dh,dh,dh,dh,dh,dz,dz,dz,dz,dz,f,f,f,f,f,f,f,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,gh,gh,gh,gh,gh,gh,gh,gh,gh,gh,gh,gn,gn,gn,gv,gv,gv,gv,gv,gv,gv,gz,gz,gz,gz,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,kf,kf,kf,kf,kf,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kn,kn,kn,kn,kn,kn,ks,ks,ks,ks,ks,ks,ks,l,l,l,l,ll,ll,ll,ll,ll,ll,ll,ll,n,n,n,n,n,n,n,ng,ng,ng,ng,ng,r,r,r,r,r,r,r,r,r,r,r,rr,rr,rr,rr,rr,rr,rr,rr,s,s,s,s,s,s,s,s,s,s,s,t,t,t,t,t,t,t,th,th,th,th,th,th,th,th,ts,ts,ts,ts,ts,v,v,v,v,v,v,v,v,v,v,z,z,z,z,z,z,z,z,z,z,z,z
a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,ae,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,i,i,i,i,i,i,i,i,i,i,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,oe,oe,oe,oe,oe,oe,oe,oe,oe,oe,oe,oe,oe,oe,oe,oe,ou,ou,ou,ou,ou,ou,ou,ou,ou,ou,ou,ou,ou,ou,ou,ou,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue,ue
dh,dh,dh,dh,dh,dz,dz,dz,dz,dz,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,gh,gh,gh,gh,gh,gh,gh,gh,gh,gh,ghz,ghz,ghz,ghz,ghz,gz,gz,gz,gz,gz,k,k,k,k,k,k,k,k,k,k,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,khs,khs,khs,khs,khs,ks,ks,ks,ks,ks,ks,l,l,l,l,l,l,l,l,l,l,ll,ll,ll,ll,ll,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,ng,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rr,rrg,rrg,rrg,rrg,rrg,rrgh,rrgh,rrgh,rrgh,rrgh,rs,rs,rs,rs,rs,rz,rz,rz,rz,rz,s,s,s,s,s,th,th,th,th,th,ts,ts,ts,ts,ts,z,z,z,z,z,z,z,z,z,z
droyne
v,v,v,v,v,v,v,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,vc,cvc,cvc,cvc,cvc,cvc,cvc,cvc
v,v,v,v,v,v,cv,cv,cv,cv,cv,cv,vc,vc,vc,vc,vc,vc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc

b,b,b,b,b,b,b,b,br,br,br,br,d,d,d,d,d,d,d,d,d,d,d,d,dr,dr,dr,dr,dr,f,f,f,f,f,f,f,f,f,f,f,f,f,h,h,h,h,h,h,h,h,h,h,h,h,h,k,k,k,k,k,k,k,k,k,k,k,k,k,kr,kr,kr,l,l,l,l,l,l,l,l,l,m,m,m,m,m,m,m,m,m,m,m,m,m,m,n,n,n,n,n,n,n,n,n,n,n,n,n,n,p,p,p,p,p,p,p,p,p,p,p,p,pr,pr,r,r,r,r,r,r,r,r,r,r,r,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,ss,ss,ss,ss,ss,ss,ss,ss,ss,ss,st,st,st,t,t,t,t,t,t,t,t,t,t,th,th,th,th,th,th,tr,tr,tr,ts,ts,ts,ts,ts,ts,ts,ts,ts,tw,tw,tw,tw,tw,tw,tw,tw,tw,v,v,v,v,v,v,v,v,v
a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,ay,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,oy,oy,oy,oy,oy,oy,oy,oy,oy,oy,oy,oy,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,ya,ya,ya,ya,ya,ya,ya,ya,ya,yo,yo,yo,yo,yo,yo,yo,yu,yu,yu,yu,yu,yu,yu,yu,yu,yu,yu
b,b,b,b,b,b,d,d,d,d,d,d,d,d,d,d,d,f,f,f,f,f,h,h,h,h,h,h,k,k,k,k,k,k,k,k,l,l,l,l,lb,lb,ld,ld,ld,ld,ld,ld,ld,lk,lk,lk,lk,lm,lm,lm,ln,lp,ls,ls,lt,lt,m,m,m,m,m,m,m,m,m,m,m,n,n,n,n,n,n,n,p,p,p,p,p,p,p,p,p,p,p,p,r,r,r,r,r,r,r,r,r,rd,rd,rd,rf,rf,rk,rk,rk,rk,rk,rm,rm,rm,rm,rn,rn,rn,rp,rs,rs,rs,rs,rt,rt,rt,rt,rt,rv,rv,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,sk,sk,sk,sk,sk,sk,ss,ss,ss,ss,ss,ss,ss,ss,st,st,st,st,st,t,t,t,t,t,t,t,t,t,t,t,t,th,th,th,th,th,th,ts,ts,ts,ts,ts,ts,ts,ts,ts,ts,v,v,v,v,x,x,x,x,x,x,x,x,x,x,x,x
vilani
v,v,v,v,v,v,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,vc,vc,vc,vc,vc,vc,vc,vc,cvc,cvc,cvc,cvc,cvc,cvc,cvc
cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cv,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc,cvc

k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,d,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,l,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,kh,n,n,n,n,n,n,n,n,n,n,s,s,s,s,s,s,s,s,s,s,p,p,p,p,b,b,b,b,z,z,z,z,r,r,r,r
a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,a,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,e,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,i,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,u,aa,aa,aa,aa,aa,aa,aa,aa,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,ii,uu,uu,uu,uu,uu,uu,uu,uu
r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,n,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,sh,g,g,g,g,g,g,g,g,g,g,g,g,g,g,g,s,s,s,s,s,s,s,s,s,s,s,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k,k
