#include "debug.h"

#define BMP180_ADDRESS  0x77  // BMP180 I2C address

// Global variables for calibration data
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;

void IIC_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef I2C_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; // PC1 - SDA
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // PC2 - SCL
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    I2C_InitStructure.I2C_ClockSpeed = 100000; // Standard speed
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);

    I2C_Cmd(I2C1, ENABLE);
}

void I2C_WriteByte(uint8_t devAddr, uint8_t reg, uint8_t data)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, devAddr << 1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, reg);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_SendData(I2C1, data);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t I2C_ReadByte(uint8_t devAddr, uint8_t reg)
{
    uint8_t receivedData;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, devAddr << 1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(I2C1, reg);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    I2C_Send7bitAddress(I2C1, devAddr << 1, I2C_Direction_Receiver);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET);
    receivedData = I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1, ENABLE);
    return receivedData;
}

void BMP180_Init()
{
    // Read calibration data from EEPROM
    AC1 = (I2C_ReadByte(BMP180_ADDRESS, 0xAA) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xAB);
    AC2 = (I2C_ReadByte(BMP180_ADDRESS, 0xAC) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xAD);
    AC3 = (I2C_ReadByte(BMP180_ADDRESS, 0xAE) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xAF);
    AC4 = (I2C_ReadByte(BMP180_ADDRESS, 0xB0) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xB1);
    AC5 = (I2C_ReadByte(BMP180_ADDRESS, 0xB2) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xB3);
    AC6 = (I2C_ReadByte(BMP180_ADDRESS, 0xB4) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xB5);
    B1 = (I2C_ReadByte(BMP180_ADDRESS, 0xB6) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xB7);
    B2 = (I2C_ReadByte(BMP180_ADDRESS, 0xB8) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xB9);
    MB = (I2C_ReadByte(BMP180_ADDRESS, 0xBA) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xBB);
    MC = (I2C_ReadByte(BMP180_ADDRESS, 0xBC) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xBD);
    MD = (I2C_ReadByte(BMP180_ADDRESS, 0xBE) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xBF);
}

uint16_t BMP180_GetTemperature()
{
    I2C_WriteByte(BMP180_ADDRESS, 0xF4, 0x2E);
    Delay_Ms(5);
    return (I2C_ReadByte(BMP180_ADDRESS, 0xF6) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xF7);
}

uint16_t BMP180_GetPressure()
{
    I2C_WriteByte(BMP180_ADDRESS, 0xF4, 0x2E);
    Delay_Ms(5);
    return (I2C_ReadByte(BMP180_ADDRESS, 0xF6) << 8) | I2C_ReadByte(BMP180_ADDRESS, 0xF7);

}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("CH32V003F4P6 - BMP180 Data Collection\r\n");
    IIC_Init();
    BMP180_Init();

    while (1)
    {
        uint16_t tempRaw = BMP180_GetTemperature();  // Get raw temperature

        // Step 1: Intermediate calculations
        int X1 = ((tempRaw - AC6) * AC5) >> 15;
        int X2 = (MC << 11) / (X1 + MD);
        int B5 = X1 + X2;

        // Step 2: Convert to Celsius (scale properly)
        int temperature = (B5 + 8) >> 4;  // Divide by 16 using bit shift

        printf("Temperature: %d.%01d C\r\n", temperature / 10, temperature % 10);  // Prints XX.X°C
        Delay_Ms(1000);
    }


}
