EXT=-1.4
OBJ=../../../lpc-ext/src/lpc_ext.o
H=../../../lpc-ext/src/lpc_ext.h
CFLAGS=-fPIC -DPIC
LDFLAGS=-shared
UUID_LDFLAGS=`uuid-config --ldflags`
MATH_LDFLAGS=-lm
CC=cc
LD=$(CC)

clean:
	rm -f bin/* kfun/*.o

all:	bin/uuid_kfun$(EXT) bin/math_kfun$(EXT) bin/string_kfun$(EXT)

kfun/uuid_kfun.o:	kfun/uuid_kfun.c $(H)
	$(CC) -o $@ -c $(CFLAGS) -I. -I../../../lpc-ext/src kfun/uuid_kfun.c

bin/uuid_kfun$(EXT):	kfun/uuid_kfun.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) $(UUID_LDFLAGS) $+

kfun/math_kfun.o:	kfun/math_kfun.c $(H)
	$(CC) -o $@ -c $(CFLAGS) -I. -I../../../lpc-ext/src kfun/math_kfun.c

bin/math_kfun$(EXT):	kfun/math_kfun.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) $(MATH_LDFLAGS) $+

kfun/string_kfun.o:	kfun/string_kfun.c $(H)
	$(CC) -o $@ -c $(CFLAGS) -I. -I../../../lpc-ext/src kfun/string_kfun.c

bin/string_kfun$(EXT):	kfun/string_kfun.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) $+
