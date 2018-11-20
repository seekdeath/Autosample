#include <includes.h>
#include "StatusCode.h"
#include "ZLG522S.h"
#include "ZLG522S_Comm.h"
#include "I2C.h"
extern    OS_EVENT       *UARTSem;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    unsigned char ZLG522S_CommInit(void))
// ��������:    ZLG522S����ͨ�Žӿڳ�ʼ��
// �������:    -		
// �������:    
// �� �� ֵ:    ����ִ�н��
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
// ����ԭ��:    bit SetModeBaud(void)
// ��������:    ����ģ��Ĳ�����
// �������:    -		
// �������:    -
// �� �� ֵ:    1--ִ�гɹ���0--ִ��ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*unsigned char  SetModeBaud(void)
{
//	unsigned char err;
	unsigned char SetCunt = 3;
        int           temp;
	while(SetCunt)
	{
		Sendchar(USART1,STX);
                for(temp= 4000;temp != 0;temp--);		// ��ʱ30us
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
// ����ԭ��:    unsigned char ZLG522S_Cmd(void)
// ��������:    ZLG522Sģ������ִ�к���
// �������:    -		
// �������:    -
// �� �� ֵ:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ZLG522S_Cmd(void)
{
	unsigned char ucStatus=STATUS_COMM_ERR;//err;
		
	SEQ += 0x10;
	g_ucSerBuf[CMDTYPE] |= SEQ;	// ����������������һ���ֽ�
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
                  {							// ����ƥ��
                          ucStatus = STATUS_COMM_OK;
                  }
                  else
                          ucStatus = STATUS_COMM_SEQ_ERR;		// ���Ų�ƥ��
            }
 //       }
	return ucStatus;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    bit Ser_Send(void)
// ��������:    ���ͳ���
// �������:    -		
// �������:    -
// �� �� ֵ:    TRUE--���ͳɹ���FALSE--����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Ser_Send(void)
{
    unsigned char i,BCC;

    g_ucSerBuf[FRAMELEN] = g_ucSerBuf[LENGTH] + PRTCLEN;	//֡���ȵ������ݳ��ȼ�Э�鳤��
    BCC = 0;
    for(i=0; i<(g_ucSerBuf[FRAMELEN]-2); i++)
    BCC ^= g_ucSerBuf[i];
    g_ucSerBuf[g_ucSerBuf[FRAMELEN]-2] = ~BCC;
    g_ucSerBuf[g_ucSerBuf[FRAMELEN]-1] = ETX;	
    g_ucSerBuf[g_ucSerBuf[FRAMELEN]] = 0x00;                            
    g_ucSerBufIdx = FRAMELEN;						// ����֡��ʼ��ַ
    {// ʹ��I2Cͨ��	/.
	SerDTvalid = FALSE;						// ���յ�������Ч
	//ZLGSendStr(USART1,g_ucSerBuf);
//        return I2C_BufferWrite(g_ucSerBuf,g_ucSerBuf[FRAMELEN],0xB2);
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ����ԭ��:    bit Ser_Rcv(void)
// ��������:    ���ճ���
// �������:    -		
// �������:    -
// �� �� ֵ:    TRUE--���ճɹ���FALSE--����ʧ��
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Ser_Rcv(void)
{
	unsigned char i,BCC;
	if (SerDTvalid)
	{											// �ѽ���һ֡����,��������У��
		if(g_ucSerBuf[g_ucSerBuf[FRAMELEN]-1] == ETX && g_ucSerBuf[FRAMELEN] == (g_ucSerBuf[LENGTH]+PRTCLEN))
		{// ��ȷ���յ�ETX,��֡���ȵ������ݳ��ȼ�Э�鳤��,��������Ч
			BCC = 0;
			for(i=0; i<(g_ucSerBuf[FRAMELEN]-1); i++)
			{
				BCC ^= g_ucSerBuf[i];
			}
			if(BCC == 0xff)
			{
				SerDTvalid = FALSE;				// BCCУ��ͨ��
				return TRUE;				
			}
		}
		SerDTvalid = FALSE;
	}
	return FALSE;
}


