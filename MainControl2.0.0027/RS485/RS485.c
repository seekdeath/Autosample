#include "includes.h"

FMT_STATEA  m_bFmtStateA;
FMT_STATEB  m_bFmtStateB;
FMT_SAMPLEA   m_bFmtSampleA;
FMT_SAMPLEB   m_bFmtSampleB;

static OS_EVENT *Uart0Sem;
static int UART0SendBuf[(UART1_SEND_QUEUE_LENGTH + sizeof(int) - 1) / sizeof(int)];
uint16 crc16(uint8 *puchMsg,uint8 usDataLen) // puchMsg message to calculate CRC upon 
{	
  uint16 itemp=0xffff;
  uint8 i;
  while(usDataLen--){
    itemp^=(*puchMsg++)<<8;
    for(i=0;i<8;i++){
      if(itemp&0x8000){
          itemp<<=1;
          itemp^=0x1021;
      }else{
          itemp<<=1;
      }
    }
  }
  return itemp;
}

//===========================================================================
// ��Ascii��ת��Ϊ��������
//===========================================================================
unsigned char AsciiToHex(uint8 c)
{
	if ('0' <= c && c <= '9')
		return c-'0';
	else if ('A' <= c && c <= 'F')
		return c-'A'+0x0A;
	else if ('a' <= c && c <= 'f')
		return c-'a'+0x0A;
	else
		return 0;
}

//===========================================================================
// Ascii�뵽Byteת��
//===========================================================================
uint8 PackByte(uint8* p)
{
	return (AsciiToHex(p[0]) << 4) | AsciiToHex(p[1]);
}
//===========================================================================
// Ascii�뵽WORDת��
//===========================================================================
uint16 PackWord(uint8* p)
{
	return ((unsigned short)PackByte(p) << 8) | PackByte(p + 2);
}
//===========================================================================
// Ascii�뵽floatת��
//===========================================================================

float  AscIIToFloat(uint8* p){
  float m_ftmp;
  uint8 m_dtmp[4];
  m_dtmp[0]=PackByte(p);
  m_dtmp[1]=PackByte(p+2);
  m_dtmp[2]=PackByte(p+4);
  m_dtmp[3]=PackByte(p+6);
  memcpy(&m_ftmp,m_dtmp,4);
  return m_ftmp;
}

void FloatToASCII(float m_fValue,uint8 *m_lpBuf){
     uint8 m_btmp[4];
     uint8 i;
     
     memcpy(m_btmp,&m_fValue,4);
     for(i=0;i<4;i++){
            m_lpBuf[2*i]=HexToASCII232(m_btmp[i]>>4);
            m_lpBuf[2*i+1]=HexToASCII232(m_btmp[i]&0x0f);
      }
}

uint8 HexToASCII232(uint8 c){
	if ( c <= 9)
		return c+'0';
	else if (0x0A <= c && c <= 0x0F)
		return (c-0x0A)+'A';
	else
		return 0;
}
/*********************************************************************************************************
** ��������: Uart4WriteFull
** ��������: ���ݶ���д��������� 
** �䡡��: Buf:ָ�����ݶ���
**        Data:��Ҫ��ӵ�����
**        Mod: ��ӷ�ʽ
** �䡡��:TRUE  :�ɹ�
**        FALSE:ʧ��
** ȫ�ֱ���: ��
** ����ģ��: ��
**
********************************************************************************************************/
uint8 Uart1WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
  {
    uint16 temp;
    
    Mod = Mod;
    
    temp = QueueSize((void *)Buf);
    while (temp <= QueueNData((void *)Buf))         /* �ȴ����ݶ��в��� */
      {
        OSTimeDly(2);
      }
    return QueueWrite((void *)Buf, Data);           /* ����������� */
 
    
  }

