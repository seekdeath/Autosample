#include <includes.h>
#include <Motor.h>
#include <MotorTrasmition.h>
#include <MotionErr.h>
#include <MotorHook.h>

void HookReset(void)
{
        u8 bOk=1;
        u16 status;
        s32 HookXCurPos,HookXSteps;
        s32 HookYCurPos,HookYSteps;
        //断开电磁铁
        GPIO_ResetBits(GPIOC,GPIO_Pin_4);
        
        MotorGetPos(MotorMotiveHook.nAddr, &HookXCurPos);
        MotorGetPos(MotorUDHook.nAddr, &HookYCurPos);
        
        //  升降电机复位
	TRY(bOk = MotorReset(MotorUDHook.nAddr));
        if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
        while  (1)//升降电机
        {
                //获取升降电机状态
                TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
		if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                if (status == 0x0000) break;
                //升降电机原点失败
		else if (status >0x8000)  {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}
                //是否取消
                if (IS_Hook_Cancel_FLAG()){
                  TRY(MotorCancel(MotorUDHook.nAddr));
                  TRY(MotorCancel(MotorMotiveHook.nAddr));
                  Reset_Hook_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
        }
        
        MotorGetLastStep(MotorUDHook.nAddr, &HookYSteps);
        if ((abs(HookYCurPos)-abs(HookYSteps))>MAXERRSTEP)
        {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}
        
         // 复位时减慢横向电机速度
        TRY(bOk = MotorSetSpeed(MotorMotiveHook.nAddr, 800,800,MotorMotiveHook.nSpeedStep));
        if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
        
        // 移动电机复位
	TRY(bOk = MotorReset(MotorMotiveHook.nAddr));
        if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
        while (1)//移动电机
        {
                //获取加样针状态
                TRY(bOk = MotorGetStatus(MotorMotiveHook.nAddr, &status));
		if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
                if (status == 0x0000) break;
                //加样针原点失败
		else if (status >0x8000)  {Set_Hook_State(Hook_Err_Motor_Origin);return;}
                if (IS_Hook_Cancel_FLAG()){
                  TRY(MotorCancel(MotorUDHook.nAddr));
                  TRY(MotorCancel(MotorMotiveHook.nAddr));
                  Reset_Hook_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
        }
        
        MotorGetLastStep(MotorMotiveHook.nAddr, &HookXSteps);
        if ((abs(HookXCurPos)-abs(HookXSteps))>MAXERRSTEP)
        {Set_Hook_State(Hook_Err_Motor_Origin);return;}
        
       //恢复横向电机频率
       TRY(bOk = MotorSetHighSpeed(MotorMotiveHook.nAddr, MotorMotiveHook.nSpeedMin,MotorMotiveHook.nSpeedMax,MotorMotiveHook.nSpeedStep));
       if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
     
       Set_Hook_State(Hook_Idle);
}


//杯勾移动
void HookMove(void)
{
	u8	bOk = 1,flag;
	u16	status;
	s16     Posx,Posy;
        u8      Posz;
           

	//读取电机移动位置
        GetHookParam(&flag,&Posx,&Posy,&Posz);
        //MotorGetPos(MotorUDHook.nAddr, &HookYCurPos);
        if ((flag & 0x01) == 0x01)
        {
            //获取升降电机状态
            TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
            if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
            if (status == 0x0000)
            { 
                //开始移动前升降电机先复位
                TRY(bOk = MotorMoveTo(MotorUDHook.nAddr,0));
                if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
            }
            while  (1)//升降电机
            {
                    //获取升降电机状态
                    TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
                    if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                    if (status == 0x0000) break;
                    //升降电机原点失败
                    else if (status >0x8000)  {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}
                    //是否取消
                    if (IS_Hook_Cancel_FLAG()){
                      TRY(MotorCancel(MotorUDHook.nAddr));
                      TRY(MotorCancel(MotorMotiveHook.nAddr));
                      Reset_Hook_Cancel_FLAG();
                      return;
                    }
                    OSTimeDly(10);
            }
            /*MotorGetLastStep(MotorUDHook.nAddr, &HookYSteps);
            if ((abs(HookYCurPos)-abs(HookYSteps))>10)
            {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}*/
            
        }
 
        if ((flag & 0x01) == 0x01)//横向移动
        {
                //横向移动
                TRY(bOk = MotorMoveTo(MotorMotiveHook.nAddr,Posx));
                if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
                while  (1)
                {
                    //横向移动状态
                    TRY(bOk = MotorGetStatus(MotorMotiveHook.nAddr, &status));
		    if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
                    if (status == 0x0000) break;
                    //横向移动错误
		    else if (status >0x8000)  {Set_Hook_State(Hook_Err_Motor_Origin);return;}
                    //是否取消
                    if (IS_Hook_Cancel_FLAG()){
                      TRY(MotorCancel(MotorMotiveHook.nAddr));
                      TRY(MotorCancel(MotorUDHook.nAddr));
                      Reset_Hook_Cancel_FLAG();
                      return;
                    }
                    OSTimeDly(10);
                }
        }
        if ((flag & 0x02) == 0x02 && Posy != 0)//升降移动
        {
                //纵向移动
                TRY(bOk = MotorMoveTo(MotorUDHook.nAddr,Posy));
                if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                while  (1)
                {
                    //纵向移动状态
                    TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
		    if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                    if (status == 0x0000) break;
                    //纵向移动失败
		    else if (status >0x8000)  {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}
                    //是否取消
                    if (IS_Hook_Cancel_FLAG()){
                      TRY(MotorCancel(MotorMotiveHook.nAddr));
                      TRY(MotorCancel(MotorUDHook.nAddr));
                      Reset_Hook_Cancel_FLAG();
                      return;
                    }
                    OSTimeDly(10);
                }
        }
        if ((flag & 0x04) == 0x04)//电磁铁动作
        {
                if (Posz==1)
                  //吸合电磁铁
                  GPIO_SetBits(GPIOC,GPIO_Pin_4);
                else if (Posz==0)
                  //断开电磁铁
                  GPIO_ResetBits(GPIOC,GPIO_Pin_4);
        }
        TRY(bOk = MotorMoveTo(MotorUDHook.nAddr,0));
        //if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
        //OSTimeDly(10);
        
	Set_Hook_State(Hook_Idle);
}

void InitHookMotor(void)
{
     u8	bOk = 1;
    
     //  设置升降电机参数
     TRY(bOk = MotorSetConfig(MotorUDHook.nAddr, MotorUDHook.nCtrlWord, MotorUDHook.nTimeoutSteps));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     //  设置升降电机频率
     TRY(bOk = MotorSetHighSpeed(MotorUDHook.nAddr, MotorUDHook.nSpeedMin,MotorUDHook.nSpeedMax,MotorUDHook.nSpeedStep));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     
     // 设置移动电机参数
     TRY(bOk = MotorSetConfig(MotorMotiveHook.nAddr, MotorMotiveHook.nCtrlWord, MotorMotiveHook.nTimeoutSteps));
     if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);}
     // 设置移动电机频率
     TRY(bOk = MotorSetHighSpeed(MotorMotiveHook.nAddr, MotorMotiveHook.nSpeedMin,MotorMotiveHook.nSpeedMax,MotorMotiveHook.nSpeedStep));
     if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);}
     // 设置电流
     TRY(bOk = MotorSetSpeedLevel(MotorUDHook.nAddr, MotorUDHook.LowspeedI,MotorUDHook.HighspeedI,MotorUDHook.HoldI));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     TRY(bOk = MotorSetSpeedK(MotorUDHook.nAddr, MotorUDHook.SpeedK , MotorUDHook.ISensitive));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     // 设置电流
     TRY(bOk = MotorSetSpeedLevel(MotorMotiveHook.nAddr, MotorMotiveHook.LowspeedI,MotorMotiveHook.HighspeedI,MotorMotiveHook.HoldI));
     if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);}
     TRY(bOk = MotorSetSpeedK(MotorMotiveHook.nAddr, MotorMotiveHook.SpeedK,MotorMotiveHook.ISensitive));
     if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);}
}

