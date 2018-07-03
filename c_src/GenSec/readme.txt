
Here are new revisions of old programs: gensec, mapsub, and subsec.

Gensec is a sector generator.  The new features are that it's
MegaTraveller compatible, allows you to generate a single sector at a
time, and allows you to specify system density and sector maturity with
words (rift, sparse, dense, mature, frontier, etc.).  It also has a UWP
format compatible with MegaTraveller, with the population multiplier,
gas giant and planetoid counts.  It also logs the version of the format
in the output file, so that other programs can recognize what version of
gensec output they have.  An acknowledgement goes to Fred Schiff for
supplying the changes to this program.

Mapsub is a subsector mapping program.  It takes one subsector's worth
of gensec output and turns it into a hex map of the subsector.  Two
modes of output are supported: ascii, in which the hexes are drawn with
standard ascii symbols like underscore, slash, backslash, and asterisk;
and postscript, suitable for sending to most postscript devices.  This
program has been totally rewritten and is much more maintainable,
readable, and expandable than it used to be.  It can esily be adjusted
to add other forms of output or formats of input.  Acknowledgements are
due to Fred Schiff for supplying changes to the old version of mapsub,
and to Yngve Larsson for the postscript additions.  It also fixes the
bug where the hex staggering was incompatible with the Traveller rules.

Subsec is an AWK program which helps you extract one subsector's worth
of worlds out of an entire sector's worth of worlds.  It understands
both the old-fashioned and the new-fangled gensec ouutput formats.

I've tested these on a Sun 3/60 running SunOS 4.0, and a VAX 11/780
running BSD.  Make sure if you run any system that uses index() in place
of strchr() or random() in place of rand() that you compile with the
-DBSD option.  Particularly, the BSD rand() output looks very flaky and
using random() is MUCH better.  For example, an entire sector of worlds
ALL had only ODD numbers for world sizes, when I didn't use the -DBSD
option.  

If you come up with any fixes/additions/suggestions, please let me know
and I will happily graft them in.