/*********************************************************************************************************
** ��������: UART1Putch
** ��������: ����һ���ֽ�����
** �䡡��: Data�����͵���������
** �䡡��:��
** ȫ�ֱ���: ��
** ����ģ��: ��
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void UART1Putch(uint8 Data)
  {
     #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
    CPU_SR         cpu_sr;
     #endif
     CPU_CRITICAL_ENTER();
     /* Write a character to the USART */
     USART_SendData(USART1, (CPU_INT08U) Data);
     while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
     CPU_CRITICAL_EXIT();
  }

/*********************************************************************************************************
** ��������: UART1Write
** ��������: ���Ͷ���ֽ�����
** �䡡��: Data:�������ݴ洢λ��
**        NByte:�������ݸ���
** �䡡��:��
** ȫ�ֱ���: ��
** ����ģ��: UART0Putch
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void UART1Write(uint8 *Data, uint16 NByte)
  {
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif   
    OS_ENTER_CRITICAL();
    while (NByte-- > 0)
      {
        UART1Putch(*Data++);
      }
    OS_EXIT_CRITICAL();
  }

/*********************************************************************************************************
** ��������: UART1Getch
** ��������: ����һ���ֽ�
** �䡡��: ��
** �䡡��: ���յ�������
** ȫ�ֱ���: ��
** ����ģ��: ��
** data==0��һֱ�ȴ����ܣ�data��0���ó�ʱ����
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 UART1Getch(uint16 Data,uint8 *m_dErr)
  {
    uint8 err;
 //   #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
  //   CPU_SR         cpu_sr;
  //  #endif   
 //  OS_ENTER_CRITICAL();
    if ((USART1->SR & USART_FLAG_RXNE) == 0){          /* û���յ����� */
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     /* ��������ж� */
        OSSemPend(Uart0Sem,Data, m_dErr);                      /* �ȴ��������� */
     }
    err =  USART_ReceiveData(USART1);                      /* ��ȡ�յ������� */
  //  OS_EXIT_CRITICAL();
    return err;
  } 

/*********************************************************************************************************
** ��������: RS485SendEnable
** ��������: UART4_485���ͽ��սſ��ƣ�����
** �䡡��: ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** 
**------------------------------------------------------------------------------------------------------
**-------------------------------------------------------------------------------------------------------
** �޸���:    we
** �ա���:    2009.09.19
**------------------------------------------------------------------------------------------------------
********************************************************************************************************

********************************************************************************************************/

void RS485SendEnable (void)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
}


/*********************************************************************************************************
** ��������: RS485SendEnable
** ��������: UART4_485���ͽ��սſ��ƣ�����
** �䡡��: ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** 
**------------------------------------------------------------------------------------------------------
**-------------------------------------------------------------------------------------------------------
** �޸���:    we
** �ա���:    2009.09.19
**------------------------------------------------------------------------------------------------------
********************************************************************************************************

********************************************************************************************************/

void RS485SendDisable (void)
{
  GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);	
}


//----------------------------------------------------------------------------
//�����жϷ������
//----------------------------------------------------------------------------
void RS485_RxTxISRHandle(void){
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif
    uint8   i,temp;
    uint16  m_dSize;
    CPU_CRITICAL_ENTER();                                       
    OSIntNesting++;
    CPU_CRITICAL_EXIT();
  //�����������ʱ��RXNEIEΪ1�����ʱ�����������õ����ݱ������
    if(((USART1->SR&0x08)==0x08)&&((USART1->CR1&0x20)==0x20)){
        USART_ReceiveData(USART1); 
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);  
    }else if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    ///  m_bRecvData= USART_ReceiveData(USART1);                /* �������ݿ��� */
        OSSemPost(Uart0Sem);                                    /* ֪ͨ�������� */
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);         /* ��ֹ���ռ��ַ���ʱ�ж� */       
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);   
        
    }
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
        m_dSize=QueueNData(UART0SendBuf);
        if(m_dSize>1){
           for (i = 0; i < m_dSize; i++){ /* ����FIFO������� */ 
              if (QueueRead(&temp, UART0SendBuf) == QUEUE_OK){
                  USART_SendData(USART1, temp);
                  while (!(USART1->SR & USART_FLAG_TXE));
                  USART_ClearITPendingBit(USART1, USART_IT_TXE);
             }else{
                USART_ITConfig(USART1, USART_IT_TXE, DISABLE);   /* �������ж� */      /* ���пգ����ֹ�����ж� */
             }
          }  
        }else{
          USART_ITConfig(USART1, USART_IT_TXE, DISABLE);   /* �������ж� */ 
          RS485SendDisable();
        }        
    }       
    OSIntExit();

}