void HandleHook(void)
{
  if (IS_Hook_R_FLAG()){
      Set_Hook_State(Hook_Busy);
      Reset_Hook_R_FLAG();
      HookReset();
  }
  if (IS_Hook_FLAG()){
      Set_Hook_State(Hook_Busy);
      Reset_Hook_FLAG();
      HookMove();
  }
}

u8 Hook_Cancel_FLAG=0;
u8  IS_Hook_Cancel_FLAG(void)
{
    return Hook_Cancel_FLAG;
}
void Set_Hook_Cancel_FLAG(void)
{
    Hook_Cancel_FLAG=1;
}
void Reset_Hook_Cancel_FLAG(void)
{
    Hook_Cancel_FLAG=0;
}

//杯钩电机复位标志
u8 Hook_R_FLAG=0;
//杯钩复位标志有效？
u8 IS_Hook_R_FLAG(void)
{
    return Hook_R_FLAG;
}
//置位杯钩复位标志
void Set_Hook_R_FLAG(void)
{
    Hook_R_FLAG=1;
}
//复位杯钩复位标志
void Reset_Hook_R_FLAG(void)
{
    Hook_R_FLAG=0;
}

//杯钩命令状态
u8 Hook_FLAG=0;
//判断杯钩命令状态
u8 IS_Hook_FLAG(void)
{
    return Hook_FLAG;
}
//置位杯钩命令
void Set_Hook_FLAG(void)
{
    Hook_FLAG=1;
}
//复位杯钩命令
void Reset_Hook_FLAG(void)
{
    Hook_FLAG=0;
}

