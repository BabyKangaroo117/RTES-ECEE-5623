INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= pthread.c assignment-1.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	pthread assignment-1

clean:
	-rm -f *.o *.d
	-rm -f pthread assignment-1

pthread: pthread.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread 

assignment-1: assignment-1.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread 
depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
