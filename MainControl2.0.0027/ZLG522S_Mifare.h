//============= �������� ====================================================================================
#define PICC_REQUEST_EN 			1	        // ����
#define PICC_ANTICOLL_EN			1		// ����ײ
#define PICC_SELECT_EN				1		// ѡ��
#define PICC_HALT_EN				1		// ����
#define PICC_ACTIVE_TYPEA_EN		1			// A�Ϳ�����

#define PICC_AUTH_E2_EN				1		// Mifare����֤��Կ(E2����Կ��֤)
#define PICC_AUTH_KEY_EN			1		// Mifare����֤��Կ(ֱ����Կ��֤)
#define PICC_READ_EN				1		// ������
#define PICC_WRITE_EN				1		// д����
#define PICC_UL_WRITE_EN			0		// ULд
#define PICC_VALUE_OPER_EN			1		// ֵ�����

#define PICC_RESET_EN				1		// ��Ƭ��λ(ֹͣ�ز�һ��ʱ��)
#define PICC_AUTO_DETEGT_EN			1		// �Զ��������(6�ֽ���Կ)
#define PICC_AUTO_DETEGT_16_EN		0			// �Զ��������(16�ֽ���Կ)
#define PICC_AD_RCV_EN				1		// �жϲ�����Ķ��Զ���������
#define PICC_READ_AD_DT_EN			1		// ���Զ�������ݵ�����
//============= �������� ====================================================================================
//����A�Ϳ�����
#define REQ_STD					0x26		// ����(IDLE ��)������	
#define REQ_ALL					0x52		// ����(ALL  ��)������
#define SEL_CODE_LV1				0x93		// ����ײ/ѡ��ȼ�1������
#define SEL_CODE_LV2				0x95		// ����ײ/ѡ��ȼ�2������
#define SEL_CODE_LV3				0x97		// ����ײ/ѡ��ȼ�3������
#define MIFARE_COPY                             0xC2		// ֵ��������
#define MIFARE_INCREMENT                        0xC1		// ֵ������
#define MIFARE_DECREMENT                        0xC0		// ֵ������

#define KEY_A            			0x60		// ��ԿA
#define KEY_B            			0x61		// ��ԿB
//============= A�Ϳ��Զ����������� =======================================================================
#define AD_SERIALOUT				0x01		// �����������
#define AD_INTENABLE				0x02		// �ж�ʹ��
#define AD_CONTINUE				0x04		// �������֮��������
#define AD_HALT					0x08		// ��󽫿�����

#define AD_TX_ALERT				0x00		// TX1,TX2�������
#define AD_TX_ONLYTX1				0x01		// ��TX1���
#define AD_TX_ONLYTX2				0x02		// ��TX2���
#define AD_TX_ALL				0x03		// TX1,TX2ͬʱ���

#define AD_STOP					0x00		// ֹͣ���	
#define AD_REENABLE				0x01		// �������

