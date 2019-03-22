#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorRotate.h>
//void resetRotateMotor()
//{
//    u8	bOk = 1;
//	u16	status;
//    
//        // 电机复位
//	TRY(bOk = MotorReset(motorAddS.nAddr));
//    if (!bOk) 
//    {
//        Set_Add_State(Add_Err_Transmition);
//        return;
//    }
//    
//    while  (1)//
//    {
//            //获取状态
//        TRY(bOk = MotorGetStatus(motorAddS.nAddr, &status));
//        if (!bOk) {Set_Add_State(Add_Err_Transmition);return;}
//        if (status == 0x0000) 
//        {
//            addResetFlag = 1;
//            break;
//        }
//            //原点失败
//        else if (status >0x8000)  {Set_Add_State(Add_Err_Origin);return;}
//            //是否取消
//        if (IS_Add_Cancel_FLAG())
//        {
//            TRY(MotorCancel(motorAddS.nAddr));
//            Reset_Add_Cancel_FLAG();
//            return;
//        }
//        OSTimeDly(100);
//    }
//    Set_Add_State(Add_Idle);
//}
///
///----------------------------------------------------------------------------
/// 设置坐标
///----------------------------------------------------------------------------
void RotateSetPos(void)
{
	u8	bOk = 1,RotateFLAG;
	u16	status;
	s32	Pos;
    
    GetRotatePos(&RotateFLAG,&Pos);
    if (RotateFLAG == 0x81 || RotateFLAG == 0x01)//判断add1motor目标位置是否有效
    {
        TRY(bOk = MotorMoveTo(motorRotateS.nAddr, Pos));
        if (!bOk) {Set_Rotate_State(Rotate_Err_Transmition);return;}
        while  (1)//样品泵
        {
            //获取状态
            TRY(bOk = MotorGetStatus(motorRotateS.nAddr, &status));
            if (!bOk) {Set_Rotate_State(Add_Err_Transmition);return;}
            if (status == 0x0000) 
            {
                rotateArriveFlag = 1;
                rotateRunFlag = 0;
                break;
            }
            //原点失败
            else if (status >0x8000)  {Set_Rotate_State(Rotate_Err_Origin);return;}
            //是否取消
            if (IS_Rotate_Cancel_FLAG())
            {
                TRY(MotorCancel(motorRotateS.nAddr));
                Reset_Rotate_Cancel_FLAG();
                return;
            }
            OSTimeDly(10);
        }
    }
    Set_Rotate_State(Rotate_Idle);
}


void InitRotateMotor(void)
{
    u8 bOk = 1;
     // 设置电机参数
    TRY(bOk = MotorSetConfig(motorRotateS.nAddr, motorRotateS.nCtrlWord, motorRotateS.nTimeoutSteps));
    if (!bOk) {Set_Rotate_State(Rotate_Err_Transmition);}
     
    // 设置电机频率
    TRY(bOk = MotorSetHighSpeed(motorRotateS.nAddr, motorRotateS.nSpeedMin, motorRotateS.nSpeedMax,motorRotateS.nSpeedStep));
    if (!bOk) {Set_Rotate_State(Rotate_Err_Transmition);}
     
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(motorRotateS.nAddr, motorRotateS.LowspeedI,motorRotateS.HighspeedI,motorRotateS.HoldI));
     if (!bOk) {Set_Rotate_State(Rotate_Err_Transmition);}
     TRY(bOk = MotorSetSpeedK(motorRotateS.nAddr, motorRotateS.SpeedK,motorRotateS.ISensitive));
     if (!bOk) {Set_Rotate_State(Rotate_Err_Transmition);}
    
}

///----------------------------------------------------------------------------
/// 任务
///----------------------------------------------------------------------------
void HandleRotate(void)
{
//    if (IS_Add_R_FLAG())
//    {
//        Set_Add_State(Add_Busy);
//        RESET_Add_R_FLAG();
//        resetAddMotor();
//    }
    if (IS_Rotate_MOVE_FLAG())
    {
        Set_Rotate_State(Rotate_Busy);
        RESET_Rotate_MOVE_FLAG();
        RotateSetPos();
    }
}

u8 Rotate_Cancel_FLAG=0;
u8 IS_Rotate_Cancel_FLAG(void)
{
    return Rotate_Cancel_FLAG;
}
void Set_Rotate_Cancel_FLAG(void)
{
    Rotate_Cancel_FLAG=1;
}
void Reset_Rotate_Cancel_FLAG(void)
{
    Rotate_Cancel_FLAG=0;
}
////复位标志
//u8 Add_R_FLAG=0;
////复位标志有效
//u8 IS_Add_R_FLAG(void)
//{
//    return Add_R_FLAG;
//}
//u8 GET_Add_R_FLAG(void)
//{
//    return Add_R_FLAG;
//}
//设定复位标志有效
//void SET_Add_R_FLAG(void)
//{
//    Add_R_FLAG=1;
//}
//取消复位标志
//void RESET_Add_R_FLAG(void)
//{
//    Add_R_FLAG=0;
//}
//移动标志位
u8 Rotate_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Rotate_MOVE_FLAG(void)
{
    return Rotate_MOVE_FLAG;
}
//置位移动标志位
void SET_Rotate_MOVE_FLAG(void)
{
    Rotate_MOVE_FLAG=1;
}
//复位移动标志位
void RESET_Rotate_MOVE_FLAG(void)
{
    Rotate_MOVE_FLAG=0;
}

//标志
u8 Rotate_FLAG;
//坐标
s32 Rotate_POS;
//获取标志，坐标
void GetRotatePos(u8* RotateFLAG,s32* Pos)
{
    *RotateFLAG=Rotate_FLAG;
    *Pos=Rotate_POS;
}
//设定标志，坐标
void SetRotatePos(u8 RotateFLAG,s32 Pos)
{
    Rotate_FLAG=RotateFLAG;
    Rotate_POS=Pos;
}
