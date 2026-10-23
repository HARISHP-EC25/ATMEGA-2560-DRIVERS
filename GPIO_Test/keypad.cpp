#include "keypad.h"
#include <avr/io.h>
#include <util/delay.h>

/* Encapsulated character map for a standard 4x4 keypad */
static const char key_map[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void keypad_init(void) {
    /* Configure PORTL (Arduino Mega pins 42-49):
     * PL0 - PL3 (Lower Nibble) as Outputs -> Rows
     * PL4 - PL7 (Upper Nibble) as Inputs  -> Columns */
    DDRL = 0x0F;   

    /* Initialize PORTL:
     * Enable internal pull-up resistors on column inputs (PL4-PL7 = 1)
     * Set row outputs to HIGH initially */
    PORTL = 0xF0;  
}

char keypad_get_key(void) {
    uint8_t row, col;

    for (row = 0; row < 4; row++) {
        /* Drive one row LOW at a time while keeping other rows HIGH 
         * row_mask clears the specific row bit (0 to 3) to 0 */
        uint8_t row_mask = ~(1 << row) & 0x0F;
        PORTL = (PORTL & 0xF0) | row_mask;

        /* Brief stabilization delay for the signal */
        _delay_ms(2);

        /* Read the state of the column pins (PL4 to PL7) from PINL */
        uint8_t pin_state = PINL;

        for (col = 0; col < 4; col++) {
            /* Check if the corresponding column pin (bit 4 + col) is pulled LOW */
            if (!(pin_state & (1 << (col + 4)))) {
                
                /* Software debounce */
                _delay_ms(20);
                
                /* Wait until the user releases the key to prevent multiple triggers */
                while (!(PINL & (1 << (col + 4))));
                
                /* Return the mapped character */
                return key_map[row][col];
            }
        }
    }

    return '\0'; // Return null character if no key is pressed
}