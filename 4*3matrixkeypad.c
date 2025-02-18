#include "debug.h"

/* Define Row and Column Pins */
#define ROW1   GPIO_Pin_0  // PC0
#define ROW2   GPIO_Pin_1  // PC1
#define ROW3   GPIO_Pin_2  // PC2
#define ROW4   GPIO_Pin_3  // PC3
#define COL1   GPIO_Pin_4  // PC4
#define COL2   GPIO_Pin_5  // PC5
#define COL3   GPIO_Pin_6  // PC6

/* Keypad Layout */
/* Corrected Keypad Layout */
char keypad[4][3] = {
    {'9', '6', '3'},  // Row 1
    {'8', '5', '2'},  // Row 2
    {'7', '4', '1'},  // Row 3
    {'*', '0', '#'}   // Row 4 (Assumed to be correct)
};


/* GPIO Configuration */
void Keypad_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure ROWS (Outputs) */
    GPIO_InitStructure.GPIO_Pin = ROW1 | ROW2 | ROW3 | ROW4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Configure COLUMNS (Inputs with Pull-down) */
    GPIO_InitStructure.GPIO_Pin = COL1 | COL2 | COL3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/* Function to Scan Keypad */
char Keypad_Scan(void)
{
    for (int row = 0; row < 4; row++)
    {
        /* Set all rows LOW */
        GPIO_ResetBits(GPIOC, ROW1 | ROW2 | ROW3 | ROW4);
        /* Set the current row HIGH */
        GPIO_SetBits(GPIOC, (ROW1 << row));

        /* Check each column */
        if (GPIO_ReadInputDataBit(GPIOC, COL1)) return keypad[row][0];
        if (GPIO_ReadInputDataBit(GPIOC, COL2)) return keypad[row][1];
        if (GPIO_ReadInputDataBit(GPIOC, COL3)) return keypad[row][2];
    }
    return '\0'; // No key pressed
}

/* Main Function */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    Keypad_Init();
    printf("4x3 Keypad Interfacing Started\n");

    while (1)
    {
        char key = Keypad_Scan();
        if (key != '\0')
        {
            printf("Key Pressed: %c\n", key);
            Delay_Ms(300); // Debounce delay
        }
    }
}
