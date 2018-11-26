#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <Agreement.h>
#include  <ucos_ii.h>
#include  <uart.h>
#include  <stm32f10x_lib.h>
#include  <stm32f10x_systick.h>
#include  <stm32f10x_usart.h>

#include  <Motor.h>
#include  <MotionErr.h>
#include  <Motortrasmition.h>
#include  <i2c.h>
#include  <MotorTrasmition.h>
//#include  <motor.h>

#define N_XY_BAO 50                             //通讯包的最大字节数
#define HAND_BAO 0x3E                           //数据包头
#define HAND_END 0x0D                           //收到数据校验正确
#define ADDR    'A'

char aRxBuff[N_XY_BAO]={0};			// 接收数据缓冲区
char aTxBuff[N_XY_BAO]={0};		        // 发送数据缓冲区
unsigned char NRxBuff=0;			// 接收帧长度, 字节数
unsigned char NTxBuff=0;			// 发送帧长度, 字节数
unsigned char NTxd=0;				// 以发送的字节数
unsigned char StBao=0;				// 接收数据包完成标志
int nLastError = 0;			        // 最后一次发生的错误
u32 nReceiveTimeout;		                // 接收超时

//=============================================================================
//CRC校验, 冗余多项式为0x11021
//pbuffer：指向要校验的数据缓冲区的指针
//n_byte：校验的字节数
//pjiao_zhi：计算出的校验值
//返回值：pjiao_zhi
//=============================================================================
const unsigned short crc_table[256] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7, 
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF, 
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6, 
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE, 
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485, 
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D, 
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4, 
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC, 
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823, 
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B, 
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12, 
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A, 
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41, 
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49, 
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70, 
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78, 
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F, 
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067, 
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E, 
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256, 
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D, 
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C, 
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634, 
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB, 
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3, 
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A, 
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92, 
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9, 
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1, 
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8, 
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0, 
};
//---------------------------------------------------------------------------
unsigned short cal_crc(unsigned char *pBuffer, unsigned char n_byte)
{
	unsigned short  crc=0xFFFF;
	unsigned char i;
	while(n_byte--)
	{
		i = crc>>8;
		crc <<= 8;
		crc ^= crc_table[i ^ *pBuffer++];
	}
	return crc;
}
//===========================================================================
// 将Ascii码转换为二进制数
//===========================================================================
unsigned char AsciiToHex(char c)
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
unsigned char PackByte(char* p)
{
	return (AsciiToHex(p[0]) << 4) | AsciiToHex(p[1]);
}

//===========================================================================
// Ascii码到WORD转换
//===========================================================================
unsigned int PackWord(char* p)
{
	return ((unsigned int)PackByte(p) << 8) | PackByte(p+2);
}

//===========================================================================
// Ascii码到DWORD转换
//===========================================================================
unsigned  int  PackDword(char* p)
{
	return ((unsigned int)PackWord(p) << 16)  | PackWord(p + 4);
}


void ReceiverData(unsigned char sq0)
{
	if (FRAME_OK())	// 上一次接收的包还没处理完, 拒绝接收
		return;
	
	if (sq0 == '>')
		NRxBuff = 0;
	aRxBuff[NRxBuff++] = sq0;	                // 保存数据
	
	if((NRxBuff==1)&&(sq0!='>'))			// 包头错误
		NRxBuff = 0;
	else if (NRxBuff == 2)                          // 地址错误
       {
             if (sq0 != GetAddress())
                 NRxBuff = 0;
       }
	else if(NRxBuff==N_XY_BAO)
		NRxBuff = 0;
	else if ((sq0=='\r')||(sq0=='\n'))		// 如果检测到包尾则处理
	{
		StBao=1;
		nReceiveTimeout = OSTimeGet ();
	}
}

