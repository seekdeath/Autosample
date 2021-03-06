#include <includes.h>
#include <Motor.h>

u8  addCount = 0;
//u8  add2Count = 0;
//u8  maxCount  
//add
int addTotalStep = 7400;   //总体行程总步数
int addIntervelStep = 800;///暂定每个间隔步数为620步
//move1
int move1TotalStep = 3200;
int move1FirstBack = 1000;
int move1BackStep = 1000;
int move1LastStep = 200;
//move2
int move2TotalStep = 1000;
int move2BackStep = 20;
//rotate
int rotateStep = 1610;
int rotateback = 10;

//int add2TotalStep = 7400;
//int add2IntervelStep = 800;

int addStep = 0;//准备运行步数
//int add2Step = 0;//
//sub
int subTotalStep = 6030;
int subIntervelStep = 850;

////定义点变量备用
////
u8  addRunFlag = 0;
//u8  add2RunFlag = 0;
u8  move1RunFlag = 0;
u8  move2RunFlag = 0;
//u8  move3RunFlag = 0;
u8  rotateRunFlag = 0;
u8  subRunFlag = 0;

///
u8  addArriveFlag = 0;
//u8  add2ArriveFlag = 0;
u8  move1ArriveFlag = 0;
u8  move2ArriveFlag = 0;
//u8  move3ArriveFlag = 0;
u8  rotateArriveFlag = 0;
u8  subArriveFlag = 0;

///
u8  addResetFlag = 0;
//u8  add2ResetFlag = 0;
u8  move1ResetFlag = 0;
u8  move2ResetFlag = 0;
//u8  move3ResetFlag = 0;
u8  rotateResetFlag = 0;
u8  subResetFlag = 0;

u8  led1Flag = 0;
u8  led2Flag = 0;
u8  led3Flag = 0;
u8  led4Flag = 0;

u8  move1WorkFlag = 0;
u8  move2WorkFlag = 0;
//u8  move3WorkFlag = 0;
u8  rotateWorkFlag = 0;
u8  subWorkFlag = 0;

u8  move1Flag = 0;
u8  move2Flag = 0;
//u8  move3Flag = 0;
u8  ratateFlag = 0;
u8  subFlag = 0;

u8 move1Count = 0;
u8 move2Count = 0;
//u8 move3Count = 0;
//u8 move4Count = 0;
u8 subCount = 0;


//MotorGL  MotorX,MotorY,MotorRegent,MotorSample,MotorMotiveHook;
//MotorGL  MotorUDHook,MotorSyringSample,MotorSyringReagent,MotorCollect,MotorPump,MotorRoll;
//MotorGL  motorAddS1,motorAddS2,motorMoveS1,motorMoveS2,motorMoveS3,motorMoveS4,motorSubS;
MotorGL  motorAddS,motorMoveS1,motorRotateS,motorSubS,motorMoveS2;


