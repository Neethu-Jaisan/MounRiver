#include "debug.h"

/* Define the LED pins (PD0, PD2, PD3, PD4, PC1, PC2, PC3, PC4) */
#define LED_PIN_0 GPIO_Pin_0  // LED on PD0
#define LED_PIN_1 GPIO_Pin_2  // LED on PD2
#define LED_PIN_2 GPIO_Pin_3  // LED on PD3
#define LED_PIN_3 GPIO_Pin_4  // LED on PD4
#define LED_PIN_4 GPIO_Pin_1  // LED on PC1
#define LED_PIN_5 GPIO_Pin_2  // LED on PC2
#define LED_PIN_6 GPIO_Pin_3  // LED on PC3
#define LED_PIN_7 GPIO_Pin_4  // LED on PC4

/* Function to configure the GPIO for the LEDs */
void GPIO_LED_CFG(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* Enable the clock for Port D and Port C */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure PD0, PD2, PD3, PD4 as output (push-pull) */
    GPIO_InitStructure.GPIO_Pin = LED_PIN_0 | LED_PIN_1 | LED_PIN_2 | LED_PIN_3;  // Pins PD0, PD2, PD3, PD4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // Speed: 50 MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // Output: Push-pull
    GPIO_Init(GPIOD, &GPIO_InitStructure);  // Apply settings

    /* Configure PC1, PC2, PC3, PC4 as output (push-pull) */
    GPIO_InitStructure.GPIO_Pin = LED_PIN_4 | LED_PIN_5 | LED_PIN_6 | LED_PIN_7;  // Pins PC1, PC2, PC3, PC4
    GPIO_Init(GPIOC, &GPIO_InitStructure);  // Apply settings
}

/* Function to toggle the LEDs */
void LED_Toggle(void)
{
    /* Toggle the LEDs on PD0, PD2, PD3, PD4 */
    if (GPIO_ReadOutputDataBit(GPIOD, LED_PIN_0)) {
        GPIO_ResetBits(GPIOD, LED_PIN_0);  // Turn off LED on PD0
    } else {
        GPIO_SetBits(GPIOD, LED_PIN_0);    // Turn on LED on PD0
    }

    if (GPIO_ReadOutputDataBit(GPIOD, LED_PIN_1)) {
        GPIO_ResetBits(GPIOD, LED_PIN_1);  // Turn off LED on PD2
    } else {
        GPIO_SetBits(GPIOD, LED_PIN_1);    // Turn on LED on PD2
    }

    if (GPIO_ReadOutputDataBit(GPIOD, LED_PIN_2)) {
        GPIO_ResetBits(GPIOD, LED_PIN_2);  // Turn off LED on PD3
    } else {
        GPIO_SetBits(GPIOD, LED_PIN_2);    // Turn on LED on PD3
    }

    if (GPIO_ReadOutputDataBit(GPIOD, LED_PIN_3)) {
        GPIO_ResetBits(GPIOD, LED_PIN_3);  // Turn off LED on PD4
    } else {
        GPIO_SetBits(GPIOD, LED_PIN_3);    // Turn on LED on PD4
    }

    /* Toggle the LEDs on PC1, PC2, PC3, PC4 */
    if (GPIO_ReadOutputDataBit(GPIOC, LED_PIN_4)) {
        GPIO_ResetBits(GPIOC, LED_PIN_4);  // Turn off LED on PC1
    } else {
        GPIO_SetBits(GPIOC, LED_PIN_4);    // Turn on LED on PC1
    }

    if (GPIO_ReadOutputDataBit(GPIOC, LED_PIN_5)) {
        GPIO_ResetBits(GPIOC, LED_PIN_5);  // Turn off LED on PC2
    } else {
        GPIO_SetBits(GPIOC, LED_PIN_5);    // Turn on LED on PC2
    }

    if (GPIO_ReadOutputDataBit(GPIOC, LED_PIN_6)) {
        GPIO_ResetBits(GPIOC, LED_PIN_6);  // Turn off LED on PC3
    } else {
        GPIO_SetBits(GPIOC, LED_PIN_6);    // Turn on LED on PC3
    }

    if (GPIO_ReadOutputDataBit(GPIOC, LED_PIN_7)) {
        GPIO_ResetBits(GPIOC, LED_PIN_7);  // Turn off LED on PC4
    } else {
        GPIO_SetBits(GPIOC, LED_PIN_7);    // Turn on LED on PC4
    }
}

/* Main function */
int main(void)
{
    /* System initialization */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);

    /* Configure LED pins */
    GPIO_LED_CFG();

    while(1)
    {
        /* Toggle the LEDs */
        LED_Toggle();

        /* Delay to create blinking effect */
        Delay_Ms(500);  // Delay for 500ms (adjust for blink rate)
    }
}
