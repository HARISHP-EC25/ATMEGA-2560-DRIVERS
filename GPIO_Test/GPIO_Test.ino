#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "ultra.h"
#include "ir.h"
#include "buzzer.h"

// Function to handle button press with debounce
bool check_button_press(void) {
    // Check if button is pressed (Active-LOW with internal pull-up)
    if (!(PINA & (1 << PINA0))) {
        _delay_ms(50); // Software debounce delay
        
        // Re-check to ensure it's a valid press
        if (!(PINA & (1 << PINA0))) {
            // Wait until the button is released to prevent multi-triggering
            while (!(PINA & (1 << PINA0)));
            return true; // Single valid press registered
        }
    }
    return false;
}

void setup() {
    // 1. Initialize all core bare-metal drivers
    lcd_init();
    ultra_init();
    ir_init();
    buzzer_init();

    // 2. Configure Button Pin (PA0 / Pin 22) as Input with Internal Pull-Up
    DDRA &= ~(1 << DDA0);   // Set PA0 as input
    PORTA |= (1 << PORTA0); // Enable internal pull-up resistor
}

void loop() {
    // Tracks the current power state of the LCD screen
    static bool lcd_power_state = true; 

    // Check if the single push button was pressed
    if (check_button_press()) {
        lcd_power_state = !lcd_power_state; // Toggle state

        if (lcd_power_state) {
            // Command 0x0C: Turn LCD Display ON, Cursor OFF
            lcd_command(0x0C); 
        } else {
            // Command 0x08: Turn LCD Display OFF (blanks screen, preserves RAM)
            lcd_command(0x08); 
            buzzer_off(); // Ensure buzzer stops when screen is off
        }
    }

    // Run parking assistance logic only when the LCD is turned ON
    if (lcd_power_state) {
        long distance = ultra_read_distance();
        int free_slots = ir_get_total_free();

        // Control buzzer based on obstacle distance threshold (< 20 cm)
        if (distance < 20) {
            buzzer_on();
        } else {
            buzzer_off();
        }

        _delay_ms(100); // Sensor stabilization delay
    } else {
        // Low CPU polling delay when the LCD is blanked
        _delay_ms(50);
    }
}