void SendCommand(const char* lpszFormat, ...)
{
	char* p = aTxBuff;					// 发送缓冲区
	va_list va;
	va_start(va, lpszFormat);
	*p++ = '>';					        // 起始符
       *p++ = FRAME_ADDR;                                      // 加热地址
        p += vsprintf(p, lpszFormat, va);
	va_end(va);        
	p += sprintf(p, "%04X\r\n", cal_crc((unsigned char*)aTxBuff, p-aTxBuff));
       FRAME_RESET();
       GPIO_SetBits(GPIOA,GPIO_Pin_11);
       OSTimeDly(1);
	UART1_SendStr((char*)aTxBuff,p-aTxBuff);
       OSTimeDly(2);
       GPIO_ResetBits(GPIOA,GPIO_Pin_11);
}

void HandleCommand()
{
  u8 VAL_CMD;
  if(((OSTimeGet()-nReceiveTimeout) > 100)||                            // 距接收完时间超过100ms, 不要回复指令
  cal_crc((unsigned char*)aRxBuff, NRxBuff-5) != PackWord(FRAME_CRC))   // 帧最小长度与CRC校验错
  {
      FRAME_RESET();
      return;
  }
  
  FRAME_END();
  
  VAL_CMD=PackByte(FRAME_CMD);//获得命令值
  char nLen = 0;
   
  switch(VAL_CMD)
  {
         case 0x00:	//转发数据
           {
               extern unsigned char NRxBuff1;

	          *FRAME_CRC=0;
              nLen = SendCommand1("%s", FRAME_DATA);
              aRxBuff1[nLen-1] = 0x00;
	          if (nLen != 0)
		      SendCommand("00%s", &aRxBuff1[1]);
	          else
		      FRAME_RESET();
	      
              break;
           }
         case 0x01:     //读取固件版本信息("01")
           SendCommand("01%s", GetVersion());
           break;
         case 0x02:	//查询系统状态("02")
           switch (PackByte(FRAME_DATA))
           {
//              case 0x01:SendCommand("0201%02hX",Get_XYZ_State());break;
//              case 0x02:SendCommand("0202%02hX",Get_Hook_State());break;
//              case 0x03:SendCommand("0203%02hX",Get_Syringe_State());break;
//              case 0x04:SendCommand("0204%02hX",Get_Collect_State());break;
//              case 0x05:SendCommand("0205%02hX",Get_WaterPump_State());break;
           }
           break;
         case 0x03:	//写 EEPROM("03")
           {
                char buf[25];	// 最大长度, 实际上位机在传送时按8字节传送
	        int nAddr;
		unsigned char nLen, c, n;
		char* p = FRAME_DATA;

		nAddr = PackWord(p);
		p += 4;
		nLen = PackByte(p);
		p += 2;
		for(n=0; n<nLen; n++, p+=2)
			buf[n] = PackByte(p);
		SendCommand("03");
	        // 将数据写入EEPROM, Write24c02和Read24c02须更改, 地址可到1K
		// 如果每写一个字节就延时20ms, 效率有点太低
		// Write24c02((unsigned char*)buf, nAddr, nLen);
		c = nLen;
                I2C_PageWrite((u8*)buf,c,nAddr,0xA0);
           }
           break;
//         case 0x04:     //写 EEPROM("04")
//           {
//                char buf[25];	// 最大长度, 实际上位机在传送时按8字节传送
//		int offset;
//		unsigned char nbyte;
//		char i;
//		char* p = FRAME_DATA;
//			
//		offset = PackWord(p);
//		p += 4;
//		nbyte = PackByte(p);
//		p += 2;
//                I2C_ReadBuffer((u8*)buf,nbyte,offset,0xA0);
//		p = aTxBuff;
//		*p++ = '>';
//		*p++ = GetAddress();
//		*p++ = '0';
//                *p++ = '4';
//		for(i=0;i<nbyte;i++)
//			p += sprintf(p, "%02lX", (buf[i]&0xff));
//		p += sprintf(p, "%04X\r\r", cal_crc((unsigned char*)aTxBuff, p-aTxBuff));
//		FRAME_RESET();
//		GPIO_SetBits(GPIOA,GPIO_Pin_11);
//                OSTimeDly(1);
//                UART1_SendStr((char*)aTxBuff,p-aTxBuff);
//                OSTimeDly(1);
//                GPIO_ResetBits(GPIOA,GPIO_Pin_11);
//                break; 
//           }
         case 0x05:     //系统复位急停
           {
//                switch (PackByte(FRAME_DATA))
//                {
//                      case 0x01:
//                           //if (Get_XYZ_State()==0)
//                           //{
//                               Set_XYZ_State(XYZ_Idle);
//                               if (FRAME_DATA[2]=='0')Set_XYZ_R_FLAG();
//                               else if (FRAME_DATA[2]=='1')Set_XYZ_Cancel();
//                            //}
//                            
//                            SendCommand("05");
//                            break;
//                      
//                    case 0x02:
//                            //if (Get_Hook_State()==0)
//                            //{
//                               Set_Hook_State(Hook_Idle);
//                               if (FRAME_DATA[2]=='0')Set_Hook_R_FLAG();
//                               else if (FRAME_DATA[2]=='1')Set_Hook_Cancel_FLAG();
//                            //}
//                            SendCommand("05");
//                            break;
//                      
//                    case 0x03:
//                            //if (Get_Syringe_State()==0){
//                                Set_Syringe_State(Syringe_Idle);
//                                if (FRAME_DATA[2]=='0')SET_Syringe_R_FLAG();
//                                else if (FRAME_DATA[2]=='1')Set_Syringe_Cancel_FLAG();
//                            //    }
//                            SendCommand("05");
//                            break;
//                }
           }
           break;
         case 0x06:     //锁定/释放电机("06")
           {
               u8 bOk=1;
               if (FRAME_DATA[0]=='0')
                  TRY(bOk = MotorUnlock(FRAME_DATA[1], 1))
               else if (FRAME_DATA[0]=='1')
                  TRY(bOk = MotorUnlock(FRAME_DATA[1], 0))
               bOk=bOk;
               SendCommand("06");
           }
           break;
        case 0x07:      //读取最近一次步数
          {
            s32  pStep;
            u8   bOk=1;
            TRY(bOk = MotorGetLastStep(FRAME_DATA[0], &pStep))
            bOk=bOk;
            SendCommand("07%08lX",pStep);
          }
          break;
          
        case 0x08:      //设置XYZ加样臂坐标
          {
//              s32 XPos=0,YPos=0;
//              u8 Z1Z2Flag = 0xFF;
//              s32 Z1Pos=0,Z2Pos=0;
//              Z1Z2Flag = FRAME_DATA[0]-'0';
//              sscanf((char const *)&FRAME_DATA[1],"%08lX%08lX%08lX%08lX",&XPos,&YPos,&Z1Pos,&Z2Pos);
////              if (Get_XYZ_State()==0){
//                  Set_XYZ_Pos(XPos,YPos);
//                  SetZ1Z2Pos(Z1Z2Flag,Z1Pos,Z2Pos);
//                  Set_XYZ_XY_FLAG();
////              }
              SendCommand("08");
          }
          break;
          
        case 0x09:      //设置加样针坐标
          {
//              u8 Z1Z2Flag = 0xFF;
//              s32 Pos1=0,Pos2=0;
//              //if (Get_XYZ_State()==0)
//              //{
//                  Z1Z2Flag = FRAME_DATA[0]-'0';
//                  sscanf((char const *)&FRAME_DATA[1],"%08lX%08lX",&Pos1,&Pos2);
//                  switch(Z1Z2Flag)
//                  {
//                    //加样针移动
//                    case 0:
//                      SetZ1Z2Pos(Z1Z2Flag,Pos1,Pos2);
//                      Set_XYZ_Z1Z2_FLAG();
//                      break;
//                      
//                    //样品针液位感应
//                    case 1:
//                      SetZ1Z2Pos(Z1Z2Flag,Pos1,Pos2);
//                      Set_XYZ_TL_FLAG();
//                      break;
//                      
//                    //试剂针液位感应
//                    case 2:
//                      SetZ1Z2Pos(Z1Z2Flag,Pos1,Pos2);
//                      Set_XYZ_TL_FLAG();
//                      break;
//                  }
//              //}
              SendCommand("09");
          }
          break;
        case 0x0A:      //读取XYZ坐标
          {
//              s32 XPos,YPos,Z1Pos,Z2Pos;
//              ReadXYZPos(&XPos,&YPos,&Z1Pos,&Z2Pos);
//              SendCommand("0A%08lX%08lX%08lX%08lX",XPos,YPos,Z1Pos,Z2Pos);
          }
          break;
        case 0x0B:      //设置杯钩参数 
          {
//              s16 Pos1,Pos2;
//              u8  flag,Pos3;
////              if (Get_Hook_State()==0)
////              {
//                  flag = PackByte(FRAME_DATA);
//                  sscanf((char const *)&FRAME_DATA[2],"%04hX%04hX",&Pos1,&Pos2);
//                  Pos3=PackByte(&FRAME_DATA[10]);
//                  SetHookParam(flag,Pos1,Pos2,Pos3);
//                  Set_Hook_FLAG();
////              }
              SendCommand("0B");
          }
          break;
          case 0x0C:      //读取杯钩参数 
          {
//              s32 Pos1,Pos2;
//              u8 Pos3,bOk=1;
//              //u8  flag,bOk=1;
//              //flag=PackByte(FRAME_DATA);
//              //if (flag==0x01)
//              //{
//              TRY(bOk = MotorGetPos(MotorMotiveHook.nAddr,&Pos1));
//              //}
//              //else if (flag==0x02)
//              //{
//              TRY(bOk = MotorGetPos(MotorUDHook.nAddr,&Pos2));
//              //}
//              //else if (flag==0x03)
//              //{
//              if (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_10))
//                 Pos3=0x01;
//              else
//                 Pos3=0x00;
//              //}
//              bOk=bOk;
//              SendCommand("0C%04hX%04hX%02hX",Pos1,Pos2,Pos3);
          }
          break;
          case 0x0D:      //输送测试杯子 
          {
//              u16  time;
//              
//              time=PackWord(FRAME_DATA);
//              Set_Collect_State(Collect_Idle);
//              SetCollectTime(time);
//              Set_Collect_FLAG();
//              
              SendCommand("0D");
          }
          break;
          case 0x0E:      //设置加样泵坐标 
          {
//              s32 param;
//              u8  flag;
//              u16 Per;
//              flag = PackByte(&FRAME_DATA[0]);
//              sscanf((char const *)&FRAME_DATA[2],"%08lX",&param);
//              Per = PackByte(&FRAME_DATA[10]);
//              SetSyringePos(flag,param,Per);
//              SET_Syringe_MOVE_FLAG();
              SendCommand("0E");
          }
          break;
          case 0x0F:      //读取加样泵坐标 
          {
//              u8  bOk=1;
//              s32 Pos;
//              u8  flag;
//              flag = PackByte(&FRAME_DATA[0]);
//              if (flag == 0x01)
//              {
//                  TRY(bOk = MotorGetPos(MotorSyringSample.nAddr,&Pos));
//                  bOk = bOk;
//                  SendCommand("0F%08lX",Pos);
//              }
//              else if (flag == 0x02)
//              {
//                  TRY(bOk = MotorGetPos(MotorSyringReagent.nAddr,&Pos));
//                  bOk = bOk;
//                  SendCommand("0F%08lX",Pos);
//              }
          }
          break;
          case 0x10:      //转停冲洗泵 
          {
//              u8 flag =0;
//              u8 Channel=0;
//              u8 ONOFF[3]={0};
//              u16 time[3]={0};
//              
//              u8 c = FRAME_DATA[0];
//              if (c != '\0')//有参数
//              {
//                flag=PackByte(&FRAME_DATA[0]);
//                for (Channel=0;Channel<3;Channel++)
//                {
//                  ONOFF[Channel]=FRAME_DATA[2+Channel*7]-'0';
//                  time[Channel]=PackWord(&FRAME_DATA[5+Channel*7]);
//                  if (flag&(0x01<<Channel))
//                    SetWaterPumpParam(Channel,ONOFF[Channel],time[Channel]);
//                }
//              }
//              //读取冲洗泵状态
//              for (Channel=0;Channel<3;Channel++)
//                GetWaterPumpParam(Channel,&ONOFF[Channel],&time[Channel]);
//              SendCommand("10%hX%hX%hX",ONOFF[0],ONOFF[1],ONOFF[2]);
          }
          break;
          case 0x11:     //蜂鸣器
          {
//              u8 state;
//              u16 OnTime,OffTime;
//              state=FRAME_DATA[0]-'0';
//              OnTime=PackWord(&FRAME_DATA[1]);
//              OffTime=PackWord(&FRAME_DATA[5]);
//              if (state)
//                SET_BEEP_CON();
//              else
//                RESET_BEEP_CON();
//              SetBeepParam(OnTime,OffTime);
//              SendCommand("11");
          }
          break;
          case 0x12:     //开合电磁阀
          {
//              u8 select,state;
//              select=PackByte(FRAME_DATA);
//              state=PackByte(&FRAME_DATA[2]);
//              
//              switch(select)
//              {
//              case 1:
//              case 2:
//                   //1,2号电磁阀使用加样泵控制板接口
//                   SetSyringePos(select+3,state,0);
//                   SET_Syringe_MOVE_FLAG();
//                   break;
//                   /*if (state)
//                      GPIO_SetBits(GPIOB,GPIO_Pin_10);
//                   else
//                      GPIO_ResetBits(GPIOB,GPIO_Pin_10);
//                   break;*/
//              
//              case 3:
//                   //1,2,3号电磁阀使用主控制板接口控制
//                   /*SetMagneticSelect(select);
//                   SetMagneticState(state);
//                   SET_MAGNETIC_ACT();
//                   break;*/
//                   if (state)
//                      GPIO_SetBits(GPIOB,GPIO_Pin_13);
//                   else
//                      GPIO_ResetBits(GPIOB,GPIO_Pin_13);
//                   break;
//              case 4:
//              case 5:
//                   break;
//              }
              SendCommand("12");
          }
          break;
        case 0x13:     //控制步进排水泵
          {
              /*u8 state;
              u16 OnTime;
              state=FRAME_DATA[0]-'0';
              OnTime=PackWord(&FRAME_DATA[1]);
              if (state)
              {
                SetDrainTime(OnTime);
                Set_Drain_FLAG();
              }
              else
                Set_Drain_Cancel();*/
              SendCommand("13");
          }
          break;
          
          case 0x14:
            {
               u8 InputBit=0;
               InputBit = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
               InputBit += '0';
               SendCommand("14%c",InputBit);
               break;
            }
        
          case 0x15:
              {
//                u8 uMotorAdr;
//                u16   nSpeedMin;
//                u16   nSpeedMax;
//                u16   nSpeedStep;
//                
//                uMotorAdr = FRAME_DATA[0];
//                
//                nSpeedMin = PackWord(&FRAME_DATA[1]);
//                nSpeedMax = PackWord(&FRAME_DATA[5]);
//                nSpeedStep = PackByte(&FRAME_DATA[9]);

//                switch(uMotorAdr)
//                {
//                  case MOTOR_Z1_ADDR:
//                      MotorSample.nSpeedMin = nSpeedMin;
//                      MotorSample.nSpeedMax = nSpeedMax;
//                      MotorSample.nSpeedStep = nSpeedStep;
//                      InitXYZMotor();
//                      break;
//                
//                  case MOTOR_Z2_ADDR:
//                      MotorRegent.nSpeedMin = nSpeedMin;
//                      MotorRegent.nSpeedMax = nSpeedMax;
//                      MotorRegent.nSpeedStep = nSpeedStep;
//                      InitXYZMotor();
//                      break;
//                   
//                  case MOTOR_X_ADDR:
//                      MotorX.nSpeedMin = nSpeedMin;
//                      MotorX.nSpeedMax = nSpeedMax;
//                      MotorX.nSpeedStep = nSpeedStep;
//                      InitXYZMotor();
//                      break;
//                
//                  case MOTOR_Y_ADDR:
//                      MotorY.nSpeedMin = nSpeedMin;
//                      MotorY.nSpeedMax = nSpeedMax;
//                      MotorY.nSpeedStep = nSpeedStep;
//                      InitXYZMotor();
//                      break;
//              
//                  case MOTOR_HOOK_ADDR:
//                      MotorMotiveHook.nSpeedMin = nSpeedMin;
//                      MotorMotiveHook.nSpeedMax = nSpeedMax;
//                      MotorMotiveHook.nSpeedStep = nSpeedStep;
//                      InitHookMotor();
//                      break;
//              
//                  case MOTOR_HOOKUD_ADDR:
//                      MotorUDHook.nSpeedMin = nSpeedMin;
//                      MotorUDHook.nSpeedMax = nSpeedMax;
//                      MotorUDHook.nSpeedStep = nSpeedStep;
//                      InitHookMotor();
//                      break;
//                
//                  case MOTOR_SYRINGE_ADDR:
//                      MotorSyringSample.nSpeedMin = nSpeedMin;
//                      MotorSyringSample.nSpeedMax = nSpeedMax;
//                      MotorSyringSample.nSpeedStep = nSpeedStep;
//                      InitSyringeMotor();
//                      break;
//                
//                  case MOTOR_DRAIN_ADDR:
//                      MotorSyringReagent.nSpeedMin = nSpeedMin;
//                      MotorSyringReagent.nSpeedMax = nSpeedMax;
//                      MotorSyringReagent.nSpeedStep = nSpeedStep;
//                      InitSyringeMotor();
//                      break;
//                }
                SendCommand("15");
             }
             break;
    
       case 0x16:
            {
//              u8 uMotorAdr;
//              uMotorAdr = FRAME_DATA[0];
//              u16   nSpeedMin;
//              u16   nSpeedMax;
//              u16   nSpeedStep;
              
//              switch(uMotorAdr)
//              {
//                case MOTOR_Z1_ADDR:
//                    nSpeedMin = MotorSample.nSpeedMin;
//                    nSpeedMax = MotorSample.nSpeedMax;
//                    nSpeedStep = MotorSample.nSpeedStep;
//                    break;
//                
//                case MOTOR_Z2_ADDR:
//                    nSpeedMin = MotorRegent.nSpeedMin;
//                    nSpeedMax = MotorRegent.nSpeedMax;
//                    nSpeedStep = MotorRegent.nSpeedStep;
//                    break;
//                   
//                case MOTOR_X_ADDR:
//                    nSpeedMin = MotorX.nSpeedMax;
//                    nSpeedMax = MotorX.nSpeedMin;
//                    nSpeedStep = MotorX.nSpeedStep;
//                    break;
//                
//                case MOTOR_Y_ADDR:
//                    nSpeedMin = MotorY.nSpeedMax;
//                    nSpeedMax = MotorY.nSpeedMin;
//                    nSpeedStep = MotorY.nSpeedStep;
//                    break;
//              
//                case MOTOR_HOOK_ADDR:
//                    nSpeedMin =  MotorMotiveHook.nSpeedMax;
//                    nSpeedMax =  MotorMotiveHook.nSpeedMin;
//                    nSpeedStep = MotorMotiveHook.nSpeedStep;
//                    break;
//              
//                case MOTOR_HOOKUD_ADDR:
//                    nSpeedMin = MotorUDHook.nSpeedMax;
//                    nSpeedMax = MotorUDHook.nSpeedMin;
//                    nSpeedStep =MotorUDHook.nSpeedStep;
//                    break;
//                
//                case MOTOR_SYRINGE_ADDR:
//                    nSpeedMin = MotorSyringSample.nSpeedMax;
//                    nSpeedMax = MotorSyringSample.nSpeedMin;
//                    nSpeedStep = MotorSyringSample.nSpeedStep;
//                    break;
//                
//                case MOTOR_DRAIN_ADDR:
//                    nSpeedMin = MotorSyringReagent.nSpeedMax;
//                    nSpeedMax = MotorSyringReagent.nSpeedMin;
//                    nSpeedStep = MotorSyringReagent.nSpeedStep;
//                    break;
//              }
//              SendCommand("16%04hX%04hX%02hX",nSpeedMin,nSpeedMax,nSpeedStep);
            }
         break;
         case 0x17:     //控制缺液报警
          {
              SendCommand("17%01hX",LackFlag);
          }
          break;
        default:FRAME_RESET();break;
  }        
}
