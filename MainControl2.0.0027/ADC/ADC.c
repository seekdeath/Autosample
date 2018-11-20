#include "includes.h"
#define ADC1_DR_Address    ((u32)0x4001244C)
uint16 ADC_ConvertedValue[140]={0};
uint16  m_dIValue[14]={0};

float m_fCurVolt[14]={0};
float m_fCurVoltRes[14]={0};
CPU_INT08U pBuf[14]={0};
     uint16 m_dTorqueSampleAD0=0,m_dTorqueSampleAD1=0,m_dTorqueSampleAD2=0,m_dTorqueSampleAD3=0,m_dTorqueSampleAD4=0,m_dTorqueSampleAD5=0,m_dTorqueSampleAD6=0,m_dTorqueSampleAD7=0,m_dTorqueSampleAD8=0,m_dTorqueSampleAD9=0,m_dTorqueSampleAD10=0,m_dTorqueSampleAD11=0,m_dTorqueSampleAD12=0,m_dTorqueSampleAD13=0; 
uint16 m_ADCBuf0[CURBUFSIZE]={0};  //电压平滑滤波缓冲区
uint16 m_ADCBuf1[CURBUFSIZE]={0}; 
uint16 m_ADCBuf2[CURBUFSIZE]={0}; 
uint16 m_ADCBuf3[CURBUFSIZE]={0}; 
uint16 m_ADCBuf4[CURBUFSIZE]={0}; 
uint16 m_ADCBuf5[CURBUFSIZE]={0}; 
uint16 m_ADCBuf6[CURBUFSIZE]={0}; 
uint16 m_ADCBuf7[CURBUFSIZE]={0}; 
uint16 m_ADCBuf8[CURBUFSIZE]={0}; 
uint16 m_ADCBuf9[CURBUFSIZE]={0}; 
uint16 m_ADCBuf10[CURBUFSIZE]={0}; 
uint16 m_ADCBuf11[CURBUFSIZE]={0}; 
uint16 m_ADCBuf12[CURBUFSIZE]={0}; 
uint16 m_ADCBuf13[CURBUFSIZE]={0}; 





void QS_GPIO_Init(void){
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;

  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);  
  GPIO_WriteBit(GPIOB,GPIO_Pin_2,Bit_RESET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);  
  GPIO_WriteBit(GPIOB,GPIO_Pin_11,Bit_RESET);
  GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);  
  GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
}

/*********************************************************************************************************
** 函数名称: AD_Init
** 功能描述: AD_Init初始化
** 输　入: 无
** 输　出: 无
** 全局变量: 无
** 调用模块:GPIO_InitTypeDef，DMA_InitStructure
**
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void AD_Init(void){
    
 /*Enable CPIOA、B、C clock*/
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    /* Enable DMA clock*/
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* Enable ADC1 clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//   DMA_Cmd(DMA1_Channel1, DISABLE);
   /* Enable ADC1 DMA interface*/
//   ADC_DMACmd(ADC1, DISABLE);  
  /* Enable ADC1 */
   ADC_Cmd(ADC1, DISABLE);
  
  ADC_InitTypeDef ADC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;	
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Configure PC0 (ADC Channel0)as analog input*/ 
  //ADC_CH10-->PC0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;             //ADI6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;             //ADIA
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /*DMA Channal1 Confogrution*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 140;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE); 
     
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode       = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel       = 14;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */ 
  //常规转换序列1：通道16
  

  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);   //PA0
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_13Cycles5);   //PA1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_13Cycles5);   //PA2
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_13Cycles5);   //PA3
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_13Cycles5);   //PA4
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_13Cycles5);   //PA5
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 7, ADC_SampleTime_13Cycles5); //PC1   
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 8, ADC_SampleTime_13Cycles5); //PC3
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 9, ADC_SampleTime_13Cycles5); //PC0 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 10, ADC_SampleTime_13Cycles5); //PC2 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 11, ADC_SampleTime_13Cycles5);   //PA6
  ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 12, ADC_SampleTime_13Cycles5);   //PA7
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 13, ADC_SampleTime_13Cycles5); //PC4 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 14, ADC_SampleTime_13Cycles5); //PC5

  /* Enable ADC1 DMA interface*/
  ADC_DMACmd(ADC1, ENABLE);  
   
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1)); 
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);     //软件触发 

}


