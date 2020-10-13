VPATH = ../libap:../pr
SEARCHDIR = -I../libap
SEARCHDIR2 = -I../pr
LIBPATH = ../libap/
LIBPATH2 = ../pr/
MYLIB =  random.o
MYLIB2 = $(LIBPATH2)io.o $(LIBPATH)utility.o $(LIBPATH2)manip.o
LIBS = -lm
CFLAGS = -O
COMP = gcc

random.o: random.c
	$(COMP) -c $(CFLAGS) random.c

rbg1.o: rbg1.c
	$(COMP) -c $(CFLAGS) rbg1.c $(SEARCHDIR) $(SEARCHDIR2)

rbg1:	rbg1.o random.o
	$(COMP) $(CFLAGS) -o rbg1 rbg1.o $(MYLIB) $(LIBS)

rbg2.o: rbg2.c
	$(COMP) -c $(CFLAGS) rbg2.c $(SEARCHDIR) $(SEARCHDIR2)

rbg2:	rbg2.o random.o
	$(COMP) $(CFLAGS) -o rbg2 rbg2.o $(MYLIB) $(LIBS)

chang: chang.c
	$(COMP) $(CFLAGS) -o chang chang.c

#import dependencies

rbg1.o: misc.h bgraph.h random.h
rbg2.o: misc.h bgraph.h random.h
