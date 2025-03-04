#ifndef BMP180_H
#define BMP180_H

#include "debug.h"
#include <stdint.h>

#define BMP180_ADDRESS  0x77
#define OSS 3

/* Function Prototypes */
void IIC_Init(void);
void BMP180_Init(void);
int32_t BMP180_GetTemperature(void);
int32_t BMP180_GetPressure(void);

/* New function to read and print sensor data */
void BMP180_PrintData(void);

#endif // BMP180_H
