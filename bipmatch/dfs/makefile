VPATH = ../libdb
SEARCHDIR = -I$(VPATH)
LIBPATH = $(VPATH)/
LIBS = -lm
MYLIB =  $(LIBPATH)io.o  $(LIBPATH)manip.o  $(LIBPATH)utility.o $(LIBPATH)timer.o
OBJ = main.o fp.o
SRCS = main.c fp.c
CFLAGS = -O
COMP=gcc

main.o : main.c
	$(COMP) -c $(CFLAGS) main.c $(SEARCHDIR)
fp.o : fp.c
	$(COMP) -c $(CFLAGS) fp.c $(SEARCHDIR)

match:	$(OBJ) $(MYLIB)
	$(COMP) $(CFLAGSEXE) -o match $(OBJ) $(MYLIB) $(LIBS)

#import dependencies

fp.o:		fp.h misc.h utility.h graph.h
io.o:		misc.h utility.h graph.h manip.h io.h
manip.o:	misc.h utility.h graph.h manip.h
main.o:		fp.h misc.h utility.h graph.h io.h timer.h






