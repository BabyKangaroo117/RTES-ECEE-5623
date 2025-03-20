INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O3 $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= pthread.c assignment-2.c assignment-3.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	pthread assignment-2 assignment-3

clean:
	-rm -f *.o *.d
	-rm -f perfmon pthread assignment-2 assignment-3

distclean:
	-rm -f *.o *.d
	-rm -f pthread assignment-2 assignment-3

pthread: pthread.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread
	
assignment-2: assignment-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread

assignment-3: assignment-3.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
