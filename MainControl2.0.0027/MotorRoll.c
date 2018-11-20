#include <MotorRoll.h>
//#include <MotionErr.h>
#include <Motor.h>
#include <stm32f10x_type.h>
#include <includes.h>
#include <Motortrasmition.h>
void InitRollMotor(void)
{
     u8	bOk = 1;
    
     //  �����մ��������
     TRY(bOk = MotorSetConfig(MotorRoll.nAddr, MotorRoll.nCtrlWord, MotorRoll.nTimeoutSteps));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     //  �����մ����Ƶ��
     TRY(bOk = MotorSetHighSpeed(MotorRoll.nAddr, MotorRoll.nSpeedMin,MotorRoll.nSpeedMax,MotorRoll.nSpeedStep));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     
     // ���õ���
     TRY(bOk = MotorSetSpeedLevel(MotorRoll.nAddr, MotorRoll.LowspeedI,MotorRoll.HighspeedI,MotorRoll.HoldI));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(MotorRoll.nAddr, MotorRoll.SpeedK , MotorRoll.ISensitive));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     // ���õ���

}
