#include <includes.h>
#include <WaterPump.h>
#include <MotionErr.h>
#include <MotorTrasmition.h>
#include <Motor.h>
u8 WaterPump_ONOFF[3]={0};
u16 WaterPump_TIME[3]={0};
u8 WaterPump_FLAG = 0;
u8 Pump_FLAG = 0;
void SetWaterPumpParam(u8 Channel,u8 ONOFF,u16 time)
{
    WaterPump_ONOFF[Channel]=ONOFF;
    WaterPump_TIME[Channel]=time;
}

void GetWaterPumpParam(u8 Channel,u8 *ONOFF,u16 *time)
{
    *ONOFF=WaterPump_ONOFF[Channel];
    *time=WaterPump_TIME[Channel];
}

void InitWaterPump(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_14|GPIO_Pin_13);
}

void InitWaterPumpMotor(void)
{
  u8	bOk = 1;//
       // 设置试剂针电机参数
/*    TRY(bOk = MotorSetConfig(MotorPump.nAddr, MotorPump.nCtrlWord, MotorPump.nTimeoutSteps));
    if (!bOk) {Set_WaterPump_State(Drain_Err_Transmition);}
    // 设置排水泵电机频率
    TRY(bOk = MotorSetSpeed(MotorPump.nAddr, MotorPump.nSpeedMin,MotorPump.nSpeedMax,MotorPump.nSpeedStep));
    if (!bOk) {Set_WaterPump_State(Drain_Err_Transmition);}
    u8	bOk = 1;//*/
     
    // 设置排水泵电机频率
    TRY(bOk = MotorSetHighSpeed(MotorPump.nAddr, MotorPump.nSpeedMin,MotorPump.nSpeedMax,MotorPump.nSpeedStep));
    if (!bOk) {Set_WaterPump_State(Drain_Err_Transmition);}//MotorSetHighSpeed
      
    // 设置电流
    TRY(bOk = MotorSetSpeedLevel(MotorPump.nAddr, MotorPump.LowspeedI,MotorPump.HighspeedI,MotorPump.HoldI));
    if (!bOk) {Set_WaterPump_State(Drain_Err_Transmition);}
    TRY(bOk = MotorSetSpeedK(MotorPump.nAddr, MotorPump.SpeedK , MotorPump.ISensitive));
    if (!bOk) {Set_WaterPump_State(Drain_Err_Transmition);}
       // 设置排水泵电机参数
    TRY(bOk = MotorSetConfig(MotorPump.nAddr, MotorPump.nCtrlWord, MotorPump.nTimeoutSteps));
    if (!bOk) {Set_WaterPump_State(Drain_Err_Transmition);}


}
void ENABLE_PUMP(u8 Channel)
{
    switch(Channel)
    {
      case 0:GPIO_SetBits(GPIOB,GPIO_Pin_14);break;
      case 1:
        {
          WaterPump_FLAG = 1;
          
          if(Pump_FLAG == 0)
          {
            SendCommand1(NULL,"KI2");
            Pump_FLAG = 1;
          }
          break;
        }//SendCommand1(NULL,"KI0");break;//GPIO_SetBits(GPIOB,GPIO_Pin_13);break;
      case 2:break;
    }
}

void DISABLE_PUMP(u8 Channel)
{
    
    switch(Channel)
    {
      case 0:GPIO_ResetBits(GPIOB,GPIO_Pin_14);break;
      case 1:
        {
          WaterPump_FLAG = 0;
          if(Pump_FLAG == 1)
          {
            SendCommand1(NULL,"KF");
//            SendCommand1(NULL,"KI0");
            Pump_FLAG = 0;
//            SendCommand1(NULL,"KF");
          }
          break;//SendCommand1(NULL,"KI0");break;//GPIO_ResetBits(GPIOB,GPIO_Pin_13);break;
        }
      case 2:break;
    }
}
void HandleWaterPump(void)
{
    u8 Channel;
    u8 ONOFF[3];
    u16 time[3];
    for (Channel=0;Channel<3;Channel++)
    {
        GetWaterPumpParam(Channel,&ONOFF[Channel],&time[Channel]);
        if (ONOFF[Channel])
        {
          if (Channel == 1)
          {
            ONOFF[Channel]=0;
            
            WaterPump_FLAG = 1;
          
            //if(Pump_FLAG == 0)
            //{
              u32 steps;
              steps = time[Channel]*MotorPump.nSpeedMax/10;
              SendCommand1(NULL,"KV%08X",steps);
              time[Channel]=0;
              //Pump_FLAG = 1;
            //}
          }
          else
          {
            if (time[Channel]>0)
              time[Channel]--;
            else
              ONOFF[Channel]=0;
            ENABLE_PUMP(Channel);
          }
        }
        else
        {
            time[Channel]=0;
            if (Channel!= 1)
              DISABLE_PUMP(Channel);
        }
        SetWaterPumpParam(Channel,ONOFF[Channel],time[Channel]);
    }
}

          