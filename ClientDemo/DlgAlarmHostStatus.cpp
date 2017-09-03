// DlgAlarmHostStatus.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostStatus dialog

CDlgAlarmHostStatus::CDlgAlarmHostStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAlarmHostStatus)
	m_bSirenStaus = FALSE;
	//}}AFX_DATA_INIT
    memset(&m_struAlarmHostAbility, 0, sizeof(m_struAlarmHostAbility));
    m_iDevIndex = -1;
    m_lServerID = -1;
    m_lStartChan = -1;
    m_lChannel = -1;
    memset(&m_struAlarmMainStatus, 0, sizeof(m_struAlarmMainStatus));
    
    memset(&m_struAlarmInSetup, 0, sizeof(m_struAlarmInSetup));
    m_iPopMenuType = 0;
}


void CDlgAlarmHostStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmHostStatus)
	DDX_Control(pDX, IDC_LIST_MEMORY_STATUS, m_listMemoryStatus);
	DDX_Control(pDX, IDC_LIST_FAULT_STATUS, m_listFaultStatus);
	DDX_Control(pDX, IDC_LIST_SUBSYSTEM, m_listSubSystem);
	DDX_Control(pDX, IDC_LIST_BYPASS_STATUS, m_listBypassStatus);
	DDX_Control(pDX, IDC_LIST_SETUP_ALARMIN_STATUS, m_listSetupAlarmInStatus);
	DDX_Control(pDX, IDC_LIST_ALARMOUT_STATUS, m_listAlarmOutStatus);
	DDX_Control(pDX, IDC_LIST_ALARMIN_STATUS, m_listAlarmInStaus);
	DDX_Check(pDX, IDC_CHK_SIREN_STATUS, m_bSirenStaus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostStatus, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmHostStatus)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_BN_CLICKED(IDC_BTN_BYPASS_ALARMCHAN, OnBtnBypassAlarmchan)
	ON_BN_CLICKED(IDC_BTN_CLOSE_ALARMCHAN, OnBtnCloseAlarmchan)
	ON_BN_CLICKED(IDC_BTN_SETUP_ALARMCHAN, OnBtnSetupAlarmchan)
	ON_BN_CLICKED(IDC_BTN_UN_BYPASS_ALARMCHAN, OnBtnUnBypassAlarmchan)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SETUP_ALARMIN_STATUS, OnRclickListSetupAlarminStatus)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BYPASS_STATUS, OnRclickListBypassStatus)
	ON_COMMAND(ID_MENU_SELECT_ALL, OnMenuSelectAll)
	ON_COMMAND(ID_MENU_CANCEL_ALL, OnMenuCancelAll)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostStatus message handlers

BOOL CDlgAlarmHostStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    InitWnd();
    GetAlarmHostMainStatus(m_struAlarmMainStatus);
    SetAlarmHostMainStatusToWnd(m_struAlarmMainStatus);
    GetAlarmHostOtherStatus(m_struAlarmOtherStatus);
    SetAlarmHostOtherStatusToWnd(m_struAlarmOtherStatus);
	SetTimer(NULL, 500, NULL);
    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgAlarmHostStatus::InitWnd()
{
    char szLan[128] = {0};
    m_listSetupAlarmInStatus.SetExtendedStyle(LVS_EX_CHECKBOXES); 
	int i = 0;
    for ( i = 0; i < (m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum); i++)
    {
        sprintf(szLan, "Alarm In %d", (i+1));
        m_listSetupAlarmInStatus.InsertItem(i, szLan);
    }
    
    m_listAlarmOutStatus.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (i = 0; i < (m_struAlarmHostAbility.wLocalAlarmOutNum + m_struAlarmHostAbility.wExpandAlarmOutNum); i++)
    {
        sprintf(szLan, "Alarm out %d", (i+1));
        m_listAlarmOutStatus.InsertItem(i, szLan);
    }

    m_listAlarmInStaus.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (i = 0; i < (m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum); i++)
    {
        sprintf(szLan, "Alarm In %d", (i+1));
        m_listAlarmInStaus.InsertItem(i, szLan);
    }

    m_listBypassStatus.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (i = 0; i < (m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum); i++)
    {
        sprintf(szLan, "Alarm In %d", (i+1));
        m_listBypassStatus.InsertItem(i, szLan);
    }

	m_listSubSystem.SetExtendedStyle(LVS_EX_CHECKBOXES);
	for (i=0; i<m_struAlarmHostAbility.wSubSystemNum; i++)
	{
		sprintf(szLan, "Subsystem %d", (i+1));
		m_listSubSystem.InsertItem(i, szLan);
	}

	//故障状态
    m_listFaultStatus.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (i = 0; i < (m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum); i++)
    {
        sprintf(szLan, "Alarm In %d", (i+1));
        m_listFaultStatus.InsertItem(i, szLan);
    }

	m_listMemoryStatus.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (i = 0; i < (m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum); i++)
    {
        sprintf(szLan, "Alarm In %d", (i+1));
        m_listMemoryStatus.InsertItem(i, szLan);
    }
}

