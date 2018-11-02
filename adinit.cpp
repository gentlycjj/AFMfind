// Sys.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "pci2003.h"


WORD InUserRegion[32];
ULONG LinearAddr, PhysAddr; 


BOOL InitDeviceProAD(    // 初始化设备，当返回TRUE后,设备即刻开始传输.
										HANDLE hDevice, // 设备对象
										PPCI2003_PARA_AD pPara); // 硬件参数, 它仅在此函数中决定硬件状态
BOOL ReadDeviceProAD_NotEmpty( // 用非空标志读取设备上的AD数据
								HANDLE hDevice,     // 设备句柄
								PWORD InUserRegion, // 将用于接受数据的用户缓冲区
								ULONG ReadSizeWords);	// 读入的数据长度
BOOL ReleaseDeviceProAD( // 关闭设备,禁止传输,且释放资源
								    HANDLE hDevice);    // 设备句柄

BOOL InitDeviceProAD( HANDLE hDevice, PPCI2003_PARA_AD pPara )
{
	ULONG Index=0;   
	ULONG Control;
	WORD TimerCount;
	BYTE TimerCountL, TimerCountH;	
	PCI2003_PARA_AD Para = *pPara;

	
	Control=(Para.Gains-1)*32 + Para.FirstChannel;    
	TimerCount =(WORD)((1000000*2)/Para.Frequency);   // 频率
	TimerCountL= (BYTE)(TimerCount&0x00ff);
	TimerCountH= (BYTE)((TimerCount&0xff00)>>8);

	// 取得0号映射寄存器的线性基地址
    PCI2003_GetDeviceAddr( hDevice, &LinearAddr, &PhysAddr,0) ;  
	// 开始设备实际操作
	// 设备控制字，D0=1:允许中断，D1=1:允许AD转换，D2=0:允许开关量输出
	// 此处是不允许开关量输出    
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x280, 0x08);   
	////////////////////////////////////////////////////////
	//置首通道,末通道, 增益（D7-D5）+首通道(D4-D0)
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x200, Control);
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x210, Para.LastChannel);  // 末通道	
    /////////////////////////////////////////////////////////	
	// 写入8254工作控制字，方式3，计数器0，（该端口低8位有效）,双字节，计数器工作方式2   
	 PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x220, 0x34);
	// 程序产生写控制字寄存器0x230的时序信号（写8254控制字）
	// D3：WR53； D2：CS53； D1D0：A1A0
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0b);	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x03);	
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0f);
	
	// 写计数器0的计数值(低8位)
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x220, TimerCountL);
    
	// 程序产生写计数器0的计数值的时序信号
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x08);	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x00);	
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0c);    
	// 写计数器0的计数值(高8位)
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x220, TimerCountH);    
	// 程序产生写计数器0的计数值的时序信号
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x08);	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x00);	
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0c); 

	//清FIFO存储器
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x290, 0x07);  
    	// 允许8253工作	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x280, Para.TriggerSource); //外触发

	return TRUE;

}


BOOL ReadDeviceProAD_NotEmpty( HANDLE hDevice, PWORD InUserRegion, ULONG ReadSizeWords)
{
	register ULONG i;    
	register ULONG Value;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// 以下读取数据代码属非空方式
	for( i=0; i<ReadSizeWords; i++)
	{	
		do
		{
			Value = PCI2003_ReadRegisterULong(hDevice, LinearAddr, 0x260)&0x00000001;    // 操作
		}
		while(Value!=1); // D1=1: 正在转换 D2=0: 完成

		//读Fifo信号,	
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);
		InUserRegion[i]=(WORD) PCI2003_ReadRegisterULong(hDevice, LinearAddr, 0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);
	}
	
	return TRUE;


}











