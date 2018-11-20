

#define IN_QUEUE
#include "includes.h"

/*********************************************************************************************************
** ��������: QueueCreate
** ��������: ��ʼ�����ݶ���
** �䡡��: Buf      ��Ϊ���з���Ĵ洢�ռ��ַ
**         SizeOfBuf��Ϊ���з���Ĵ洢�ռ��С���ֽڣ�
**         ReadEmpty��Ϊ���ж���ʱ�������
**         WriteFull��Ϊ����д��ʱ�������
** �䡡��: NOT_OK:��������
**         QUEUE_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 QueueCreate(void *Buf,uint32 SizeOfBuf,uint8 (* ReadEmpty)(),uint8 (* WriteFull)())
  {
    DataQueue *Queue; // ������нṹ Queue
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif
    if (Buf != NULL && SizeOfBuf >= (sizeof(DataQueue)))        // �жϲ����Ƿ���Ч 
      {
        Queue = (DataQueue *)Buf;                               // 
        OS_ENTER_CRITICAL();
                                                                // ��ʼ���ṹ������ 
        Queue->MaxData = (SizeOfBuf - (uint32)(((DataQueue *)0)->Buf)) / 
                         sizeof(QUEUE_DATA_TYPE);               // ������п��Դ洢��������Ŀ 
        Queue->End = Queue->Buf + Queue->MaxData;               // �������ݻ���Ľ�����ַ 
        Queue->Out = Queue->Buf;
        Queue->In = Queue->Buf;
        Queue->NData = 0;
        Queue->ReadEmpty = ReadEmpty;
        Queue->WriteFull = WriteFull;

        OS_EXIT_CRITICAL();

        return QUEUE_OK;
      }
    else
      {
        return NOT_OK;
      }
  }


/*********************************************************************************************************
** ��������: QueueRead
** ��������: ��ȡ�����е�����
** �䡡��: Ret:�洢���ص���Ϣ�ĵ�ַ
**         Buf:ָ����е�ָ��
** �䡡��: NOT_OK     ����������
**         QUEUE_OK   ���յ���Ϣ
**         QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 QueueRead(QUEUE_DATA_TYPE *Ret, void *Buf)
  {
    uint8 err;
    DataQueue *Queue;
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif
    err = NOT_OK;
    if (Buf != NULL)                                            /* �����Ƿ���Ч */
      {                                                         /* ��Ч */
        Queue = (DataQueue *)Buf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->NData > 0)                                   /* �����Ƿ�Ϊ�� */
          {                                                     /* ����         */
            *Ret = Queue->Out[0];                               /* ���ݳ���     */
            Queue->Out++;                                       /* ��������ָ�� */
            if (Queue->Out >= Queue->End)
              {
                Queue->Out = Queue->Buf;
              }
            Queue->NData--;                                     /* ���ݼ���      */
            err = QUEUE_OK;
          }
        else
          {                                                     /* ��              */
            err = QUEUE_EMPTY;
            if (Queue->ReadEmpty != NULL)                       /* �����û������� */
              {
                err = Queue->ReadEmpty(Ret, Queue);
              }
          }
        OS_EXIT_CRITICAL();
      }
    return err;
  }

/*********************************************************************************************************
** ��������: QueueWrite
** ��������: FIFO��ʽ��������
** �䡡��: Buf :ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: NOT_OK   :��������
**         QUEUE_FULL:������
**         QUEUE_OK  :���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_WRITE
#define EN_QUEUE_WRITE      0
#endif

#if EN_QUEUE_WRITE > 0

uint8 QueueWrite(void *Buf, QUEUE_DATA_TYPE Data)
  {
    uint8 err;
    DataQueue *Queue;
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif
    err = NOT_OK;
    if (Buf != NULL)                                                    /* �����Ƿ���Ч */
      {
        Queue = (DataQueue *)Buf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)                              /* �����Ƿ���  */
          {                                                             /* ����        */
            Queue->In[0] = Data;                                        /* �������    */
            Queue->In++;                                                /* �������ָ��*/
            if (Queue->In >= Queue->End)
              {
                Queue->In = Queue->Buf;
              }
            Queue->NData++;                                             /* ��������    */
            err = QUEUE_OK;
          }
        else
          {                                                             /* ��           */
            err = QUEUE_FULL;
            if (Queue->WriteFull != NULL)                               /* �����û������� */
              {
                err = Queue->WriteFull(Queue, Data, Q_WRITE_MODE);
              }
          }
        OS_EXIT_CRITICAL();
      }
    return err;
  }
