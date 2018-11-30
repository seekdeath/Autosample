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
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   IAR STM32-SK Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.10
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_LCD_EN                            DEF_DISABLED//  DEF_ENABLED
#define  APP_OS_PROBE_EN                       DEF_DISABLED // DEF_ENABLED
#define  APP_PROBE_COM_EN                      DEF_DISABLED//  DEF_ENABLED

/*
*********************************************************************************************************
*                                            BSP CONFIGURATION
*********************************************************************************************************
*/

#define  BSP_CFG_LED_SPI2_EN                     DEF_ENABLED    /* Enable/disable LEDs on SPI port.                     */
#define  BSP_CFG_LED_PIOC_EN                     DEF_ENABLED    /* Enable/disable PIOC LEDs.                            */

/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/



#define  OS_TASK_TMR_PRIO                           OS_LOWEST_PRIO-2
#define  APP_TASK_Uart1_Deal_PRIO                   OS_LOWEST_PRIO-15
#define  APP_TASK_START_PRIO                        OS_LOWEST_PRIO-3

#define  APP_TASK_HandleAdd1_PRIO                   OS_LOWEST_PRIO-13
#define  APP_TASK_HandleAdd2_PRIO                   OS_LOWEST_PRIO-8
#define  APP_TASK_HandleMove1_PRIO                  OS_LOWEST_PRIO-7
#define  APP_TASK_HandleMove2_PRIO                  OS_LOWEST_PRIO-5
#define  APP_TASK_HandleMove3_PRIO                  OS_LOWEST_PRIO-11
#define  APP_TASK_HandleMove4_PRIO                  OS_LOWEST_PRIO-9
#define  APP_TASK_HandleSub_PRIO                    OS_LOWEST_PRIO-12
#define  APP_TASK_PRIO                              OS_LOWEST_PRIO-16
#define  APP_TASK_LED_PRIO                          OS_LOWEST_PRIO-4

// #define  APP_TASK_HandleXYZ_PRIO                    OS_LOWEST_PRIO-10
// #define  APP_TASK_HandleSyringe_PRIO                OS_LOWEST_PRIO-8
// #define  APP_TASK_HandleHook_PRIO                   OS_LOWEST_PRIO-7
// #define  APP_TASK_HandleWaterPump_PRIO              OS_LOWEST_PRIO-5
// #define  APP_TASK_HandleCollect_PRIO                OS_LOWEST_PRIO-4
// #define  APP_TASK_START_PRIO                        OS_LOWEST_PRIO-3


//#define  APP_TASK_HandleDrain_PRIO                  OS_LOWEST_PRIO-9
//#define  APP_TASK_IC_PRIO                           OS_LOWEST_PRIO-3
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/



// #define  APP_TASK_START_STK_SIZE                   64
// //#define  APP_TASK_HandleDrain_STK_SIZE             32
// #define  APP_TASK_HandleXYZ_STK_SIZE               64//64
// #define  APP_TASK_HandleSyringe_STK_SIZE           128
// #define  APP_TASK_HandleCollect_STK_SIZE           128//64
// #define  APP_TASK_HandleHook_STK_SIZE              128
// #define  APP_TASK_HandleWaterPump_STK_SIZE         128//32
// //#define  APP_TASK_IC_STK_SIZE                      64
// #define  APP_TASK_Uart1_Deal_STK_SIZE              256//128

#define  APP_TASK_START_STK_SIZE                    44
#define  APP_TASK_Uart1_Deal_STK_SIZE               50//128
#define  APP_TASK_Add1_STK_SIZE                     120
#define  APP_TASK_Add2_STK_SIZE                     120//64
#define  APP_TASK_Move1_STK_SIZE                    120
#define  APP_TASK_Move2_STK_SIZE                    120//64
#define  APP_TASK_Move3_STK_SIZE                    140
#define  APP_TASK_Move4_STK_SIZE                    100//32
#define  APP_TASK_Sub_STK_SIZE                      120
//#define  APP_TASK_STK_SIZE                          200//256
//#define  APP_TASK_LED_STK_SIZE                      32


/*
*********************************************************************************************************
*                                                 LCD
*********************************************************************************************************
*/

#define  DISP_BUS_WIDTH                                    4    /* LCD controller is accessed with a 4 bit bus.         */

/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED

/*
*********************************************************************************************************
*                                                 PROBE
*********************************************************************************************************
*/

#define  OS_PROBE_TASK                                     1    /* Task will be created for uC/Probe OS Plug-In.        */
#define  OS_PROBE_TMR_32_BITS                              0    /* uC/Probe OS Plugin timer is a 16-bit timer.          */
#define  OS_PROBE_TIMER_SEL                                2    /* Select timer 2.                                      */
#define  OS_PROBE_HOOKS_EN                                 1    /* Hooks to update OS_TCB profiling members included.   */
#define  OS_PROBE_USE_FP                                   1



#endif
