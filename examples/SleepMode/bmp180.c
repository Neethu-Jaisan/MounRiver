#include "bmp180.h"
#include "debug.h"
#include <math.h>

// Calibration Variables
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;
int32_t B5;

// I2C Initialization
void IIC_Init() {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef I2C_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStructure);

    I2C_Cmd(I2C1, ENABLE);
}

// I2C Write Byte
void I2C_WriteByte(uint8_t devAddr, uint8_t reg, uint8_t data) {
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

// I2C Read 16-bit Data
uint16_t I2C_Read16(uint8_t devAddr, uint8_t reg) {
    uint16_t data;
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
    data = I2C_ReceiveData(I2C1) << 8;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET);
    data |= I2C_ReceiveData(I2C1);
    I2C_GenerateSTOP(I2C1, ENABLE);
    return data;
}

// BMP180 Initialization
void BMP180_Init() {
    AC1 = I2C_Read16(BMP180_ADDRESS, 0xAA);
    AC2 = I2C_Read16(BMP180_ADDRESS, 0xAC);
    AC3 = I2C_Read16(BMP180_ADDRESS, 0xAE);
    AC4 = I2C_Read16(BMP180_ADDRESS, 0xB0);
    AC5 = I2C_Read16(BMP180_ADDRESS, 0xB2);
    AC6 = I2C_Read16(BMP180_ADDRESS, 0xB4);
    B1 = I2C_Read16(BMP180_ADDRESS, 0xB6);
    B2 = I2C_Read16(BMP180_ADDRESS, 0xB8);
    MB = I2C_Read16(BMP180_ADDRESS, 0xBA);
    MC = I2C_Read16(BMP180_ADDRESS, 0xBC);
    MD = I2C_Read16(BMP180_ADDRESS, 0xBE);
}

// Get Temperature from BMP180
int32_t BMP180_GetTemperature() {
    I2C_WriteByte(BMP180_ADDRESS, 0xF4, 0x2E);
    Delay_Ms(5);
    int32_t UT = I2C_Read16(BMP180_ADDRESS, 0xF6);

    int32_t X1 = ((UT - AC6) * AC5) >> 15;
    int32_t X2 = (MC << 11) / (X1 + MD);
    B5 = X1 + X2;

    return ((B5 + 8) >> 4); // Temperature in 0.1°C
}

// Get Pressure from BMP180
int32_t BMP180_GetPressure() {
    I2C_WriteByte(BMP180_ADDRESS, 0xF4, 0x34 + (OSS << 6));
    Delay_Ms(26);

    int32_t UP = ((I2C_Read16(BMP180_ADDRESS, 0xF6) << 8) | I2C_Read16(BMP180_ADDRESS, 0xF8)) >> (8 - OSS);

    int32_t B6 = B5 - 4000;
    int32_t X1 = (B2 * (B6 * B6 >> 12)) >> 11;
    int32_t X2 = (AC2 * B6) >> 11;
    int32_t X3 = X1 + X2;
    int32_t B3 = (((AC1 * 4 + X3) << OSS) + 2) >> 2;
    X1 = (AC3 * B6) >> 13;
    X2 = (B1 * (B6 * B6 >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    uint32_t B4 = (AC4 * (uint32_t)(X3 + 32768)) >> 15;
    uint32_t B7 = ((uint32_t)UP - B3) * (50000 >> OSS);

    int32_t pressure = (B7 < 0x80000000) ? (B7 * 2) / B4 : (B7 / B4) * 2;
    X1 = (pressure >> 8) * (pressure >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * pressure) >> 16;
    pressure += (X1 + X2 + 3791) >> 4;

    return pressure;
}

/* New Function: Read sensor data and print it */
void BMP180_PrintData(void)
{
    int32_t temperature = BMP180_GetTemperature();
    int32_t pressure = BMP180_GetPressure();
    printf("Temperature: %d.%d C\r\n", temperature / 10, temperature % 10);
    printf("Pressure: %ld Pa\r\n", pressure);
}
