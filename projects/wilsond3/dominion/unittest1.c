#include <stdbool.h>    // bool, true, false
#include <stddef.h>     // size_t
#include <string.h>     // memcmp(), memcpy(), memset()
#include <stdio.h>      // printf()
#include <stdlib.h>     // malloc(), free(), NULL
#include "dominion.h"   // struct gameState



typedef struct gameState gameState;




/*******************************************************************************
* This struct holds the states that could potentially be modified by in-game   *
* functions. The values are stored as bit flags in positions shown below.      *
*                                                                              *
*      playerOne/Two/Three/Four |   generalOutpost  |        turn              *
*      -------------------------+-------------------+--------------------      *
*          hand...........1     | numPlayers......1 | whoseTurn.........1      *
*          handCount......2     | supplyCount.....2 | phase.............2      *
*          deck...........4     | embargoTokens...4 | numActions........4      *
*          deckCount......8     | outpostPlayed...8 | coins.............8      *
*          discard.......16     | outPostTurn....16 | numBuys..........16      *
*          discardCount..32     |                   | playedCards......32      *
*                               |                   | playedCardCount..64      *
*                                                                              *
* NOTE: This struct must be initialized with all it's fields set to 0.         *
*******************************************************************************/
typedef struct  {   
   char playerOne;
   char playerTwo;
   char playerThree;
   char playerFour;
   char turn;
   char general;
} ChangedStates;

typedef enum {
   HAND          =  1,
   HAND_COUNT    =  2,
   DECK          =  4,
   DECK_COUNT    =  8,
   DISCARD       = 16,
   DISCARD_COUNT = 32
} PlayerStateFlags;

typedef enum {
   NUM_PLAYERS    =  1,
   SUPPLY_COUNT   =  2,
   EMBARGO_TOKENS =  4,
   OUTPOST_PLAYED =  8,
   OUTPOST_TURN   = 16
} GeneralStateFlags;

typedef enum {
   WHOSE_TURN        =  1,
   PHASE             =  2,
   NUM_ACTIONS       =  4,
   COINS             =  8,
   NUM_BUYS          = 16,
   PLAYED_CARDS      = 32,
   PLAYED_CARD_COUNT = 64
} TurnStateFlags;



bool isStateUnchanged(
   const gameState*,
   const gameState*,
   ChangedStates);

gameState* copyStateWithChanges(
   const gameState*,
   const gameState*,
   ChangedStates);

void copyPlayerChanges(gameState*, const gameState*, ChangedStates, int);
void copyTurnChanges(gameState*, const gameState*, ChangedStates);
void copyGeneralChanges(gameState*, const gameState*, ChangedStates);


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
                
   int k2[10] = {remodel, minion, feast, gardens, sea_hag,
                cutpurse, smithy, village, baron, great_hall};
   
   
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



/*******************************************************************************
*    Function: isStateUnchanged                                                *
*                                                                              *
* Description: Determines if the original state's properties that were not     *
*              allowed to be changed didn't. original represents a gameState*  *
*              before a function call and question represents that state after *
*              the function call. changed store all allowable state changes    *
*              according to the rules of the function call in question. If     *
*              all states that were supposed to remain unchanged are in fact   *
*              unchanged, then this function will return true, false otherwise.*
*                                                                              *
*      Inputs: original - const gameState* (state before function to test)     *
*              question - const gameState* (state after function to test)      *
*              changed  - ChangedStates (states that are allowed to change)    *
*                                                                              *
*     Outputs: bool - did the function in question only modify the allowable   *
*                     state properties?                                        *
*                                                                              *
* NOTE: This function does not check if a state property that was allowed to be*
*       mutated, was mutated appropriately.                                    *
*******************************************************************************/
bool isStateUnchanged(
   const gameState* original,
   const gameState* question,
   ChangedStates    changed)
{   
      // Make a copy of original w/ allowed state changes from question
   gameState* mutated = copyStateWithChanges(original, question, changed);
   
      // Ensures only changes were state properties allowed to be changed
   bool isUnchanged = memcmp(mutated, question, sizeof (gameState)) == 0;
   
   free(mutated);
   return isUnchanged;
}


/*******************************************************************************
*    Function: copyStateWithChanges                                            *
*                                                                              *
* Description: Copies original gameState object into a dynamically allocated   *
*              gameState object mutated. Checks for all allowed changes and    *
*              copies those state properties from question to mutated. Returns *
*              mutated at the end.                                             *
*                                                                              *
*      Inputs: original - const gameState* (state before function to test)     *
*              question - const gameState* (state after function to test)      *
*              changed  - ChangedStates (states that are allowed to change)    *
*                                                                              *
*     Outputs: gameState* - copy of original with allowable state changes      *
*                           copied from question.                              *
*******************************************************************************/
gameState* copyStateWithChanges(
   const gameState* original,
   const gameState* question,
   ChangedStates    changed)
{
   gameState* mutated = malloc(sizeof (gameState));
   memcpy(mutated, original, sizeof (gameState));
   
   copyPlayerChanges(mutated, question, changed, 1);
   copyPlayerChanges(mutated, question, changed, 2);
   copyPlayerChanges(mutated, question, changed, 3);
   copyPlayerChanges(mutated, question, changed, 4);
   copyTurnChanges(mutated, question, changed);
   copyGeneralChanges(mutated, question, changed);
   
   return mutated;
}


