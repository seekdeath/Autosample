#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorAddSam.h>
void resetAddMotor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorAddS.nAddr));
    if (!bOk) 
    {
        Set_Add_State(Add_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状态
        TRY(bOk = MotorGetStatus(motorAddS.nAddr, &status));
        if (!bOk) {Set_Add_State(Add_Err_Transmition);return;}
        if (status == 0x0000) 
        {
            addResetFlag = 1;
            break;
        }
            //原点失败
        else if (status >0x8000)  {Set_Add_State(Add_Err_Origin);return;}
            //是否取消
        if (IS_Add_Cancel_FLAG())
        {
            TRY(MotorCancel(motorAddS.nAddr));
            Reset_Add_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Add_State(Add_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void AddSetPos(void)
{
	u8	bOk = 1,AddFLAG;
	u16	status;
	s32	Pos;
    
    GetAddPos(&AddFLAG,&Pos);
    if (AddFLAG == 0x81 || AddFLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorAddS.nAddr, Pos));
        if (!bOk) {Set_Add_State(Add_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorAddS.nAddr, &status));
            if (!bOk) {Set_Add_State(Add_Err_Transmition);return;}
            if (status == 0x0000) 
            {
                addArriveFlag = 1;
                addRunFlag = 0;
                break;
            }
            //原点失败
            else if (status >0x8000)  {Set_Add_State(Add_Err_Origin);return;}
            //是否取消
            if (IS_Add_Cancel_FLAG())
            {
                TRY(MotorCancel(motorAddS.nAddr));
                Reset_Add_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Add_State(Add_Idle);
}


void InitAddMotor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorAddS.nAddr, motorAddS.nCtrlWord, motorAddS.nTimeoutSteps));
    if (!bOk) {Set_Add_State(Add_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorAddS.nAddr, motorAddS.nSpeedMin, motorAddS.nSpeedMax,motorAddS.nSpeedStep));
    if (!bOk) {Set_Add_State(Add_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorAddS.nAddr, motorAddS.LowspeedI,motorAddS.HighspeedI,motorAddS.HoldI));
     if (!bOk) {Set_Add_State(Add_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorAddS.nAddr, motorAddS.SpeedK,motorAddS.ISensitive));
     if (!bOk) {Set_Add_State(Add_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleAdd(void)
{
    if (IS_Add_R_FLAG())
    {
        Set_Add_State(Add_Busy);
        RESET_Add_R_FLAG();
        resetAddMotor();
    }
    if (IS_Add_MOVE_FLAG())
    {
        Set_Add_State(Add_Busy);
        RESET_Add_MOVE_FLAG();
        AddSetPos();
    }
}

u8 Add_Cancel_FLAG=0;
u8 IS_Add_Cancel_FLAG(void)
{
    return Add_Cancel_FLAG;
}
void Set_Add_Cancel_FLAG(void)
{
    Add_Cancel_FLAG=1;
}
void Reset_Add_Cancel_FLAG(void)
{
    Add_Cancel_FLAG=0;
}
//复位标志
u8 Add_R_FLAG=0;
//复位标志有效
u8 IS_Add_R_FLAG(void)
{
    return Add_R_FLAG;
}
u8 GET_Add_R_FLAG(void)
{
    return Add_R_FLAG;
}
//设定复位标志有效
void SET_Add_R_FLAG(void)
{
    Add_R_FLAG=1;
}
//取消复位标志
void RESET_Add_R_FLAG(void)
{
    Add_R_FLAG=0;
}
//移动标志位
u8 Add_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Add_MOVE_FLAG(void)
{
    return Add_MOVE_FLAG;
}
//置位移动标志位
void SET_Add_MOVE_FLAG(void)
{
    Add_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Add_MOVE_FLAG(void)
{
    Add_MOVE_FLAG=0;
}

//标志
u8 Add_FLAG;
//坐标
s32 Add_POS;
//获取标志，坐标
void GetAddPos(u8* AddFLAG,s32* Pos)
{
    *AddFLAG=Add_FLAG;
    *Pos=Add_POS;
}
//设定标志，坐标
void SetAddPos(u8 AddFLAG,s32 Pos)
{
    Add_FLAG=AddFLAG;
    Add_POS=Pos;
}
