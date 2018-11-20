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
        //�Ͽ������
        GPIO_ResetBits(GPIOC,GPIO_Pin_4);
        
        MotorGetPos(MotorMotiveHook.nAddr, &HookXCurPos);
        MotorGetPos(MotorUDHook.nAddr, &HookYCurPos);
        
        //  ���������λ
	TRY(bOk = MotorReset(MotorUDHook.nAddr));
        if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
        while  (1)//�������
        {
                //��ȡ�������״̬
                TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
		if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                if (status == 0x0000) break;
                //�������ԭ��ʧ��
		else if (status >0x8000)  {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}
                //�Ƿ�ȡ��
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
        
         // ��λʱ�����������ٶ�
        TRY(bOk = MotorSetSpeed(MotorMotiveHook.nAddr, 800,800,MotorMotiveHook.nSpeedStep));
        if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
        
        // �ƶ������λ
	TRY(bOk = MotorReset(MotorMotiveHook.nAddr));
        if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
        while (1)//�ƶ����
        {
                //��ȡ������״̬
                TRY(bOk = MotorGetStatus(MotorMotiveHook.nAddr, &status));
		if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
                if (status == 0x0000) break;
                //������ԭ��ʧ��
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
        
       //�ָ�������Ƶ��
       TRY(bOk = MotorSetHighSpeed(MotorMotiveHook.nAddr, MotorMotiveHook.nSpeedMin,MotorMotiveHook.nSpeedMax,MotorMotiveHook.nSpeedStep));
       if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
     
       Set_Hook_State(Hook_Idle);
}


//�����ƶ�
void HookMove(void)
{
	u8	bOk = 1,flag;
	u16	status;
	s16     Posx,Posy;
        u8      Posz;
           

	//��ȡ����ƶ�λ��
        GetHookParam(&flag,&Posx,&Posy,&Posz);
        //MotorGetPos(MotorUDHook.nAddr, &HookYCurPos);
        if ((flag & 0x01) == 0x01)
        {
            //��ȡ�������״̬
            TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
            if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
            if (status == 0x0000)
            { 
                //��ʼ�ƶ�ǰ��������ȸ�λ
                TRY(bOk = MotorMoveTo(MotorUDHook.nAddr,0));
                if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
            }
            while  (1)//�������
            {
                    //��ȡ�������״̬
                    TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
                    if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                    if (status == 0x0000) break;
                    //�������ԭ��ʧ��
                    else if (status >0x8000)  {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}
                    //�Ƿ�ȡ��
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
 
        if ((flag & 0x01) == 0x01)//�����ƶ�
        {
                //�����ƶ�
                TRY(bOk = MotorMoveTo(MotorMotiveHook.nAddr,Posx));
                if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
                while  (1)
                {
                    //�����ƶ�״̬
                    TRY(bOk = MotorGetStatus(MotorMotiveHook.nAddr, &status));
		    if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);return;}
                    if (status == 0x0000) break;
                    //�����ƶ�����
		    else if (status >0x8000)  {Set_Hook_State(Hook_Err_Motor_Origin);return;}
                    //�Ƿ�ȡ��
                    if (IS_Hook_Cancel_FLAG()){
                      TRY(MotorCancel(MotorMotiveHook.nAddr));
                      TRY(MotorCancel(MotorUDHook.nAddr));
                      Reset_Hook_Cancel_FLAG();
                      return;
                    }
                    OSTimeDly(10);
                }
        }
        if ((flag & 0x02) == 0x02 && Posy != 0)//�����ƶ�
        {
                //�����ƶ�
                TRY(bOk = MotorMoveTo(MotorUDHook.nAddr,Posy));
                if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                while  (1)
                {
                    //�����ƶ�״̬
                    TRY(bOk = MotorGetStatus(MotorUDHook.nAddr, &status));
		    if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);return;}
                    if (status == 0x0000) break;
                    //�����ƶ�ʧ��
		    else if (status >0x8000)  {Set_Hook_State(Hook_Err_UDMotor_Origin);return;}
                    //�Ƿ�ȡ��
                    if (IS_Hook_Cancel_FLAG()){
                      TRY(MotorCancel(MotorMotiveHook.nAddr));
                      TRY(MotorCancel(MotorUDHook.nAddr));
                      Reset_Hook_Cancel_FLAG();
                      return;
                    }
                    OSTimeDly(10);
                }
        }
        if ((flag & 0x04) == 0x04)//���������
        {
                if (Posz==1)
                  //���ϵ����
                  GPIO_SetBits(GPIOC,GPIO_Pin_4);
                else if (Posz==0)
                  //�Ͽ������
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
    
     //  ���������������
     TRY(bOk = MotorSetConfig(MotorUDHook.nAddr, MotorUDHook.nCtrlWord, MotorUDHook.nTimeoutSteps));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     //  �����������Ƶ��
     TRY(bOk = MotorSetHighSpeed(MotorUDHook.nAddr, MotorUDHook.nSpeedMin,MotorUDHook.nSpeedMax,MotorUDHook.nSpeedStep));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     
     // �����ƶ��������
     TRY(bOk = MotorSetConfig(MotorMotiveHook.nAddr, MotorMotiveHook.nCtrlWord, MotorMotiveHook.nTimeoutSteps));
     if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);}
     // �����ƶ����Ƶ��
     TRY(bOk = MotorSetHighSpeed(MotorMotiveHook.nAddr, MotorMotiveHook.nSpeedMin,MotorMotiveHook.nSpeedMax,MotorMotiveHook.nSpeedStep));
     if (!bOk) {Set_Hook_State(Hook_Err_Motor_Transmition);}
     // ���õ���
     TRY(bOk = MotorSetSpeedLevel(MotorUDHook.nAddr, MotorUDHook.LowspeedI,MotorUDHook.HighspeedI,MotorUDHook.HoldI));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     TRY(bOk = MotorSetSpeedK(MotorUDHook.nAddr, MotorUDHook.SpeedK , MotorUDHook.ISensitive));
     if (!bOk) {Set_Hook_State(Hook_Err_UDMotor_Transmition);}
     // ���õ���
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

