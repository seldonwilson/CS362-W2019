/***********************************************************************************
* Filename: testHelpers.c                                                          *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*  Created: 2 Feb 2019                                                             *
* Modified: 2 Feb 2019                                                             *
*                                                                                  *
* Description: Contains definitions for functions which are used in more than one  *
*    unit test for the dominion card game program. See testHelpers.h interface.    *
*                                                                                  *
***********************************************************************************/
#include <stdbool.h>     // bool, true, false
#include <stddef.h>      // size_t
#include <stdio.h>       // printf()
#include <stdlib.h>      // malloc(), free()
#include <string.h>      // memcmp(), memcpy()
#include "testHelpers.h" // prototypes


/***********************************************************************************
*    Function: isStateUnchanged                                                    *
*                                                                                  *
* Description: Determines if original's properties that were not allowed to change *
*              didn't. If all properties that were supposed to stay unchanged are  *
*              unchanged, then this function will return true, false otherwise.    *
*                                                                                  *
*      Inputs: original - const gameState* (state before function to test)         *
*              question - const gameState* (state after function to test)          *
*              changed  - ChangedStates (states that are allowed to change)        *
*                                                                                  *
*     Outputs: bool - were only mutable state properties mutated?                  *
***********************************************************************************/
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


/***********************************************************************************
*    Function: copyStateWithChanges                                                *
*                                                                                  *
* Description: Copies original gameState object into a dynamically allocated game- *
*              State object mutated. Checks for all allowed changes and copies     *
*              those state properties from question to mutated. Returns mutated at *
*              the end.                                                            *
*                                                                                  *
*      Inputs: original - const gameState* (state before function to test)         *
*              question - const gameState* (state after function to test)          *
*              changed  - ChangedStates (states that are allowed to change)        *
*                                                                                  *
*     Outputs: gameState* - copy of original with allowable state changes          *
*                           copied from question.                                  *
***********************************************************************************/
gameState* copyStateWithChanges(
   const gameState* original,
   const gameState* question,
   ChangedStates    changed)
{
      // mutated is a copy of original
   gameState* mutated = malloc(sizeof (gameState));
   memcpy(mutated, original, sizeof (gameState));
   
      // copy allowable changes from question into mutated
   copyPlayerChanges(mutated, question, changed, 1);
   copyPlayerChanges(mutated, question, changed, 2);
   copyPlayerChanges(mutated, question, changed, 3);
   copyPlayerChanges(mutated, question, changed, 4);
   copyTurnChanges(mutated, question, changed);
   copyGeneralChanges(mutated, question, changed);
   
   return mutated;
}


/***********************************************************************************
*    Function: copyPlayerChanges                                                   *
*                                                                                  *
* Description: Takes all player state properties which are allowed to change and   *
*              copies them from question into mutated. State properties which      *
*              should not change are left unmodified.                              *
*                                                                                  *
*      Inputs: mutated   - const gameState* (original state copy w/ changes)       *
*              question  - const gameState* (state after function to test)         *
*              changed   - ChangedStates (states that are allowed to change)       *
*              playerNum - int (the number of the player - 1 is player 1)          *
*                                                                                  *
*     Outputs: void                                                                *
***********************************************************************************/
void copyPlayerChanges(
   gameState*       mutated,
   const gameState* question,
   ChangedStates    changed, 
   int              playerNum)
{
      // Get mutable state masks for playerNum
   char playerState;
   switch (playerNum) {
      case 1: playerState = changed.playerOne;   break;
      case 2: playerState = changed.playerTwo;   break;
      case 3: playerState = changed.playerThree; break;
      case 4: playerState = changed.playerFour;  break;
      default: playerState = '\0';
   }

      // index of playerNum
   const int pi = playerNum - 1;
   const size_t deckSize = sizeof (int) * MAX_DECK; 
   
      // If a change is allowed, copy that property from question into mutated
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


/***********************************************************************************
*    Function: copyTurnChanges                                                     *
*                                                                                  *
* Description: Takes all turn state properties which are allowed to be changed and *
*              copies them from question into mutated. State properties which      *
*              should not change are left unmodified.                              *
*                                                                                  *
*      Inputs: mutated  - const gameState* (original state copy w/ changes)        *
*              question - const gameState* (state after function to test)          *
*              changed  - ChangedStates (states that are allowed to change)        *
*                                                                                  *
*     Outputs: void                                                                *
***********************************************************************************/
void copyTurnChanges(
   gameState*       mutated,
   const gameState* question,
   ChangedStates changed)
{
   
      // If a change is allowed, copy that property from question into mutated
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


/***********************************************************************************
*    Function: copyGeneralChanges                                                  *
*                                                                                  *
* Description: Takes all general state properties which are allowed to be changed  *
*              and copies them from question into mutated. State properties which  *
*              should not change are left unmodified.                              *
*                                                                                  *
*      Inputs: mutated  - const gameState* (original state copy w/ changes)        *
*              question - const gameState* (state after function to test)          *
*              changed  - ChangedStates (states that are allowed to change)        *
*                                                                                  *
*     Outputs: void                                                                *
***********************************************************************************/
void copyGeneralChanges(
   gameState*       mutated,
   const gameState* question,
   ChangedStates changed)
{
   const size_t supplySize = sizeof (int) * (treasure_map + 1);

      // If a change is allowed, copy that property from question into mutated
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




void myAssert(bool expr, const char* msg, const char* err, bool quitOnError) {
   if (!expr) {
      fprintf(stderr, "FAILED: %s %s\n", msg, err);
      
      if (quitOnError) {
         printf("Quitting...\n");
         exit(1);
      }
   }
   else if (PRINT_PASSED)
      printf("PASSED: %s\n", msg);
}
