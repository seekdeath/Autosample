#include <includes.h>
#include "ZLG522S_Mifare.h"
#include "ZLG522S_Comm.h"
#include "StatusCode.h"
#include <string.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccRequest(unsigned char ucReqCode,unsigned char *pATQ)
// 函数功能:    请求
// 输入参数:    unsigned char ucReqCode			// 请求代码		
// 输出参数:    unsigned char *pATQ			// 请求回应
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PiccAnticoll( unsigned char ucSelCode,unsigned char ucBcnt,unsigned char *pUID)
// 函数功能:    防碰撞
// 输入参数:    unsigned char ucSelCode			// 防碰撞等级
//							// SELCODE1	0x93	等级1
//							// SELCODE2	0x95	等级2
//							// SELCODE3	0x97	等级3
//		unsigned char ucBcnt			// 已知卡号的位数，通常设置为0
//		unsigned char *pSnr			// 已知的卡号，若不知道，则不用理会
// 输出参数:    unsigned char *pSnr			// 卡号
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PiccSelect(unsigned char ucSelCode,unsigned char *pUID,unsigned char *pSAK)
// 函数功能:    选择卡
// 输入参数:    unsigned char ucSelCode			// 选择(防碰撞等级)
//							// SELCODE1	0x93	等级1
//							// SELCODE2	0x95	等级2
//							// SELCODE3	0x97	等级3
//		unsigned char *pUID			// 卡号
// 输出参数:    unsigned char *pSAK			// 选择应答
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PiccHalt()
// 函数功能:    将激活的卡挂起
// 输入参数:    -	
// 输出参数:    -
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PiccAuthE2(unsigned char ucKeyType, unsigned char *pUID,unsigned char ucKeySector, unsigned char ucBlock)
// 函数功能:    Mifare1卡验证命令，验证成功后，以后的通信被加密。
// 入口参数:    unsigned char ucKeyType			// 密钥类型：KEY_A -- 密钥A;	KEY_B -- 密钥B;
//		unsigned char *pUID			// 4字节UID
//		unsigned char ucKeySector		// 密钥存放扇区
//		unsigned char ucBlock			// 验证的块号
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	验证后，访问该块所在的扇区(相同权限)均不用再验证密钥    
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
// 函数原型:    unsigned char PiccAuthKey(unsigned char ucKeyType, unsigned char *pUID,unsigned char *pKey, unsigned char ucBlock)
// 函数功能:    Mifare1卡验证命令，验证成功后，以后的通信被加密。
// 入口参数:    unsigned char ucKeyType			// 密钥类型：KEY_A -- 密钥A;	KEY_B -- 密钥B;
//		unsigned char *pUID			// 4字节UID
//		unsigned char *pKey			// 6字节密钥
//		unsigned char ucBlock			// 验证的块号
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	验证后，访问该块所在的扇区(相同权限)均不用再验证密钥    
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
// 函数原型:    unsigned char PiccRead(unsigned char ucBlock, unsigned char *pBuf)
// 函数功能:    Mifare读命令
// 入口参数:    unsigned char ucBlock	; 读的块号
// 出口参数:    unsigned char *pBuf	; 读出的16字节数据
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	-   
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
// 函数原型:    unsigned char PiccWrite( unsigned char ucBlock, unsigned char *pBfr)
// 函数功能:    Mifare写命令
// 入口参数:    unsigned char ucBlock			; 写入的块地址。
//		unsigned char *pBfr			; 16字节写数据缓冲区首址。
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	-   
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
// 函数原型:    unsigned char PiccULWrite( unsigned char ucBlock, unsigned char *pBuf)
// 函数功能:    Mifare写命令
// 入口参数:    unsigned char ucBlock			; 写入的块地址。
//		unsigned char *pBfr			; 4字节写数据缓冲区首址。
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。
// 说    明:	-   
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
// 函数原型:    unsigned char PiccValueOper(unsigned char ucOperMode, unsigned char ucSBlock, 
//		unsigned char *pValue, unsigned char ucDBlock)
// 函数功能:    Mifare值操作命令
// 入口参数:    unsigned char ucOperMode,		; 值操作模式
//		unsigned char ucSBlock			; 要操作的块地址。
//		unsigned char *pValue,			; 4字节要操作的值，低字节在低地址。
//		unsigned char ucDBlock			; 值操作的目地块
// 出口参数:    -
// 返 回 值:    STATUS_SUCCESS -- 成功；其它值 -- 失败。  
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
// 函数原型:    unsigned char PiccActiveTypeA(	unsigned char unMode, unsigned char ucReqCode, 
//		unsigned char *pATQ, unsigned char *pSAK,unsigned char *pUIDLen, unsigned char *pUID))
// 函数功能:    模块信息配置
// 输入参数:    unsigned char unMode			// 0 -- 执行防碰撞循环，1 -- 只执行一次防碰撞
//		unsigned char ucReqCode			// 请求码	
// 输出参数:    unsigned char *pATQ			// 请求回应
//		unsigned char *pSAK			// 最后一级选择应答
//		unsigned char *pUIDLen			// 卡号长度
//		unsigned char *pUID			// 卡号
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PiccReset(unsigned char _1ms)
// 函数功能:    卡片复位(关闭天线载波)
// 输入参数:    unsigned char _1ms				// 关闭载波的时间			
// 输出参数:    -
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PiccAutoDetect(unsigned char ucADMode, unsigned char ucTxMode, 
//		unsigned char ucReqCode, unsigned char ucAuthMode,unsigned char ucKeyType, 
//              unsigned char *pKey,unsigned char ucBlock)
// 函数功能:    A型卡自动检测命令
// 输入参数:    unsigned char ucADMode			// 自动检测模式
//		unsigned char ucTxMode			// 天线驱动模式
//		unsigned char ucReqCode			// 请求模式
//		unsigned char ucAuthMode		// 认证模式
//		unsigned char ucKeyType			// 密钥类型
//		unsigned char *pKey			// 密钥
//		unsigned char ucBlock			// 读的块号
// 输出参数:    unsigned char *pRegValue		// 读出的值
// 返 回 值:    命令执行结果
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
	{// 设置读数据
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
// 函数原型:    unsigned char PiccADRcv(unsigned char *pTxDrv, unsigned char *pATQ, 
//		unsigned char *pSAK, unsigned char *pUIDLen,unsigned char *pUID, unsigned char *pDataBuf)
// 函数功能:    读主动上传的自动检测的数据
// 输入参数:    -
// 输出参数:    unsigned char *pTxDrv			// 那个天线检测到的卡
//							// 0x01--仅TX1;	0x02--仅TX2;	0x03--TX1、TX2
//		unsigned char *pATQ			// 请求回应
//		unsigned char *pSAK			// 选择应答
//		unsigned char *pUIDLen			// UID长度
//		unsigned char *pUID			// UID
//		unsigned char *pDataBuf			// 读出的数据
// 返 回 值:    命令执行结果
// 说    明:	该函数用于自动读取模块自动检测时主动发送的数据
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if PICC_AD_RCV_EN
unsigned char PiccADRcv(unsigned char *pTxDrv, unsigned char *pATQ, unsigned char *pSAK,
						unsigned char *pUIDLen, unsigned char *pUID, unsigned char *pDataBuf)
{
	unsigned char Status = STATUS_IO_TIMEOUT;

	if(Ser_Rcv())
	{	
		if(SEQ == (g_ucSerBuf[CMDTYPE] & 0xF0))
		{//包号匹配
			Status = STATUS_SUCCESS;
			*pTxDrv = g_ucSerBuf[DATA];
			memcpy(pATQ, &g_ucSerBuf[DATA+1], 2);
			*pSAK = g_ucSerBuf[DATA+3];
			*pUIDLen = g_ucSerBuf[DATA+4];
			memcpy(pUID, &g_ucSerBuf[DATA+5], g_ucSerBuf[DATA+4]);

		// 若没有使能读数据，则不用处理下面的数据
			if(g_ucSerBuf[LENGTH] == g_ucSerBuf[DATA+4] + 21)
				memcpy(pDataBuf, &g_ucSerBuf[DATA+5+g_ucSerBuf[DATA+4]], 16);
		}
		else
			Status = STATUS_COMM_SEQ_ERR;	//包号不匹配
	}
	return Status;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char PiccReadADDT( unsigned char ucMode, unsigned char *pTxDrv, 
//		unsigned char *pATQ, unsigned char *pSAK,unsigned char *pUIDLen, unsigned char *pUID, 
//		unsigned char *pDataBuf)
// 函数功能:    读自动检测的数据
// 输入参数:    unsigned char ucMode			// 0x00 -- 停止检测；	0x01 -- 继续检测
// 输出参数:    unsigned char *pTxDrv			// 那个天线检测到的卡
//							// 0x01--仅TX1;	0x02--仅TX2;	0x03--TX1、TX2
//		unsigned char *pATQ			// 请求回应
//		unsigned char *pSAK			// 选择应答
//		unsigned char *pUIDLen			// UID长度
//		unsigned char *pUID			// UID
//		unsigned char *pDataBuf			// 读出的数据
// 返 回 值:    0x00 -- 成功;	0xFF -- 未检测到卡;	0x9C -- 函数未执行			
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





