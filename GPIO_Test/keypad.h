#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>

/* Initialize the 4x4 matrix keypad port and pins */
void keypad_init(void);

/* Scan the matrix and return the character pressed, or '\0' if no key is pressed */
char keypad_get_key(void);

#endif