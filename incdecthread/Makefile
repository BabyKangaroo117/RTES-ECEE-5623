INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O3 $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= pthread.c assignment-2.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	pthread assignment-2

clean:
	-rm -f *.o *.d
	-rm -f perfmon pthread assignment-2

distclean:
	-rm -f *.o *.d
	-rm -f pthread assignment-2

pthread: pthread.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread
	
pthread: assignment-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread


depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
