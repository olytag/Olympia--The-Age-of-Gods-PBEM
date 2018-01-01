
CFLAGS = -g

OBJECTS = mapgen.o rnd.o z.o

all:	mapgen

mapgen:	$(OBJECTS)
	$(CC) $(CFLAGS) -o mapgen $(OBJECTS) $(LFLAGS)

clean:
	rm -f *.o

clobber:	clean
	rm -f mapgen

install:
	mkdir -p ~/bin;cp mapgen ~/bin

mapgen.o:	mapgen.c z.h
z.o:		z.c z.h
rnd.o:		rnd.c
