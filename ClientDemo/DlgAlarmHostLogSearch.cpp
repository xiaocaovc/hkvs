// DlgAlarmHostLogSearch.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostLogSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostLogSearch dialog
CDlgAlarmHostLogSearch * g_pLogSearch = NULL;

UINT GetAlarmHostLogThread(LPVOID pParam)
{
    CString csTmp;
    char szLan[128] = {0};
    LONG lRet = -1;
    NET_DVR_ALARMHOST_LOG_RET struAlarmHostLog = {0};
    
    while (1) 
    {
        lRet = NET_DVR_FindNextAlarmHostLog(g_pLogSearch->m_lSearchHandle, &struAlarmHostLog);
        if (lRet == NET_DVR_FILE_SUCCESS)
        {
            int nIndex = 0;
            csTmp.Format("%d", g_pLogSearch->m_lLogNum+1);
            g_pLogSearch->m_listAlarmHostLog.InsertItem(g_pLogSearch->m_lLogNum, csTmp,nIndex);
            nIndex++;
            csTmp.Format("%04d-%02d-%02d %02d:%02d:%02d", struAlarmHostLog.struLogTime.dwYear, struAlarmHostLog.struLogTime.dwMonth, struAlarmHostLog.struLogTime.dwDay, \
                struAlarmHostLog.struLogTime.dwHour,struAlarmHostLog.struLogTime.dwMinute, struAlarmHostLog.struLogTime.dwSecond);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);	
            nIndex++;
            
            char szTmp[40] = {0};
            memcpy(szTmp, struAlarmHostLog.sUserName, sizeof(struAlarmHostLog.sUserName));
            csTmp.Format("%s", szTmp);
            
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);	
            nIndex++;
            
            g_pLogSearch->InfoMajorTypeMap(struAlarmHostLog.wMajorType, csTmp);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
            nIndex++;

            if (MAJOR_ALARMHOST_ALARM == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorAlarmMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }
            else if (MAJOR_ALARMHOST_EXCEPTION == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorExceptionMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }
            else if (MAJOR_ALARMHOST_OPERATION == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorOperationMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }
            else if (MAJ0R_ALARMHOST_EVENT == struAlarmHostLog.wMajorType)
            {
                g_pLogSearch->InfoMinorEventMap(struAlarmHostLog.wMinorType, csTmp);
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
                nIndex++;
            }

            csTmp.Format("%d", struAlarmHostLog.wParam);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
            nIndex++;
            
            csTmp.Format("%s",struAlarmHostLog.struIPAddr.sIpV4);
            g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, csTmp);
            nIndex++;
            
            if (struAlarmHostLog.dwInfoLen > 0)
            {
                g_pLogSearch->m_listAlarmHostLog.SetItemText(g_pLogSearch->m_lLogNum, nIndex, struAlarmHostLog.sInfo);
                nIndex++;
            }
            
            g_pLogSearch->m_lLogNum++;
        }
        else
        {
            if (lRet == NET_DVR_ISFINDING)
            {
                g_pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_SHOW);
				g_StringLanType(szLan, "日志搜索中......", "Searching...");
                g_pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText(szLan);
                Sleep(5);
                continue;
            }
            if ((lRet == NET_DVR_NOMOREFILE) || (lRet == NET_DVR_FILE_NOFIND))
            {
                g_StringLanType(szLan, "搜索日志", "Search Log");
                g_pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
                g_pLogSearch->m_bSearch = FALSE;
                g_pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_HIDE);
                g_StringLanType(szLan, "搜索日志结束!", "Search log Ending");
                AfxMessageBox(szLan);
                break;
            }
            else
            {
                g_StringLanType(szLan, "搜索日志", "Search Log");
                g_pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
                g_pLogSearch->m_bSearch = FALSE;
                g_StringLanType(szLan, "由于服务器忙,或网络故障,搜索日志异常终止!",\
                    "Since the server is busy, or network failure, search the log abnormal termination");
                AfxMessageBox(szLan);
                break;
            }
        }
    }
    CloseHandle(g_pLogSearch->m_hFileThread);
    g_pLogSearch->m_hFileThread = NULL;
    NET_DVR_FindAlarmHostLogClose(g_pLogSearch->m_lSearchHandle);
    return 0;
}

CDlgAlarmHostLogSearch::CDlgAlarmHostLogSearch(CWnd* pParent /*=NULL*/)
: CDialog(CDlgAlarmHostLogSearch::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDlgAlarmHostLogSearch)
    m_ctDataStart = 0;
    m_ctDataStop = 0;
    m_ctTimeStart = 0;
    m_ctTimeStop = 0;
    //}}AFX_DATA_INIT
    m_lSearchHandle = -1;
    m_iDevIndex = -1;
    memset(&m_struSearchParam, 0, sizeof(m_struSearchParam));
    m_hFileThread = NULL;
    m_lLogNum = 0;
    m_bSearch = FALSE;
}


void CDlgAlarmHostLogSearch::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgAlarmHostLogSearch)
    DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
    DDX_Control(pDX, IDC_LIST_ALARM_HOST_LOG, m_listAlarmHostLog);
    DDX_Control(pDX, IDC_COMBO_MINOR_TYPE, m_comboMinorType);
    DDX_Control(pDX, IDC_COMBO_MAJOR_TYPE, m_comboMajorType);
    DDX_DateTimeCtrl(pDX, IDC_DATE_LOG_START, m_ctDataStart);
    DDX_DateTimeCtrl(pDX, IDC_DATE_LOG_STOP, m_ctDataStop);
    DDX_DateTimeCtrl(pDX, IDC_TIME_LOG_START, m_ctTimeStart);
    DDX_DateTimeCtrl(pDX, IDC_TIME_LOG_STOP, m_ctTimeStop);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostLogSearch, CDialog)
