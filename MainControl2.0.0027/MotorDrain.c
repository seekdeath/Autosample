#include <includes.h>
#include <MotionErr.h>
#include <MotorDrain.h>
#include <MotorTrasmition.h>
#include <Motor.h>

void Drain(void)
{
    u16 time, num=0;
    u8	bOk = 1;
    u16	status;
     
    //获取排水泵状态
    TRY(bOk = MotorGetStatus(MotorDrain.nAddr, &status));
    //通信失败
    if (!bOk) {Set_Drain_State(Drain_Err_Transmition);return;}
      
    if (status >0x8000) 
    {
        Set_Drain_State(Drain_Err_Transmition);
        return;
    }
    
     //读取排水时间
    GetDrainTime(&time);
    TRY(MotorRun(MotorDrain.nAddr,1));
   
    while(1)
    {
        if (IS_Drain_Cancel())
        {
          TRY(MotorRun(MotorDrain.nAddr,0));
          Reset_Drain_Cancel();
          break;
        }
        OSTimeDly(100);
        num++;
        if (num==time)
        {
          TRY(MotorRun(MotorDrain.nAddr,0));
          //Set_Drain_State(Drain_Err_Transmition);
          break;
        }
    }

    Set_Drain_State(Drain_Idle);
}

void InitDrainMotor(void)
{
  u8 bOk = 1;
  
  // 设置排水泵电机参数
  TRY(bOk = MotorSetConfig(MotorDrain.nAddr, MotorDrain.nCtrlWord, MotorDrain.nTimeoutSteps));
  if (!bOk) {Set_Drain_State(Drain_Err_Transmition);}
  // 设置排水泵电机频率
  TRY(bOk = MotorSetSpeed(MotorDrain.nAddr, MotorDrain.nSpeedMin, MotorDrain.nSpeedMax,MotorDrain.nSpeedStep));
  if (!bOk) {Set_Drain_State(Drain_Err_Transmition);}
     
}

void HandleDrain(void)
{
    if (IS_Drain_FLAG())
    {
        Reset_Drain_FLAG();
        Set_Drain_State(Drain_Busy);
        Drain();
    }
}


//排水组件状态
u8 Drain_FLAG=0;
//判断排水组件标志
u8 IS_Drain_FLAG(void)
{
    return Drain_FLAG;
}
//置位排水组件标志
void Set_Drain_FLAG(void)
{
    Drain_FLAG=1;
}
//复位排水组件标志
void Reset_Drain_FLAG(void)
{
    Drain_FLAG=0;
}

u8 Drain_Cancel=0;
u8 IS_Drain_Cancel(void)
{
    return Drain_Cancel;
}
void Set_Drain_Cancel(void)
{
    Drain_Cancel=1;
}
void Reset_Drain_Cancel(void)
{
    Drain_Cancel=0;
}


u16 Drain_Time=0;
void GetDrainTime(u16* time)
{
    *time=Drain_Time;
}
void SetDrainTime(u16 time)
{
    Drain_Time=time;
}