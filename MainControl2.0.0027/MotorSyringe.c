#include <includes.h>
#include <MotorTrasmition.h>
#include <Motor.h>
#include <MotionErr.h>
#include <MotorSyringe.h>
///----------------------------------------------------------------------------
/// ��λע���ģ��
///----------------------------------------------------------------------------
void SyringeReset(void)
{
	u8	bOk = 1;
	u16	status;
    
        // �����õ����λ
	TRY(bOk = MotorReset(MotorSyringSample.nAddr));
        if (!bOk) 
        {
          Set_Syringe_State(Syringe_Err_Transmition_S);
          return;
        }
        
        // �Լ��õ����λ
	TRY(bOk = MotorReset(MotorSyringReagent.nAddr));
        if (!bOk) 
        {
          Set_Syringe_State(Syringe_Err_Transmition_R);
          return;
        }
        
        while  (1)//������
        {
                //��ȡ������״̬
                TRY(bOk = MotorGetStatus(MotorSyringSample.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
                if (status == 0x0000) break;
                //��Ʒ��ԭ��ʧ��
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Origin_S);return;}
                //�Ƿ�ȡ��
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringSample.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(100);
        }
        
        while  (1)//�Լ���
        {
                //��ȡ�Լ���״̬
                TRY(bOk = MotorGetStatus(MotorSyringReagent.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
                if (status == 0x0000) break;
                //�Լ���ԭ��ʧ��
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Origin_R);return;}
                //�Ƿ�ȡ��
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringReagent.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
        }
        Set_Syringe_State(Syringe_Idle);
}

///----------------------------------------------------------------------------
/// ����ע�������
///----------------------------------------------------------------------------
void SyringeSetPos(void)
{
	u8	bOk = 1,SyringeFLAG;
	u16	status;
	s32	Pos;
        u16     Per;
        u16     Min,Max;
        
        GetSyringePos(&SyringeFLAG,&Pos,&Per);
        if (SyringeFLAG == 0x81 || SyringeFLAG == 0x01)//�ж���Ʒ��Ŀ��λ���Ƿ���Ч
        {
            Min = (u16)(((float)MotorSyringSample.nSpeedMin/100.0)*(float)Per);
            Max = (u16)(((float)MotorSyringSample.nSpeedMax/100.0)*(float)Per);
            TRY(bOk = MotorSetHighSpeed(MotorSyringSample.nAddr, Min , Max ,MotorSyringSample.nSpeedStep));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
            TRY(bOk = MotorMoveTo(MotorSyringSample.nAddr, Pos));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
            while  (1)//��Ʒ��
            {
                //��ȡ��Ʒ��״̬
                TRY(bOk = MotorGetStatus(MotorSyringSample.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
                if (status == 0x0000) break;
                //��Ʒ��ԭ��ʧ��
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
                //�Ƿ�ȡ��
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringSample.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
            }
        }
        else if (SyringeFLAG == 0x82 || SyringeFLAG == 0x02)
        {
            Min = (u16)(((float)MotorSyringReagent.nSpeedMin/100.0)*(float)Per);
            Max = (u16)(((float)MotorSyringReagent.nSpeedMax/100.0)*(float)Per);
            TRY(bOk = MotorSetHighSpeed(MotorSyringReagent.nAddr, Min , Max ,MotorSyringReagent.nSpeedStep));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
            TRY(bOk = MotorMoveTo(MotorSyringReagent.nAddr, Pos));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
            while  (1)//�Լ���
            {
                //��ȡ�Լ���״̬
                TRY(bOk = MotorGetStatus(MotorSyringReagent.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
                if (status == 0x0000) break;
                //�Լ���ԭ��ʧ��
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
                //�Ƿ�ȡ��
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringReagent.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
            }
        }
        else if (SyringeFLAG==0x03)
        {
            TRY(bOk = MotorMoveTo(MotorSyringSample.nAddr, Pos));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
            TRY(bOk = MotorMoveTo(MotorSyringReagent.nAddr, Pos));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
            
            while  (1)//��Ʒ��
            {
                //��ȡ��Ʒ��״̬
                TRY(bOk = MotorGetStatus(MotorSyringSample.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
                if (status == 0x0000) break;
                //��Ʒ��ԭ��ʧ��
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
                //�Ƿ�ȡ��
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringSample.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
            }
            
            while  (1)//�Լ���
            {
                //��ȡ�Լ���״̬
                TRY(bOk = MotorGetStatus(MotorSyringReagent.nAddr, &status));
		if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
                if (status == 0x0000) break;
                //�Լ���ԭ��ʧ��
		else if (status >0x8000)  {Set_Syringe_State(Syringe_Err_Transmition_R);return;}
                //�Ƿ�ȡ��
                if (IS_Syringe_Cancel_FLAG()){
                  TRY(MotorCancel(MotorSyringReagent.nAddr));
                  Reset_Syringe_Cancel_FLAG();
                  return;
                }
                OSTimeDly(10);
            } 
        }
        else if (SyringeFLAG ==0x04)
        {
            TRY(bOk = MotorSetValve(MotorSyringSample.nAddr, Pos));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
        }
        else if (SyringeFLAG==0x05)
        {
            TRY(bOk = MotorSetValve(MotorSyringReagent.nAddr, Pos));
            if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);return;}
        }
        
        
        Set_Syringe_State(Syringe_Idle);
}

void InitSyringeMotor(void)
{
    u8 bOk = 1;
    // ���ü����õ������
    TRY(bOk = MotorSetConfig(MotorSyringSample.nAddr, MotorSyringSample.nCtrlWord, MotorSyringSample.nTimeoutSteps));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);}
     
    // ���ü����õ��Ƶ��
    TRY(bOk = MotorSetHighSpeed(MotorSyringSample.nAddr, MotorSyringSample.nSpeedMin, MotorSyringSample.nSpeedMax,MotorSyringSample.nSpeedStep));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_S);}
    
    // �����Լ��õ������
    TRY(bOk = MotorSetConfig(MotorSyringReagent.nAddr, MotorSyringReagent.nCtrlWord, MotorSyringReagent.nTimeoutSteps));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);}
     
    // �����Լ��õ��Ƶ��
    TRY(bOk = MotorSetHighSpeed(MotorSyringReagent.nAddr, MotorSyringReagent.nSpeedMin, MotorSyringReagent.nSpeedMax,MotorSyringReagent.nSpeedStep));
    if (!bOk) {Set_Syringe_State(Syringe_Err_Transmition_R);}
    
    // ���õ���
     TRY(bOk = MotorSetSpeedLevel(MotorSyringSample.nAddr, MotorSyringSample.LowspeedI,MotorSyringSample.HighspeedI,MotorSyringSample.HoldI));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_S);}
     TRY(bOk = MotorSetSpeedK(MotorSyringSample.nAddr, MotorSyringSample.SpeedK,MotorSyringSample.ISensitive));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_S);}
    
    // ���õ���
     TRY(bOk = MotorSetSpeedLevel(MotorSyringReagent.nAddr, MotorSyringReagent.LowspeedI,MotorSyringReagent.HighspeedI,MotorSyringReagent.HoldI));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_R);}
     TRY(bOk = MotorSetSpeedK(MotorSyringReagent.nAddr, MotorSyringReagent.SpeedK,MotorSyringReagent.ISensitive));
     if (!bOk) {Set_Hook_State(Syringe_Err_Transmition_R);}
}

