#include <stm32f10x_lib.h>
#include <stm32f10x_tim.h>
#include <tim.h>

void TIM_Configuration3(void)//TIM初始化函数
{
  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 	//TIM 基本事件结构体
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //使能TIM 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
/*    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);*/
    
    TIM_DeInit(TIM3); 
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Prescaler = 199; 		        //TIM 预分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 计数模式
    TIM_TimeBaseStructure.TIM_Period = 159; 			//TIM 计数周期
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//TIM 时钟分频
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);				//PWM周期预加载
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//TIM 输出模式为PWM1 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//允许输出
    TIM_OCInitStructure.TIM_Pulse = 0; 			 //比较寄存器为500 占空比=比较寄存器/计数周期
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出优先级
//    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_Low;
    
    TIM_OC3Init(TIM3,&TIM_OCInitStructure);			//TIM3 CHANNEL1 INIT
//    TIM_OC2Init(TIM3,&TIM_OCInitStructure);			//TIM3 CHANNEL2 INIT
    
    TIM_CtrlPWMOutputs(TIM3, ENABLE); 				//允许TIM3 PWM 输出
    
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);           //外部输出控制的预加载（通道2)
//    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);		//外部输出控制的预加载（通道2)
    
    TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Repetitive);		//连续输出
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);				//PWM周期预加载
    
    TIM_Cmd(TIM3,ENABLE); 					//使能TIM3
/*    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //响应优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //允许中断
    NVIC_Init(&NVIC_InitStructure);                             //写入设置
    TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);*/
}


void PwmIni  (void)
{
    TIM_Configuration1();
//    TIM_Configuration2();
    TIM_Configuration3();
}
           