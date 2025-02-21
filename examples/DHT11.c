#include "debug.h"

#define DHT11_PIN GPIO_Pin_0
#define DHT11_PORT GPIOD

void DHT11_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
}

void DHT11_Start(void) {
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
    Delay_Ms(18);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
    Delay_Us(30);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

uint8_t DHT11_CheckResponse(void) {
    uint8_t response = 0;
    Delay_Us(40);
    if (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
        Delay_Us(80);
        if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) response = 1;
        while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN));
    }
    return response;
}

uint8_t DHT11_ReadByte(void) {
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++) {
        while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN));
        Delay_Us(40);
        if (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) data |= (1 << (7 - i));
        while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN));
    }
    return data;
}

int main(void) {
    uint8_t  h_int,h_dec,t_int, t_dec;
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    //printf("SystemClk: %d\r\n", SystemCoreClock);
    //printf("DHT11 Humidity & Temperature Reading on PD0\r\n");

    DHT11_Init();

    while (1) {
        DHT11_Start();
        if (DHT11_CheckResponse()) {
            h_int = DHT11_ReadByte();
            h_dec=DHT11_ReadByte(); // Skip decimal humidity
            t_int = DHT11_ReadByte();
            t_dec=DHT11_ReadByte(); // Skip decimal temperature
            printf("Humidity: %d.%d%%, Temperature: %d.%d°C\r\n", h_int, h_dec, t_int, t_dec);
        } else {
            printf("DHT11 No Response!\r\n");
        }
        Delay_Ms(5000);
    }
}
