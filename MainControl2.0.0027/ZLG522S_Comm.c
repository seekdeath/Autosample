#include <includes.h>
#include "StatusCode.h"
#include "ZLG522S.h"
#include "ZLG522S_Comm.h"
#include "I2C.h"
extern    OS_EVENT       *UARTSem;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ZLG522S_CommInit(void))
// 函数功能:    ZLG522S主控通信接口初始化
// 输入参数:    -		
// 输出参数:    
// 返 回 值:    命令执行结果
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*unsigned char ZLG522S_CommInit(void)
{
	Setupuart1();
	if (SetModeBaud()==TRUE)
          return STATUS_COMM_OK;
        else
	  return STATUS_COMM_ERR;
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    bit SetModeBaud(void)
// 函数功能:    设置模块的波特率
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    1--执行成功；0--执行失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*unsigned char  SetModeBaud(void)
{
//	unsigned char err;
	unsigned char SetCunt = 3;
        int           temp;
	while(SetCunt)
	{
		Sendchar(USART1,STX);
                for(temp= 4000;temp != 0;temp--);		// 延时30us
                Sendchar(USART1,STX);
                while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET)
                {
                    if (temp++>=20)break;
                    OSTimeDly(1);
                }
                if (temp<=20)
		{
		    temp=USART_ReceiveData(USART1);
	            if(temp == ACK)
		      return TRUE;
		    else
		    {
			SetCunt--;
			continue;
		    }
		}
                else
                {
                        USART_ReceiveData(USART1);
			break;
                }
                    
	}
	return FALSE;
}*/
extern OS_EVENT  *I2CSem;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ZLG522S_Cmd(void)
// 函数功能:    ZLG522S模块命令执行函数
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ZLG522S_Cmd(void)
{
	unsigned char ucStatus=STATUS_COMM_ERR;//err;
		
	SEQ += 0x10;
	g_ucSerBuf[CMDTYPE] |= SEQ;	// 命令类型与包号组成一个字节
	if(!Ser_Send())	
	  return STATUS_COMM_ERR;
        while (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5));
        I2C_ReadByte(g_ucSerBuf,0xB2);
        SerDTvalid=TRUE;
	    if(!Ser_Rcv())
              ucStatus=STATUS_COMM_SEQ_ERR;
            else
            {	
                  if(SEQ == (g_ucSerBuf[CMDTYPE]&0xf0))
                  {							// 包号匹配
                          ucStatus = STATUS_COMM_OK;
                  }
                  else
                          ucStatus = STATUS_COMM_SEQ_ERR;		// 包号不匹配
            }
 //       }
	return ucStatus;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    bit Ser_Send(void)
// 函数功能:    发送程序
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    TRUE--发送成功；FALSE--发送失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Ser_Send(void)
{
    unsigned char i,BCC;

    g_ucSerBuf[FRAMELEN] = g_ucSerBuf[LENGTH] + PRTCLEN;	//帧长度等于数据长度加协议长度
    BCC = 0;
    for(i=0; i<(g_ucSerBuf[FRAMELEN]-2); i++)
    BCC ^= g_ucSerBuf[i];
    g_ucSerBuf[g_ucSerBuf[FRAMELEN]-2] = ~BCC;
    g_ucSerBuf[g_ucSerBuf[FRAMELEN]-1] = ETX;	
    g_ucSerBuf[g_ucSerBuf[FRAMELEN]] = 0x00;                            
    g_ucSerBufIdx = FRAMELEN;						// 设置帧起始地址
    {// 使用I2C通信	/.
	SerDTvalid = FALSE;						// 接收的数据无效
	//ZLGSendStr(USART1,g_ucSerBuf);
//        return I2C_BufferWrite(g_ucSerBuf,g_ucSerBuf[FRAMELEN],0xB2);
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    bit Ser_Rcv(void)
// 函数功能:    接收程序
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    TRUE--接收成功；FALSE--接收失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Ser_Rcv(void)
{
	unsigned char i,BCC;
	if (SerDTvalid)
	{											// 已接收一帧数据,进行数据校验
		if(g_ucSerBuf[g_ucSerBuf[FRAMELEN]-1] == ETX && g_ucSerBuf[FRAMELEN] == (g_ucSerBuf[LENGTH]+PRTCLEN))
		{// 正确接收到ETX,及帧长度等于数据长度加协议长度,则数据有效
			BCC = 0;
			for(i=0; i<(g_ucSerBuf[FRAMELEN]-1); i++)
			{
				BCC ^= g_ucSerBuf[i];
			}
			if(BCC == 0xff)
			{
				SerDTvalid = FALSE;				// BCC校验通过
				return TRUE;				
			}
		}
		SerDTvalid = FALSE;
	}
	return FALSE;
}


