// DlgLinkStatus.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgLinkStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLinkStatus dialog


CDlgLinkStatus::CDlgLinkStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLinkStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLinkStatus)
	//}}AFX_DATA_INIT
    m_lServerID = -1;
    m_iDevIndex = -1;
    memset(&m_struLinkStatus, 0, sizeof(m_struLinkStatus));
}


void CDlgLinkStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLinkStatus)
	DDX_Control(pDX, IDC_LIST_LINK_STATUS, m_listLinkStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLinkStatus, CDialog)
	//{{AFX_MSG_MAP(CDlgLinkStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLinkStatus message handlers

BOOL CDlgLinkStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    InitWnd();
    
    if (!GetLinkStatus(m_struLinkStatus))
    {
        AfxMessageBox("Fai to get Link status");
    //    return;
    }
	
    SetLinkStatusToWnd(m_struLinkStatus);
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgLinkStatus::GetLinkStatus(NET_DVR_LINK_STATUS &struLinkStatus)
{
    DWORD dwReturn = 0;
    if (NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_LINK_STATUS, 0, &struLinkStatus, sizeof(struLinkStatus), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_LINK_STATUS");
        return TRUE;
    } 
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_LINK_STATUS");
        return FALSE;
    }
}

void CDlgLinkStatus::InitWnd()
{
    int nIndex = 0;
    char szLan[128] = {0};
    g_StringLanType(szLan, "索引", "Index");
    m_listLinkStatus.InsertColumn(nIndex, szLan, LVCFMT_LEFT, 70, -1);
    nIndex++;

    g_StringLanType(szLan, "通道", "Channel");
    m_listLinkStatus.InsertColumn(nIndex, szLan, LVCFMT_LEFT, 70, -1);
    nIndex++;

    g_StringLanType(szLan, "客户端IP", "Client IP");
    m_listLinkStatus.InsertColumn(nIndex, szLan, LVCFMT_LEFT, 300, -1);
    nIndex++;

    m_listLinkStatus.SetExtendedStyle(m_listLinkStatus.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
}

void CDlgLinkStatus::SetLinkStatusToWnd(NET_DVR_LINK_STATUS &struLinkStatus)
{
    int i = 0;
    char szLan[128] = {0};
    int nSubIndex = 0;
    for (i = 0; i < struLinkStatus.wLinkNum; i++)
    {
        nSubIndex = 0;
        sprintf(szLan, "%d", i + 1);   
        m_listLinkStatus.InsertItem(i, szLan);
        nSubIndex++;
        
        sprintf(szLan, "%d", struLinkStatus.struOneLink[i].lChannel);
        m_listLinkStatus.SetItemText(i,nSubIndex, szLan);
        nSubIndex++;

        sprintf(szLan, "%s", struLinkStatus.struOneLink[i].struIP.sIpV4);
        m_listLinkStatus.SetItemText(i,nSubIndex, szLan);
        nSubIndex++;
    }
}