//���������λ��־
u8 Hook_R_FLAG=0;
//������λ��־��Ч��
u8 IS_Hook_R_FLAG(void)
{
    return Hook_R_FLAG;
}
//��λ������λ��־
void Set_Hook_R_FLAG(void)
{
    Hook_R_FLAG=1;
}
//��λ������λ��־
void Reset_Hook_R_FLAG(void)
{
    Hook_R_FLAG=0;
}

//��������״̬
u8 Hook_FLAG=0;
//�жϱ�������״̬
u8 IS_Hook_FLAG(void)
{
    return Hook_FLAG;
}
//��λ��������
void Set_Hook_FLAG(void)
{
    Hook_FLAG=1;
}
//��λ��������
void Reset_Hook_FLAG(void)
{
    Hook_FLAG=0;
}

u8  Hook_Param_FLAG=0;
//���������ƶ��ľ���
u16  Hook_X_Pos=0;
//���������ľ���
u16  Hook_Y_Pos=0;
//������Ķ���
u8   Hook_Z_Pos=0;

//�趨��������
void SetHookParam(u8 flag,s16 Pos_x,s16 Pos_y,u8 Pos_z)
{
    Hook_Param_FLAG=flag;
    Hook_X_Pos = Pos_x;
    Hook_Y_Pos = Pos_y;
    Hook_Z_Pos = Pos_z;
}
//��ȡ��������
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
        GPIO_SetBits(GPIOB,GPIO_Pin_10);//12V��������ϱ���  
    else if (Channel==1)
        GPIO_SetBits(GPIOB,GPIO_Pin_2);//12V��������ϱ��� 
    else if (Channel==2)
        GPIO_SetBits(GPIOB,GPIO_Pin_1);//12V��������ϱ��� 
    else if (Channel==3)
        GPIO_SetBits(GPIOB,GPIO_Pin_0);//12V��������ϱ��� */
    GPIO_SetBits(GPIOB,GPIO_Pin_11);//24V��������϶���
    OSTimeDly(200);
    GPIO_ResetBits(GPIOB,GPIO_Pin_11);//24V��������϶���
}

void CloseMagnetic(u8 Channel)
{
    if (Channel==0)
        GPIO_ResetBits(GPIOB,GPIO_Pin_10);//12V��������ϱ���  
    else if (Channel==1)
        GPIO_ResetBits(GPIOB,GPIO_Pin_2);//12V��������ϱ��� 
    else if (Channel==2)
        GPIO_ResetBits(GPIOB,GPIO_Pin_1);//12V��������ϱ��� 
    else if (Channel==3)
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);//12V��������ϱ��� 
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
    
    //������˿ڳ�ʼ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC,GPIO_Pin_4);//�ϵ�����ض�״̬
    
    
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