u8  Hook_Param_FLAG=0;
//杯钩横向移动的距离
u16  Hook_X_Pos=0;
//杯钩升降的距离
u16  Hook_Y_Pos=0;
//电磁铁的动作
u8   Hook_Z_Pos=0;

//设定杯钩参数
void SetHookParam(u8 flag,s16 Pos_x,s16 Pos_y,u8 Pos_z)
{
    Hook_Param_FLAG=flag;
    Hook_X_Pos = Pos_x;
    Hook_Y_Pos = Pos_y;
    Hook_Z_Pos = Pos_z;
}
//读取杯钩参数
void GetHookParam(u8* flag,s16* Pos_x,s16* Pos_y,u8* Pos_z)
{
     *flag = Hook_Param_FLAG;
    *Pos_x = Hook_X_Pos;
    *Pos_y = Hook_Y_Pos;
    *Pos_z = Hook_Z_Pos;
}

void ActMagnetic(u8 Channel)
{   
    if (Channel==0)
        GPIO_SetBits(GPIOB,GPIO_Pin_10);//12V电磁铁吸合保持  
    else if (Channel==1)
        GPIO_SetBits(GPIOB,GPIO_Pin_2);//12V电磁铁吸合保持 
    else if (Channel==2)
        GPIO_SetBits(GPIOB,GPIO_Pin_1);//12V电磁铁吸合保持 
    else if (Channel==3)
        GPIO_SetBits(GPIOB,GPIO_Pin_0);//12V电磁铁吸合保持 */
    GPIO_SetBits(GPIOB,GPIO_Pin_11);//24V电磁铁吸合动作
    OSTimeDly(200);
    GPIO_ResetBits(GPIOB,GPIO_Pin_11);//24V电磁铁吸合动作
}

void CloseMagnetic(u8 Channel)
{
    if (Channel==0)
        GPIO_ResetBits(GPIOB,GPIO_Pin_10);//12V电磁铁吸合保持  
    else if (Channel==1)
        GPIO_ResetBits(GPIOB,GPIO_Pin_2);//12V电磁铁吸合保持 
    else if (Channel==2)
        GPIO_ResetBits(GPIOB,GPIO_Pin_1);//12V电磁铁吸合保持 
    else if (Channel==3)
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);//12V电磁铁吸合保持 
}


void Init_BeepGPIO(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void BeepControl(void)
{
    u16 OnTime,OffTime;
    GetBeepParam(&OnTime,&OffTime);
    GPIO_SetBits(GPIOC,GPIO_Pin_13);
    OSTimeDly(OnTime);
    GPIO_ResetBits(GPIOC,GPIO_Pin_13);
    OSTimeDly(OffTime);
}

void Init_HookMagnetic(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOB,GPIO_Pin_10);
    
    //电磁铁端口初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_4);//上电磁铁关断状态
    
    
}

void MagneticControl(void)
{
    u8 State,Select;
    Select=GetMagneticSelect();
    State=GetMagneticState();
    if (Select==0x01)
    {
        if (State)
            ActMagnetic(0);
        else
            CloseMagnetic(0);
    }
    else if (Select==0x02)
    {
        if (State)
            ActMagnetic(1);
        else
            CloseMagnetic(1);
    }
    else if (Select==0x03)
    {
        if (State)
            ActMagnetic(2);
        else
            CloseMagnetic(2);
    }
}

u8 MagneticSelect=1;
u8 MagneticState=0;
u8 GetMagneticSelect(void)
{
    return MagneticSelect;
}
void SetMagneticSelect(u8 flag)
{
    MagneticSelect=flag;
}
u8 GetMagneticState(void)
{
    return MagneticState;
}
void SetMagneticState(u8 flag)
{
    MagneticState=flag;
}
u8 MagneticFlag=0;
u8 IS_MAGNETIC_ACT(void)
{
    return MagneticFlag;
}
void SET_MAGNETIC_ACT(void)
{
    MagneticFlag=1;
}
void RESET_MAGNETIC_ACT(void)
{
    MagneticFlag=0;
}

u8 BEEP_FLAG=0;
u8 IS_BEEP_CON(void)
{
    return BEEP_FLAG;
}
void SET_BEEP_CON(void)
{
    BEEP_FLAG=1;
}
void RESET_BEEP_CON(void)
{
    BEEP_FLAG=0;
}
u16 BeepOnTime=0,BeepOffTime=0;
void GetBeepParam(u16 *OnTime,u16 *OffTime)
{
    *OnTime=BeepOnTime;
    *OffTime=BeepOffTime;
}
void SetBeepParam(u16 OnTime,u16 OffTime)
{
    BeepOnTime=OnTime;
    BeepOffTime=OffTime;
}