//#define MOTOR_ADDSAMPLE1_ADDR		    'C'	        //2
//#define MOTOR_MOVESAMPLE1_ADDR	        'E'	        //4
//#define MOTOR_MOVESAMPLE2_ADDR          'F'             //5
//#define MOTOR_ROTATESAMPLE_ADDR         'H'             //7
//#define MOTOR_SUBSAMPLE_ADDR            'I'             //8	
void MotorInit(void)
{
    //样品推进电机1
    motorAddS.nAddr= MOTOR_ADDSAMPLE_ADDR;
    motorAddS.nSpeedMax=4000;
    motorAddS.nSpeedMin=600;
    motorAddS.nSpeedStep=50;
    motorAddS.nTimeoutSteps=30;
    motorAddS.SpeedK = 6080;
    motorAddS.ISensitive = 150;
    motorAddS.HoldI = 0X40;//40
    motorAddS.LowspeedI = 0X49;//4f
    motorAddS.HighspeedI = 0x4f;//4f
    motorAddS.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE; //| MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    //样品推进电机2
//    motorAddS2.nAddr=MOTOR_ADDSAMPLE2_ADDR;
//    motorAddS2.nSpeedMax=4000;
//    motorAddS2.nSpeedMin=600;
//    motorAddS2.nSpeedStep=50;
//    motorAddS2.nTimeoutSteps=30;
//    motorAddS2.SpeedK = 6080;
//    motorAddS2.ISensitive = 150;
//    motorAddS2.HoldI = 0X40;
//    motorAddS2.LowspeedI = 0X49;
//    motorAddS2.HighspeedI = 0x4f;
//    motorAddS2.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;//| MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    //横向运输1
    motorMoveS1.nAddr=MOTOR_MOVESAMPLE1_ADDR;
    motorMoveS1.nSpeedMax=4000;//16000//38000
    motorMoveS1.nSpeedMin=800; //1000
    motorMoveS1.nSpeedStep=50; // 300//200
    motorMoveS1.nTimeoutSteps=20;
    motorMoveS1.SpeedK = 4080;
    motorMoveS1.ISensitive = 40;
    motorMoveS1.HoldI = 0X00;
    motorMoveS1.LowspeedI = 0x49;
    motorMoveS1.HighspeedI = 0x49;
    motorMoveS1.nCtrlWord=MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;//| MCF_USES_ENCODER;
    //横向运输2
    motorMoveS2.nAddr=MOTOR_MOVESAMPLE2_ADDR;
    motorMoveS2.nSpeedMax=4000;
    motorMoveS2.nSpeedMin=800;
    motorMoveS2.nSpeedStep=50; // 400
    motorMoveS2.nTimeoutSteps=20;
    motorMoveS2.SpeedK = 4080;
    motorMoveS2.ISensitive = 40;
    motorMoveS2.HoldI = 0X00;
    motorMoveS2.LowspeedI = 0x49;
    motorMoveS2.HighspeedI = 0x49;
    motorMoveS2.nCtrlWord=MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;//| MCF_USES_ENCODER;
    //横向运输3
//    motorMoveS3.nAddr= MOTOR_MOVESAMPLE3_ADDR;
//    motorMoveS3.nSpeedMax=4000;//1000;
//    motorMoveS3.nSpeedMin=800; //400
//    motorMoveS3.nSpeedStep=50; //180
//    motorMoveS3.nTimeoutSteps=20;
//    motorMoveS3.SpeedK = 3080;
//    motorMoveS3.ISensitive = 30;
//    motorMoveS3.HoldI = 0X00;
//    motorMoveS3.LowspeedI = 0X49;
//    motorMoveS3.HighspeedI= 0x49;
//    motorMoveS3.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;// | MCF_USES_ENCODER;
    //横向运输4 运输到退样品
    motorRotateS.nAddr=MOTOR_ROTATESAMPLE_ADDR;
    motorRotateS.nSpeedMax=4000;
    motorRotateS.nSpeedMin=800;
    motorRotateS.nSpeedStep=50;
    motorRotateS.nTimeoutSteps=20;
    motorRotateS.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;
    motorRotateS.SpeedK = 6080;
    motorRotateS.ISensitive = 50;
    motorRotateS.HoldI = 0X40;
    motorRotateS.LowspeedI = 0X49;
    motorRotateS.HighspeedI = 0x49;
    //退样品电机
    motorSubS.nAddr=MOTOR_SUBSAMPLE_ADDR;
    motorSubS.nSpeedMax=3000;
    motorSubS.nSpeedMin=600;
    motorSubS.nSpeedStep=50;
    motorSubS.nTimeoutSteps=20;
    motorSubS.SpeedK = 6080;
    motorSubS.ISensitive = 50;
    motorSubS.HoldI = 0X40;
    motorSubS.LowspeedI = 0X49;
    motorSubS.HighspeedI = 0x49;
    motorSubS.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;
 }



