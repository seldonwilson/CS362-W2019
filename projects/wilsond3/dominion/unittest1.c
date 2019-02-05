/***********************************************************************************
* Filename: unittest1.c                                                            *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*  Created: 2 Feb 2019                                                             *
* Modified: 3 Feb 2019                                                             *
*                                                                                  *
* Description: Unit test for the shuffle() function in the dominion card game pro- *
*    gram. The test iterates over all player counts and all possible deck sizes w/ *
*    a gameState initialized using the same random seed and same kingdom cards.    *
*    W/in these constraints the shuffle() function is tested to ensure it:         *
*                                                                                  *
*    (1) Only modifies the state of the given player's deck                        *
*    (2) Deck contents remain the same before and after shuffle()                  *
*    (3) Deck actually is in a different order after shuffle()                     *
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

   // Functions to test shuffle()
static void _testOnlyDeckModified(const gameState*, const gameState*, int, int);
static void _testSameDeckContents(const gameState*, const gameState*, int, int);
static void _testDeckShuffled(const gameState*, gameState*, int, int);

   // Comparator to sort cards (taken from dominion.c)
static int _compare(const void*, const void*);


/***********************************************************************************
**************************** shuffle() Unit Test Driver ****************************
***********************************************************************************/
int main(void) {
   int k[10] = { adventurer, council_room, feast,   gardens, mine,
                 remodel,    smithy,       village, baron,   great_hall };

   const int NUM_PLAYERS = 4;
   
      // Iterate over each player count
   for (int p = 1; p <= NUM_PLAYERS; ++p) {      
         // Iterate over each possible deck size
      for (int d = 0; d <= MAX_DECK; ++d) {
         
            // gameStates before and after the shuffle() call
         gameState modified, original;
         if (initializeGame(NUM_PLAYERS, k, 100, &modified)) {
            fprintf(stdout, "ERROR: gameState could not be initialized.\n");
            exit(1);
         }
         
            // We want to test shuffle for a given deckCount d 
         modified.deckCount[p - 1] = d;
         
            // Copy modified into original to store pre-shuffle() state 
         memcpy(&original, &modified, sizeof (gameState));

            // Function we are testing
         shuffle(p - 1, &modified);
                  
            // Test that only player p's deck is modified
         _testOnlyDeckModified(&original, &modified, p, d);
            // Test that cards in deck have not changed
         _testSameDeckContents(&original, &modified, p, d);
            // Test if deck is in different order (modified may mutate)
         _testDeckShuffled(&original, &modified, p, d);
      }
   }

   return 0;
}


/***********************************************************************************
*    Function: _testOnlyDeckModified                                               *
*                                                                                  *
* Description: Determines if the only change between the original and question     *
*              state's is the deck on the given player.                            *
*                                                                                  *
*      Inputs: original - const gameState* (state before function to test)         *
*              question - const gameState* (state after function to test)          *
*              player   - int (player whose deck is allowed to change)             *
*              deckSize - int (size of given player's deck – only used for err)    *
*                                                                                  *
*     Outputs: void                                                                *
***********************************************************************************/
static void _testOnlyDeckModified(
   const gameState* original, 
   const gameState* question,
   int player,
   int deckSize)
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
   
      // The current player's deck may be modified in the shuffle() call
   *playerState |= DECK;

      // Set msg for test
   sprintf(
      msg,
      "player: %d | deck size: %3d - only player %d's deck changed!",
      player, deckSize, player
   );

      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);

      // Assert that only the deck will change states
   myAssert(isStateUnchanged(original, question, mutableStates), msg, err, false);
}