//{{AFX_MSG_MAP(CDlgAlarmHostLogSearch)
ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
ON_CBN_SELCHANGE(IDC_COMBO_MAJOR_TYPE, OnSelchangeComboMajorType)
	ON_BN_CLICKED(IDC_BTN_EXPORT_LOG, OnBtnExportLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostLogSearch message handlers

BOOL CDlgAlarmHostLogSearch::OnInitDialog() 
{
    CDialog::OnInitDialog();
    //todo!!!
    CTime ctCurTime = CTime::GetCurrentTime();
    CTime time(ctCurTime.GetYear(),ctCurTime.GetMonth(),ctCurTime.GetDay(),0,0,0);
    CTime time0(ctCurTime.GetYear(),ctCurTime.GetMonth(),ctCurTime.GetDay(),23,59,59);
    m_ctDataStart = time;
    m_ctTimeStart = time;
    m_ctDataStop = ctCurTime;
    m_ctTimeStop = time0;
    
    
    g_pLogSearch = this;
    
    InitWnd();
    
    m_comboMode.SetCurSel(2); // 默认按时间查找
    m_comboMajorType.SetCurSel(0);
    OnSelchangeComboMajorType();
    
    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmHostLogSearch::InitWnd()
{
    InitModeCombo();
    InitMajorTypeCombo();
    InitAlarmHostLogList();
}

void CDlgAlarmHostLogSearch::InitAlarmHostLogList()
{
    char szLan[128] = {0};
    m_listAlarmHostLog.SetExtendedStyle(m_listAlarmHostLog.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLan, "序号", "No.");
    m_listAlarmHostLog.InsertColumn(0, szLan,LVCFMT_RIGHT, 40, -1);
    g_StringLanType(szLan, "日志时间", "Log Time");
    m_listAlarmHostLog.InsertColumn(1, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "操作用户", "Operator user");
    m_listAlarmHostLog.InsertColumn(2, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "主类型", "Major Type");
    m_listAlarmHostLog.InsertColumn(3, szLan, LVCFMT_LEFT,80, -1);
    g_StringLanType(szLan, "次类型", "Minor Type");
    m_listAlarmHostLog.InsertColumn(4, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "参数", "Param");
    m_listAlarmHostLog.InsertColumn(5, szLan, LVCFMT_LEFT, 50, -1);
    g_StringLanType(szLan, "远程主机", "Remote Host");
    m_listAlarmHostLog.InsertColumn(6, szLan, LVCFMT_LEFT, 80, -1);
    g_StringLanType(szLan, "描述", "Description");
    m_listAlarmHostLog.InsertColumn(7, szLan, LVCFMT_LEFT, 251, -1);
    
    
    //     m_listDeviceLog.SetExtendedStyle(m_listDeviceLog.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    //     g_StringLanType(szLan, "序号", "No.");
    //     m_listDeviceLog.InsertColumn(0, szLan, LVCFMT_RIGHT, 60, -1);
    //     g_StringLanType(szLan, "日志时间", "Log Time");
    //     m_listDeviceLog.InsertColumn(1, szLan, LVCFMT_LEFT, 140, -1);
    //     g_StringLanType(szLan, "主类型", "Major Type");
    //     m_listDeviceLog.InsertColumn(2, szLan, LVCFMT_LEFT,80, -1);
    //     g_StringLanType(szLan, "次类型", "Minor Type");
    //     m_listDeviceLog.InsertColumn(3, szLan, LVCFMT_LEFT, 150, -1);
    //     g_StringLanType(szLan, "远程主机", "Remote Host");
    //     m_listDeviceLog.InsertColumn(4, szLan, LVCFMT_LEFT, 100, -1);
    //     g_StringLanType(szLan, "描述", "Description");
    //     m_listDeviceLog.InsertColumn(5, szLan, LVCFMT_LEFT, 251, -1);
    
}


void CDlgAlarmHostLogSearch::InitModeCombo()
{
    char szLan[128] = {0};
    m_comboMode.ResetContent();
    g_StringLanType(szLan, "全部", "All");
    m_comboMode.AddString(szLan);
    g_StringLanType(szLan, "按类型", "By type");
    m_comboMode.AddString(szLan);
    g_StringLanType(szLan, "按时间", "By time");
    m_comboMode.AddString(szLan);
    g_StringLanType(szLan, "按时间和类型", "By type & time");
    m_comboMode.AddString(szLan);
}

void CDlgAlarmHostLogSearch::InitMajorTypeCombo()
{
    char szLan[128] = {0};
    m_comboMajorType.ResetContent();
    g_StringLanType(szLan, "全部", "All");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "报警", "Alarm");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "异常", "Abnormal");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "操作", "Operation");
    m_comboMajorType.AddString(szLan);
    g_StringLanType(szLan, "事件", "Event");
    m_comboMajorType.AddString(szLan);       
}

void CDlgAlarmHostLogSearch::InitMinorTypeCombo()
{
    int iMajorType = m_comboMajorType.GetCurSel();
    char szLan[128] = {0};
    if (0 == iMajorType)
    {
        int nIndex = 0;
        m_comboMinorType.ResetContent();
        m_comboMinorType.InsertString(nIndex, "All");
        m_comboMinorType.SetItemData(nIndex, 0);
        nIndex++;
    }
    else if (1 == iMajorType)
    {
        InitMinorAlarmCombo();
        
    }
    else if (2 == iMajorType)
    {
        InitMinorExcepCombo();
    }
    else if (3 == iMajorType)
    {
        InitMinorOperatorCombo();
    }
    else if (4 == iMajorType)
    {
        InitMinorEventCombo();
    }
}

void CDlgAlarmHostLogSearch::InitMinorAlarmCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
    m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SHORT_CIRCUIT");
    m_comboMinorType.SetItemData(nIndex, MINOR_SHORT_CIRCUIT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BROKEN_CIRCUIT");
    m_comboMinorType.SetItemData(nIndex, MINOR_BROKEN_CIRCUIT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_NORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_NORMAL);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_PASSWORD_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_PASSWORD_ERROR);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ID_CARD_ILLEGALLY");
    m_comboMinorType.SetItemData(nIndex, MINOR_ID_CARD_ILLEGALLY);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_REMOVE_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_REMOVE_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEV_REMOVE");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEV_REMOVE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEV_REMOVE_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEV_REMOVE_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT1");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT1);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT2");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT2);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT3");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT3);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BELOW_ALARM_LIMIT4");
    m_comboMinorType.SetItemData(nIndex, MINOR_BELOW_ALARM_LIMIT4);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT1");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT1);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT2");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT2);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT3");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT3);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ABOVE_ALARM_LIMIT4");
    m_comboMinorType.SetItemData(nIndex, MINOR_ABOVE_ALARM_LIMIT3);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_URGENCYBTN_ON");
    m_comboMinorType.SetItemData(nIndex, MINOR_URGENCYBTN_ON);
    nIndex++;    
	m_comboMinorType.InsertString(nIndex, "MINOR_URGENCYBTN_OFF");
    m_comboMinorType.SetItemData(nIndex, MINOR_URGENCYBTN_OFF);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_VIRTUAL_DEFENCE_BANDIT");
    m_comboMinorType.SetItemData(nIndex, MINOR_VIRTUAL_DEFENCE_BANDIT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_VIRTUAL_DEFENCE_FIRE");
    m_comboMinorType.SetItemData(nIndex, MINOR_VIRTUAL_DEFENCE_FIRE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_VIRTUAL_DEFENCE_URGENT");
    m_comboMinorType.SetItemData(nIndex, MINOR_VIRTUAL_DEFENCE_URGENT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOTDET_START");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOTDET_START);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOTDET_STOP");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOTDET_STOP);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HIDE_ALARM_START");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HIDE_ALARM_START);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HIDE_ALARM_STOP");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HIDE_ALARM_STOP);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_UPS_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_UPS_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ELECTRICITY_METER_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ELECTRICITY_METER_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SWITCH_POWER_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SWITCH_POWER_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_TEMP_HUMI_ALARM");
	m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_TEMP_HUMI_ALARM);
	nIndex++;
}

