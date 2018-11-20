#include <includes.h>
#include "ZLG522S_Mifare.h"
#include "ZLG522S_Comm.h"
#include "StatusCode.h"
#include <string.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccRequest(unsigned char ucReqCode,unsigned char *pATQ)
// ��������:    ����
// �������:    unsigned char ucReqCode			// �������		
// �������:    unsigned char *pATQ			// �����Ӧ
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_REQUEST_EN
unsigned char PiccRequest(unsigned char ucReqCode,unsigned char *pATQ)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'A';
	g_ucSerBuf[LENGTH] = 1;
	g_ucSerBuf[DATA] = ucReqCode;
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
			memcpy(pATQ, &g_ucSerBuf[DATA], 2);
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAnticoll( unsigned char ucSelCode,unsigned char ucBcnt,unsigned char *pUID)
// ��������:    ����ײ
// �������:    unsigned char ucSelCode			// ����ײ�ȼ�
//							// SELCODE1	0x93	�ȼ�1
//							// SELCODE2	0x95	�ȼ�2
//							// SELCODE3	0x97	�ȼ�3
//		unsigned char ucBcnt			// ��֪���ŵ�λ����ͨ������Ϊ0
//		unsigned char *pSnr			// ��֪�Ŀ��ţ�����֪�����������
// �������:    unsigned char *pSnr			// ����
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_ANTICOLL_EN
unsigned char PiccAnticoll(unsigned char ucSelCode,unsigned char ucBcnt,unsigned char *pUID)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'B';
	g_ucSerBuf[LENGTH]  = 6;
	g_ucSerBuf[DATA]    = ucSelCode;
	g_ucSerBuf[DATA + 1]= ucBcnt;
	memcpy(&g_ucSerBuf[DATA + 2], pUID, 4);
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
		{
			memcpy(pUID, &g_ucSerBuf[DATA], 4);
		}
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccSelect(unsigned char ucSelCode,unsigned char *pUID,unsigned char *pSAK)
// ��������:    ѡ��
// �������:    unsigned char ucSelCode			// ѡ��(����ײ�ȼ�)
//							// SELCODE1	0x93	�ȼ�1
//							// SELCODE2	0x95	�ȼ�2
//							// SELCODE3	0x97	�ȼ�3
//		unsigned char *pUID			// ����
// �������:    unsigned char *pSAK			// ѡ��Ӧ��
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_SELECT_EN
unsigned char PiccSelect(unsigned char ucSelCode,unsigned char *pUID,unsigned char *pSAK)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'C';
	g_ucSerBuf[LENGTH] = 5;
	g_ucSerBuf[DATA]   = ucSelCode;
	memcpy(&g_ucSerBuf[DATA + 1], pUID, 4);
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
		{
			*pSAK = g_ucSerBuf[DATA];
		}
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccHalt()
// ��������:    ������Ŀ�����
// �������:    -	
// �������:    -
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_HALT_EN
unsigned char PiccHalt()
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'D';
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
// ����ԭ��:    unsigned char PiccAuthE2(unsigned char ucKeyType, unsigned char *pUID,unsigned char ucKeySector, unsigned char ucBlock)
// ��������:    Mifare1����֤�����֤�ɹ����Ժ��ͨ�ű����ܡ�
// ��ڲ���:    unsigned char ucKeyType			// ��Կ���ͣ�KEY_A -- ��ԿA;	KEY_B -- ��ԿB;
//		unsigned char *pUID			// 4�ֽ�UID
//		unsigned char ucKeySector		// ��Կ�������
//		unsigned char ucBlock			// ��֤�Ŀ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��֤�󣬷��ʸÿ����ڵ�����(��ͬȨ��)����������֤��Կ    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_AUTH_E2_EN
unsigned char PiccAuthE2(unsigned char ucKeyType, unsigned char *pUID,
						 unsigned char ucKeySector, unsigned char ucBlock)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'E';
	g_ucSerBuf[LENGTH] = 7;
	g_ucSerBuf[DATA] = ucKeyType;
	memcpy(&g_ucSerBuf[DATA+1],pUID,4);
	g_ucSerBuf[DATA+5] = ucKeySector;
	g_ucSerBuf[DATA+6] = ucBlock;	

	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif				// PICC_AUTH_E2_EN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAuthKey(unsigned char ucKeyType, unsigned char *pUID,unsigned char *pKey, unsigned char ucBlock)
