/***********************************************************************************
* Filename: unittest2.c                                                            *
*   Author: Daniel Wilson                                                          *
*   E-mail: wilsond3@oregonstate.edu                                               *
*   Course: CS 362                                                                 *
*  Created: 3 Feb 2019                                                             *
* Modified: 3 Feb 2019                                                             *
*                                                                                  *
* Description: Unit test for the getCost() function in the dominion card game pro- *
*    gram. This function takes an integer value representing a given card and      *
*    returns the cost to purchase that given card, as an int. This unit test simply*
*    establishes an array of the costs for each card, using the ordering provided  *
*    by enum CARD in dominion.h and used the costs from                            *
*    http://wiki.dominionstrategy.com to ensure the validity of the values. b/c    *
*    the getCost function doesn't gain access to a state object, nor does it rely  *
*    on player number, deck size, or basically anything other than the card number *
*    there was only a test made against the card number.                           *
*                                                                                  *
***********************************************************************************/
#include <stdio.h>            // sprintf() 
#include "dominion_helpers.h" // getCost()
#include "testHelpers.h"      // myassert()

   // Message buffers used by the testing functions
char msg[255];
char err[255];


/***********************************************************************************
**************************** getCost() Unit Test Driver ****************************
***********************************************************************************/
int main(void)
{
      // cardCosts[CARD] = cardCost (uses CARD enum in dominion.h for indicies)
   int cardCosts[] = {
      0, 2, 5, 8, 0, 3, 6, 6, 5, 4,
      4, 5, 4, 4, 3, 4, 3, 5, 3, 5,
      3, 4, 2, 5, 4, 4, 4
   };

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
      myAssert(actualCost == testCost, msg, err, false);
   }

   return 0;
}