void CDlgAlarmHostLogSearch::InitMinorOperatorCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
    m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_GUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_GUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_UNGUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_UNGUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BYPASS");
    m_comboMinorType.SetItemData(nIndex, MINOR_BYPASS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DURESS_ACCESS");
    m_comboMinorType.SetItemData(nIndex, MINOR_DURESS_ACCESS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_REBOOT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_REBOOT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_REBOOT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_REBOOT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_LOCAL_UPGRADE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_LOCAL_UPGRADE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_UPGRADE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_UPGRADE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RECOVERY_DEFAULT_PARAM");
    m_comboMinorType.SetItemData(nIndex, MINOR_RECOVERY_DEFAULT_PARAM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ALARM_OUTPUT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARM_OUTPUT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ACCESS_OPEN");
    m_comboMinorType.SetItemData(nIndex, MINOR_ACCESS_OPEN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ACCESS_CLOSE");
    m_comboMinorType.SetItemData(nIndex, MINOR_ACCESS_CLOSE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SIREN_OPEN");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIREN_OPEN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SIREN_CLOSE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIREN_CLOSE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_ZONE_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_ZONE_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_ALARMOUT_CONIFG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_ALARMOUT_CONIFG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_ANALOG_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_ANALOG_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RS485_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_RS485_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_PHONE_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_PHONE_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_ADMIN");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_ADMIN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_NETUSER_PARAM");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_NETUSER_PARAM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_ADMIN");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_ADMIN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_NETUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_NETUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_NETUSER_PARAM");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_NETUSER_PARAM);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_NETUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_NETUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_OPERATORUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_OPERATORUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_OPERATORUSER_PW");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_OPERATORUSER_PW);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_OPERATORUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_OPERATORUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_ADD_KEYPADUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_ADD_KEYPADUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_DEL_KEYPADUSER");
    m_comboMinorType.SetItemData(nIndex, MINOR_DEL_KEYPADUSER);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTEUSER_LOGIN");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTEUSER_LOGIN);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTEUSER_LOGOUT");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTEUSER_LOGOUT);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTE_GUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTE_GUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_REMOTE_UNGUARD");
    m_comboMinorType.SetItemData(nIndex, MINOR_REMOTE_UNGUARD);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_MOD_HOST_CONFIG");
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_HOST_CONFIG);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RESTORE_BYPASS");
    m_comboMinorType.SetItemData(nIndex, MINOR_RESTORE_BYPASS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMOUT_OPEN");// 报警输出开启
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMOUT_OPEN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMOUT_CLOSE");// 报警输出开启
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMOUT_CLOSE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_SUBSYSTEM_PARAM");// 修改子系统参数配置
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_SUBSYSTEM_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_GROUP_BYPASS");// 组旁路
    m_comboMinorType.SetItemData(nIndex, MINOR_GROUP_BYPASS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_RESTORE_GROUP_BYPASS");// 组旁路恢复
    m_comboMinorType.SetItemData(nIndex, MINOR_RESTORE_GROUP_BYPASS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_GRPS_PARAM");// 修改GPRS参数
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_GRPS_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_NET_REPORT_PARAM");// 修改网络上报参数配置
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_NET_REPORT_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_REPORT_MOD");// 修改上传方式配置
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_REPORT_MOD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_MOD_GATEWAY_PARAM");// 修改门禁参数配置
    m_comboMinorType.SetItemData(nIndex, MINOR_MOD_GATEWAY_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_START_REC");// 远程开始录像
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_START_REC);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_STOP_REC");// 远程停止录像
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_STOP_REC);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_START_TRANS_CHAN");// 开始透明传输
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_START_TRANS_CHAN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_STOP_TRANS_CHAN");// 停止透明传输
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_STOP_TRANS_CHAN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_START_VT");// 开始语音对讲
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_START_VT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_STOP_VTM");// 停止语音对讲
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_STOP_VTM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_PLAYBYFILE");// 远程按文件回放
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_PLAYBYFILE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_PLAYBYTIME");// 远程按时间回放
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_PLAYBYTIME);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_PTZCTRL");// 远程云台控制
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_PTZCTRL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_FORMAT_HDD");// 远程格式化硬盘
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_FORMAT_HDD);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_LOCKFILE");// 远程锁定文件
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_LOCKFILE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_UNLOCKFILE");// 远程解锁文件
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_UNLOCKFILE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_CFGFILE_OUTPUT");// 远程导出配置文件
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_CFGFILE_OUTPUT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_CFGFILE_INTPUT");// 远程导入配置文件
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_CFGFILE_INTPUT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REMOTE_RECFILE_OUTPUT");// 远程导出录象文件
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REMOTE_RECFILE_OUTPUT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_STAY_ARM");// 留守布防
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_STAY_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_QUICK_ARM");// 即时布防
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_QUICK_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_AUTOMATIC_ARM");// 自动布防
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_AUTOMATIC_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_AUTOMATIC_DISARM");// 自动撤防
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_AUTOMATIC_DISARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_KEYSWITCH_ARM");// 钥匙防区布防
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_KEYSWITCH_ARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_KEYSWITCH_DISARM");// 钥匙防区撤防
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_KEYSWITCH_DISARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CLEAR_ALARM");// 消警
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CLEAR_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_FAULT_CFG");// 修改系统故障配置
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_FAULT_CFG);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG");// 修改事件触发报警输出配置
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_SEARCH_EXTERNAL_MODULE");// 搜索外接模块
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_SEARCH_EXTERNAL_MODULE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REGISTER_EXTERNAL_MODULE");// 重新注册外接模块
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REGISTER_EXTERNAL_MODULE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_CLOSE_KEYBOARD_ALARM");// 关闭键盘故障提示音
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_CLOSE_KEYBOARD_ALARM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_3G_PARAM");// 修改3G参数
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_3G_PARAM);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_MOD_PRINT_PARAM");// 修改3G参数
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_MOD_PRINT_PARAM);
    nIndex++;
}

void CDlgAlarmHostLogSearch::InitMinorExcepCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
	m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_POWER_ON");
    m_comboMinorType.SetItemData(nIndex, MINOR_POWER_ON);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_POWER_OFF");
    m_comboMinorType.SetItemData(nIndex, MINOR_POWER_OFF);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WDT_RESET");
    m_comboMinorType.SetItemData(nIndex, MINOR_WDT_RESET);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_LOW_BATTERY_VOLTAGE");
    m_comboMinorType.SetItemData(nIndex, MINOR_LOW_BATTERY_VOLTAGE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_AC_LOSS");
    m_comboMinorType.SetItemData(nIndex, MINOR_AC_LOSS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_AC_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_AC_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RTC_EXCEPTION");
    m_comboMinorType.SetItemData(nIndex, MINOR_RTC_EXCEPTION);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_NETWORK_CONNECT_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_NETWORK_CONNECT_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_NETWORK_CONNECT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_NETWORK_CONNECT_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_TEL_LINE_CONNECT_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_TEL_LINE_CONNECT_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_TEL_LINE_CONNECT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_TEL_LINE_CONNECT_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_EXPANDER_BUS_LOSS");
    m_comboMinorType.SetItemData(nIndex, MINOR_EXPANDER_BUS_LOSS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_EXPANDER_BUS_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_EXPANDER_BUS_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_BUS_LOSS");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_BUS_LOSS);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_KEYPAD_BUS_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_KEYPAD_BUS_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SENSOR_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SENSOR_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SENSOR_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SENSOR_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RS485_CONNECT_FAILURE");
    m_comboMinorType.SetItemData(nIndex, MINOR_RS485_CONNECT_FAILURE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_RS485_CONNECT_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_RS485_CONNECT_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_BATTERT_VOLTAGE_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_BATTERT_VOLTAGE_RESTORE);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_WIRED_NETWORK_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRED_NETWORK_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_WIRED_NETWORK_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_WIRED_NETWORK_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_GPRS_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_GPRS_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_GPRS_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_GPRS_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_3G_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_3G_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_3G_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_3G_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SIM_CARD_ABNORMAL");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIM_CARD_ABNORMAL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_SIM_CARD_RESTORE");
    m_comboMinorType.SetItemData(nIndex, MINOR_SIM_CARD_RESTORE);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_VI_LOST");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_VI_LOST);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_ILLEGAL_ACCESS");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_ILLEGAL_ACCESS);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HD_FULL");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HD_FULL);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_HD_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_HD_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_DCD_LOST");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_DCD_LOST);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_IP_CONFLICT");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_IP_CONFLICT);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_NET_BROKEN");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_NET_BROKEN);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_REC_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_REC_ERROR);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_VI_EXCEPTION");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_VI_EXCEPTION);
    nIndex++;
	m_comboMinorType.InsertString(nIndex, "MINOR_ALARMHOST_FORMAT_HDD_ERROR");
    m_comboMinorType.SetItemData(nIndex, MINOR_ALARMHOST_FORMAT_HDD_ERROR);
    nIndex++;
}

