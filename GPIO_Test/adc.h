#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* Initialize the Analog-to-Digital Converter */
void adc_init(void);

/* Read the analog value from a specified channel (0 to 15) */
uint16_t adc_read(uint8_t channel);

#endif