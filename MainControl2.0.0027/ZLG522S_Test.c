#include <includes.h>
#include "ZLG522S_Test.h"
#include "ZLG522S_Dvc.h"
#include "ZLG522S_Mifare.h"
#include "ZLG522S_Comm.h"
#include "StatusCode.h"
#include "gpio.h"
#include <i2c.h>
#include <string.h>
extern    OS_EVENT       *UARTSem;
//============= �������� ====================================================================================
//============= ȫ�ֱ������� ================================================================================
unsigned char ucMFKey[16] 	= {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//unsigned char ucKey1[16] 	= {0x00, 0x6A, 0x49, 0x6C, 0x6F, 0x76, 0x65, 0x59,0x4C, 0x6A, 0x49, 0x6C, 0x6F, 0x76, 0x65, 0x00};
unsigned char ucKey1[16] 	= {0x00, 0x6A, 0x49, 0x6C, 0x6F, 0x76, 0xFF, 0x07,0x80, 0x69, 0x49, 0x6C, 0x6F, 0x76, 0x65, 0x00};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned short TestZLG500S(unsigned char ucModuleType)
// ��������:    ģ�����
// �������:    unsigned char ucModuleType		// ģ������
// �������:    -
// �� �� ֵ:    ���Խ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned short TestZLG500S(unsigned char ucModuleType)
{
	unsigned char ucTemp,i;
	unsigned short Status;

	unsigned char ucTxMode;
	unsigned char ucATQ[2];
	unsigned char ucSAK;
	unsigned char ucUIDLen;
	unsigned char ucUID[10];
	unsigned char DataBuff[280];

	ucModuleType = ucModuleType;
//============= IICͨ�Ų��� =================================================================================
//	ModulePower(FALSE);

// ����ģ��ͨ�Žӿ�
//	SetCommMode(0);							// �Զ����IICģʽ
//	ModulePower(TRUE);
	Status = ZLG522S_CommInit();
 	if(Status)  return Status;

	Status = GetDvcInfo(DataBuff);
	if(Status)  return Status;

// ����ģ�鹦��
	Status = PCDLoadKey(KEY_A, 0x01, ucKey1);
	if(Status)  return Status;

	Status = PCDClose();
	if(Status)  return Status;

	Status = PCDConfig();
   	if(Status)  return Status;

	Status =  PiccRequest(REQ_ALL, ucATQ);
	if(Status)  return Status;

	Status = PiccAnticoll(SEL_CODE_LV1, 0, ucUID);
	if(Status)  return Status;
	ucUIDLen = 4;

	Status = PiccSelect(SEL_CODE_LV1, ucUID, &ucSAK);
	if(Status)  return Status;

	Status = PiccHalt();
	if(Status)  return Status;

	Status = PiccRequest(REQ_STD, ucATQ);
	if (Status == STATUS_SUCCESS)
	{// ��һ������������ִ�гɹ������������ִ��ʧ��
		Status = STATUS_FUNCTION_ERR;
		if(Status)  return Status;
	}

	Status = PiccRequest(REQ_STD, ucATQ);
	if (Status == STATUS_SUCCESS)
	{// �ڶ�������������ִ�гɹ������������ִ��ʧ��
		Status = STATUS_FUNCTION_ERR;
		if(Status)  return Status;
	}

	Status = PiccReset(10);						// �ر��ز�10ms��ʵ�ֿ�Ƭ��λ
	if(Status)  return Status;

	Status = PiccActiveTypeA(0, REQ_STD, &ucATQ[0], &ucSAK, &ucUIDLen, ucUID);
	if(Status)  return Status;

/*        //������Կ
        Status = PiccAuthKey(KEY_A, &ucUID[ucUIDLen - 4], ucMFKey, 0x07);
	if(Status)  return Status;
        
        Status = PiccWrite(0x07, ucKey1);
	if(Status)  return Status;*/
        
        Status = PiccAuthKey(KEY_A, &ucUID[ucUIDLen - 4], ucKey1, 0x07);
	if(Status)  return Status;
        
	// ��֤��Կ��ע�⿨�ŵ�����
//	Status = PiccAuthKey(KEY_A, &ucUID[ucUIDLen - 4], ucMFKey, 0x0B);
//	if(Status)  return Status;
        
        Status = PiccRead(0x07, &DataBuff[0]);	// �������е����ݣ�У���������Ƿ���ȷ
	if(Status)  return Status;
        
//        Status = PiccWrite(0x0B, ucKey1);
//	if(Status)  return Status;
        
	Status = PiccReset(10);					// �ر��ز�10ms��ʵ�ֿ�Ƭ��λ
	if(Status)  return Status;

	Status = PiccActiveTypeA(0, REQ_STD, &ucATQ[0], &ucSAK, &ucUIDLen, ucUID);
	if(Status)  return Status;

	// ��֤��Կ��ע�⿨�ŵ�����
	Status = PiccAuthE2(KEY_A, &ucUID[ucUIDLen - 4], 0x01, 0x0C);
	if(Status)  return Status;

	DataBuff[0]  = 0x0F; DataBuff[1]  = 0x57; DataBuff[2]  = 0xE8; DataBuff[3]  = 0x10;
	DataBuff[4]  = 0xF0; DataBuff[5]  = 0xA8; DataBuff[6]  = 0x17; DataBuff[7]  = 0xEF;
	DataBuff[8]  = 0x0F; DataBuff[9]  = 0x57; DataBuff[10] = 0xE8; DataBuff[11] = 0x10;
	DataBuff[12] = 0x04; DataBuff[13] = 0xFB; DataBuff[14] = 0x04; DataBuff[15] = 0xFB;

	Status = PiccWrite(0x0C, DataBuff);
	if(Status)  return Status;

	DataBuff[16] = 0xC2; DataBuff[17] = 0x29; DataBuff[18] = 0xDA; DataBuff[19] = 0x07;
/*	*/
	Status = PiccValueOper(MIFARE_INCREMENT, 0x0C, &DataBuff[16], 0x0D);
	if(Status)  return Status;

	DataBuff[16] = 0x0F; DataBuff[17] = 0xA7; DataBuff[18] = 0xE2; DataBuff[19] = 0x7F;

	Status = PiccValueOper(MIFARE_DECREMENT, 0x0D, &DataBuff[16], 0x0C);
	if(Status)  return Status;

	Status = PiccRead(0x0C, &DataBuff[20]);	// �������е����ݣ�У���������Ƿ���ȷ
	if(Status)  return Status;

	DataBuff[0]  = 0xC2; DataBuff[1]  = 0xD9; DataBuff[2]  = 0xDF; DataBuff[3]  = 0x98;
	DataBuff[4]  = 0x3D; DataBuff[5]  = 0x26; DataBuff[6]  = 0x20; DataBuff[7]  = 0x67;
	DataBuff[8]  = 0xC2; DataBuff[9]  = 0xD9; DataBuff[10] = 0xDF; DataBuff[11] = 0x98;
	DataBuff[12] = 0x04; DataBuff[13] = 0xFB; DataBuff[14] = 0x04; DataBuff[15] = 0xFB;

	if(memcmp(DataBuff, &DataBuff[20], 16))				// У��
		Status = STATUS_OTHER_ERROR;
	if(Status)  return Status;

	Status = PiccHalt();
	if(Status)  return Status;

//============= �����Զ�������� ==================================================================
// �����������ͻ����ݣ����غ�ֹͣ��⣬���󽫿�����
// TXȫ���ͣ�ֱ��������֤����ԿA������14��

	Status = PiccAutoDetect(AD_SERIALOUT | AD_HALT, AD_TX_ALL, REQ_STD, 'F', KEY_A, ucMFKey, 0x0E);
	if(Status)  return Status;

	ucTemp = 0;
	do
	{

		ucTemp++;
		if (ucTemp > 7) 
			ucTemp = 0;
		OSTimeDlyHMSM (0, 0, 0, 100);
		Status = PiccADRcv(&ucTxMode, ucATQ, &ucSAK, &ucUIDLen, ucUID, DataBuff);
	}
	while(STATUS_SUCCESS != Status);
	if(Status)  return Status;

// �����жϣ����غ������⣬���󽫿�����
// TXȫ���ͣ�1��������֤����ԿA������14��
	ucTemp = 1;					
	Status = PiccAutoDetect(AD_CONTINUE | AD_INTENABLE | AD_HALT, AD_TX_ALL, REQ_STD, 'E', KEY_A, &ucTemp, 0x0E);
	if(Status)  return Status;


	i = 2;
	ucTemp = 0;
	while(i)
	{
		if (0 == (ucTemp & 0x03))
		if (ucTemp > 0x0f) 
			ucTemp = 0;
		OSTimeDlyHMSM (0, 0, 0, 50);
		if(!GetNINT())
		{
			if(STATUS_SUCCESS == PiccReadADDT(AD_REENABLE, &ucTxMode, ucATQ, &ucSAK, &ucUIDLen, ucUID, DataBuff))
				i--;
		}
	}
// �ö��Զ����������ֹͣ�Զ����
	Status = PiccReadADDT(AD_STOP, &ucTxMode, ucATQ, &ucSAK, &ucUIDLen, ucUID, DataBuff);
	if ((STATUS_NO_TARGET == Status)||(0xCC == Status))
		Status = STATUS_SUCCESS;
	else
	{
		if (STATUS_NO_TARGET == Status)
			Status = STATUS_SUCCESS;
	}
	if(Status)  return Status;

//=================================================================================================
// �����������


//	Status = PiccReset(10);					// �ر��ز�10ms��ʵ�ֿ�Ƭ��λ
//	if(Status)  return Status;

	while(1)
	{
		Status = PiccReset(10);					// �ر��ز�10ms��ʵ�ֿ�Ƭ��λ
		if(Status)  return Status;

	   	OSTimeDlyHMSM (0, 0, 0, 5);
		if (STATUS_SUCCESS == PiccActiveTypeA(0, REQ_STD, &ucATQ[0], &ucSAK, &ucUIDLen, ucUID));
//			SetLED();
//		else
//			ClrLED();
	}
}
//extern OS_EVENT *I2CSem;
unsigned short IC_AutoDetective(void)
{
	unsigned short Status;
//        unsigned char err;
	unsigned char ucTxMode;
	unsigned char ucATQ[2];
	unsigned char ucSAK;
	unsigned char ucUIDLen;
	unsigned char ucUID[10];
	unsigned char DataBuff[280];
        unsigned short          Money;
     
        while (1)
	{
                Status = PiccAutoDetect(AD_SERIALOUT | AD_CONTINUE | AD_HALT, AD_TX_ALL, REQ_STD, 'F', KEY_A, ucMFKey, 0x11);
	        if(Status)  return Status;
                
                while (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5));
                I2C_ReadByte(g_ucSerBuf,0xB2);
                SerDTvalid=TRUE;
                
		Status = PiccADRcv(&ucTxMode, ucATQ, &ucSAK, &ucUIDLen, ucUID, DataBuff);
                if(Status)  return Status;
                
                sscanf((char const *)DataBuff,"%04hX",(unsigned short *)&Money);
                if (Money>5)
                Money=Money-5;
                sprintf((char *)DataBuff,"%04hX",*(unsigned short *)&Money);
                
                Status = PiccActiveTypeA(0, REQ_ALL, ucATQ, &ucSAK, &ucUIDLen, ucUID);
	        if(Status)  return Status;
                
                Status = PiccAuthKey(KEY_A, &ucUID[ucUIDLen - 4], ucMFKey, 0x11);
	        if(Status)  return Status;
                                
                Status = PiccWrite(0x11, DataBuff);
	        if(Status)  return Status;
                
                Status = PiccHalt();
	        if(Status)  return Status;
                
	}
//        return Status;
}
