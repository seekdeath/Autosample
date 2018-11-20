#include <includes.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorTrasmition.h>
#include <MotorXYZ.h>
///----------------------------------------------------------------------------
/// 复位XYZ模块
///----------------------------------------------------------------------------
void XYZReset(void)
{
	u8	bOk = 1;
	u16	status;
        s32     Z1POS,Z2POS;
        s32     Z1STEP,Z2STEP;
        s32     XPOS,YPOS;
        s32     XSTEP,YSTEP;
        
        MotorGetPos(MotorSample.nAddr, &Z1POS);
        MotorGetPos(MotorRegent.nAddr, &Z2POS);
        MotorGetPos(MotorX.nAddr, &XPOS);
        MotorGetPos(MotorY.nAddr, &YPOS);
        
        // 样品针电机复位
	TRY(bOk = MotorReset(MotorSample.nAddr));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
        
        // 试剂针电机复位
	TRY(bOk = MotorReset(MotorRegent.nAddr));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
        
        
        
        while  (1)//样品针
        {
                //获取样品针状态
                TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                if (status == 0x0000) break;
                //样品针原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z1_Origin);return;}
                //是否取消
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        MotorGetLastStep(MotorSample.nAddr, &Z1STEP);
        if ((abs(Z1POS)-abs(Z1STEP))>MAXERRSTEP)
        {Set_XYZ_State(XYZ_Err_Z1_Origin);return;}
          
        while (1)//加样针
        {
                //获取加样针状态
                TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                if (status == 0x0000) break;
                //加样针原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z2_Origin);return;}
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        MotorGetLastStep(MotorRegent.nAddr, &Z2STEP);
        if ((abs(Z2POS)-abs(Z2STEP))>MAXERRSTEP)
        {Set_XYZ_State(XYZ_Err_Z2_Origin);return;}
        
        // 复位时减慢X速度
        TRY(bOk = MotorSetHighSpeed(MotorX.nAddr, 1000/*MotorX.nSpeedMin*/,1000,MotorX.nSpeedStep));
        if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);return;}
    
        // 复位时减慢Y速度
        TRY(bOk = MotorSetHighSpeed(MotorY.nAddr, 700/*MotorY.nSpeedMin*/,700,MotorY.nSpeedStep));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);return;}
        
        // X电机复位
	TRY(bOk = MotorReset(MotorX.nAddr));
        if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);return;}
        
         // Y电机复位
	TRY(bOk = MotorReset(MotorY.nAddr));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);return;}

        while (1)//X电机
        {
                //获取X电机状态
                TRY(bOk = MotorGetStatus(MotorX.nAddr, &status));
                if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);return;}
                if (status == 0x0000) break;
                //X电机原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_X_Origin);return;}
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        MotorGetLastStep(MotorX.nAddr, &XSTEP);
        if ((abs(XPOS)-abs(XSTEP))>MAXERRSTEP)
        {Set_XYZ_State(XYZ_Err_X_Origin);return;}
        while (1)//Y电机
        {
                //获取Y电机状态
                TRY(bOk = MotorGetStatus(MotorY.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);return;}
                if (status == 0x0000) break;
                //Y电机原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Y_Origin);return;}
		// XYZ电机是否取消
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        
        MotorGetLastStep(MotorY.nAddr, &YSTEP);
        if ((abs(YPOS)-abs(YSTEP))>MAXERRSTEP)
        {Set_XYZ_State(XYZ_Err_Y_Origin);return;}
        
        // 恢复X电机频率
        TRY(bOk = MotorSetHighSpeed(MotorX.nAddr, MotorX.nSpeedMin,MotorX.nSpeedMax,MotorX.nSpeedStep));
         if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);return;}
    
        // 恢复Y电机频率
        TRY(bOk = MotorSetHighSpeed(MotorY.nAddr, MotorY.nSpeedMin,MotorY.nSpeedMax,MotorY.nSpeedStep));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);return;}
 
        
	Set_XYZ_State(XYZ_Idle);
}

