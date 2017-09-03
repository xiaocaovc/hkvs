// DlgXmlAbility.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgXmlAbility.h"
#include "DlgSoftHardAbility.h"
#include "DlgEncodeAbility.h"
#include "DlgNetworkAbility.h"
#include "DlgIPCFrontAbility.h"
#include "DlgCurrentEncodeAbility.h"
#include "DlgDynChanAbility.h"
#include "DlgXmlItcAbility.h"
#include "XmlUserAbility.h"
#include "DlgXmlJpegCapAbility.h"
#include "DlgXmlNetAppAbility.h"
#include "DlgXmlVideoPic.h"
#include "DlgXmlSerialAbility.h"
#include "DlgXmlAlarmAbility.h"
#include "DlgXmlDecoderAbility.h"
#include "DlgXmlRaidAbility.h"
#include "DlgXmlWallAblity.h"
#include "DlgChanInputAbility.h"
#include "DlgRecordAbility.h"
#include "DlgRemoteControlAbility.h"
#include "DlgXmlEventAbility.h"
#include "DlgXmlLCDAbility.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgXmlAbility dialog

CDlgXmlAbility::CDlgXmlAbility(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgXmlAbility::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgXmlAbility)
	m_dwVideoChan = 0;
	//}}AFX_DATA_INIT
    m_lServerID = -1;
    m_iDevIndex = -1;
}

CDlgXmlAbility::~CDlgXmlAbility()
{
	if (NULL != m_pRecvBuf)
	{
		delete[] m_pRecvBuf;
	}
}

