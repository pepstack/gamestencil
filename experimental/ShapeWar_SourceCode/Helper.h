#pragma once

/**
* Get the '1' bit in the bit array of this value, and return the index of this '1' bit.
* But remember, the value must only have one '1' bit , this method won't check for this,
* so you must make sure about that . And the reture value is from 0 to 63 .
* You do not need to specific the second and third parameters.
*/
unsigned int getbitone64(unsigned _int64 value, unsigned int number = 64, int index = 0) ;