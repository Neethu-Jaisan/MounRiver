#include "debug.h"

/* Define LCD Pins */
#define LCD_RS   GPIO_Pin_0  // PC0
#define LCD_E    GPIO_Pin_1  // PC1
#define LCD_D4   GPIO_Pin_2  // PC2
#define LCD_D5   GPIO_Pin_3  // PC3
#define LCD_D6   GPIO_Pin_4  // PC4
#define LCD_D7   GPIO_Pin_5  // PC5

/* Function to Send Command */
void LCD_Command(uint8_t cmd)
{
    GPIO_ResetBits(GPIOC, LCD_RS); // RS = 0 for command
    GPIO_WriteBit(GPIOC, LCD_D4, (cmd >> 4) & 1);
    GPIO_WriteBit(GPIOC, LCD_D5, (cmd >> 5) & 1);
    GPIO_WriteBit(GPIOC, LCD_D6, (cmd >> 6) & 1);
    GPIO_WriteBit(GPIOC, LCD_D7, (cmd >> 7) & 1);

    GPIO_SetBits(GPIOC, LCD_E);
    Delay_Ms(1);
    GPIO_ResetBits(GPIOC, LCD_E);

    GPIO_WriteBit(GPIOC, LCD_D4, (cmd >> 0) & 1);
    GPIO_WriteBit(GPIOC, LCD_D5, (cmd >> 1) & 1);
    GPIO_WriteBit(GPIOC, LCD_D6, (cmd >> 2) & 1);
    GPIO_WriteBit(GPIOC, LCD_D7, (cmd >> 3) & 1);

    GPIO_SetBits(GPIOC, LCD_E);
    Delay_Ms(1);
    GPIO_ResetBits(GPIOC, LCD_E);
    Delay_Ms(2);
}

/* Function to Send Data */
void LCD_Char(char data)
{
    GPIO_SetBits(GPIOC, LCD_RS); // RS = 1 for data
    GPIO_WriteBit(GPIOC, LCD_D4, (data >> 4) & 1);
    GPIO_WriteBit(GPIOC, LCD_D5, (data >> 5) & 1);
    GPIO_WriteBit(GPIOC, LCD_D6, (data >> 6) & 1);
    GPIO_WriteBit(GPIOC, LCD_D7, (data >> 7) & 1);

    GPIO_SetBits(GPIOC, LCD_E);
    Delay_Ms(1);
    GPIO_ResetBits(GPIOC, LCD_E);

    GPIO_WriteBit(GPIOC, LCD_D4, (data >> 0) & 1);
    GPIO_WriteBit(GPIOC, LCD_D5, (data >> 1) & 1);
    GPIO_WriteBit(GPIOC, LCD_D6, (data >> 2) & 1);
    GPIO_WriteBit(GPIOC, LCD_D7, (data >> 3) & 1);

    GPIO_SetBits(GPIOC, LCD_E);
    Delay_Ms(1);
    GPIO_ResetBits(GPIOC, LCD_E);
    Delay_Ms(2);
}

/* Function to Initialize LCD */
void LCD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* Enable Clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    /* Configure LCD Pins as Output */
    GPIO_InitStructure.GPIO_Pin = LCD_RS | LCD_E | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* LCD Initialization */
    Delay_Ms(20);
    LCD_Command(0x02); // Set 4-bit mode
    LCD_Command(0x28); // 2-line, 5x7 matrix
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x06); // Auto-increment cursor
    LCD_Command(0x01); // Clear Display
}

/* Function to Print String */
void LCD_String(char *str)
{
    while (*str)
    {
        LCD_Char(*str++);
    }
}

/* Main Function */
int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    LCD_Init();

    LCD_String("Hello, CH32V003!");
    LCD_Command(0xC0); // Move to Second Line
    LCD_String("LCD 16x2 Display");

    while (1)
    {
    }
}