/*********************************************************************************************************
** 函数名称: ADC_Sample
** 功能描述: 3各通道ADC数据采集
** 输　入: ADC通道采集的数据
** 输　出: 无
** 全局变量: ADC_ConvertedValue
** 调用模块:GPIO_InitTypeDef，DMA_InitStructure
**
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

void ADC_Sample(void){

   CPU_INT32U  ADC0_Value=0,ADC1_Value=0,ADC2_Value=0,ADC3_Value=0,ADC4_Value=0,ADC5_Value=0,ADC6_Value=0,ADC7_Value=0,ADC8_Value=0,ADC9_Value=0,ADC10_Value=0,ADC11_Value=0,ADC12_Value=0,ADC13_Value=0;
   CPU_INT32U  i;
      for(i=0;i<140;i=i+14)
      {    
        ADC0_Value += ADC_ConvertedValue[i];
        ADC1_Value += ADC_ConvertedValue[i+1];
        ADC2_Value += ADC_ConvertedValue[i+2];
        ADC3_Value += ADC_ConvertedValue[i+3];
        ADC4_Value += ADC_ConvertedValue[i+4];
        ADC5_Value += ADC_ConvertedValue[i+5];
        ADC6_Value += ADC_ConvertedValue[i+6];
        ADC7_Value += ADC_ConvertedValue[i+7];
        ADC8_Value += ADC_ConvertedValue[i+8];
        ADC9_Value += ADC_ConvertedValue[i+9];
        ADC10_Value += ADC_ConvertedValue[i+10];
        ADC11_Value += ADC_ConvertedValue[i+11];
        ADC12_Value += ADC_ConvertedValue[i+12];
        ADC13_Value += ADC_ConvertedValue[i+13];
      }
    m_dIValue[0] = ADC0_Value/10;
    m_dIValue[1] = ADC1_Value/10;
    m_dIValue[2] = ADC2_Value/10;  
    m_dIValue[3] = ADC3_Value/10;
    m_dIValue[4] = ADC4_Value/10;
    m_dIValue[5] = ADC5_Value/10; 
    m_dIValue[6] = ADC6_Value/10;
    m_dIValue[7] = ADC7_Value/10;
    m_dIValue[8] = ADC8_Value/10; 
    m_dIValue[9] = ADC9_Value/10;
    m_dIValue[10] = ADC10_Value/10;
    m_dIValue[11] = ADC11_Value/10; 
    m_dIValue[12] = ADC12_Value/10;
    m_dIValue[13] = ADC13_Value/10;
}

void ADC_Deal(uint8 channel){
 
     static   CPU_INT08U    pBuf0=0,pBuf1=0,pBuf2=0,pBuf3=0,pBuf4=0,pBuf5=0,pBuf6=0,pBuf7=0,pBuf8=0,pBuf9=0,pBuf10=0,pBuf11=0,pBuf12=0,pBuf13=0;
     static   uint32        res0=0,res1=0,res2=0,res3=0,res4=0,res5=0,res6=0,res7=0,res8=0,res9=0,res10=0,res11=0,res12=0,res13=0;
     ADC_Sample();
     switch(channel){

     case 0x01:
            res0-= m_ADCBuf0[pBuf0];
            m_ADCBuf0[pBuf0] = m_dIValue[0];
            res0 += m_ADCBuf0[pBuf0];
            pBuf0++;
            if(pBuf0 == CURBUFSIZE)  
             pBuf0=0;
            m_fCurVolt[0] = (float)( 3.0*((float)res0 / 4095.0 / (float)CURBUFSIZE));
            if (DEVICE_ID==DEVICE_AP6A)         //AP6 A板
              FloatToASCII(m_fCurVolt[0],(uint8 *)&m_bFmtSampleA.m_bSampleVolt28vADI1); 
            else if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[0],(uint8 *)&m_bFmtSampleB.m_bSampleVolt28vADI1); 
            break;
     case 0x02:
            res1-= m_ADCBuf1[pBuf1];
            m_ADCBuf1[pBuf1] = m_dIValue[1];
            res1 += m_ADCBuf1[pBuf1];
            pBuf1++;
            if(pBuf1 == CURBUFSIZE)  
              pBuf1=0;
            m_fCurVolt[1] = (float)( 3.0*((float)res1 / 4095.0 / (float)CURBUFSIZE));  
            if (DEVICE_ID==DEVICE_AP6A)         //AP6 A板
              FloatToASCII(m_fCurVolt[1],(uint8 *)&m_bFmtSampleA.m_bSampleIA28vADI2); 
            else if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[1],(uint8 *)&m_bFmtSampleB.m_bSampleIA28vADI2); 
            break;
     case 0x03:
            res2-= m_ADCBuf2[pBuf2];
            m_ADCBuf2[pBuf2] = m_dIValue[2];
            res2 += m_ADCBuf2[pBuf2];
            pBuf2++;
            if(pBuf2 == CURBUFSIZE)  
            pBuf2=0;
            m_fCurVolt[2] = (float)( 3.0*((float)res2 / 4095.0 / (float)CURBUFSIZE));  
            
            break;
     case 0x04:
            res3-= m_ADCBuf3[pBuf3];
            m_ADCBuf3[pBuf3] = m_dIValue[3];
            res3 += m_ADCBuf3[pBuf3];
            pBuf3++;
            if(pBuf3 == CURBUFSIZE)  
             pBuf3=0;
            m_fCurVolt[3] = (float)( 3.0*((float)res3 / 4095.0 / (float)CURBUFSIZE));     
            break;
     case 0x05:
            res4-= m_ADCBuf4[pBuf4];
            m_ADCBuf4[pBuf4] = m_dIValue[4];
            res4 += m_ADCBuf4[pBuf4];
            pBuf4++;
            if(pBuf4 == CURBUFSIZE)  
            pBuf4=0;
            m_fCurVolt[4] = (float)( 3.0*((float)res4 / 4095.0 / (float)CURBUFSIZE));      
            break;
     case 0x06:     
            res5-= m_ADCBuf5[pBuf5];
            m_ADCBuf5[pBuf5] = m_dIValue[5];
            res5 += m_ADCBuf5[pBuf5];
            pBuf5++;
            if(pBuf5 == CURBUFSIZE)  
            pBuf5=0;
            m_fCurVolt[5] = (float)( 3.0*((float)res5 / 4095.0 / (float)CURBUFSIZE));  
            break;
     case 0x07:     
            res6-= m_ADCBuf6[pBuf6];
            m_ADCBuf6[pBuf6] = m_dIValue[6];
            res6 += m_ADCBuf6[pBuf6];
            pBuf6++;
            if(pBuf6 == CURBUFSIZE)  
            pBuf6=0;
            m_fCurVolt[6] = (float)( 3.0*((float)res6 / 4095.0 / (float)CURBUFSIZE));  //未分压时电压值
            m_fCurVolt[6] = m_fCurVolt[6]*4.86;                                         //原端电压值
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[6],(uint8 *)&m_bFmtSampleB.m_bSample2VoltADI7);
            break;
     case 0x08: 
            res7-= m_ADCBuf7[pBuf7];
            m_ADCBuf7[pBuf7] = m_dIValue[7];
            res7 += m_ADCBuf7[pBuf7];
            pBuf7++;
            if(pBuf7 == CURBUFSIZE)  
            pBuf7=0;
            m_fCurVolt[7] = (float)( 3.0*((float)res7 / 4095.0 / (float)CURBUFSIZE));
            m_fCurVolt[7] = m_fCurVolt[7]*2.0;
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[7],(uint8 *)&m_bFmtSampleB.m_bSample2VoltADI8);
            break;
     case 0x09:      
            res8-= m_ADCBuf8[pBuf8];
            m_ADCBuf8[pBuf8] = m_dIValue[8];
            res8 += m_ADCBuf8[pBuf8];
            pBuf8++;
            if(pBuf8 == CURBUFSIZE)  
            pBuf8=0;
            m_fCurVolt[8] = (float)( 3.0*((float)res8 / 4095.0 / (float)CURBUFSIZE)); 
            m_fCurVolt[8] = m_fCurVolt[8]*2.0;
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[8],(uint8 *)&m_bFmtSampleB.m_bSample2VoltADI9);
            break;
     case 0x0A:        
            res9-= m_ADCBuf9[pBuf9];
            m_ADCBuf9[pBuf9] =m_dIValue[9];
            res9 += m_ADCBuf9[pBuf9];
            pBuf9++;
            if(pBuf9 == CURBUFSIZE)  
            pBuf9=0;
            m_fCurVolt[9] = (float)( 3.0*((float)res9 / 4095.0 / (float)CURBUFSIZE)); 
            m_fCurVolt[9] = m_fCurVolt[9]*(-2.0);
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[9],(uint8 *)&m_bFmtSampleB.m_bSample2VoltADIA);
            break;
     case 0x0B: 
            res10-= m_ADCBuf10[pBuf10];
            m_ADCBuf10[pBuf10] =m_dIValue[10];
            res10 += m_ADCBuf10[pBuf10];
            pBuf10++;
            if(pBuf10 == CURBUFSIZE)  
            pBuf10=0;
            m_fCurVolt[10] = (float)( 3.0*((float)res10 / 4095.0 / (float)CURBUFSIZE)); //放大后的电压
            m_fCurVolt[10] = m_fCurVolt[10]/10.01;                                //还原后的电流值
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[10],(uint8 *)&m_bFmtSampleB.m_bSample2IAADIB);
            break;
     case 0x0C:
            res11-= m_ADCBuf11[pBuf11];
            m_ADCBuf11[pBuf11] =m_dIValue[11];
            res11 += m_ADCBuf11[pBuf11];
            pBuf11++;
            if(pBuf11 == CURBUFSIZE)  
            pBuf11=0;
            m_fCurVolt[11] = (float)( 3.0*((float)res11 / 4095.0 / (float)CURBUFSIZE)); //放大后的电压
            m_fCurVolt[11] = m_fCurVolt[11]/100.0;                                //还原后的电流值
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[11],(uint8 *)&m_bFmtSampleB.m_bSample2IAADIC);
            break;
     case 0x0D:         
            res12-= m_ADCBuf12[pBuf12];
            m_ADCBuf12[pBuf12] =m_dIValue[12];
            res12 += m_ADCBuf12[pBuf12];
            pBuf12++;
            if(pBuf12 == CURBUFSIZE)  
            pBuf12=0;
            m_fCurVolt[12] = (float)( 3.0*((float)res12 / 4095.0 / (float)CURBUFSIZE)); //放大后的电压
            m_fCurVolt[12] = m_fCurVolt[12]/100.0;                                //还原后的电流值
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[12],(uint8 *)&m_bFmtSampleB.m_bSample2IAADID);
            break;
     case 0x0E:
            res13-= m_ADCBuf13[pBuf13];
            m_ADCBuf13[pBuf13] = m_dIValue[13];
            res13 += m_ADCBuf13[pBuf13];
            pBuf13++;
            if(pBuf13 == CURBUFSIZE)  
            pBuf13=0;
            m_fCurVolt[13] = (float)( 3.0*((float)res13 / 4095.0 / (float)CURBUFSIZE)); //放大后的电压
            m_fCurVolt[13] = m_fCurVolt[13]/100.0;                                //还原后的电流值
            if (DEVICE_ID==DEVICE_AP6B)    //AP6 B板
              FloatToASCII(m_fCurVolt[13],(uint8 *)&m_bFmtSampleB.m_bSample2IAADIE);
            break;
     default:
          break;
    }
}
    

