#include <includes.h>
#include "ZLG522S_Dvc.h"
#include "ZLG522S_Comm.h"
#include "StatusCode.h"
#include <string.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char GetDvcInfo(unsigned char *DvcInfo)
// ��������:    ��ȡģ����Ϣ
// �������:    -		
// �������:    unsigned char *DvcInfo			// ģ����Ϣ
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if GET_DVC_INFO_EN
unsigned char GetDvcInfo(unsigned char *pDvcInfo)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_DVC_CONTROL;
	g_ucSerBuf[COMMAND] = 'A';
	g_ucSerBuf[LENGTH] = 0;
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
		  memcpy(pDvcInfo, &g_ucSerBuf[DATA], g_ucSerBuf[LENGTH]);
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDConfig()
// ��������:    ���ö�д��оƬ
// �������:    -		
// �������:    
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PCD_CONFIG_EN
unsigned char PCDConfig()
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_DVC_CONTROL;
	g_ucSerBuf[COMMAND] = 'B';
	g_ucSerBuf[LENGTH] = 0;
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDClose()
// ��������:    ģ����Ϣ����
// �������:    -		
// �������:    
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PCD_CLOSE_EN
unsigned char PCDClose()
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_DVC_CONTROL;
	g_ucSerBuf[COMMAND] = 'C';
	g_ucSerBuf[LENGTH] = 0;
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDLoadKey(unsigned char ucKeyType, unsigned char ucKeySNr,unsigned char *pKey)
// ��������:    װ����Կ
// �������:    unsigned char ucKeyType			// ��Կ����	KEY_A;KEY_B	
//		unsigned char ucKeySNr			// ��Կ�������0~3
//		unsigned char *pKey			// 6�ֽ���Կ
// �������:    -
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	���������ԿΪ6�ֽڣ���ģ����Զ���չΪ16�ֽ�
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PCD_LOAD_KEY_EN
unsigned char PCDLoadKey(unsigned char ucKeyType,unsigned char ucKeySNr,unsigned char *pKey)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_DVC_CONTROL;
	g_ucSerBuf[COMMAND] = 'E';
	g_ucSerBuf[LENGTH] = 8;
	g_ucSerBuf[DATA] = ucKeyType;
	g_ucSerBuf[DATA+1] = ucKeySNr;
	memcpy(&g_ucSerBuf[DATA+2], pKey, 6);
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDWriteReg(unsigned char ucRegAddr,unsigned char ucRegValue)
// ��������:    д��доƬ�Ĵ���
// �������:    unsigned char ucRegAddr			// �Ĵ�����ַ
//		unsigned char ucRegValue		// д���ֵ
// �������:    -
// �� �� ֵ:    ����ִ�н��	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PCD_WRITE_REG_EN
unsigned char PCDWriteReg(unsigned char ucRegAddr,unsigned char ucRegValue)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_DVC_CONTROL;
	g_ucSerBuf[COMMAND] = 'F';
	g_ucSerBuf[LENGTH] = 2;
	g_ucSerBuf[DATA] = ucRegAddr;
	g_ucSerBuf[DATA+1] = ucRegValue;
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDReadReg(unsigned char ucRegAddr,unsigned char *pRegValue)
// ��������:    ����доƬ�Ĵ���
// �������:    unsigned char ucRegAddr			// �Ĵ�����ַ			
// �������:    unsigned char *pRegValue		// ������ֵ
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PCD_READ_REG_EN
unsigned char PCDReadReg(unsigned char ucRegAddr,unsigned char *pRegValue)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_DVC_CONTROL;
	g_ucSerBuf[COMMAND] = 'G';
	g_ucSerBuf[LENGTH] = 1;
	g_ucSerBuf[DATA] = ucRegAddr;
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
			*pRegValue = g_ucSerBuf[DATA];
	}
	return Status;
}
#endif

unsigned char GetNINT(void)
{
    return 0;
}