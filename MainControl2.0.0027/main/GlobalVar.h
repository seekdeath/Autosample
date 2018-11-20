
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


//static  OS_STK          App_TaskHandleDrainStk[APP_TASK_HandleDrain_STK_SIZE];
static  OS_STK          App_TaskHandleXYZStk[APP_TASK_HandleXYZ_STK_SIZE];
static  OS_STK          App_TaskHandleHookStk[APP_TASK_HandleHook_STK_SIZE];
static  OS_STK          App_TaskHandleSyringeStk[APP_TASK_HandleSyringe_STK_SIZE];
static  OS_STK          App_TaskHandleCollectStk[APP_TASK_HandleCollect_STK_SIZE];
static  OS_STK          App_TaskHandleWaterPumpStk[APP_TASK_HandleWaterPump_STK_SIZE];
static  OS_STK          App_TaskStartStk[APP_TASK_START_STK_SIZE]; 
//static  OS_STK          App_TaskICStk[APP_TASK_IC_STK_SIZE]; 
static  OS_STK          App_TaskUart1_DealStk[APP_TASK_Uart1_Deal_STK_SIZE]; 
//---------------------------------------------------
static  void  App_TaskCreate              (void);
static  void  App_EventCreate             (void);
static  void  App_TaskStart               (void        *p_arg);
static  void  App_TaskHandleDrain         (void        *p_arg);
static  void  App_TaskHandleXYZ           (void        *p_arg);
static  void  App_TaskHandleHook          (void        *p_arg);
static  void  App_TaskHandleSyringe       (void        *p_arg);
static  void  App_TaskHandleCollect       (void        *p_arg);
static  void  App_TaskHandleWaterPump     (void        *p_arg);
static  void  App_TaskIC                  (void        *p_arg);
static  void  App_TaskUart1_Deal          (void        *p_arg);
#endif