////BACK
//void MotorInitback(void)
//{
//    //X,Y电机参数设置
//    MotorSample.nAddr= MOTOR_Z1_ADDR;
//    MotorSample.nSpeedMax=4000;
//    MotorSample.nSpeedMin=2000;
//    MotorSample.nSpeedStep=50;
//    MotorSample.nTimeoutSteps=7;
//    MotorSample.SpeedK = 6080;
//    MotorSample.ISensitive = 150;
//    MotorSample.HoldI = 0X40;//40
//    MotorSample.LowspeedI = 0X49;//4f
//    MotorSample.HighspeedI = 0x49;//4f
//    MotorSample.nCtrlWord =MCF_USES_ORG | MCF_LEFT_LIMIT_AS_SENSOR | MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
//    //
//    MotorRegent.nAddr=MOTOR_Z2_ADDR;
//    MotorRegent.nSpeedMax=4000;
//    MotorRegent.nSpeedMin=2000;
//    MotorRegent.nSpeedStep=50;
//    MotorRegent.nTimeoutSteps=7;
//    MotorRegent.SpeedK = 6080;
//    MotorRegent.ISensitive = 150;
//    MotorRegent.HoldI = 0X40;
//    MotorRegent.LowspeedI = 0X49;
//    MotorRegent.HighspeedI = 0x49;
//    MotorRegent.nCtrlWord =MCF_USES_ORG | MCF_LEFT_LIMIT_AS_SENSOR | MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
//    //
//    MotorX.nAddr=MOTOR_X_ADDR;
//    MotorX.nSpeedMax=4000;//16000//38000
//    MotorX.nSpeedMin=1000; //1000
//    MotorX.nSpeedStep=400; // 300//200
//    MotorX.nTimeoutSteps=100;
//    MotorX.SpeedK = 4080;
//    MotorX.ISensitive = 40;
//    MotorX.HoldI = 0X00;
//    MotorX.LowspeedI = 0x4F;
//    MotorX.HighspeedI = 0x54;
//    /*MotorX.nSpeedMax=4500;
//    MotorX.nSpeedMin=200;
//    MotorX.nSpeedStep=1000;
//    MotorX.nTimeoutSteps=20;*/
//    MotorX.nCtrlWord=MCF_USES_ORG | MCF_RESET_AS_VALVE| MCF_USES_ENCODER;
//    MotorY.nAddr=MOTOR_Y_ADDR;
//    MotorY.nSpeedMax=16000;
//    MotorY.nSpeedMin=1000;
//    MotorY.nSpeedStep=400; // 400
//    MotorY.nTimeoutSteps=100;
//    MotorY.SpeedK = 4080;
//    MotorY.ISensitive = 40;
//    MotorY.HoldI = 0X00;
//    MotorY.LowspeedI = 0x4F;
//    MotorY.HighspeedI = 0x54;
//    /*MotorY.nSpeedMax=5500;
//    MotorY.nSpeedMin=250;
//    MotorY.nSpeedStep=700;
//    MotorY.nTimeoutSteps=15;*/
//    MotorY.nCtrlWord=MCF_USES_ORG | MCF_RESET_AS_VALVE| MCF_USES_ENCODER;
//    //杯钩电机设置
//    MotorMotiveHook.nAddr= MOTOR_HOOK_ADDR;
//    MotorMotiveHook.nSpeedMax=36000;//1000;
//    MotorMotiveHook.nSpeedMin=1000; //400
//    MotorMotiveHook.nSpeedStep=180; //180
//    MotorMotiveHook.nTimeoutSteps=30;
//    MotorMotiveHook.SpeedK = 3080;
//    MotorMotiveHook.ISensitive = 30;
//    MotorMotiveHook.HoldI = 0X00;
//    MotorMotiveHook.LowspeedI = 0X4F;
//    MotorMotiveHook.HighspeedI= 0x54;
//    MotorMotiveHook.nCtrlWord =MCF_USES_ORG | MCF_RESET_AS_VALVE | MCF_USES_ENCODER;
//    MotorUDHook.nAddr=MOTOR_HOOKUD_ADDR;
//    MotorUDHook.nSpeedMax=10000;
//    MotorUDHook.nSpeedMin=2000;
//    MotorUDHook.nSpeedStep=80;
//    MotorUDHook.nTimeoutSteps=5;
//    MotorUDHook.nCtrlWord =MCF_USES_ORG | MCF_RESET_AS_VALVE | MCF_ORG;
//    MotorUDHook.SpeedK = 6080;
//    MotorUDHook.ISensitive = 50;
//    MotorUDHook.HoldI = 0X40;
//    MotorUDHook.LowspeedI = 0X48;
//    MotorUDHook.HighspeedI = 0x4B;
//    //样品泵电机初始化
//    MotorSyringSample.nAddr=MOTOR_SYRINGE_ADDR;
//    MotorSyringSample.nSpeedMax=6000;
//    MotorSyringSample.nSpeedMin=800;
//    MotorSyringSample.nSpeedStep=200;
//    MotorSyringSample.nTimeoutSteps=6;
//    MotorSyringSample.SpeedK = 6080;
//    MotorSyringSample.ISensitive = 50;
//    MotorSyringSample.HoldI = 0X40;
//    MotorSyringSample.LowspeedI = 0X48;
//    MotorSyringSample.HighspeedI = 0x4F;
//    MotorSyringSample.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;
//    //试剂泵电机初始化
//    MotorSyringReagent.nAddr=MOTOR_DRAIN_ADDR;
//    MotorSyringReagent.nSpeedMax=6000;
//    MotorSyringReagent.nSpeedMin=800;
//    MotorSyringReagent.nSpeedStep=200;
//    MotorSyringReagent.nTimeoutSteps=6;
//    MotorSyringReagent.SpeedK = 6080;
//    MotorSyringReagent.ISensitive = 50;
//    MotorSyringReagent.HoldI = 0X40;
//    MotorSyringReagent.LowspeedI = 0X48;
//    MotorSyringReagent.HighspeedI = 0x4F;
//    MotorSyringReagent.nCtrlWord = MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;
//        //排水泵电机
//    /*MotorPump.nAddr=MOTOR_PUMP_ADDR;
//    MotorPump.nSpeedMax=800;
//    MotorPump.nSpeedMin=200;
//    MotorPump.nSpeedStep=200;
//    MotorPump.nTimeoutSteps=500;
//    MotorPump.nCtrlWord = MCF_USES_ORG;*/
//    MotorPump.nAddr=MOTOR_PUMP_ADDR;
//    MotorPump.nSpeedMax=7000;
//    MotorPump.nSpeedMin=1000;
//    MotorPump.nSpeedStep=200;
//    MotorPump.nTimeoutSteps=30;
//    MotorPump.SpeedK = 6080;
//    MotorPump.ISensitive = 50;
//    MotorPump.HoldI = 0X40;
//    MotorPump.LowspeedI = 0X4F;
//    MotorPump.HighspeedI = 0x4F;
//    MotorPump.nCtrlWord = MCF_USES_ORG;
//    
//    //收带电机减速比25:1的用1500 600.20:1的用1100 450. 50:1  3000 800  16:1 900 400
//    MotorRoll.nAddr= MOTOR_ROLL_ADDR;
//    MotorRoll.nSpeedMax=1500;          //600
//    MotorRoll.nSpeedMin=600;          //300
//    MotorRoll.nSpeedStep=50;          // 50
//    MotorRoll.nTimeoutSteps=20;       //
//    MotorRoll.SpeedK = 6080;          // 
//    MotorRoll.ISensitive = 150;       //
//    MotorRoll.HoldI = 0X40;           //40       
//    MotorRoll.LowspeedI = 0X4C;       //4C
//    MotorRoll.HighspeedI = 0x4C;      //4C
//    MotorRoll.nCtrlWord =MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
//    /*
//        MotorRoll.nAddr= MOTOR_ROLL_ADDR;
//    MotorRoll.nSpeedMax=10;          //600
//    MotorRoll.nSpeedMin=1;          //300
//    MotorRoll.nSpeedStep=1;          // 50
//    MotorRoll.nTimeoutSteps=20;       //
//    MotorRoll.SpeedK = 6080;          // 
//    MotorRoll.ISensitive = 150;       //
//    MotorRoll.HoldI = 0X40;           //40       
//    MotorRoll.LowspeedI = 0XC6;       //4C
//    MotorRoll.HighspeedI = 0xC9;      //4C
//    MotorRoll.nCtrlWord =MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
//    */
// }


    