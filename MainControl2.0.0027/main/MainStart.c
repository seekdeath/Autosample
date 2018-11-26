#include <includes.h>
#include <stdio.h>
#include <Agreement.h>
//#include <ZLG522S_Test.h>
//#include <ZLG522S.h>
//#include <ZLG522S_Comm.h>
#include <uart.h>
#include <I2C.h>
#include <GPIO.h>/////2011 8 10小改
//#include <MotorXYZ.h>
//#include <MotorHook.h>
//#include <MotorSyringe.h>
//#include <MotorCollect.h>
#include <Motor.h>
//#include <Waterpump.h>
//#include <MotorDrain.h>
#include <tim.h>
//#include <MotorRoll.h>
#include <MotorAddSam.h>
#include <MotorMoveSam.h>
#include <MotorSubSam.h>
#include <microswitch.h>
#include <Motortrasmition.h>
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
                             (OS_STK        * )&appTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&appTaskStartStk[0],
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
    initMS();
        
//    Init_HookMagnetic();
//    MotorCollectInit();
//    Init_BeepGPIO();
//    InitWaterPump();
    
    App_EventCreate();                                          /* Create application events.                           */
    App_TaskCreate();                                           /* Create application tasks.                            */    

    
        
    while(1)
    {
        // if (IS_MAGNETIC_ACT())
        // {
        //   RESET_MAGNETIC_ACT();
        //   MagneticControl();
        // }
        // if (IS_BEEP_CON())
        // {
        //   BeepControl();
        //   continue;
        // }
        
            GPIO_SetBits(GPIOA,GPIO_Pin_4);
            OSTimeDly(200);
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
    OSTimeDly(200);
     
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
     
        
     os_err = OSTaskCreateExt((void (*)(void *))appTaskUart1_Deal,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskUart1_DealStk[APP_TASK_Uart1_Deal_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_Uart1_Deal_PRIO,
                             (INT16U          ) APP_TASK_Uart1_Deal_PRIO,
                             (OS_STK        * )&appTaskUart1_DealStk[0],
                             (INT32U          ) APP_TASK_Uart1_Deal_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
    
   #if (OS_TASK_NAME_SIZE >= 10)
        OSTaskNameSet(APP_TASK_Uart1_Deal_PRIO, "TaskUart1", &os_err);
    #endif
        
     os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleAdd1,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskAdd1Stk[APP_TASK_Add1_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleAdd1_PRIO,
                             (INT16U          ) APP_TASK_HandleAdd1_PRIO,
                             (OS_STK        * )&appTaskAdd1Stk[0],
                             (INT32U          ) APP_TASK_Add1_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleAdd1_PRIO, "TaskAdd1", &os_err);
#endif   
    
     os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleAdd2,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskAdd2Stk[APP_TASK_Add2_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleAdd2_PRIO,
                             (INT16U          ) APP_TASK_HandleAdd2_PRIO,
                             (OS_STK        * )&appTaskAdd2Stk[0],
                             (INT32U          ) APP_TASK_Add2_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleAdd2_PRIO, "TaskAdd2", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleMove1,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskMove1Stk[APP_TASK_Move1_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleMove1_PRIO,
                             (INT16U          ) APP_TASK_HandleMove1_PRIO,
                             (OS_STK        * )&appTaskMove1Stk[0],
                             (INT32U          ) APP_TASK_Move1_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleMove1_PRIO, "TaskMove1", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleMove2,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskMove2Stk[APP_TASK_Move2_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleMove2_PRIO,
                             (INT16U          ) APP_TASK_HandleMove2_PRIO,
                             (OS_STK        * )&appTaskMove2Stk[0],
                             (INT32U          ) APP_TASK_Move2_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleMove2_PRIO, "TaskMove2", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleMove3,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskMove3Stk[APP_TASK_Move3_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleMove3_PRIO,
                             (INT16U          ) APP_TASK_HandleMove3_PRIO,
                             (OS_STK        * )&appTaskMove3Stk[0],
                             (INT32U          ) APP_TASK_Move3_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleMove3_PRIO, "TaskMove3", &os_err);
#endif
    
    os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleMove4,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskMove4Stk[APP_TASK_Move4_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleMove4_PRIO,
                             (INT16U          ) APP_TASK_HandleMove4_PRIO,
                             (OS_STK        * )&appTaskMove4Stk[0],
                             (INT32U          ) APP_TASK_Move4_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleMove4_PRIO, "TaskMove4", &os_err);
#endif

    os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleSub,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskSubStk[APP_TASK_Sub_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleSub_PRIO,
                             (INT16U          ) APP_TASK_HandleSub_PRIO,
                             (OS_STK        * )&appTaskSubStk[0],
                             (INT32U          ) APP_TASK_Sub_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleSub_PRIO, "TaskSub", &os_err);
#endif

   
}

static void  appTaskHandleAdd1 (void *p_arg){   
    InitAdd1Motor();
    InitAdd2Motor();
    InitMove1Motor();
    InitMove2Motor();
    InitMove3Motor();
    InitMove4Motor();
    InitSubMotor();
    resetAdd1Motor();
    resetAdd2Motor();
//    resetMove1Motor();
//    resetMove2Motor();
//    resetMove3Motor();
//    resetMove4Motor();
//    resetSubMotor();
//    int a=0;
    while (1)
    {
//        HandleXYZ();
        if(Signal2 == 1)
        {
            SendCommand1(NULL,"CJ00001B50");
        }
        if(Signal4 == 1)
        {
            SendCommand1(NULL,"CD");
        }
        if(Signal3 == 1)
        {
            SendCommand1(NULL,"DJ00001B50");
        }
        if(Signal5 == 1)
        {
            SendCommand1(NULL,"DD");
        }
        
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  appTaskHandleAdd2 (void *p_arg){   
//    InitAdd2Motor();
//    int a=0;
    while (1)
    {
//        HandleXYZ();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  appTaskHandleMove1 (void *p_arg){   
//    InitMove1Motor();
//    int a=0;
    while (1)
    {
//        HandleXYZ();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  appTaskHandleMove2 (void *p_arg){   
//    InitMove2Motor();
    
    while (1)
    {
//        HandleHook();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  appTaskHandleMove3 (void *p_arg)
{   
//    InitMove3Motor();
    while (1)
    {
//        HandleSyringe();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  appTaskHandleMove4 (void *p_arg){   

//  InitMove4Motor();
    while (1)
    {

        OSTimeDlyHMSM (0, 0, 0, 100);
    }
}

static void  appTaskHandleSub (void *p_arg){   
//  InitSubMotor();
    while (1)
    {
        HandleSub();
        OSTimeDlyHMSM (0, 0, 0, 100);
    }
}


static void  appTaskUart1_Deal (void *p_arg){   
    while (1)
    {
        if (FRAME_OK())
          HandleCommand();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }     
}   











