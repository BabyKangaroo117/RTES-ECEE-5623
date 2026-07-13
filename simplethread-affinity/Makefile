INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= pthread.c HW3.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	pthread HW3

clean:
	-rm -f *.o *.d
	-rm -f pthread HW3

pthread: pthread.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread

HW3: HW3.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread
depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
