// DlgXmlVideoPic.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgXmlVideoPic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgXmlVideoPic dialog


CDlgXmlVideoPic::CDlgXmlVideoPic(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgXmlVideoPic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgXmlVideoPic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lServerID = -1;
    m_iDevIndex = -1;
    m_pRecvBuf = NULL;
    m_pSendBuf = NULL;
}


void CDlgXmlVideoPic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgXmlVideoPic)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgXmlVideoPic, CDialog)
	//{{AFX_MSG_MAP(CDlgXmlVideoPic)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgXmlVideoPic message handlers
BOOL CDlgXmlVideoPic::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    if (NULL == m_pRecvBuf)
    {
        m_pRecvBuf = new char[XML_BUF];
		memset(m_pRecvBuf, 0, XML_BUF);
    }
    
    if (NULL == m_pSendBuf)
    {
        m_pSendBuf = new char[XML_BUF];
		memset(m_pSendBuf, 0, XML_BUF);
    }
	//////////////////////////////////////////////////////////////////////////
	//	GetCurrentEncodeAbility();
	//	WirteBufToFile(m_pRecvBuf,strlen(m_pRecvBuf));
	//////////////////////////////////////////////////////////////////////////
    GetVideoPicAbility();
	
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgXmlVideoPic::GetVideoPicAbility()
{
    memset(m_pRecvBuf, 0, XML_BUF);
	//	strcpy(m_pSendBuf,"<AudioVideoCompressInfo><ChannelNumber>17</ChannelNumber></AudioVideoCompressInfo>");
    strcpy(m_pSendBuf,"<VideoPicAbility version=\"2.0\"><channelNO>1</channelNO></VideoPicAbility>");
	
    if (!NET_DVR_GetDeviceAbility(m_lServerID, DEVICE_VIDEOPIC_ABILITY, m_pSendBuf, strlen(m_pSendBuf), m_pRecvBuf, XML_BUF))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "DEVICE_VIDEOPIC_ABILITY");
        return FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "DEVICE_VIDEOPIC_ABILITY");
    }
	
	WirteBufToFile(m_pRecvBuf, strlen(m_pRecvBuf));
	
	return TRUE;
}

BOOL CDlgXmlVideoPic::WirteBufToFile(char *pBuf, DWORD dwBufSize)
{
    SYSTEMTIME t;
    GetLocalTime(&t);
    char chTime[128];
    char cFilename[256] = {0};
    sprintf(chTime,"%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d",t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
    
    sprintf(cFilename, "%s\\VideoPic_%s.xml", g_struLocalParam.chPictureSavePath, chTime);
    
    
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

void CDlgXmlVideoPic::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pSendBuf != NULL)
    {
        delete []m_pSendBuf;
        m_pSendBuf = NULL;
    }
	
    if (m_pRecvBuf != NULL)
    {
        delete []m_pRecvBuf;
        m_pRecvBuf = NULL;
    }
}
