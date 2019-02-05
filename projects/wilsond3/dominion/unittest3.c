/***********************************************************************************
* Filename: unittest3.c                                                            *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*  Created: 3 Feb 2019                                                             *
* Modified: 4 Feb 2019                                                             *
*                                                                                  *
* Description: Unit test for the gainCard() function in the dominion card game     *
*    program. This function should allow a given player to gain a card from the    *
*    supply and either add it to their discard pile, deck or hand. This unit test  *
*    tests for:                                                                    *
*                                                                                  *
*    (1) Only allowable state properties are modified:                             *
*        - player's deck/discard/hand array and count                              *
*        - count of supplies in game                                               *
*    (2) Only next card in player's discard/deck/hand array is modified            *
*    (3) Count of deck/discard/hand go up by exactly one                           *
*    (4) Fail on card not in supply                                                *
*    (5) Count of given supply is decreased by one                                 *
*                                                                                  *
***********************************************************************************/
#include <stdio.h>            // fprintf()
#include <stdlib.h>           // exit()
#include <string.h>           // memcpy()
#include "dominion.h"         // enum CARD
#include "dominion_helpers.h" // gainCard()
#include "testHelpers.h"      // gameState, ChangedStates, myAssert(), 
                              // isStateUnchanged()

   // Message buffers used by the testing functions
char msg[255];
char err[255];

typedef gameState gs;

   // Functions to test gainCard()
static void _testProperties(const gs*, const gs*, int, int, int, int);
static void _testCardGainable(const gs*, int, int);
static void _testPlayerCounts(const gs*, const gs*, int, int, int);
static void _testSupplyCounts(const gs*, const gs*, int, int);
static void _testGainedCardPos(const gs*, const gs*, int, int, int, int);

/***********************************************************************************
*************************** gainCard() Unit Test Driver ****************************
***********************************************************************************/
int main(void)
{
   int k[10] = { adventurer, council_room, feast,   gardens, mine,
                 remodel,    smithy,       village, baron,   great_hall };

   const int NUM_PLAYERS = 4;
   
   for (int player = 1; player <= NUM_PLAYERS; ++player) {
      for (int card = curse; card <= treasure_map; ++card) {
         for (int toFlag = 0; toFlag <= 2; ++toFlag) {
            for (int count = 0; count <= 500; ++count) {
                  // gameStates before and after gainCard() call
               gameState modified, original;
               if (initializeGame(NUM_PLAYERS, k, 100, &modified)) {
                  fprintf(stdout, "ERROR: gameState could not be initialized.\n");
                  exit(1);
               }

               modified.deckCount[player - 1]    = count;
               modified.handCount[player - 1]    = count;
               modified.discardCount[player - 1] = count;
               
                  // Copy modified into original to store pre-gameCard() state 
               memcpy(&original, &modified, sizeof (gameState));
               int ret = gainCard(card, &modified, toFlag, player - 1);

                  // Test that only allowable properties mutated
               _testProperties(&original, &modified, player, toFlag, card, count);            
                  // Test gainCard() only works with card available in supply
               _testCardGainable(&original, card, ret);
                  // Test count of given player's array went up by exactly one
               _testPlayerCounts(&original, &modified, player, toFlag, ret);
                  // Test the supply count of card went down by one, if available
               _testSupplyCounts(&original, &modified, card, ret);
                  // Ensure gained card is placed at end of array and in bounds
               _testGainedCardPos(&original, &modified, player, toFlag, card, ret);
            }
         }
      }
   }

   return 0;
}


