// DlgProductionTest.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgProductionTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProductionTest dialog


CDlgProductionTest::CDlgProductionTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProductionTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProductionTest)
	m_bAgeingCmd = FALSE;
	m_bDevFan1 = FALSE;
	m_bDevFan2 = FALSE;
	m_bDevHeater1 = FALSE;
	m_bDevHeater2 = FALSE;
	m_bRedlightTest = FALSE;
	m_fHuml = 0.0f;
	m_fTemp = 0.0f;
	m_dwChan = 0;
	m_csDialSwitch = _T("");
	//}}AFX_DATA_INIT
}


void CDlgProductionTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProductionTest)
	DDX_Control(pDX, IDC_COMBO_SOUND_MODE, m_comSoundMode);
	DDX_Control(pDX, IDC_COM_SELFCHECK, m_comSelfCheck);
	DDX_Check(pDX, IDC_CHK_AGEING_COMMAND, m_bAgeingCmd);
	DDX_Check(pDX, IDC_CHK_DEVFAN1, m_bDevFan1);
	DDX_Check(pDX, IDC_CHK_DEVFAN2, m_bDevFan2);
	DDX_Check(pDX, IDC_CHK_DEVHEATER1, m_bDevHeater1);
	DDX_Check(pDX, IDC_CHK_DEVHEATER2, m_bDevHeater2);
	DDX_Check(pDX, IDC_CHK_REDLIGHT_TESTMODE_COMMAND, m_bRedlightTest);
	DDX_Text(pDX, IDC_EDIT_HUMI, m_fHuml);
	DDX_Text(pDX, IDC_EDIT_TEMP, m_fTemp);
	DDX_Text(pDX, IDC_EDIT_CHANNEL, m_dwChan);
	DDX_Text(pDX, IDC_EDIT_DIAL_SWITCH, m_csDialSwitch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProductionTest, CDialog)
	//{{AFX_MSG_MAP(CDlgProductionTest)
	ON_BN_CLICKED(IDC_BUT_SELFCHECK_REFRESH, OnButSelfcheckRefresh)
	ON_BN_CLICKED(IDC_BUT_START_COMMAND, OnButStartCommand)
	ON_BN_CLICKED(IDC_BUT_START_DEVTEST, OnButStartDevtest)
	ON_BN_CLICKED(IDC_BUT_RESUME_INITRACKPOS, OnButResumeInitrackpos)
	ON_BN_CLICKED(IDC_BTN_GET_TEMP_HUMI, OnBtnGetTempHumi)
	ON_BN_CLICKED(IDC_BTN_SET_AUTOFOCUS, OnBtnSetAutofocus)
	ON_BN_CLICKED(IDC_BTN_GET_SOUNDMODE, OnBtnGetSoundmode)
	ON_BN_CLICKED(IDC_BTN_SET_SOUNDMODE, OnBtnSetSoundmode)
	ON_BN_CLICKED(IDC_BTN_GET_DIAL_SWITCH, OnBtnGetDialSwitch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProductionTest message handlers

void CDlgProductionTest::OnButSelfcheckRefresh() 
{
	// TODO: Add your control notification handler code here
    NET_DVR_TEST_SELFCHECK_RESULT  struSelfcheckResult; 
    CString csError; 
    if (!NET_DVR_GetDeviceStatus(m_lUserID, NET_DVR_GET_SELFCHECK_RESULT, 1, NULL, 0, NULL, &struSelfcheckResult, sizeof(NET_DVR_TEST_SELFCHECK_RESULT)))
    {
        csError.Format(_T("获取错误 Error Code %d"),NET_DVR_GetLastError()); 
        AfxMessageBox(csError);
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetDeviceStatus NET_DVR_GET_SELFCHECK_RESULT");
        m_comSelfCheck.SetCurSel(0);
        UpdateData(FALSE); 
        return ; 
    }
    switch (struSelfcheckResult.bySelfCheckStatus)
    {
    case 0:
    case 1:       
    case 2:         
    case 3:
        m_comSelfCheck.SetCurSel(struSelfcheckResult.bySelfCheckStatus+1); 
        break; 
    default:
        AfxMessageBox(_T("返回未知状态"));
        m_comSelfCheck.SetCurSel(0);
    }
    UpdateData(FALSE); 
}

void CDlgProductionTest::OnButStartCommand() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); 
    NET_DVR_TEST_COMMAND struTestCmd; 
    memset(&struTestCmd, 0, sizeof(struTestCmd)); 
    struTestCmd.dwSize = sizeof(NET_DVR_TEST_COMMAND); 
    struTestCmd.dwChannel = m_dwChannel; 
    struTestCmd.byTestCommand[0] = m_bAgeingCmd; 
    struTestCmd.byTestCommand[1] = m_bRedlightTest; 

    if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_SET_TEST_COMMAND, &struTestCmd, sizeof(NET_DVR_TEST_COMMAND)))
    {
        CString csError; 
        csError.Format(_T("发送开启命令失败 Error Code %d"),NET_DVR_GetLastError()); 
        AfxMessageBox(csError);
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_RemoteControl NET_DVR_TEST_COMMAND");
    }
    m_bAgeingCmd = FALSE; 
    m_bRedlightTest = FALSE; 
    UpdateData(FALSE); 	
}

