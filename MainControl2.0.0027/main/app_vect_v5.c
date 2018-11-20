/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                         EXCEPTION VECTORS
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   IAR STM32-SK Evaluation Board
*
* Filename      : app_vect_v5.c
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

#include <includes.h>
#include <ZLG522S_Comm.h>
#include <I2C.h>
#include <Motortrasmition.h>
#include <Agreement.h>
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  union {
    CPU_FNCT_VOID   Fnct;
    void           *Ptr;
} APP_INTVECT_ELEM;

/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

#pragma language=extended
#pragma segment="CSTACK"

static  void  App_NMI_ISR        (void);

static  void  App_Fault_ISR      (void);

static  void  App_BusFault_ISR   (void);

static  void  App_UsageFault_ISR (void);

static  void  App_MemFault_ISR   (void);

static  void  App_Spurious_ISR   (void);

extern  void  __iar_program_start(void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  EXCEPTION / INTERRUPT VECTOR TABLE
*
* Note(s) : (1) The Cortex-M3 may have up to 256 external interrupts, which are the final entries in the
*               vector table.  The STM32 has 48 external interrupt vectors.
*********************************************************************************************************
*/

__root  const  APP_INTVECT_ELEM  __vector_table[] @ ".intvec" = {
    { .Ptr = (void *)__sfe( "CSTACK" )},                        /*  0, SP start value.                                  */
    __iar_program_start,                                        /*  1, PC start value.                                  */
    App_NMI_ISR,                                                /*  2, NMI.                                             */
    App_Fault_ISR,                                              /*  3, Hard Fault.                                      */
    App_MemFault_ISR,                                           /*  4, Memory Management.                               */
    App_BusFault_ISR,                                           /*  5, Bus Fault.                                       */
    App_UsageFault_ISR,                                         /*  6, Usage Fault.                                     */
    App_Spurious_ISR,                                           /*  7, Reserved.                                        */
    App_Spurious_ISR,                                           /*  8, Reserved.                                        */
    App_Spurious_ISR,                                           /*  9, Reserved.                                        */
    App_Spurious_ISR,                                           /* 10, Reserved.                                        */
    App_Spurious_ISR,                                           /* 11, SVCall.                                          */
    App_Spurious_ISR,                                           /* 12, Debug Monitor.                                   */
    App_Spurious_ISR,                                           /* 13, Reserved.                                        */
    OS_CPU_PendSVHandler,                                       /* 14, PendSV Handler.                                  */
    OS_CPU_SysTickHandler,                                      /* 15, uC/OS-II Tick ISR Handler.                       */

    BSP_IntHandlerWWDG,                                         /* 16, INTISR[  0]  Window Watchdog.                    */
    BSP_IntHandlerPVD,                                          /* 17, INTISR[  1]  PVD through EXTI Line Detection.    */
    BSP_IntHandlerTAMPER,                                       /* 18, INTISR[  2]  Tamper Interrupt.                   */
    BSP_IntHandlerRTC,                                          /* 19, INTISR[  3]  RTC Global Interrupt.               */
    BSP_IntHandlerFLASH,                                        /* 20, INTISR[  4]  FLASH Global Interrupt.             */
    BSP_IntHandlerRCC,                                          /* 21, INTISR[  5]  RCC Global Interrupt.               */
    BSP_IntHandlerEXTI0,                                        /* 22, INTISR[  6]  EXTI Line0 Interrupt.               */
    BSP_IntHandlerEXTI1,                                        /* 23, INTISR[  7]  EXTI Line1 Interrupt.               */
    BSP_IntHandlerEXTI2,                                        /* 24, INTISR[  8]  EXTI Line2 Interrupt.               */
    BSP_IntHandlerEXTI3,                                        /* 25, INTISR[  9]  EXTI Line3 Interrupt.               */
    BSP_IntHandlerEXTI4,                                        /* 26, INTISR[ 10]  EXTI Line4 Interrupt.               */
    BSP_IntHandlerDMA1_CH1,                                     /* 27, INTISR[ 11]  DMA Channel1 Global Interrupt.      */
    BSP_IntHandlerDMA1_CH2,                                     /* 28, INTISR[ 12]  DMA Channel2 Global Interrupt.      */
    BSP_IntHandlerDMA1_CH3,                                     /* 29, INTISR[ 13]  DMA Channel3 Global Interrupt.      */
    BSP_IntHandlerDMA1_CH4,                                     /* 30, INTISR[ 14]  DMA Channel4 Global Interrupt.      */
    BSP_IntHandlerDMA1_CH5,                                     /* 31, INTISR[ 15]  DMA Channel5 Global Interrupt.      */

    BSP_IntHandlerDMA1_CH6,                                     /* 32, INTISR[ 16]  DMA Channel6 Global Interrupt.      */
    BSP_IntHandlerDMA1_CH7,                                     /* 33, INTISR[ 17]  DMA Channel7 Global Interrupt.      */
    BSP_IntHandlerADC1_2,                                       /* 34, INTISR[ 18]  ADC1 & ADC2 Global Interrupt.       */
    BSP_IntHandlerUSB_HP_CAN_TX,                                /* 35, INTISR[ 19]  USB High Prio / CAN TX  Interrupts. */
    BSP_IntHandlerUSB_LP_CAN_RX0,                               /* 36, INTISR[ 20]  USB Low  Prio / CAN RX0 Interrupts. */
    BSP_IntHandlerCAN_RX1,                                      /* 37, INTISR[ 21]  CAN RX1 Interrupt.                  */
    BSP_IntHandlerCAN_SCE,                                      /* 38, INTISR[ 22]  CAN SCE Interrupt.                  */
    BSP_IntHandlerEXTI9_5,                                      /* 39, INTISR[ 23]  EXTI Line[9:5] Interrupt.           */
    BSP_IntHandlerTIM1_BRK,                                     /* 40, INTISR[ 24]  TIM1 Break  Interrupt.              */
    BSP_IntHandlerTIM1_UP,                                      /* 41, INTISR[ 25]  TIM1 Update Interrupt.              */
    BSP_IntHandlerTIM1_TRG_COM,                                 /* 42, INTISR[ 26]  TIM1 Trig & Commutation Interrupts. */
    BSP_IntHandlerTIM1_CC,                                      /* 43, INTISR[ 27]  TIM1 Capture Compare Interrupt.     */
    BSP_IntHandlerTIM2,                                         /* 44, INTISR[ 28]  TIM2 Global Interrupt.              */
    BSP_IntHandlerTIM3,                                         /* 45, INTISR[ 29]  TIM3 Global Interrupt.              */
    BSP_IntHandlerTIM4,                                         /* 46, INTISR[ 30]  TIM4 Global Interrupt.              */
    BSP_IntHandlerI2C1_EV,                                      /* 47, INTISR[ 31]  I2C1 Event  Interrupt.              */

    BSP_IntHandlerI2C1_ER,                                      /* 48, INTISR[ 32]  I2C1 Error  Interrupt.              */
    BSP_IntHandlerI2C2_EV,                                      /* 49, INTISR[ 33]  I2C2 Event  Interrupt.              */
    BSP_IntHandlerI2C2_ER,                                      /* 50, INTISR[ 34]  I2C2 Error  Interrupt.              */
    BSP_IntHandlerSPI1,                                         /* 51, INTISR[ 35]  SPI1 Global Interrupt.              */
    BSP_IntHandlerSPI2,                                         /* 52, INTISR[ 36]  SPI2 Global Interrupt.              */
    BSP_IntHandlerUSART1,                                       /* 53, INTISR[ 37]  USART1 Global Interrupt.            */
    BSP_IntHandlerUSART2,                                       /* 54, INTISR[ 38]  USART2 Global Interrupt.            */
    BSP_IntHandlerUSART3,                                       /* 55, INTISR[ 39]  USART3 Global Interrupt.            */
    BSP_IntHandlerEXTI15_10,                                    /* 56, INTISR[ 40]  EXTI Line [15:10] Interrupts.       */
    BSP_IntHandlerRTCAlarm,                                     /* 57, INTISR[ 41]  RTC Alarm EXT Line Interrupt.       */
    BSP_IntHandlerUSBWakeUp,                                    /* 58, INTISR[ 42]  USB Wakeup from Suspend EXTI Int.   */
    BSP_IntHandlerUSBWakeUp,                          //#define  BSP_INT_ID_TIM8_BRK                              43    /* TIM8 Break Interrupt                                 */
    BSP_IntHandlerUSBWakeUp,                         //#define  BSP_INT_ID_TIM8_UP                               44    /* TIM8 Update Interrupt                                */
    BSP_IntHandlerUSBWakeUp,                        //#define  BSP_INT_ID_TIM8_TRG_COM                          45    /* TIM8 Trigger and Commutation Interrupt               */
    BSP_IntHandlerUSBWakeUp,                       //#define  BSP_INT_ID_TIM8_CC                               46    /* TIM8 Capture Compare Interrupt                       */
    BSP_IntHandlerUSBWakeUp,                        //#define  BSP_INT_ID_ADC3                                  47    /* ADC3 global Interrupt                                */
    BSP_IntHandlerUSBWakeUp,                        //#define  BSP_INT_ID_FSMC                                  48    /* FSMC global Interrupt                                */
    BSP_IntHandlerUSBWakeUp,                         //#define  BSP_INT_ID_SDIO                                  49    /* SDIO global Interrupt                                */
    BSP_IntHandlerUSBWakeUp,                           //#define  BSP_INT_ID_TIM5                                  50    /* TIM5 global Interrupt                                */
    BSP_IntHandlerUSBWakeUp,                         //#define  BSP_INT_ID_SPI3                                  51    /* SPI3 global Interrupt                                */
    BSP_IntHandlerUSART4,                             //#define  BSP_INT_ID_UART4                                 52    /* UART4 global Interrupt                               */
    BSP_IntHandlerUSART5,                               //#define  BSP_INT_ID_UART5                                 53    /* UART5 global Interrupt                               */
    BSP_IntHandlerUSBWakeUp                         //#define  BSP_INT_ID_TIM6                                  54    /* TIM6 global Interrupt                                */
                              //#define  BSP_INT_ID_TIM7                                  55    /* TIM7 global Interrupt                                */
                                    //#define  BSP_INT_ID_DMA2_CH1                              56    /* DMA2 Channel 1 global Interrupt                      */
                                              //#define  BSP_INT_ID_DMA2_CH2                              57    /* DMA2 Channel 2 global Interrupt                      */
                                      //#define  BSP_INT_ID_DMA2_CH3                              58    /* DMA2 Channel 3 global Interrupt                      */
                                          //#define  BSP_INT_ID_DMA2_CH4_5                            59    /* DMA2 Channel 4 and DMA2 Channel 5 global Interrupt   */   
    
};

/*
*********************************************************************************************************
*                                            App_NMI_ISR()
*
* Description : Handle Non-Maskable Interrupt (NMI).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) Since the NMI is not being used, this serves merely as a catch for a spurious
*                   exception.
*********************************************************************************************************
*/

static  void  App_NMI_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}

