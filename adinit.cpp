// Sys.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"
#include "conio.h"

#include "pci2003.h"


WORD InUserRegion[32];
ULONG LinearAddr, PhysAddr; 


BOOL InitDeviceProAD(    // ��ʼ���豸��������TRUE��,�豸���̿�ʼ����.
										HANDLE hDevice, // �豸����
										PPCI2003_PARA_AD pPara); // Ӳ������, �����ڴ˺����о���Ӳ��״̬
BOOL ReadDeviceProAD_NotEmpty( // �÷ǿձ�־��ȡ�豸�ϵ�AD����
								HANDLE hDevice,     // �豸���
								PWORD InUserRegion, // �����ڽ������ݵ��û�������
								ULONG ReadSizeWords);	// ��������ݳ���
BOOL ReleaseDeviceProAD( // �ر��豸,��ֹ����,���ͷ���Դ
								    HANDLE hDevice);    // �豸���

BOOL InitDeviceProAD( HANDLE hDevice, PPCI2003_PARA_AD pPara )
{
	ULONG Index=0;   
	ULONG Control;
	WORD TimerCount;
	BYTE TimerCountL, TimerCountH;	
	PCI2003_PARA_AD Para = *pPara;

	
	Control=(Para.Gains-1)*32 + Para.FirstChannel;    
	TimerCount =(WORD)((1000000*2)/Para.Frequency);   // Ƶ��
	TimerCountL= (BYTE)(TimerCount&0x00ff);
	TimerCountH= (BYTE)((TimerCount&0xff00)>>8);

	// ȡ��0��ӳ��Ĵ��������Ի���ַ
    PCI2003_GetDeviceAddr( hDevice, &LinearAddr, &PhysAddr,0) ;  
	// ��ʼ�豸ʵ�ʲ���
	// �豸�����֣�D0=1:�����жϣ�D1=1:����ADת����D2=0:�����������
	// �˴��ǲ������������    
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x280, 0x08);   
	////////////////////////////////////////////////////////
	//����ͨ��,ĩͨ��, ���棨D7-D5��+��ͨ��(D4-D0)
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x200, Control);
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x210, Para.LastChannel);  // ĩͨ��	
    /////////////////////////////////////////////////////////	
	// д��8254���������֣���ʽ3��������0�����ö˿ڵ�8λ��Ч��,˫�ֽڣ�������������ʽ2   
	 PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x220, 0x34);
	// �������д�����ּĴ���0x230��ʱ���źţ�д8254�����֣�
	// D3��WR53�� D2��CS53�� D1D0��A1A0
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0b);	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x03);	
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0f);
	
	// д������0�ļ���ֵ(��8λ)
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x220, TimerCountL);
    
	// �������д������0�ļ���ֵ��ʱ���ź�
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x08);	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x00);	
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0c);    
	// д������0�ļ���ֵ(��8λ)
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x220, TimerCountH);    
	// �������д������0�ļ���ֵ��ʱ���ź�
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x08);	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x00);	
    PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x230, 0x0c); 

	//��FIFO�洢��
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x290, 0x07);  
    	// ����8253����	
	PCI2003_WriteRegisterULong( hDevice, LinearAddr, 0x280, Para.TriggerSource); //�ⴥ��

	return TRUE;

}


BOOL ReadDeviceProAD_NotEmpty( HANDLE hDevice, PWORD InUserRegion, ULONG ReadSizeWords)
{
	register ULONG i;    
	register ULONG Value;
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// ���¶�ȡ���ݴ������ǿշ�ʽ
	for( i=0; i<ReadSizeWords; i++)
	{	
		do
		{
			Value = PCI2003_ReadRegisterULong(hDevice, LinearAddr, 0x260)&0x00000001;    // ����
		}
		while(Value!=1); // D1=1: ����ת�� D2=0: ���

		//��Fifo�ź�,	
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x00);
		InUserRegion[i]=(WORD) PCI2003_ReadRegisterULong(hDevice, LinearAddr, 0x270); 
		PCI2003_WriteRegisterULong(hDevice, LinearAddr, 0x2a0, 0x01);
	}
	
	return TRUE;


}











