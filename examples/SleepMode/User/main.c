#include "debug.h"
#include "bmp180.h"

/* Initialize TIM2 to generate an interrupt every 10 seconds */
void TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    /* Enable TIM2 Clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Configure TIM2 to overflow every 10 seconds */
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1; // 10,000 ms = 10 sec
    TIM_TimeBaseStructure.TIM_Prescaler = 48000 - 1; // Assuming 48 MHz clock -> 1 ms tick
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Enable TIM2 Interrupt */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* Configure NVIC for TIM2 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Start TIM2 */
    TIM_Cmd(TIM2, ENABLE);
}

/* TIM2 Interrupt Handler */
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        printf("TIM2 Wake-up Event!\r\n");
    }
}

int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);

    printf("SystemClk: %d\r\n", SystemCoreClock);
    printf("TIM2 Sleep Mode and BMP180 Sensor Test\r\n");

    /* Initialize TIM2 and BMP180 sensor */
    TIM2_Init();
    IIC_Init();
    BMP180_Init();

    while (1)
    {
        /* Modular function that reads and prints sensor data */
        BMP180_PrintData();

        printf("Entering Sleep Mode...\r\n");
        __WFI();  // Enter sleep mode (will be woken by TIM2 interrupt)
        printf("Woken up!\r\n");

        Delay_Ms(1000);  // Delay before next cycle
    }
}
