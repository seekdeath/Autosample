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
/*void CupStateConfig(void)//ץ��״̬�ܽų�ʼ��
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

  EXTI_InitStructure.EXTI_Line = EXTI_Line5;            //�趨�ⲿ�ж�0

  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //�趨�ж�ģʽ

  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�趨�½��ش���ģʽ

  EXTI_InitStructure.EXTI_LineCmd = ENABLE;

  EXTI_Init(&EXTI_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
}
void LackWater(void)//ץ��״̬�ܽų�ʼ��
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
//case 'A':	// ��ȡ�汾
//		SendCommand("A%s", GetVersion());
//		break;
//
//	case 'B':	// ��ȡ����
//		SendCommand("B%04X", GetLastError());
//		break;
//
//	case 'C':	// ���ò���
//		{
//			config = PackWord(FRAME_DATA);
//			nMaxOrgStep = PackWord(FRAME_DATA+4);
//			nMaxOrgStep *= 1000;
//			SendCommand("C");
//			EnableEncoder(bUsesEncoder);
//		}
//		break;
//
//	case 'D':	// ��λ
//		SetLastError(E_BUSY);
//		SendCommand("D");
//		FRAME_RESET();
//		ShutDown();
//		MotorInit();
//		return;
//
//	case 'E':	// ֹͣ
//		SendCommand("E");
//		ShutDown();
//		if (GetLastError() == E_BUSY)
//			SetLastError(E_IDLE);
//		break;
//
//	case 'F':	// �ͷŵ��
//		SendCommand("F");
//		EnableMotor(FRAME_DATA[0]=='0');
//		UnlockMotor(FRAME_DATA[0]!='0');
//		break;
//
//	case 'G':	// �����һ�����в���
//		SendCommand("G%08lX", IsLeftToRight(bLastDir) ? nMovedStep : -nMovedStep);
//		break;
//
//	case 'H':	// ���õ��Ƶ��
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
//			// ʹ�ñ�����ʱ, ���������һ������, Ƶ�ʲű仯һ��
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
//	case 'I':	// ��/ͣ���
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
//	case 'J':	// ����λ��
//		{
//                        SetLastError(E_BUSY);
//			long nDst = PackDword(FRAME_DATA);
//			SendCommand("J");
//			ShutDown();
//			MoveTo(nDst);
//		}
//		break;
//
//	case 'K':	// ��ȡ����
//                if (bUsesEncoder)
//                {
//                  nCurPos = TIM_GetCounter(TIM1) - 32768;
//                }
//                  SendCommand("K%08lX", nCurPos);
//                
//		break;
//
//	case 'L':	// ��
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
//                TIM2->CCER = TIM2->CCER | 0x1000;//�������
//		TIM_Cmd(TIM2,ENABLE);					// ������ʱ��
//		break;
//
//	case 'M':	// ��ȡ���������
//		SendCommand("M%08lX", nLeftOrgPos);
//		break;
//
//	case 'N':	// ��ȡ�س̲�
//		SendCommand("N%04X%04X", nLeftDev, nRightDev);
//		break;
//
//	case 'O':	// ��ȡ������λ��
//		SendCommand("O%08lX", nCurPosEncoder);
//		break;
//
//	case 'P':	// ��ȡ������ֵ
//		SendCommand("P%04X%04X", (int)GetSensor(), status);
//		break;
//
//	case 'Q':	// ���Һλ
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
//	case 'R':	// ���÷�״̬
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