/***********************************************************************************
*    Function: _testSameDeckContents                                               *
*                                                                                  *
* Description: Determines if shuffle altered the contents of the deck.             *
*                                                                                  *
*      Inputs: original - const gameState* (state before function to test)         *
*              question - const gameState* (state after function to test)          *
*              player   - int (player whose deck is allowed to change)             *
*              deckSize - int (size of given player's deck – only used for err)    *
*                                                                                  *
*     Outputs: void                                                                *
***********************************************************************************/
static void _testSameDeckContents(
   const gameState* original, 
   const gameState* question,
   int player,
   int deckSize)
{
      // Make sorted copies of original and question deck
   int originalDeck[deckSize];
   int questionDeck[deckSize];
   memcpy(originalDeck, original->deck[player - 1], sizeof (int) * deckSize);
   memcpy(questionDeck, question->deck[player - 1], sizeof (int) * deckSize);
   qsort((void*) originalDeck, deckSize, sizeof (int), _compare);
   qsort((void*) questionDeck, deckSize, sizeof (int), _compare);
   
      // Check for sameness of contents between decks
   bool sameContents = true;
   for (int i = 0; i < deckSize && sameContents; ++i) {
      if (originalDeck[i] != questionDeck[i]) {
         sameContents = false;
      }
   }
   
      // Set messages for assertion test
   sprintf(
      msg,
      "player: %d | deck size: %3d - none of player %d's cards lost after shuffle!",
      player, deckSize, player
   );
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);
   
      // Assert the deck's contents didn't change after a shuffle
   myAssert(sameContents, msg, err, false);  
}


/***********************************************************************************
*    Function: _testDeckShuffled                                                   *
*                                                                                  *
* Description: Determines if the original and question deck are in a different or- *
*              der. Only checks order on deckSize ≥ 2 and decks with more than one *
*              card type. If decks are in same order, question state's deck for    *
*              given player is shuffled again.                                     *
*                                                                                  *
*      Inputs: original - const gameState* (state before function to test)         *
*              question - gameState* (state after function to test, may mutate)    *
*              player   - int (player whose deck is allowed to change)             *
*              deckSize - int (size of given player's deck – only used for err)    *
*                                                                                  *
*     Outputs: void                                                                *
*                                                                                  *
*        Note: Just checks for different order (agnostic about distribution) and   *
*              should be the last call of the current test run because question is *
*              potentially modified when deck contains different cards (common).   *
***********************************************************************************/
static void _testDeckShuffled(
   const gameState* original, 
   gameState* question,
   int player,
   int deckSize)
{
   bool isShuffled = true;

      // Deck sizes of 0 or 1 are trivially shuffled
   if (deckSize <= 1) { 
         // Make copies of original and question deck
      int originalDeck[deckSize];
      int questionDeck[deckSize];
      memcpy(originalDeck, original->deck[player - 1], sizeof (int) * deckSize);
      memcpy(questionDeck, question->deck[player - 1], sizeof (int) * deckSize);

         // Check if the deck is all the same card (if so, trvially sorted)
      bool isDeckAllSameCard = true;
      for (int i = 0; i < deckSize && isDeckAllSameCard; ++i) {
         if (originalDeck[i] != questionDeck[i]) {
            isDeckAllSameCard = false;
         }
      }

         // If the deck contains different cards, check if it's actually shuffled
      if (!isDeckAllSameCard) {
         isShuffled = false;
         
            // Test for different deck order up to 1,000,000 times
         for (int i = 0; i < 1000000 && !isShuffled; ++i) {
            for (int j = 0; j < original->deckCount[player] && !isShuffled; ++j) {
               if (original->deck[player][j] != question->deck[player][j]) {
                  isShuffled = true;
               }
            }

               // Deck order the same (try again)
            shuffle(player - 1, question);
         }
      }
   }
   
      // Set messages for assertion test
   sprintf(
      msg,
      "player: %d | deck size: %3d - player %d's deck shuffled!",
      player, deckSize, player
   );
      // Set err message for test (line should refer to where myAssert() is called)
   sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);
   
      // Assert the deck is shuffled
   myAssert(isShuffled, msg, err, false);
}  


   // Taken from dominion.c to sort cards
static int _compare(const void* a, const void* b) {
  if (*(int*)a > *(int*)b)
    return 1;
  if (*(int*)a < *(int*)b)
    return -1;
  return 0;
}
