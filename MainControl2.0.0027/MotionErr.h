#include <stm32f10x_type.h>
#define   XYZ_Idle                        0x00    //XYZ组件空闲
#define   XYZ_Busy                        0x01    //XYZ组件忙
#define   XYZ_Err_X_Transmition           0x80    //XYZ组件X轴电机通信错误
#define   XYZ_Err_X_Origin                0x81    //XYZ组件X轴原点传感器失效
#define   XYZ_Err_Y_Transmition           0x82    //XYZ组件Y轴电机通信错误
#define   XYZ_Err_Y_Origin                0x83    //XYZ组件Y轴原点传感器失效
#define   XYZ_Err_Z1_Transmition          0x84    //XYZ组件Z1轴电机通信错误
#define   XYZ_Err_Z1_Origin               0x85    //XYZ组件Z1轴原点传感器失效
#define   XYZ_Err_Z2_Transmition          0x86    //XYZ组件Z2轴电机通信错误
#define   XYZ_Err_Z2_Origin               0x87    //XYZ组件Z2轴原点传感器失效
#define   XYZ_Err_Sample_LiquidLevel      0x88    //样品针液位传感失败
#define   XYZ_Err_Regent_LiquidLevel      0x89    //试剂针液位传感失败


#define   Hook_Idle                       0x00    //组件空闲
#define   Hook_Busy                       0x01    //组件忙，此时不应再向组件发送控制指令
#define   Hook_Err_Motor_Transmition      0x80    //杯勾移动电机通信错误
#define   Hook_Err_Motor_Origin           0x81    //杯勾移动电机原点传感器失效
#define   Hook_Err_UDMotor_Transmition    0x82    //杯勾升降电机通信错误
#define   Hook_Err_UDMotor_Origin         0x83    //杯勾升降电机原点传感器失效
#define   Hook_Err_OCMagnetic             0x84    //杯勾开合电磁铁传感器失效
#define   Roll_Err_Transmition            0x85

#define   Syringe_Idle                    0x00    //组件空闲
#define   Syringe_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Syringe_Err_Transmition_S       0x80    //加样泵电机通信错误
#define   Syringe_Err_Origin_S            0x81    //加样泵原点传感器失效
#define   Syringe_Err_Transmition_R       0x82    //加样泵电机通信错误
#define   Syringe_Err_Origin_R            0x83    //加样泵原点传感器失效

#define   Collect_Idle                    0x00    //组件空闲
#define   Collect_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Collect_Err_Transmition         0x80    //收带电机通信错误
#define   Collect_Err_Micromotion         0x81    //收带微动传感器失效

#define   Drain_Idle                      0x00    //组件空闲
#define   Drain_Busy                      0x01    //组件忙，此时不应再向组件发送控制指令
#define   Drain_Err_Transmition           0x80    //排水电机通信错误


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
////////////////////////////////////////////////////

#define   Add1_Idle                    0x00    //组件空闲
#define   Add1_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Add1_Err_Transmition         0x80    //加样泵电机通信错误
#define   Add1_Err_Origin            0x81    //加样泵原点传感器失效

#define   Add2_Idle                    0x00    //组件空闲
#define   Add2_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Add2_Err_Transmition         0x80    //加样泵电机通信错误
#define   Add2_Err_Origin            0x81    //加样泵原点传感器失效

#define   Move1_Idle                    0x00    //组件空闲
#define   Move1_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Move1_Err_Transmition         0x80    //加样泵电机通信错误
#define   Move1_Err_Origin            0x81    //加样泵原点传感器失效

#define   Move2_Idle                    0x00    //组件空闲
#define   Move2_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Move2_Err_Transmition         0x80    //加样泵电机通信错误
#define   Move3_Err_Origin            0x81    //加样泵原点传感器失效

#define   Move3_Idle                    0x00    //组件空闲
#define   Move3_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Move3_Err_Transmition         0x80    //加样泵电机通信错误
#define   Move3_Err_Origin            0x81    //加样泵原点传感器失效

#define   Move4_Idle                    0x00    //组件空闲
#define   Move4_Busy                    0x01    //组件忙，此时不应再向组件发送控制指令
#define   Move4_Err_Transmition         0x80    //加样泵电机通信错误
#define   Move4_Err_Origin            0x81    //加样泵原点传感器失效


u8 Get_Add1_State(void);
void Set_Add1_State(u8 State);
u8 Get_Add2_State(void);
void Set_Add2_State(u8 State);
u8 Get_Move1_State(void);
void Set_Move1_State(u8 State);
u8 Get_Move2_State(void);
void Set_Move2_State(u8 State);
u8 Get_Move3_State(void);
void Set_Move3_State(u8 State);
u8 Get_Move4_State(void);
void Set_Move4_State(u8 State);
u8 Get_Sub_State(void);
void Set_Sub_State(u8 State);
