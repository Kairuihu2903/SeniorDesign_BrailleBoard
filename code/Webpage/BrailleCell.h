#include "BrailleToEnglish.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BRAILLE_CELL
#define BRAILLE_CELL

// an array of 'containers' that hold the stepper motors so i can iter thru and accuate the dots of each cell
// idk what type to use though yet
extern char *BrailleCellArray[][]; 

// ex: {"100000","110000"} represents a device w/ two cells set to 'A' and 'B'
// might not be needed but this is a way of solving the problem of double raising/lowering dots
extern char *BrailleCellStates[]; 

extern void BC_resetCell(int cell_arr_num); // recesses all dots of a cell, may not be needed?

extern void BC_setCell(int cell_arr_num, char character)
{
    // convert character into b_code
    char *b_code = E2B_findValueByKey(character);

    // iter thru the list of stepper motors of a given cell
    for (int i = 0; i < 6; i++)
    {
       
        // int stepper = BrailleCellArray[cell_arr_num][i]; // idk the type of the stepper motor, say int for now

        if (b_code[i] == '0')
        {
            // accuate stepper down ... will need to make another file called 
        }
        else // b_code[i] == '1'
        {
            // accuate stepper up
        }
        
    }

    // given a from the user, I would iter thru the string calling BC_setCell() to represent the braille char on the board

    // each cell has 6 stepper motors & each motor has 4 lines

    // so i would iter thru the list of stepper motors contained in a cell and accuate them accordingly

    // and each stepper motors has 4 lines (gpio pins)
}

#endif
