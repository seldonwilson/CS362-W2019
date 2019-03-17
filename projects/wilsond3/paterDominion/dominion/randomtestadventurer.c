/***********************************************************************************
* Filename: randomcardtest1.c                                                      *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*                                                                                  *
* Description: Random test for the Adventurer card in the Dominion card game       *
*    program. The card should grant the player three additional cards. The unit    *
*    test will test for:                                                           *
*                                                                                  *
*    (1) The only changed state values are the given player's hand, deck, discard  *
*        their requisite counts and numActions.                                    *
*    (2) The number of cards in the player's hand should increase by one at most.  *
*        b/c the states of the other player's arrays could not have changed not can*
*        the supply pile change states, the cards must have come from the player's *
*        discard pile or deck.                                                     *
*                                                                                  *
***********************************************************************************/
#include <stdbool.h>     // bool, false, true
#include <string.h>      // memcpy(), memset()
#include <stdio.h>       // fprintf(), sprintf() 
#include <stdlib.h>      // exit(), qsort(), bsearch(), srand(), NULL
#include <time.h>        // time()
#include "dominion.h"    // struct gameState, shuffle(), MAX_DECK
#include "testHelpers.h" // isStateUnchanged(), myassert()

#define NUM_KINGDOM_CARDS 10
#define PK_SIZE           treasure_map - adventurer + 1
#define NUM_PLAYERS       4

   // Message buffers used by the testing functions
char msg[255];
char err[255];

   // Functions to test Great Hall card
static void _testPropertiesAdventurer(const gameState*, const gameState*, int);
static int _getTreasureCountDeckAndDiscard(const gameState*, int);


/***********************************************************************************
**************************** Adventurer Random Test Driver *************************
***********************************************************************************/
int main(void)
{
   srand(time(NULL));
   
   for (int i = 0; i != 10000; ++i) {
         // Randomly generate a the kingdom cards to play this game
      int pk[PK_SIZE];
      for (int i = 0; i != PK_SIZE; ++i) {
         pk[i] = i + adventurer;
      }
   
      int k[NUM_KINGDOM_CARDS];
      for (int i = 0; i != NUM_KINGDOM_CARDS; ++i) {
         int r = rand() % (PK_SIZE - i);
         k[i] = pk[r];
      
         int temp = pk[PK_SIZE - i - 1];
         pk[PK_SIZE - i - 1] = pk[r];
         pk[r] = temp;
      }
      
         // Randomly select a number of players [2, 4]
      int numPlayers = rand() % 3 + 2;
         // Randomly select a deck size for the current player
      int deckSize = rand() % (MAX_DECK + 1);
         // Randomly select a hand size for the current player
      int handSize = rand() % (MAX_HAND + 1);
         // Randomly select a discard size for the current player
      int discardSize = rand() % (MAX_DECK);
         // Randomly select the current player
      int currentPlayer = rand() % numPlayers;
      
         // Make sure to allow for deckSize to be zero
      if (rand() % MAX_DECK == 0) {
         deckSize = 0;
      }

      gameState modified, original;
      if (initializeGame(numPlayers, k, rand(), &modified)) {
         fprintf(stdout, "ERROR: gameState could not be initialized.\n");
         exit(1);
      }
      
         // Set sizes of each array count
      modified.deckCount[currentPlayer] = deckSize;
      modified.handCount[currentPlayer] = handSize;
      modified.discardCount[currentPlayer] = discardSize;
      
         // Set current player
      modified.whoseTurn = currentPlayer;

         // Assign each array of the current player a random card
      for (int i = 0; i != deckSize; ++i) {
         modified.deck[currentPlayer][i] = rand() % treasure_map;
      }
      for (int i = 0; i != handSize; ++i) {
         modified.hand[currentPlayer][i] = rand() % treasure_map;
      }
      for (int i = 0; i != discardSize; ++i) {
         modified.discard[currentPlayer][i] = rand() % treasure_map;
      }
      
         // Set last card in hand to be Adventurer, so we can play it...
      modified.hand[currentPlayer][handSize] = adventurer;
      
         // Copy modified into original to store pre-shuffle() state 
      memcpy(&original, &modified, sizeof (gameState));

         // Play Smithy
      int ret = playCard(handSize, 0, 0, 0, &modified);
      
      testCardPlaySuccessful(
         ret, adventurer, currentPlayer, deckSize, handSize, discardSize
      );

      if (ret == 0) {
            // Test only current player's state properties are modified
         _testPropertiesAdventurer(&original, &modified, currentPlayer + 1);

         int maxTreasure = _getTreasureCountDeckAndDiscard(&original, currentPlayer + 1);
         if (maxTreasure > 2) {
            maxTreasure = 2;
         }
            /* Test that player's hand size changes by number of treasures in
               their discard pile and deck minus one
            */
         testHandChange(&original, &modified, currentPlayer + 1, maxTreasure - 1);
      }
      
   }
      
   return 0;
}



static void _testPropertiesAdventurer(
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

static int _getTreasureCountDeckAndDiscard(const gameState* gs, int p) {
   int count = 0;
   
   for (int i = 0; i < gs->deckCount[p - 1]; ++i) {
      int card = gs->deck[p - 1][i];
      if (card == copper || card == silver || card == gold) {
         ++ count;
      }
   }
   
   for (int i = 0; i < gs->discardCount[p - 1]; ++i) {
      int card = gs->discard[p - 1][i];
      if (card == copper || card == silver || card == gold) {
         ++ count;
      }
   }
   
   return count;
}
