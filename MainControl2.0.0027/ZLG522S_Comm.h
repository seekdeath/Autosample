//============= 常量定义 ====================================================================================
// 通信命令类型
#define CMD_TYPE_DVC_CONTROL		        1			// 设备控制类命令
#define CMD_TYPE_TYPE_A				2			// TypeA(包括Mifare卡)类命令				
#define CMD_TYPE_ISO7816			5			// ISO7816类命令
#define CMD_TYPE_COMM_PROT			6			// 通信协议(包括ISO14443-4)类命令
#define CMD_TYPE_PLUS_CPU			7			// PLUS CPU卡(不包括Mifare卡)类命令
// 波特率设置
#define BAUD_CNT 					8
#define	BAUD_57600					256L - (Fpclk/192L)/57600L
#define	BAUD_28800					256L - (Fpclk/192L)/28800L
#define	BAUD_19200					256L - (Fpclk/192L)/19200L
#define	BAUD_9600					256L - (Fpclk/192L)/9600L

// 定义缓冲区大小
#define MAX_SER_SIZE      			64+2+6

#define ZLG522S_DELAY_20MS			2
#define ZLG522S_DELAY_500MS			50
// 通信命令类型
#define CMD_TYPE_DVC_CONTROL		        1			// 设备控制类命令
#define CMD_TYPE_TYPE_A				2			// TypeA(包括Mifare卡)类命令				
#define CMD_TYPE_ISO7816			5			// ISO7816类命令
#define CMD_TYPE_COMM_PROT			6			// 通信协议(包括ISO14443-4)类命令
#define CMD_TYPE_PLUS_CPU			7			// PLUS CPU卡(不包括Mifare卡)类命令

// 定义串行数据缓冲区各字符单元索引值
#define FRAMELEN				0			// 帧长度
#define CMDTYPE					1			// 包号
#define COMMAND					2			// 命令
#define STATUS					2			// 响应状态
#define LENGTH      				3			// 数据长度
#define DATA        				4			// 数据起始位置
#define PRTCLEN					6			// 协议字符长度

// 通信协议常量
#define STX     					0x20
#define ETX     					0x03
#define ACK						0x06
#define	ZLG522S_COMM_EXT        extern
//===========================================================================================================
ZLG522S_COMM_EXT unsigned char g_ucSerBuf[MAX_SER_SIZE];      // 串行数据缓冲
ZLG522S_COMM_EXT unsigned char g_ucSerBufIdx;	              // 串行数据缓冲区索引
ZLG522S_COMM_EXT unsigned char SEQ;			      // 包号,见通信协议
ZLG522S_COMM_EXT unsigned char g_ucIICAddr;		      // IIC通信地址
ZLG522S_COMM_EXT unsigned char g_ucBaudNum;		      // UART通信波特率编号

ZLG522S_COMM_EXT unsigned char SerDTvalid;			      // 串行数据接收有效位
ZLG522S_COMM_EXT unsigned char g_bUART;			              // 使用UART做通信总线(只有在硬件设置模式下,有效)
ZLG522S_COMM_EXT unsigned char g_bI2C;				      // 使用I2C做通信总线
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    void ChangeBaud(unsigned char ucBaudNum)
// 函数功能:    改变与模块通信的波特率
// 输入参数:    unsigned char ucBaudNum			// 波特率编号0～7		
// 输出参数:    -
// 说    明:    由于硬件限制，该函数只能使用0、1、2、4编号的波特率
//				0--9600；1--19200；2--28800；3--；4--57600
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SetBaud(unsigned char ucBaudNum);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    bit SetModeBaud(void)
// 函数功能:    设置模块的波特率
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    1--执行成功；0--执行失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char  SetModeBaud(void);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ZLG522S_CommInit(void))
// 函数功能:    ZLG522S主控通信接口初始化
// 输入参数:    -		
// 输出参数:    
// 返 回 值:    命令执行结果
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ZLG522S_CommInit(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    bit Ser_Rcv(void)
// 函数功能:    接收程序
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    TRUE--接收成功；FALSE--接收失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Ser_Rcv(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    bit Ser_Send(void)
// 函数功能:    发送程序
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    TRUE--发送成功；FALSE--发送失败
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char Ser_Send(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数原型:    unsigned char ZLG522S_Cmd(void)
// 函数功能:    ZLG522S模块命令执行函数
// 输入参数:    -		
// 输出参数:    -
// 返 回 值:    -
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ZLG522S_Cmd(void);

//===========================================================================================================

