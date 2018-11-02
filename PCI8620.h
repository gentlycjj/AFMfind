#ifndef _PCI8620_DEVICE_
#define _PCI8620_DEVICE_
//#################### AD硬件参数PCI8620_PARA_AD定义 #####################
// 用于AD采样的实际硬件参数
typedef struct _PCI8620_PARA_AD     
{
	LONG ADMode;            // AD模式选择(连续/分组方式)
	LONG FirstChannel;      // 首通道[0,15]
	LONG LastChannel;       // 末通道[0,15],要求末通道必须大于或等于首通道
	LONG Frequency;         // 采集频率,单位为Hz, [31, 250000]
	LONG GroupInterval;     // 分组时的组间间隔(单位：微秒)[1, 419430]
	LONG LoopsOfGroup;	    // 组内循环次数[1, 65535]
	LONG Gains;				// 增益设置
	LONG InputRange;		// 模拟量输入量程范围
	LONG TriggerMode;       // 触发模式选择
	LONG TriggerSource;		// 触发源选择
	LONG TrigLevelVolt;		// 触发电平(0~10000mV)
	LONG TriggerType;		// 触发类型
	LONG TriggerDir;		// 触发方向选择(正向/负向触发)
	LONG ClockSource;		// 时钟源选择(内/外时钟源)
	LONG bClockOutput;      // 允许时钟输出
	LONG GroundingMode;		// 接地模式
} PCI8620_PARA_AD, *PPCI8620_PARA_AD;

//***********************************************************
// AD硬件参数PCI8620_PARA_AD中的ADMode所使用工作模式选项
const long PCI8620_ADMODE_SEQUENCE		= 0x00; // 连续采样
const long PCI8620_ADMODE_GROUP			= 0x01; // 分组采样

//***********************************************************
// AD硬件参数PCI8620_PARA_AD中的InputRange模拟量输入范围所使用的选项
const long PCI8620_INPUT_N10000_P10000mV= 0x00; // ±10000mV
const long PCI8620_INPUT_N5000_P5000mV	= 0x01; // ±5000mV
const long PCI8620_INPUT_N2500_P2500mV	= 0x02; // ±2500mV
const long PCI8620_INPUT_0_P10000mV		= 0x03; // 0～10000mV

//***********************************************************
// AD参数PCI8620_PARA_AD中的Gains使用的硬件增益选项
const long PCI8620_GAINS_1MULT			= 0x00; // 1倍增益
const long PCI8620_GAINS_2MULT			= 0x01; // 2倍增益
const long PCI8620_GAINS_4MULT			= 0x02; // 4倍增益
const long PCI8620_GAINS_8MULT			= 0x03; // 8倍增益

//***********************************************************
// AD硬件参数PCI8620_PARA_AD中的TriggerMode成员变量所使用触发模式选项
const long PCI8620_TRIGMODE_SOFT		= 0x00; // 软件触发(属于内触发)
const long PCI8620_TRIGMODE_POST		= 0x01; // 硬件触发(属于外触发)

//***********************************************************
// AD硬件参数PCI8620_PARA_AD中的TriggerSource触发源信号所使用的选项
const long PCI8620_TRIGSRC_ATR			= 0x00; // 选择外部ATR作为触发源
const long PCI8620_TRIGSRC_DTR			= 0x01; // 选择外部DTR作为触发源

// AD硬件参数PCI8620_PARA_AD中的TriggerType触发类型所使用的选项
const long PCI8620_TRIGTYPE_EDGE		= 0x00; // 边沿触发
const long PCI8620_TRIGTYPE_PULSE		= 0x01; // 电平触发

//***********************************************************
// AD硬件参数PCI8620_PARA_AD中的TriggerDir触发方向所使用的选项
const long PCI8620_TRIGDIR_NEGATIVE		= 0x00; // 负向触发(低电平/下降沿触发)
const long PCI8620_TRIGDIR_POSITIVE		= 0x01; // 正向触发(高电平/上升沿触发)
const long PCI8620_TRIGDIR_POSIT_NEGAT	= 0x02; // 正负向触发(高/低电平或上升/下降沿触发)

//***********************************************************
// AD硬件参数PCI8620_PARA_AD中的ClockSource时钟源所使用的选项
const long PCI8620_CLOCKSRC_IN			= 0x00; // 内部时钟
const long PCI8620_CLOCKSRC_OUT			= 0x01; // 外部时钟

