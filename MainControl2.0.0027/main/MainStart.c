#include <includes.h>
#include <stdio.h>
#include <Agreement.h>
#include <ZLG522S_Test.h>
#include <ZLG522S.h>
#include <ZLG522S_Comm.h>
#include <uart.h>
#include <I2C.h>
#include <GPIO.h>/////2011 8 10小改
#include <MotorXYZ.h>
#include <MotorHook.h>
#include <MotorSyringe.h>
#include <MotorCollect.h>
#include <Motor.h>
#include <Waterpump.h>
#include <MotorDrain.h>
#include <tim.h>
#include <MotorRoll.h>
#define MAX_SER_SIZE      			64+2+6
unsigned char g_ucSerBuf[MAX_SER_SIZE];      // 串行数据缓冲区
unsigned char g_ucSerBufIdx;	             // 串行数据缓冲区索引
unsigned char SerDTvalid;		     // 串行数据接收有效位
unsigned char SEQ;			     // 包号,见通信协议
//unsigned char cupstate=0;                                 // 抓杯状态
unsigned char LackFlag=0;                   //缺液标示

OS_EVENT       *I2CSem;
OS_EVENT       *UartSem;
OS_EVENT       *MutexUart2;







int  main (void)
{
    CPU_INT08U  os_err;
    BSP_IntDisAll();                                            /* Disable all ints until we are ready to accept them.  */
        
    OSInit();                                                     /* Initialize "uC/OS-II, The Real-Time Kernel".         */
                                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */
    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 12) 
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

    OSStart();     
    
    return (0);
}

static  void  App_TaskStart (void *p_arg)
{

         
   (void)p_arg;
    
   BSP_Init();                                                 /* Initialize BSP functions.                            */
   OS_CPU_SysTickInit();    
                                                                /* Initialize the SysTick.                              */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity.                              */
#endif
    Setupuart1();
    Setupuart2();
    I2C_GPIO_Config();
    MotorInit();
        
    Init_HookMagnetic();
    MotorCollectInit();
    Init_BeepGPIO();
    InitWaterPump();
    
    App_EventCreate();                                          /* Create application events.                           */
    App_TaskCreate();                                           /* Create application tasks.                            */    

    
        
    while(1){
        if (IS_MAGNETIC_ACT()){
          RESET_MAGNETIC_ACT();
          MagneticControl();
      }
      if (IS_BEEP_CON()){
          BeepControl();
          continue;
      }
      OSTimeDly(100);
    }
    
}

static  void  App_EventCreate (void){
  u8 err=0;
  UartSem=OSSemCreate(1);
  MutexUart2=OSMutexCreate (4, &err);
  
//  I2CSem=OSSemCreate(0);
}

