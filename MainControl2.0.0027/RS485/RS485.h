#ifndef _RS485_h_
#define _RS485_h_

#define FRAME_HEADER  '>'
#define FRAME_END     '\r'
#define DEVICE_ID     'F'

#define DEVICE_AP6A   'E'
#define DEVICE_AP6B   'F'

#define OutPut_TC28A            'C'
#define OutPut_28A              'D'
#define OutControl_10M          'E'
#define OnPowerControl_10M      'F'
#define SampleData              'G'


#define OutPut_TC28B            'C'
#define OutPut_28B              'D'
#define OutPutOnOff_Control     'E'
#define SampleData_28V          'F'
#define SampleData_SecVolt      'G'
#define SampleData_SecCur       'H'
#define DDS2_OnP                'I'

#define GetVersion   '1'
#define GetError     '0'



uint16   crc16(uint8 *puchMsg,uint8 usDataLen);
uint8    AsciiToHex(uint8 c);
uint8    PackByte(uint8* p);
uint16   PackWord(uint8* p);
float    AscIIToFloat(uint8* p);
uint8    HexToASCII232(uint8 c);
void      FloatToASCII(float m_fValue,uint8 *m_lpBuf);
void     UART1Putch(uint8 Data);
void     UART1Write(uint8 *Data, uint16 NByte);
uint8    UART1Getch(uint16 Data,uint8 *m_dErr);
void     RS485SendEnable (void);
void     RS485SendDisable (void);
void     RS485_RxTxISRHandle(void);
uint8    RS485_Init(void);
void Uart1_SendAckPkt(uint8 m_bCmd);
void Uart1_SendADPkt(uint8 *cp,uint8 len);
void     Uart1_ProtocolAnalys(uint8 *pBuffer, uint8 m_bLen);
#endif