// AD硬件参数PCI8620_PARA_AD中的bClockOutput成员变量所使用内部和外部时钟源选项
const long PCI8620_CLOCKOUT_DISABLE		= 0x00; // 禁止本卡上的自带时钟向外输出
const long PCI8620_CLOCKOUT_ENABLE		= 0x01; // 允许本卡上的自带时钟向外输出

//***********************************************************
// AD参数(PCI8620_PARA_AD)中的GroundingMode使用的模拟信号接地方式选项
const long PCI8620_GNDMODE_SE			= 0x00;	// 单端方式(SE:Single end)
const long PCI8620_GNDMODE_DI			= 0x01;	// 双端方式(DI:Differential)

//*************************************************************************************
// 用于AD采样的实际硬件参数
typedef struct _PCI8620_STATUS_AD     
{
	LONG bNotEmpty;			// 板载FIFO存储器的非空标志，=TRUE非空， = FALSE 空
	LONG bHalf;				// 板载FIFO存储器的半满标志，=TRUE半满以上， = FALSE 半满以下
	LONG bOverflow;			// 板载FIFO存储器的溢出标志，= TRUE已发生溢出， = FALSE　未发生溢出
} PCI8620_STATUS_AD, *PPCI8620_STATUS_AD;

const int PCI8620_MAX_SEGMENT_COUNT = 64;
typedef struct _PCI8620_STATUS_DMA     
{
	LONG iCurSegmentID;								// 当前段缓冲ID,表示DMA正在传输的缓冲区段
	LONG bSegmentSts[PCI8620_MAX_SEGMENT_COUNT];	// 各个缓冲区的新旧状态,=1表示该相应缓冲区数据为新,否则为旧
	LONG bBufferOverflow;							// 返回溢出状态
} PCI8620_STATUS_DMA, *PPCI8620_STATUS_DMA;

//***********************************************************
// DA输出函数WriteDeviceDA的模拟量输出范围参数OutputRange所使用的选项
const long PCI8620_OUTPUT_0_P5000mV			= 0x00;		// 0～5000mV
const long PCI8620_OUTPUT_0_P10000mV		= 0x01;		// 0～10000mV
const long PCI8620_OUTPUT_0_P10800mV		= 0x02;		// 0～10800mV
const long PCI8620_OUTPUT_N5000_P5000mV		= 0x03;		// ±5000mV
const long PCI8620_OUTPUT_N10000_P10000mV	= 0x04;		// ±10000mV
const long PCI8620_OUTPUT_N10800_P10800mV	= 0x05;		// ±10800mV

//***********************************************************
// 用于计数器CNT的参数
// 硬件参数ControlMode控制字模式选项
const long PCI8620_GATEMODE_POSITIVE_0	= 0x00;		// COUNTER:GATE高电平时计数，低电平时停止计数，计数时重新写入初值，按初值计数
													// 计数结束产生中断：写入初值开始计数时OUT开始为0，当计数到0时OUT为1

const long PCI8620_GATEMODE_RISING_1	= 0x01;		// COUNTER:GATE上边沿触发计数，计数中出现GATE上升沿重新装入初值计数
													// 可编程单拍脉冲：当写入初值时OUT为1，当开始计数时OUT为0，当计数到0时OUT再次为1

const long PCI8620_GATEMODE_POSITIVE_2	= 0x02;		// COUNTER:GATE高电平时计数，低电平时停止计数，计数时重新写入初值，按初值计数
													// 频率发生器：计数期间OUT为1，计数到0后输出一个周期的0，并重新装入计数值计数

const long PCI8620_GATEMODE_POSITIVE_3	= 0x03;		// COUNTER:GATE高电平时计数，低电平时停止计数，计数时重新写入初值，按初值计数
													// 方波发生器：计数期间OUT为1，计数到0后输出一个周期的0，并重新装入计数值计数

const long PCI8620_GATEMODE_POSITIVE_4	= 0x04;		// COUNTER:GATE高电平时计数，低电平时停止计数，计数时重新写入初值，按初值计数
													// 软件触发选通：写入初值OUT为1， 计数结束OUT输出一个周期低电平信号

