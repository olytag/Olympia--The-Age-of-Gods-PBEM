#  Olympia PBM
#  (C) Copyright 1992, 1993 by Richard Skrenta, Jr.  All rights reserved.
#
#  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF RICHARD SKRENTA, JR.
#  IT REPRESENTS WORK-IN-PROGRESS AND IS NOT REDISTRIBUTABLE.
#
#  The copyright notice above does not evidence any actual or
#  intended publication of such source code.

CC=gcc
# Normal
LFLAGS = -lm 
CFLAGS = -g -DLINUX -DNEW_TRADE -m32
#CFLAGS = -g -DNEW_TRADE
# With ccmalloc
#LFLAGS = -lm -lccmalloc -ldl
#CFLAGS = -g -DLINUX -DNEW_TRADE -L /home/olyg3/Src/ccmalloc/lib
# With mpatrol
# LFLAGS = -lm -lmpatrol -lbfd -liberty
# CFLAGS = -g -DLINUX -DNEW_TRADE -L /home/srt/mpatrol/build/unix

OLYOBJ =	add.o adv.o alchem.o art.o basic.o beast.o build.o buy.o \
		check.o c1.o c2.o cloud.o code.o combat.o day.o dir.o \
		display.o eat.o faery.o garr.o gate.o glob.o gm.o hades.o \
		immed.o input.o io.o loc.o lore.o main.o make.o move.o \
		necro.o npc.o order.o perm.o produce.o quest.o relig.o \
		report.o savage.o scry.o seed.o sout.o stack.o stealth.o \
		storm.o summary.o swear.o u.o use.o effect.o trading.o \
		shipcraft.o mining.o win.o kill.o ranger.o times.o \
		artifacts.o map.o hero.o

OBJECTS = $(OLYOBJ) z.o

HEADERS =	loop.h code.h dir.h display.h etc.h garr.h loc.h order.h \
		sout.h stack.h swear.h u.h use.h effect.h relig.h combat.h \
		quest.h build.h kill.h artifacts.h eat.h

all:	oly

oly:	$(OBJECTS)
	$(CC) $(CFLAGS) -o tester $(OBJECTS) $(LFLAGS)

ztar:
	tar -czf ../tagsrc.tar.Z *.[ch] Makefile

clean:
	rm -f *.o

clobber:	clean
	rm -f oly

diff:
	@sh diff.sh

cpio:
	cd ..; make cpio

tags:
	ctags -t *.[ch]

install:
	mkdir ~/bin; cp tester ~/bin/olytag

$(OLYOBJ):	z.h oly.h oly3.h $(HEADERS)
z.o:		z.h
io.o:		io.c z.h oly.h $(HEADERS)
	$(CC) $(CFLAGS) -c io.c 2>&1 | grep -v multi-char
