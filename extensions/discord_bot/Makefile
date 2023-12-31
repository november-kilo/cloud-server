EXT=-1.4
OBJ=../../../lpc-ext/src/lpc_ext.o
H=../../../lpc-ext/src/lpc_ext.h
CFLAGS=-fPIC -DPIC
LDFLAGS=-shared
CC=cc
LD=$(CC)
BOT_FLAGS=-ldiscord -lcurl -pthread

all:	bin/discord_kfun$(EXT) discord_bot

discord_bot:
	$(CC) ./bot/discord_bot.c -o bin/discord_bot $(BOT_FLAGS)

kfun/discord_kfun.o:	kfun/discord_kfun.c $(H)
	$(CC) -o $@ -c $(CFLAGS) -I. -I../../../lpc-ext/src kfun/discord_kfun.c

bin/discord_kfun$(EXT):	kfun/discord_kfun.o $(OBJ)
	$(LD) -o $@ $(LDFLAGS) $+

clean:
	rm -f bin/discord_kfun$(EXT) kfun/*.o
	rm -rf bin/discord_bot
