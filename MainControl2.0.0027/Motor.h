extern u8  add1Count ;
extern u8  add2Count ;

extern int add1TotalStep ;   //总体行程总步数
extern int add1IntervelStep ;///暂定每个间隔步数为200步

extern int add2TotalStep ;
extern int add2IntervelStep ;

extern int add1Step ;//准备运行步数
extern int add2Step ;//

////定义点变量备用
////
extern u8  add1RunFlag ;
extern u8  add2RunFlag ;
extern u8  move1RunFlag ;
extern u8  move2RunFlag ;
extern u8  move3RunFlag ;
extern u8  move4RunFlag ;
extern u8  subRunFlag ;

///
extern u8  add1ArriveFlag ;
extern u8  add2ArriveFlag ;
extern u8  move1ArriveFlag ;
extern u8  move2ArriveFlag ;
extern u8  move3ArriveFlag ;
extern u8  move4ArriveFlag ;
extern u8  subArriveFlag ;

///
extern u8  add1ResetFlag ;
extern u8  add2ResetFlag ;
extern u8  move1ResetFlag ;
extern u8  move2ResetFlag ;
extern u8  move3ResetFlag ;
extern u8  move4ResetFlag ;
extern u8  subResetFlag ;

extern u8  led1Flag ;
extern u8  led2Flag ;
extern u8  led3Flag ;
extern u8  led4Flag ;

extern u8  move1WorkFlag ;
extern u8  move2WorkFlag ;
extern u8  move3WorkFlag ;
extern u8  move4WorkFlag ;

extern u8  move2Flag ;
extern u8  move3Flag ;

#define MOTOR_COUNT					10				// 该板控制的电机数
#define MOTOR_ADDR_TO_INDEX(addr)	((addr)-'C')	// 将电机地址转换为索引值
#define MOTOR_INDEX_TO_ADDR(idx)	((idx) +'C')	// 将电机索引转换为地址

#define MOTOR_ADDSAMPLE1_ADDR		'C'	        //2
#define MOTOR_ADDSAMPLE2_ADDR		'D'		//3
#define MOTOR_MOVESAMPLE1_ADDR	        'E'	        //4
#define MOTOR_MOVESAMPLE2_ADDR          'F'             //5
#define MOTOR_MOVESAMPLE3_ADDR          'G'             //6
#define MOTOR_MOVESAMPLE4_ADDR          'H'             //7
#define MOTOR_SUBSAMPLE_ADDR            'I'             //8				
#define MOTOR_DRAIN_ADDR        'J'		//9		
#define MOTOR_PUMP_ADDR         'K'             //10
#define MOTOR_ROLL_ADDR         'L'             //11

#define MCF_USES_ORG			0x0001		// 使用原点
#define MCF_USES_DEV			0x0002		// 使用回程差
#define MCF_USES_LEFT_LIMIT		0x0004		// 使用左限位
#define MCF_USES_RIGHT_LIMIT		0x0008		// 使用右限位
#define MCF_USES_ENCODER		0x0010		// 使用编码器
#define MCF_ORG				0x0020		// 原点传感器值
#define MCF_LEFT_LIMIT			0x0040		// 左限位传感器值
#define MCF_RIGHT_LIMIT			0x0080		// 右限位传感器值
#define MCF_DIR_RIGHT2LEFT		0x0100		// 从右向左时方向控制输出值
#define MCF_LEFT_LIMIT_AS_SENSOR	0x0200		// 左限位用作液位传感器
#define MCF_RIGHT_LIMIT_AS_SENSOR	0x0400		// 右限位用作液位传感器
#define MCF_RESET_AS_VALVE		0x0800		// 将电机Reset信号作为阀控制端
#define MCF_SWAP_SENSOR			0x1000		// 将电机板PS1和PS3传感器位置交换

#define MAXERRSTEP                      100


// MotorRun 标志
#define MRF_STOP					0			// 电机停止
#define MRF_RIGHT					1			// 电机正转
#define MRF_LEFT					2			// 电机反转


typedef struct MGL MotorGL;
struct MGL
{
    u8    nAddr;
    u8    LastStep;
    u16   nSpeedMin;
    u16   nSpeedMax;
    u16   nSpeedStep;
    u16   nCtrlWord;
    u16   nTimeoutSteps;
    u32   Location;
    u8    HoldI;
    u8    LowspeedI;
    u8    HighspeedI;
    u16   SpeedK;
    u16   ISensitive;
};


extern MotorGL  MotorX,MotorY,MotorRegent,MotorSample,MotorMotiveHook;
extern MotorGL  MotorUDHook,MotorSyringSample,MotorSyringReagent,MotorCollect,MotorPump,MotorRoll;
extern MotorGL  motorAddS1,motorAddS2,motorMoveS1,motorMoveS2,motorMoveS3,motorMoveS4,motorSubS;
void MotorInit(void);
