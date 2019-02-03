#include <stdbool.h>     // bool, false, true
#include <string.h>      // memcpy(), memset()
#include <stdio.h>       // printf(), sprintf()
#include <stdlib.h>      // exit()
#include "dominion.h"    // struct gameState, shuffle()
#include "testHelpers.h" // isStateUnchanged(), myassert()

/* Tests that shuffle only modifies the shuffled player's deck. Tests this at all
   player counts [2, 4] and all shufflable possible deck sizes [0, MAX_DECK]

   If deck size is 0, the deck shouldn't shuffle and shuffle() should return !0




*/
int main(void) {
   fprintf(stdout, "shuffle() test\n");
   fprintf(stdout, "==============\n");
   fflush(stdout);

   int k[10] = {
      adventurer,
      council_room,
      feast,
      gardens,
      mine,
      remodel,
      smithy,
      village,
      baron,
      great_hall
   };

   const int NUM_PLAYERS = 4;
   char msgBuffer[256];
   char errBuffer[256];
   
      // Iterate over each player count
   for (int p = 1; p <= NUM_PLAYERS; ++p) {
      ChangedStates mutableStates;
      memset(&mutableStates, 0, sizeof (ChangedStates));
      
      char* playerState;
      switch (p) {
         case 1: playerState = &mutableStates.playerOne;   break;
         case 2: playerState = &mutableStates.playerTwo;   break;
         case 3: playerState = &mutableStates.playerThree; break;
         case 4: playerState = &mutableStates.playerFour;  break;
         default: printf("Invalid player #\n"); exit(1);
      }
      
      *playerState |= DECK;
      
         // Iterate over each possible deck size
      for (int d = 0; d <= MAX_DECK; ++d) {
         gameState modified, original;
         
            // original is a copy of modified. Both have deckCount adjusted to d
         if (initializeGame(NUM_PLAYERS, k, 100, &modified)) {
            printf("ERROR: gameState could not be initialized.\n");
            exit(1);
         }
         modified.deckCount[p - 1] = d;
         memcpy(&original, &modified, sizeof (gameState));

         shuffle(p - 1, &modified);
                  
         sprintf(
            msgBuffer,
            "player: %d | deck size: %3d - only player %d's deck changed!",
            p, d, p);
         sprintf(errBuffer, "@ %s, line %d", __FILE__, __LINE__ + 2);

         myAssert(
            isStateUnchanged(&original, &modified, mutableStates), 
            msgBuffer,
            errBuffer,
            false);
         // use sprintf to store the formatted message in a string!
         
         // DECK ACTUALLY SHUFFLED
         
         
         // DECK CONTAINS SAME ITEMS AS BEFORE
      }
   }


   return 0;
}
