#include "ir.h"
#include "gpio.h"

static uint8_t ir_port_num;
static uint8_t pin_s1;
static uint8_t pin_s2;

void ir_init_dual(uint8_t port, uint8_t pin1, uint8_t pin2)
{
    ir_port_num = port;
    pin_s1 = pin1;
    pin_s2 = pin2;

    /* Configure both IR pins as INPUTs */
    pin_config(ir_port_num, pin_s1, INPUT);
    pin_config(ir_port_num, pin_s2, INPUT);
}

uint8_t ir_get_total_free(void)
{
    /* Read pins. 
     * Note: If your IR sensor outputs LOW when an object is present and HIGH when free, 
     * use (pin_read(...) == HIGH). If inverted, change to == LOW. */
    uint8_t slot1_free = (pin_read(ir_port_num, pin_s1) == HIGH) ? 1 : 0;
    uint8_t slot2_free = (pin_read(ir_port_num, pin_s2) == HIGH) ? 1 : 0;

    return slot1_free + slot2_free;
}