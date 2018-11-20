#include <includes.h>
#include <MotionErr.h>
#include <MotorCollect.h>
#include <Motortrasmition.h>
#include <Motor.h>

void InitRollMotor(void)
{
     u8	bOk = 1;
    
     //  设置收带电机参数
     TRY(bOk = MotorSetConfig(MotorRoll.nAddr, MotorRoll.nCtrlWord, MotorRoll.nTimeoutSteps));
     if (!bOk) {Set_Hook_State(Collect_Err_Transmition);}
     //  设置收带电机频率
     TRY(bOk = MotorSetHighSpeed(MotorRoll.nAddr, MotorRoll.nSpeedMin,MotorRoll.nSpeedMax,MotorRoll.nSpeedStep));
     if (!bOk) {Set_Hook_State(Collect_Err_Transmition);}
     
     // 设置电流
     TRY(bOk = MotorSetSpeedLevel(MotorRoll.nAddr, MotorRoll.LowspeedI,MotorRoll.HighspeedI,MotorRoll.HoldI));
     if (!bOk) {Set_Hook_State(Collect_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(MotorRoll.nAddr, MotorRoll.SpeedK , MotorRoll.ISensitive));
     if (!bOk) {Set_Hook_State(Collect_Err_Transmition);}
     // 设置电流

}
void MotorCollectInit(void)
{   
    //读取到位标志管脚
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //驱动5V电机管脚
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_14|GPIO_Pin_15);
//    TIM3_Configuration();
}

void MoveCollect(void)
{
    u16 time,num=0;
     u8 temp=0;
    GetCollectTime(&time);
    
//    TIM_SetCompare3(TIM3, 140);//百分之十占空比
/*    GPIO_SetBits(GPIOC,GPIO_Pin_14);
    GPIO_ResetBits(GPIOC,GPIO_Pin_15);
*/
   
    temp = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
//    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))
    if(temp == 1)
    {
       Set_Collect_State(Collect_Idle);
       return ;
    }
    MotorRun(MOTOR_ROLL_ADDR,1);  
//    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))
//    {
//       MotorRun(MOTOR_ROLL_ADDR,2);
//       OSTimeDly(300);
//       MotorUnlock(MOTOR_ROLL_ADDR,1);
//    }
//    
//    while (!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))
    temp = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
    while(!temp)
    {
        if (IS_Collect_Cancel())
        {
          Reset_Collect_Cancel();
          break;
        }
        OSTimeDly(100);
        num++;
        if (num==time)
        {
          break;
        }
        temp = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
    }
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9) || (num==time))
    {
       MotorRun(MOTOR_ROLL_ADDR,2);
       OSTimeDly(100);
       MotorUnlock(MOTOR_ROLL_ADDR,1);
    }
//    TIM_SetCompare3(TIM3, 0);//百分之0占空比
    if (num==time)
    {
          Set_Collect_State(Collect_Err_Micromotion);
    }
    else
    {
          Set_Collect_State(Collect_Idle);
    }
}



void TIM3_Configuration(void)
{
  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
    TIM_OCInitTypeDef  TIM_OCInitStructure; 
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    
    TIM_DeInit(TIM3); 
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_OCStructInit(&TIM_OCInitStructure); 
    TIM_TimeBaseStructure.TIM_Prescaler = 20; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseStructure.TIM_Period = 999; 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4; 
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
    TIM_OCInitStructure.TIM_Pulse = 100; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
    
    TIM_OC4Init(TIM3,&TIM_OCInitStructure);
    
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
    
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    TIM_SelectOnePulseMode(TIM3,TIM_OPMode_Repetitive);
    
    TIM_ARRPreloadConfig(TIM3, ENABLE);
    
    TIM_Cmd(TIM3,DISABLE);
  
}


void HandleCollect(void)
{
    if (IS_Collect_FLAG())
    {
        Reset_Collect_FLAG();
        Set_Collect_State(Collect_Busy);
        MoveCollect();
    }
}
//収带组件状态
u8 Collect_FLAG=0;
//判断收到组件标志
u8 IS_Collect_FLAG(void)
{
    return Collect_FLAG;
}
//置位収带组件标志
void Set_Collect_FLAG(void)
{
    Collect_FLAG=1;
}
//复位収带组件标志
void Reset_Collect_FLAG(void)
{
    Collect_FLAG=0;
}

u8 Collect_Cancel=0;
u8 IS_Collect_Cancel(void)
{
    return Collect_Cancel;
}
void Set_Collect_Cancel(void)
{
    Collect_Cancel=1;
}
void Reset_Collect_Cancel(void)
{
    Collect_Cancel=0;
}


u16 Collect_Time=0;
void GetCollectTime(u16* time)
{
    *time=Collect_Time;
}
void SetCollectTime(u16 time)
{
    Collect_Time=time;
}