void CDlgProductionTest::OnButStartDevtest() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE); 
    NET_DVR_TEST_DEVMODULE_CFG struTestDevModuleCfg;
    memset(&struTestDevModuleCfg, 0, sizeof(struTestDevModuleCfg)); 
    struTestDevModuleCfg.dwSize = sizeof(struTestDevModuleCfg);
    struTestDevModuleCfg.dwChannel = m_dwChannel; 
    struTestDevModuleCfg.byDevFanModuleType[0] = m_bDevFan1; 
    struTestDevModuleCfg.byDevFanModuleType[1] = m_bDevFan2; 
    struTestDevModuleCfg.byDevHeaterModuleType[0] = m_bDevHeater1; 
    struTestDevModuleCfg.byDevHeaterModuleType[1] = m_bDevHeater2; 
    if (! NET_DVR_RemoteControl(m_lUserID, NET_DVR_SET_TEST_DEVMODULE, &struTestDevModuleCfg, sizeof(struTestDevModuleCfg)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SetDVRConfig NET_DVR_SET_TEST_DEVMODULE");
        CString csError; 
        csError.Format(_T("发送开启命令失败 Error Code %d"), NET_DVR_GetLastError()); 
        AfxMessageBox(csError);  
    }
    else
    {
        // 刷新一下状态
        //OnButRefreshDevtest(); 
        m_bDevFan1 = FALSE; 
        m_bDevFan2 = FALSE; 
        m_bDevHeater1 = FALSE; 
        m_bDevHeater2 = FALSE; 
        UpdateData(FALSE); 
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_LOGO_OVERLAYCFG");
	}	
}

BOOL CDlgProductionTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	// TODO: Add extra initialization here
    char szLan[128] = {0};
    m_comSelfCheck.ResetContent();
    sprintf(szLan, "   "); 
    m_comSelfCheck.AddString(szLan); 
    sprintf(szLan, "未开始"); 
    m_comSelfCheck.AddString(szLan); 
    sprintf(szLan, "成功");
    m_comSelfCheck.AddString(szLan); 
    sprintf(szLan, "失败");
    m_comSelfCheck.AddString(szLan); 
    sprintf(szLan, "自检中");
    m_comSelfCheck.AddString(szLan);

	m_comSoundMode.SetCurSel(0);

	m_dwChan = m_dwChannel;

    UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProductionTest::OnButResumeInitrackpos() 
{
	// TODO: Add your control notification handler code here
    NET_DVR_RESUME_INITRACKPOS_CFG struResumeITPCfg; 
    memset(&struResumeITPCfg, 0, sizeof(struResumeITPCfg)); 
    struResumeITPCfg.dwSize = sizeof(struResumeITPCfg); 
    struResumeITPCfg.dwChannel = m_dwChannel;
    if (! NET_DVR_RemoteControl(m_lUserID, NET_DVR_RESUME_INITRACKPOS, &struResumeITPCfg, sizeof(struResumeITPCfg)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_RemoteControl NET_DVR_RESUME_INITRACKPOS");
        CString csError; 
        csError.Format(_T("Error Code %d"), NET_DVR_GetLastError()); 
        AfxMessageBox(csError);  
	}	
}

void CDlgProductionTest::OnBtnGetTempHumi() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE); 
	DWORD dwRet = 0;
	char szLan[128] = {0};
	NET_DVR_TEMP_HUMI struTempHuml;
	memset(&struTempHuml, 0, sizeof(struTempHuml));

	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_TEMP_HUMI, m_dwChannel, &struTempHuml, sizeof(struTempHuml), &dwRet))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_TEMP_HUMI Failed!");
		g_StringLanType(szLan, "获取温湿度失败", "Failed to get the Temperature And Humidity Parameters");
		AfxMessageBox(szLan);
		m_fTemp = 0;
		m_fHuml = 0;
		UpdateData(FALSE); 
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_TEMP_HUMI Succ!");
	}

 	int iTemp = struTempHuml.iTemp/1000;
 	float fTemp2 = float(struTempHuml.iTemp%1000)/1000;
 	m_fTemp = (float)iTemp + fTemp2;
 
 	int iHuml = struTempHuml.iHumidity/1000;
	float fHuml2 = float(struTempHuml.iHumidity%1000)/1000;
 	m_fHuml = (float)iHuml + fHuml2;
	

	UpdateData(FALSE);
}

