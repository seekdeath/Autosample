#include <includes.h>
#include <gpio.h>
#include <Agreement.h>
void LD1Ini(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
}
/*void CupStateConfig(void)//抓杯状态管脚初始化
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}*/
void GPIO_Interrupt(void)
{
  
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;            //设定外部中断0

  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //设定中断模式

  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //设定下降沿触发模式

  EXTI_InitStructure.EXTI_LineCmd = ENABLE;

  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
}
void LackWater(void)//抓杯状态管脚初始化
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void GetLackState(void)
{
  unsigned char Lacktemp1;
  unsigned char Lacktemp2;
  if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
  {
    OSTimeDlyHMSM (0, 0, 0, 10);
    if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2))
    Lacktemp1 = 1;
  }
  else Lacktemp1 = 0;
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
  {
    OSTimeDlyHMSM (0, 0, 0, 10);
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
    Lacktemp2 = 2;
  }
  else Lacktemp2 = 0;
  LackFlag = Lacktemp1 + Lacktemp2;
}
////
//case 'A':	// 读取版本
//		SendCommand("A%s", GetVersion());
//		break;
//
//	case 'B':	// 读取错误
//		SendCommand("B%04X", GetLastError());
//		break;
//
//	case 'C':	// 设置参数
//		{
//			config = PackWord(FRAME_DATA);
//			nMaxOrgStep = PackWord(FRAME_DATA+4);
//			nMaxOrgStep *= 1000;
//			SendCommand("C");
//			EnableEncoder(bUsesEncoder);
//		}
//		break;
//
//	case 'D':	// 复位
//		SetLastError(E_BUSY);
//		SendCommand("D");
//		FRAME_RESET();
//		ShutDown();
//		MotorInit();
//		return;
//
//	case 'E':	// 停止
//		SendCommand("E");
//		ShutDown();
//		if (GetLastError() == E_BUSY)
//			SetLastError(E_IDLE);
//		break;
//
//	case 'F':	// 释放电机
//		SendCommand("F");
//		EnableMotor(FRAME_DATA[0]=='0');
//		UnlockMotor(FRAME_DATA[0]!='0');
//		break;
//
//	case 'G':	// 读最后一次运行步数
//		SendCommand("G%08lX", IsLeftToRight(bLastDir) ? nMovedStep : -nMovedStep);
//		break;
//
//	case 'H':	// 设置电机频率
//		{
//			/*float nMin,nMax,nAcc,nCal=0,nTime=0;
//                        //float nAcc;
//			nMin = KSPEEDMIN*(float)(8*(PackWord(FRAME_DATA)));
//			nMax = KSPEEDMAX*(float)(8*(PackWord(FRAME_DATA+4)));
//			nAcc = ((KACC*(float)(PackByte(FRAME_DATA+8)))/5.0);
//                        //nAcc = nAcc/1000.0;
//			SendCommand("H");
//                        nCal = nMin;
//                        while (nCal < nMax)
//                        {
//                            nTime += (1.0/((float)nCal));
//                            nCal += nAcc;
//                        }
//			nSpeedMin = (int)nMin;
//			nSpeedMax = (int)nMax;
//			nSpeedTime = nTime*2;*/
//                        nSpeedMin = PackWord(FRAME_DATA);
//			nSpeedMax = PackWord(FRAME_DATA+4);
//			nSpeedTime = PackWord(FRAME_DATA+8);
//                        nSpeedTime = nSpeedTime/1000.0;
//                        SendCommand("H");
//                        //CreateMotorTable(nSpeedMin,nSpeedMax,nSpeedTime);
//			// 使用编码器时, 编码器输出一个脉冲, 频率才变化一次
//			//if (bUsesEncoder)
//				//nSpeedAcc *= WHISPER;
//                        /*
//			if (nSpeedAcc == 0)
//				nMaxAccStep = 0;
//			else
//				nMaxAccStep = (nSpeedMax-nSpeedMin)/nSpeedAcc;*/
//                        
//		}
//		break;
//
//	case 'I':	// 启/停电机
//		SendCommand("I");
//		if (FRAME_DATA[0] == '0')
//		{
//			ShutDown();
//			if (GetLastError() == E_BUSY)
//				SetLastError(E_IDLE);
//		}
//		else if (FRAME_DATA[0] == '1')
//		{
//			MoveTo(nCurPos+10000);
//			bLoop = 1;
//			bLastDir = DIR_L2R;
//                        SetDIR(bLastDir);
//		}
//		else
//		{
//			MoveTo(nCurPos-10000);
//			bLoop = 1;
//			bLastDir = !DIR_L2R;
//                        SetDIR(bLastDir);
//		}
//		break;
//
//	case 'J':	// 设置位置
//		{
//                        SetLastError(E_BUSY);
//			long nDst = PackDword(FRAME_DATA);
//			SendCommand("J");
//			ShutDown();
//			MoveTo(nDst);
//		}
//		break;
//
//	case 'K':	// 读取坐标
//                if (bUsesEncoder)
//                {
//                  nCurPos = TIM_GetCounter(TIM1) - 32768;
//                }
//                  SendCommand("K%08lX", nCurPos);
//                
//		break;
//
//	case 'L':	// 振动
//                SetLastError(E_BUSY);
//		SendCommand("L");
//		ShutDown();
//		nOffset = (int)PackWord(FRAME_DATA);
//		nSpeed = PackWord(FRAME_DATA+4);
//		ApplySpeed();
//		if (nOffset < 0)
//		{
//			bLastDir = !DIR_L2R;
//                        SetDIR(bLastDir);
//			nOffset = -nOffset;
//		}
//		else
//		{
//			bLastDir = DIR_L2R;
//                        SetDIR(bLastDir)
//		}
//		nMovedStep = 0;
//		UnlockMotor(0);
//		EnableMotor(1);
//		bRun = 1;
//		bLoop = 0;
//		bShake = 1;
//		bGotOrg = 0;
//                TIM2->CCER = TIM2->CCER | 0x1000;//开启输出
//		TIM_Cmd(TIM2,ENABLE);					// 启动定时器
//		break;
//
//	case 'M':	// 读取负零点坐标
//		SendCommand("M%08lX", nLeftOrgPos);
//		break;
//
//	case 'N':	// 读取回程差
//		SendCommand("N%04X%04X", nLeftDev, nRightDev);
//		break;
//
//	case 'O':	// 读取编码器位置
//		SendCommand("O%08lX", nCurPosEncoder);
//		break;
//
//	case 'P':	// 读取传感器值
//		SendCommand("P%04X%04X", (int)GetSensor(), status);
//		break;
//
//	case 'Q':	// 检测液位
//		if (FRAME_DATA[0] == '0')
//			nLevelMaxStep = PackDword(FRAME_DATA+1);
//		else if (FRAME_DATA[0] == '1')
//			nLevelOffset = PackDword(FRAME_DATA+1);
//		SendCommand("Q");
//
//		if (FRAME_DATA[0] == '1')
//		{
//			SetLastError(E_BUSY);
//			MoveTo(nLevelMaxStep);
//			bTestLevel = 1;
//			bGotLevel = 0;
//		}
//		break;
//
//	case 'R':	// 设置阀状态
//		SetValve(PackByte(FRAME_DATA));
//		SendCommand("R");
//		break;
//                
//        case 'S':
//                MotorPartParamRead();
//                SendCommand("S%02lX%02lX%02lX",MotorStateRigistor[5]
//                            ,MotorStateRigistor[6],MotorStateRigistor[7]);
//		break;
//        case 'T':
//          {
//                //u8 i;
//                LOWSPEEDLEVEL = PackByte(FRAME_DATA);
//                HIGHSPEEDLEVEL = PackByte(&FRAME_DATA[2]);
//                HOLDSPEEDLEVEL = PackByte(&FRAME_DATA[4]);
//                /*for (i=0;i<16;i++)
//                {
//                    if (ITable[0][i]&0x1F > HIGHSPEEDLEVEL&0x1F)
//                        ITable[0][i] = (ITable[0][i]&0xD0)|(HIGHSPEEDLEVEL&0x1F);
//                }*/
//                SendCommand("T");
//		break;
//          }
//        case 'U':
//          {
//                float  TEMPK;
//                TEMPK = PackWord(FRAME_DATA);
//                SpeedK = TEMPK/1000.0;
//                TEMPK = PackWord(&FRAME_DATA[2]);
//                ISensitive = (int)TEMPK;
//                SendCommand("U");
//		break;
//          }