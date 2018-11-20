#include <includes.h>
#include <Motor.h>

MotorGL  MotorX,MotorY,MotorRegent,MotorSample,MotorMotiveHook;
MotorGL  MotorUDHook,MotorSyringSample,MotorSyringReagent,MotorCollect,MotorPump,MotorRoll;

void MotorInit(void)
{
    //X,Y电机参数设置
    MotorSample.nAddr= MOTOR_Z1_ADDR;
    MotorSample.nSpeedMax=16000;
    MotorSample.nSpeedMin=2000;
    MotorSample.nSpeedStep=50;
    MotorSample.nTimeoutSteps=7;
    MotorSample.SpeedK = 6080;
    MotorSample.ISensitive = 150;
    MotorSample.HoldI = 0X40;//40
    MotorSample.LowspeedI = 0X49;//4f
    MotorSample.HighspeedI = 0x49;//4f
    MotorSample.nCtrlWord =MCF_USES_ORG | MCF_LEFT_LIMIT_AS_SENSOR | MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    MotorRegent.nAddr=MOTOR_Z2_ADDR;
    MotorRegent.nSpeedMax=16000;
    MotorRegent.nSpeedMin=2000;
    MotorRegent.nSpeedStep=50;
    MotorRegent.nTimeoutSteps=7;
    MotorRegent.SpeedK = 6080;
    MotorRegent.ISensitive = 150;
    MotorRegent.HoldI = 0X40;
    MotorRegent.LowspeedI = 0X49;
    MotorRegent.HighspeedI = 0x49;
    MotorRegent.nCtrlWord =MCF_USES_ORG | MCF_LEFT_LIMIT_AS_SENSOR | MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    MotorX.nAddr=MOTOR_X_ADDR;
    MotorX.nSpeedMax=20000;//16000//38000
    MotorX.nSpeedMin=1000; //1000
    MotorX.nSpeedStep=400; // 300//200
    MotorX.nTimeoutSteps=100;
    MotorX.SpeedK = 4080;
    MotorX.ISensitive = 40;
    MotorX.HoldI = 0X00;
    MotorX.LowspeedI = 0x4F;
    MotorX.HighspeedI = 0x54;
    /*MotorX.nSpeedMax=4500;
    MotorX.nSpeedMin=200;
    MotorX.nSpeedStep=1000;
    MotorX.nTimeoutSteps=20;*/
    MotorX.nCtrlWord=MCF_USES_ORG | MCF_RESET_AS_VALVE| MCF_USES_ENCODER;
    MotorY.nAddr=MOTOR_Y_ADDR;
    MotorY.nSpeedMax=16000;
    MotorY.nSpeedMin=1000;
    MotorY.nSpeedStep=400; // 400
    MotorY.nTimeoutSteps=100;
    MotorY.SpeedK = 4080;
    MotorY.ISensitive = 40;
    MotorY.HoldI = 0X00;
    MotorY.LowspeedI = 0x4F;
    MotorY.HighspeedI = 0x54;
    /*MotorY.nSpeedMax=5500;
    MotorY.nSpeedMin=250;
    MotorY.nSpeedStep=700;
    MotorY.nTimeoutSteps=15;*/
    MotorY.nCtrlWord=MCF_USES_ORG | MCF_RESET_AS_VALVE| MCF_USES_ENCODER;
    //杯钩电机设置
    MotorMotiveHook.nAddr= MOTOR_HOOK_ADDR;
    MotorMotiveHook.nSpeedMax=36000;//1000;
    MotorMotiveHook.nSpeedMin=1000; //400
    MotorMotiveHook.nSpeedStep=180; //180
    MotorMotiveHook.nTimeoutSteps=30;
    MotorMotiveHook.SpeedK = 3080;
    MotorMotiveHook.ISensitive = 30;
    MotorMotiveHook.HoldI = 0X00;
    MotorMotiveHook.LowspeedI = 0X4F;
    MotorMotiveHook.HighspeedI= 0x54;
    MotorMotiveHook.nCtrlWord =MCF_USES_ORG | MCF_RESET_AS_VALVE | MCF_USES_ENCODER;
    MotorUDHook.nAddr=MOTOR_HOOKUD_ADDR;
    MotorUDHook.nSpeedMax=10000;
    MotorUDHook.nSpeedMin=2000;
    MotorUDHook.nSpeedStep=80;
    MotorUDHook.nTimeoutSteps=5;
    MotorUDHook.nCtrlWord =MCF_USES_ORG | MCF_RESET_AS_VALVE | MCF_ORG;
    MotorUDHook.SpeedK = 6080;
    MotorUDHook.ISensitive = 50;
    MotorUDHook.HoldI = 0X40;
    MotorUDHook.LowspeedI = 0X48;
    MotorUDHook.HighspeedI = 0x4B;
    //样品泵电机初始化
    MotorSyringSample.nAddr=MOTOR_SYRINGE_ADDR;
    MotorSyringSample.nSpeedMax=6000;
    MotorSyringSample.nSpeedMin=800;
    MotorSyringSample.nSpeedStep=200;
    MotorSyringSample.nTimeoutSteps=6;
    MotorSyringSample.SpeedK = 6080;
    MotorSyringSample.ISensitive = 50;
    MotorSyringSample.HoldI = 0X40;
    MotorSyringSample.LowspeedI = 0X48;
    MotorSyringSample.HighspeedI = 0x4F;
    MotorSyringSample.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;
    //试剂泵电机初始化
    MotorSyringReagent.nAddr=MOTOR_DRAIN_ADDR;
    MotorSyringReagent.nSpeedMax=6000;
    MotorSyringReagent.nSpeedMin=800;
    MotorSyringReagent.nSpeedStep=200;
    MotorSyringReagent.nTimeoutSteps=6;
    MotorSyringReagent.SpeedK = 6080;
    MotorSyringReagent.ISensitive = 50;
    MotorSyringReagent.HoldI = 0X40;
    MotorSyringReagent.LowspeedI = 0X48;
    MotorSyringReagent.HighspeedI = 0x4F;
    MotorSyringReagent.nCtrlWord = MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;
        //排水泵电机
    /*MotorPump.nAddr=MOTOR_PUMP_ADDR;
    MotorPump.nSpeedMax=800;
    MotorPump.nSpeedMin=200;
    MotorPump.nSpeedStep=200;
    MotorPump.nTimeoutSteps=500;
    MotorPump.nCtrlWord = MCF_USES_ORG;*/
    MotorPump.nAddr=MOTOR_PUMP_ADDR;
    MotorPump.nSpeedMax=7000;
    MotorPump.nSpeedMin=1000;
    MotorPump.nSpeedStep=200;
    MotorPump.nTimeoutSteps=30;
    MotorPump.SpeedK = 6080;
    MotorPump.ISensitive = 50;
    MotorPump.HoldI = 0X40;
    MotorPump.LowspeedI = 0X4F;
    MotorPump.HighspeedI = 0x4F;
    MotorPump.nCtrlWord = MCF_USES_ORG;
    
    //收带电机减速比25:1的用1500 600.20:1的用1100 450. 50:1  3000 800  16:1 900 400
    MotorRoll.nAddr= MOTOR_ROLL_ADDR;
    MotorRoll.nSpeedMax=1500;          //600
    MotorRoll.nSpeedMin=600;          //300
    MotorRoll.nSpeedStep=50;          // 50
    MotorRoll.nTimeoutSteps=20;       //
    MotorRoll.SpeedK = 6080;          // 
    MotorRoll.ISensitive = 150;       //
    MotorRoll.HoldI = 0X40;           //40       
    MotorRoll.LowspeedI = 0X4C;       //4C
    MotorRoll.HighspeedI = 0x4C;      //4C
    MotorRoll.nCtrlWord =MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    /*
        MotorRoll.nAddr= MOTOR_ROLL_ADDR;
    MotorRoll.nSpeedMax=10;          //600
    MotorRoll.nSpeedMin=1;          //300
    MotorRoll.nSpeedStep=1;          // 50
    MotorRoll.nTimeoutSteps=20;       //
    MotorRoll.SpeedK = 6080;          // 
    MotorRoll.ISensitive = 150;       //
    MotorRoll.HoldI = 0X40;           //40       
    MotorRoll.LowspeedI = 0XC6;       //4C
    MotorRoll.HighspeedI = 0xC9;      //4C
    MotorRoll.nCtrlWord =MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    */
 }



    