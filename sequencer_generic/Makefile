INCLUDE_DIRS = 
LIB_DIRS = 
CC=gcc

CDEFS=
CFLAGS= -O0 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= 

HFILES= 
CFILES= \
	seqgenex0.c \
	seqgen.c \
	seqgen2.c \
	seqgen3.c \
	seqv4l2.c \
	capturelib.c \
	assignment-5.c \
	assignment-1-course-2.c \
	assignment-2-course-2.c \
	assignment-3-course-2.c \
	assignment-4-course-2.c \
	assignment-5-course-2.c \
	assignment-6-course-2.c \
	assignment-7-course-2.c

SRCS= ${HFILES} ${CFILES}
OBJS= ${CFILES:.c=.o}

all:	seqgenex0 seqgen seqgen2 seqgen3 seqv4l2 clock_times capture assignment-5 assignment-1-course-2 assignment-2-course-2 assignment-3-course-2 assignment-4-course-2 assignment-5-course-2 \
	assignment-6-course-2 assignment-7-course-2 

clean:
	-rm -f *.o *.d frames/*.pgm frames/*.ppm
	-rm -f seqgenex0 seqgen seqgen2 seqgen3 seqv4l2 clock_times capture assignment-5 assignment-1-course-2 assignment-2-course-2 assignment-3-course-2 assignment-4-course-2 assignment-5-course-2 \
		assignment-6-course-2 assignment-7-course-2 
 

seqgenex0: seqgenex0.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

seqv4l2: seqv4l2.o capturelib.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o capturelib.o -lpthread -lrt

seqgen3: seqgen3.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

seqgen2: seqgen2.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

seqgen: seqgen.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

clock_times: clock_times.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

capture: capture.o capturelib.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o capturelib.o -lrt

assignment-5: assignment-5.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

assignment-1-course-2: assignment-1-course-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

assignment-2-course-2: assignment-2-course-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

assignment-3-course-2: assignment-3-course-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

assignment-4-course-2: assignment-4-course-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

assignment-5-course-2: assignment-5-course-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

assignment-6-course-2: assignment-6-course-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

assignment-7-course-2: assignment-7-course-2.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $@.o -lpthread -lrt

depend:

.c.o:
	$(CC) $(CFLAGS) -c $<
