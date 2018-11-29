extern u8  add1Count ;
extern u8  add2Count ;

extern int add1TotalStep ;   //�����г��ܲ���
extern int add1IntervelStep ;///�ݶ�ÿ���������Ϊ200��

extern int add2TotalStep ;
extern int add2IntervelStep ;

extern int add1Step ;//׼�����в���
extern int add2Step ;//

////������������
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

#define MOTOR_COUNT					10				// �ð���Ƶĵ����
#define MOTOR_ADDR_TO_INDEX(addr)	((addr)-'C')	// �������ַת��Ϊ����ֵ
#define MOTOR_INDEX_TO_ADDR(idx)	((idx) +'C')	// ���������ת��Ϊ��ַ

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

#define MCF_USES_ORG			0x0001		// ʹ��ԭ��
#define MCF_USES_DEV			0x0002		// ʹ�ûس̲�
#define MCF_USES_LEFT_LIMIT		0x0004		// ʹ������λ
#define MCF_USES_RIGHT_LIMIT		0x0008		// ʹ������λ
#define MCF_USES_ENCODER		0x0010		// ʹ�ñ�����
#define MCF_ORG				0x0020		// ԭ�㴫����ֵ
#define MCF_LEFT_LIMIT			0x0040		// ����λ������ֵ
#define MCF_RIGHT_LIMIT			0x0080		// ����λ������ֵ
#define MCF_DIR_RIGHT2LEFT		0x0100		// ��������ʱ����������ֵ
#define MCF_LEFT_LIMIT_AS_SENSOR	0x0200		// ����λ����Һλ������
#define MCF_RIGHT_LIMIT_AS_SENSOR	0x0400		// ����λ����Һλ������
#define MCF_RESET_AS_VALVE		0x0800		// �����Reset�ź���Ϊ�����ƶ�
#define MCF_SWAP_SENSOR			0x1000		// �������PS1��PS3������λ�ý���

#define MAXERRSTEP                      100


// MotorRun ��־
#define MRF_STOP					0			// ���ֹͣ
#define MRF_RIGHT					1			// �����ת
#define MRF_LEFT					2			// �����ת


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
