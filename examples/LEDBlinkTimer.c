#include "debug.h"  // Include standard debug and system functions

/* Define the LED pin */
#define LED_PIN GPIO_Pin_0  // LED connected to PD0

/**
 * @brief Configures the GPIO pin for the LED
 */
void GPIO_LED_CFG(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};  // Initialize GPIO structure

    /* Enable the clock for Port D */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    /* Configure PD0 as output (push-pull) */
    GPIO_InitStructure.GPIO_Pin = LED_PIN;          // Select PD0 as the LED pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Set speed to 50 MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // Set as Push-Pull Output mode
    GPIO_Init(GPIOD, &GPIO_InitStructure);          // Apply configuration to GPIOD

    /* Set LED initially OFF */
    GPIO_ResetBits(GPIOD, LED_PIN);
}

/**
 * @brief Configures TIM2 to generate an interrupt every 1 second
 */
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0}; // Initialize timer structure
    NVIC_InitTypeDef NVIC_InitStructure = {0}; // Initialize interrupt configuration structure

    /* Enable the clock for TIM2 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* TIM2 configuration for 1-second delay */
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;   // Set timer period (1000 counts = 1 second at 1kHz)
    TIM_TimeBaseStructure.TIM_Prescaler = 48000 - 1; // Set prescaler (48 MHz / 48000 = 1 kHz tick rate)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // No clock division
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Count upwards
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // Apply timer configuration

    /* Enable TIM2 update interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* Configure NVIC (Nested Vectored Interrupt Controller) for TIM2 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // Assign TIM2 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // Set preemption priority (lower = higher priority)
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // Set sub-priority
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // Enable the interrupt channel
    NVIC_Init(&NVIC_InitStructure); // Apply NVIC configuration

    /* Start TIM2 */
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief TIM2 Interrupt Service Routine (ISR)
 * This function executes every 1 second when TIM2 reaches its set period.
 */
void TIM2_IRQHandler(void) __attribute__((interrupt)); // Define function with interrupt attribute
void TIM2_IRQHandler(void)
{
    /* Check if the TIM2 update interrupt flag is set */
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // Clear the interrupt flag to avoid repeated execution

        /* Toggle the LED */
        GPIO_WriteBit(GPIOD, LED_PIN, (BitAction)(!GPIO_ReadOutputDataBit(GPIOD, LED_PIN)));

        /* Debug message to confirm ISR is running */
        printf("TIM2 Interrupt Triggered!\r\n");
    }
}

/**
 * @brief Main function
 */
int main(void)
{
    /* Set NVIC priority group (determines interrupt priority handling) */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Update system clock information */
    SystemCoreClockUpdate();

    /* Initialize UART for serial debugging */
    USART_Printf_Init(115200);

    /* Print system information */
    printf("System Clock: %d Hz\r\n", SystemCoreClock);
    printf("Chip ID: %08X\r\n", DBGMCU_GetCHIPID());

    /* Configure LED pin */
    GPIO_LED_CFG();

    /* Test LED manually before enabling timer */
    GPIO_WriteBit(GPIOD, LED_PIN, Bit_SET);  // Turn LED ON
    Delay_Ms(1000);                          // Wait 1 second
    GPIO_WriteBit(GPIOD, LED_PIN, Bit_RESET); // Turn LED OFF
    Delay_Ms(1000);                          // Wait 1 second

    /* Configure TIM2 to generate interrupt every 1 second */
    TIM2_Init();

    /* Infinite loop - LED toggling is handled by the TIM2 interrupt */
    while (1);
}
