#include <includes.h>
#include <gpio.h>
#include <Agreement.h>
void LD1Ini(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
}
/*void CupStateConfig(void)//抓杯状态管脚初始化
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}*/
void GPIO_Interrupt(void)
{
  
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;            //设定外部中断0

  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //设定中断模式

  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //设定下降沿触发模式

  EXTI_InitStructure.EXTI_LineCmd = ENABLE;

  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
}
void LackWater(void)//抓杯状态管脚初始化
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void GetLackState(void)
{
  unsigned char Lacktemp1;
  unsigned char Lacktemp2;
  if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
  {
    OSTimeDlyHMSM (0, 0, 0, 10);
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
    Lacktemp1 = 1;
  }
  else Lacktemp1 = 0;
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
  {
    OSTimeDlyHMSM (0, 0, 0, 10);
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
    Lacktemp2 = 2;
  }
  else Lacktemp2 = 0;
  LackFlag = Lacktemp1 + Lacktemp2;
}