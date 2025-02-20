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

/* Function to reset all LEDs */
void Reset_All_LEDs(void)
{
    GPIO_ResetBits(GPIOD, LED_PIN_0 | LED_PIN_1 | LED_PIN_2 | LED_PIN_3);  // Turn off LEDs on PD0, PD2, PD3, PD4
    GPIO_ResetBits(GPIOC, LED_PIN_4 | LED_PIN_5 | LED_PIN_6 | LED_PIN_7);  // Turn off LEDs on PC1, PC2, PC3, PC4
}

/* Function to show LED chaser effect */
void LED_Chaser(void)
{
    int i;

    /* Chase through each LED one by one */
    for (i = 0; i < 8; i++)
    {
        /* Reset all LEDs */
        Reset_All_LEDs();

        /* Turn on the current LED in the sequence */
        switch (i)
        {
            case 0: GPIO_SetBits(GPIOD, LED_PIN_0); break;  // PD0
            case 1: GPIO_SetBits(GPIOD, LED_PIN_1); break;  // PD2
            case 2: GPIO_SetBits(GPIOD, LED_PIN_2); break;  // PD3
            case 3: GPIO_SetBits(GPIOD, LED_PIN_3); break;  // PD4
            case 4: GPIO_SetBits(GPIOC, LED_PIN_4); break;  // PC1
            case 5: GPIO_SetBits(GPIOC, LED_PIN_5); break;  // PC2
            case 6: GPIO_SetBits(GPIOC, LED_PIN_6); break;  // PC3
            case 7: GPIO_SetBits(GPIOC, LED_PIN_7); break;  // PC4
        }

        /* Wait for a short time (to create the chaser effect) */
        Delay_Ms(200);  // Adjust delay for desired speed
    }
    int j;

    /* Chase through each LED one by one */
    for (j = 0; j < 8; j++)
    {
        /* Reset all LEDs */
        Reset_All_LEDs();

        /* Turn on the current LED in the sequence */
        switch (j)
        {
            case 0: GPIO_SetBits(GPIOD, LED_PIN_0);  // PD0
            GPIO_SetBits(GPIOD, LED_PIN_1); break;  // PD2
            case 1: GPIO_SetBits(GPIOD, LED_PIN_2);  // PD3
            GPIO_SetBits(GPIOD, LED_PIN_3); break;  // PD4
            case 2: GPIO_SetBits(GPIOC, LED_PIN_4);  // PC1
            GPIO_SetBits(GPIOC, LED_PIN_5); break;  // PC2
            case 3: GPIO_SetBits(GPIOC, LED_PIN_6);// PC3
            GPIO_SetBits(GPIOC, LED_PIN_7); break;  // PC4
        }

        /* Wait for a short time (to create the chaser effect) */
        Delay_Ms(200);  // Adjust delay for desired speed
    }
    int k;

    /* Chase through each LED one by one */
    for (k = 0; k < 8; k++)
    {
        /* Reset all LEDs */
        Reset_All_LEDs();

        /* Turn on the current LED in the sequence */
        switch (k)
        {
            case 0: GPIO_SetBits(GPIOD, LED_PIN_0);  // PD0
            GPIO_SetBits(GPIOD, LED_PIN_1); // PD2
            GPIO_SetBits(GPIOD, LED_PIN_2);  // PD3
            GPIO_SetBits(GPIOD, LED_PIN_3); break;  // PD4
            case 1: GPIO_SetBits(GPIOC, LED_PIN_4);  // PC1
            GPIO_SetBits(GPIOC, LED_PIN_5);  // PC2
            GPIO_SetBits(GPIOC, LED_PIN_6);// PC3
            GPIO_SetBits(GPIOC, LED_PIN_7); break;  // PC4
        }

        /* Wait for a short time (to create the chaser effect) */
        Delay_Ms(200);  // Adjust delay for desired speed
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
        /* Show LED chaser effect */
        LED_Chaser();
    }
}
