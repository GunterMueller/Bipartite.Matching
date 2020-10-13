VPATH = ../libap:../pr
SEARCHDIR = -I../libap
SEARCHDIR2 = -I../pr
LIBPATH = ../libap/
LIBPATH2 = ../pr/
LIBS = -lm
MYLIB =  $(LIBPATH2)io.o $(LIBPATH)utility.o $(LIBPATH2)manip.o
CFLAGS = -O
COMP = gcc
FCOMP = /usr/lang/f77

d2b.o: d2b.c
	$(COMP) -c $(CFLAGS) d2b.c $(SEARCHDIR) $(SEARCHDIR2) -I../pr

d2b:	d2b.o $(MYLIB)
	$(COMP) $(CFLAGS) -o d2b d2b.o $(MYLIB)

timer.o:  timer.c
	$(COMP) -O -c timer.c

bcm:  bcm.f timer.o
	$(FCOMP) -O -o bcm timer.o bcm.f

#import dependencies

d2b.o : misc.h utility.h bgraph.h io.h manip.h