/*
*********************************************************************************************************
*                                             App_Fault_ISR()
*
* Description : Handle hard fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Fault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_BusFault_ISR()
*
* Description : Handle bus fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_BusFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                          App_UsageFault_ISR()
*
* Description : Handle usage fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_UsageFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_MemFault_ISR()
*
* Description : Handle memory fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_MemFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle spurious interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Spurious_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}

extern OS_EVENT       *UARTSem;
static void BSP_IntHandlerUSART1()
{
    u16 RxData;
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif
    CPU_CRITICAL_ENTER();                                       
    OSIntNesting++;
    CPU_CRITICAL_EXIT();
  //当数据溢出的时候，RXNEIE为1，这个时候数据是无用的数据必须读出
    if(((USART1->SR&0x08)==0x08)&&((USART1->CR1&0x20)==0x20)){
        USART_ReceiveData(USART1); 
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);  
    }else if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        RxData= USART_ReceiveData(USART1); 
        ReceiverData(RxData);
//        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);           
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);                                 
    }
    OSIntExit();
}

static void BSP_IntHandlerUSART2()
{
    u16 RxData;
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif
    CPU_CRITICAL_ENTER();                                       
    OSIntNesting++;
    CPU_CRITICAL_EXIT();
  //当数据溢出的时候，RXNEIE为1，这个时候数据是无用的数据必须读出
    if(((USART2->SR&0x08)==0x08)&&((USART2->CR1&0x20)==0x20)){
        USART_ReceiveData(USART2); 
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);  
    }else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        RxData= USART_ReceiveData(USART2); 
        ReceiverData1(RxData);
//        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);           
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);                                 
    }
    OSIntExit();
}

extern OS_EVENT *I2CSem;
static void BSP_IntHandlerEXTI9_5()
{
    if (EXTI_GetITStatus(EXTI_Line5))
    {
        if (I2C_ReadByte(g_ucSerBuf,0xB2)){
          SerDTvalid=TRUE;
          OSSemPost(I2CSem);
        }
        EXTI_ClearFlag(EXTI_Line5);
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
}
