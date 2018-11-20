/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

#define PwmStartAddr            0
#define SensitiveStartAddr      16
#define FreqStartAddr           64
#define SysParStartAddr         80
#define ChannelValAddr          1024
/*
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7
*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_8
#define SCL_L         GPIOB->BRR  = GPIO_Pin_8 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_9
#define SDA_L         GPIOB->BRR  = GPIO_Pin_9

#define SCL_read      GPIOB->IDR  & GPIO_Pin_8
#define SDA_read      GPIOB->IDR  & GPIO_Pin_9

#define I2C_PageSize  8  //24C02Ã¿Ò³8×Ö½Ú

void I2C_GPIO_Config(void);
//bool I2C_BufferWrite(u8* pBuffer,u16 length,u8 DeviceAddress);
bool I2C_BufferWrite(u8* pBuffer, u8 length,     u16 WriteAddress, u8 DeviceAddress);
bool I2C_ReadByte(u8* pBuffer,  u8 DeviceAddress);
bool I2C_ReadBuffer(u8* pBuffer,   u8 length,     u16 ReadAddress,  u8 DeviceAddress);
void I2C_PageWrite(  u8* pBuffer, u8 length,     u16 WriteAddress, u8 DeviceAddress);