void CDlgAlarmHostLogSearch::InitMinorEventCombo()
{
    int nIndex = 0;
    m_comboMinorType.ResetContent();
    
    m_comboMinorType.InsertString(nIndex, "All");
    m_comboMinorType.SetItemData(nIndex, 0);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SCHOOLTIME_IRGI_B");
    m_comboMinorType.SetItemData(nIndex, MINOR_SCHOOLTIME_IRGI_B);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SCHOOLTIME_SDK");
    m_comboMinorType.SetItemData(nIndex, MINOR_SCHOOLTIME_SDK);
    nIndex++;
    m_comboMinorType.InsertString(nIndex, "MINOR_SCHOOLTIME_SELFTEST");
    m_comboMinorType.SetItemData(nIndex, MINOR_SCHOOLTIME_SELFTEST);
    nIndex++;
}

void CDlgAlarmHostLogSearch::OnBtnSearch() 
{
    LONG lSelectMode = m_comboMode.GetCurSel();
    UpdateData(TRUE);
    GetSearchParamFromWnd(m_struSearchParam);
    m_lSearchHandle = NET_DVR_FindAlarmHostLog(m_lServerID, lSelectMode, &m_struSearchParam);
    if (m_lSearchHandle < 0)
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_FindAlarmHostLog");
        AfxMessageBox("Fail to find log");
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_FindAlarmHostLog");
    }
    
    char szLan[128] = {0};
    
    DWORD dwThreadId;
    m_listAlarmHostLog.DeleteAllItems();
    m_bSearch = TRUE;
    m_lLogNum = 0;
    if (m_hFileThread == NULL)
    {
        m_hFileThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetAlarmHostLogThread),this,0,&dwThreadId);		
    }
    if (m_hFileThread == NULL)
    {
        g_StringLanType(szLan, "打开线程失败", "Open thread failed");
        AfxMessageBox(szLan);
        return;
    }
    g_StringLanType(szLan, "停止搜索", "Stop Search");
    GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
    m_bSearch = TRUE;
}

void CDlgAlarmHostLogSearch::OnCancel() 
{
	OnBtnExit();
}

void CDlgAlarmHostLogSearch::OnBtnExit() 
{
    if (m_hFileThread)
    {
        TerminateThread(m_hFileThread, 0);
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
        NET_DVR_FindAlarmHostLogClose(m_lSearchHandle);
    }
    CDialog::OnCancel();
}


void CDlgAlarmHostLogSearch::GetSearchParamFromWnd(NET_DVR_ALARMHOST_SEARCH_LOG_PARAM &struSearchParam)
{
    struSearchParam.wMajorType = m_comboMajorType.GetCurSel();
    struSearchParam.wMinorType = m_comboMinorType.GetCurSel();
    struSearchParam.struStartTime.dwYear = m_ctDataStart.GetYear();
    struSearchParam.struStartTime.dwMonth = m_ctDataStart.GetMonth();
    struSearchParam.struStartTime.dwDay = m_ctDataStart.GetDay();
    struSearchParam.struStartTime.dwHour = m_ctTimeStart.GetHour();
    struSearchParam.struStartTime.dwMinute = m_ctTimeStart.GetMinute();
    struSearchParam.struStartTime.dwSecond = m_ctTimeStart.GetSecond();
    
    struSearchParam.struEndTime.dwYear = m_ctDataStop.GetYear();
    struSearchParam.struEndTime.dwMonth = m_ctDataStop.GetMonth();
    struSearchParam.struEndTime.dwDay = m_ctDataStop.GetDay();
    struSearchParam.struEndTime.dwHour = m_ctTimeStop.GetHour();
    struSearchParam.struEndTime.dwMinute = m_ctTimeStop.GetMinute();
    struSearchParam.struEndTime.dwSecond = m_ctTimeStop.GetSecond();
}

void CDlgAlarmHostLogSearch::OnSelchangeComboMajorType() 
{
    InitMinorTypeCombo();
    m_comboMinorType.SetCurSel(0);
}

