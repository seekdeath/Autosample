#include <includes.h>
void Setupuart1();
void Setupuart2();
void Setupuart3();
void Setupuart4();
void Setupuart5();
void Sendchar(USART_TypeDef* USARTx,unsigned char ch);
void ZLGSendStr(USART_TypeDef* USARTx,unsigned char *buf);
void  UART1_SendStr(char *pBuffer, unsigned int Size);
void UART2_SendStr(char *pBuffer, unsigned int Size);