///----------------------------------------------------------------------------
/// 设置XYZ位置
///----------------------------------------------------------------------------
void XYSetPos(void)
{
	u8	bOk = 1;
	u16	status;
	s32     XPos=0,YPos=0;
        s32     Z1Pos,Z2Pos;
        s32     Z1STEP,Z2STEP;
        u8      Z1Z2Flag;
        
        MotorGetPos(MotorSample.nAddr, &Z1Pos);
        MotorGetPos(MotorRegent.nAddr, &Z2Pos);
        //获取指令坐标
        Get_XYZ_Pos(&XPos,&YPos);
        // 样品针电机复位
        TRY(bOk = MotorMoveTo(MotorSample.nAddr,0));
	//TRY(bOk = MotorReset(MotorSample.nAddr));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
        
        // 试剂针电机复位
        TRY(bOk = MotorMoveTo(MotorRegent.nAddr,0));
	//TRY(bOk = MotorReset(MotorRegent.nAddr));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
        
        //等待样品针，试剂针回零
        while  (1)//样品针
        {
                //获取样品针状态
                TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                if (status == 0x0000) 
                    break;
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z1_Origin);return;}
                
                //是否取消
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        MotorGetLastStep(MotorSample.nAddr, &Z1STEP);
        if ((abs(Z1Pos)-abs(Z1STEP))>MAXERRSTEP)
        {Set_XYZ_State(XYZ_Err_Z1_Origin);return;}
        while (1)//加样针
        {
                //获取加样针状态
                TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                if (status == 0x0000) break;
                //加样针原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z2_Origin);return;}
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        MotorGetLastStep(MotorRegent.nAddr, &Z2STEP);
        if ((abs(Z2Pos)-abs(Z2STEP))>MAXERRSTEP)
        {Set_XYZ_State(XYZ_Err_Z2_Origin);return;}
        //设置X坐标
	TRY(bOk = MotorMoveTo(MotorX.nAddr,XPos));
        if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);return;}
        
        //设置Y坐标
	TRY(bOk = MotorMoveTo(MotorY.nAddr,YPos));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);return;}
        
        while (1)//X电机
        {
                //获取X电机状态
                TRY(bOk = MotorGetStatus(MotorX.nAddr, &status));
                if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);return;}
                if (status == 0x0000) break;
                //X电机原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_X_Origin);return;}
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        while (1)//Y电机
        {
                //获取Y电机状态
                TRY(bOk = MotorGetStatus(MotorY.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);return;}
                if (status == 0x0000) break;
                //Y电机原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Y_Origin);return;}
		// XYZ电机是否取消
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
        }
        GetZ1Z2Pos(&Z1Z2Flag,&Z1Pos,&Z2Pos);
        if (Z1Z2Flag==0)
        {
            if (Z1Pos != 0) 
            {
                //设置Z1坐标
                TRY(bOk = MotorMoveTo(MotorSample.nAddr,Z1Pos));
                if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                
                while  (1)//样品针
                {
                    //获取样品针状态
                    TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		    if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                    if (status == 0x0000) break;
                    //样品针原点失败
		    else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z1_Origin);return;}
                
                    //是否取消
                    if (IS_XYZ_Cancel()){
                      TRY(MotorCancel(MotorSample.nAddr));
                      TRY(MotorCancel(MotorRegent.nAddr));
                      TRY(MotorCancel(MotorX.nAddr));
                      TRY(MotorCancel(MotorY.nAddr));
                      Reset_XYZ_Cancel();
                      return;
                    }
                    OSTimeDly(10);
                }
            }
            if (Z2Pos != 0)
            {
                //设置Z2坐标
                TRY(bOk = MotorMoveTo(MotorRegent.nAddr,Z2Pos));
                if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                while (1)//试剂针
                {
                    //获取加样针状态
                    TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		    if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                    if (status == 0x0000) break;
                    //加样针原点失败
		    else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z2_Origin);return;}
                
                    if (IS_XYZ_Cancel()){
                      TRY(MotorCancel(MotorSample.nAddr));
                      TRY(MotorCancel(MotorRegent.nAddr));
                      TRY(MotorCancel(MotorX.nAddr));
                      TRY(MotorCancel(MotorY.nAddr));
                      Reset_XYZ_Cancel();
                      return;
                    }
                    OSTimeDly(10);
                }
            }
        }
        else if (Z1Z2Flag==0x01)
        {
            TRY(bOk = MotorTestLevel(MotorSample.nAddr, Z2Pos, Z1Pos));
	    if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
            while  (1)//样品针
            {
                //获取样品针状态
                TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                if (status == 0x0000) break;
                else if (status == 0x8005)//样品针液位感应失败
		{
                   TRY(bOk = MotorMoveTo(MotorSample.nAddr,0));
                   if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                   
		   do
		   {
                      TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		      if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
		   }while(status == 0x0001);
                   Set_XYZ_State( XYZ_Err_Sample_LiquidLevel);
                   return;
		}
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Sample_LiquidLevel);return;}
                
                //是否取消
                if (IS_XYZ_Cancel())
                {
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
            }
        }
        else if (Z1Z2Flag==0x02)
        {
            TRY(bOk = MotorTestLevel(MotorRegent.nAddr, Z2Pos, Z1Pos));
	    if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
            while (1)//试剂针
            {
                //获取加样针状态
                TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                if (status == 0x0000) break;
                else if (status == 0x8005)//试剂针液位感应失败
		{
                   TRY(bOk = MotorMoveTo(MotorRegent.nAddr,0));
                   if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                   
		   do
		   {
                      TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		      if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
		   }while(status == 0x0001);
			
                   Set_XYZ_State(XYZ_Err_Regent_LiquidLevel);
                   return;
		}
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Regent_LiquidLevel);return;}

                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
          }
        }
        Set_XYZ_State(XYZ_Idle);
}

