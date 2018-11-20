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
// 将Ascii码转换为二进制数
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
// Ascii码到Byte转换
//===========================================================================
uint8 PackByte(uint8* p)
{
	return (AsciiToHex(p[0]) << 4) | AsciiToHex(p[1]);
}
//===========================================================================
// Ascii码到WORD转换
//===========================================================================
uint16 PackWord(uint8* p)
{
	return ((unsigned short)PackByte(p) << 8) | PackByte(p + 2);
}
//===========================================================================
// Ascii码到float转换
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
** 函数名称: Uart4WriteFull
** 功能描述: 数据队列写满处理程序 
** 输　入: Buf:指向数据队列
**        Data:将要入队的数据
**        Mod: 入队方式
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: 无
** 调用模块: 无
**
********************************************************************************************************/
uint8 Uart1WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
  {
    uint16 temp;
    
    Mod = Mod;
    
    temp = QueueSize((void *)Buf);
    while (temp <= QueueNData((void *)Buf))         /* 等待数据队列不满 */
      {
        OSTimeDly(2);
      }
    return QueueWrite((void *)Buf, Data);           /* 数据重新入队 */
 
    
  }

/*********************************************************************************************************
** 函数名称: UART1Putch
** 功能描述: 发送一个字节数据
** 输　入: Data：发送的数据数据
** 输　出:无
** 全局变量: 无
** 调用模块: 无
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
** 函数名称: UART1Write
** 功能描述: 发送多个字节数据
** 输　入: Data:发送数据存储位置
**        NByte:发送数据个数
** 输　出:无
** 全局变量: 无
** 调用模块: UART0Putch
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
** 函数名称: UART1Getch
** 功能描述: 接收一个字节
** 输　入: 无
** 输　出: 接收到的数据
** 全局变量: 无
** 调用模块: 无
** data==0，一直等待接受，data》0设置超时周期
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 UART1Getch(uint16 Data,uint8 *m_dErr)
  {
    uint8 err;
 //   #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
  //   CPU_SR         cpu_sr;
  //  #endif   
 //  OS_ENTER_CRITICAL();
    if ((USART1->SR & USART_FLAG_RXNE) == 0){          /* 没有收到数据 */
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);     /* 允许接收中断 */
        OSSemPend(Uart0Sem,Data, m_dErr);                      /* 等待接收数据 */
     }
    err =  USART_ReceiveData(USART1);                      /* 读取收到的数据 */
  //  OS_EXIT_CRITICAL();
    return err;
  } 

/*********************************************************************************************************
** 函数名称: RS485SendEnable
** 功能描述: UART4_485发送接收脚控制，发送
** 输　入: 无
** 输　出: 无
** 全局变量: 无
** 调用模块: 
** 
**------------------------------------------------------------------------------------------------------
**-------------------------------------------------------------------------------------------------------
** 修改人:    we
** 日　期:    2009.09.19
**------------------------------------------------------------------------------------------------------
********************************************************************************************************

********************************************************************************************************/

void RS485SendEnable (void)
{
    GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_SET);
}


/*********************************************************************************************************
** 函数名称: RS485SendEnable
** 功能描述: UART4_485发送接收脚控制，接收
** 输　入: 无
** 输　出: 无
** 全局变量: 无
** 调用模块: 
** 
**------------------------------------------------------------------------------------------------------
**-------------------------------------------------------------------------------------------------------
** 修改人:    we
** 日　期:    2009.09.19
**------------------------------------------------------------------------------------------------------
********************************************************************************************************

********************************************************************************************************/

void RS485SendDisable (void)
{
  GPIO_WriteBit(GPIOA,GPIO_Pin_11,Bit_RESET);	
}


