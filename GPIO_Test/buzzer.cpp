#include "buzzer.h"
#include "gpio.h"
#include <util/delay.h>

static uint8_t buzzer_port;
static uint8_t buzzer_pin;

void buzzer_init(uint8_t port, uint8_t pin)
{
    buzzer_port = port;
    buzzer_pin = pin;

    /* Configure Pin 23 (PA1) as OUTPUT */
    pin_config(buzzer_port, buzzer_pin, OUTPUT);
    pin_write(buzzer_port, buzzer_pin, LOW);
}

void buzzer_update_pattern(uint16_t distance_cm)
{
    if (distance_cm > 50)
    {
        /* SAFE: Completely SILENT */
        pin_write(buzzer_port, buzzer_pin, LOW);
    }
    else if (distance_cm > 30)
    {
        /* CAUTION: Slow beep */
        pin_write(buzzer_port, buzzer_pin, HIGH);
        _delay_ms(100);
        pin_write(buzzer_port, buzzer_pin, LOW);
    }
    else if (distance_cm > 15)
    {
        /* WARNING: Fast beep */
        pin_write(buzzer_port, buzzer_pin, HIGH);
        _delay_ms(40);
        pin_write(buzzer_port, buzzer_pin, LOW);
    }
    else
    {
        /* STOP!! (1-15 cm or 0): Continuous SOLID tone */
        pin_write(buzzer_port, buzzer_pin, HIGH);
    }
}