///----------------------------------------------------------------------------
/// ע�������
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
//�ø�λ��־
u8 Syringe_R_FLAG=0;
//�ø�λ��־��Ч
u8 IS_Syringe_R_FLAG(void)
{
    return Syringe_R_FLAG;
}
//�趨�ø�λ��־��Ч
void SET_Syringe_R_FLAG(void)
{
    Syringe_R_FLAG=1;
}
//ȡ���ø�λ��־
void RESET_Syringe_R_FLAG(void)
{
    Syringe_R_FLAG=0;
}
//���ƶ���־λ
u8 Syringe_MOVE_FLAG=0;
//���ƶ���־λ��Ч��
u8 IS_Syringe_MOVE_FLAG(void)
{
    return Syringe_MOVE_FLAG;
}
//��λ���ƶ���־λ
void SET_Syringe_MOVE_FLAG(void)
{
    Syringe_MOVE_FLAG=1;
}
//��λ���ƶ���־λ
void RESET_Syringe_MOVE_FLAG(void)
{
    Syringe_MOVE_FLAG=0;
}
u16 Syringe_Per;
//�õı�־
u8 Syringe_FLAG;
//���������꣬�Լ�������
s32 Syringe_POS;
//��ȡ��־������
void GetSyringePos(u8* SyringeFLAG,s32* Pos, u16 *Per)
{
    *SyringeFLAG=Syringe_FLAG;
    *Pos=Syringe_POS;
    *Per=Syringe_Per;
}
//�趨�ñ�־������
void SetSyringePos(u8 SyringeFLAG,s32 Pos,u16 Per)
{
    Syringe_FLAG=SyringeFLAG;
    Syringe_POS=Pos;
    Syringe_Per = Per;
}


    
