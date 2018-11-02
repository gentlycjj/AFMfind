// 用于AD采集的参数结构
#ifndef _PCI2003_PARA_AD
typedef struct _PCI2003_PARA_AD      // 用于AD转换的硬件参数
{
	DWORD FirstChannel;       // 首通道
	DWORD LastChannel;        // 末通道
	DWORD Frequency;          // 采集频率(Hz)
	DWORD Gains;              // 采集增益
	DWORD TriggerSource;      // 触发源选择
}PCI2003_PARA_AD,*PPCI2003_PARA_AD;
#endif

//***********************************************************
// AD转换触发方式控制字
#define PCI2003_OUT_TRIGGER 0x0032    // 外触发方式
#define PCI2003_IN_TRIGGER  0x0012    // 内触发方式

//***********************************************************
// 用于开关量的参数结构
#ifndef _PCI2003_PARA_DO
typedef struct _PCI2003_PARA_DO      // 数字量输出参数
{
	BYTE DO0;       // 0通道
	BYTE DO1;       // 1通道
	BYTE DO2;       // 2通道
	BYTE DO3;       // 3通道
	BYTE DO4;       // 4通道
	BYTE DO5;       // 5通道
	BYTE DO6;       // 6通道
	BYTE DO7;       // 7通道
	BYTE DO8;       // 8通道
	BYTE DO9;       // 9通道
	BYTE DO10;       // 10通道
	BYTE DO11;       // 11通道
	BYTE DO12;       // 12通道
	BYTE DO13;       // 13通道
	BYTE DO14;       // 14通道
	BYTE DO15;       // 15通道
} PCI2003_PARA_DO,*PPCI2003_PARA_DO;
#endif

#ifndef _PCI2003_PARA_DI
typedef struct _PCI2003_PARA_DI      // 数字量输入参数
{
	BYTE DI0;       // 0通道
	BYTE DI1;       // 1通道
	BYTE DI2;       // 2通道
	BYTE DI3;       // 3通道
	BYTE DI4;       // 4通道
	BYTE DI5;       // 5通道
	BYTE DI6;       // 6通道
	BYTE DI7;       // 7通道
	BYTE DI8;       // 8通道
	BYTE DI9;       // 9通道
	BYTE DI10;       // 10通道
	BYTE DI11;       // 11通道
	BYTE DI12;       // 12通道
	BYTE DI13;       // 13通道
	BYTE DI14;       // 14通道
	BYTE DI15;       // 15通道
} PCI2003_PARA_DI,*PPCI2003_PARA_DI;
#endif
//***********************************************************

//***********************************************************
// CreateFileObject所用的文件操作方式控制字(可通过或指令实现多种方式并操作)
#define PCI2003_modeRead          0x0000   // 只读文件方式
#define PCI2003_modeWrite         0x0001   // 只写文件方式
#define	PCI2003_modeReadWrite     0x0002   // 既读又写文件方式
#define PCI2003_modeCreate        0x1000   // 如果文件不存可以创建该文件，如果存在，则重建此文件，并清0
#define PCI2003_typeText          0x4000   // 以文本方式操作文件


