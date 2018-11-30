#include <includes.h>
#include <Motor.h>

u8  add1Count = 0;
u8  add2Count = 0;
//u8  maxCount  

int add1TotalStep = 7400;   //�����г��ܲ���
int add1IntervelStep = 800;///�ݶ�ÿ���������Ϊ620��

int add2TotalStep = 7400;
int add2IntervelStep = 800;

int add1Step = 0;//׼�����в���
int add2Step = 0;//

int subTotalStep = 6030;
int subIntervelStep = 850;

////������������
////
u8  add1RunFlag = 0;
u8  add2RunFlag = 0;
u8  move1RunFlag = 0;
u8  move2RunFlag = 0;
u8  move3RunFlag = 0;
u8  move4RunFlag = 0;
u8  subRunFlag = 0;

///
u8  add1ArriveFlag = 0;
u8  add2ArriveFlag = 0;
u8  move1ArriveFlag = 0;
u8  move2ArriveFlag = 0;
u8  move3ArriveFlag = 0;
u8  move4ArriveFlag = 0;
u8  subArriveFlag = 0;

///
u8  add1ResetFlag = 0;
u8  add2ResetFlag = 0;
u8  move1ResetFlag = 0;
u8  move2ResetFlag = 0;
u8  move3ResetFlag = 0;
u8  move4ResetFlag = 0;
u8  subResetFlag = 0;

u8  led1Flag = 0;
u8  led2Flag = 0;
u8  led3Flag = 0;
u8  led4Flag = 0;

u8  move1WorkFlag = 0;
u8  move2WorkFlag = 0;
u8  move3WorkFlag = 0;
u8  move4WorkFlag = 0;
u8  subWorkFlag = 0;

u8  move1Flag = 0;
u8  move2Flag = 0;
u8  move3Flag = 0;
u8  move4Flag = 0;
u8  subFlag = 0;

u8 move1Count = 0;
u8 move2Count = 0;
u8 move3Count = 0;
u8 move4Count = 0;
u8 subCount = 0;


MotorGL  MotorX,MotorY,MotorRegent,MotorSample,MotorMotiveHook;
MotorGL  MotorUDHook,MotorSyringSample,MotorSyringReagent,MotorCollect,MotorPump,MotorRoll;
MotorGL  motorAddS1,motorAddS2,motorMoveS1,motorMoveS2,motorMoveS3,motorMoveS4,motorSubS;
//#define MOTOR_ADDSAMPLE1_ADDR		  'C'	        //2
//#define MOTOR_ADDSAMPLE2_ADDR		  'D'		//3
//#define MOTOR_MOVESAMPLE1_ADDR	  'E'	        //4
//#define MOTOR_MOVESAMPLE2_ADDR          'F'             //5
//#define MOTOR_MOVESAMPLE3_ADDR          'G'             //6
//#define MOTOR_MOVESAMPLE4_ADDR          'H'             //7
//#define MOTOR_SUBSAMPLE_ADDR            'I'             //8
void MotorInit(void)
{
    //��Ʒ�ƽ����1
    motorAddS1.nAddr= MOTOR_ADDSAMPLE1_ADDR;
    motorAddS1.nSpeedMax=4000;
    motorAddS1.nSpeedMin=600;
    motorAddS1.nSpeedStep=50;
    motorAddS1.nTimeoutSteps=30;
    motorAddS1.SpeedK = 6080;
    motorAddS1.ISensitive = 150;
    motorAddS1.HoldI = 0X40;//40
    motorAddS1.LowspeedI = 0X49;//4f
    motorAddS1.HighspeedI = 0x4f;//4f
    motorAddS1.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE; //| MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    //��Ʒ�ƽ����2
    motorAddS2.nAddr=MOTOR_ADDSAMPLE2_ADDR;
    motorAddS2.nSpeedMax=4000;
    motorAddS2.nSpeedMin=600;
    motorAddS2.nSpeedStep=50;
    motorAddS2.nTimeoutSteps=30;
    motorAddS2.SpeedK = 6080;
    motorAddS2.ISensitive = 150;
    motorAddS2.HoldI = 0X40;
    motorAddS2.LowspeedI = 0X49;
    motorAddS2.HighspeedI = 0x4f;
    motorAddS2.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;//| MCF_RESET_AS_VALVE ;//| MCF_LEFT_LIMIT;
    //��������1
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
    //��������2
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
    //��������3
    motorMoveS3.nAddr= MOTOR_MOVESAMPLE3_ADDR;
    motorMoveS3.nSpeedMax=4000;//1000;
    motorMoveS3.nSpeedMin=800; //400
    motorMoveS3.nSpeedStep=50; //180
    motorMoveS3.nTimeoutSteps=20;
    motorMoveS3.SpeedK = 3080;
    motorMoveS3.ISensitive = 30;
    motorMoveS3.HoldI = 0X00;
    motorMoveS3.LowspeedI = 0X49;
    motorMoveS3.HighspeedI= 0x49;
    motorMoveS3.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;// | MCF_USES_ENCODER;
    //��������4 ���䵽����Ʒ
    motorMoveS4.nAddr=MOTOR_MOVESAMPLE4_ADDR;
    motorMoveS4.nSpeedMax=4000;
    motorMoveS4.nSpeedMin=800;
    motorMoveS4.nSpeedStep=50;
    motorMoveS4.nTimeoutSteps=20;
    motorMoveS4.nCtrlWord =MCF_USES_ORG | MCF_USES_DEV | MCF_RESET_AS_VALVE;
    motorMoveS4.SpeedK = 6080;
    motorMoveS4.ISensitive = 50;
    motorMoveS4.HoldI = 0X40;
    motorMoveS4.LowspeedI = 0X49;
    motorMoveS4.HighspeedI = 0x49;
    //����Ʒ���
    motorSubS.nAddr=MOTOR_SUBSAMPLE_ADDR;
    motorSubS.nSpeedMax=4000;
    motorSubS.nSpeedMin=800;
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
//    //X,Y�����������
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
//    //�����������
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
//    //��Ʒ�õ����ʼ��
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
//    //�Լ��õ����ʼ��
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
//        //��ˮ�õ��
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
//    //�մ�������ٱ�25:1����1500 600.20:1����1100 450. 50:1  3000 800  16:1 900 400
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


    