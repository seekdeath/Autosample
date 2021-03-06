#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorSubSam.h>
void resetSubMotor()
{
    u8	bOk = 1;
	u16	status;
    
        // 电机复位
	TRY(bOk = MotorReset(motorSubS.nAddr));
    if (!bOk) 
    {
        Set_Sub_State(Sub_Err_Transmition);
        return;
    }
    
    while  (1)//
    {
            //获取状�?        
            TRY(bOk = MotorGetStatus(motorSubS.nAddr, &status));
        if (!bOk) {Set_Sub_State(Sub_Err_Transmition);return;}
        if (status == 0x0000) 
        {
            subResetFlag = 1;
            break;
        }
            //原点失败
        else if (status >0x8000)  {Set_Sub_State(Sub_Err_Origin);return;}
            //是否取消
        if (IS_Sub_Cancel_FLAG())
        {
            TRY(MotorCancel(motorSubS.nAddr));
            Reset_Sub_Cancel_FLAG();
            return;
        }
        OSTimeDly(100);
    }
    Set_Sub_State(Sub_Idle);
}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void SubSetPos(void)
{
	u8	bOk = 1,SubFLAG;
	u16	status;
	s32	Pos;
    
    GetSubPos(&SubFLAG,&Pos);
    if (SubFLAG == 0x81 || SubFLAG == 0x01)//判断Submotor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorSubS.nAddr, Pos));
        if (!bOk) {Set_Sub_State(Sub_Err_Transmition);return;}
        while  (1)//样品�?        
        {
            //获取状�?            
            TRY(bOk = MotorGetStatus(motorSubS.nAddr, &status));
            if (!bOk) {Set_Sub_State(Sub_Err_Transmition);return;}
            if (status == 0x0000) 
            {
                subArriveFlag = 1;
                break;
            }
            //原点失败
            else if (status >0x8000)  {Set_Sub_State(Sub_Err_Origin);return;}
            //是否取消
            if (IS_Sub_Cancel_FLAG())
            {
                TRY(MotorCancel(motorSubS.nAddr));
                Reset_Sub_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Sub_State(Sub_Idle);
}


void InitSubMotor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorSubS.nAddr, motorSubS.nCtrlWord, motorSubS.nTimeoutSteps));
    if (!bOk) {Set_Sub_State(Sub_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorSubS.nAddr, motorSubS.nSpeedMin, motorSubS.nSpeedMax,motorSubS.nSpeedStep));
    if (!bOk) {Set_Sub_State(Sub_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorSubS.nAddr, motorSubS.LowspeedI,motorSubS.HighspeedI,motorSubS.HoldI));
     if (!bOk) {Set_Sub_State(Sub_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorSubS.nAddr, motorSubS.SpeedK,motorSubS.ISensitive));
     if (!bOk) {Set_Sub_State(Sub_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleSub(void)
{
    if (IS_Sub_R_FLAG())
    {
        Set_Sub_State(Sub_Busy);
        RESET_Sub_R_FLAG();
        resetSubMotor();
    }
    if (IS_Sub_MOVE_FLAG())
    {
        Set_Sub_State(Sub_Busy);
        RESET_Sub_MOVE_FLAG();
        SubSetPos();
    }
}

u8 Sub_Cancel_FLAG=0;
u8 IS_Sub_Cancel_FLAG(void)
{
    return Sub_Cancel_FLAG;
}
void Set_Sub_Cancel_FLAG(void)
{
    Sub_Cancel_FLAG=1;
}
void Reset_Sub_Cancel_FLAG(void)
{
    Sub_Cancel_FLAG=0;
}
//复位标志
u8 Sub_R_FLAG=0;
u8 IS_Sub_R_FLAG(void)
{
    return Sub_R_FLAG;
}
//复位标志有效
u8 GET_Sub_R_FLAG(void)
{
    return Sub_R_FLAG;
}
//设定复位标志有效
void SET_Sub_R_FLAG(void)
{
    Sub_R_FLAG=1;
}
//取消复位标志

void RESET_Sub_R_FLAG(void)
{
    Sub_R_FLAG=0;
}
//移动标志�?
u8 Sub_MOVE_FLAG=0;
//移动标志位有效？

u8 IS_Sub_MOVE_FLAG(void)
{
    return Sub_MOVE_FLAG;
}
//置位移动标志�?
void SET_Sub_MOVE_FLAG(void)
{
    Sub_MOVE_FLAG=1;
}
//复位移动标志�?
void RESET_Sub_MOVE_FLAG(void)
{
    Sub_MOVE_FLAG=0;
}

//标志
u8 Sub_FLAG;
//坐标
s32 Sub_POS;
//获取标志，坐�?
void GetSubPos(u8* SubFLAG,s32* Pos)
{
    *SubFLAG=Sub_FLAG;
    *Pos=Sub_POS;
}
//设定标志，坐�?
void SetSubPos(u8 SubFLAG,s32 Pos)
{
    Sub_FLAG=SubFLAG;
    Sub_POS=Pos;
}

void subWork(void)//6650
{
    u8 flag = 1;
    int step = 0;
    while(flag)
    {
        if(subFlag == 0)
        {
            subResetFlag = 0;
            resetSubMotor();
            
            subFlag = 1;
        }
        if(subFlag == 1)
        {
            if(subResetFlag == 1)
            {
                subFlag = 2;
            }
        }
        if(subFlag == 2)
        {
            step = subTotalStep - subCount * subIntervelStep;//subIntervelStep;//620;//
            SetSubPos(0x01,step);
            SET_Sub_MOVE_FLAG();
            subArriveFlag = 0;
            subFlag = 3;
            OSTimeDlyHMSM(0,0,0,20);
        }
        if(subFlag == 3)
        {
            if(subArriveFlag == 1)
            {
                subFlag = 4;
            }
        }
        if(subFlag == 4)
        {
            subResetFlag = 0;
            resetSubMotor();
            
            subFlag = 5;
        }
        if(subFlag == 5)
        {
            if(subResetFlag == 1)
            {
                // subCount ++;
                flag = 0;
                subFlag = 0;
            }
        }
        OSTimeDlyHMSM(0,0,0,10);
    }
}