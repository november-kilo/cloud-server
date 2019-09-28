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
SDS=./sds
STRING=.

all:	math_kfuns.$(EXT) perl_kfuns.$(EXT) string_kfuns.$(EXT)

math:	math_kfuns.$(EXT)

perl:	perl_kfuns.$(EXT)

string:	string_kfuns.$(EXT)

$(MATH)/math_kfuns.o:	$(MATH)/math_kfuns.c
	$(CC) -o $@ -c $(CFLAGS) -I$(EXTMOD) -Wall $(MATH)/math_kfuns.c

math_kfuns.$(EXT):	$(MATH)/math_kfuns.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) -lm $+

$(PERL)/perl_kfuns.o:	$(PERL)/perl_kfuns.c
	$(CC) -o $@ -c $(CFLAGS) -I$(EXTMOD) -Wall `perl -MExtUtils::Embed -e ccopts` $(PERL)/perl_kfuns.c

perl_kfuns.$(EXT):	$(PERL)/perl_kfuns.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) `perl -MExtUtils::Embed -e ldopts` $+

$(SDS)/sds.o:	$(SDS)/sds.c
	$(CC) -o $@ -c $(CFLAGS) -I$(SDS) -Wall $(SDS)/sds.c

$(STRING)/string_utils.o:	$(STRING)/string_utils.c
	$(CC) -o $@ -c $(CFLAGS) -I$(SDS) -I$(EXTMOD) -Wall $(STRING)/string_utils.c

$(STRING)/string_kfuns.o:	$(STRING)/string_kfuns.c
	$(CC) -o $@ -c $(CFLAGS) -I$(EXTMOD) -I$(SDS) -Wall $(STRING)/string_kfuns.c

string_kfuns.$(EXT):		$(STRING)/string_kfuns.o $(STRING)/string_utils.o $(SDS)/sds.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) $+

clean:
	rm -f *.o $(SDS)/*.o *.$(EXT);

