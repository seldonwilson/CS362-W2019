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
*    (2) Actually
*                                                                                  *
***********************************************************************************/
#include <stdbool.h>     // bool, false, true
#include <string.h>      // memcpy(), memset()
#include <stdio.h>       // sprintf() 
#include "dominion.h"    // getCost()
#include "testHelpers.h" // myassert()

   // Message buffers used by the testing functions
char msg[255];
char err[255];


/***********************************************************************************
**************************** getCost() Unit Test Driver ****************************
***********************************************************************************/
int main(void) {
      // cardCosts[CARD] = cardCost (uses CARD enum in dominion.h for indicies)
   int cardCosts = [
      0, 2, 5, 8, 0, 3, 6, 6, 5, 4,
      4, 5, 4, 4, 3, 4, 3, 5, 3, 5,
      3, 4, 2, 5, 4, 4, 4
   ];

   for (int i = 0; i <= treasure_map; ++i) {
         // Values to test against
      int testCost = getCost(i);
      int actualCost = cardCosts[i];
      
         // Set messages for myAssert()
      sprintf(
         msg,
         "CARD: %2d | Actual Cost: %d | getCost(): %d - Costs Match!",
         i, actualCost, testCost
      );
      sprintf(err, "@ %s, line %d", __FILE__, __LINE__ + 3);

            // Assert actual card cost matches the return value of getCost()
      myAssert(acutalCost == testCost, msg, err, false);
   }

   return 0;
}
