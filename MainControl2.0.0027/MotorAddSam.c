#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorAddSam.h>
void resetAdd1Motor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorAddS1.nAddr));
    if (!bOk) 
    {
        Set_Add1_State(Add1_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状态
        TRY(bOk = MotorGetStatus(motorAddS1.nAddr, &status));
        if (!bOk) {Set_Add1_State(Add1_Err_Transmition);return;}
        if (status == 0x0000) 
        {
            add1ResetFlag = 1;
            break;
        }
            //原点失败
        else if (status >0x8000)  {Set_Add1_State(Add1_Err_Origin);return;}
            //是否取消
        if (IS_Add1_Cancel_FLAG())
        {
            TRY(MotorCancel(motorAddS1.nAddr));
            Reset_Add1_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Add1_State(Add1_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Add1SetPos(void)
{
	u8	bOk = 1,Add1FLAG;
	u16	status;
	s32	Pos;
    
    GetAdd1Pos(&Add1FLAG,&Pos);
    if (Add1FLAG == 0x81 || Add1FLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorAddS1.nAddr, Pos));
        if (!bOk) {Set_Add1_State(Add1_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorAddS1.nAddr, &status));
            if (!bOk) {Set_Add1_State(Add1_Err_Transmition);return;}
            if (status == 0x0000) 
            {
                add1ArriveFlag = 1;
                break;
            }
            //原点失败
            else if (status >0x8000)  {Set_Add1_State(Add1_Err_Origin);return;}
            //是否取消
            if (IS_Add1_Cancel_FLAG())
            {
                TRY(MotorCancel(motorAddS1.nAddr));
                Reset_Add1_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Add1_State(Add1_Idle);
}


void InitAdd1Motor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorAddS1.nAddr, motorAddS1.nCtrlWord, motorAddS1.nTimeoutSteps));
    if (!bOk) {Set_Add1_State(Add1_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorAddS1.nAddr, motorAddS1.nSpeedMin, motorAddS1.nSpeedMax,motorAddS1.nSpeedStep));
    if (!bOk) {Set_Add1_State(Add1_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorAddS1.nAddr, motorAddS1.LowspeedI,motorAddS1.HighspeedI,motorAddS1.HoldI));
     if (!bOk) {Set_Add1_State(Add1_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorAddS1.nAddr, motorAddS1.SpeedK,motorAddS1.ISensitive));
     if (!bOk) {Set_Add1_State(Add1_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleAdd1(void)
{
    if (IS_Add1_R_FLAG())
    {
        Set_Add1_State(Add1_Busy);
        RESET_Add1_R_FLAG();
        resetAdd1Motor();
    }
    if (IS_Add1_MOVE_FLAG())
    {
        Set_Add1_State(Add1_Busy);
        RESET_Add1_MOVE_FLAG();
        Add1SetPos();
    }
}

u8 Add1_Cancel_FLAG=0;
u8 IS_Add1_Cancel_FLAG(void)
{
    return Add1_Cancel_FLAG;
}
void Set_Add1_Cancel_FLAG(void)
{
    Add1_Cancel_FLAG=1;
}
void Reset_Add1_Cancel_FLAG(void)
{
    Add1_Cancel_FLAG=0;
}
//复位标志
u8 Add1_R_FLAG=0;
//复位标志有效
u8 IS_Add1_R_FLAG(void)
{
    return Add1_R_FLAG;
}
u8 GET_Add1_R_FLAG(void)
{
    return Add1_R_FLAG;
}
//设定复位标志有效
void SET_Add1_R_FLAG(void)
{
    Add1_R_FLAG=1;
}
//取消复位标志
void RESET_Add1_R_FLAG(void)
{
    Add1_R_FLAG=0;
}
//移动标志位
u8 Add1_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Add1_MOVE_FLAG(void)
{
    return Add1_MOVE_FLAG;
}
//置位移动标志位
void SET_Add1_MOVE_FLAG(void)
{
    Add1_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Add1_MOVE_FLAG(void)
{
    Add1_MOVE_FLAG=0;
}

//标志
u8 Add1_FLAG;
//坐标
s32 Add1_POS;
//获取标志，坐标
void GetAdd1Pos(u8* Add1FLAG,s32* Pos)
{
    *Add1FLAG=Add1_FLAG;
    *Pos=Add1_POS;
}
//设定标志，坐标
void SetAdd1Pos(u8 Add1FLAG,s32 Pos)
{
    Add1_FLAG=Add1FLAG;
    Add1_POS=Pos;
}
///////////////////AddS2
void resetAdd2Motor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorAddS2.nAddr));
    if (!bOk) 
    {
        Set_Add2_State(Add2_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状态
        TRY(bOk = MotorGetStatus(motorAddS2.nAddr, &status));
        if (!bOk) {Set_Add2_State(Add2_Err_Transmition);return;}
        if (status == 0x0000) 
        {
            add2ResetFlag = 1;
            break;
        }
            //原点失败
        else if (status >0x8000)  {Set_Add2_State(Add2_Err_Origin);return;}
            //是否取消
        if (IS_Add2_Cancel_FLAG())
        {
            TRY(MotorCancel(motorAddS2.nAddr));
            Reset_Add2_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Add2_State(Add2_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Add2SetPos(void)
{
	u8	bOk = 1,Add2FLAG;
	u16	status;
	s32	Pos;
    
    GetAdd1Pos(&Add2FLAG,&Pos);
    if (Add2FLAG == 0x81 || Add2FLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorAddS2.nAddr, Pos));
        if (!bOk) {Set_Add2_State(Add2_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorAddS2.nAddr, &status));
            if (!bOk) {Set_Add2_State(Add2_Err_Transmition);return;}
            if (status == 0x0000) 
            {
                add2ArriveFlag = 1;
                break;
            }
            //原点失败
            else if (status >0x8000)  {Set_Add2_State(Add2_Err_Origin);return;}
            //是否取消
            if (IS_Add2_Cancel_FLAG())
            {
                TRY(MotorCancel(motorAddS2.nAddr));
                Reset_Add2_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Add2_State(Add2_Idle);
}


void InitAdd2Motor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorAddS2.nAddr, motorAddS2.nCtrlWord, motorAddS2.nTimeoutSteps));
    if (!bOk) {Set_Add2_State(Add1_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorAddS2.nAddr, motorAddS2.nSpeedMin, motorAddS2.nSpeedMax,motorAddS2.nSpeedStep));
    if (!bOk) {Set_Add2_State(Add2_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorAddS2.nAddr, motorAddS2.LowspeedI,motorAddS2.HighspeedI,motorAddS2.HoldI));
     if (!bOk) {Set_Add1_State(Add2_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorAddS2.nAddr, motorAddS2.SpeedK,motorAddS2.ISensitive));
     if (!bOk) {Set_Add1_State(Add2_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleAdd2(void)
{
    if (IS_Add2_R_FLAG())
    {
        Set_Add2_State(Add1_Busy);
        RESET_Add2_R_FLAG();
        resetAdd2Motor();
    }
    if (IS_Add2_MOVE_FLAG())
    {
        Set_Add2_State(Add2_Busy);
        RESET_Add2_MOVE_FLAG();
        Add2SetPos();
    }
}

u8 Add2_Cancel_FLAG=0;
u8 IS_Add2_Cancel_FLAG(void)
{
    return Add2_Cancel_FLAG;
}
void Set_Add2_Cancel_FLAG(void)
{
    Add2_Cancel_FLAG=1;
}
void Reset_Add2_Cancel_FLAG(void)
{
    Add2_Cancel_FLAG=0;
}
//复位标志
u8 Add2_R_FLAG=0;
u8 IS_Add2_R_FLAG(void)
{
    return Add2_R_FLAG;
}
//复位标志有效
u8 GET_Add2_R_FLAG(void)
{
    return Add2_R_FLAG;
}
//设定复位标志有效
void SET_Add2_R_FLAG(void)
{
    Add2_R_FLAG=1;
}
//取消复位标志
void RESET_Add2_R_FLAG(void)
{
    Add2_R_FLAG=0;
}
//移动标志位
u8 Add2_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Add2_MOVE_FLAG(void)
{
    return Add2_MOVE_FLAG;
}
//置位移动标志位
void SET_Add2_MOVE_FLAG(void)
{
    Add2_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Add2_MOVE_FLAG(void)
{
    Add2_MOVE_FLAG=0;
}

//标志
u8 Add2_FLAG;
//坐标
s32 Add2_POS;
//获取标志，坐标
void GetAdd2Pos(u8* Add2FLAG,s32* Pos)
{
    *Add2FLAG=Add2_FLAG;
    *Pos=Add2_POS;
}
//设定标志，坐标
void SetAdd2Pos(u8 Add2FLAG,s32 Pos)
{
    Add2_FLAG=Add2FLAG;
    Add2_POS=Pos;
}