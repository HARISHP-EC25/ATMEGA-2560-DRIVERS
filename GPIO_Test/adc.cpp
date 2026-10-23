#include "adc.h"
#include <avr/io.h>

void adc_init(void)
{
    /* 1. Select Reference Voltage:
     * REFS1 = 0, REFS0 = 1 -> AVCC with external capacitor on AREF pin (5V reference) */
    ADMUX = (1 << REFS0);

    /* 2. Configure Control and Status Register A (ADCSRA):
     * ADEN = 1 -> Enable ADC
     * ADPS2:0 = 111 -> Prescaler of 128 (16 MHz / 128 = 125 kHz safe ADC clock frequency) */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t channel)
{
    /* Ensure channel is restricted between 0 and 15 */
    channel &= 0x0F;

    /* Clear lower 5 bits (MUX4:0) in ADMUX and set the lower 3 bits (MUX2:0) */
    ADMUX = (ADMUX & 0xE0) | (channel & 0x07);

    /* Handle the 6th channel bit (MUX5) located in ADCSRB 
     * Required for channels 8 to 15 (such as PORTK / pins A8-A15) */
    if (channel & 0x08) {
        ADCSRB |= (1 << MUX5);
    } else {
        ADCSRB &= ~(1 << MUX5);
    }

    /* Start the conversion by setting the ADSC (ADC Start Conversion) bit */
    ADCSRA |= (1 << ADSC);

    /* Wait for conversion to finish (ADSC automatically clears when complete) */
    while (ADCSRA & (1 << ADSC));

    /* Return the combined 10-bit ADC result (ADCL and ADCH registers) */
    return ADC;
}