#endif

/*********************************************************************************************************
** ��������: QueueWriteFront
** ��������: LIFO��ʽ��������
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: QUEUE_FULL:������
**         QUEUE_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_WRITE_FRONT
#define EN_QUEUE_WRITE_FRONT    0
#endif

#if EN_QUEUE_WRITE_FRONT > 0

uint8 QueueWriteFront(void *Buf, QUEUE_DATA_TYPE Data)
  {
    uint8 err;
    DataQueue *Queue;

    err = NOT_OK;
    if (Buf != NULL)                                                    /* �����Ƿ���Ч */
      {
        Queue = (DataQueue *)Buf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)                              /* �����Ƿ���  */
          {                                                             /* ���� */
            Queue->Out--;                                               /* ��������ָ�� */
            if (Queue->Out < Queue->Buf)
              {
                Queue->Out = Queue->End - 1;
              }
            Queue->Out[0] = Data;                                       /* �������     */
            Queue->NData++;                                             /* ������Ŀ���� */
            err = QUEUE_OK;
          }
        else
          {                                                             /* ��           */
            err = QUEUE_FULL;
            if (Queue->WriteFull != NULL)                               /* �����û������� */
              {
                err = Queue->WriteFull(Queue, Data, Q_WRITE_FRONT_MODE);
              }
         }
        OS_EXIT_CRITICAL();
      }
    return err;
  }

#endif

/*********************************************************************************************************
** ��������: QueueNData
** ��������: ȡ�ö�����������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_NDATA
#define EN_QUEUE_NDATA    0
#endif

#if EN_QUEUE_NDATA > 0

uint16 QueueNData(void *Buf)
  {
    uint16 temp;
    #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif   
    temp = 0;                                                   /* ������Ч����0 */
    if (Buf != NULL)
      {
        OS_ENTER_CRITICAL();
        temp = ((DataQueue *)Buf)->NData;
        OS_EXIT_CRITICAL();
      }
    return temp;
  }

#endif

/*********************************************************************************************************
** ��������: QueueSize
** ��������: ȡ�ö���������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ����������
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_SIZE
#define EN_QUEUE_SIZE    0
#endif

#if EN_QUEUE_SIZE > 0

uint16 QueueSize(void *Buf)
  {
    uint16 temp;
     #if (CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL)
     CPU_SR         cpu_sr;
    #endif   
    temp = 0;                                                   /* ������Ч����0 */
    if (Buf != NULL)
      {
        OS_ENTER_CRITICAL();
        temp = ((DataQueue *)Buf)->MaxData;
        OS_EXIT_CRITICAL();
      }
    return temp;
  }

#endif

/*********************************************************************************************************
** ��������: OSQFlush
** ��������: ��ն���
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_FLUSH
#define EN_QUEUE_FLUSH    0
#endif

#if EN_QUEUE_FLUSH > 0

void QueueFlush(void *Buf)
  {
    DataQueue *Queue;
    
    if (Buf != NULL)                                                /* �����Ƿ���Ч */
      {                                                             /* ��Ч         */
        Queue = (DataQueue *)Buf;
        OS_ENTER_CRITICAL();
        Queue->Out = Queue->Buf;
        Queue->In = Queue->Buf;
        Queue->NData = 0;                                           /* ������ĿΪ0 */
        OS_EXIT_CRITICAL();
      }
  }

#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
