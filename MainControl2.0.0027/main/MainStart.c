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
#include <MotorRotate.h>
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

#define MAXCOUNT    20

///






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
       
        
//        GPIO_SetBits(GPIOA,GPIO_Pin_4);
//        OSTimeDly(200);
//        GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//        OSTimeDly(200);
        if(Signal1 == 1)
        {
            GPIO_SetBits(GPIOA,GPIO_Pin_4);
        }
        else GPIO_ResetBits(GPIOA,GPIO_Pin_4);
        if(Signal2 == 1)
        {
            GPIO_SetBits(GPIOA,GPIO_Pin_5);
        }
        else GPIO_ResetBits(GPIOA,GPIO_Pin_5);
        if(Signal3 == 1)
        {
            GPIO_SetBits(GPIOA,GPIO_Pin_6);
        }
        else GPIO_ResetBits(GPIOA,GPIO_Pin_6);
        if(Signal4 == 1)
        {
            GPIO_SetBits(GPIOA,GPIO_Pin_7);
        }
        else GPIO_ResetBits(GPIOA,GPIO_Pin_7);
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
    
    
    os_err = OSTaskCreateExt((void (*)(void *))appTaskHandleRotate,
                             (void          * ) 0,
                             (OS_STK        * )&appTaskRotateStk[APP_TASK_Rotate_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_HandleRotate_PRIO,
                             (INT16U          ) APP_TASK_HandleRotate_PRIO,
                             (OS_STK        * )&appTaskRotateStk[0],
                             (INT32U          ) APP_TASK_Rotate_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if (OS_TASK_NAME_SIZE >= 10)
    OSTaskNameSet(APP_TASK_HandleRotate_PRIO, "TaskRotate", &os_err);
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
/*
状态：  1，两个进样位，右侧进样位需要传送到左侧的进样位后，再由驱动机构送入采样位。
        2，左边进样机构为急诊进样，从头到尾优先处理
        Signal2 and Signal4 是普通进样位
        Signal3 and Signal5 是急诊进样位



*/

static void  appTaskHandleAdd1 (void *p_arg){   
//    InitAdd1Motor();
//    InitAdd2Motor();
//    InitMove1Motor();
//    InitMove2Motor();
//    InitMove3Motor();
//    InitMove4Motor();
//    InitSubMotor();
//    resetAdd1Motor();
//    resetAdd2Motor();
//    resetMove1Motor();
//    resetMove2Motor();
//    resetMove3Motor();
//    resetMove4Motor();
//    resetSubMotor();
    // SendCommand1(NULL,"FJ00000100");
//    SendCommand1(NULL,"IJ0000078E");
    
//    int a=0;
    while (1)
    {

        if(Signal2 == 1)
        {
            // if((add2RunFlag != 1)&&(add2ResetFlag != 1))
            
            if((addCount != 0)&&(addRunFlag == 0)&&(addResetFlag == 1)&&(addCount != MAXCOUNT))
            {
                addStep = addTotalStep - addCount * addIntervelStep;
                SetAddPos(0x01,addStep);
                SET_Add_MOVE_FLAG();
                addRunFlag = 1;
                addResetFlag = 0;
                addCount++;
            }
            if((addCount == 0)&&(addRunFlag == 0)&&(addResetFlag == 1))
            {
                SetAddPos(0x01,addTotalStep);
                SET_Add_MOVE_FLAG();
                addRunFlag = 1;
                addResetFlag = 0;
                addCount++;
            }
        }
        if((addArriveFlag == 1)&&(Signal2 == 1))///只有到位后或触发到位开关的可以复位
        {
            RESET_Add_MOVE_FLAG();
            resetAddMotor();
            addArriveFlag = 0;
        }
        if((addCount != 0)&&(Signal4 != 1)&&(addRunFlag != 1))//进杯区当前数量不为0，但到位微动开关未触发，则驱动进杯机构使其到位。
        {
            SetAddPos(0x01,addTotalStep - addCount * addIntervelStep);//前进距离为当前数量X架间距
            SET_Add_MOVE_FLAG();
            addRunFlag = 1;
            addResetFlag = 0;
        }

        //
//        HandleAdd2();
        HandleMove1();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  appTaskHandleMove1 (void *p_arg){   
//    InitMove1Motor();
//    int a=0;
    while (1)
    {
//        HandleXYZ();
        //
        ////////横向传送机构运动
         if((addCount !=0)&&(Signal2 == 1)&&(subCount != 10))
        {
            move1Work();
            addCount --;
            subCount ++;
            // move1Count++;
//            add2Count ++;
        }
        
        //
        HandleRotate();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}

static void  appTaskHandleMove2 (void *p_arg){   
//    InitMove2Motor();
    
    while (1)
    {
//        HandleHook();
        //
//        if((add2Count != 0)&&(Signal5 == 1)&&(move3Count == 0))
//        {
//            move2Work();
            // if(move1Count == 1)
            // {
            //     move1Count = 0;
            // }
//            if(add2Count != 0)
//            {
//                add2Count --;
//            }
//            move2Count ++;
//        }
        //
        
//        HandleMove3();
        if(Signal4 == 1)
        {
            move2Work();
        }
        OSTimeDlyHMSM (0, 0, 0, 10);
    }
}


static void  appTaskHandleRotate (void *p_arg){   

//  InitMove4Motor();
    while (1)
    {
        //
//        if((move4WorkFlag == 0)&&(move3Count == 1))
//        {
//            move4Work();
//            move3Count --;
//            move4Count ++;
//        }
//        
        //
        // HandleMove4();
        HandleSub();
        OSTimeDlyHMSM (0, 0, 0, 100);
    }
}

static void  appTaskHandleSub (void *p_arg){   
    int step = 0;
//  InitSubMotor();
    while (1)
    {
        //
//        if((subWorkFlag == 0) && (move4Count == 1))
//        {
//            subWork();
//            move4Count --;
//            subCount ++;
//
//        }
//        //
//        // HandleSub();
//        HandleMove1();
        if(Signal3 == 1)
        {
            subWork();
        }
        if((Signal4 != 1)&&(subCount != 0))
        {
            step = subTotalStep - subCount * subIntervelStep;//subIntervelStep;//620;//
            SetSubPos(0x01,step);
            SET_Sub_MOVE_FLAG();
            subArriveFlag = 0;
            OSTimeDlyHMSM(0,0,0,20);
        }
        if(Signal4 == 1)
        {
            resetSubMotor();
            subResetFlag = 0;
        }
       OSTimeDlyHMSM (0, 0, 0, 100);
    }
}



static void  appTaskUart1_Deal (void *p_arg){
    InitAddMotor();
    InitMove1Motor();
    InitMove2Motor();
    InitRotateMotor();
    InitSubMotor();
    
    resetAddMotor();
    resetMove1Motor();
    resetMove2Motor();
    resetSubMotor();

//    SendCommand1(NULL,"IJ0000078E");
    while (1)
    {
        if (FRAME_OK())
          HandleCommand();
        OSTimeDlyHMSM (0, 0, 0, 10);
    }     
}   