uint8 RS485_Init(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
//  USART_ClockInitTypeDef  USART_ClockInitStruct;
  USART_DeInit(USART1);
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA , ENABLE);
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 , ENABLE);
  
  /* Enable GPIOA clock                                                       */
 

  /* Configure USART1 Rx (PA10) as input floating                             */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Tx (PA9) as alternate function push-pull                */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);
  
  USART_InitStructure.USART_BaudRate            = 115200;
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits            = USART_StopBits_1;
  USART_InitStructure.USART_Parity              = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);  
  
  
   if (QueueCreate((void *)UART0SendBuf,
                     sizeof(UART0SendBuf),
                     NULL,
                     (uint8 (*)())Uart1WriteFull)
                     == NOT_OK){
        return FALSE;
    }
    Uart0Sem = OSSemCreate(0);
    if (Uart0Sem == NULL){
        return FALSE;
    }  
    
  BSP_IntVectSet(BSP_INT_ID_USART1, RS485_RxTxISRHandle);    //����modbus�жϷ������
  BSP_IntEn(BSP_INT_ID_USART1); //�����ж�ʹ��     
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  return TRUE;
}

void Uart1_SendAckPkt(uint8 m_bCmd){
     uint8 CommIndex;  
     uint8  CommBuf[60]={0};
     CPU_INT16U crc16tem;
     RS485SendEnable();  //485ʹ��
     OSTimeDly(20);
     CommIndex=0;
     CommBuf[CommIndex++]=FRAME_HEADER;                       
     CommBuf[CommIndex++]=DEVICE_ID;                      
     CommBuf[CommIndex++]=m_bCmd;              
     crc16tem=crc16( CommBuf,CommIndex);                    
     CommBuf[CommIndex++]=HexToASCII232((crc16tem>>12) &0x0f);
     CommBuf[CommIndex++]=HexToASCII232((crc16tem>>8) &0x0f);
     CommBuf[CommIndex++]=HexToASCII232((crc16tem>>4) &0x0f);
     CommBuf[CommIndex++]=HexToASCII232((crc16tem) &0x0f);     
     CommBuf[CommIndex++]=FRAME_END; 
     UART1Write(CommBuf,CommIndex);
     OSTimeDly(10);
     RS485SendDisable();
}


void Uart1_SendADPkt(uint8 *cp,uint8 len){
     uint8 CommIndex;  
     uint8 i;
     uint8  CommBuf[60];
     CPU_INT16U crc16tem;
     RS485SendEnable();  //485ʹ��
     OSTimeDly(20);
     CommIndex=0;
     CommBuf[CommIndex++]=FRAME_HEADER;                       
     CommBuf[CommIndex++]=DEVICE_ID;                      
     CommBuf[CommIndex++]=SampleData;   
     for(i=0;i<len;i++){
           CommBuf[CommIndex++]=*cp++;    
     }
     crc16tem=crc16( CommBuf,CommIndex);                    
     CommBuf[CommIndex++]=HexToASCII232((crc16tem>>12) &0x0f);
     CommBuf[CommIndex++]=HexToASCII232((crc16tem>>8) &0x0f);
     CommBuf[CommIndex++]=HexToASCII232((crc16tem>>4) &0x0f);
     CommBuf[CommIndex++]=HexToASCII232((crc16tem) &0x0f);   
     CommBuf[CommIndex++]=FRAME_END; 
     UART1Write(CommBuf,CommIndex); 
     OSTimeDly(10);
     RS485SendDisable();
}

