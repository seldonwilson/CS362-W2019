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

randomtestadventurer: randomtestadventurer.c testHelpers.o dominion.o
	gcc -o randomtestadventurer -g randomtestadventurer.c testHelpers.o dominion.o rngs.o $(CFLAGS)

randomtestcard1: randomtestcard1.c testHelpers.o dominion.o
	gcc -o randomtestcard1 -g randomtestcard1.c testHelpers.o dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c testHelpers.o dominion.o
	gcc -o randomtestcard2 -g randomtestcard2.c testHelpers.o dominion.o rngs.o $(CFLAGS)

randomtestresults: randomtestadventurer randomtestcard1 randomtestcard2
	./randomtestadventurer > randomtestadventurer.out
	gcov -bf ./dominion.c >> randomtestadventurer.out
	rm -f dominion.c.gcov
	./randomtestcard2 > randomtestcard2.out
	gcov -bf ./dominion.c >> randomtestcard2.out
	rm -f dominion.c.gcov
	./randomtestcard3 > randomtestcard3.out
	gcov -bf ./dominion.c >> randomtestcard3.out

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