/*******************************************************************************
*    Function: copyPlayerChanges                                               *
*                                                                              *
* Description: Takes all player state properties which are allowed to change   *
*              and copies them from question into mutated. State properties    *
*              which should not change are left unmodified.                    *
*                                                                              *
*      Inputs: mutated   - const gameState* (original state copy w/ changes)   *
*              question  - const gameState* (state after function to test)     *
*              changed   - ChangedStates (states that are allowed to change)   *
*              playerNum - int (the number of the player - 1 is player 1)      *
*                                                                              *
*     Outputs: void                                                            *
*******************************************************************************/
void copyPlayerChanges(
   gameState*       mutated,
   const gameState* question,
   ChangedStates    changed, 
   int              playerNum)
{
   char playerState;
   switch (playerNum) {
      case 1: playerState = changed.playerOne;   break;
      case 2: playerState = changed.playerTwo;   break;
      case 3: playerState = changed.playerThree; break;
      case 4: playerState = changed.playerFour;  break;
      default: playerState = '\0';
   }

   const int pi = playerNum - 1;
   const size_t deckSize = sizeof (int) * MAX_DECK; 
   
   if (playerState & HAND)
      memcpy(mutated->hand[pi], question->hand[pi], deckSize);
   if (playerState & HAND_COUNT)
      mutated->handCount[pi] = question->handCount[pi];
   if (playerState & DECK)
      memcpy(mutated->deck[pi], question->deck[pi], deckSize);
   if (playerState & DECK_COUNT)
      mutated->deckCount[pi] = question->deckCount[pi];
   if (playerState & DISCARD)
      memcpy(mutated->discard[pi], question->discard[pi], deckSize);
   if (playerState & DISCARD_COUNT)
      mutated->discardCount[pi] = question->discardCount[pi];
}


/*******************************************************************************
*    Function: copyTurnChanges                                                 *
*                                                                              *
* Description: Takes all turn state properties which are allowed to be changed *
*              and copies them from question into mutated. State properties    *
*              which should not change are left unmodified.                    *
*                                                                              *
*      Inputs: mutated  - const gameState* (original state copy w/ changes)    *
*              question - const gameState* (state after function to test)      *
*              changed  - ChangedStates (states that are allowed to change)    *
*                                                                              *
*     Outputs: void                                                            *
*******************************************************************************/
void copyTurnChanges(
   gameState*       mutated,
   const gameState* question,
   ChangedStates changed)
{
   if (changed.turn & WHOSE_TURN)
      mutated->whoseTurn = question->whoseTurn;
   if (changed.turn & PHASE)
      mutated->phase = question->phase;
   if (changed.turn & NUM_ACTIONS)
      mutated->numActions = question->numActions;
   if (changed.turn & COINS)
      mutated->coins = question->coins;
   if (changed.turn & NUM_BUYS)
      mutated->numBuys = question->numBuys;
   if (changed.turn & PLAYED_CARDS)
      memcpy(mutated->playedCards, question->playedCards, sizeof (int) * MAX_DECK);
   if (changed.turn & PLAYED_CARD_COUNT)
      mutated->playedCardCount = question->playedCardCount;
}


/*******************************************************************************
*    Function: copyGeneralChanges                                              *
*                                                                              *
* Description: Takes all general state properties which are allowed to be      *
*              changed and copies them from question into mutated. State       *
*              properties which should not change are left unmodified.         *
*                                                                              *
*      Inputs: mutated  - const gameState* (original state copy w/ changes)    *
*              question - const gameState* (state after function to test)      *
*              changed  - ChangedStates (states that are allowed to change)    *
*                                                                              *
*     Outputs: void                                                            *
*******************************************************************************/
void copyGeneralChanges(
   gameState*       mutated,
   const gameState* question,
   ChangedStates changed)
{
   const size_t supplySize = sizeof (int) * (treasure_map + 1);

   if (changed.general & NUM_PLAYERS)
      mutated->numPlayers = question->numPlayers;
   if (changed.general & SUPPLY_COUNT)
      memcpy(mutated->supplyCount, question->supplyCount, supplySize);
   if (changed.general & EMBARGO_TOKENS)
      memcpy(mutated->embargoTokens, question->embargoTokens, supplySize);
   if (changed.general & OUTPOST_PLAYED)
      mutated->outpostPlayed = question->outpostPlayed;
   if (changed.general & OUTPOST_TURN)
      mutated->outpostTurn = question->outpostTurn;
}
