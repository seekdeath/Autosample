#include <MotorRoll.h>
//#include <MotionErr.h>
#include <Motor.h>
#include <stm32f10x_type.h>
#include <includes.h>
#include <Motortrasmition.h>
void InitRollMotor(void)
{
     u8	bOk = 1;
    
     //  设置收带电机参数
     TRY(bOk = MotorSetConfig(MotorRoll.nAddr, MotorRoll.nCtrlWord, MotorRoll.nTimeoutSteps));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     //  设置收带电机频率
     TRY(bOk = MotorSetHighSpeed(MotorRoll.nAddr, MotorRoll.nSpeedMin,MotorRoll.nSpeedMax,MotorRoll.nSpeedStep));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     
     // 设置电流
     TRY(bOk = MotorSetSpeedLevel(MotorRoll.nAddr, MotorRoll.LowspeedI,MotorRoll.HighspeedI,MotorRoll.HoldI));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(MotorRoll.nAddr, MotorRoll.SpeedK , MotorRoll.ISensitive));
     if (!bOk) {Set_Hook_State(Roll_Err_Transmition);}
     // 设置电流

}