void Uart1_ProtocolAnalys(uint8 *pBuffer, uint8 m_bLen){//len��>���ӻ���ַ��������ݡ�CRCУ�顢������
      uint8* msg=pBuffer; 
      if (DEVICE_ID==DEVICE_AP6A)       //AP6  A��
      {
          switch(pBuffer[1]){
          case GetVersion:	        // ��ȡ�汾��Ϣ
               break;     
          case GetError:	        // ��ȡ����
               break;
          case OutPut_TC28A:            //A��TC+28V���
              Uart1_SendAckPkt(OutPut_TC28A);
              m_bFmtStateA.m_bControl_TC28VA= msg[2];
              if(m_bFmtStateA.m_bControl_TC28VA=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
              else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
              break;
          case OutPut_28A:              //A��+28V���
                Uart1_SendAckPkt(OutPut_28A);
                m_bFmtStateA.m_bControl_28VA = msg[2];
                if(m_bFmtStateA.m_bControl_28VA=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
                else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
            break;
          case OutControl_10M:          //10M���Ⱦ����������
                Uart1_SendAckPkt(OutControl_10M);
                m_bFmtStateA.m_bControl_10M_Out= msg[2];
                if(m_bFmtStateA.m_bControl_10M_Out=='1'){
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
                }
                else 
                {
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);
                }
            break;
          case OnPowerControl_10M:      //10M���Ⱦ���ӵ����
                Uart1_SendAckPkt(OnPowerControl_10M);
                m_bFmtStateA.m_bControl_10M_OnP= msg[2];
                if(m_bFmtStateA.m_bControl_10M_OnP=='1'){
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
                }
                else{ 
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
                }
            break;
          case SampleData:              //�ɼ�+28V��ѹ�͵�����ֵ
              Uart1_SendADPkt(m_bFmtSampleA.m_bSampleVolt28vADI1,16);
            break;
          default:
            break;
          }
      }
      else if (DEVICE_ID==DEVICE_AP6B)
      {
          switch(pBuffer[1]){
          case GetVersion:	        // ��ȡ�汾��Ϣ
               break;     
          case GetError:	        // ��ȡ����
               break;
          case OutPut_TC28B:            //B��TC+28V���
              Uart1_SendAckPkt(OutPut_TC28B);
              m_bFmtStateB.m_bControl_TC28VB= msg[2];
              if(m_bFmtStateB.m_bControl_TC28VB=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
              else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
            break;
          case OutPut_28B:              //B��+28V���
                Uart1_SendAckPkt(OutPut_28B);
                m_bFmtStateB.m_bControl_28VB = msg[2];
                if(m_bFmtStateB.m_bControl_28VB=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
                else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
            break;
          case OutPutOnOff_Control:     //OUTPUT ON/OFF �ӵ����
                Uart1_SendAckPkt(OutPutOnOff_Control);
                m_bFmtStateB.m_bControl_OUTPUT= msg[2];
                if(m_bFmtStateB.m_bControl_OUTPUT=='1'){
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
                    OSTimeDly(1000);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
                }
                else {
                    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
                    OSTimeDly(1000);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
                }
            break;
          case SampleData_28V:      //B��+28V��ѹ����ֵ
                Uart1_SendADPkt(m_bFmtSampleB.m_bSampleVolt28vADI1,16);
                break;
          case SampleData_SecVolt:  //�ɼ�B�����ε�ѹֵ
                Uart1_SendADPkt(m_bFmtSampleB.m_bSample2VoltADI7,32);
                break;
          case SampleData_SecCur:   //�ɼ�B�����ε���ֵ
                Uart1_SendADPkt(m_bFmtSampleB.m_bSample2IAADIB,32);
                break;
          case DDS2_OnP:            //DDS2�ӵ����
                Uart1_SendAckPkt(DDS2_OnP);
                m_bFmtStateB.m_bControl_DDS2Onp=msg[2];
                if(m_bFmtStateB.m_bControl_DDS2Onp=='1'){
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
                }
                else{ 
                    GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
                    GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
                }
                break;
          default:
            break;
         }
      }
}