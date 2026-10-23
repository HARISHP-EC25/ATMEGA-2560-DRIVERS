#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

void buzzer_init(uint8_t port, uint8_t pin);
void buzzer_update_pattern(uint16_t distance_cm);

#endif