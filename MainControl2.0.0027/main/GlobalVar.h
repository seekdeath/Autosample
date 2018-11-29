
/*
*********************************************************************************************************
// usr var define
*********************************************************************************************************
*/

#ifndef  __GlobalVar_H__
#define  __GlobalVar_H__
#include  <ucos_ii.h>
#include  <bsp.h>


typedef unsigned char  uint8;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  int8;                    /* �з���8λ���ͱ���                        */
typedef unsigned short uint16;                  /* �޷���16λ���ͱ���                       */
typedef signed   short int16;                   /* �з���16λ���ͱ���                       */
typedef unsigned int   uint32;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   int32;                   /* �з���32λ���ͱ���                       */
typedef float          fp32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         fp64;                    /* ˫���ȸ�������64λ���ȣ�                 */


// //static  OS_STK          App_TaskHandleDrainStk[APP_TASK_HandleDrain_STK_SIZE];
// static  OS_STK          App_TaskHandleXYZStk[APP_TASK_HandleXYZ_STK_SIZE];
// static  OS_STK          App_TaskHandleHookStk[APP_TASK_HandleHook_STK_SIZE];
// static  OS_STK          App_TaskHandleSyringeStk[APP_TASK_HandleSyringe_STK_SIZE];
// static  OS_STK          App_TaskHandleCollectStk[APP_TASK_HandleCollect_STK_SIZE];
// static  OS_STK          App_TaskHandleWaterPumpStk[APP_TASK_HandleWaterPump_STK_SIZE];
// static  OS_STK          App_TaskStartStk[APP_TASK_START_STK_SIZE]; 
// //static  OS_STK          App_TaskICStk[APP_TASK_IC_STK_SIZE]; 
// static  OS_STK          App_TaskUart1_DealStk[APP_TASK_Uart1_Deal_STK_SIZE]; 


// #define  APP_TASK_START_STK_SIZE                    64
// #define  APP_TASK_Uart1_Deal_STK_SIZE               256//128
// #define  APP_TASK_Add1_STK_SIZE                     32
// #define  APP_TASK_Add2_STK_SIZE                     128//64
// #define  APP_TASK_Move1_STK_SIZE                    128
// #define  APP_TASK_Move2_STK_SIZE                    128//64
// #define  APP_TASK_Move3_STK_SIZE                    128
// #define  APP_TASK_Move4_STK_SIZE                    128//32
// #define  APP_TASK_Sub_STK_SIZE                      128

static  OS_STK          appTaskStartStk[APP_TASK_START_STK_SIZE]; 
static  OS_STK          appTaskUart1_DealStk[APP_TASK_Uart1_Deal_STK_SIZE];
static  OS_STK          appTaskAdd1Stk[APP_TASK_Add1_STK_SIZE]; 
static  OS_STK          appTaskAdd2Stk[APP_TASK_Add2_STK_SIZE]; 
static  OS_STK          appTaskMove1Stk[APP_TASK_Move1_STK_SIZE]; 
static  OS_STK          appTaskMove2Stk[APP_TASK_Move2_STK_SIZE];
static  OS_STK          appTaskMove3Stk[APP_TASK_Move3_STK_SIZE];
static  OS_STK          appTaskMove4Stk[APP_TASK_Move4_STK_SIZE];
static  OS_STK          appTaskSubStk[APP_TASK_Sub_STK_SIZE];
static  OS_STK          appTaskStk[APP_TASK_STK_SIZE];
static  OS_STK          appTaskLedStk[APP_TASK_LED_STK_SIZE];
//---------------------------------------------------
static  void  App_TaskCreate              (void);
static  void  App_EventCreate             (void);
static  void  App_TaskStart               (void        *p_arg);
static  void  appTaskUart1_Deal          (void        *p_arg);

static  void  appTaskHandleAdd1         (void        *p_arg);
static  void  appTaskHandleAdd2         (void        *p_arg);
static  void  appTaskHandleMove1        (void        *p_arg);
static  void  appTaskHandleMove2        (void        *p_arg);
static  void  appTaskHandleMove3        (void        *p_arg);
static  void  appTaskHandleMove4        (void        *p_arg);
static  void  appTaskHandleSub          (void        *p_arg);
static  void  appTaskHandle                   (void        *p_arg);
static  void  appTaskHandleLed          (void        *p_arg);

#endif
