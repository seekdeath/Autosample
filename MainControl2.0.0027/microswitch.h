#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
void initMS(void);
#define Signal2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)
#define Signal3  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)
#define Signal4  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define Signal5  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define Signal1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)
#define Signal6  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define Signal7  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define Signal8  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)
#define Signal9  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)

