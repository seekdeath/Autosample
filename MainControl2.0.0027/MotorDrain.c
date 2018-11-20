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
     
    //��ȡ��ˮ��״̬
    TRY(bOk = MotorGetStatus(MotorDrain.nAddr, &status));
    //ͨ��ʧ��
    if (!bOk) {Set_Drain_State(Drain_Err_Transmition);return;}
      
    if (status >0x8000) 
    {
        Set_Drain_State(Drain_Err_Transmition);
        return;
    }
    
     //��ȡ��ˮʱ��
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
  
  // ������ˮ�õ������
  TRY(bOk = MotorSetConfig(MotorDrain.nAddr, MotorDrain.nCtrlWord, MotorDrain.nTimeoutSteps));
  if (!bOk) {Set_Drain_State(Drain_Err_Transmition);}
  // ������ˮ�õ��Ƶ��
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


//��ˮ���״̬
u8 Drain_FLAG=0;
//�ж���ˮ�����־
u8 IS_Drain_FLAG(void)
{
    return Drain_FLAG;
}
//��λ��ˮ�����־
void Set_Drain_FLAG(void)
{
    Drain_FLAG=1;
}
//��λ��ˮ�����־
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