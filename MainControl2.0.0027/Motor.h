

#define MOTOR_COUNT					10				// �ð���Ƶĵ����
#define MOTOR_ADDR_TO_INDEX(addr)	((addr)-'C')	// �������ַת��Ϊ����ֵ
#define MOTOR_INDEX_TO_ADDR(idx)	((idx) +'C')	// ���������ת��Ϊ��ַ

#define MOTOR_Z1_ADDR		'C'	        //2
#define MOTOR_Z2_ADDR		'D'		//3
#define MOTOR_X_ADDR	        'E'	        //4
#define MOTOR_Y_ADDR            'F'             //5
#define MOTOR_HOOK_ADDR         'G'             //6
#define MOTOR_HOOKUD_ADDR       'H'             //7
#define MOTOR_SYRINGE_ADDR      'I'             //8				
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
void MotorInit(void);