INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= pthread.c helloworld.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	pthread helloworld

clean:
	-rm -f *.o *.d
	-rm -f pthread
	-rm -f helloworld

pthread: pthread.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread

helloworld: helloworld.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
