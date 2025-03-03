#include "debug.h"
#include <math.h>

#define BMP180_ADDRESS  0x77  // BMP180 I2C address
#define OSS 3  // Oversampling setting (0,1,2,3)

// Global variables for calibration data
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;
int32_t B5;

void IIC_Init() {
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

uint16_t BMP180_GetTemperature() {
    I2C_WriteByte(BMP180_ADDRESS, 0xF4, 0x2E);
    Delay_Ms(5);
    return (I2C_Read16(BMP180_ADDRESS, 0xF6));
}

int32_t BMP180_GetPressure() {
    I2C_WriteByte(BMP180_ADDRESS, 0xF4, 0x34 + (OSS << 6));
    Delay_Ms(26);
    return (I2C_Read16(BMP180_ADDRESS, 0xF6) << 8) | I2C_Read16(BMP180_ADDRESS, 0xF8);
}

int32_t BMP180_GetAltitude(int32_t pressure) {
    int32_t seaLevelPressure = 101325;
    return ((seaLevelPressure - pressure) * 44330) / seaLevelPressure;
}

int main() {
    Delay_Init(); // Ensure delay functions work
    USART_Printf_Init(115200); // Initialize debugging UART
    printf("BMP180 Initialization...\n\r");

    IIC_Init();
    BMP180_Init();

    while (1) {
        int32_t temperature = (BMP180_GetTemperature()/100);
        int32_t pressure = BMP180_GetPressure();
        int32_t altitude = BMP180_GetAltitude(pressure);

        printf("Temperature: %d.%d C\r\n", temperature / 10, temperature % 10);
        printf("Pressure: %ld Pa\r\n", pressure);
        printf("Altitude: %ld m\n\r", altitude);

        Delay_Ms(1000);
    }

    return 0;
}

