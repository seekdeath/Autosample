#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorMoveSam.h>
#include <MotorRotate.h>
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
            //获取状�?        
      TRY(bOk = MotorGetStatus(motorMoveS1.nAddr, &status));
        if (!bOk) {Set_Move1_State(Move1_Err_Transmition);return;}
        if (status == 0x0000)
        { 
            move1ResetFlag = 1;
            break;
        }
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
        while  (1)//样品�?        
        {
            //获取状�?            
          TRY(bOk = MotorGetStatus(motorMoveS1.nAddr, &status));
            if (!bOk) {Set_Move1_State(Move1_Err_Transmition);return;}
            if (status == 0x0000) 
            {
                move1ArriveFlag = 1;
                break;
            }
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
        resetMove1Motor();
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
u8 IS_Move1_R_FLAG(void)
{
    return Move1_R_FLAG;
}
//复位标志有效
u8 GET_Move1_R_FLAG(void)
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
//移动标志�?
u8 Move1_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move1_MOVE_FLAG(void)
{
    return Move1_MOVE_FLAG;
}
//置位移动标志�?
void SET_Move1_MOVE_FLAG(void)
{
    Move1_MOVE_FLAG=1;
}
//复位移动标志�?
void RESET_Move1_MOVE_FLAG(void)
{
    Move1_MOVE_FLAG=0;
}

//标志
u8 Move1_FLAG;
//坐标
s32 Move1_POS;
//获取标志，坐�?
void GetMove1Pos(u8* Move1FLAG,s32* Pos)
{
    *Move1FLAG=Move1_FLAG;
    *Pos=Move1_POS;
}
//设定标志，坐�?
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
            //获取状�?        
        TRY(bOk = MotorGetStatus(motorMoveS2.nAddr, &status));
        if (!bOk) {Set_Move2_State(Move2_Err_Transmition);return;}
        if (status == 0x0000) 
        {
            move2ResetFlag = 1;
            break;
        }
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
        while  (1)//样品�?        
        {
            //获取状�?            
          TRY(bOk = MotorGetStatus(motorMoveS2.nAddr, &status));
            if (!bOk) {Set_Move2_State(Move2_Err_Transmition);return;}
            if (status == 0x0000) 
            {
                move2ArriveFlag = 1;
                break;
            }
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
        resetMove2Motor();
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
u8 IS_Move2_R_FLAG(void)
{
    return Move2_R_FLAG;
}
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
//移动标志�?
u8 Move2_MOVE_FLAG=0;
//移动标志位有效？
u8 IS_Move2_MOVE_FLAG(void)
{
    return Move2_MOVE_FLAG;
}
//置位移动标志�?
void SET_Move2_MOVE_FLAG(void)
{
    Move2_MOVE_FLAG=1;
}
//复位移动标志�?
void RESET_Move2_MOVE_FLAG(void)
{
    Move2_MOVE_FLAG=0;
}

//标志
u8 Move2_FLAG;
//坐标
s32 Move2_POS;
//获取标志，坐�?
void GetMove2Pos(u8* Move2FLAG,s32* Pos)
{
    *Move2FLAG=Move2_FLAG;
    *Pos=Move2_POS;
}
//设定标志，坐�?
void SetMove2Pos(u8 Move2FLAG,s32 Pos)
{
    Move2_FLAG=Move2FLAG;
    Move2_POS=Pos;
}


void move1Work(void)
{
    u8 flag = 1;
    move1WorkFlag = 1;
    u8 loopflag = 1;
    u8 loopCount = 0;
    while(flag)
    {
         if(move1Flag == 0)//复位
        {
            resetMove1Motor();
            move1Flag = 1; 
        }
        if(move1Flag == 1)
        {
            if(move1ResetFlag == 1)//复位完成
            {
                // move1Flag = 2;
                move1ArriveFlag = 0;
                SetMove1Pos(0X01,move1TotalStep);//前进步数到固定位置
                SET_Move1_MOVE_FLAG();
                move1Flag = 2;
                OSTimeDlyHMSM (0, 0, 0, 20);

            }
        }
        if(move1Flag == 2)//运行第一步动作
        {
            if(move1ArriveFlag == 1)//到达指定位置
            {
                SetMove1Pos(0x01,move1FirstBack);//回退一下
                SET_Move1_MOVE_FLAG();
                // move1Flag = 5;
                OSTimeDlyHMSM(0,0,0,20);
                move1Flag = 3;
            }
        }
        // for(int move1count = 0;move1count<10;move1count++)
        while(loopflag)
        {
            if(move1Flag == 3)
            {
                if(move1ArriveFlag == 1)
                {
                    SetMove1Pos(0x01,move1TotalStep);
                    SET_Move1_MOVE_FLAG();
                    move1Flag = 4;
                    OSTimeDlyHMSM(0,0,0,20);
                }
            }
            if(move1Flag == 4)
            {
                if(move1ArriveFlag == 1)
                {
                    SetMove1Pos(0x01,move1BackStep);
                    SET_Move1_MOVE_FLAG();
                    move1Flag = 5;
                    OSTimeDlyHMSM(0,0,0,20);
                }
            }
            if(move1Flag == 5)
            {
                if(move1ArriveFlag == 1)
                {
                    // SetMove1Pos(0x01,)
                    //吸合电磁铁
                    OSTimeDlyHMSM(0,0,0,100);
                    move1Flag = 6;

                }
            }
            if(move1Flag == 6)
            {
                SetRotatePos(0x01,rotateStep);
                SET_Rotate_MOVE_FLAG();
                move1Flag = 7;
                OSTimeDlyHMSM(0,0,0,20);
            }
            if(move1Flag == 7)
            {
                if(rotateArriveFlag == 1)
                {
                    //释放电磁铁
                    OSTimeDlyHMSM(0,0,0,100);
                    move1Flag = 8;
                }
            }
            if(move1Flag == 8)
            {
                if(loopCount > 9)
                {
                    SetMove1Pos(0x01,move1LastStep);
                    SET_Move1_MOVE_FLAG();
                    move1Flag = 9;
                    loopCount = 0;
                    loopflag = 0;
                    OSTimeDlyHMSM (0, 0, 0, 10);
                }
                else 
                {
                    move1Flag = 3;
                    loopCount ++;
                }
            }
        }
        if(move1Flag == 9)
        {
            move1Flag = 0;
            flag = 0;
            move1WorkFlag = 0;
        }
        
        OSTimeDlyHMSM (0, 0, 0, 10);

    }
    
}
void move2Work(void)
{
    u8 flag = 1;
    move2WorkFlag = 1;
    while(flag)
    {
        if(move2Flag == 0)
        {
            resetMove2Motor();
            move2Flag = 1;
        }
        if(move2Flag == 1)
        {
            
            if(move2ResetFlag == 1)
            {
                move2Flag = 2;
                move2ArriveFlag = 0;
            }
        }
        if(move2Flag == 2)
        {
            SetMove2Pos(0x01,3240);
            SET_Move2_MOVE_FLAG();
            move2Flag = 3;
            OSTimeDlyHMSM (0, 0, 0, 20);
        }
        if(move2Flag == 3)
        {
            if(move2ArriveFlag == 1)
            {
                move2Flag = 4;
            }
        }
        if(move2Flag == 4)
        {
            move2ResetFlag = 0;
            resetMove2Motor();
            
            move2Flag = 5;
        }
        if(move2Flag == 5)
        {
            if(move2ResetFlag == 1)
            {
                flag = 0;
                move2Flag = 0;
                move2WorkFlag = 0;
//                add2Count --;
            }
        }
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
    
}
