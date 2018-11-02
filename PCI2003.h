// ����AD�ɼ��Ĳ����ṹ
#ifndef _PCI2003_PARA_AD
typedef struct _PCI2003_PARA_AD      // ����ADת����Ӳ������
{
	DWORD FirstChannel;       // ��ͨ��
	DWORD LastChannel;        // ĩͨ��
	DWORD Frequency;          // �ɼ�Ƶ��(Hz)
	DWORD Gains;              // �ɼ�����
	DWORD TriggerSource;      // ����Դѡ��
}PCI2003_PARA_AD,*PPCI2003_PARA_AD;
#endif

//***********************************************************
// ADת��������ʽ������
#define PCI2003_OUT_TRIGGER 0x0032    // �ⴥ����ʽ
#define PCI2003_IN_TRIGGER  0x0012    // �ڴ�����ʽ

//***********************************************************
// ���ڿ������Ĳ����ṹ
#ifndef _PCI2003_PARA_DO
typedef struct _PCI2003_PARA_DO      // �������������
{
	BYTE DO0;       // 0ͨ��
	BYTE DO1;       // 1ͨ��
	BYTE DO2;       // 2ͨ��
	BYTE DO3;       // 3ͨ��
	BYTE DO4;       // 4ͨ��
	BYTE DO5;       // 5ͨ��
	BYTE DO6;       // 6ͨ��
	BYTE DO7;       // 7ͨ��
	BYTE DO8;       // 8ͨ��
	BYTE DO9;       // 9ͨ��
	BYTE DO10;       // 10ͨ��
	BYTE DO11;       // 11ͨ��
	BYTE DO12;       // 12ͨ��
	BYTE DO13;       // 13ͨ��
	BYTE DO14;       // 14ͨ��
	BYTE DO15;       // 15ͨ��
} PCI2003_PARA_DO,*PPCI2003_PARA_DO;
#endif

#ifndef _PCI2003_PARA_DI
typedef struct _PCI2003_PARA_DI      // �������������
{
	BYTE DI0;       // 0ͨ��
	BYTE DI1;       // 1ͨ��
	BYTE DI2;       // 2ͨ��
	BYTE DI3;       // 3ͨ��
	BYTE DI4;       // 4ͨ��
	BYTE DI5;       // 5ͨ��
	BYTE DI6;       // 6ͨ��
	BYTE DI7;       // 7ͨ��
	BYTE DI8;       // 8ͨ��
	BYTE DI9;       // 9ͨ��
	BYTE DI10;       // 10ͨ��
	BYTE DI11;       // 11ͨ��
	BYTE DI12;       // 12ͨ��
	BYTE DI13;       // 13ͨ��
	BYTE DI14;       // 14ͨ��
	BYTE DI15;       // 15ͨ��
} PCI2003_PARA_DI,*PPCI2003_PARA_DI;
#endif
//***********************************************************

//***********************************************************
// CreateFileObject���õ��ļ�������ʽ������(��ͨ����ָ��ʵ�ֶ��ַ�ʽ������)
#define PCI2003_modeRead          0x0000   // ֻ���ļ���ʽ
#define PCI2003_modeWrite         0x0001   // ֻд�ļ���ʽ
#define	PCI2003_modeReadWrite     0x0002   // �ȶ���д�ļ���ʽ
#define PCI2003_modeCreate        0x1000   // ����ļ�������Դ������ļ���������ڣ����ؽ����ļ�������0
#define PCI2003_typeText          0x4000   // ���ı���ʽ�����ļ�


