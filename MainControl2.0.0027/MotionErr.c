#include <includes.h>
#include <MotionErr.h>
u8    XYZ_State=0;    //XYZ组件状态     默认空闲
u8    Hook_State=0;   //杯勾组件状态    默认空闲
u8    Syringe_State=0;   //加样泵组件状态  默认空闲
u8    Collect_State=0;//收带组件状态    默认空闲
u8    WaterPumpState=0;
u8    Drain_State=0;  //排水组件状态    默认空闲

u8 Get_XYZ_State(void) 
{
    return   XYZ_State;
}

void Set_XYZ_State(u8 State)
{
    XYZ_State=State;
}

u8 Get_Hook_State(void)
{
    return   Hook_State;
}

void Set_Hook_State(u8 State)
{
    Hook_State=State;
}

u8 Get_Syringe_State(void)
{
    return   Syringe_State;
}

void Set_Syringe_State(u8 State)
{
    Syringe_State=State;
}

u8 Get_Collect_State(void)
{
    return   Collect_State;
}

void Set_Collect_State(u8 State)
{
    Collect_State=State;
}


u8 Get_WaterPump_State(void)
{
    return   WaterPumpState;
}

void Set_WaterPump_State(u8 State)
{
    WaterPumpState=State;
}


u8 Get_Drain_State(void)
{
    return   Drain_State;
}

void Set_Drain_State(u8 State)
{
    Drain_State=State;
}