BOOL CDlgAlarmHostStatus::GetAlarmHostMainStatus(NET_DVR_ALARMHOST_MAIN_STATUS_V40& struAlarmMainStatus)
{
    BOOL bRet = FALSE;
    DWORD dwReturn = 0;
    
    if (!NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_ALARMHOST_MAIN_STATUS_V40, 0, &struAlarmMainStatus, sizeof(struAlarmMainStatus), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_ALARMHOST_MAIN_STATUS_V40");
        bRet = FALSE;
    } 
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_ALARMHOST_MAIN_STATUS_V40");
        bRet = TRUE;
    }
    return bRet;
}

BOOL CDlgAlarmHostStatus::GetAlarmHostOtherStatus(NET_DVR_ALARMHOST_OTHER_STATUS& struAlarmOtherStatus)
{
    BOOL bRet = FALSE;
    DWORD dwReturn = 0;
    
    if (!NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_ALARMHOST_OTHER_STATUS, 0, &struAlarmOtherStatus, sizeof(struAlarmOtherStatus), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_ALARMHOST_OTHER_STATUS");
        bRet = FALSE;
    } 
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_ALARMHOST_OTHER_STATUS");
        bRet = TRUE;
    }
    return bRet;
}

void CDlgAlarmHostStatus::SetAlarmHostMainStatusToWnd(NET_DVR_ALARMHOST_MAIN_STATUS_V40& struAlarmMainStatus)
{
	int i = 0;
    for (i = 0; i < m_listAlarmInStaus.GetItemCount(); i++ )
    {
        if (1 ==  struAlarmMainStatus.byAlarmInStatus[i])
        {
            m_listAlarmInStaus.SetCheck(i, TRUE);
        }
        else
        {
            m_listAlarmInStaus.SetCheck(i, FALSE);
        }
    }

    for ( i = 0; i < m_listAlarmOutStatus.GetItemCount(); i++ )
    {
        if (1 == struAlarmMainStatus.byAlarmOutStatus[i])
        {
            m_listAlarmOutStatus.SetCheck(i, TRUE);
        }
        else
        {
            m_listAlarmOutStatus.SetCheck(i, FALSE);
        }
    }

    for ( i = 0; i < m_listSetupAlarmInStatus.GetItemCount(); i++ )
    {
        if (1 == struAlarmMainStatus.bySetupAlarmStatus[i])
        {
            m_listSetupAlarmInStatus.SetCheck(i, TRUE);
        }
        else
        {
            m_listSetupAlarmInStatus.SetCheck(i, FALSE);
        }
    }
   
    for ( i = 0; i < m_listBypassStatus.GetItemCount(); i++ )
    {
        if (1 == struAlarmMainStatus.byBypassStatus[i])
        {
            m_listBypassStatus.SetCheck(i, TRUE);
        }
        else
        {
            m_listBypassStatus.SetCheck(i, FALSE);
        }
    }

	for (i=0; i<m_listSubSystem.GetItemCount(); i++)
	{
		if (1 == struAlarmMainStatus.bySubSystemGuardStatus[i])
		{
			m_listSubSystem.SetCheck(i, TRUE);
		}
		else
		{
			m_listSubSystem.SetCheck(i, FALSE);
		}
	}

	//故障状态
	for (i=0; i<m_listFaultStatus.GetItemCount(); i++)
	{
		m_listFaultStatus.SetCheck(i, (BOOL)struAlarmMainStatus.byAlarmInFaultStatus[i]);
	}

	for (i=0; i<m_listMemoryStatus.GetItemCount(); i++)
	{
		if (1 == struAlarmMainStatus.byAlarmInMemoryStatus[i])
		{
			m_listMemoryStatus.SetCheck(i, TRUE);
		}
		else
		{
			m_listMemoryStatus.SetCheck(i, FALSE);
		}
	}
	UpdateData(FALSE);
}

void CDlgAlarmHostStatus::SetAlarmHostOtherStatusToWnd(NET_DVR_ALARMHOST_OTHER_STATUS& struAlarmOtherStatus)
{
    m_bSirenStaus = struAlarmOtherStatus.bySirenStatus[0]; // 目前只有一个
	UpdateData(FALSE);
}