//***********************************************************
#ifndef DEFINING
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	//######################## ����ͨ�ú��� #################################
	// �����ڱ��豸�����������
	DEVAPI HANDLE FAR PASCAL PCI2003_CreateDevice(int DeviceID);  // �����豸����
	DEVAPI int FAR PASCAL PCI2003_GetDeviceCount(HANDLE hDevice); // ȡ���豸��̨��
	DEVAPI BOOL PCI2003_ListDevice(HANDLE hDevice); // �б�ϵͳ���е����еĸ�PCI�豸
	DEVAPI BOOL PCI2003_ListDeviceEx(HANDLE hDevice); // �б�ϵͳ���е����еĸ�PCI�豸
    DEVAPI BOOL FAR PASCAL PCI2003_ReleaseDevice(HANDLE hDevice); // �ر��豸,��ֹ����,���ͷ���Դ

	//####################### AD���ݶ�ȡ���� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������ĳ�ʼ���豸�Ͷ�ȡ
	// AD�������������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�
    DEVAPI BOOL FAR PASCAL PCI2003_InitDeviceProAD(    // ��ʼ���豸��������TRUE��,�豸���̿�ʼ����.
										HANDLE hDevice, // �豸����
										PPCI2003_PARA_AD pADPara); // Ӳ������, �����ڴ˺����о���Ӳ��״̬
    DEVAPI BOOL FAR PASCAL PCI2003_ReadDeviceProAD_NotEmpty( // �÷ǿձ�־��ȡ�豸�ϵ�AD����
								HANDLE hDevice,     // �豸���
								PWORD pADBuffer, // �����ڽ������ݵ��û�������
								ULONG nReadSizeWords); // ��������ݳ���
    
	DEVAPI BOOL FAR PASCAL PCI2003_GetDevStatusAD_Half( // �ȴ�FIFO������־������TRUE�������
								HANDLE hDevice);     // �豸���

	///////////////////////////////////////////////////////////////////////
	DEVAPI BOOL FAR PASCAL PCI2003_ReadDeviceProAD_Half(                    // FIFO������AD����
								HANDLE hDevice,     // �豸���
								PWORD pADBuffer, // �����ڽ������ݵ��û�������(�������뿪��65536���ֵĿռ�)
								ULONG nReadSizeWords);
	DEVAPI BOOL FAR PASCAL PCI2003_ReleaseDeviceProAD(HANDLE hDevice); // ֹͣAD�����ͷ�AD������Դ

	DEVAPI BOOL FAR PASCAL PCI2003_InitDeviceIntAD(HANDLE hDevice, HANDLE hEvent, ULONG nFifoHalfLength, PPCI2003_PARA_AD pADPara);
	DEVAPI LONG FAR PASCAL PCI2003_ReadDeviceIntAD(HANDLE hDevice,PWORD pADBuffer,ULONG nReadSizeWords);
	DEVAPI BOOL FAR PASCAL PCI2003_ReleaseDeviceIntAD(HANDLE hDevice);

   	//################# AD��Ӳ�������������� ########################
	DEVAPI BOOL FAR PASCAL PCI2003_SaveParameter(HANDLE hDevice, PPCI2003_PARA_AD pADPara);  
    DEVAPI BOOL FAR PASCAL PCI2003_LoadParameter(HANDLE hDevice, PPCI2003_PARA_AD pADPara);

	//####################### DA����������� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������һ������������
	// ��ʵ�ָ��١�������DA�������
	DEVAPI BOOL FAR PASCAL PCI2003_WriteDeviceProDA(HANDLE hDevice,  // �豸������
													WORD nDAData,     // ׼�������DA����LSBԭ��
													int nDAChannel);  // ָ�������DAͨ��

	//####################### ����I/O����������� #################################
	// �û�����ʹ��WriteRegisterULong��ReadRegisterULong�Ⱥ���ֱ�ӿ��ƼĴ�������I/O
	// �����������ʹ����������������ʡ�£�������Ҫ�����ļĴ��������λ�����ȣ���ֻ
	// ����VB�����Ե����Բ�����ô�򵥵�ʵ�ָ�������ͨ���Ŀ��ơ�
    DEVAPI BOOL FAR PASCAL PCI2003_SetDeviceDO(             // ���������״̬
										HANDLE hDevice,     // �豸���								        
										PPCI2003_PARA_DO pDOPara);  // ����״̬

	DEVAPI BOOL FAR PASCAL PCI2003_GetDeviceDI(             // ȡ�ÿ�����״̬     
										HANDLE hDevice,     // �豸���								        
										PPCI2003_PARA_DI pDIPara);  // ����״̬

	//################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	DEVAPI BOOL FAR PASCAL PCI2003_GetDeviceAddr( // ȡ��ָ����ָ���豸ID�ŵ�ӳ��Ĵ��������Ի���ַ�������豸����
												HANDLE hDevice,  // �豸������
												PULONG LinearAddr, // ����ָ��ӳ��Ĵ��������Ե�ַ
											    	PULONG PhysAddr,   //  ����ָ��ӳ��Ĵ����������ַ
												int RegisterID = 0); // �豸ӳ��Ĵ�����ID�ţ�0-5��
    DEVAPI BOOL FAR PASCAL PCI2003_WriteRegisterByte( // ���豸��ӳ��Ĵ����ռ�ָ���˿�д�뵥��������
											HANDLE hDevice,  // �豸����
											ULONG LinearAddr, // ָ��ӳ��Ĵ��������Ի���ַ
											ULONG OffsetBytes,  // ����ڻ���ַ��ƫ��λ��
											BYTE Value);		// ��ָ����ַд�뵥�ֽ����ݣ����ַ�����Ի���ַ��ƫ��λ�þ�����
	DEVAPI BOOL FAR PASCAL PCI2003_WriteRegisterWord( // д˫�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes,  
											WORD Value);
	DEVAPI BOOL FAR PASCAL PCI2003_WriteRegisterULong( // д�Ľ������ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes,  
											ULONG Value);
	DEVAPI BYTE FAR PASCAL PCI2003_ReadRegisterByte( // ���뵥�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes);
	DEVAPI WORD FAR PASCAL PCI2003_ReadRegisterWord( // ����˫�ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes);
	DEVAPI ULONG FAR PASCAL PCI2003_ReadRegisterULong( // �������ֽ����ݣ�����ͬ�ϣ�
											HANDLE hDevice, 
											ULONG LinearAddr, 
											ULONG OffsetBytes);

	//################# I/O�˿�ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	// ����Щ������Ҫ�����ڴ�ͳ�豸����ISA���ߡ����ڡ����ڵ��豸���������ڱ�PCI�豸
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortByte(HANDLE hDevice, UINT nPort, BYTE Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortWord(HANDLE hDevice, UINT nPort, WORD Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortULong(HANDLE hDevice, UINT nPort, ULONG Value);

    DEVAPI BYTE FAR PASCAL PCI2003_ReadPortByte(HANDLE hDevice, UINT nPort);
    DEVAPI WORD FAR PASCAL PCI2003_ReadPortWord(HANDLE hDevice, UINT nPort);
    DEVAPI ULONG FAR PASCAL PCI2003_ReadPortULong(HANDLE hDevice, UINT nPort);

	#if (WINVER==0x0400) // ֻ������NT4.0,��������ϵͳ�����ƹ�����CommUser��
	DEVAPI BOOL FAR PASCAL PCI2003_WritePortByteEx(UINT nPort, BYTE Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortWordEx(UINT nPort, WORD Value);
    DEVAPI BOOL FAR PASCAL PCI2003_WritePortULongEx(UINT nPort, ULONG Value);
    DEVAPI BYTE FAR PASCAL PCI2003_ReadPortByteEx(UINT nPort);
    DEVAPI WORD FAR PASCAL PCI2003_ReadPortWordEx(UINT nPort);
    DEVAPI ULONG FAR PASCAL PCI2003_ReadPortULongEx(UINT nPort);
	#endif

   	//########################## �ļ��������� ############################
    DEVAPI HANDLE FAR PASCAL PCI2003_CreateFileObject(                    // ��ʼ�ļ�ϵͳ
										  HANDLE hDevice,     // �豸����
										  LPCTSTR NewFileName,  // ���ļ���
										  int Mode);		 // �ļ�������ʽ    
    DEVAPI BOOL FAR PASCAL PCI2003_WriteFile(      // �����û��ռ�������
										HANDLE hFileObject,  // �豸����
										PWORD pUserRegion, // �û����ݿռ��ַ
										ULONG nWriteSizeBytes); // ��������С(�ֽ�)

    DEVAPI BOOL FAR PASCAL PCI2003_ReadFile(  // ������
										  HANDLE hFileObject,     // �豸����
										  PWORD pFileUserRegion, // �����ļ����ݵ��û��ڴ滺����
										  ULONG OffsetBytes,   // ���ļ�ǰ�˿�ʼ��ƫ��λ��
										  ULONG nReadSizeBytes); // ��ƫ��λ�ÿ�ʼ�����ֽ���

	DEVAPI BOOL FAR PASCAL PCI2003_SetFileOffset( // �����ļ�ƫ��ָ��
										  HANDLE hFileObject,   // �ļ�����
										  ULONG nOffsetBytes);  // �ļ�ƫ��λ�ã�����Ϊ��λ��  

	DEVAPI ULONG FAR PASCAL PCI2003_GetFileLength(HANDLE hFileObject); // ȡ��ָ���ļ����ȣ��ֽڣ�

    DEVAPI BOOL FAR PASCAL PCI2003_ReleaseFile(HANDLE hFileObject);
    DEVAPI ULONGLONG FAR PASCAL PCI2003_GetDiskFreeBytes(               // ���ָ���̷��Ĵ��̿ռ�
								      LPCTSTR strDiskName);            // �̷���,��C��Ϊ"C:\\", D��Ϊ"D:\\"
  

	//################### �̲߳������� ########################
	DEVAPI HANDLE FAR PASCAL PCI2003_CreateSystemEvent(void); // �����ں��¼�����
	DEVAPI BOOL FAR PASCAL PCI2003_ReleaseSystemEvent(HANDLE hEvent); // �ͷ��ں��¼�����
	DEVAPI BOOL FAR PASCAL PCI2003_CreateVBThread(HANDLE *hThread, LPTHREAD_START_ROUTINE StartThread);
    DEVAPI BOOL FAR PASCAL PCI2003_TerminateVBThread(HANDLE hThreadHandle);
	// ������ʱ������������CPUʱ��(��Win98/95/Me����1����Ϊ��λ)
	DEVAPI BOOL FAR PASCAL PCI2003_DelayTimeNs(HANDLE hDevice, LONG nTime);  

#ifdef __cplusplus
}
#endif
