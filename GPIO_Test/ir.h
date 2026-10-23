#ifndef IR_H
#define IR_H

#include <stdint.h>

/* Initialize two IR sensor pins on the specified port */
void ir_init_dual(uint8_t port, uint8_t pin1, uint8_t pin2);

/* Returns the total number of free slots (0, 1, or 2) */
uint8_t ir_get_total_free(void);

#endif