void Z1Z2SetPos(void)
{
        u8 bOk=1;
        u8 Z1Z2Flag;
        u16 status;
        s32 Z1Pos,Z2Pos;
        GetZ1Z2Pos(&Z1Z2Flag,&Z1Pos,&Z2Pos);
        if (Z1Z2Flag==0)
        {
            //设置Z1坐标
            TRY(bOk = MotorMoveTo(MotorSample.nAddr,Z1Pos));
            if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
       
            //设置Z2坐标
            TRY(bOk = MotorMoveTo(MotorRegent.nAddr,Z2Pos));
            if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
            
            while  (1)//样品针
            {
                //获取样品针状态
                TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                if (status == 0x0000) break;
                //样品针原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z1_Origin);return;}
                
                //是否取消
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
            }
        
            while (1)//试剂针
            {
                //获取加样针状态
                TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                if (status == 0x0000) break;
                //加样针原点失败
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Z2_Origin);return;}
                
                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
          }
        }
      
        Set_XYZ_State(XYZ_Idle);
}
        
void ReadXYZPos(s32* XPos,s32* YPos,s32* Z1Pos,s32* Z2Pos)
{
        u8	bOk = 1;
        TRY(bOk = MotorGetPos(MotorSample.nAddr,Z1Pos));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
        TRY(bOk = MotorGetPos(MotorRegent.nAddr,Z2Pos));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
        TRY(bOk = MotorGetPos(MotorX.nAddr,XPos));
        if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);return;}
        TRY(bOk = MotorGetPos(MotorY.nAddr,YPos));
        if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);return;}
}
///----------------------------------------------------------------------------
/// 检测液面
///----------------------------------------------------------------------------
void TestLevel(void)
{
	u8	bOk = 1;
	u16	status;
        u8  Z1Z2Flag;
        s32 Z1Pos,Z2Pos;
        GetZ1Z2Pos(&Z1Z2Flag,&Z1Pos,&Z2Pos);
        if (Z1Z2Flag==0x01)
        {
	    TRY(bOk = MotorTestLevel(MotorSample.nAddr, Z2Pos, Z1Pos));
	    if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
            
            while  (1)//样品针
            {
                //获取样品针状态
                TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                if (status == 0x0000) break;
                else if (status == 0x8005)//样品针液位感应失败
		{
                   TRY(bOk = MotorMoveTo(MotorSample.nAddr,0));
                   if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
                   
		   do
		   {
                      TRY(bOk = MotorGetStatus(MotorSample.nAddr, &status));
		      if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);return;}
		   }while(status == 0x0001);
                   Set_XYZ_State( XYZ_Err_Sample_LiquidLevel);
                   return;
		}
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Sample_LiquidLevel);return;}
                
                //是否取消
                if (IS_XYZ_Cancel())
                {
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
            }
        }
        
        if (Z1Z2Flag==0x02)
        {
	    TRY(bOk = MotorTestLevel(MotorRegent.nAddr, Z2Pos, Z1Pos));
	    if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
            
            while (1)//试剂针
            {
                //获取加样针状态
                TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                if (status == 0x0000) break;
                else if (status == 0x8005)//试剂针液位感应失败
		{
                   TRY(bOk = MotorMoveTo(MotorRegent.nAddr,0));
                   if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
                   
		   do
		   {
                      TRY(bOk = MotorGetStatus(MotorRegent.nAddr, &status));
		      if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);return;}
		   }while(status == 0x0001);
			
                   Set_XYZ_State(XYZ_Err_Regent_LiquidLevel);
                   return;
		}
		else if (status >0x8000)  {Set_XYZ_State(XYZ_Err_Regent_LiquidLevel);return;}

                if (IS_XYZ_Cancel()){
                  TRY(MotorCancel(MotorSample.nAddr));
                  TRY(MotorCancel(MotorRegent.nAddr));
                  TRY(MotorCancel(MotorX.nAddr));
                  TRY(MotorCancel(MotorY.nAddr));
                  Reset_XYZ_Cancel();
                  return;
                }
                OSTimeDly(10);
          }

       }
       
       Set_XYZ_State(XYZ_Idle);
}

