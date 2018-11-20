#include <includes.h>
#include <Agreement.h>
#include <Motortrasmition.h>
#include <uart.h>
extern  OS_EVENT       *UartSem;
extern  OS_EVENT       *MutexUart2;
#ifndef NULL
#define NULL			(void*)0
#endif
char aRxBuff1[FRAME_SIZE];
char aTxBuff1[FRAME_SIZE];
unsigned char NRxBuff1;
unsigned char NTxBuff1;
unsigned char StBao1;

char IsFrame1Ok(void)
{
    return StBao1;
}

void ResetFrame1(void)
{
        //int i;
        StBao1=0;
	NRxBuff1=0;
        /*for (i=0;i<FRAME_SIZE;i++)
          aRxBuff1[i] = 0x00;*/
}

//****************************************************************************
//接收数据包处理，向缓冲区中写入接收到的数据
//****************************************************************************
void ReceiverData1(INT8U sq0)
{
	if (FRAME_OK1())	// 上一次接收的包还没处理完, 拒绝接收
		return;

	if (sq0 == '>')
		NRxBuff1 = 0;
        
	aRxBuff1[NRxBuff1++] = sq0;				// 保存数据
	if((NRxBuff1==1)&&(sq0!='>'))			// 包头错误
		NRxBuff1 = 0;
	else if(NRxBuff1==FRAME_SIZE)
		NRxBuff1 = 0;
	else if ((sq0=='\r'))				// 如果检测到包尾则处理
	{
                StBao1=1;
	}
}

/**
按给定的格式发送一条指令.
@param[in, out] pReceiveBuf		返回数据的存放地址, 大小不小于FRAME_SIZE, 当不需要返回数据时, 该参数可传递NULL
@param[in]	lpszFormat			输出格式. lpszFormat只包含指令和数据部分. \see printf
@retval 接收到的数据帧长度, 为0时接收失败.
*/
char SendCommand1(char* pReceiveBuf, const char* lpszFormat, ...)
{
	char* p = aTxBuff1;					// 发送缓冲区
	INT8U err;
	va_list va;
        OSMutexPend(MutexUart2, 0, &err);
        if (err==OS_NO_ERR)
        {
	// 准备指令
	va_start(va, lpszFormat);
	*p++ = '>';							// 起始符
	p += vsprintf(p, lpszFormat, va);
	va_end(va);
	p += sprintf(p, "%04X\r\r\r", cal_crc((INT8U*)aTxBuff1, p-aTxBuff1));
	// 发送指令
        //OSSchedLock ();
        UART2_SendStr(aTxBuff1,p-aTxBuff1);
	//OSSchedUnlock ();
        
	// 等待下位机回复
	err = wait_back(aTxBuff1[1]);
        //FRAME_END1(aRxBuff1,NRxBuff1);
        FRAME_RESET1();
        }
        OSMutexPost(MutexUart2);
	return err;
}

/**
等待下位机回复.
@retval 接收到的数据帧长度. 0: 失败.
*/
char wait_back(char nAddr)
{
	INT8U err=0;

	// 等待接收帧
	while (!IsFrame1Ok())
        {
            OSTimeDly(5);
            err=err+1;
            if (err==20)break;
        }

	if (err <20)
	{
		//校验
		if (NRxBuff1 < 6 ||	// 帧最小长度
			aRxBuff1[1] != nAddr ||
			PackWord(FRAME_CRC1) != cal_crc((INT8U*)aRxBuff1, NRxBuff1-5))
		{
//			FRAME_RESET1();
			return 0;
		}
		return NRxBuff1;
	}
	return 0;
}

char MotorGetStatus(char nAddr, u16* pStatus)
{
	char buf[FRAME_SIZE];
	char nLen;
	if (!(nLen = SendCommand1(buf, "%cB", nAddr)))
		return 0;
        nLen=nLen;
	*pStatus = PackWord(FRAME_DATA1);
	return 1;
}

char MotorReset(char nAddr)
{
	return SendCommand1(NULL, "%cD", nAddr);
}

char MotorSetConfig(char nAddr, u16 nConfig, u16 nTimeoutStep)
{
	return SendCommand1(NULL, "%cC%04X%04X", nAddr, nConfig, nTimeoutStep);
}

char MotorCancel(char nAddr)
{
	return SendCommand1(NULL, "%cE", nAddr);
}

char MotorUnlock(char nAddr, char bUnlock)
{
	return SendCommand1(NULL, "%cF%X", nAddr, bUnlock & 0xF);
}

char MotorGetLastStep(char nAddr, s32* pStep)
{
	char buf[FRAME_SIZE];
	char nLen;

	if (!(nLen = SendCommand1(buf, "%cG", nAddr)))
		return 0;
        nLen=nLen;
	*pStep = PackDword(FRAME_DATA1);
	return 1;
}

char MotorSetSpeed(char nAddr, u16 nStart, u16 nEnd, u8 nStep)
{
	return SendCommand1(NULL, "%cH%04X%04X%02X", nAddr, nStart, nEnd, nStep);
}


char MotorSetHighSpeed(char nAddr, u16 nStart, u16 nEnd, u16 nStep)
{
	return SendCommand1(NULL, "%cH%04X%04X%04X", nAddr, nStart, nEnd, nStep);
}


char MotorRun(char nAddr, char bRun)
{
	return SendCommand1(NULL, "%cI%X", nAddr, bRun & 0xF);
}

char MotorMoveTo(char nAddr, s32 nPos)
{
	return SendCommand1(NULL, "%cJ%08lX", nAddr, nPos);
}

char MotorGetPos(char nAddr, s32* pPos)
{
	char buf[FRAME_SIZE];
	char nLen;

	if (!(nLen = SendCommand1(buf, "%cK", nAddr)))
		return 0;
        nLen=nLen;
	*pPos = PackDword(FRAME_DATA1);
	return 1;
}

char MotorShake(char nAddr, s16 nAmp, u16 nFreq)
{
	return SendCommand1(NULL, "%cL%04X%04X", nAddr, nAmp, nFreq);
}

char MotorGetNegPos(char nAddr, s32* pPos)
{
	char buf[FRAME_SIZE];
	char nLen;

	if (!(nLen = SendCommand1(buf, "%cM", nAddr)))
		return 0;
        nLen=nLen;
	*pPos = PackDword(FRAME_DATA1);
	return 1;
}

char MotorGetSensor(char nAddr, u16* pSensor)
{
	char buf[FRAME_SIZE];
	char nLen;

	if (!(nLen = SendCommand1(buf, "%cP", nAddr)))
		return 0;
        nLen=nLen;
	*pSensor = PackWord(FRAME_DATA1);
	return 1;
}

char MotorTestLevel(char nAddr, s32 nOffset, s32 nMaxPos)
{
	if (!SendCommand1(NULL, "%cQ0%08lX", nAddr, nMaxPos))
		return 0;

	return SendCommand1(NULL, "%cQ1%08lX", nAddr, nOffset);
}

char MotorSetValve(char nAddr, u8 bLock)
{
	return SendCommand1(NULL, "%cR%02hX", nAddr, bLock);
}

char MotorSetSpeedLevel(char nAddr, u8 low , u8 high , u8 hold)
{
	return SendCommand1(NULL, "%cT%02hX%02hX%02hX", nAddr, low , high , hold);
}

char MotorSetSpeedK(char nAddr, u16 SpeedK,u16 ISensitive)
{
	return SendCommand1(NULL, "%cU%04hX%04hX", nAddr, SpeedK , ISensitive);
}