static void _testProperties(
   const gameState* original, 
   const gameState* question, 
   int player, 
   int toFlag,
   int card,
   int count)
{
   ChangedStates mutableStates;
   memset(&mutableStates, 0, sizeof (ChangedStates));

      // playerState should refer to the state of player being tested
   char* playerState;
   switch (player) {
      case 1: playerState = &mutableStates.playerOne;   break;
      case 2: playerState = &mutableStates.playerTwo;   break;
      case 3: playerState = &mutableStates.playerThree; break;
      case 4: playerState = &mutableStates.playerFour;  break;
   }

   switch (toFlag) {
      case 0: *playerState |= DISCARD; *playerState |= DISCARD_COUNT; break;
      case 1: *playerState |= DECK;    *playerState |= DECK_COUNT;    break;
      case 2: *playerState |= HAND;    *playerState |= HAND_COUNT;    break;
   }

   mutableStates.general |= SUPPLY_COUNT;

      // Set msg for test
   sprintf(
      msg,
      "player: %d | flag: %d | card: %2d | count: %3d "
      "- only allowable states changed!",
      player, toFlag, card, count
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);

      // Assert that only the deck will change states
   myAssert(isStateUnchanged(original, question, mutableStates), msg, err, false);
}


static void _testCardGainable(
   const gameState* original,
   int card,
   int ret)
{
      // Set msg for test
   sprintf(
      msg,
      "card: %2d | supply count: %2d - only available cards gained!",
      card, original->supplyCount[card]
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);
   
      // Assert a card was only gained if it was in the supply
   myAssert(
      (ret != 0 && original->supplyCount[card] <= 0) || 
      (ret == 0 && original->supplyCount[card] > 0),
      msg, err, false
   );
}


static void _testPlayerCounts(
   const gameState* original, 
   const gameState* question, 
   int player, 
   int toFlag,
   int ret)
{
   int p = player - 1;
   int originalSize;
   int questionSize;
   switch (toFlag) {
      case 0: originalSize = original->discardCount[p]; 
              questionSize = question->discardCount[p];
              break;
      
      case 1: originalSize = original->deckCount[p]; 
              questionSize = question->deckCount[p];
              break;
              
      case 2: originalSize = original->handCount[p]; 
              questionSize = question->handCount[p];
              break;
   }

      // Set msg for test
   sprintf(
      msg,
      "player: %d | toFlag: %d - if card in supply, count went up by exactly one!",
      player, toFlag
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);
   
      // Assert given player's appropriate array only increased by one
   myAssert(
      (ret == 0 && questionSize - originalSize == 1) || 
      (ret != 0 && questionSize == originalSize), 
      msg, err, false
   );
}


static void _testSupplyCounts(
   const gameState* original, 
   const gameState* question,
   int card,
   int ret)
{
      // Set msg for test
   sprintf(
      msg,
      "card: %d - if card was available, supply count went down by exactly one!",
      card
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);

   myAssert(
      (ret == 0 && (original->supplyCount[card] - question->supplyCount[card] == 1))
      || (ret != 0 && (original->supplyCount[card] == question->supplyCount[card])),
      msg, err, false
   );
}


static void _testGainedCardPos(
   const gameState* original, 
   const gameState* question, 
   int player, 
   int toFlag,
   int card,
   int ret)
{
   int p = player - 1;
   int originalSize;
   int questionSize;
   int maxSize;
   const int* arr;
   switch (toFlag) {
      case 0: originalSize = original->discardCount[p]; 
              questionSize = question->discardCount[p];
              maxSize = MAX_DECK;
              arr = question->discard[p];
              break;
      
      case 1: originalSize = original->deckCount[p]; 
              questionSize = question->deckCount[p];
              maxSize = MAX_DECK;
              arr = question->deck[p];
              break;
              
      case 2: originalSize = original->handCount[p]; 
              questionSize = question->handCount[p];
              maxSize = MAX_HAND;
              arr = question->hand[p];
              break;
   }

      // Set msg for test
   sprintf(
      msg,
      "card: %2d | ret: %d | index %3d - card added to next position in array!",
      card, ret, originalSize
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);
      
      // Assert the card gained went into the next position of the array
   myAssert((ret != 0) || (arr[originalSize] == card), msg, err, false);
   
      // Set msg for test
   sprintf(
      msg,
      "card: %2d | ret: %d | index %3d - card added to index in bounds!",
      card, ret, originalSize
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);
   
      // Assert the size of the array holding gained card is less than maximum
   myAssert((ret == 0 && questionSize <= maxSize) || ret != 0, msg, err, false);
}