//============= ȫ�ֱ���/�������� ===========================================================================
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccRequest(unsigned char ucReqCode,unsigned char *pATQ)
// ��������:    ����
// �������:    unsigned char ucReqCode			// �������		
// �������:    unsigned char *pATQ			// �����Ӧ
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccRequest(unsigned char ucReqCode,unsigned char *pATQ);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAnticoll( unsigned char ucSelCode,unsigned char ucBcnt,unsigned char *pUID)
// ��������:    ����ײ
// �������:    unsigned char ucSelCode			// ����ײ�ȼ�
//							// SELCODE1	0x93	�ȼ�1
//							// SELCODE2	0x95	�ȼ�2
//							// SELCODE3	0x97	�ȼ�3
//		unsigned char ucBcnt	                // ��֪���ŵ�λ����ͨ������Ϊ0
//		unsigned char *pSnr	                // ��֪�Ŀ��ţ�����֪�����������
// �������:    unsigned char *pSnr			// ����
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccAnticoll(unsigned char ucSelCode,unsigned char ucBcnt,unsigned char *pUID);

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
unsigned char PiccSelect(unsigned char ucSelCode,unsigned char *pUID,unsigned char *pSAK);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccHalt()
// ��������:    ������Ŀ�����
// �������:    -	
// �������:    -
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccHalt();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccActiveTypeA(	unsigned char unMode, unsigned char ucReqCode,
//              unsigned char *pATQ, unsigned char *pSAK,unsigned char *pUIDLen, unsigned char *pUID))
// ��������:    ģ����Ϣ����
// �������:    unsigned char unMode			// 0 -- ִ�з���ײѭ����1 -- ִֻ��һ�η���ײ
//		unsigned char ucReqCode			// ������	
// �������:    unsigned char *pATQ			// �����Ӧ
//		unsigned char *pSAK			// ���һ��ѡ��Ӧ��
//		unsigned char *pUIDLen			// ���ų���
//		unsigned char *pUID			// ����
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccActiveTypeA(unsigned char unMode, unsigned char ucReqCode, 
		unsigned char *pATQ, unsigned char *pSAK,unsigned char *pUIDLen, unsigned char *pUID);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccReset(unsigned char _1ms)
// ��������:    ��Ƭ��λ(�ر������ز�)
// �������:    unsigned char _1ms				// �ر��ز���ʱ��			
// �������:    -
// �� �� ֵ:    ����ִ�н��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccReset(unsigned char _1ms);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAutoDetect(unsigned char ucADMode, unsigned char ucTxMode, 
//		unsigned char ucReqCode, unsigned char ucAuthMode, 
//		unsigned char ucKeyType, unsigned char *pKey, 
//		unsigned char ucBlock)
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
unsigned char PiccAutoDetect(unsigned char ucADMode, unsigned char ucTxMode, 
		unsigned char ucReqCode, unsigned char ucAuthMode, 
		unsigned char ucKeyType, unsigned char *pKey, 
		unsigned char ucBlock);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccADRcv(unsigned char *pTxDrv, unsigned char *pATQ, 
//		unsigned char *pSAK, unsigned char *pUIDLen, 
//		unsigned char *pUID, unsigned char *pDataBuf)
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
unsigned char PiccADRcv(unsigned char *pTxDrv, unsigned char *pATQ, unsigned char *pSAK,
	      unsigned char *pUIDLen, unsigned char *pUID, unsigned char *pDataBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccReadADDT( unsigned char ucMode, unsigned char *pTxDrv, 
//		unsigned char *pATQ, unsigned char *pSAK, 
//		unsigned char *pUIDLen, unsigned char *pUID, 
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
unsigned char PiccReadADDT( unsigned char ucMode, unsigned char *pTxDrv, unsigned char *pATQ, 
		unsigned char *pSAK, unsigned char *pUIDLen, 
		unsigned char *pUID, unsigned char *pDataBuf);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAuthE2(unsigned char ucKeyType, unsigned char *pUID, 
//		unsigned char ucKeySector, unsigned char ucBlock)
// ��������:    Mifare1����֤�����֤�ɹ����Ժ��ͨ�ű����ܡ�
// ��ڲ���:    unsigned char ucKeyType			// ��Կ���ͣ�KEY_A -- ��ԿA;	KEY_B -- ��ԿB;
//		unsigned char *pUID			// 4�ֽ�UID
//		unsigned char ucKeySector		// ��Կ�������
//		unsigned char ucBlock			// ��֤�Ŀ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��֤�󣬷��ʸÿ����ڵ�����(��ͬȨ��)����������֤��Կ    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccAuthE2(unsigned char ucKeyType, unsigned char *pUID,
		unsigned char ucKeySector, unsigned char ucBlock);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAuthKey(unsigned char ucKeyType, unsigned char *pUID, 
//		unsigned char *pKey, unsigned char ucBlock)
// ��������:    Mifare1����֤�����֤�ɹ����Ժ��ͨ�ű����ܡ�
// ��ڲ���:    unsigned char ucKeyType			// ��Կ���ͣ�KEY_A -- ��ԿA;	KEY_B -- ��ԿB;
//		unsigned char *pUID			// 4�ֽ�UID
//		unsigned char *pKey			// 6�ֽ���Կ
//		unsigned char ucBlock			// ��֤�Ŀ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	��֤�󣬷��ʸÿ����ڵ�����(��ͬȨ��)����������֤��Կ    
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccAuthKey(unsigned char ucKeyType, unsigned char *pUID, 
						  unsigned char *pKey, unsigned char ucBlock);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccRead(unsigned char ucBlock, unsigned char *pBuf)
// ��������:    Mifare������
// ��ڲ���:    unsigned char ucBlock			; ���Ŀ��
// ���ڲ���:    unsigned char *pBuf			; ������16�ֽ�����
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�
// ˵    ��:	-   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccRead(unsigned char ucBlock, unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccWrite( unsigned char ucBlock, unsigned char *pBfr)
// ��������:    Mifareд����
// ��ڲ���:    unsigned char ucBlock			; д��Ŀ��ַ��
//		unsigned char *pBfr			; 16�ֽ�д���ݻ�������ַ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccWrite(unsigned char ucBlock,unsigned char *pBuf);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccULWrite( unsigned char ucBlock, unsigned char *pBuf)
// ��������:    Mifareд����
// ��ڲ���:    unsigned char ucBlock			; д��Ŀ��ַ��
//		unsigned char *pBfr			; 4�ֽ�д���ݻ�������ַ��
// ���ڲ���:    -
// �� �� ֵ:    STATUS_SUCCESS -- �ɹ�������ֵ -- ʧ�ܡ�   
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PiccULWrite(unsigned char ucBlock, unsigned char *pBuf);

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
unsigned char PiccValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
		unsigned char *pValue, unsigned char ucDBlock);
//===========================================================================================================