static  void  App_TaskCreate (void)
{
     CPU_INT08U  os_err;
     
/*      os_err = OSTaskCreateExt((void (*)(void *))App_TaskIC,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskICStk[APP_TASK_IC_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_IC_PRIO,
                             (INT16U          ) APP_TASK_IC_PRIO,
                             (OS_STK        * )&App_TaskICStk[0],
                             (INT32U          ) APP_TASK_IC_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

   #if (OS_TASK_NAME_SIZE >= 10)
        OSTaskNameSet(APP_TASK_IC_PRIO, "HandleTaskIC", &os_err);
    #endif*/
        
     os_err = OSTaskCreateExt((void (*)(void *))App_TaskUart1_Deal,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskUart1_DealStk[APP_TASK_Uart1_Deal_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_Uart1_Deal_PRIO,
                             (INT16U          ) APP_TASK_Uart1_Deal_PRIO,
                             (OS_STK        * )&App_TaskUart1_DealStk[0],
                             (INT32U          ) APP_TASK_Uart1_Deal_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    
   #if (OS_TASK_NAME_SIZE >= 10)
        OSTaskNameSet(APP_TASK_Uart1_Deal_PRIO, "HandleTaskUart1", &os_err);
    #endif
        
     os_err = OSTaskCreateExt((void (*)(void *))App_TaskHandleXYZ,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskHandleXYZStk[APP_TASK_HandleXYZ_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleXYZ_PRIO,
                             (INT16U          ) APP_TASK_HandleXYZ_PRIO,
                             (OS_STK        * )&App_TaskHandleXYZStk[0],
                             (INT32U          ) APP_TASK_HandleXYZ_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleXYZ_PRIO, "HandleXYZ", &os_err);
#endif   
    
     os_err = OSTaskCreateExt((void (*)(void *))App_TaskHandleHook,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskHandleHookStk[APP_TASK_HandleHook_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleHook_PRIO,
                             (INT16U          ) APP_TASK_HandleHook_PRIO,
                             (OS_STK        * )&App_TaskHandleHookStk[0],
                             (INT32U          ) APP_TASK_HandleHook_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleHook_PRIO, "HandleHook", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *))App_TaskHandleSyringe,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskHandleSyringeStk[APP_TASK_HandleSyringe_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleSyringe_PRIO,
                             (INT16U          ) APP_TASK_HandleSyringe_PRIO,
                             (OS_STK        * )&App_TaskHandleSyringeStk[0],
                             (INT32U          ) APP_TASK_HandleSyringe_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleSyringe_PRIO, "HandleSyringe", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *))App_TaskHandleCollect,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskHandleCollectStk[APP_TASK_HandleCollect_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleCollect_PRIO,
                             (INT16U          ) APP_TASK_HandleCollect_PRIO,
                             (OS_STK        * )&App_TaskHandleCollectStk[0],
                             (INT32U          ) APP_TASK_HandleCollect_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleCollect_PRIO, "HandleCollect", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *))App_TaskHandleWaterPump,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskHandleWaterPumpStk[APP_TASK_HandleWaterPump_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleWaterPump_PRIO,
                             (INT16U          ) APP_TASK_HandleWaterPump_PRIO,
                             (OS_STK        * )&App_TaskHandleWaterPumpStk[0],
                             (INT32U          ) APP_TASK_HandleWaterPump_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleWaterPump_PRIO, "HandleWaterPump", &os_err);
#endif
    
/*       os_err = OSTaskCreateExt((void (*)(void *))App_TaskHandleDrain,
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskHandleDrainStk[APP_TASK_HandleDrain_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleDrain_PRIO,
                             (INT16U          ) APP_TASK_HandleDrain_PRIO,
                             (OS_STK        * )&App_TaskHandleDrainStk[0],
                             (INT32U          ) APP_TASK_HandleDrain_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleDrain_PRIO, "HandleDrain", &os_err);
#endif*/
   
}

static void  App_TaskHandleXYZ (void *p_arg){   
    InitXYZMotor();
//    int a=0;
    while (1)
    {
        HandleXYZ();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  App_TaskHandleHook (void *p_arg){   
    InitHookMotor();
    
    while (1)
    {
        HandleHook();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  App_TaskHandleSyringe (void *p_arg)
{   
    InitSyringeMotor();
    while (1)
    {
        HandleSyringe();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  App_TaskHandleCollect (void *p_arg){   
  LackWater();
  InitRollMotor();
    while (1)
    {
        HandleCollect();
        GetLackState();
        OSTimeDlyHMSM (0, 0, 0, 100);
    }
}

static void  App_TaskHandleWaterPump (void *p_arg){   
  InitWaterPumpMotor();
    while (1)
    {
        HandleWaterPump();
        OSTimeDlyHMSM (0, 0, 0, 100);
    }
}
/*
static void  App_TaskHandleDrain (void *p_arg){   
    //InitDrainMotor();
    while (1)
    {
        //HandleDrain();
        OSTimeDlyHMSM (0, 0, 0, 100);
    }
}*/

static void  App_TaskUart1_Deal (void *p_arg){   
    while (1)
    {
        if (FRAME_OK())
          HandleCommand();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }     
}   
/*       
static void  App_TaskIC (void *p_arg){  
  CupStateConfig();

    while (1)
    {
//        IC_AutoDetective();
      if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9))
        cupstate=0;
      else cupstate=1;
        OSTimeDlyHMSM (0, 0, 0, 100);
    } 
//    TestZLG500S(0);
}
*/