// ��������:    Mifare1����֤�����֤�ɹ����Ժ��ͨ�ű����ܡ�
// ��ڲ���:    unsigned char ucKeyType			// ��Կ���ͣ�KEY_A -- ��ԿA;	KEY_B -- ��ԿB;
//		unsigned char *pUID			// 4�ֽ�UID
//		unsigned char *pKey			// 6�ֽ���Կ
//		unsigned char ucBlock			// ��֤�Ŀ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��֤�󣬷��ʸÿ����ڵ�����(��ͬȨ��)����������֤��Կ    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_AUTH_KEY_EN
unsigned char PiccAuthKey(unsigned char ucKeyType, unsigned char *pUID, 
						  unsigned char *pKey, unsigned char ucBlock)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'F';
	g_ucSerBuf[LENGTH] = 12;
	g_ucSerBuf[DATA] = ucKeyType;
	memcpy(&g_ucSerBuf[DATA+1], pUID, 4);
	memcpy(&g_ucSerBuf[DATA+5], pKey, 6);
	g_ucSerBuf[DATA+11] = ucBlock;	

	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif				// PICC_AUTH_KEY_EN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccRead(unsigned char ucBlock, unsigned char *pBuf)
// ��������:    Mifare������
// ��ڲ���:    unsigned char ucBlock	; ���Ŀ��
// ���ڲ���:    unsigned char *pBuf	; ������16�ֽ�����
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	-   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if	PICC_READ_EN
unsigned char PiccRead(unsigned char ucBlock, unsigned char *pBuf)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'G';
	g_ucSerBuf[LENGTH] = 1;
	g_ucSerBuf[DATA] = ucBlock;
		
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
		{
			memcpy(pBuf, &g_ucSerBuf[DATA], 16);
		}
	}
	return Status;
}
#endif				// PICC_READ_EN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccWrite( unsigned char ucBlock, unsigned char *pBfr)
// ��������:    Mifareд����
// ��ڲ���:    unsigned char ucBlock			; д��Ŀ��ַ��
//		unsigned char *pBfr			; 16�ֽ�д���ݻ�������ַ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	-   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_WRITE_EN
unsigned char PiccWrite(unsigned char ucBlock,unsigned char *pBuf)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'H';
	g_ucSerBuf[LENGTH] = 17;
	g_ucSerBuf[DATA] = ucBlock;
	memcpy(&g_ucSerBuf[DATA+1], pBuf, 16);
		
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif				// PICC_WRITE_EN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccULWrite( unsigned char ucBlock, unsigned char *pBuf)
// ��������:    Mifareд����
// ��ڲ���:    unsigned char ucBlock			; д��Ŀ��ַ��
//		unsigned char *pBfr			; 4�ֽ�д���ݻ�������ַ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	-   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_UL_WRITE_EN
unsigned char PiccULWrite(unsigned char ucBlock, unsigned char *pBuf)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'I';
	g_ucSerBuf[LENGTH] = 5;
	g_ucSerBuf[DATA] = ucBlock;
	memcpy(&g_ucSerBuf[DATA+1],pBuf,4);
		
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif				// PICC_UL_WRITE_EN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//		unsigned char *pValue, unsigned char ucDBlock)
// ��������:    Mifareֵ��������
// ��ڲ���:    unsigned char ucOperMode,		; ֵ����ģʽ
//		unsigned char ucSBlock			; Ҫ�����Ŀ��ַ��
//		unsigned char *pValue,			; 4�ֽ�Ҫ������ֵ�����ֽ��ڵ͵�ַ��
//		unsigned char ucDBlock			; ֵ������Ŀ�ؿ�
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_VALUE_OPER_EN
unsigned char PiccValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
							unsigned char *pValue, unsigned char ucDBlock)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'J';
	g_ucSerBuf[LENGTH] = 7;
	g_ucSerBuf[DATA] = ucOperMode;
	g_ucSerBuf[DATA+1] = ucSBlock;
	memcpy(&g_ucSerBuf[DATA+2], pValue, 4);
	g_ucSerBuf[DATA+6] = ucDBlock;
		
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif				// PICC_VALUE_OPER_EN
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccActiveTypeA(	unsigned char unMode, unsigned char ucReqCode, 
//		unsigned char *pATQ, unsigned char *pSAK,unsigned char *pUIDLen, unsigned char *pUID))
// ��������:    ģ����Ϣ����
// �������:    unsigned char unMode			// 0 -- ִ�з���ײѭ����1 -- ִֻ��һ�η���ײ
//		unsigned char ucReqCode			// ������	
// �������:    unsigned char *pATQ			// �����Ӧ
//		unsigned char *pSAK			// ���һ��ѡ��Ӧ��
//		unsigned char *pUIDLen			// ���ų���
//		unsigned char *pUID			// ����
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_ACTIVE_TYPEA_EN
unsigned char PiccActiveTypeA(unsigned char unMode, unsigned char ucReqCode, 
                              unsigned char *pATQ, unsigned char *pSAK,
			      unsigned char *pUIDLen, unsigned char *pUID)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'M';
	g_ucSerBuf[LENGTH] = 2;
	g_ucSerBuf[DATA] = unMode;
	g_ucSerBuf[DATA+1] = ucReqCode;
		
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
		{
			memcpy(pATQ, &g_ucSerBuf[DATA], 2);
			*pSAK = g_ucSerBuf[DATA+2];
			*pUIDLen = g_ucSerBuf[DATA+3];
			memcpy(pUID, &g_ucSerBuf[DATA+4], *pUIDLen);
		}
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccReset(unsigned char _1ms)
// ��������:    ��Ƭ��λ(�ر������ز�)
// �������:    unsigned char _1ms				// �ر��ز���ʱ��			
// �������:    -
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_RESET_EN
unsigned char PiccReset(unsigned char _1ms)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'L';
	g_ucSerBuf[LENGTH] = 1;
	g_ucSerBuf[DATA] = _1ms;
			
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAutoDetect(unsigned char ucADMode, unsigned char ucTxMode, 
//		unsigned char ucReqCode, unsigned char ucAuthMode,unsigned char ucKeyType, 
//              unsigned char *pKey,unsigned char ucBlock)
// ��������:    A�Ϳ��Զ��������
// �������:    unsigned char ucADMode			// �Զ����ģʽ
//		unsigned char ucTxMode			// ��������ģʽ
//		unsigned char ucReqCode			// ����ģʽ
//		unsigned char ucAuthMode		// ��֤ģʽ
//		unsigned char ucKeyType			// ��Կ����
//		unsigned char *pKey			// ��Կ
//		unsigned char ucBlock			// ���Ŀ��
// �������:    unsigned char *pRegValue		// ������ֵ
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_AUTO_DETEGT_EN
unsigned char PiccAutoDetect(unsigned char ucADMode, unsigned char ucTxMode, 
							 unsigned char ucReqCode, unsigned char ucAuthMode, 
							 unsigned char ucKeyType, unsigned char *pKey, 
							 unsigned char ucBlock)
{
	unsigned char Status;
	
	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'N';

	if(ucAuthMode == 0)
		g_ucSerBuf[LENGTH] = 4;
	else
	{// ���ö�����
		g_ucSerBuf[DATA + 4] = ucKeyType;			// 
		if(ucAuthMode == 'E')
		{
			g_ucSerBuf[DATA + 5] = *pKey;
			g_ucSerBuf[DATA + 6] = ucBlock;
			g_ucSerBuf[LENGTH] = 7;
		}
		else if(ucAuthMode == 'F')
		{
			memcpy(&g_ucSerBuf[DATA + 5], pKey, 6);
			g_ucSerBuf[DATA + 11] = ucBlock;
			g_ucSerBuf[LENGTH] = 12;
		}
		else
			return STATUS_INVALID_PARAMETER;
	}

	g_ucSerBuf[DATA] = ucADMode;
	g_ucSerBuf[DATA + 1] = ucTxMode;
	g_ucSerBuf[DATA + 2] = ucReqCode;
	g_ucSerBuf[DATA + 3] = ucAuthMode;
	
	Status = ZLG522S_Cmd();
	
	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccADRcv(unsigned char *pTxDrv, unsigned char *pATQ, 
//		unsigned char *pSAK, unsigned char *pUIDLen,unsigned char *pUID, unsigned char *pDataBuf)
// ��������:    �������ϴ����Զ���������
// �������:    -
// �������:    unsigned char *pTxDrv			// �Ǹ����߼�⵽�Ŀ�
//							// 0x01--��TX1;	0x02--��TX2;	0x03--TX1��TX2
//		unsigned char *pATQ			// �����Ӧ
//		unsigned char *pSAK			// ѡ��Ӧ��
//		unsigned char *pUIDLen			// UID����
//		unsigned char *pUID			// UID
//		unsigned char *pDataBuf			// ����������
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	�ú��������Զ���ȡģ���Զ����ʱ�������͵�����
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_AD_RCV_EN
unsigned char PiccADRcv(unsigned char *pTxDrv, unsigned char *pATQ, unsigned char *pSAK,
						unsigned char *pUIDLen, unsigned char *pUID, unsigned char *pDataBuf)
{
	unsigned char Status = STATUS_IO_TIMEOUT;

	if(Ser_Rcv())
	{	
		if(SEQ == (g_ucSerBuf[CMDTYPE] & 0xF0))
		{//����ƥ��
			Status = STATUS_SUCCESS;
			*pTxDrv = g_ucSerBuf[DATA];
			memcpy(pATQ, &g_ucSerBuf[DATA+1], 2);
			*pSAK = g_ucSerBuf[DATA+3];
			*pUIDLen = g_ucSerBuf[DATA+4];
			memcpy(pUID, &g_ucSerBuf[DATA+5], g_ucSerBuf[DATA+4]);

		// ��û��ʹ�ܶ����ݣ����ô������������
			if(g_ucSerBuf[LENGTH] == g_ucSerBuf[DATA+4] + 21)
				memcpy(pDataBuf, &g_ucSerBuf[DATA+5+g_ucSerBuf[DATA+4]], 16);
		}
		else
			Status = STATUS_COMM_SEQ_ERR;	//���Ų�ƥ��
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccReadADDT( unsigned char ucMode, unsigned char *pTxDrv, 
//		unsigned char *pATQ, unsigned char *pSAK,unsigned char *pUIDLen, unsigned char *pUID, 
//		unsigned char *pDataBuf)
// ��������:    ���Զ���������
// �������:    unsigned char ucMode			// 0x00 -- ֹͣ��⣻	0x01 -- �������
// �������:    unsigned char *pTxDrv			// �Ǹ����߼�⵽�Ŀ�
//							// 0x01--��TX1;	0x02--��TX2;	0x03--TX1��TX2
//		unsigned char *pATQ			// �����Ӧ
//		unsigned char *pSAK			// ѡ��Ӧ��
//		unsigned char *pUIDLen			// UID����
//		unsigned char *pUID			// UID
//		unsigned char *pDataBuf			// ����������
// �� �� ֵ:    0x00 -- �ɹ�;	0xFF -- δ��⵽��;	0x9C -- ����δִ��			
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_READ_AD_DT_EN
unsigned char PiccReadADDT( unsigned char ucMode, unsigned char *pTxDrv, unsigned char *pATQ, 
							unsigned char *pSAK, unsigned char *pUIDLen, 
							unsigned char *pUID, unsigned char *pDataBuf)
{
	unsigned char Status;

	g_ucSerBuf[CMDTYPE] = CMD_TYPE_TYPE_A;
	g_ucSerBuf[COMMAND] = 'O';
	g_ucSerBuf[LENGTH] = 1;
	g_ucSerBuf[DATA] = ucMode;
	
	Status = ZLG522S_Cmd();

	if(Status == STATUS_COMM_OK)
	{
		Status = g_ucSerBuf[STATUS];
		if(Status == STATUS_SUCCESS)
		{
			*pTxDrv = g_ucSerBuf[DATA];
			memcpy(pATQ, &g_ucSerBuf[DATA+1], 2);
			*pSAK = g_ucSerBuf[DATA+3];
			*pUIDLen = g_ucSerBuf[DATA+4];
			memcpy(pUID, &g_ucSerBuf[DATA+5], g_ucSerBuf[DATA+4]);
			if(g_ucSerBuf[LENGTH] == g_ucSerBuf[DATA+4] + 21)
				memcpy(pDataBuf, &g_ucSerBuf[DATA+5+g_ucSerBuf[DATA+4]], 16);
		}
	}

	return Status;
}
#endif
//============ The end ======================================================================================





