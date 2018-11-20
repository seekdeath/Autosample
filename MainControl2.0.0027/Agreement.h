#ifndef __Receiver
#define __Receiver

extern char aRxBuff[];
extern char aTxBuff[];
extern unsigned char NRxBuff;
extern unsigned char NTxBuff;
extern unsigned char NTxd;
extern unsigned char StBao;
extern int nLastError;
//extern unsigned char cupstate;
extern unsigned char LackFlag;
void Cancel(void);
unsigned short cal_crc(unsigned char *pBuffer, unsigned char n_byte);
unsigned char AsciiToHex(char c);
unsigned char PackByte(char* p);
unsigned int  PackWord(char* p);
unsigned int  PackDword(char* p);
void ReceiverData(unsigned char sq0);
void SendCommand(const char *lpszFormat, ...);
void HandleCommand();

//#define SetLastError(e)		        nLastError=(e)				// ���ô���
//#define GetLastError()		        nLastError				// ���ش���
#define FRAME_CRC			(&(aRxBuff)[NRxBuff-5])			// ����CRC��ʼ��ַ
#define FRAME_END()			(aRxBuff)[NRxBuff-5] = '\0'		// ��'\0'����֡
#define FRAME_DATA			(&(aRxBuff)[4])		                // ����֡����ָ��
#define FRAME_CMD			(&(aRxBuff)[2])			        // ����֡�����ַ�
#define FRAME_ADDR                      ((aRxBuff)[1])                          //���ص�ַ
#define FRAME_RESET()		        {StBao=0; NRxBuff=0;}			// ���ý��ձ�־
#define FRAME_OK()			(StBao)			                // ���ؽ��հ�״̬, �����ɽ���Ϊ��0ֵ

#define GetAddress()	'A'			// ������ַ
#define GetVersion()	"2.0.0027"		// ��������汾

//��1.0.0008�޸���һ��Һλ̽��ı�־����������17�����ѯץ��״̬��
//1.0.0011 �����˱�����λʱ��������ܸ�λ������
//1.0.0012 ������XY���������ٸ�λ��
//1.0.0013 �����˱������ٸ�λ
//         �޸�����Ʒ�롢�Լ��롢�����������������ٶ�
//1.0.0014 ������Һλ��Ӧָ���BUG
//1.0.0015 XYZ�ƶ�ǰ���û���ָ�λ������
//1.0.0016 ������Һλ̽��ʧ�ܺ��Զ����㶯��
//         ȥ���˹���״̬�£����ܼ���ִ�ж������߼�
//         ������Z���ƶ�ʱ״̬��ȡ����λ��
//         ������XY�Ḵλʱ����ߺ����Ƶ��
//1.0.0017 �����˼���������Ƶ�ʣ��Ա�����Һλ��Ӧ�Ŀɿ���
//2.0.0016 ����XY�����۵������ٶȣ�����Ӧխ����ͻ�е�˶����Դ���������ͣ������
//2.0.0025 �ӳ��մ���λ���ɴ�ʱ��
//2.0.0026 �Ż��մ�
//2.0.0027 ��ˮ��ʱ�任��ɲ����������
//-------------------------------------------------------------------------------------------------
// �����������
//-------------------------------------------------------------------------------------------------
#define E_IDLE	         0x0000   //00��ϵͳ����
#define E_FILTER_RUN	 0x0001   //01��ת���˹�Ƭ��
#define E_FILTER_RESET	 0x0002   //02���˹�Ƭ��λ����
#define E_LIGHT_SOURCE	 0x0003   //03����Դ��ѹ��ȡ����
#define E_OPTICS_TEST	 0x0004   //04�����ѧ���԰�ͨ��ʧ��
#define E_MOTOR 	 0x0005   //05���������ư�ͨ��ʧ��

#endif