//***********************************************************
#ifndef DEFINING
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//######################## 常规通用函数 #################################
	// 适用于本设备的最基本操作
	DEVAPI HANDLE FAR PASCAL PCI2003_CreateDevice(int DeviceID);  // 创建设备对象
	DEVAPI int FAR PASCAL PCI2003_GetDeviceCount(HANDLE hDevice); // 取得设备总台数
	DEVAPI BOOL PCI2003_ListDevice(HANDLE hDevice); // 列表系统当中的所有的该PCI设备
	DEVAPI BOOL PCI2003_ListDeviceEx(HANDLE hDevice); // 列表系统当中的所有的该PCI设备
    DEVAPI BOOL FAR PASCAL PCI2003_ReleaseDevice(HANDLE hDevice); // 关闭设备,禁止传输,且释放资源

	//####################### AD数据读取函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面的初始化设备和读取
	// AD数据两个函数便能轻松高效地实现高速、连续的数据采集
    DEVAPI BOOL FAR PASCAL PCI2003_InitDeviceProAD(    // 初始化设备，当返回TRUE后,设备即刻开始传输.
										HANDLE hDevice, // 设备对象
										PPCI2003_PARA_AD pADPara); // 硬件参数, 它仅在此函数中决定硬件状态
    DEVAPI BOOL FAR PASCAL PCI2003_ReadDeviceProAD_NotEmpty( // 用非空标志读取设备上的AD数据
								HANDLE hDevice,     // 设备句柄
								PWORD pADBuffer, // 将用于接受数据的用户缓冲区
								ULONG nReadSizeWords); // 读入的数据长度
    
	DEVAPI BOOL FAR PASCAL PCI2003_GetDevStatusAD_Half( // 等待FIFO半满标志，返回TRUE，则半满
								HANDLE hDevice);     // 设备句柄

	///////////////////////////////////////////////////////////////////////
	DEVAPI BOOL FAR PASCAL PCI2003_ReadDeviceProAD_Half(                    // FIFO半满读AD数据
								HANDLE hDevice,     // 设备句柄
								PWORD pADBuffer, // 将用于接受数据的用户缓冲区(该区必须开辟65536个字的空间)
								ULONG nReadSizeWords);
	DEVAPI BOOL FAR PASCAL PCI2003_ReleaseDeviceProAD(HANDLE hDevice); // 停止AD，且释放AD部件资源

	DEVAPI BOOL FAR PASCAL PCI2003_InitDeviceIntAD(HANDLE hDevice, HANDLE hEvent, ULONG nFifoHalfLength, PPCI2003_PARA_AD pADPara);
	DEVAPI LONG FAR PASCAL PCI2003_ReadDeviceIntAD(HANDLE hDevice,PWORD pADBuffer,ULONG nReadSizeWords);
	DEVAPI BOOL FAR PASCAL PCI2003_ReleaseDeviceIntAD(HANDLE hDevice);

   	//################# AD的硬件参数操作函数 ########################
	DEVAPI BOOL FAR PASCAL PCI2003_SaveParameter(HANDLE hDevice, PPCI2003_PARA_AD pADPara);  
    DEVAPI BOOL FAR PASCAL PCI2003_LoadParameter(HANDLE hDevice, PPCI2003_PARA_AD pADPara);

	//####################### DA数据输出函数 #################################
	// 适于大多数普通用户，这些接口最简单、最快捷、最可靠，让用户不必知道设备
	// 低层复杂的硬件控制协议和繁多的软件控制编程，仅用下面一个函数便能轻
	// 松实现高速、连续的DA数据输出
	DEVAPI BOOL FAR PASCAL PCI2003_WriteDeviceProDA(HANDLE hDevice,  // 设备对象句柄
													WORD nDAData,     // 准备输出的DA数据LSB原码
													int nDAChannel);  // 指定输出的DA通道

	//####################### 数字I/O输入输出函数 #################################
	// 用户可以使用WriteRegisterULong和ReadRegisterULong等函数直接控制寄存器进行I/O
	// 输入输出，但使用下面两个函数更省事，它不需要您关心寄存器分配和位操作等，而只
	// 需象VB等语言的属性操作那么简单地实现各开关量通道的控制。
    DEVAPI BOOL FAR PASCAL PCI2003_SetDeviceDO(             // 输出开关量状态
										HANDLE hDevice,     // 设备句柄								        
										PPCI2003_PARA_DO pDOPara);  // 开关状态

	DEVAPI BOOL FAR PASCAL PCI2003_GetDeviceDI(             // 取得开关量状态     
										HANDLE hDevice,     // 设备句柄								        
										PPCI2003_PARA_DI pDIPara);  // 开关状态

	//################# 内存映射寄存器直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	DEVAPI BOOL FAR PASCAL PCI2003_GetDeviceAddr( // 取得指定的指定设备ID号的映射寄存器的线性基地址，返回设备总数
												HANDLE hDevice,  // 设备对象句柄
												PULONG LinearAddr, // 返回指定映射寄存器的线性地址
											    	PULONG PhysAddr,   //  返回指定映射寄存器的物理地址
												int RegisterID = 0); // 设备映射寄存器的ID号（0-5）
    DEVAPI BOOL FAR PASCAL PCI2003_WriteRegisterByte( // 往设备的映射寄存器空间指定端口写入单节字数据
											HANDLE hDevice,  // 设备对象
											ULONG LinearAddr, // 指定映射寄存器的线性基地址
											ULONG OffsetBytes,  // 相对于基地址的偏移位置
											BYTE Value);		// 往指定地址写入单字节数据（其地址由线性基地址和偏移位置决定）
	DEVAPI BOOL FAR PASCAL PCI2003_WriteRegisterWord( // 写双字节数据（其余同上）
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes,  
											WORD Value);
	DEVAPI BOOL FAR PASCAL PCI2003_WriteRegisterULong( // 写四节字数据（其余同上）
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes,  
											ULONG Value);
	DEVAPI BYTE FAR PASCAL PCI2003_ReadRegisterByte( // 读入单字节数据（其余同上）
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes);
	DEVAPI WORD FAR PASCAL PCI2003_ReadRegisterWord( // 读入双字节数据（其余同上）
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes);
	DEVAPI ULONG FAR PASCAL PCI2003_ReadRegisterULong( // 读入四字节数据（其余同上）
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes);

	//################# I/O端口直接操作及读写函数 ########################
	// 适用于用户对本设备更直接、更特殊、更低层、更复杂的控制。比如根据特殊的
	// 控制对象需要特殊的控制流程和控制效率时，则用户可以使用这些接口予以实现。
	// 但这些函数主要适用于传统设备，如ISA总线、并口、串口等设备，不能用于本PCI设备
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortByte(HANDLE hDevice, UINT nPort, BYTE Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortWord(HANDLE hDevice, UINT nPort, WORD Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortULong(HANDLE hDevice, UINT nPort, ULONG Value);

    DEVAPI BYTE FAR PASCAL PCI2003_ReadPortByte(HANDLE hDevice, UINT nPort);
    DEVAPI WORD FAR PASCAL PCI2003_ReadPortWord(HANDLE hDevice, UINT nPort);
    DEVAPI ULONG FAR PASCAL PCI2003_ReadPortULong(HANDLE hDevice, UINT nPort);

	#if (WINVER==0x0400) // 只能用于NT4.0,其他操作系统的类似功能在CommUser中
	DEVAPI BOOL FAR PASCAL PCI2003_WritePortByteEx(UINT nPort, BYTE Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortWordEx(UINT nPort, WORD Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortULongEx(UINT nPort, ULONG Value);
    DEVAPI BYTE FAR PASCAL PCI2003_ReadPortByteEx(UINT nPort);
    DEVAPI WORD FAR PASCAL PCI2003_ReadPortWordEx(UINT nPort);
    DEVAPI ULONG FAR PASCAL PCI2003_ReadPortULongEx(UINT nPort);
	#endif

   	//########################## 文件操作函数 ############################
    DEVAPI HANDLE FAR PASCAL PCI2003_CreateFileObject(                    // 初始文件系统
										  HANDLE hDevice,     // 设备对象
										  LPCTSTR NewFileName,  // 新文件名
										  int Mode);		 // 文件操作方式    
    DEVAPI BOOL FAR PASCAL PCI2003_WriteFile(      // 保存用户空间中数据
										HANDLE hFileObject,  // 设备对象
										PWORD pUserRegion, // 用户数据空间地址
										ULONG nWriteSizeBytes); // 缓冲区大小(字节)

    DEVAPI BOOL FAR PASCAL PCI2003_ReadFile(  // 读数据
										  HANDLE hFileObject,     // 设备对象
										  PWORD pFileUserRegion, // 接受文件数据的用户内存缓冲区
										  ULONG OffsetBytes,   // 从文件前端开始的偏移位置
										  ULONG nReadSizeBytes); // 从偏移位置开始读的字节数

	DEVAPI BOOL FAR PASCAL PCI2003_SetFileOffset( // 设置文件偏移指针
										  HANDLE hFileObject,   // 文件对象
										  ULONG nOffsetBytes);  // 文件偏移位置（以字为单位）  

	DEVAPI ULONG FAR PASCAL PCI2003_GetFileLength(HANDLE hFileObject); // 取得指定文件长度（字节）

    DEVAPI BOOL FAR PASCAL PCI2003_ReleaseFile(HANDLE hFileObject);
    DEVAPI ULONGLONG FAR PASCAL PCI2003_GetDiskFreeBytes(               // 获得指定盘符的磁盘空间
								      LPCTSTR strDiskName);            // 盘符名,如C盘为"C:\\", D盘为"D:\\"
  

	//################### 线程操作函数 ########################
	DEVAPI HANDLE FAR PASCAL PCI2003_CreateSystemEvent(void); // 创建内核事件对象
	DEVAPI BOOL FAR PASCAL PCI2003_ReleaseSystemEvent(HANDLE hEvent); // 释放内核事件对象
	DEVAPI BOOL FAR PASCAL PCI2003_CreateVBThread(HANDLE *hThread, LPTHREAD_START_ROUTINE StartThread);
    DEVAPI BOOL FAR PASCAL PCI2003_TerminateVBThread(HANDLE hThreadHandle);
	// 纳秒延时操作，不消耗CPU时间(在Win98/95/Me下以1毫秒为单位)
	DEVAPI BOOL FAR PASCAL PCI2003_DelayTimeNs(HANDLE hDevice, LONG nTime);  

#ifdef __cplusplus
}
#endif