//初始化XYZ电机参数
void InitXYZMotor(void)
{
     u8 bOk =1;
     
    // 设置样品针电机参数
    TRY(bOk = MotorSetConfig(MotorSample.nAddr, MotorSample.nCtrlWord, MotorSample.nTimeoutSteps));
    if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);}
   
       // 设置样品针电机频率
    TRY(bOk = MotorSetHighSpeed(MotorSample.nAddr, MotorSample.nSpeedMin,MotorSample.nSpeedMax,MotorSample.nSpeedStep));
    if (!bOk) {Set_XYZ_State(XYZ_Err_Z1_Transmition);}
    
    // 设置电流
    TRY(bOk = MotorSetSpeedLevel(MotorSample.nAddr, MotorSample.LowspeedI,MotorSample.HighspeedI,MotorSample.HoldI));
    if (!bOk) {Set_Hook_State(XYZ_Err_Z1_Transmition);}
    
    TRY(bOk = MotorSetSpeedK(MotorSample.nAddr, MotorSample.SpeedK,MotorSample.ISensitive));
     if (!bOk) {Set_Hook_State(XYZ_Err_Z1_Transmition);}
    
    // 设置试剂针电机参数
    TRY(bOk = MotorSetConfig(MotorRegent.nAddr, MotorRegent.nCtrlWord, MotorRegent.nTimeoutSteps));
    if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);}
    // 设置试剂针电机频率
    TRY(bOk = MotorSetHighSpeed(MotorRegent.nAddr, MotorRegent.nSpeedMin,MotorRegent.nSpeedMax,MotorRegent.nSpeedStep));
    if (!bOk) {Set_XYZ_State(XYZ_Err_Z2_Transmition);}
    
    // 设置电流
    TRY(bOk = MotorSetSpeedLevel(MotorRegent.nAddr, MotorRegent.LowspeedI,MotorRegent.HighspeedI,MotorRegent.HoldI));
    if (!bOk) {Set_Hook_State(XYZ_Err_Z2_Transmition);}
    
    TRY(bOk = MotorSetSpeedK(MotorRegent.nAddr, MotorRegent.SpeedK,MotorRegent.ISensitive));
    if (!bOk) {Set_Hook_State(XYZ_Err_Z2_Transmition);}
    
    // 设置X电机参数
    TRY(bOk = MotorSetConfig(MotorX.nAddr, MotorX.nCtrlWord, MotorX.nTimeoutSteps));
    if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);}
    // 设置X电机频率
    TRY(bOk = MotorSetHighSpeed(MotorX.nAddr, MotorX.nSpeedMin,MotorX.nSpeedMax,MotorX.nSpeedStep));
    if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);}
    
    // 设置X电机频率
    TRY(bOk = MotorSetSpeedLevel(MotorX.nAddr, MotorX.LowspeedI,MotorX.HighspeedI,MotorX.HoldI));
    if (!bOk) {Set_XYZ_State(XYZ_Err_X_Transmition);}
    TRY(bOk = MotorSetSpeedK(MotorX.nAddr, MotorX.SpeedK,MotorX.ISensitive));
    if (!bOk) {Set_Hook_State(XYZ_Err_X_Transmition);}
    
    // 设置Y电机参数
    TRY(bOk = MotorSetConfig(MotorY.nAddr, MotorY.nCtrlWord, MotorY.nTimeoutSteps));
    if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);}
    // 设置Y电机频率
    TRY(bOk = MotorSetHighSpeed(MotorY.nAddr, MotorY.nSpeedMin,MotorY.nSpeedMax,MotorY.nSpeedStep));
    if (!bOk) {Set_XYZ_State(XYZ_Err_Y_Transmition);}
    
    // 设置电流
     TRY(bOk = MotorSetSpeedLevel(MotorY.nAddr, MotorY.LowspeedI,MotorY.HighspeedI,MotorY.HoldI));
     if (!bOk) {Set_Hook_State(XYZ_Err_Y_Transmition);}
     TRY(bOk = MotorSetSpeedK(MotorY.nAddr, MotorY.SpeedK,MotorY.ISensitive));
     if (!bOk) {Set_Hook_State(XYZ_Err_Y_Transmition);}
    
}