void CDlgAlarmHostLogSearch::InfoMinorAlarmMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_SHORT_CIRCUIT:
        g_StringLanType(szLan, "短路报警", "SHORT_CIRCUIT");
        break;
    case MINOR_BROKEN_CIRCUIT:           // 断路报警
        g_StringLanType(szLan, "断路报警", "BROKEN_CIRCUIT");
        break;
    case MINOR_ALARM_RESET:              // 报警复位
        g_StringLanType(szLan, "报警复位", "ALARM_RESET");
        break;
    case MINOR_ALARM_NORMAL:				// 报警恢复正常
        g_StringLanType(szLan, "报警恢复正常", "ALARM_NORMAL");
        break;
    case MINOR_PASSWORD_ERROR:			// 密码错误（连续3次输入密码错误）
        g_StringLanType(szLan, "密码输入错误", "PASSWORD_ERROR");
        break;
    case MINOR_ID_CARD_ILLEGALLY:		// 非法感应卡ID
        g_StringLanType(szLan, "非法感应卡ID", "ID_CARD_ILLEGALLY");
        break;
    case MINOR_KEYPAD_REMOVE:			// 键盘防拆
        g_StringLanType(szLan, "键盘防拆", "KEYPAD_REMOVE");
        break;
    case MINOR_KEYPAD_REMOVE_RESTORE:	// 键盘防拆复位
        g_StringLanType(szLan, "键盘防拆复位", "KEYPAD_REMOVE_RESTORE");
        break;
    case MINOR_DEV_REMOVE:				// 设备防拆
        g_StringLanType(szLan, "设备防拆", "DEV_REMOVE");
        break;
    case MINOR_DEV_REMOVE_RESTORE:		// 设备防拆复位
        g_StringLanType(szLan, "设备防拆复位", "DEV_REMOVE_RESTORE");
        break;
    case MINOR_BELOW_ALARM_LIMIT1:		// 模拟量低于报警限1
        g_StringLanType(szLan, "模拟量低于报警限1", "BELOW_ALARM_LIMIT1");
        break;
    case MINOR_BELOW_ALARM_LIMIT2:		// 模拟量低于报警限2
        g_StringLanType(szLan, "模拟量低于报警限2", "BELOW_ALARM_LIMIT2");
        break;
    case MINOR_BELOW_ALARM_LIMIT3:		// 模拟量低于报警限3
        g_StringLanType(szLan, "模拟量低于报警限3", "BELOW_ALARM_LIMIT3");
        break;
    case MINOR_BELOW_ALARM_LIMIT4:		// 模拟量低于报警限4
        g_StringLanType(szLan, "模拟量低于报警限4", "BELOW_ALARM_LIMIT4");
        break;
    case MINOR_ABOVE_ALARM_LIMIT1:		// 模拟量低于报警限1
        g_StringLanType(szLan, "模拟量高于报警限1", "ABOVE_ALARM_LIMIT1");
        break;
    case MINOR_ABOVE_ALARM_LIMIT2:		// 模拟量低于报警限2
        g_StringLanType(szLan, "模拟量高于报警限2", "ABOVE_ALARM_LIMIT2");
        break;
    case MINOR_ABOVE_ALARM_LIMIT3:		// 模拟量低于报警限3
        g_StringLanType(szLan, "模拟量高于报警限3", "ABOVE_ALARM_LIMIT3");
        break;
    case MINOR_ABOVE_ALARM_LIMIT4:		// 模拟量低于报警限4
        g_StringLanType(szLan, "模拟量高于报警限4", "ABOVE_ALARM_LIMIT4");
        break;
	case MINOR_URGENCYBTN_ON:			//紧急按钮按下
		g_StringLanType(szLan, "紧急按钮按下", "URGENCYBTN_ON");
		break;
	case MINOR_URGENCYBTN_OFF:			//紧急按钮复位
		g_StringLanType(szLan, "紧急按钮复位", "URGENCYBTN_OFF");
		break;
	case MINOR_VIRTUAL_DEFENCE_BANDIT:			//软防区匪警
		g_StringLanType(szLan, "软防区匪警", "VIRTUAL_DEFENCE_BANDIT");
		break;
	case MINOR_VIRTUAL_DEFENCE_FIRE:			//软防区火警
		g_StringLanType(szLan, "软防区火警", "VIRTUAL_DEFENCE_FIRE");
		break;
	case MINOR_VIRTUAL_DEFENCE_URGENT:			//软防区紧急
		g_StringLanType(szLan, "软防区紧急", "VIRTUAL_DEFENCE_URGENT");
		break;
	case MINOR_ALARMHOST_MOTDET_START:			//移动侦测报警开始
		g_StringLanType(szLan, "移动侦测报警开始", "ALARMHOST_MOTDET_START");
		break;
	case MINOR_ALARMHOST_MOTDET_STOP:			//移动侦测报警结束
		g_StringLanType(szLan, "移动侦测报警结束", "ALARMHOST_MOTDET_STOP");
		break;
	case MINOR_ALARMHOST_HIDE_ALARM_START:			//遮挡报警开始
		g_StringLanType(szLan, "遮挡报警开始", "ALARMHOST_HIDE_ALARM_START");
		break;
	case MINOR_ALARMHOST_HIDE_ALARM_STOP:			//遮挡报警结束
		g_StringLanType(szLan, "遮挡报警结束", "ALARMHOST_HIDE_ALARM_STOP");
		break;
	case MINOR_ALARMHOST_UPS_ALARM:					//UPS报警
		g_StringLanType(szLan, "UPS报警", "ALARMHOST_UPS_ALARM");
		break;
	case MINOR_ALARMHOST_ELECTRICITY_METER_ALARM:	//电量表报警
		g_StringLanType(szLan, "电量表报警", "ALARMHOST_ELECTRICITY_METER_ALARM");
		break;
	case MINOR_ALARMHOST_SWITCH_POWER_ALARM:		//开关电源报警
		g_StringLanType(szLan, "开关电源报警", "ALARMHOST_SWITCH_POWER_ALARM");
		break;
	case MINOR_ALARMHOST_GAS_DETECT_SYS_ALARM:		//气体检测系统报警
		g_StringLanType(szLan, "气体检测系统报警", "ALARMHOST_GAS_DETECT_SYS_ALARM");
		break;
	case MINOR_ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM: //变压器温显表报警
		g_StringLanType(szLan, "变压器温显表报警", "ALARMHOST_TRANSFORMER_TEMPRATURE_ALARM");
		break;
	case MINOR_ALARMHOST_TEMP_HUMI_ALARM: //温湿度传感器报警
		g_StringLanType(szLan, "温湿度传感器报警", "ALARMHOST_TEMP_HUMI_ALARM");
		break;
    default:
        sprintf(szLan, "%d", dwMinorType);  
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMinorExceptionMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_POWER_ON:	// 上电
        g_StringLanType(szLan, "上电", "POWER_ON");
        break;
    case MINOR_POWER_OFF:				// 掉电
        g_StringLanType(szLan, "掉电", "POWER_OFF");
        break;
    case MINOR_WDT_RESET:				// WDT 复位
        g_StringLanType(szLan, "WDT复位", "WDT_RESET");
        break;
    case MINOR_LOW_BATTERY_VOLTAGE:		// 蓄电池电压低
        g_StringLanType(szLan, "蓄电池电压低", "LOW_BATTERY_VOLTAGE");
        break;
    case MINOR_AC_LOSS:					// 交流电断电
        g_StringLanType(szLan, "交流电断电", "AC_LOSS");
        break;
    case MINOR_AC_RESTORE:				// 交流电恢复
        g_StringLanType(szLan, "交流电恢复", "AC_RESTORE");
        break;
    case MINOR_RTC_EXCEPTION:			// RTC实时时钟异常
        g_StringLanType(szLan, "RTC实时时钟异常", "RTC_EXCEPTION");
        break;
    case MINOR_NETWORK_CONNECT_FAILURE:	// 网络连接断
        g_StringLanType(szLan, "网络连接断", "NETWORK_CONNECT_FAILURE");
        break;
    case MINOR_NETWORK_CONNECT_RESTORE:	// 网络连接恢复
        g_StringLanType(szLan, "网络连接恢复", "NETWORK_CONNECT_RESTORE");
        break;
    case MINOR_TEL_LINE_CONNECT_FAILURE:	// 电话线连接断
        g_StringLanType(szLan, "电话线连接断", "TEL_LINE_CONNECT_FAILURE");
        break;
    case MINOR_TEL_LINE_CONNECT_RESTORE:	// 电话线连接恢复
        g_StringLanType(szLan, "电话线连接恢复", "TEL_LINE_CONNECT_RESTORE");
        break;
    case MINOR_EXPANDER_BUS_LOSS:		// 扩展总线模块掉线
        g_StringLanType(szLan, "扩展总线模块掉线", "EXPANDER_BUS_LOSS");
        break;
    case MINOR_EXPANDER_BUS_RESTORE:		// 扩展总线模块掉线恢复
        g_StringLanType(szLan, "扩展总线模块掉线恢复", "EXPANDER_BUS_RESTORE");
        break;
    case MINOR_KEYPAD_BUS_LOSS:			// 键盘总线模块掉线
        g_StringLanType(szLan, "键盘总线模块掉线", "KEYPAD_BUS_LOSS");
        break;
    case MINOR_KEYPAD_BUS_RESTORE:		// 键盘总线模块掉线恢复
        g_StringLanType(szLan, "键盘总线模块掉线恢复", "KEYPAD_BUS_RESTORE");
        break;
    case MINOR_SENSOR_FAILURE:			// 模拟量传感器故障
        g_StringLanType(szLan, "模拟量传感器故障", "SENSOR_FAILURE");
        break;
    case MINOR_SENSOR_RESTORE:			// 模拟量传感器恢复
        g_StringLanType(szLan, "模拟量传感器故障恢复", "SENSOR_RESTORE");
        break;
    case MINOR_RS485_CONNECT_FAILURE:	// RS485通道连接断
        g_StringLanType(szLan, "485通道连接断", "RS485_CONNECT_FAILURE");
        break;
    case MINOR_RS485_CONNECT_RESTORE:	// RS485通道连接断恢复
        g_StringLanType(szLan, "485通道连接断恢复", "RS485_CONNECT_RESTORE");
        break;
    case MINOR_BATTERT_VOLTAGE_RESTORE:
        g_StringLanType(szLan, "蓄电池电压恢复正常", "BATTERT_VOLTAGE_RESTORE");
        break;
	case MINOR_WIRED_NETWORK_ABNORMAL:			// 有线网络异常
        g_StringLanType(szLan, "有线网络异常", "WIRED_NETWORK_ABNORMAL");
        break;
    case MINOR_WIRED_NETWORK_RESTORE:	// 有线网络恢复正常
        g_StringLanType(szLan, "有线网络恢复正常", "WIRED_NETWORK_RESTORE");
        break;
    case MINOR_GPRS_ABNORMAL:	// GPRS通信异常
        g_StringLanType(szLan, "GPRS通信异常", "GPRS_ABNORMAL");
        break;
    case MINOR_GPRS_RESTORE: //GPRS恢复正常
        g_StringLanType(szLan, "GPRS恢复正常", "GPRS_RESTORE");
        break;
	case MINOR_3G_ABNORMAL: //3G通信异常
        g_StringLanType(szLan, "3G通信异常", "3G_ABNORMAL");
        break;
	case MINOR_3G_RESTORE: //3G恢复正常
        g_StringLanType(szLan, "3G恢复正常", "3G_RESTORE");
        break;
	case MINOR_SIM_CARD_ABNORMAL: //SIM卡异常
        g_StringLanType(szLan, "SIM卡异常", "SIM_CARD_ABNORMAL");
        break;
	case MINOR_SIM_CARD_RESTORE: //SIM卡恢复正常
        g_StringLanType(szLan, "SIM卡恢复正常", "SIM_CARD_RESTORE");
        break;
	case MINOR_ALARMHOST_VI_LOST: //视频信号丢失
        g_StringLanType(szLan, "视频信号丢失", "ALARMHOST_VI_LOST");
        break;
	case MINOR_ALARMHOST_ILLEGAL_ACCESS: //非法访问
        g_StringLanType(szLan, "非法访问", "ALARMHOST_ILLEGAL_ACCESS");
        break;
	case MINOR_ALARMHOST_HD_FULL: //硬盘满
        g_StringLanType(szLan, "硬盘满", "ALARMHOST_HD_FULL");
        break;
	case MINOR_ALARMHOST_HD_ERROR: //硬盘错误
        g_StringLanType(szLan, "硬盘错误", "ALARMHOST_HD_ERROR");
        break;
	case MINOR_ALARMHOST_DCD_LOST: //MODEM 掉线
        g_StringLanType(szLan, "MODEM 掉线", "ALARMHOST_DCD_LOST");
        break;
	case MINOR_ALARMHOST_IP_CONFLICT: //IP地址冲突
        g_StringLanType(szLan, "IP地址冲突", "ALARMHOST_IP_CONFLICT");
        break;
	case MINOR_ALARMHOST_NET_BROKEN: //网络断开
        g_StringLanType(szLan, "网络断开", "ALARMHOST_NET_BROKEN");
        break;
	case MINOR_ALARMHOST_REC_ERROR: //录像出错
        g_StringLanType(szLan, "录像出错", "ALARMHOST_REC_ERROR");
        break;
	case MINOR_ALARMHOST_VI_EXCEPTION: //视频输入异常
        g_StringLanType(szLan, "视频输入异常", "ALARMHOST_VI_EXCEPTION");
        break;
	case MINOR_ALARMHOST_FORMAT_HDD_ERROR: //远程格式化硬盘失败
		g_StringLanType(szLan, "远程格式化硬盘失败", "ALARMHOST_FORMAT_HDD_ERROR");
        break;
	case MINOR_ALARMHOST_USB_ERROR: //USB通信故障
		g_StringLanType(szLan, "USB通信故障", "ALARMHOST_USB_ERROR");
        break;
	case MINOR_ALARMHOST_USB_RESTORE: //USB通信故障恢复
		g_StringLanType(szLan, "USB通信故障恢复", "ALARMHOST_USB_RESTORE");
        break;
	case MINOR_ALARMHOST_PRINT_ERROR: //打印机故障
		g_StringLanType(szLan, "打印机故障", "ALARMHOST_PRINT_ERROR");
        break;
	case MINOR_ALARMHOST_PRINT_RESTORE: //打印机故障恢复
		g_StringLanType(szLan, "打印机故障恢复", "ALARMHOST_PRINT_RESTORE");
        break;
    default:
        sprintf(szLan, "%d", dwMinorType);
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMinorOperationMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_GUARD:	// 普通布防
        g_StringLanType(szLan, "普通布防", "GUARD");
        break;
    case MINOR_UNGUARD:					// 普通撤防
        g_StringLanType(szLan, "普通撤防", "UNGUARD");
        break;
    case MINOR_BYPASS:					// 旁路
        g_StringLanType(szLan, "旁路", "BYPASS");
        break;
    case MINOR_DURESS_ACCESS:			// 挟持
        g_StringLanType(szLan, "挟持", "DURESS_ACCESS");
        break;
    case MINOR_ALARMHOST_LOCAL_REBOOT:	// 本地重启
        g_StringLanType(szLan, "本地重启", "ALARMHOST_LOCAL_REBOOT");
        break;
    case MINOR_ALARMHOST_REMOTE_REBOOT:	// 远程重启
        g_StringLanType(szLan, "远程重启", "ALARMHOST_REMOTE_REBOOT");
        break;
    case MINOR_ALARMHOST_LOCAL_UPGRADE:	// 本地升级
        g_StringLanType(szLan, "本地升级", "ALARMHOST_LOCAL_UPGRADE");
        break;
    case MINOR_ALARMHOST_REMOTE_UPGRADE:	// 远程升级
        g_StringLanType(szLan, "远程升级", "ALARMHOST_REMOTE_UPGRADE");
        break;
    case MINOR_RECOVERY_DEFAULT_PARAM:	// 恢复默认参数
        g_StringLanType(szLan, "恢复默认参数", "RECOVERY_DEFAULT_PARAM");
        break;
    case MINOR_ALARM_OUTPUT:				// 控制报警输出
        g_StringLanType(szLan, "控制报警输出", "ALARM_OUTPUT");
        break;
    case MINOR_ACCESS_OPEN:				// 控制门禁开
        g_StringLanType(szLan, "控制门禁开", "ACCESS_OPEN");
        break;
    case MINOR_ACCESS_CLOSE:				// 控制门禁关
        g_StringLanType(szLan, "控制门禁关", "ACCESS_CLOSE");
        break;
    case MINOR_SIREN_OPEN:				// 控制警号开
        g_StringLanType(szLan, "控制警号开", "SIREN_OPEN");
        break;
    case MINOR_SIREN_CLOSE:				// 控制警号关
        g_StringLanType(szLan, "控制警号关", "SIREN_CLOSE");
        break;
    case MINOR_MOD_ZONE_CONFIG:		// 修改防区设置
        g_StringLanType(szLan, "修改防区设置", "MOD_ZONE_CONFIG");
        break;
    case MINOR_MOD_ALARMOUT_CONIFG:	// 控制报警输出配置
        g_StringLanType(szLan, "修改报警输出设置", "MOD_ALARMOUT_CONIFG");
        break;
    case MINOR_MOD_ANALOG_CONFIG:		// 修改模拟量配置
        g_StringLanType(szLan, "修改模拟量设置", "MOD_ANALOG_CONFIG");
        break;
    case MINOR_RS485_CONFIG:				// 修改485通道配置
        g_StringLanType(szLan, "修改485通道配置", "RS485_CONFIG");
        break;
    case MINOR_PHONE_CONFIG:				// 修改拨号配置
        g_StringLanType(szLan, "修改拨号配置", "PHONE_CONFIG");
        break;
    case MINOR_ADD_ADMIN:        // 增加管理员
        g_StringLanType(szLan, "增加管理员", "ADD_ADMIN");
        break;
    case MINOR_MOD_ADMIN_PARAM:	// 修改管理员参数
        g_StringLanType(szLan, "修改管理员参数", "MOD_ADMIN_PARAM");
        break;
    case MINOR_DEL_ADMIN:		// 删除管理员
        g_StringLanType(szLan, "删除管理员", "DEL_ADMIN");
        break;
    case MINOR_ADD_NETUSER:		// 增加后端操作员
        g_StringLanType(szLan, "增加后端操作员", "ADD_NETUSER");
        break;
    case MINOR_MOD_NETUSER_PARAM:	// 修改后端操作员参数
        g_StringLanType(szLan, "修改后端操作员参数", "MOD_NETUSER_PARAM");
        break;
    case MINOR_DEL_NETUSER:				// 删除后端操作员
        g_StringLanType(szLan, "删除后端操作员", "DEL_NETUSER");
        break;
    case MINOR_ADD_OPERATORUSER:			// 增加前端操作员
        g_StringLanType(szLan, "增加前端操作员", "ADD_OPERATORUSER");
        break;
    case MINOR_MOD_OPERATORUSER_PW:		//修改前端操作员密码
        g_StringLanType(szLan, "修改前端操作员密码", "MOD_OPERATORUSER_PW");
        break;
    case MINOR_DEL_OPERATORUSER:			// 删除前端操作员
        g_StringLanType(szLan, "删除前端操作员", "DEL_OPERATORUSER");
        break;
    case MINOR_ADD_KEYPADUSER:			// 增加键盘/读卡器用户	
        g_StringLanType(szLan, "增加键盘/读卡器用户	", "ADD_KEYPADUSER");
        break;
    case MINOR_DEL_KEYPADUSER:			// 删除键盘/读卡器用户	
        g_StringLanType(szLan, "删除键盘/读卡器", "DEL_KEYPADUSER");
        break;
    case MINOR_REMOTEUSER_LOGIN:		// 远程用户登陆
        g_StringLanType(szLan, "远程用户登陆", "REMOTEUSER_LOGIN");
        break;
    case MINOR_REMOTEUSER_LOGOUT:		// 远程用户注销
        g_StringLanType(szLan, "远程用户注销", "REMOTEUSER_LOGOUT");
        break;
    case MINOR_REMOTE_GUARD:				// 远程布防
        g_StringLanType(szLan, "远程布防", "REMOTE_GUARD");
        break;
    case MINOR_REMOTE_UNGUARD:			// 远程撤防
        g_StringLanType(szLan, "远程撤防", "REMOTE_UNGUARD");
        break;
    case MINOR_MOD_HOST_CONFIG:
        g_StringLanType(szLan, "修改主机配置", "MOD_HOST_CONFIG");
        break;
    case MINOR_RESTORE_BYPASS:
        g_StringLanType(szLan, "旁路恢复", "RESTORE_BYPASS");
        break;
    case MINOR_ALARMOUT_OPEN:	//报警输出开启
        g_StringLanType(szLan, "报警输出开启", "ALARMOUT_OPEN");
        break;
    case MINOR_ALARMOUT_CLOSE:	//报警输出关闭
        g_StringLanType(szLan, "报警输出关闭", "ALARMOUT_CLOSE");
        break;
    case MINOR_MOD_SUBSYSTEM_PARAM:	//修改子系统参数配置
        g_StringLanType(szLan, "修改子系统参数配置", "MOD_SUBSYSTEM_PARAM");
        break;
    case MINOR_GROUP_BYPASS:	//组旁路
        g_StringLanType(szLan, "组旁路", "GROUP_BYPASS");
        break;
    case MINOR_RESTORE_GROUP_BYPASS:	//组旁路恢复
        g_StringLanType(szLan, "组旁路恢复", "RESTORE_GROUP_BYPASS");
        break;
    case MINOR_MOD_GRPS_PARAM:	//修改GPRS参数
        g_StringLanType(szLan, "修改GPRS参数", "MOD_GRPS_PARAM");
        break;
    case MINOR_MOD_NET_REPORT_PARAM:	//修改网络上报参数配置
        g_StringLanType(szLan, "修改网络上报参数配置", "MOD_NET_REPORT_PARAM");
        break;
    case MINOR_MOD_REPORT_MOD:	//修改上传方式配置
        g_StringLanType(szLan, "修改上传方式配置", "MOD_REPORT_MOD");
        break;
    case MINOR_MOD_GATEWAY_PARAM:	//修改门禁参数配置
        g_StringLanType(szLan, "修改门禁参数配置", "MOD_GATEWAY_PARAM");
        break;
    case MINOR_ALARMHOST_REMOTE_START_REC:	//远程开始录像
        g_StringLanType(szLan, "远程开始录像", "ALARMHOST_REMOTE_START_REC");
        break;
    case MINOR_ALARMHOST_REMOTE_STOP_REC:	//远程停止录像
        g_StringLanType(szLan, "远程停止录像", "ALARMHOST_REMOTE_STOP_REC");
        break;
    case MINOR_ALARMHOST_START_TRANS_CHAN:	//开始透明传输
        g_StringLanType(szLan, "开始透明传输", "ALARMHOST_START_TRANS_CHAN");
        break;
    case MINOR_ALARMHOST_STOP_TRANS_CHAN:	//停止透明传输
        g_StringLanType(szLan, "停止透明传输", "ALARMHOST_STOP_TRANS_CHAN");
        break;
    case MINOR_ALARMHOST_START_VT:	//开始语音对讲
        g_StringLanType(szLan, "开始语音对讲", "ALARMHOST_START_VT");
        break;
	case MINOR_ALARMHOST_STOP_VTM:	//停止语音对讲
        g_StringLanType(szLan, "停止语音对讲", "ALARMHOST_STOP_VTM");
        break;
    case MINOR_ALARMHOST_REMOTE_PLAYBYFILE:	//远程按文件回放
        g_StringLanType(szLan, "远程按文件回放", "ALARMHOST_REMOTE_PLAYBYFILE");
        break;
	case MINOR_ALARMHOST_REMOTE_PLAYBYTIME:	//远程按时间回放
        g_StringLanType(szLan, "远程按时间回放", "ALARMHOST_REMOTE_PLAYBYTIME");
        break;
    case MINOR_ALARMHOST_REMOTE_PTZCTRL:	//远程云台控制
        g_StringLanType(szLan, "远程云台控制", "ALARMHOST_REMOTE_PTZCTRL");
        break;
	case MINOR_ALARMHOST_REMOTE_FORMAT_HDD:	//远程格式化硬盘
        g_StringLanType(szLan, "远程格式化硬盘", "ALARMHOST_REMOTE_FORMAT_HDD");
        break;
    case MINOR_ALARMHOST_REMOTE_LOCKFILE:	//远程锁定文件
        g_StringLanType(szLan, "远程锁定文件", "ALARMHOST_REMOTE_LOCKFILE");
        break;
	case MINOR_ALARMHOST_REMOTE_UNLOCKFILE:	//远程解锁文件
        g_StringLanType(szLan, "远程解锁文件", "ALARMHOST_REMOTE_UNLOCKFILE");
        break;
    case MINOR_ALARMHOST_REMOTE_CFGFILE_OUTPUT:	//远程导出配置文件
        g_StringLanType(szLan, "远程导出配置文件", "ALARMHOST_REMOTE_CFGFILE_OUTPUT");
        break;
	case MINOR_ALARMHOST_REMOTE_CFGFILE_INTPUT:	//远程导入配置文件
        g_StringLanType(szLan, "远程导入配置文件", "ALARMHOST_REMOTE_CFGFILE_INTPUT");
        break;
    case MINOR_ALARMHOST_STAY_ARM:	//留守布防
        g_StringLanType(szLan, "留守布防", "ALARMHOST_STAY_ARM");
        break;
    case MINOR_ALARMHOST_QUICK_ARM:	//即时布防
        g_StringLanType(szLan, "即时布防", "ALARMHOST_QUICK_ARM");
        break;
	case MINOR_ALARMHOST_AUTOMATIC_ARM:	//自动布防
        g_StringLanType(szLan, "自动布防", "ALARMHOST_AUTOMATIC_ARM");
        break;
	case MINOR_ALARMHOST_AUTOMATIC_DISARM:	//自动撤防
        g_StringLanType(szLan, "自动撤防", "ALARMHOST_AUTOMATIC_DISARM");
        break;
	case MINOR_ALARMHOST_KEYSWITCH_ARM:	//钥匙防区布防
        g_StringLanType(szLan, "钥匙防区布防", "ALARMHOST_KEYSWITCH_ARM");
        break;
	case MINOR_ALARMHOST_KEYSWITCH_DISARM:	//钥匙防区撤防
        g_StringLanType(szLan, "钥匙防区撤防", "ALARMHOST_KEYSWITCH_DISARM");
        break;

	case MINOR_ALARMHOST_CLEAR_ALARM:	//消警
        g_StringLanType(szLan, "消警", "ALARMHOST_CLEAR_ALARM");
        break;
	case MINOR_ALARMHOST_MOD_FAULT_CFG:	//修改系统故障配置
        g_StringLanType(szLan, "修改系统故障配置", "ALARMHOST_MOD_FAULT_CFG");
        break;
	case MINOR_ALARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG:	//修改事件触发报警输出配置
        g_StringLanType(szLan, "修改事件触发报警输出配置", "LARMHOST_MOD_EVENT_TRIGGER_ALARMOUT_CFG");
        break;
	case MINOR_ALARMHOST_SEARCH_EXTERNAL_MODULE:	//搜索外接模块
        g_StringLanType(szLan, "搜索外接模块", "ALARMHOST_SEARCH_EXTERNAL_MODULE");
        break;
	case MINOR_ALARMHOST_REGISTER_EXTERNAL_MODULE:	//重新注册外接模块
        g_StringLanType(szLan, "重新注册外接模块", "ALARMHOST_REGISTER_EXTERNAL_MODULE");
        break;
	case MINOR_ALARMHOST_CLOSE_KEYBOARD_ALARM:	//关闭键盘故障提示音
        g_StringLanType(szLan, "关闭键盘故障提示音", "ALARMHOST_CLOSE_KEYBOARD_ALARM");
        break;
	case MINOR_ALARMHOST_MOD_3G_PARAM:	//修改3G参数
        g_StringLanType(szLan, "修改3G参数", "ALARMHOST_MOD_3G_PARAM");
        break;
	case MINOR_ALARMHOST_MOD_PRINT_PARAM:
		g_StringLanType(szLan, "修改打印机配置", "ALARMHOST_MOD_PRINT_PARAM");
        break;
    default:
        sprintf(szLan, "%d", dwMinorType);
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMinorEventMap(DWORD dwMinorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMinorType)
    {
    case MINOR_SCHOOLTIME_IRGI_B:		// B码校时
        g_StringLanType(szLan, "B码校时", "SCHOOLTIME_IRGI_B");
        break;
    case MINOR_SCHOOLTIME_SDK:				// SDK校时
        g_StringLanType(szLan, "SDK校时", "SCHOOLTIME_SDK");
        break;
    case MINOR_SCHOOLTIME_SELFTEST:			// 定时自检校时
        g_StringLanType(szLan, "定时自检校时", "SCHOOLTIME_SELFTEST");
        break;
    default:
        sprintf(szLan, "%d", dwMinorType);
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::InfoMajorTypeMap(DWORD dwMajorType, CString &szTemp)
{
    char szLan[256] = {0};
    switch (dwMajorType)
    {
    case MAJOR_ALARMHOST_ALARM:
        g_StringLanType(szLan, "报警", "ALARM");
        break;
    case MAJOR_ALARMHOST_EXCEPTION:
        g_StringLanType(szLan, "异常", "EXCEPTION");
        break;
    case MAJOR_ALARMHOST_OPERATION:
        g_StringLanType(szLan, "操作", "OPERATION");
        break;
    case MAJ0R_ALARMHOST_EVENT:
        g_StringLanType(szLan, "事件", "EVENT");
        break;
    default:
        break;
    }
    szTemp.Format("%s", szLan);
}

void CDlgAlarmHostLogSearch::OnBtnExportLog() 
{
    CString csFileName = "";
    CString csTmp = "";
    GetCurrentDirectory(1024, csTmp.GetBuffer(0));

    csFileName += csTmp;
    csFileName.Format("%s%s",csFileName.GetBuffer(0), "\\Alarm host log");
    if (!CreateDirectory(csFileName, NULL))
    {
        TRACE("CreateDirectory Fail");
    }
    

    SYSTEMTIME t;
    GetLocalTime(&t);
    csTmp.Format("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d%3.3d", t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds);
    
    csFileName = csFileName + "\\" + csTmp + ".txt";
    
    HANDLE hFile = INVALID_HANDLE_VALUE;

    hFile = CreateFile(csFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    int i = 0;
    char szLan[4096] = {0};
    CString strTmp = "";
    for (i = 0; i < m_listAlarmHostLog.GetItemCount(); i++)
    {
        memset(szLan, 0, sizeof(szLan));
        strTmp = m_listAlarmHostLog.GetItemText(i, 0);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));
        
        strTmp = m_listAlarmHostLog.GetItemText(i, 1);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp = m_listAlarmHostLog.GetItemText(i, 2);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));
           
        strTmp = m_listAlarmHostLog.GetItemText(i, 3);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 4);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 5);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 6);
        sprintf(szLan, "%s  %s", szLan, strTmp.GetBuffer(0));

        strTmp  = m_listAlarmHostLog.GetItemText(i, 7);
        sprintf(szLan, "%s  %s\r\n", szLan, strTmp.GetBuffer(0));
        DWORD dwReturn = 0;
        WriteFile(hFile, szLan, strlen(szLan), &dwReturn, NULL);
    }

    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
    }
}
