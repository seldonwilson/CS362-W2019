/***********************************************************************************
* Filename: unittest4.c                                                            *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*  Created: 4 Feb 2019                                                             *
* Modified: 4 Feb 2019                                                             *
*                                                                                  *
* Description: Unit test for fullDeckCount() function in the dominion card game    *
*    program. This function takes a player and card number along with the game     *
*    state object and returns the number of that given card in total a player has  *
*    in their hand, discard pile and deck. This unit test will test the following: *
*                                                                                  *
*    (1) No property of the gameState object is modified                           *
*    (2) Cards not in the current game return a count of 0                         *
*                                                                                  *
***********************************************************************************/
#include <stdio.h>            // fprintf()
#include <stdlib.h>           // exit()
#include <string.h>           // memcpy()
#include "dominion.h"         // enum CARD, fullDeckCount()
#include "testHelpers.h"      // gameState, ChangedStates, myAssert(), 
                              // isStateUnchanged()

   // Message buffers used by the testing functions
char msg[255];
char err[255];


   // Functions to test fullDeckCount()
static void _testProperties(const gameState*, const gameState*);
static void _testCardsNotInGame(const gameState*, int, int);
static void _testCardsInGame(const gameState*, int, int, int);

/***********************************************************************************
*************************** gainCard() Unit Test Driver ****************************
***********************************************************************************/
int main(void)
{
   int k[10] = { adventurer, council_room, feast,   gardens, mine,
                 remodel,    smithy,       village, baron,   great_hall };

   const int NUM_PLAYERS = 4;
   
   for (int player = 2; player <= NUM_PLAYERS; ++player) {
      for (int card = curse; card <= treasure_map; ++card) {
            // gameStates before and after gainCard() call
         gameState modified, original;
         if (initializeGame(NUM_PLAYERS, k, 100, &modified)) {
            fprintf(stdout, "ERROR: gameState could not be initialized.\n");
            exit(1);
         }

            // Copy modified into original to store pre-gameCard() state 
         memcpy(&original, &modified, sizeof (gameState));

            // Function in question
         int ret = fullDeckCount(player, card, &modified);

            // Test that there were no state changes after the call
         _testProperties(&original, &modified);
            // Test that cards not in the game return a count of 0
         _testCardsNotInGame(&original, card, ret);
            // Test that cards in the game return a count in allowable range
         _testCardsInGame(&original, player, card, ret);
      }
   }

   return 0;
}


static void _testProperties(const gameState* original, const gameState* question)
{
   ChangedStates mutableStates;
   memset(&mutableStates, 0, sizeof (ChangedStates));
   
      // Set msg for test
   sprintf(msg, "fullDeckCount() - No state changes occured!");

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);
   
      // Assert there were no state changes of any kind
   myAssert(isStateUnchanged(original, question, mutableStates), msg, err, false);
}


static void _testCardsNotInGame(const gameState* original, int card, int ret)
{
      // If the card is not in the game it should have returned a count of 0
   if (original->supplyCount[card] == -1) {
      sprintf(msg, "fullDeckCount() - cards not in game return count of 0!");
      sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 1);
      myAssert(ret == 0, msg, err, false);
   }
}


static void _testCardsInGame(const gameState* original, int p, int card, int ret)
{
   if (original->supplyCount[card] > -1) {
      int upperLimit;
      if (card == great_hall || card == gardens) {
         if (p == 2) {
            upperLimit = 8;
         }
         else {
            upperLimit = 12;
         }   
      }
      else {
         upperLimit = 10;
      }
      
      sprintf(msg, "fullDeckCount() - cards in game return possible count!", card);
      sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 1);
      myAssert(ret >= 0 && ret <= upperLimit, msg, err, false);
   }
}