///----------------------------------------------------------------------------
/// XYZ和任务
///----------------------------------------------------------------------------
void HandleXYZ(void)
{
        if (IS_XYZ_R_FLAG()){
          Set_XYZ_State(XYZ_Busy);
          Reset_XYZ_R_FLAG();
          XYZReset();
        }
        if (IS_XYZ_XY_FLAG()){
          Set_XYZ_State(XYZ_Busy);
          Reset_XYZ_XY_FLAG();
          XYSetPos();
        }
        if (IS_XYZ_Z1Z2_FLAG()){
          Set_XYZ_State(XYZ_Busy);
          Reset_XYZ_Z1Z2_FLAG();
          Z1Z2SetPos();
        }
        if (IS_XYZ_TL_FLAG()){
          Set_XYZ_State(XYZ_Busy);
          Reset_XYZ_TL_FLAG();
          TestLevel();
        }
}

u8 XYZ_Cancel=0;
u8 IS_XYZ_Cancel(void)
{
    return XYZ_Cancel;
}
void Set_XYZ_Cancel(void)
{
    XYZ_Cancel=1;
}
void Reset_XYZ_Cancel(void)
{
    XYZ_Cancel=0;
}

//XYZ电机复位标志
u8 XYZ_R_FLAG=0;
//XYZ电机复位有效？
u8 IS_XYZ_R_FLAG(void)
{
    return XYZ_R_FLAG;
}
//设定XYZ电机复位有效
void Set_XYZ_R_FLAG(void)
{
    XYZ_R_FLAG=1;
}
//取消XYZ电机复位有效
void Reset_XYZ_R_FLAG(void)
{
    XYZ_R_FLAG=0;
}


//XYZ电机XY坐标移动标志
u8 XYZ_XY_FLAG=0;
//XYZ电机XY坐标有效？
u8 IS_XYZ_XY_FLAG(void)
{
    return XYZ_XY_FLAG;
}
//设置XYZ电机XY坐标有效
void Set_XYZ_XY_FLAG(void)
{
    XYZ_XY_FLAG=1;
}
//取消XYZ电机XY坐标有效
void Reset_XYZ_XY_FLAG(void)
{
    XYZ_XY_FLAG=0;
}


//XYZ电机Z1Z2坐标移动标志
u8 XYZ_Z1Z2_FLAG=0;
//XYZ电机Z1Z2坐标移动标志有效?
u8 IS_XYZ_Z1Z2_FLAG(void)
{
    return XYZ_Z1Z2_FLAG;
}
//设定XYZ电机Z1Z2坐标移动标志有效
void Set_XYZ_Z1Z2_FLAG(void)
{
    XYZ_Z1Z2_FLAG=1;
}
//取消XYZ电机Z1Z2坐标移动标志有效
void Reset_XYZ_Z1Z2_FLAG(void)
{
    XYZ_Z1Z2_FLAG=0;
}

//XYZ电机探液位标志
u8 XYZ_TL_FLAG=0;
//XYZ电机探液位标志有效？
u8 IS_XYZ_TL_FLAG(void)
{
    return XYZ_TL_FLAG;
}
//设定XYZ电机探液位标志有效
void Set_XYZ_TL_FLAG(void)
{
    XYZ_TL_FLAG=1;
}
//取消XYZ电机探液位标志有效
void Reset_XYZ_TL_FLAG(void)
{
    XYZ_TL_FLAG=0;
}

//XY坐标
s32 XYZ_XPos=0,XYZ_YPos=0,XYZ_Z1Pos=0,XYZ_Z2Pos=0;
u8  XYZ_Z1Z2Flag=0;
//设定XY坐标
void Set_XYZ_Pos(s32 XPos,s32 YPos)
{
    XYZ_XPos=XPos;
    XYZ_YPos=YPos;
}
//读取XY坐标
void Get_XYZ_Pos(s32* XPos,s32* YPos)
{
    *XPos=XYZ_XPos;
    *YPos=XYZ_YPos;
}
//设定Z1Z2坐标
void SetZ1Z2Pos(u8 Z1Z2Flag,s32 Z1Pos,s32 Z2Pos)
{
    XYZ_Z1Z2Flag=Z1Z2Flag;
    XYZ_Z1Pos=Z1Pos;
    XYZ_Z2Pos=Z2Pos;
}
//读取Z1Z2坐标
void GetZ1Z2Pos(u8* Z1Z2Flag,s32* Z1Pos,s32* Z2Pos)
{
    *Z1Z2Flag=XYZ_Z1Z2Flag;
    *Z1Pos=XYZ_Z1Pos;
    *Z2Pos=XYZ_Z2Pos;
}
