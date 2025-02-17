#include "debug.h"

/* Define the LED pin */
#define LED_PIN GPIO_Pin_0  // LED on PD0

/* Function to configure the GPIO for the LED */
void GPIO_LED_CFG(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* Enable the clock for Port D */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    /* Configure PD0 as output (push-pull) */
    GPIO_InitStructure.GPIO_Pin = LED_PIN;  // Use PD0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // Speed: 50 MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // Output: Push-pull
    GPIO_Init(GPIOD, &GPIO_InitStructure);  // Apply settings
}

/* Function to toggle the LED on PD0 */
void LED_Toggle(void)
{
    /* Toggle the LED by writing the opposite value */
    if (GPIO_ReadOutputDataBit(GPIOD, LED_PIN)) {
        GPIO_ResetBits(GPIOD, LED_PIN);  // Turn off LED
    } else {
        GPIO_SetBits(GPIOD, LED_PIN);    // Turn on LED
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

    /* Configure LED pin */
    GPIO_LED_CFG();

    while(1)
    {
        /* Toggle the LED on PD0 */
        LED_Toggle();

        /* Delay to create blinking effect */
        Delay_Ms(500);  // Delay for 500ms (adjust for blink rate)
    }
}
