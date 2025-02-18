#include "debug.h"

#define IR_PIN GPIO_Pin_0 // PD0 (IR Receiver pin)

#define IR_TIMEOUT 1000 // Timeout to check for signal in ms

// Variables for pulse width detection
volatile uint32_t high_pulse_duration = 0;
volatile uint32_t low_pulse_duration = 0;
volatile uint8_t ir_signal_received = 0;

void IR_Receiver_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // Change to GPIOD for PD0

    // Configure IR receiver pin as input
    GPIO_InitStructure.GPIO_Pin = IR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Internal pull-up
    GPIO_Init(GPIOD, &GPIO_InitStructure); // Change to GPIOD
}

void IR_Receiver_Timer_Init(void)
{
    // Timer initialization for pulse width measurement
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_InitStructure = {0};
    TIM_InitStructure.TIM_Period = 0xFFFF;
    TIM_InitStructure.TIM_Prescaler = 72 - 1;  // Prescaler to generate 1ms intervals
    TIM_InitStructure.TIM_ClockDivision = 0;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
    TIM_Cmd(TIM2, ENABLE);
}

void IR_Receiver_Decode(void)
{
    uint8_t key = 0;

    if (ir_signal_received)
    {
        // Process received IR signal based on pulse duration and decode it
        if (high_pulse_duration > 1000 && low_pulse_duration < 500) // Example pattern for key "1"
        {
            key = 1;
        }
        else if (high_pulse_duration > 1200 && low_pulse_duration < 600) // Example pattern for key "2"
        {
            key = 2;
        }

        // You can add more conditions based on pulse length to map other keys
        // Print the decoded key
        printf("Key Pressed: %d\n", key);
        ir_signal_received = 0; // Reset signal flag
    }
}

void IR_Receiver_ISR(void)
{
    static uint32_t last_time = 0;

    // Read the current IR pin state (either high or low)
    uint8_t ir_state = GPIO_ReadInputDataBit(GPIOD, IR_PIN); // Change to GPIOD for PD0

    // Time when the signal changes
    uint32_t current_time = TIM2->CNT;

    if (ir_state == RESET) // Signal goes LOW (start of a pulse)
    {
        low_pulse_duration = current_time - last_time; // Record duration of LOW pulse
    }
    else // Signal goes HIGH (start of HIGH pulse)
    {
        high_pulse_duration = current_time - last_time; // Record duration of HIGH pulse
        ir_signal_received = 1; // Mark signal as received
    }

    last_time = current_time; // Update last time
}

int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);

    // Initialize IR receiver and timer
    IR_Receiver_Init();
    IR_Receiver_Timer_Init();

    printf("IR Receiver Initialized\n");

    while (1)
    {
        IR_Receiver_Decode();
        Delay_Ms(500); // Check for a new key press every 500ms
    }
}
