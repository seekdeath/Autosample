#include <I2C.h>
#include <includes.h>
#include <ZLG522S_Comm.h>
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  /* Configure I2C1 pins: SCL and SDA */
/*  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void I2C_delay(void)
{	
   u8 i = 100; //这里可以优化速度	，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   } 
}

bool I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	SCL_L;
	I2C_delay();
	return TRUE;
}

void I2C_Stop(void)
{
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

void I2C_Ack(void)
{	
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

void I2C_NoAck(void)
{	
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

bool I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
          SCL_L;
          return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;
}

void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    while(i--)
    {
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
	SCL_H;
        I2C_delay();
        SCL_L;
        I2C_delay();
    }
}

u8 I2C_ReceiveByte(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
      SCL_L;
      I2C_delay();
    }
    return ReceiveByte;
}

/*
bool I2C_BufferWrite(u8* pBuffer,u16 length,u8 DeviceAddress)
{
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}	  
    while(length--)
    {
	I2C_SendByte(*pBuffer);
	if (!I2C_WaitAck()){I2C_Stop(); return FALSE;}
        pBuffer++;
    }
    I2C_Stop();
	  //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
    return TRUE;
}*/

//注意不能跨页写
//写入1串数据      待写入数组地址    待写入长度      待写入地址       器件类型(24c16或SD2403)
bool I2C_BufferWrite(u8* pBuffer, u8 length,     u16 WriteAddress, u8 DeviceAddress)
{
    if(!I2C_Start())return FALSE;
    I2C_SendByte(((WriteAddress & 0x0700) >>7) | DeviceAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((u8)(WriteAddress & 0x00FF));   //设置低起始地址      
	  I2C_WaitAck();	
	  
		while(length--)
		{
		  I2C_SendByte(* pBuffer);
		  I2C_WaitAck();
                  pBuffer++;
		}
	  I2C_Stop();
	  //注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
	  //Systick_Delay_1ms(10);
          OSTimeDly(10);
	  return TRUE;
}

extern OS_EVENT *I2CSem;
bool I2C_ReadByte(u8* pBuffer,  u8 DeviceAddress)
{	
    u8 ucLen;
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress|0x01);//器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    OSTimeDlyHMSM (0, 0, 0, 5);
    *pBuffer = I2C_ReceiveByte();
    I2C_Ack();
    ucLen =  *pBuffer - 2;
    pBuffer++;
    if ((ucLen < PRTCLEN - 2) || (ucLen > MAX_SER_SIZE -2))
    {
        I2C_Stop();
        return FALSE;
    }
    for(; ucLen != 0 ; ucLen--)
    {
	*pBuffer = I2C_ReceiveByte();   			// 接收一字节数据并发送应答位/
        I2C_Ack();
        pBuffer++;
    } 
    *pBuffer = I2C_ReceiveByte();
    I2C_NoAck();
    I2C_Stop();
    return TRUE;
}


//读出1串数据         存放读出数据  待读出长度      待读出地址       器件类型(24c16或SD2403)	
bool I2C_ReadBuffer(u8* pBuffer,   u8 length,     u16 ReadAddress,  u8 DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | DeviceAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((u8)(ReadAddress & 0x00FF));   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | DeviceAddress | 0x0001);
    I2C_WaitAck();
    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)I2C_NoAck();
      else I2C_Ack(); 
      pBuffer++;
      length--;
    }
    I2C_Stop();
    return TRUE;
}




//跨页写入1串数据  待写入数组地址    待写入长度      待写入地址       器件类型(24c16或SD2403)
void I2C_PageWrite(  u8* pBuffer, u8 length,     u16 WriteAddress, u8 DeviceAddress)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr  = WriteAddress % I2C_PageSize;      //写入地址是开始页的第几位
    count = I2C_PageSize - Addr;					    //在开始页要写入的个数
    NumOfPage   =  length / I2C_PageSize;     //要写入的页数
    NumOfSingle =  length % I2C_PageSize;     //不足一页的个数

    if(Addr == 0)         //写入地址是页的开始 
    {
      if(NumOfPage == 0)  //数据小于一页 
      {
        I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //写少于一页的数据
      }
      else		            //数据大于等于一页  
      {
        while(NumOfPage)//要写入的页数
        {
          I2C_BufferWrite(pBuffer,I2C_PageSize,WriteAddress,DeviceAddress);//写一页的数据
          WriteAddress +=  I2C_PageSize;
          pBuffer      +=  I2C_PageSize;
					NumOfPage--;
					OSTimeDly(10);
         }
        if(NumOfSingle!=0)//剩余数据小于一页
        {
          I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //写少于一页的数据
					OSTimeDly(10);
        }
       }
    }

    else                  //写入地址不是页的开始 
    {
      if(NumOfPage== 0)   //数据小于一页 
      {
        I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //写少于一页的数据
      }
      else                //数据大于等于一页
      {
        length       -= count;
        NumOfPage     = length / I2C_PageSize;  //重新计算要写入的页数
        NumOfSingle   = length % I2C_PageSize;  //重新计算不足一页的个数	
      
        if(count != 0)
        {  
				  I2C_BufferWrite(pBuffer,count,WriteAddress,DeviceAddress);      //将开始的空间写满一页
          WriteAddress += count;
          pBuffer      += count;
         } 
      
        while(NumOfPage--)  //要写入的页数
        {
          I2C_BufferWrite(pBuffer,I2C_PageSize,WriteAddress,DeviceAddress);//写一页的数据
          WriteAddress +=  I2C_PageSize;
          pBuffer      +=  I2C_PageSize; 
        }
        if(NumOfSingle != 0)//剩余数据小于一页
        {
          I2C_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //写少于一页的数据 
        }
      }
    } 
}