void CDlgXmlAbility::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgXmlAbility)
	DDX_Text(pDX, IDC_EDIT_ENCODE_VIDEO_CHAN, m_dwVideoChan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgXmlAbility, CDialog)
	//{{AFX_MSG_MAP(CDlgXmlAbility)
	ON_BN_CLICKED(IDC_BTN_SOFTHARD_ABILITY, OnBtnSofthardAbility)
	ON_BN_CLICKED(IDC_BTN_ENCODE_ABILITY, OnBtnEncodeAbility)
	ON_BN_CLICKED(IDC_BTN_NETWORK, OnBtnNetwork)
	ON_BN_CLICKED(IDC_BTN_IPC_FRONT_ABILITY, OnBtnIpcFrontAbility)
	ON_BN_CLICKED(IDC_BTN_DYNCHAN_ABILITY, OnBtnDynchanAbility)
	ON_BN_CLICKED(IDC_BTN_ITC_ITS_ABILITY, OnBtnItcItsAbility)
	ON_BN_CLICKED(IDC_BTN_USER_ABILITY, OnBtnUserAbility)
	ON_BN_CLICKED(IDC_BTN_JPEG_CAP_ABILITY, OnBtnJpegCapAbility)
	ON_BN_CLICKED(IDC_BTN_NETAPP_ABILITY, OnBtnNetappAbility)
	ON_BN_CLICKED(IDC_BTN_VIDEO_PIC_ABILITY, OnBtnVideoPicAbility)
	ON_BN_CLICKED(IDC_BTN_SERIAL_ABILITY, OnBtnSerialAbility)
	ON_BN_CLICKED(IDC_BTN_ALARM_ABILITY, OnBtnAlarmAbility)
	ON_BN_CLICKED(IDC_BTN_DECODER_ABILITY, OnBtnDecoderAbility)
	ON_BN_CLICKED(IDC_BTN_RAID_ABILITY, OnBtnRaidAbility)
	ON_BN_CLICKED(IDC_BTN_WALL_ABILITY, OnBtnWallAbility)
	ON_BN_CLICKED(IDC_BTN_CHAN_INPUT_ABILITY, OnBtnChanInputAbility)
	ON_BN_CLICKED(IDC_BTN_RECORD_ABILITY, OnBtnRecordAbility)
	ON_BN_CLICKED(IDC_BTN_REMOTE_CONTROL_ABILITY, OnBtnRemoteControlAbility)
	ON_BN_CLICKED(IDC_BTN_EVENT_ABILITY, OnBtnEventAbility)
	ON_BN_CLICKED(IDC_BTN_ALARMHOST, OnBtnAlarmhost)
	ON_BN_CLICKED(IDC_BTN_LCD, OnBtnLcd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgXmlAbility message handlers

void CDlgXmlAbility::OnBtnSofthardAbility() 
{
    CDlgSoftHardAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}


BOOL CDlgXmlAbility::OnInitDialog() 
{
	CDialog::OnInitDialog();

     m_pRecvBuf = new char[XML_BUF]; // 3M数据接收XML缓冲区
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgXmlAbility::OnBtnEncodeAbility() 
{
//     GetEncodeAbility();	
//     ParseEncodeAbility(m_pRecvBuf, strlen(m_pRecvBuf));
	UpdateData(TRUE);
    CDlgEncodeAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
	dlg.m_dwVideoChan = m_dwVideoChan;
    dlg.DoModal();
}



void CDlgXmlAbility::OnBtnNetwork() 
{
    CDlgNetworkAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}


void CDlgXmlAbility::OnBtnIpcFrontAbility() 
{
    CDlgIPCFrontAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnDynchanAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgDynChanAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnItcItsAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlItcAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnUserAbility() 
{
	// TODO: Add your control notification handler code here
	CXmlUserAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnJpegCapAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlJpegCapAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnNetappAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlNetAppAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnVideoPicAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlVideoPic dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnSerialAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlSerialAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnAlarmAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlAlarmAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnDecoderAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlDecoderAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnRaidAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlRaidAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnWallAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlWallAblity dlg;
	dlg.m_lServerID = m_lServerID;
	dlg.DoModal();
}

void CDlgXmlAbility::OnBtnChanInputAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgChanInputAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnRecordAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgRecordAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnRemoteControlAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgRemoteControlAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnEventAbility() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlEventAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CDlgXmlAbility::OnBtnAlarmhost() 
{
	// TODO: Add your control notification handler code here
	char sSendBuf[1024] = "<AlarmHostAbility version=\"2.0\"></AlarmHostAbility>";

	GetDeviceAbility(DEVICE_ABILITY_INFO, sSendBuf, strlen(sSendBuf));
}


/****************************************************
函数名:     GetDeviceAbility
函数描述:   获取指定节点的xml能力
输入参数:   
dwCommand:	能力集命令
sSendBuf：	发送缓冲区
dwBufLen：  发送缓冲区长度
返回值:  
TRUE表示成功, FALSE表示失败 
*****************************************************/
BOOL CDlgXmlAbility::GetDeviceAbility(DWORD dwCommand, char* sSendBuf, DWORD dwBufLen) 
{
	if (NET_DVR_GetDeviceAbility(m_lServerID, dwCommand, sSendBuf, dwBufLen, m_pRecvBuf, XML_BUF))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "Get ability successful");
		MessageBox("获取成功");
		WirteBufToFile(m_pRecvBuf, strlen(m_pRecvBuf));
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "Get ability failed");
		MessageBox("获取失败");
		return FALSE;
    }
	return TRUE;
}

/****************************************************
函数名:     WirteBufToFile
函数描述:   将缓冲区中的能力集数据写成XML文件
输入参数:   
pBuf：缓冲区指针
dwBufSize：缓冲区大小
返回值:  
TRUE表示成功, FALSE表示失败 
*****************************************************/
BOOL CDlgXmlAbility::WirteBufToFile(char *pBuf, DWORD dwBufSize)
{
	SYSTEMTIME t;
    GetLocalTime(&t);
    char chTime[128];
    char cFilename[256] = {0};
    sprintf(chTime,"%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
    
    sprintf(cFilename, "%s\\AlarmHostAbiltiy_%s.xml", g_struLocalParam.chPictureSavePath, chTime);
    
    
    HANDLE hFile = CreateFile(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        int nError = 0;
        nError = GetLastError();
        return FALSE;
    }
    DWORD dwReturn = 0;
    WriteFile(hFile, pBuf, dwBufSize, &dwReturn, NULL);
    CloseHandle(hFile);
    hFile = NULL;
    
    return TRUE;
}

void CDlgXmlAbility::OnBtnLcd() 
{
	// TODO: Add your control notification handler code here
	CDlgXmlLCDAbility dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}
