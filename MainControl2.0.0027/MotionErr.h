#include <stm32f10x_type.h>
#define   XYZ_Idle                        0x00    //XYZ�������
#define   XYZ_Busy                        0x01    //XYZ���æ
#define   XYZ_Err_X_Transmition           0x80    //XYZ���X����ͨ�Ŵ���
#define   XYZ_Err_X_Origin                0x81    //XYZ���X��ԭ�㴫����ʧЧ
#define   XYZ_Err_Y_Transmition           0x82    //XYZ���Y����ͨ�Ŵ���
#define   XYZ_Err_Y_Origin                0x83    //XYZ���Y��ԭ�㴫����ʧЧ
#define   XYZ_Err_Z1_Transmition          0x84    //XYZ���Z1����ͨ�Ŵ���
#define   XYZ_Err_Z1_Origin               0x85    //XYZ���Z1��ԭ�㴫����ʧЧ
#define   XYZ_Err_Z2_Transmition          0x86    //XYZ���Z2����ͨ�Ŵ���
#define   XYZ_Err_Z2_Origin               0x87    //XYZ���Z2��ԭ�㴫����ʧЧ
#define   XYZ_Err_Sample_LiquidLevel      0x88    //��Ʒ��Һλ����ʧ��
#define   XYZ_Err_Regent_LiquidLevel      0x89    //�Լ���Һλ����ʧ��


#define   Hook_Idle                       0x00    //�������
#define   Hook_Busy                       0x01    //���æ����ʱ��Ӧ����������Ϳ���ָ��
#define   Hook_Err_Motor_Transmition      0x80    //�����ƶ����ͨ�Ŵ���
#define   Hook_Err_Motor_Origin           0x81    //�����ƶ����ԭ�㴫����ʧЧ
#define   Hook_Err_UDMotor_Transmition    0x82    //�����������ͨ�Ŵ���
#define   Hook_Err_UDMotor_Origin         0x83    //�����������ԭ�㴫����ʧЧ
#define   Hook_Err_OCMagnetic             0x84    //�������ϵ����������ʧЧ
#define   Roll_Err_Transmition            0x85

#define   Syringe_Idle                    0x00    //�������
#define   Syringe_Busy                    0x01    //���æ����ʱ��Ӧ����������Ϳ���ָ��
#define   Syringe_Err_Transmition_S       0x80    //�����õ��ͨ�Ŵ���
#define   Syringe_Err_Origin_S            0x81    //������ԭ�㴫����ʧЧ
#define   Syringe_Err_Transmition_R       0x82    //�����õ��ͨ�Ŵ���
#define   Syringe_Err_Origin_R            0x83    //������ԭ�㴫����ʧЧ

#define   Collect_Idle                    0x00    //�������
#define   Collect_Busy                    0x01    //���æ����ʱ��Ӧ����������Ϳ���ָ��
#define   Collect_Err_Transmition         0x80    //�մ����ͨ�Ŵ���
#define   Collect_Err_Micromotion         0x81    //�մ�΢��������ʧЧ

#define   Drain_Idle                      0x00    //�������
#define   Drain_Busy                      0x01    //���æ����ʱ��Ӧ����������Ϳ���ָ��
#define   Drain_Err_Transmition           0x80    //��ˮ���ͨ�Ŵ���

u8 Get_XYZ_State(void);
void Set_XYZ_State(u8 State);
u8 Get_Hook_State(void);
void Set_Hook_State(u8 State);
u8 Get_Syringe_State(void);
void Set_Syringe_State(u8 State);
u8 Get_Collect_State(void);
void Set_Collect_State(u8 State);
u8 Get_WaterPump_State(void);
void Set_WaterPump_State(u8 State);
u8 Get_Drain_State(void);
void Set_Drain_State(u8 State);


