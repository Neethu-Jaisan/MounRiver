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
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Turn off LED initially */
    GPIO_ResetBits(GPIOD, LED_PIN);
}

/* Function to configure TIM2 to generate an interrupt every 1 second */
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    /* Enable the clock for TIM2 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* TIM2 configuration for 1s delay */
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;  // ✅ Correct: 1 second at 1kHz tick
    TIM_TimeBaseStructure.TIM_Prescaler = 48000 - 1; // ✅ Correct: 1 kHz timer tick
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Enable TIM2 update interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* Configure NVIC for TIM2 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Start TIM2 */
    TIM_Cmd(TIM2, ENABLE);
}

/* TIM2 Interrupt Service Routine */
void TIM2_IRQHandler(void) __attribute__((interrupt));
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // Clear interrupt flag

        /* Toggle the LED */
        GPIO_WriteBit(GPIOD, LED_PIN, (BitAction)(!GPIO_ReadOutputDataBit(GPIOD, LED_PIN)));

        /* Debug print */
        printf("TIM2 Interrupt Triggered!\r\n");
    }
}

/* Main function */
int main(void)
{
    /* System initialization */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    USART_Printf_Init(115200);

    printf("System Clock: %d Hz\r\n", SystemCoreClock);
    printf("Chip ID: %08X\r\n", DBGMCU_GetCHIPID());

    /* Configure LED pin */
    GPIO_LED_CFG();

    /* Test LED manually
    GPIO_WriteBit(GPIOD, LED_PIN, Bit_SET);
    Delay_Ms(1000);
    GPIO_WriteBit(GPIOD, LED_PIN, Bit_RESET);
    Delay_Ms(1000);*/

    /* Configure TIM2 for 1s interrupt */
    TIM2_Init();

    while (1)
    {
        /* Main loop does nothing; LED toggling is handled by TIM2 interrupt */
    }
}
