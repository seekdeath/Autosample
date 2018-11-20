#include <includes.h>
#include "ZLG522S_Dvc.h"
#include "ZLG522S_Comm.h"
#include "StatusCode.h"
#include <string.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char GetDvcInfo(unsigned char *DvcInfo)
// 函数功能:    获取模块信息
// 输入参数:    -		
// 输出参数:    unsigned char *DvcInfo			// 模块信息
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PCDConfig()
// 函数功能:    配置读写卡芯片
// 输入参数:    -		
// 输出参数:    
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PCDClose()
// 函数功能:    模块信息配置
// 输入参数:    -		
// 输出参数:    
// 返 回 值:    命令执行结果
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
// 函数原型:    unsigned char PCDLoadKey(unsigned char ucKeyType, unsigned char ucKeySNr,unsigned char *pKey)
// 函数功能:    装载密钥
// 输入参数:    unsigned char ucKeyType			// 密钥类型	KEY_A;KEY_B	
//		unsigned char ucKeySNr			// 密钥存放扇区0~3
//		unsigned char *pKey			// 6字节密钥
// 输出参数:    -
// 返 回 值:    命令执行结果
// 说    明:	若输入的密钥为6字节，则模块会自动扩展为16字节
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
// 函数原型:    unsigned char PCDWriteReg(unsigned char ucRegAddr,unsigned char ucRegValue)
// 函数功能:    写读写芯片寄存器
// 输入参数:    unsigned char ucRegAddr			// 寄存器地址
//		unsigned char ucRegValue		// 写入的值
// 输出参数:    -
// 返 回 值:    命令执行结果	
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
// 函数原型:    unsigned char PCDReadReg(unsigned char ucRegAddr,unsigned char *pRegValue)
// 函数功能:    读读写芯片寄存器
// 输入参数:    unsigned char ucRegAddr			// 寄存器地址			
// 输出参数:    unsigned char *pRegValue		// 读出的值
// 返 回 值:    命令执行结果
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