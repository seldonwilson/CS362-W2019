/***********************************************************************************
* Filename: cardtest1.c                                                            *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*  Created: 4 Feb 2019                                                             *
* Modified: 4 Feb 2019                                                             *
*                                                                                  *
* Description: Unit test for the Great Hall card in the Dominion card game program.*
*    The card should grant the player an additional action and allow them to draw a*
*    card. The unit test will test for:                                            *
*                                                                                  *
*    (1) The only changed state values are the given player's hand, deck, discard  *
*        their requisite counts and numActions.                                    *
*    (2) The number of cards in the player's hand should be the same b/c they dis- *
*        card the card after playing it and draw one (net gain of zero).           *
*    (3) The number of actions should increase by same after card is played.       *
*    (4) The Great Hall should be in the last valid position of discard pile.      *
*                                                                                  *
***********************************************************************************/
#include <stdbool.h>     // bool, false, true
#include <string.h>      // memcpy(), memset()
#include <stdio.h>       // fprintf(), sprintf() 
#include <stdlib.h>      // exit(), qsort(), bsearch()
#include "dominion.h"    // struct gameState, shuffle(), MAX_DECK
#include "testHelpers.h" // isStateUnchanged(), myassert()

   // Message buffers used by the testing functions
char msg[255];
char err[255];

   // Functions to test Great Hall card
static void _testPropertiesGreatHall(const gameState*, const gameState*, int);


/***********************************************************************************
**************************** Great Hall Unit Test Driver ***************************
***********************************************************************************/
int main(void)
{
   int k[10] = { adventurer, council_room, feast,   gardens, mine,
                 remodel,    smithy,       village, baron,   great_hall };

   const int NUM_PLAYERS = 4;
   
      // Iterate over each player count
   for (int player = 2; player <= NUM_PLAYERS; ++player) {      
         // Iterate over each possible deck, hand and discard size
      for (int deckSize = 1; deckSize <= 10; ++deckSize)
      for (int handSize = 1; handSize <= 10; ++handSize)
      for (int discardSize = 1; discardSize <= 10; ++discardSize) {
         
            // gameStates before and after the shuffle() call
         gameState modified, original;
         if (initializeGame(NUM_PLAYERS, k, 100, &modified)) {
            fprintf(stdout, "ERROR: gameState could not be initialized.\n");
            exit(1);
         }
         
            // Set sizes of each array count
         modified.deckCount[player - 1] = deckSize;
         modified.handCount[player - 1] = handSize;
         modified.discardCount[player - 1] = discardSize;
            // Set last card in hand to be Great Hall
         modified.hand[player - 1][handSize] = great_hall;
            // Set current player
         modified.whoseTurn = player - 1;
         
            // Copy modified into original to store pre-shuffle() state 
         memcpy(&original, &modified, sizeof (gameState));

            // Play Great Hall
         int ret = playCard(handSize, 0, 0, 0, &modified);

         if (ret == 0) {
               // Test only current player's state properties are modified
            _testPropertiesGreatHall(&original, &modified, player);
               // Test number of player's cards in hand should be same
            testHandChange(&original, &modified, player, 0);
               // Test number of actions increased by one
            testActionsChange(&original, &modified, player, 1); 
               // Test that Great Hall is in the last position of discard pile
            testLastInDiscard(&modified, great_hall, player, card); 
         }
      }
   }

   return 0;
}



static void _testPropertiesGreatHall(
   const gameState* original, 
   const gameState* question,
   int player)
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
   
      // The current player's arrays may be modified by the Great Hall card
   *playerState |= DECK;
   *playerState |= DECK_COUNT;
   *playerState |= HAND;
   *playerState |= HAND_COUNT;
   *playerState |= DISCARD;
   *playerState |= DISCARD_COUNT;
   
      // These properties will be modified by Great Hall
   mutableStates.turn |= NUM_ACTIONS;
   mutableStates.turn |= PLAYED_CARDS;
   mutableStates.turn |= PLAYED_CARD_COUNT;
   mutableStates.turn |= COINS;
   
      // Set msg for test
   sprintf(
      msg,
      "player: %d - only player %d's arrays changed!",
      player, player
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);

      // Assert that only the deck will change states
   myAssert(isStateUnchanged(original, question, mutableStates), msg, err, false);
}
