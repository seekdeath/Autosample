#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
void resetAdd1Motor()
{
    u8	bOk = 1;
	u16	status;
    
        // 加样泵电机复位
	TRY(bOk = MotorReset(motorAddS1.nAddr));
        if (!bOk) 
        {
          Set_Add1_State(Add1_Err_Transmition);
          return;
        }
        
        // 试剂泵电机复位
	TRY(bOk = MotorReset(MotorSyringReagent.nAddr));
        if (!bOk) 
        {
          Set_Syringe_State(Syringe_Err_Transmition_R);
          return;
        }
        
        while  (1)//加样泵
        {
                //获取加样泵状态
                TRY(bOk = MotorGetStatus(MotorSyringSample.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
                if (status == 0x0000) break;
                //样品泵原点失败
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Origin_S);return;}
                //是否取消
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringSample.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(100);
        }
        
        while  (1)//试剂泵
        {
                //获取试剂泵状态
                TRY(bOk = MotorGetStatus(MotorSyringReagent.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
                if (status == 0x0000) break;
                //试剂泵原点失败
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Origin_R);return;}
                //是否取消
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringReagent.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
        }
        Set_Syringe_State(Syringe_Idle);
}

void InitSyringeMotor(void)
{
    u8 bOk = 1;
    // 设置加样泵电机参数
    TRY(bOk = MotorSetConfig(MotorSyringSample.nAddr, MotorSyringSample.nCtrlWord, MotorSyringSample.nTimeoutSteps));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);}
     
    // 设置加样泵电机频率
    TRY(bOk = MotorSetHighSpeed(MotorSyringSample.nAddr, MotorSyringSample.nSpeedMin, MotorSyringSample.nSpeedMax,MotorSyringSample.nSpeedStep));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);}
    
    // 设置试剂泵电机参数
    TRY(bOk = MotorSetConfig(MotorSyringReagent.nAddr, MotorSyringReagent.nCtrlWord, MotorSyringReagent.nTimeoutSteps));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);}
     
    // 设置试剂泵电机频率
    TRY(bOk = MotorSetHighSpeed(MotorSyringReagent.nAddr, MotorSyringReagent.nSpeedMin, MotorSyringReagent.nSpeedMax,MotorSyringReagent.nSpeedStep));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);}
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(MotorSyringSample.nAddr, MotorSyringSample.LowspeedI,MotorSyringSample.HighspeedI,MotorSyringSample.HoldI));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_S);}
     TRY(bOk = MotorSetSpeedK(MotorSyringSample.nAddr, MotorSyringSample.SpeedK,MotorSyringSample.ISensitive));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_S);}
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(MotorSyringReagent.nAddr, MotorSyringReagent.LowspeedI,MotorSyringReagent.HighspeedI,MotorSyringReagent.HoldI));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_R);}
     TRY(bOk = MotorSetSpeedK(MotorSyringReagent.nAddr, MotorSyringReagent.SpeedK,MotorSyringReagent.ISensitive));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_R);}
}

///----------------------------------------------------------------------------
/// 注射泵任务
///----------------------------------------------------------------------------
void HandleSyringe(void)
{
    if (IS_Syringe_R_FLAG())
    {
        Set_Syringe_State(Syringe_Busy);
        RESET_Syringe_R_FLAG();
        SyringeReset();
    }
    if (IS_Syringe_MOVE_FLAG())
    {
        Set_Syringe_State(Syringe_Busy);
        RESET_Syringe_MOVE_FLAG();
        SyringeSetPos();
    }
}

u8 Syringe_Cancel_FLAG=0;
u8 IS_Syringe_Cancel_FLAG(void)
{
    return Syringe_Cancel_FLAG;
}
void Set_Syringe_Cancel_FLAG(void)
{
    Syringe_Cancel_FLAG=1;
}
void Reset_Syringe_Cancel_FLAG(void)
{
    Syringe_Cancel_FLAG=0;
}
//泵复位标志
u8 Syringe_R_FLAG=0;
//泵复位标志有效
u8 IS_Syringe_R_FLAG(void)
{
    return Syringe_R_FLAG;
}
//设定泵复位标志有效
void SET_Syringe_R_FLAG(void)
{
    Syringe_R_FLAG=1;
}
//取消泵复位标志
void RESET_Syringe_R_FLAG(void)
{
    Syringe_R_FLAG=0;
}
//泵移动标志位
u8 Syringe_MOVE_FLAG=0;
//泵移动标志位有效？
u8 IS_Syringe_MOVE_FLAG(void)
{
    return Syringe_MOVE_FLAG;
}
//置位泵移动标志位
void SET_Syringe_MOVE_FLAG(void)
{
    Syringe_MOVE_FLAG=1;
}
//复位泵移动标志位
void RESET_Syringe_MOVE_FLAG(void)
{
    Syringe_MOVE_FLAG=0;
}
u16 Add1_Per;
//泵的标志
u8 Add1_FLAG;
//采样泵坐标，试剂泵坐标
s32 Add1_POS;
//获取标志，坐标
void GetSyringePos(u8* SyringeFLAG,s32* Pos, u16 *Per)
{
    *SyringeFLAG=Syringe_FLAG;
    *Pos=Syringe_POS;
    *Per=Syringe_Per;
}
//设定泵标志，坐标
void SetAdd1Pos(u8 SyringeFLAG,s32 Pos,u16 Per)
{
    Add1_FLAG=SyringeFLAG;
    Add1_POS=Pos;
    Add1_Per = Per;
}