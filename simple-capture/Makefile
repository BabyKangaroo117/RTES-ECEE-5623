INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lrt -lpthread

HFILES= 
CFILES= capture.c decoupled-capture.c 

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	capture decoupled-capture

clean:
	-rm -f *.o *.d *.ppm *.pgm
	-rm -f capture decoupled-capture

distclean:
	-rm -f *.o *.d

capture: ${OBJS}
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o $(LIBS)

decoupled-capture: ${OBJS}
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o $(LIBS)
depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
