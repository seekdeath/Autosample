#define GET_DVC_INFO_EN				1			// ��ȡģ����Ϣ
#define PCD_CONFIG_EN				1			// ���ö�доƬ
#define PCD_CLOSE_EN				1			// �رն�доƬ
#define PCD_LOAD_KEY_EN				1			// װ��6�ֽ���Կ
#define PCD_LOAD_KEY_16_EN			1			// װ��16�ֽ���Կ
#define PCD_WRITE_REG_EN			1			// д�Ĵ���
#define PCD_READ_REG_EN				1			// ���Ĵ���
#define PCD_SET_BAUD_EN				1			// ����ģ��UARTͨ�Ų�����
#define PCD_BIND_EN				1			// ģ���

#define PICC_RESET_EN				1			// ��Ƭ��λ(ֹͣ�ز�һ��ʱ��)
#define PICC_AUTO_DETEGT_EN			1			// �Զ��������(6�ֽ���Կ)
#define PICC_AUTO_DETEGT_16_EN		        0			// �Զ��������(16�ֽ���Կ)
#define PICC_AD_RCV_EN				1			// �жϲ�����Ķ��Զ���������
#define PICC_READ_AD_DT_EN			1			// ���Զ�������ݵ�����
//============= �������� ====================================================================================
//����A�Ϳ�����
#define KEY_A						0x60
#define KEY_B						0x61
#define ISO14443A					0x00
#define ISO14443B					0x80
#define ZLG522S_EXT

//============= ȫ�ֱ���/�������� =================================================================
///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char GetDvcInfo(unsigned char *pDvcInfo)
// ��������:    ��ȡģ����Ϣ
// �������:    -		
// �������:    unsigned char *pDvcInfo			// ģ����Ϣ
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT	unsigned char GetDvcInfo(unsigned char *pDvcInfo);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDConfig()
// ��������:    ���ö�д��оƬ
// �������:    -		
// �������:    
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PCDConfig();

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDClose()
// ��������:    ģ����Ϣ����
// �������:    -		
// �������:    
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PCDClose();

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDLoadKey(unsigned char ucKeyMode, unsigned char ucKeySNr,unsigned char *pKey)
// ��������:    װ����Կ
// �������:    unsigned char ucKeyMode			// ��Կģʽ	KEY_A;KEY_B	
//		unsigned char ucKeySNr			// ��Կ�������0~3
//		unsigned char *pKey			// 6�ֽ���Կ
// �������:    -
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	���������ԿΪ6�ֽڣ���ģ����Զ���չΪ16�ֽ�
//				Key[16] = pKey[0:5] | pKey[0:5] | pKey[0:3]
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PCDLoadKey(unsigned char ucKeyMode, unsigned char ucKeySNr,
									 unsigned char *pKey);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDLoadKey16(unsigned char ucKeyMode, unsigned char ucKeySNr,unsigned char *pKey)
// ��������:    װ����Կ
// �������:    unsigned char ucKeyMode			// ��Կģʽ	KEY_A;KEY_B	
//		unsigned char ucKeySNr			// ��Կ�������0~3
//		unsigned char *pKey			// 16�ֽ���Կ
// �������:    -
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PCDLoadKey16(unsigned char ucKeyMode, unsigned char ucKeySNr,unsigned char *pKey);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDWriteReg(unsigned char ucRegAddr,unsigned char ucRegValue)
// ��������:    װ����Կ
// �������:    unsigned char ucRegAddr			// �Ĵ�����ַ
//		unsigned char ucRegValue		// д���ֵ
// �������:    -
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PCDWriteReg(unsigned char ucRegAddr,unsigned char ucRegValue);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDReadReg(unsigned char ucRegAddr,unsigned char *pRegValue)
// ��������:    ����доƬ�Ĵ���
// �������:    unsigned char ucRegAddr			// �Ĵ�����ַ			
// �������:    unsigned char *pRegValue		// ������ֵ
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PCDReadReg(unsigned char ucRegAddr,unsigned char *pRegValue);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDSetBaud(ucBaudNum)
// ��������:    �ı�ģ���ͨ�Ų�����
// �������:    unsigned char ucBaudNum			// �����ʱ��0��7			
// �������:    -
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PCDSetBaud(unsigned char ucBaudNum);

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PCDBind(const INT8U *pUserID,  INT8U *pPCDID)
// ��������:    ģ��� 
// �������:    INT8U *pUserID	
// �������:    INT8U *pPCDID
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char PCDBind(const unsigned char *pUserID,  unsigned char *pPCDID);
//=================================================================================================
//   ������TypeA�����������Ϊ2
//=================================================================================================

///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccReset(unsigned char _1ms)
// ��������:    ��Ƭ��λ(�ر������ز�)
// �������:    unsigned char _1ms				// �ر��ز���ʱ��			
// �������:    -
// �� �� ֵ:    ����ִ�н��
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PiccReset(unsigned char _1ms);

///////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PiccAutoDetect(unsigned char ucADMode, unsigned char ucTxMode, 
	    unsigned char ucReqCode, unsigned char ucAuthMode,unsigned char ucKeyType, 
            unsigned char *pKey,unsigned char ucBlock);
///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccAutoDetect(unsigned char ucADMode, unsigned char ucTxMode, 
//		unsigned char ucReqCode, unsigned char ucAuthMode,unsigned char ucKeyType, 
//              unsigned char *pKey,unsigned char ucBlock)
// ��������:    A�Ϳ��Զ��������
// �������:    -
// �������:    unsigned char *pTxDrv			// �Ǹ����߼�⵽�Ŀ�
//							// 0x01--��TX1;	0x02--��TX2;	0x03--TX1��TX2
//		unsigned char *pATQ			// �����Ӧ(2�ֽ�)
//		unsigned char *pSAK			// ѡ��Ӧ��(1�ֽ�)
//		unsigned char *pUIDLen			// UID����
//		unsigned char *pUID			// UID
//		unsigned char *pDataBuf			// ����������
// �� �� ֵ:    ����ִ�н��
// ˵    ��:	��PiccAutoDetect()����֤ģʽΪ0�򣬱�������pDataBuf����ֵΪnone
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PiccADRcv(unsigned char *pTxDrv, unsigned char *pATQ, 
	    unsigned char *pSAK,unsigned char *pUIDLen,unsigned char *pUID, unsigned char *pDataBuf);
///////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char PiccReadADDT( unsigned char ucMode, unsigned char *pTxDrv, 
//		unsigned char *pATQ, unsigned char *pSAK, unsigned char *pUIDLen, unsigned char *pUID, 
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
// ˵    ��:	��PiccAutoDetect()����֤ģʽΪ0�򣬱�������pDataBuf����ֵΪnone		
///////////////////////////////////////////////////////////////////////////////////////////////////
ZLG522S_EXT unsigned char PiccReadADDT( unsigned char ucMode, unsigned char *pTxDrv, 
	    unsigned char *pATQ, unsigned char *pSAK,unsigned char *pUIDLen, unsigned char *pUID, 
	    unsigned char *pDataBuf);



