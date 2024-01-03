#include "BrailleToEnglish.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef STEPPER
#define STEPPER

// probably define all existing stepper motors here w/ their GPIO pins

extern void S_raiseStepper(int stepper_num);

extern void S_lowerStepper(int stepper_num);

#endif
