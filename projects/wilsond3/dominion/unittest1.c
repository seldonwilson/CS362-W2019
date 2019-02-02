#include <string.h>      // memcpy(), memset()
#include <stdio.h>       // printf()
#include "dominion.h"    // struct gameState
#include "testHelpers.h" // isStateUnchanged()


int main(void) {
   ChangedStates changedStates1, changedStates2;
   memset(&changedStates1, 0, sizeof (ChangedStates));
   memset(&changedStates2, 0, sizeof (ChangedStates));
   
   changedStates1.general   |= OUTPOST_PLAYED;
   changedStates1.general   |= SUPPLY_COUNT;
   changedStates1.general   |= EMBARGO_TOKENS;
   changedStates1.turn      |= PLAYED_CARDS;
   changedStates1.playerOne |= DECK;
    
   int k1[10] = {adventurer, council_room, feast, gardens, mine,
                remodel, smithy, village, baron, great_hall};
   
   gameState original;
   if (initializeGame(4, k1, 100, &original))
      printf("Error: game1 could not be initialized.\n");
   original.outpostPlayed = 15;
   
   gameState question;
   memcpy(&question, &original, sizeof (gameState));
   question.outpostPlayed = 69;
   question.supplyCount[remodel] = 1000;
   question.outpostPlayed = -51;
   question.playedCards[1] = ambassador;
   question.deck[0][0] = treasure_map;

      // States should be accepted because modification of player1's deck
      //allowed
   if (isStateUnchanged(&original, &question, changedStates1))
      printf("States match! Yay : )\n");
   else
      printf("ERROR: Same states don't match\n");
   

   question.deck[1][0] = salvager;
   if (isStateUnchanged(&original, &question, changedStates1))
      printf("States match!\n");
   else
      printf("States don't match\n");

   return 0;
}
