# variables and directives that get used in the makefile
CC = clang
CFLAGS = -g -Wall -fpic
CPPFLAGS = 
.PHONY : clean

TESTSRC := $(wildcard test?1.c)
TESTOBJ := $(patsubst %.c,%.o,$(wildcard test?1.c))
TESTPROG := $(patsubst %.c,%,$(wildcard test?1.c))

all: libthreadsalive.a $(TESTPROG) ctxtest

libthreadsalive.a: threadsalive.o 
	$(AR) cruv $@ threadsalive.o 

ctxtest: ctxtest.o
	$(CC) -o $@ $(patsubst %.c,%.o,$<) 

$(TESTPROG): %: %.o libthreadsalive.a
	$(CC) -o $@ $(patsubst %.c,%.o,$<) libthreadsalive.a

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	rm -f *.o libthreadsalive.a $(TESTOBJ) $(TESTPROG) ctxtest

