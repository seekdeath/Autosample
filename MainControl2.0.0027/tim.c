#include <stm32f10x_lib.h>
#include <stm32f10x_tim.h>
#include <tim.h>

void TIM_Configuration3(void)//TIM��ʼ������
{
  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 	//TIM �����¼��ṹ��
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //ʹ��TIM ʱ��
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
    TIM_TimeBaseStructure.TIM_Prescaler = 199; 		        //TIM Ԥ��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ����ģʽ
    TIM_TimeBaseStructure.TIM_Period = 159; 			//TIM ��������
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//TIM ʱ�ӷ�Ƶ
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);				//PWM����Ԥ����
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		//TIM ���ģʽΪPWM1 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�������
    TIM_OCInitStructure.TIM_Pulse = 0; 			 //�ȽϼĴ���Ϊ500 ռ�ձ�=�ȽϼĴ���/��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������ȼ�
//    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_Low;
    
    TIM_OC3Init(TIM3,&TIM_OCInitStructure);			//TIM3 CHANNEL1 INIT
//    TIM_OC2Init(TIM3,&TIM_OCInitStructure);			//TIM3 CHANNEL2 INIT
    
    TIM_CtrlPWMOutputs(TIM3, ENABLE); 				//����TIM3 PWM ���
    
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);           //�ⲿ������Ƶ�Ԥ���أ�ͨ��2)
//    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);		//�ⲿ������Ƶ�Ԥ���أ�ͨ��2)
    
    TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Repetitive);		//�������
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);				//PWM����Ԥ����
    
    TIM_Cmd(TIM3,ENABLE); 					//ʹ��TIM3
/*    
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //��Ӧ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //�����ж�
    NVIC_Init(&NVIC_InitStructure);                             //д������
    TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);*/
}


void PwmIni  (void)
{
    TIM_Configuration1();
//    TIM_Configuration2();
    TIM_Configuration3();
}
           