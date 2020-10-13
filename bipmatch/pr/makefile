VPATH = ../libap
SEARCHDIR = -I$(VPATH)
LIBPATH = $(VPATH)/
LIBS = -lm
MYLIB =  io.o manip.o $(LIBPATH)utility.o $(LIBPATH)timer.o
OBJ = main.o match.o
SRCS = main.c match.c
CFLAGS = -O
COMP = gcc

manip.o: manip.c
	$(COMP) -c $(CFLAGS) manip.c 

io.o: io.c
	$(COMP) -c $(CFLAGS) io.c $(SEARCHDIR)

match.o : match.c
	$(COMP) -c $(CFLAGS) match.c $(SEARCHDIR)

main.o : main.c
	$(COMP) -c $(CFLAGS) main.c $(SEARCHDIR)

match:	$(OBJ) $(MYLIB)
	$(COMP) $(CFLAGS) -o match $(OBJ) $(MYLIB) $(LIBS) $(SEARCHDIR)

#import dependencies

io.o:		misc.h utility.h bgraph.h manip.h io.h
manip.o:	misc.h utility.h bgraph.h manip.h
main.o match.o: misc.h utility.h bgraph.h io.h timer.h

