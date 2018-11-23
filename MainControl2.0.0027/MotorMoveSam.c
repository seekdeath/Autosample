#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorMoveSam.h>
void resetMove1Motor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorMoveS1.nAddr));
    if (!bOk) 
    {
        Set_Move1_State(Move1_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状态
        TRY(bOk = MotorGetStatus(motorMoveS1.nAddr, &status));
        if (!bOk) {Set_Move1_State(Move1_Err_Transmition);return;}
        if (status == 0x0000) break;
            //原点失败
        else if (status >0x8000)  {Set_Move1_State(Move1_Err_Origin);return;}
            //是否取消
        if (IS_Move1_Cancel_FLAG())
        {
            TRY(MotorCancel(motorMoveS1.nAddr));
            Reset_Move1_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Move1_State(Move1_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move1SetPos(void)
{
	u8	bOk = 1,Move1FLAG;
	u16	status;
	s32	Pos;
    
    GetMove1Pos(&Move1FLAG,&Pos);
    if (Move1FLAG == 0x81 || Move1FLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorMoveS1.nAddr, Pos));
        if (!bOk) {Set_Move1_State(Move1_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorMoveS1.nAddr, &status));
            if (!bOk) {Set_Move1_State(Move1_Err_Transmition);return;}
            if (status == 0x0000) break;
            //原点失败
            else if (status >0x8000)  {Set_Move1_State(Move1_Err_Origin);return;}
            //是否取消
            if (IS_Move1_Cancel_FLAG())
            {
                TRY(MotorCancel(motorMoveS1.nAddr));
                Reset_Move1_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Move1_State(Move1_Idle);
}


void InitMove1Motor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorMoveS1.nAddr, motorMoveS1.nCtrlWord, motorMoveS1.nTimeoutSteps));
    if (!bOk) {Set_Move1_State(Move1_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorMoveS1.nAddr, motorMoveS1.nSpeedMin, motorMoveS1.nSpeedMax,motorMoveS1.nSpeedStep));
    if (!bOk) {Set_Move1_State(Move1_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorMoveS1.nAddr, motorMoveS1.LowspeedI,motorMoveS1.HighspeedI,motorMoveS1.HoldI));
     if (!bOk) {Set_Move1_State(Move1_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorMoveS1.nAddr, motorMoveS1.SpeedK,motorMoveS1.ISensitive));
     if (!bOk) {Set_Move1_State(Move1_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove1(void)
{
    if (IS_Move1_R_FLAG())
    {
        Set_Move1_State(Move1_Busy);
        RESET_Move1_R_FLAG();
        Move1Reset();
    }
    if (IS_Move1_MOVE_FLAG())
    {
        Set_Move1_State(Move1_Busy);
        RESET_Move1_MOVE_FLAG();
        Move1SetPos();
    }
}

u8 Move1_Cancel_FLAG=0;
u8 IS_Move1_Cancel_FLAG(void)
{
    return Move1_Cancel_FLAG;
}
void Set_Move1_Cancel_FLAG(void)
{
    Move1_Cancel_FLAG=1;
}
void Reset_Move1_Cancel_FLAG(void)
{
    Move1_Cancel_FLAG=0;
}
//复位标志
u8 Move1_R_FLAG=0;
//复位标志有效
u8 GET_Move_R_FLAG(void)
{
    return Move1_R_FLAG;
}
//设定复位标志有效
void SET_Move1_R_FLAG(void)
{
    Move1_R_FLAG=1;
}
//取消复位标志
void RESET_Move1_R_FLAG(void)
{
    Move1_R_FLAG=0;
}
//移动标志位
u8 Move1_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move1_MOVE_FLAG(void)
{
    return Move1_MOVE_FLAG;
}
//置位移动标志位
void SET_Move1_MOVE_FLAG(void)
{
    Move1_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Move1_MOVE_FLAG(void)
{
    Move1_MOVE_FLAG=0;
}

//标志
u8 Move1_FLAG;
//坐标
s32 Move1_POS;
//获取标志，坐标
void GetMove1Pos(u8* Move1FLAG,s32* Pos)
{
    *Move1FLAG=Move1_FLAG;
    *Pos=Move1_POS;
}
//设定标志，坐标
void SetMove1Pos(u8 Move1FLAG,s32 Pos)
{
    Move1_FLAG=Move1FLAG;
    Move1_POS=Pos;
}
////////////////////Move2
void resetMove2Motor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorMoveS2.nAddr));
    if (!bOk) 
    {
        Set_Move2_State(Move2_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状态
        TRY(bOk = MotorGetStatus(motorMoveS2.nAddr, &status));
        if (!bOk) {Set_Move2_State(Move2_Err_Transmition);return;}
        if (status == 0x0000) break;
            //原点失败
        else if (status >0x8000)  {Set_Move2_State(Move2_Err_Origin);return;}
            //是否取消
        if (IS_Move2_Cancel_FLAG())
        {
            TRY(MotorCancel(motorMoveS2.nAddr));
            Reset_Move2_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Move2_State(Move2_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move2SetPos(void)
{
	u8	bOk = 1,Move2FLAG;
	u16	status;
	s32	Pos;
    
    GetMove2Pos(&Move2FLAG,&Pos);
    if (Move2FLAG == 0x81 || Move2FLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorMoveS2.nAddr, Pos));
        if (!bOk) {Set_Move2_State(Move2_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorMoveS2.nAddr, &status));
            if (!bOk) {Set_Move2_State(Move2_Err_Transmition);return;}
            if (status == 0x0000) break;
            //原点失败
            else if (status >0x8000)  {Set_Move2_State(Move2_Err_Origin);return;}
            //是否取消
            if (IS_Move2_Cancel_FLAG())
            {
                TRY(MotorCancel(motorMoveS2.nAddr));
                Reset_Move2_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Move2_State(Move2_Idle);
}


void InitMove2Motor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorMoveS2.nAddr, motorMoveS2.nCtrlWord, motorMoveS2.nTimeoutSteps));
    if (!bOk) {Set_Move2_State(Move2_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorMoveS2.nAddr, motorMoveS2.nSpeedMin, motorMoveS2.nSpeedMax,motorMoveS2.nSpeedStep));
    if (!bOk) {Set_Move2_State(Move2_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorMoveS2.nAddr, motorMoveS2.LowspeedI,motorMoveS2.HighspeedI,motorMoveS2.HoldI));
     if (!bOk) {Set_Move2_State(Move2_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorMoveS2.nAddr, motorMoveS2.SpeedK,motorMoveS2.ISensitive));
     if (!bOk) {Set_Move2_State(Move2_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove2(void)
{
    if (IS_Move2_R_FLAG())
    {
        Set_Move2_State(Move2_Busy);
        RESET_Move2_R_FLAG();
        Move1Reset();
    }
    if (IS_Move2_MOVE_FLAG())
    {
        Set_Move2_State(Move2_Busy);
        RESET_Move2_MOVE_FLAG();
        Move2SetPos();
    }
}

u8 Move2_Cancel_FLAG=0;
u8 IS_Move2_Cancel_FLAG(void)
{
    return Move2_Cancel_FLAG;
}
void Set_Move2_Cancel_FLAG(void)
{
    Move2_Cancel_FLAG=1;
}
void Reset_Move2_Cancel_FLAG(void)
{
    Move2_Cancel_FLAG=0;
}
//复位标志
u8 Move2_R_FLAG=0;
//复位标志有效
u8 GET_Move2_R_FLAG(void)
{
    return Move2_R_FLAG;
}
//设定复位标志有效
void SET_Move2_R_FLAG(void)
{
    Move2_R_FLAG=1;
}
//取消复位标志
void RESET_Move2_R_FLAG(void)
{
    Move2_R_FLAG=0;
}
//移动标志位
u8 Move2_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move2_MOVE_FLAG(void)
{
    return Move2_MOVE_FLAG;
}
//置位移动标志位
void SET_Move2_MOVE_FLAG(void)
{
    Move2_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Move2_MOVE_FLAG(void)
{
    Move2_MOVE_FLAG=0;
}

//标志
u8 Move2_FLAG;
//坐标
s32 Move2_POS;
//获取标志，坐标
void GetMove2Pos(u8* Move2FLAG,s32* Pos)
{
    *Move2FLAG=Move2_FLAG;
    *Pos=Move2_POS;
}
//设定标志，坐标
void SetMove2Pos(u8 Move2FLAG,s32 Pos)
{
    Move2_FLAG=Move2FLAG;
    Move2_POS=Pos;
}
////////////////////Move3
void resetMove3Motor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorMoveS3.nAddr));
    if (!bOk) 
    {
        Set_Move3_State(Move3_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状态
        TRY(bOk = MotorGetStatus(motorMoveS3.nAddr, &status));
        if (!bOk) {Set_Move3_State(Move3_Err_Transmition);return;}
        if (status == 0x0000) break;
            //原点失败
        else if (status >0x8000)  {Set_Move3_State(Move3_Err_Origin);return;}
            //是否取消
        if (IS_Move3_Cancel_FLAG())
        {
            TRY(MotorCancel(motorMoveS3.nAddr));
            Reset_Move3_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Move3_State(Move3_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move3SetPos(void)
{
	u8	bOk = 1,Move3FLAG;
	u16	status;
	s32	Pos;
    
    GetMove3Pos(&Move3FLAG,&Pos);
    if (Move3FLAG == 0x81 || Move3FLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorMoveS3.nAddr, Pos));
        if (!bOk) {Set_Move3_State(Move3_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorMoveS3.nAddr, &status));
            if (!bOk) {Set_Move3_State(Move3_Err_Transmition);return;}
            if (status == 0x0000) break;
            //原点失败
            else if (status >0x8000)  {Set_Move3_State(Move3_Err_Origin);return;}
            //是否取消
            if (IS_Move3_Cancel_FLAG())
            {
                TRY(MotorCancel(motorMoveS3.nAddr));
                Reset_Move3_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Move3_State(Move3_Idle);
}


void InitMove3Motor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorMoveS3.nAddr, motorMoveS3.nCtrlWord, motorMoveS3.nTimeoutSteps));
    if (!bOk) {Set_Move3_State(Move3_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorMoveS3.nAddr, motorMoveS3.nSpeedMin, motorMoveS3.nSpeedMax,motorMoveS3.nSpeedStep));
    if (!bOk) {Set_Move3_State(Move3_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorMoveS3.nAddr, motorMoveS3.LowspeedI,motorMoveS3.HighspeedI,motorMoveS3.HoldI));
     if (!bOk) {Set_Move2_State(Move2_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorMoveS3.nAddr, motorMoveS3.SpeedK,motorMoveS3.ISensitive));
     if (!bOk) {Set_Move3_State(Move3_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove3(void)
{
    if (IS_Move3_R_FLAG())
    {
        Set_Move3_State(Move2_Busy);
        RESET_Move3_R_FLAG();
        Move1Reset();
    }
    if (IS_Move3_MOVE_FLAG())
    {
        Set_Move3_State(Move2_Busy);
        RESET_Move3_MOVE_FLAG();
        Move3SetPos();
    }
}

u8 Move3_Cancel_FLAG=0;
u8 IS_Move3_Cancel_FLAG(void)
{
    return Move3_Cancel_FLAG;
}
void Set_Move3_Cancel_FLAG(void)
{
    Move3_Cancel_FLAG=1;
}
void Reset_Move3_Cancel_FLAG(void)
{
    Move3_Cancel_FLAG=0;
}
//复位标志
u8 Move3_R_FLAG=0;
//复位标志有效
u8 GET_Move3_R_FLAG(void)
{
    return Move3_R_FLAG;
}
//设定复位标志有效
void SET_Move3_R_FLAG(void)
{
    Move3_R_FLAG=1;
}
//取消复位标志
void RESET_Move3_R_FLAG(void)
{
    Move3_R_FLAG=0;
}
//移动标志位
u8 Move3_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move3_MOVE_FLAG(void)
{
    return Move3_MOVE_FLAG;
}
//置位移动标志位
void SET_Move3_MOVE_FLAG(void)
{
    Move3_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Move3_MOVE_FLAG(void)
{
    Move3_MOVE_FLAG=0;
}

//标志
u8 Move3_FLAG;
//坐标
s32 Move3_POS;
//获取标志，坐标
void GetMove3Pos(u8* Move3FLAG,s32* Pos)
{
    *Move3FLAG=Move3_FLAG;
    *Pos=Move3_POS;
}
//设定标志，坐标
void SetMove3Pos(u8 Move3FLAG,s32 Pos)
{
    Move3_FLAG=Move3FLAG;
    Move3_POS=Pos;
}
////////////////////Move4
void resetMove4Motor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorMoveS4.nAddr));
    if (!bOk) 
    {
        Set_Move4_State(Move4_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状态
        TRY(bOk = MotorGetStatus(motorMoveS4.nAddr, &status));
        if (!bOk) {Set_Move4_State(Move4_Err_Transmition);return;}
        if (status == 0x0000) break;
            //原点失败
        else if (status >0x8000)  {Set_Move4_State(Move4_Err_Origin);return;}
            //是否取消
        if (IS_Move4_Cancel_FLAG())
        {
            TRY(MotorCancel(motorMoveS4.nAddr));
            Reset_Move4_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Move4_State(Move4_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void Move4SetPos(void)
{
	u8	bOk = 1,Move4FLAG;
	u16	status;
	s32	Pos;
    
    GetMove4Pos(&Move4FLAG,&Pos);
    if (Move4FLAG == 0x81 || Move4FLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorMoveS4.nAddr, Pos));
        if (!bOk) {Set_Move4_State(Move4_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorMoveS4.nAddr, &status));
            if (!bOk) {Set_Move4_State(Move4_Err_Transmition);return;}
            if (status == 0x0000) break;
            //原点失败
            else if (status >0x8000)  {Set_Move4_State(Move4_Err_Origin);return;}
            //是否取消
            if (IS_Move4_Cancel_FLAG())
            {
                TRY(MotorCancel(motorMoveS4.nAddr));
                Reset_Move4_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Move4_State(Move4_Idle);
}


void InitMove4Motor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorMoveS4.nAddr, motorMoveS4.nCtrlWord, motorMoveS4.nTimeoutSteps));
    if (!bOk) {Set_Move4_State(Move4_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorMoveS4.nAddr, motorMoveS4.nSpeedMin, motorMoveS4.nSpeedMax,motorMoveS4.nSpeedStep));
    if (!bOk) {Set_Move4_State(Move4_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorMoveS4.nAddr, motorMoveS4.LowspeedI,motorMoveS4.HighspeedI,motorMoveS4.HoldI));
     if (!bOk) {Set_Move2_State(Move2_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorMoveS4.nAddr, motorMoveS4.SpeedK,motorMoveS4.ISensitive));
     if (!bOk) {Set_Move4_State(Move4_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleMove4(void)
{
    if (IS_Move4_R_FLAG())
    {
        Set_Move4_State(Move4_Busy);
        RESET_Move4_R_FLAG();
        Move1Reset();
    }
    if (IS_Move4_MOVE_FLAG())
    {
        Set_Move4_State(Move4_Busy);
        RESET_Move4_MOVE_FLAG();
        Move4SetPos();
    }
}

u8 Move4_Cancel_FLAG=0;
u8 IS_Move4_Cancel_FLAG(void)
{
    return Move4_Cancel_FLAG;
}
void Set_Move4_Cancel_FLAG(void)
{
    Move4_Cancel_FLAG=1;
}
void Reset_Move4_Cancel_FLAG(void)
{
    Move4_Cancel_FLAG=0;
}
//复位标志
u8 Move4_R_FLAG=0;
//复位标志有效
u8 GET_Move4_R_FLAG(void)
{
    return Move4_R_FLAG;
}
//设定复位标志有效
void SET_Move4_R_FLAG(void)
{
    Move4_R_FLAG=1;
}
//取消复位标志
void RESET_Move4_R_FLAG(void)
{
    Move4_R_FLAG=0;
}
//移动标志位
u8 Move4_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move4_MOVE_FLAG(void)
{
    return Move4_MOVE_FLAG;
}
//置位移动标志位
void SET_Move4_MOVE_FLAG(void)
{
    Move4_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Move4_MOVE_FLAG(void)
{
    Move4_MOVE_FLAG=0;
}

//标志
u8 Move4_FLAG;
//坐标
s32 Move4_POS;
//获取标志，坐标
void GetMove4Pos(u8* Move4FLAG,s32* Pos)
{
    *Move4FLAG=Move4_FLAG;
    *Pos=Move4_POS;
}
//设定标志，坐标
void SetMove4Pos(u8 Move4FLAG,s32 Pos)
{
    Move4_FLAG=Move4FLAG;
    Move4_POS=Pos;
}