void CDlgAlarmHostStatus::GetSetupAlarmChanFromWnd(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
{
	UpdateData(TRUE);
    for (int i = 0; i < m_listSetupAlarmInStatus.GetItemCount(); i++)
    {
        if (TRUE == m_listSetupAlarmInStatus.GetCheck(i))
        {
            struAlarmInSetup.byAssiciateAlarmIn[i] = 1;
        }
        else
        {
            struAlarmInSetup.byAssiciateAlarmIn[i] = 0;
        }
    }
}

void CDlgAlarmHostStatus::GetBypassAlarmChanFromWnd(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
{
	UpdateData(TRUE);
    for (int i = 0; i < m_listBypassStatus.GetItemCount(); i++)
    {
        if (TRUE == m_listBypassStatus.GetCheck(i))
        {
            struAlarmInSetup.byAssiciateAlarmIn[i] = 1;
        }
        else
        {
            struAlarmInSetup.byAssiciateAlarmIn[i] = 0;
        }
    }
}

void CDlgAlarmHostStatus::OnBtnRefresh() 
{
    GetAlarmHostMainStatus(m_struAlarmMainStatus);
    SetAlarmHostMainStatusToWnd(m_struAlarmMainStatus);
	GetAlarmHostOtherStatus(m_struAlarmOtherStatus);
    SetAlarmHostOtherStatusToWnd(m_struAlarmOtherStatus);
//	OnInitDialog();
    UpdateData(FALSE);
}

void CDlgAlarmHostStatus::OnBtnBypassAlarmchan() 
{
    GetBypassAlarmChanFromWnd(m_struAlarmInSetup);
    BypassAlarmChan(m_struAlarmInSetup);
}

void CDlgAlarmHostStatus::OnBtnCloseAlarmchan() 
{
    GetSetupAlarmChanFromWnd(m_struAlarmInSetup);
    AlarmHostCloseAlarmChan(m_struAlarmInSetup);
	
}

void CDlgAlarmHostStatus::OnBtnSetupAlarmchan() 
{
    GetSetupAlarmChanFromWnd(m_struAlarmInSetup);
    AlarmHostSetAlarmChan(m_struAlarmInSetup);
}

void CDlgAlarmHostStatus::OnBtnUnBypassAlarmchan() 
{
    GetBypassAlarmChanFromWnd(m_struAlarmInSetup);
    UnBypassAlarmChan(m_struAlarmInSetup);
}


BOOL CDlgAlarmHostStatus::AlarmHostSetAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
{
    BOOL bRet = FALSE;
    
    if (!NET_DVR_AlarmHostSetupAlarmChan(m_lServerID, &struAlarmInSetup))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AlarmHostSetupAlarmChan");
        bRet =FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AlarmHostSetupAlarmChan");
        bRet = TRUE;
    }
    return bRet;
}

BOOL CDlgAlarmHostStatus::AlarmHostCloseAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
{
    BOOL bRet = FALSE;
    if (!NET_DVR_AlarmHostCloseAlarmChan(m_lServerID, &struAlarmInSetup))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AlarmHostCloseAlarmChan");
        bRet =FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AlarmHostCloseAlarmChan");
        bRet = TRUE;
    }
    return bRet;
}

BOOL CDlgAlarmHostStatus::BypassAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
{
    if (NET_DVR_BypassAlarmChan(m_lServerID, &struAlarmInSetup))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_BypassAlarmChan");
        return TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_BypassAlarmChan");
        return FALSE;
    }

}

BOOL CDlgAlarmHostStatus::UnBypassAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
{
    if (NET_DVR_UnBypassAlarmChan(m_lServerID, &struAlarmInSetup))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_UnBypassAlarmChan");
        return TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_UnBypassAlarmChan");
        return FALSE;
    }
}

void CDlgAlarmHostStatus::OnRclickListSetupAlarminStatus(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CMenu pMenu;
    
    if (!pMenu.LoadMenu(IDR_MENU_SELECT_MODE))
    {
        return;
    }
    CPoint point;
    GetCursorPos(&point);
    m_iPopMenuType = 1;
    pMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
    *pResult = 0;
}

void CDlgAlarmHostStatus::OnRclickListBypassStatus(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CMenu pMenu;
    
    if (!pMenu.LoadMenu(IDR_MENU_SELECT_MODE))
    {
        return;
    }
    CPoint point;
    GetCursorPos(&point);
    m_iPopMenuType = 2;
    pMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
    *pResult = 0;
}

void CDlgAlarmHostStatus::OnMenuSelectAll() 
{
	if (1 == m_iPopMenuType)
	{
        for (int i = 0; i < m_listSetupAlarmInStatus.GetItemCount(); i++)
        {
            m_listSetupAlarmInStatus.SetCheck(i, TRUE);
        }
	}
    else if(2 == m_iPopMenuType)
    {
        for (int i = 0; i < m_listBypassStatus.GetItemCount(); i++)
        {
            m_listBypassStatus.SetCheck(i, TRUE);
        }
    }
}

void CDlgAlarmHostStatus::OnMenuCancelAll() 
{
    if (1 == m_iPopMenuType)
    {
        for (int i = 0; i < m_listSetupAlarmInStatus.GetItemCount(); i++)
        {
            m_listSetupAlarmInStatus.SetCheck(i, FALSE);
        }
    }
    else if(2 == m_iPopMenuType)
    {
        for (int i = 0; i < m_listBypassStatus.GetItemCount(); i++)
        {
            m_listBypassStatus.SetCheck(i, FALSE);
        }
    }
	
}

#if (_MSC_VER >= 1500)	//vs2008
void CDlgAlarmHostStatus::OnTimer(UINT_PTR nIDEvent) 
#else
void CDlgAlarmHostStatus::OnTimer(UINT nIDEvent) 
#endif
{
	// TODO: Add your message handler code here and/or call default
//	OnBtnRefresh();
	CDialog::OnTimer(nIDEvent);
}
