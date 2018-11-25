# define the commands we'll use for compilation and library building
AR = ar
ARFLAGS = rcs
CC = gcc

GOOGLE_TEST_INCLUDE=${HOME}/src/googletest-release-1.8.0/googletest/include/

# define useful flags to cc/ld/etc.
CFLAGS = -g -Wall -I. -I.. -Ihtll 
LDFLAGS = -L. 


#define common dependencies
OBJS = docset.o docidmap.o fileparser.o filecrawler.o movieIndex.o LinkedList.o Hashtable.o movie.o queryprocessor.o
HEADERS = fileparser.h filecrawler.h docidmap.h movieIndex.h docset.h movie.h



# compile everything
all: main test


main: main.c $(OBJS)
	gcc -no-pie -pthread -Wall -g -o main main.c $(OBJS) -L. 


LinkedList.o: htll/LinkedList.o
	@echo ===========================
	@echo Moving LinkedList.o for testing...
	@echo ===========================
	cp htll/LinkedList.o . 

Hashtable.o: htll/Hashtable.o
	@echo ===========================
	@echo Moving Hashtable.o for testing...
	@echo ===========================
	cp htll/Hashtable.o . 

%.o: %.c $(HEADERS) FORCE
	$(CC) $(CFLAGS) -c $<

test: test_suite.o $(OBJS)
	g++ -no-pie -pthread -o test_suite test_suite_a4.o $(OBJS) \
		 -L${HOME}/lib/gtest -lgtest -lpthread
	@echo ===========================
	@echo Tests built.
	@echo Note: the tests do have warnings. 
	@echo To run tests: ./test_suite


test_suite.o: test_suite_a4.cc
	g++ -c -Wall -I $(GOOGLE_TEST_INCLUDE) test_suite_a4.cc \
		-o test_suite_a4.o

clean: FORCE
	/bin/rm -f *.o *~ main

FORCE:
