#ifndef _PCI8620_DEVICE_
#define _PCI8620_DEVICE_
//#################### ADӲ������PCI8620_PARA_AD���� #####################
// ����AD������ʵ��Ӳ������
typedef struct _PCI8620_PARA_AD     
{
	LONG ADMode;            // ADģʽѡ��(����/���鷽ʽ)
	LONG FirstChannel;      // ��ͨ��[0,15]
	LONG LastChannel;       // ĩͨ��[0,15],Ҫ��ĩͨ��������ڻ������ͨ��
	LONG Frequency;         // �ɼ�Ƶ��,��λΪHz, [31, 250000]
	LONG GroupInterval;     // ����ʱ�������(��λ��΢��)[1, 419430]
	LONG LoopsOfGroup;	    // ����ѭ������[1, 65535]
	LONG Gains;				// ��������
	LONG InputRange;		// ģ�����������̷�Χ
	LONG TriggerMode;       // ����ģʽѡ��
	LONG TriggerSource;		// ����Դѡ��
	LONG TrigLevelVolt;		// ������ƽ(0~10000mV)
	LONG TriggerType;		// ��������
	LONG TriggerDir;		// ��������ѡ��(����/���򴥷�)
	LONG ClockSource;		// ʱ��Դѡ��(��/��ʱ��Դ)
	LONG bClockOutput;      // ����ʱ�����
	LONG GroundingMode;		// �ӵ�ģʽ
} PCI8620_PARA_AD, *PPCI8620_PARA_AD;

//***********************************************************
// ADӲ������PCI8620_PARA_AD�е�ADMode��ʹ�ù���ģʽѡ��
const long PCI8620_ADMODE_SEQUENCE		= 0x00; // ��������
const long PCI8620_ADMODE_GROUP			= 0x01; // �������

//***********************************************************
// ADӲ������PCI8620_PARA_AD�е�InputRangeģ�������뷶Χ��ʹ�õ�ѡ��
const long PCI8620_INPUT_N10000_P10000mV= 0x00; // ��10000mV
const long PCI8620_INPUT_N5000_P5000mV	= 0x01; // ��5000mV
const long PCI8620_INPUT_N2500_P2500mV	= 0x02; // ��2500mV
const long PCI8620_INPUT_0_P10000mV		= 0x03; // 0��10000mV

//***********************************************************
// AD����PCI8620_PARA_AD�е�Gainsʹ�õ�Ӳ������ѡ��
const long PCI8620_GAINS_1MULT			= 0x00; // 1������
const long PCI8620_GAINS_2MULT			= 0x01; // 2������
const long PCI8620_GAINS_4MULT			= 0x02; // 4������
const long PCI8620_GAINS_8MULT			= 0x03; // 8������

//***********************************************************
// ADӲ������PCI8620_PARA_AD�е�TriggerMode��Ա������ʹ�ô���ģʽѡ��
const long PCI8620_TRIGMODE_SOFT		= 0x00; // �������(�����ڴ���)
const long PCI8620_TRIGMODE_POST		= 0x01; // Ӳ������(�����ⴥ��)

//***********************************************************
// ADӲ������PCI8620_PARA_AD�е�TriggerSource����Դ�ź���ʹ�õ�ѡ��
const long PCI8620_TRIGSRC_ATR			= 0x00; // ѡ���ⲿATR��Ϊ����Դ
const long PCI8620_TRIGSRC_DTR			= 0x01; // ѡ���ⲿDTR��Ϊ����Դ

// ADӲ������PCI8620_PARA_AD�е�TriggerType����������ʹ�õ�ѡ��
const long PCI8620_TRIGTYPE_EDGE		= 0x00; // ���ش���
const long PCI8620_TRIGTYPE_PULSE		= 0x01; // ��ƽ����

//***********************************************************
// ADӲ������PCI8620_PARA_AD�е�TriggerDir����������ʹ�õ�ѡ��
const long PCI8620_TRIGDIR_NEGATIVE		= 0x00; // ���򴥷�(�͵�ƽ/�½��ش���)
const long PCI8620_TRIGDIR_POSITIVE		= 0x01; // ���򴥷�(�ߵ�ƽ/�����ش���)
const long PCI8620_TRIGDIR_POSIT_NEGAT	= 0x02; // �����򴥷�(��/�͵�ƽ������/�½��ش���)

//***********************************************************
// ADӲ������PCI8620_PARA_AD�е�ClockSourceʱ��Դ��ʹ�õ�ѡ��
const long PCI8620_CLOCKSRC_IN			= 0x00; // �ڲ�ʱ��
const long PCI8620_CLOCKSRC_OUT			= 0x01; // �ⲿʱ��

// ADӲ������PCI8620_PARA_AD�е�bClockOutput��Ա������ʹ���ڲ����ⲿʱ��Դѡ��
const long PCI8620_CLOCKOUT_DISABLE		= 0x00; // ��ֹ�����ϵ��Դ�ʱ���������
const long PCI8620_CLOCKOUT_ENABLE		= 0x01; // �������ϵ��Դ�ʱ���������

//***********************************************************
// AD����(PCI8620_PARA_AD)�е�GroundingModeʹ�õ�ģ���źŽӵط�ʽѡ��
const long PCI8620_GNDMODE_SE			= 0x00;	// ���˷�ʽ(SE:Single end)
const long PCI8620_GNDMODE_DI			= 0x01;	// ˫�˷�ʽ(DI:Differential)

//*************************************************************************************
// ����AD������ʵ��Ӳ������
typedef struct _PCI8620_STATUS_AD     
{
	LONG bNotEmpty;			// ����FIFO�洢���ķǿձ�־��=TRUE�ǿգ� = FALSE ��
	LONG bHalf;				// ����FIFO�洢���İ�����־��=TRUE�������ϣ� = FALSE ��������
	LONG bOverflow;			// ����FIFO�洢���������־��= TRUE�ѷ�������� = FALSE��δ�������
} PCI8620_STATUS_AD, *PPCI8620_STATUS_AD;

const int PCI8620_MAX_SEGMENT_COUNT = 64;
typedef struct _PCI8620_STATUS_DMA     
{
	LONG iCurSegmentID;								// ��ǰ�λ���ID,��ʾDMA���ڴ���Ļ�������
	LONG bSegmentSts[PCI8620_MAX_SEGMENT_COUNT];	// �������������¾�״̬,=1��ʾ����Ӧ����������Ϊ��,����Ϊ��
	LONG bBufferOverflow;							// �������״̬
} PCI8620_STATUS_DMA, *PPCI8620_STATUS_DMA;

//***********************************************************
// DA�������WriteDeviceDA��ģ���������Χ����OutputRange��ʹ�õ�ѡ��
const long PCI8620_OUTPUT_0_P5000mV			= 0x00;		// 0��5000mV
const long PCI8620_OUTPUT_0_P10000mV		= 0x01;		// 0��10000mV
const long PCI8620_OUTPUT_0_P10800mV		= 0x02;		// 0��10800mV
const long PCI8620_OUTPUT_N5000_P5000mV		= 0x03;		// ��5000mV
const long PCI8620_OUTPUT_N10000_P10000mV	= 0x04;		// ��10000mV
const long PCI8620_OUTPUT_N10800_P10800mV	= 0x05;		// ��10800mV

//***********************************************************
// ���ڼ�����CNT�Ĳ���
// Ӳ������ControlMode������ģʽѡ��
const long PCI8620_GATEMODE_POSITIVE_0	= 0x00;		// COUNTER:GATE�ߵ�ƽʱ�������͵�ƽʱֹͣ����������ʱ����д���ֵ������ֵ����
													// �������������жϣ�д���ֵ��ʼ����ʱOUT��ʼΪ0����������0ʱOUTΪ1

const long PCI8620_GATEMODE_RISING_1	= 0x01;		// COUNTER:GATE�ϱ��ش��������������г���GATE����������װ���ֵ����
													// �ɱ�̵������壺��д���ֵʱOUTΪ1������ʼ����ʱOUTΪ0����������0ʱOUT�ٴ�Ϊ1

const long PCI8620_GATEMODE_POSITIVE_2	= 0x02;		// COUNTER:GATE�ߵ�ƽʱ�������͵�ƽʱֹͣ����������ʱ����д���ֵ������ֵ����
													// Ƶ�ʷ������������ڼ�OUTΪ1��������0�����һ�����ڵ�0��������װ�����ֵ����

const long PCI8620_GATEMODE_POSITIVE_3	= 0x03;		// COUNTER:GATE�ߵ�ƽʱ�������͵�ƽʱֹͣ����������ʱ����д���ֵ������ֵ����
													// �����������������ڼ�OUTΪ1��������0�����һ�����ڵ�0��������װ�����ֵ����

const long PCI8620_GATEMODE_POSITIVE_4	= 0x04;		// COUNTER:GATE�ߵ�ƽʱ�������͵�ƽʱֹͣ����������ʱ����д���ֵ������ֵ����
													// �������ѡͨ��д���ֵOUTΪ1�� ��������OUT���һ�����ڵ͵�ƽ�ź�

const long PCI8620_GATEMODE_RISING_5	= 0x05;		// COUNTER:GATE�ϱ��ش��������������г���GATE����������װ���ֵ����
													// Ӳ������ѡͨ��д���ֵOUTΪ1�� ��������OUT���һ�����ڵ͵�ƽ�ź�

//***********************************************************
// CreateFileObject���õ��ļ�������ʽ������(��ͨ����ָ��ʵ�ֶ��ַ�ʽ������)
const long PCI8620_modeRead					= 0x0000;	// ֻ���ļ���ʽ
const long PCI8620_modeWrite				= 0x0001;	// ֻд�ļ���ʽ
const long PCI8620_modeReadWrite			= 0x0002;	// �ȶ���д�ļ���ʽ
const long PCI8620_modeCreate				= 0x1000;	// ����ļ�������Դ������ļ���������ڣ����ؽ����ļ�������0
const long PCI8620_typeText					= 0x4000;	// ���ı���ʽ�����ļ�

//***********************************************************
// �û������ӿ�
#ifndef _PCI8620_DRIVER_
#define DEVAPI __declspec(dllimport)
#else
#define DEVAPI __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	//######################## �豸��������� ##############################
	HANDLE DEVAPI FAR PASCAL PCI8620_CreateDevice(int DeviceLgcID = 0);     // ���߼��Ŵ����豸����
	int DEVAPI FAR PASCAL PCI8620_GetDeviceCount(HANDLE hDevice); // ȡ���豸��̨��
	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceCurrentID(HANDLE hDevice, PLONG DeviceLgcID, PLONG DevicePhysID);
	BOOL DEVAPI FAR PASCAL PCI8620_ListDeviceDlg(HANDLE hDevice); // �ԶԻ����巽ʽ�б�ϵͳ���е����еĸ�PCI�豸
    BOOL DEVAPI FAR PASCAL PCI8620_ReleaseDevice(HANDLE hDevice); // ���ͷ��豸����

	//####################### AD���ݶ�ȡ���� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣���������ĳ�ʼ���豸�Ͷ�ȡ
	// AD�������������������ɸ�Ч��ʵ�ָ��١����������ݲɼ�

	// AD�����ѯ��ʽ����
	BOOL DEVAPI FAR PASCAL PCI8620_InitDeviceProAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
									HANDLE hDevice,			// �豸����,����CreateDevice��������
									PPCI8620_PARA_AD pADPara); // Ӳ������, �����ڴ˺����о���Ӳ��״̬

    BOOL DEVAPI FAR PASCAL PCI8620_StartDeviceProAD(		// �ڳ�ʼ��֮�������豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8620_ReadDeviceProAD_Npt(		// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(����ǿշ�ʽ)
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									ULONG ADBuffer[],		// ����ԭʼAD���ݵ��û�������
									LONG nReadSizeWords,	// �����ƫλ����������ݳ���(��)
									PLONG nRetSizeWords);   // ����ʵ�ʶ�ȡ�ĳ���(��)

	BOOL DEVAPI FAR PASCAL PCI8620_GetDevStatusProAD(		// ��AD����������ȡ���豸�ĸ���״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									PPCI8620_STATUS_AD pADStatus); // AD�ĸ�����Ϣ�ṹ��

	BOOL DEVAPI FAR PASCAL PCI8620_ReadDeviceProAD_Half(	// ��AD��־��Чʱ���ô˺�����ȡ�豸�ϵ�AD����(���������ʽ)
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									ULONG ADBuffer[],		// ����ԭʼAD���ݵ��û�������
									LONG nReadSizeWords,	// �����ƫλ����������ݳ���(��)
									PLONG nRetSizeWords);	// ����ʵ�ʶ�ȡ�ĳ���(��)
		
    BOOL DEVAPI FAR PASCAL PCI8620_StopDeviceProAD(			// �������豸֮����ͣ�豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8620_ReleaseDeviceProAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
									HANDLE hDevice);		// �豸���,����CreateDevice��������

	//##################### ADֱ���ڴ�(DMA)��ʽ����(�ϲ��û�����) ###########################
	BOOL DEVAPI FAR PASCAL PCI8620_InitDeviceDmaAD(			// ��ʼ���豸��������TRUE��,�豸��׼������.
									HANDLE hDevice,			// �豸����,����CreateDevice��������
									HANDLE hDmaEvent,		// DMA�¼����,����CreateSystemEvent����
									ULONG ADBuffer[],		// �û�������,���Ϊ��ά����
									ULONG nReadSizeWords,	// ÿ��DMAʱ,�û���ָ������Ӧ��ȡ��ʵ�ʳ���(ҪС�ڻ����nSegmentSizeWords)
									LONG nSegmentCount,		// ����ֶε�����,ȡֵ��ΧΪ2-128
									ULONG nSegmentSizeWords,// �������ֶεĶγ�(�������FIFO��������)
									PPCI8620_PARA_AD pADPara);	// Ӳ������, �����ڴ˺����о���Ӳ��״̬

	BOOL DEVAPI FAR PASCAL PCI8620_StartDeviceDmaAD(		// �ڳ�ʼ��֮�������豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8620_GetDevStatusDmaAD(		// ��AD����������ȡ��DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									PPCI8620_STATUS_DMA pDMAStatus); // ��õ�DMA����״̬

	BOOL DEVAPI FAR PASCAL PCI8620_SetDevStatusDmaAD(		// ��AD��������������DMA���й�״̬,����ֵ��ʾ�����Ƿ�ɹ�
									HANDLE hDevice,			// �豸���,����CreateDevice��������
									LONG iClrBufferID);		// Ҫ����Ļ�����ID,������Ϊ0

	BOOL DEVAPI FAR PASCAL PCI8620_StopDeviceDmaAD(			// �������豸֮����ͣ�豸
									HANDLE hDevice);		// �豸������,����CreateDevice��������

	BOOL DEVAPI FAR PASCAL PCI8620_ReleaseDeviceDmaAD(		// �ر�AD�豸,��ֹ����,���ͷ���Դ
									HANDLE hDevice);		// �豸���,����CreateDevice��������
	
	//##################### AD��Ӳ�������������� ###########################
	BOOL DEVAPI FAR PASCAL PCI8620_SaveParaAD(HANDLE hDevice, PPCI8620_PARA_AD pADPara); // ����ǰ��AD��������������ϵͳ��
    BOOL DEVAPI FAR PASCAL PCI8620_LoadParaAD(HANDLE hDevice, PPCI8620_PARA_AD pADPara); // ��AD����������ϵͳ�ж���
    BOOL DEVAPI FAR PASCAL PCI8620_ResetParaAD(HANDLE hDevice, PPCI8620_PARA_AD pADPara); // ��AD���������ָ�������Ĭ��ֵ

	//####################### DA����������� #################################
	// ���ڴ������ͨ�û�����Щ�ӿ���򵥡����ݡ���ɿ������û�����֪���豸
	// �Ͳ㸴�ӵ�Ӳ������Э��ͷ����������Ʊ�̣������������м�������������
	// ��ʵ��DA�������
	BOOL DEVAPI FAR PASCAL PCI8620_WriteDeviceDA(			// дDA����
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									LONG OutputRange,		// ������̣����嶨����ο�����ĳ������岿��
									SHORT nDAData,			// �����DAԭʼ����[0, 4095]
									int nDAChannel);		// DA���ͨ��[0-3]

	//####################### ����I/O����������� #################################
	// �û�����ʹ��WriteRegisterULong��ReadRegisterULong�Ⱥ���ֱ�ӿ��ƼĴ�������I/O
	// �����������ʹ����������������ʡ�£�������Ҫ�����ļĴ��������λ�����ȣ���ֻ
	// ����VB�����Ե����Բ�����ô�򵥵�ʵ�ָ�������ͨ���Ŀ��ơ�
    BOOL DEVAPI FAR PASCAL PCI8620_SetDeviceDO(				// ���������״̬
										HANDLE hDevice,		// �豸���								        
										BYTE bDOSts[16]);	// ����״̬

	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceDI(				// ȡ�ÿ�����״̬     
										HANDLE hDevice,		// �豸���								        
										BYTE bDISts[16]);	// ����״̬	
	
	//##################### ���������ƺ��� ##########################
    BOOL DEVAPI FAR PASCAL PCI8620_SetDeviceCNT(			// ��ʼ��������
									HANDLE	hDevice,		// �豸���
									ULONG	ContrlMode,		// ����������ģʽ
									ULONG	CNTVal,			// ������ֵ
									ULONG	ulChannel);		// ͨ��ѡ��[0-2]			

	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceCNT(			// ȡ�ø�·�������ĵ�ǰ����ֵ
									HANDLE	hDevice,		// �豸������,����CreateDevice��������
									PULONG	pCNTVal,		// ���ؼ���ֵ
									ULONG	ulChannel);		// ͨ��ѡ��[0-2]

	//################# �ڴ�ӳ��Ĵ���ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	BOOL DEVAPI  FAR PASCAL PCI8620_GetDeviceAddr( // ȡ��ָ����ָ���豸ID�ŵ�ӳ��Ĵ��������Ի���ַ�������豸����
												HANDLE hDevice,  // �豸������
												PUCHAR LinearAddr, // ����ָ��ӳ��Ĵ��������Ե�ַ
											    	PUCHAR PhysAddr,   //  ����ָ��ӳ��Ĵ����������ַ
												int RegisterID = 0); // �豸ӳ��Ĵ�����ID�ţ�0-5��
	BOOL DEVAPI FAR PASCAL PCI8620_GetDeviceBar(			// ȡ��ָ����ָ���豸�Ĵ�����BAR��ַ
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									PUCHAR  pbPCIBar[6]);	// ����PCI BAR���е�ַ,����PCI BAR���ж��ٿ��õ�ַ�뿴Ӳ��˵����

	BOOL DEVAPI FAR PASCAL PCI8620_GetDevVersion(			// ��ȡ�豸�̼�������汾
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									PULONG pulFmwVersion,	// �̼��汾
									PULONG pulDriverVersion);// �����汾

    BOOL DEVAPI FAR PASCAL PCI8620_WriteRegisterByte(		// ��ָ���Ĵ����ռ�λ��д�뵥��������
									HANDLE hDevice,			// �豸������,����CreateDevice��������
									PUCHAR pbLinearAddr,	// ָ���Ĵ��������Ի���ַ,������GetDeviceAddr�е�LinearAddr��������ֵ
									ULONG OffsetBytes,		// ��������Ի���ַ����ַ��ƫ��λ��(�ֽ�)
									BYTE Value);			// ��ָ����ַд�뵥�ֽ����ݣ����ַ�����Ի���ַ��ƫ��λ�þ�����

	BOOL DEVAPI FAR PASCAL PCI8620_WriteRegisterWord(		// д˫�ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes,  
									WORD Value);

	BOOL DEVAPI FAR PASCAL PCI8620_WriteRegisterULong(		// д�Ľ������ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes,  
									ULONG Value);

	BYTE DEVAPI FAR PASCAL PCI8620_ReadRegisterByte(		// ���뵥�ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes);

	WORD DEVAPI FAR PASCAL PCI8620_ReadRegisterWord(		// ����˫�ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes);

	ULONG DEVAPI FAR PASCAL PCI8620_ReadRegisterULong(		// �������ֽ����ݣ�����ͬ�ϣ�
									HANDLE hDevice, 
									PUCHAR pbLinearAddr, 
									ULONG OffsetBytes);

	//################# I/O�˿�ֱ�Ӳ�������д���� ########################
	// �������û��Ա��豸��ֱ�ӡ������⡢���Ͳ㡢�����ӵĿ��ơ�������������
	// ���ƶ�����Ҫ����Ŀ������̺Ϳ���Ч��ʱ�����û�����ʹ����Щ�ӿ�����ʵ�֡�
	// ����Щ������Ҫ�����ڴ�ͳ�豸����ISA���ߡ����ڡ����ڵ��豸���������ڱ�PCI�豸
    BOOL DEVAPI FAR PASCAL PCI8620_WritePortByte(			// 8λд�˿�
									HANDLE hDevice,			// �豸���(��CreateDevice����)
									PUCHAR pbPort,			// ָ���Ĵ������������ַ
									ULONG OffsetBytes,		// ������������ַ��ƫ��λ��(�ֽ�)
									BYTE Value);			// д����8λ��������
    BOOL DEVAPI FAR PASCAL PCI8620_WritePortWord(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes, WORD Value);
    BOOL DEVAPI FAR PASCAL PCI8620_WritePortULong(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes, ULONG Value);

    BYTE DEVAPI FAR PASCAL PCI8620_ReadPortByte(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes);
    WORD DEVAPI FAR PASCAL PCI8620_ReadPortWord(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes);
    ULONG DEVAPI FAR PASCAL PCI8620_ReadPortULong(HANDLE hDevice, PUCHAR pbPort, ULONG OffsetBytes);
	// �����Ҫ���û�ģʽ�£�ֱ�ӷ���Ӳ���˿ڣ��밲װ��ʹ��ISA\CommUser�µ�����ReadPortByteEx�Ⱥ���
	//########################### �̲߳������� ######################################
	HANDLE DEVAPI FAR PASCAL PCI8620_CreateSystemEvent(void); // �����ں��¼����󣬹�InitDeviceDmaAD��VB���̵߳Ⱥ���ʹ��
	BOOL DEVAPI FAR PASCAL PCI8620_ReleaseSystemEvent(HANDLE hEvent); // �ͷ��ں��¼�����

#ifdef __cplusplus
}
#endif

#ifndef _FIFO_LENGTH_
#define _FIFO_LENGTH_
//***********************************************************
// ����FIFO�洢���ĳ���(��)
const long FIFO_IDT7202_LENGTH			= 1024;
const long FIFO_IDT7203_LENGTH			= 2048;
const long FIFO_IDT7204_LENGTH			= 4096;
const long FIFO_IDT7205_LENGTH			= 8192;
const long FIFO_IDT7206_LENGTH			= 16384;
const long FIFO_IDT7207_LENGTH			= 32768;

#endif // _FIFO_LENGTH_

// �Զ������������������
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