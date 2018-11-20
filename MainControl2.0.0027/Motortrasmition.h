#define FRAME_SIZE		50
extern char aRxBuff1[];
extern char aTxBuff1[];
extern unsigned char NRxBuff1;
extern unsigned char NTxBuff1;
extern unsigned char StBao1;
#define FRAME_CRC1				(&(aRxBuff1)[NRxBuff1-5])			// ����CRC��ʼ��ַ
#define FRAME_END1(buf, len)	(buf)[(len)-5] = '\0'				// �����ݺ�, ��'\0'����ָ��֡
#define FRAME_DATA1		(&aRxBuff1[3])							// ����ָ��֡����ָ��
#define FRAME_DATA_LEN1			(NRxBuff1-8)						// ����֡���ݳ���
#define FRAME_CMD1			((aRxBuff1)[2])						// ����֡�����ַ�
#define FRAME_RESET1()			ResetFrame1()						// ���ý��ձ�־
#define FRAME_OK1()			IsFrame1Ok()						// ���ؽ��հ�״̬, �����ɽ���Ϊ��0ֵ

#define WAIT_BACK_TIMEOUTS	Ms2Tick(100)							// �ȴ���Ӧ��ʱ��
#define TRY(expression)		{unsigned char __i; for(__i=0; __i<10; __i++){if((expression)!=0)break;}}
void ReceiverData1(unsigned char sq0);
char SendCommand1(char* pReceiveBuf, const char *lpszFormat, ...);
char wait_back(char nAddr);

char MotorReset(char nAddr);
char MotorGetStatus(char nAddr, u16* pStatus);
char MotorCancel(char nAddr);
char MotorUnlock(char nAddr, char bUnlock);
char MotorGetLastStep(char nAddr, s32* pStep);
char MotorSetSpeed(char nAddr, u16 nStart, u16 nEnd, u8 nStep);
char MotorSetHighSpeed(char nAddr, u16 nStart, u16 nEnd, u16 nStep);
char MotorRun(char nAddr, char bRun);
char MotorMoveTo(char nAddr, s32 nPos);
char MotorGetPos(char nAddr, s32* pPos);
char MotorShake(char nAddr, s16 nAmp, u16 nFreq);
char MotorGetNegPos(char nAddr, s32* pPos);
char MotorGetSensor(char nAddr, u16* pSensor);
char MotorTestLevel(char nAddr, s32 nOffset, s32 nMaxPos);
char MotorSetValve(char nAddr, u8 bLock);
char MotorSetConfig(char nAddr, u16 nConfig, u16 nTimeoutStep);
char MotorSetSpeedLevel(char nAddr, u8 low , u8 high , u8 Hold);
char MotorSetSpeedK(char nAddr, u16 SpeedK,u16 ISensitive);

  