//----------------------------------------------------------------------------
//串口中断服务程序
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
  //当数据溢出的时候，RXNEIE为1，这个时候数据是无用的数据必须读出
    if(((USART1->SR&0x08)==0x08)&&((USART1->CR1&0x20)==0x20)){
        USART_ReceiveData(USART1); 
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);  
    }else if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    ///  m_bRecvData= USART_ReceiveData(USART1);                /* 接收数据可用 */
        OSSemPost(Uart0Sem);                                    /* 通知接收任务 */
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);         /* 禁止接收及字符超时中断 */       
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);   
        
    }
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {
        USART_ClearITPendingBit(USART1, USART_IT_TXE);
        m_dSize=QueueNData(UART0SendBuf);
        if(m_dSize>1){
           for (i = 0; i < m_dSize; i++){ /* 向发送FIFO填充数据 */ 
              if (QueueRead(&temp, UART0SendBuf) == QUEUE_OK){
                  USART_SendData(USART1, temp);
                  while (!(USART1->SR & USART_FLAG_TXE));
                  USART_ClearITPendingBit(USART1, USART_IT_TXE);
             }else{
                USART_ITConfig(USART1, USART_IT_TXE, DISABLE);   /* 允许发送中断 */      /* 队列空，则禁止发送中断 */
             }
          }  
        }else{
          USART_ITConfig(USART1, USART_IT_TXE, DISABLE);   /* 允许发送中断 */ 
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
    
  BSP_IntVectSet(BSP_INT_ID_USART1, RS485_RxTxISRHandle);    //设置modbus中断服务程序
  BSP_IntEn(BSP_INT_ID_USART1); //串口中断使能     
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  return TRUE;
}

void Uart1_SendAckPkt(uint8 m_bCmd){
     uint8 CommIndex;  
     uint8  CommBuf[60]={0};
     CPU_INT16U crc16tem;
     RS485SendEnable();  //485使能
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
     RS485SendEnable();  //485使能
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

void Uart1_ProtocolAnalys(uint8 *pBuffer, uint8 m_bLen){//len、>、从机地址、命令、数据、CRC校验、结束符
      uint8* msg=pBuffer; 
      if (DEVICE_ID==DEVICE_AP6A)       //AP6  A板
      {
          switch(pBuffer[1]){
          case GetVersion:	        // 读取版本信息
               break;     
          case GetError:	        // 读取错误
               break;
          case OutPut_TC28A:            //A机TC+28V输出
              Uart1_SendAckPkt(OutPut_TC28A);
              m_bFmtStateA.m_bControl_TC28VA= msg[2];
              if(m_bFmtStateA.m_bControl_TC28VA=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
              else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
              break;
          case OutPut_28A:              //A机+28V输出
                Uart1_SendAckPkt(OutPut_28A);
                m_bFmtStateA.m_bControl_28VA = msg[2];
                if(m_bFmtStateA.m_bControl_28VA=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
                else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
            break;
          case OutControl_10M:          //10M高稳晶振输出控制
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
          case OnPowerControl_10M:      //10M高稳晶振加电控制
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
          case SampleData:              //采集+28V电压和电流数值
              Uart1_SendADPkt(m_bFmtSampleA.m_bSampleVolt28vADI1,16);
            break;
          default:
            break;
          }
      }
      else if (DEVICE_ID==DEVICE_AP6B)
      {
          switch(pBuffer[1]){
          case GetVersion:	        // 读取版本信息
               break;     
          case GetError:	        // 读取错误
               break;
          case OutPut_TC28B:            //B机TC+28V输出
              Uart1_SendAckPkt(OutPut_TC28B);
              m_bFmtStateB.m_bControl_TC28VB= msg[2];
              if(m_bFmtStateB.m_bControl_TC28VB=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
              else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
            break;
          case OutPut_28B:              //B机+28V输出
                Uart1_SendAckPkt(OutPut_28B);
                m_bFmtStateB.m_bControl_28VB = msg[2];
                if(m_bFmtStateB.m_bControl_28VB=='1')
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
                else 
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
            break;
          case OutPutOnOff_Control:     //OUTPUT ON/OFF 加电控制
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
          case SampleData_28V:      //B机+28V电压电流值
                Uart1_SendADPkt(m_bFmtSampleB.m_bSampleVolt28vADI1,16);
                break;
          case SampleData_SecVolt:  //采集B机二次电压值
                Uart1_SendADPkt(m_bFmtSampleB.m_bSample2VoltADI7,32);
                break;
          case SampleData_SecCur:   //采集B机二次电流值
                Uart1_SendADPkt(m_bFmtSampleB.m_bSample2IAADIB,32);
                break;
          case DDS2_OnP:            //DDS2加电控制
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