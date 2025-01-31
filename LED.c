#include "ch32v00x.h"

// Define LED pin (PC0)
#define LED_PIN (1 << 0)

void Delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < (ms * 8000); i++) {
        __NOP(); // No operation, just waste cycles
    }
}

void LED_Init() {
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC; // Enable GPIOC clock
    GPIOC->CFGLR &= ~(0xF << (4 * 0));      // Clear PC0 mode
    GPIOC->CFGLR |= (0x3 << (4 * 0));       // Set PC0 as output (50MHz)
}

void LED_Blink() {
    while(1) {
        GPIOC->BSHR = LED_PIN; // LED ON
        Delay_ms(500);

        GPIOC->BCR = LED_PIN;  // LED OFF
        Delay_ms(500);

        GPIOC->BSHR = LED_PIN; // LED ON
        Delay_ms(500);

        GPIOC->BSHR = LED_PIN; // LED ON
        Delay_ms(500);

        GPIOC->BCR = LED_PIN;  // LED OFF
        Delay_ms(500);

        GPIOC->BCR = LED_PIN; // LED ON
        Delay_ms(500);

        GPIOC->BCR = LED_PIN;  // LED OFF
        Delay_ms(3000); // Extra delay before repeating
    }
}

int main() {
    SystemInit();  // Initialize system clock
    LED_Init();    // Initialize LED pin

    LED_Blink();   // Start blinking
}
