/***********************************************************************************
* Filename: testHelpers.c                                                          *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*  Created: 2 Feb 2019                                                             *
* Modified: 2 Feb 2019                                                             *
*                                                                                  *
* Description: Contains the interfaces for functions which are used in more than   *
*    one unit test for the dominion card game program.                             *
*                                                                                  *
***********************************************************************************/
#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <stdbool.h>  // bool, true, false
#include "dominion.h" // struct gameState

#define PRINT_PASSED false

typedef struct gameState gameState;

/***********************************************************************************
* This struct holds the states that could potentially be modified by in-game func- *
* tions. The values are stored as bit flags in positions shown below.              *
*                                                                                  *
*        playerOne/Two/Three/Four |   generalOutpost  |        turn                *
*        -------------------------+-------------------+--------------------        *
*            hand...........1     | numPlayers......1 | whoseTurn.........1        *
*            handCount......2     | supplyCount.....2 | phase.............2        *
*            deck...........4     | embargoTokens...4 | numActions........4        *
*            deckCount......8     | outpostPlayed...8 | coins.............8        *
*            discard.......16     | outPostTurn....16 | numBuys..........16        *
*            discardCount..32     |                   | playedCards......32        *
*                                 |                   | playedCardCount..64        *
*                                                                                  *
* NOTE: This struct must be initialized with all it's fields set to 0.             *
***********************************************************************************/
typedef struct  {   
   char playerOne;
   char playerTwo;
   char playerThree;
   char playerFour;
   char turn;
   char general;
} ChangedStates;


   //  Bit masks to detect mutable states regarding player state properties
typedef enum {
   HAND          =  1,
   HAND_COUNT    =  2,
   DECK          =  4,
   DECK_COUNT    =  8,
   DISCARD       = 16,
   DISCARD_COUNT = 32
} PlayerStateFlags;


   //  Bit masks to detect mutable states regarding general state properties
typedef enum {
   NUM_PLAYERS    =  1,
   SUPPLY_COUNT   =  2,
   EMBARGO_TOKENS =  4,
   OUTPOST_PLAYED =  8,
   OUTPOST_TURN   = 16
} GeneralStateFlags;


   //  Bit masks to detect mutable states regarding turn state properties
typedef enum {
   WHOSE_TURN        =  1,
   PHASE             =  2,
   NUM_ACTIONS       =  4,
   COINS             =  8,
   NUM_BUYS          = 16,
   PLAYED_CARDS      = 32,
   PLAYED_CARD_COUNT = 64
} TurnStateFlags;


   // Determines if only allowable state changes were made
bool isStateUnchanged(const gameState*, const gameState*, ChangedStates);
   // Copy all mutable state properties to first argument from second
gameState* copyStateWithChanges(const gameState*, const gameState*, ChangedStates);
   // Copy mutable player state properties to first argument from second
void copyPlayerChanges(gameState*, const gameState*, ChangedStates, int);
   // Copy mutable turn state properties to first argument from second
void copyTurnChanges(gameState*, const gameState*, ChangedStates);
   // Copy mutable general state properties to first argument from second
void copyGeneralChanges(gameState*, const gameState*, ChangedStates);
   // Prints PASSED/FAILED for a given boolean expression, can enable quit on error
void myAssert(bool, const char*, const char*, bool);
   // Tests if the player's handCount increased by the given amount
void testHandChange(const gameState*, const gameState*, int, int);
   // Tests if the numActions increased by a given amount
void testActionsChange(const gameState*, const gameState*, int, int); 
   // Tests that the return value of a played card indicates success
void testCardPlaySuccessful(int, int, int, int, int, int);
   // Unused: tests is card is in last position of discard pile
void testLastInDiscard(const gameState*, int, int);
// Changes made without compiler (proceed w/ caution)


#endif // TEST_HELPERS_H
