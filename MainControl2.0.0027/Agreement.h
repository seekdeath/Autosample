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

//#define SetLastError(e)		        nLastError=(e)				// 设置错误
//#define GetLastError()		        nLastError				// 返回错误
#define FRAME_CRC			(&(aRxBuff)[NRxBuff-5])			// 返回CRC起始地址
#define FRAME_END()			(aRxBuff)[NRxBuff-5] = '\0'		// 以'\0'结束帧
#define FRAME_DATA			(&(aRxBuff)[4])		                // 返回帧数据指针
#define FRAME_CMD			(&(aRxBuff)[2])			        // 返回帧命令字符
#define FRAME_ADDR                      ((aRxBuff)[1])                          //返回地址
#define FRAME_RESET()		        {StBao=0; NRxBuff=0;}			// 重置接收标志
#define FRAME_OK()			(StBao)			                // 返回接收包状态, 如果完成接收为非0值

#define GetAddress()	'A'			// 仪器地址
#define GetVersion()	"2.0.0027"		// 仪器软件版本

//比1.0.0008修改了一个液位探测的标志错误，增加了17命令查询抓杯状态。
//1.0.0011 修正了杯钩复位时电磁铁不能复位的问题
//1.0.0012 增加了XY加样臂慢速复位；
//1.0.0013 增加了杯钩慢速复位
//         修改了样品针、试剂针、杯钩升降电机的最大速度
//1.0.0014 修正了液位感应指令的BUG
//1.0.0015 XYZ移动前采用回零指令复位加样针
//1.0.0016 增加了液位探测失败后自动回零动作
//         去掉了故障状态下，不能继续执行动作的逻辑
//         调整了Z轴移动时状态读取语句的位置
//         调整了XY轴复位时的最高和最低频率
//1.0.0017 调整了加样针的最大频率，以便增加液位感应的可靠性
//2.0.0016 降低XY加样臂的运行速度，以适应窄版机型机械运动惯性大所产生的停顿现象。
//2.0.0025 延长收带到位后松带时间
//2.0.0026 优化收带
//2.0.0027 排水泵时间换算成步数给电机板
//-------------------------------------------------------------------------------------------------
// 公共错误代码
//-------------------------------------------------------------------------------------------------
#define E_IDLE	         0x0000   //00－系统空闲
#define E_FILTER_RUN	 0x0001   //01－转动滤光片中
#define E_FILTER_RESET	 0x0002   //02－滤光片复位错误
#define E_LIGHT_SOURCE	 0x0003   //03－光源电压读取错误
#define E_OPTICS_TEST	 0x0004   //04－与光学测试板通信失败
#define E_MOTOR 	 0x0005   //05－与电机控制板通信失败

#endif
