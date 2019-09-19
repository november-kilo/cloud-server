EXT=1.0
DEFINES=
DEBUG=-ggdb
CCFLAGS=$(DEFINES) $(DEBUG)
CC=cc

CFLAGS=	-fPIC -DPIC $(CCFLAGS)
LD=$(CC)
LDFLAGS=-shared $(DEBUG)

EXTMOD=../../lpc-ext/src
OBJ=$(EXTMOD)/lpc_ext.o
PERL=.
MATH=.

all:	math_kfuns.$(EXT) perl_kfuns.$(EXT)

$(MATH)/math_kfuns.o:	$(MATH)/math_kfuns.c $(EXTMOD)/lpc_ext.h
	$(CC) -o $@ -c $(CFLAGS) -I$(EXTMOD) -Wall $(MATH)/math_kfuns.c

math_kfuns.$(EXT):	$(MATH)/math_kfuns.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) -lm $+

$(PERL)/perl_kfuns.o:	$(PERL)/perl_kfuns.c $(EXTMOD)/lpc_ext.h
	$(CC) -o $@ -c $(CFLAGS) -I$(EXTMOD) -Wall `perl -MExtUtils::Embed -e ccopts` $(PERL)/perl_kfuns.c

perl_kfuns.$(EXT):	$(PERL)/perl_kfuns.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) `perl -MExtUtils::Embed -e ldopts` $+

clean:
	rm -f *.o *.$(EXT);

