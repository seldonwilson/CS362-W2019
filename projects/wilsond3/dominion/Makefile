CFLAGS= -Wall -Wextra -pedantic -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

testHelpers.o: testHelpers.c testHelpers.h
	gcc -c testHelpers.c -g $(CFLAGS)

unittest1: unittest1.c testHelpers.o dominion.o
	gcc -o unittest1 -g unittest1.c testHelpers.o dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c testHelpers.o dominion.o
	gcc -o unittest2 -g unittest2.c testHelpers.o dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c testHelpers.o dominion.o
	gcc -o unittest3 -g unittest3.c testHelpers.o dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c testHelpers.o dominion.o
	gcc -o unittest4 -g unittest4.c testHelpers.o dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c testHelpers.o dominion.o
	gcc -o cardtest1 -g cardtest1.c testHelpers.o dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c testHelpers.o dominion.o
	gcc -o cardtest2 -g cardtest2.c testHelpers.o dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c testHelpers.o dominion.o
	gcc -o cardtest3 -g cardtest3.c testHelpers.o dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c testHelpers.o dominion.o
	gcc -o cardtest4 -g cardtest4.c testHelpers.o dominion.o rngs.o $(CFLAGS)

unittestresults: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4
	echo "Unit Test Suite (Only Failures)" > unittestresults.out
	echo "Results for unittest1 – shuffle()" >> unittestresults.out
	echo "=================================" >> unittestresults.out
	./unittest1 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	echo "Results for unittest2 – getCost()" >> unittestresults.out
	echo "=================================" >> unittestresults.out
	rm -f dominion.c.gcov
	./unittest2 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	echo "Results for unittest3 – gainCard()" >> unittestresults.out
	echo "==================================" >> unittestresults.out
	rm -f dominion.c.gcov
	./unittest3 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	echo "Results for unittest4 – fullDeckCount()" >> unittestresults.out
	echo "=======================================" >> unittestresults.out
	rm -f dominion.c.gcov
	./unittest4 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	echo "Results for cardtest1 – Great Hall" >> unittestresults.out
	echo "==================================" >> unittestresults.out
	rm -f dominion.c.gcov
	./cardtest1 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	echo "Results for cardtest2 – Village" >> unittestresults.out
	echo "===============================" >> unittestresults.out
	rm -f dominion.c.gcov
	./cardtest2 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	echo "Results for cardtest3 – Smithy" >> unittestresults.out
	echo "==============================" >> unittestresults.out
	rm -f dominion.c.gcov
	./cardtest3 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	echo "Results for cardtest4 – Adventurer" >> unittestresults.out
	echo "==================================" >> unittestresults.out
	rm -f dominion.c.gcov
	./cardtest4 >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out
	
	rm -f dominion.c.gcov
	./unittest1 >/dev/null
	./unittest2 >/dev/null
	./unittest3 >/dev/null
	./unittest4 >/dev/null
	./cardtest1 >/dev/null
	./cardtest2 >/dev/null
	./cardtest3 >/dev/null
	./cardtest4 >/dev/null
	echo "Combined Test Suite dominion.c Coverage" >> unittestresults.out
	echo "=======================================" >> unittestresults.out
	gcov -bf ./dominion.c >> unittestresults.out

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