const long PCI8620_GATEMODE_RISING_5	= 0x05;		// COUNTER:GATE上边沿触发计数，计数中出现GATE上升沿重新装入初值计数
													// 硬件触发选通：写入初值OUT为1， 计数结束OUT输出一个周期低电平信号

//***********************************************************
// CreateFileObject所用的文件操作方式控制字(可通过或指令实现多种方式并操作)
const long PCI8620_modeRead					= 0x0000;	// 只读文件方式
const long PCI8620_modeWrite				= 0x0001;	// 只写文件方式
const long PCI8620_modeReadWrite			= 0x0002;	// 既读又写文件方式
const long PCI8620_modeCreate				= 0x1000;	// 如果文件不存可以创建该文件，如果存在，则重建此文件，并清0
const long PCI8620_typeText					= 0x4000;	// 以文本方式操作文件

//***********************************************************
// 用户函数接口
#ifndef _PCI8620_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## 设备对象管理函数 ##############################
	HANDLE DEVAPI FAR PASCAL PCI8620_CreateDevice(int DeviceLgcID = 0);     // 用逻辑号创建设备对象
	int DEVAPI FAR PASCAL PCI8620_GetDeviceCount(HANDLE hDevice); // 取得设备总台数
	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID);
	BOOL DEVAPI FAR PASCAL PCI8620_ListDeviceDlg(HANDLE hDevice); // 以对话框窗体方式列表系统当中的所有的该PCI设备
    BOOL DEVAPI FAR PASCAL PCI8620_ReleaseDevice(HANDLE hDevice); // 仅释放设备对象

	//####################### AD数据读取函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
	// AD数据两个函数便能轻松高效地实现高速、连续的数据采集

	// AD程序查询方式函数
	BOOL DEVAPI FAR PASCAL PCI8620_InitDeviceProAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									HANDLE hDevice,			// 设备对象,它由CreateDevice函数创建
									PPCI8620_PARA_AD pADPara); // 硬件参数, 它仅在此函数中决定硬件状态

    BOOL DEVAPI FAR PASCAL PCI8620_StartDeviceProAD(		// 在初始化之后，启动设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8620_ReadDeviceProAD_Npt(		// 当AD标志有效时，用此函数读取设备上的AD数据(程序非空方式)
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									ULONG ADBuffer[],		// 接受原始AD数据的用户缓冲区
									LONG nReadSizeWords,	// 相对于偏位点后读入的数据长度(字)
									PLONG nRetSizeWords);   // 返回实际读取的长度(字)

	BOOL DEVAPI FAR PASCAL PCI8620_GetDevStatusProAD(		// 在AD采样过程中取得设备的各种状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									PPCI8620_STATUS_AD pADStatus); // AD的各种信息结构体

	BOOL DEVAPI FAR PASCAL PCI8620_ReadDeviceProAD_Half(	// 当AD标志有效时，用此函数读取设备上的AD数据(程序半满方式)
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									ULONG ADBuffer[],		// 接受原始AD数据的用户缓冲区
									LONG nReadSizeWords,	// 相对于偏位点后读入的数据长度(字)
									PLONG nRetSizeWords);	// 返回实际读取的长度(字)
		
    BOOL DEVAPI FAR PASCAL PCI8620_StopDeviceProAD(			// 在启动设备之后，暂停设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8620_ReleaseDeviceProAD(		// 关闭AD设备,禁止传输,且释放资源
									HANDLE hDevice);		// 设备句柄,它由CreateDevice函数创建

	//##################### AD直接内存(DMA)方式函数(上层用户函数) ###########################
	BOOL DEVAPI FAR PASCAL PCI8620_InitDeviceDmaAD(			// 初始化设备，当返回TRUE后,设备即准备就绪.
									HANDLE hDevice,			// 设备对象,它由CreateDevice函数创建
									HANDLE hDmaEvent,		// DMA事件句柄,它由CreateSystemEvent创建
									ULONG ADBuffer[],		// 用户缓冲区,最好为两维数组
									ULONG nReadSizeWords,	// 每次DMA时,用户从指定缓冲应读取的实际长度(要小于或等于nSegmentSizeWords)
									LONG nSegmentCount,		// 缓冲分段的数量,取值范围为2-128
									ULONG nSegmentSizeWords,// 缓冲区分段的段长(必须等于FIFO半满长度)
									PPCI8620_PARA_AD pADPara);	// 硬件参数, 它仅在此函数中决定硬件状态

	BOOL DEVAPI FAR PASCAL PCI8620_StartDeviceDmaAD(		// 在初始化之后，启动设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8620_GetDevStatusDmaAD(		// 在AD采样过程中取得DMA的有关状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									PPCI8620_STATUS_DMA pDMAStatus); // 获得的DMA工作状态

	BOOL DEVAPI FAR PASCAL PCI8620_SetDevStatusDmaAD(		// 在AD采样过程中设置DMA的有关状态,返回值表示函数是否成功
									HANDLE hDevice,			// 设备句柄,它由CreateDevice函数创建
									LONG iClrBufferID);		// 要清除的缓冲区ID,将其置为0

	BOOL DEVAPI FAR PASCAL PCI8620_StopDeviceDmaAD(			// 在启动设备之后，暂停设备
									HANDLE hDevice);		// 设备对象句柄,它由CreateDevice函数创建

	BOOL DEVAPI FAR PASCAL PCI8620_ReleaseDeviceDmaAD(		// 关闭AD设备,禁止传输,且释放资源
									HANDLE hDevice);		// 设备句柄,它由CreateDevice函数创建
	
	//##################### AD的硬件参数操作函数 ###########################
	BOOL DEVAPI FAR PASCAL PCI8620_SaveParaAD(HANDLE hDevice, PPCI8620_PARA_AD pADPara); // 将当前的AD采样参数保存至系统中
    BOOL DEVAPI FAR PASCAL PCI8620_LoadParaAD(HANDLE hDevice, PPCI8620_PARA_AD pADPara); // 将AD采样参数从系统中读出
    BOOL DEVAPI FAR PASCAL PCI8620_ResetParaAD(HANDLE hDevice, PPCI8620_PARA_AD pADPara); // 将AD采样参数恢复至出厂默认值

	//####################### DA数据输出函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面其中几个函数便能轻
	// 松实现DA数据输出
	BOOL DEVAPI FAR PASCAL PCI8620_WriteDeviceDA(			// 写DA数据
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									LONG OutputRange,		// 输出量程，具体定义请参考上面的常量定义部分
									SHORT nDAData,			// 输出的DA原始数据[0, 4095]
									int nDAChannel);		// DA输出通道[0-3]

	//####################### 数字I/O输入输出函数 #################################
	// 用户可以使用WriteRegisterULong和ReadRegisterULong等函数直接控制寄存器进行I/O
	// 输入输出，但使用下面两个函数更省事，它不需要您关心寄存器分配和位操作等，而只
	// 需象VB等语言的属性操作那么简单地实现各开关量通道的控制。
    BOOL DEVAPI FAR PASCAL PCI8620_SetDeviceDO(				// 输出开关量状态
										HANDLE hDevice,		// 设备句柄								        
										BYTE bDOSts[16]);	// 开关状态

	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceDI(				// 取得开关量状态     
										HANDLE hDevice,		// 设备句柄								        
										BYTE bDISts[16]);	// 开关状态	
	
	//##################### 计数器控制函数 ##########################
    BOOL DEVAPI FAR PASCAL PCI8620_SetDeviceCNT(			// 初始化计数器
									HANDLE	hDevice,		// 设备句柄
									ULONG	ContrlMode,		// 计数器控制模式
									ULONG	CNTVal,			// 计数初值
									ULONG	ulChannel);		// 通道选择[0-2]			

	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceCNT(			// 取得各路计数器的当前计数值
									HANDLE	hDevice,		// 设备对象句柄,它由CreateDevice函数创建
									PULONG	pCNTVal,		// 返回计数值
									ULONG	ulChannel);		// 通道选择[0-2]

	//################# 内存映射寄存器直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	BOOL DEVAPI  FAR PASCAL PCI8620_GetDeviceAddr( // 取得指定的指定设备ID号的映射寄存器的线性基地址，返回设备总数
												HANDLE hDevice,  // 设备对象句柄
												PUCHAR LinearAddr, // 返回指定映射寄存器的线性地址
											    	PUCHAR PhysAddr,   //  返回指定映射寄存器的物理地址
												int RegisterID = 0); // 设备映射寄存器的ID号（0-5）
	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceBar(			// 取得指定的指定设备寄存器组BAR地址
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									PUCHAR  pbPCIBar[6]);	// 返回PCI BAR所有地址,具体PCI BAR中有多少可用地址请看硬件说明书

	BOOL DEVAPI FAR PASCAL PCI8620_GetDevVersion(			// 获取设备固件及程序版本
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									PULONG pulFmwVersion,	// 固件版本
									PULONG pulDriverVersion);// 驱动版本

    BOOL DEVAPI FAR PASCAL PCI8620_WriteRegisterByte(		// 往指定寄存器空间位置写入单节字数据
									HANDLE hDevice,			// 设备对象句柄,它由CreateDevice函数创建
									PUCHAR pbLinearAddr,	// 指定寄存器的线性基地址,它等于GetDeviceAddr中的LinearAddr参数返回值
									ULONG OffsetBytes,		// 相对于线性基地址基地址的偏移位置(字节)
									BYTE Value);			// 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）

	BOOL DEVAPI FAR PASCAL PCI8620_WriteRegisterWord(		// 写双字节数据（其余同上）
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes,  
									WORD Value);

	BOOL DEVAPI FAR PASCAL PCI8620_WriteRegisterULong(		// 写四节字数据（其余同上）
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes,  
									ULONG Value);

	BYTE DEVAPI FAR PASCAL PCI8620_ReadRegisterByte(		// 读入单字节数据（其余同上）
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes);

	WORD DEVAPI FAR PASCAL PCI8620_ReadRegisterWord(		// 读入双字节数据（其余同上）
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes);

	ULONG DEVAPI FAR PASCAL PCI8620_ReadRegisterULong(		// 读入四字节数据（其余同上）
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes);

	//################# I/O端口直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	// 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
    BOOL DEVAPI FAR PASCAL PCI8620_WritePortByte(			// 8位写端口
									HANDLE hDevice,			// 设备句柄(由CreateDevice创建)
									PUCHAR pbPort,			// 指定寄存器的物理基地址
									ULONG OffsetBytes,		// 相对于物理基地址的偏移位置(字节)
									BYTE Value);			// 写出的8位整型数据
    BOOL DEVAPI FAR PASCAL PCI8620_WritePortWord(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes, WORD Value);
    BOOL DEVAPI FAR PASCAL PCI8620_WritePortULong(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCI8620_ReadPortByte(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes);
    WORD DEVAPI FAR PASCAL PCI8620_ReadPortWord(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes);
    ULONG DEVAPI FAR PASCAL PCI8620_ReadPortULong(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes);
	// 如果您要在用户模式下，直接访问硬件端口，请安装并使用ISA\CommUser下的驱动ReadPortByteEx等函数
	//########################### 线程操作函数 ######################################
	HANDLE DEVAPI FAR PASCAL PCI8620_CreateSystemEvent(void); // 创建内核事件对象，供InitDeviceDmaAD和VB子线程等函数使用
	BOOL DEVAPI FAR PASCAL PCI8620_ReleaseSystemEvent(HANDLE hEvent); // 释放内核事件对象

#ifdef __cplusplus
}
#endif

#ifndef _FIFO_LENGTH_
#define _FIFO_LENGTH_
//***********************************************************
// 各种FIFO存储器的长度(点)
const long FIFO_IDT7202_LENGTH			= 1024;
const long FIFO_IDT7203_LENGTH			= 2048;
const long FIFO_IDT7204_LENGTH			= 4096;
const long FIFO_IDT7205_LENGTH			= 8192;
const long FIFO_IDT7206_LENGTH			= 16384;
const long FIFO_IDT7207_LENGTH			= 32768;

#endif // _FIFO_LENGTH_

// 自动包含驱动函数导入库
#ifndef _PCI8620_DRIVER_
#ifndef _WIN64
#pragma comment(lib, "PCI8620_32.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with PCI8620_32.dll...")
#pragma message("======== Successfully linked with PCI8620_32.dll")
#else
#pragma comment(lib, "PCI8620_64.lib")
#pragma message("======== Welcome to use our art company's products!")
#pragma message("======== Automatically linking with PCI8620_64.dll...")
#pragma message("======== Successfully linked with PCI8620_64.dll")
#endif
#endif // _PCI8620_DEVICE_

#endif // _PCI8620_DEVICE_