void CDlgProductionTest::OnBtnSetAutofocus() 
{
	// TODO: Add your control notification handler code here
	NET_DVR_AUTOFOCUS_TESTCFG struAutoFocus = {0};
	struAutoFocus.dwSize = sizeof(NET_DVR_AUTOFOCUS_TESTCFG);
	struAutoFocus.dwChannel = m_dwChannel;
	if (!NET_DVR_RemoteControl(m_lUserID, NET_DVR_SET_AUTOFOCUS_TEST, &struAutoFocus, sizeof(struAutoFocus)))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_RemoteControl NET_DVR_SET_AUTOFOCUS_TEST");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_RemoteControl NET_DVR_SET_AUTOFOCUS_TEST");
	}
}

void CDlgProductionTest::OnBtnGetSoundmode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	NET_DVR_ALARMSOUNDMODE_CFG struSoundMode = {0};
	struSoundMode.dwSize = sizeof(NET_DVR_ALARMSOUNDMODE_CFG);
	DWORD dwRet = 0;
	m_comSoundMode.SetCurSel(0);

	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMSOUNDMODE, m_dwChannel, &struSoundMode, sizeof(struSoundMode), &dwRet))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetDVRConfig NET_DVR_GET_ALARMSOUNDMODE");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GetDVRConfig NET_DVR_GET_ALARMSOUNDMODE");
		m_comSoundMode.SetCurSel(struSoundMode.dwSoundMode);
		m_dwChan = struSoundMode.dwChannel;
	}
	UpdateData(FALSE);
}

void CDlgProductionTest::OnBtnSetSoundmode() 
{
	// TODO: Add your control notification handler code here
	NET_DVR_ALARMSOUNDMODE_CFG struSoundMode = {0};
	struSoundMode.dwSize = sizeof(NET_DVR_ALARMSOUNDMODE_CFG);
	struSoundMode.dwChannel = m_dwChannel;
	struSoundMode.dwSoundMode = m_comSoundMode.GetCurSel();

	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMSOUNDMODE, m_dwChannel, &struSoundMode, sizeof(struSoundMode)))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SetDVRConfig NET_DVR_SET_ALARMSOUNDMODE");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SetDVRConfig NET_DVR_SET_ALARMSOUNDMODE");
	}
}

void CDlgProductionTest::OnBtnGetDialSwitch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	NET_DVR_DIAL_SWITCH_CFG struDialSwitch = {0};
	struDialSwitch.dwSize = sizeof(NET_DVR_DIAL_SWITCH_CFG);
	DWORD dwRet = 0;
	m_csDialSwitch.Format(_T(""));
	
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_DIAL_SWITCH_CFG, m_dwChannel, &struDialSwitch, sizeof(struDialSwitch), &dwRet))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetDVRConfig NET_DVR_GET_DIAL_SWITCH_CFG");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GetDVRConfig NET_DVR_GET_DIAL_SWITCH_CFG");
		m_csDialSwitch.Format("%X", struDialSwitch.dwSwitchState);
	}
	UpdateData(FALSE);
}
