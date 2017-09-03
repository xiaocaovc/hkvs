// DlgProductAlarmHostCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgProductAlarmHostCfg.h"
#include "DlgAlarmHostStatus.h"
#include "DlgAlarmOutCfg.h"
#include "DlgAlarmHostAbility.h"
#include "DlgAlarmHostRs485.h"
#include "DlgAlarmKeyboardUser.h"
#include "DlgAlarmNetUser.h"
#include "DlgAlarmOperatorUser.h"
#include "DlgAlarmHostEnableCfg.h"
#include "DlgGatewayCfg.h"
#include "DlgAlarmHostReportMode.h"
#include "DlgAlarmHostGPRS.h"
#include "DlgAlarmHostNetCfg.h"
#include "DlgAlarmHostAudioAssociateAlarm.h"
#include "DlgAlarmHostLED.h"
#include "DlgSafetyCabin.h"
#include "DlgAlarmhostModuleCfg.h"
#include "DlgAlarmHostExternalDevState.h"
#include "DlgAlarmhostExternalDevLimitValue.h"
#include "DlgAlarmhostSensorLinkage.h"
#include "DlgAlarmhostHistoryData.h"
#include "DlgAlarmhostPointCfg.h"
#include "DlgAlarmhostDataUploadMode.h"
#include "DlgAlarmhostPointCfg.h"
#include "DlgAlarmhostHistoryData.h"
#include "DlgAlarmhostDataUploadMode.h"
#include "DlgAlarmhostPrinter.h"
#include "DlgAlarmHostLogSearch.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProductAlarmHostCfg dialog


CDlgProductAlarmHostCfg::CDlgProductAlarmHostCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProductAlarmHostCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProductAlarmHostCfg)
	m_csAlarmInName = _T("");
	m_iDelayTime = 0;
	m_iHour11 = 0;
	m_iHour12 = 0;
	m_iHour21 = 0;
	m_iHour22 = 0;
	m_iMin11 = 0;
	m_iMin12 = 0;
	m_iMin21 = 0;
	m_iMin22 = 0;
	m_iMin31 = 0;
	m_iMin32 = 0;
	m_iMin41 = 0;
	m_iMin42 = 0;
	m_iHour31 = 0;
	m_iHour32 = 0;
	m_iHour41 = 0;
	m_iHour42 = 0;
	m_bGroupBypass = FALSE;
	m_chUploadReport = FALSE;
	//}}AFX_DATA_INIT
    m_iDevIndex = -1;
    m_lServerID = -1;
    m_lStartChan = -1;
    m_lChannel = -1;
    memset(&m_struAlarmHostAbility, 0, sizeof(m_struAlarmHostAbility));
    memset(&m_struAlarmInParam, 0, sizeof(m_struAlarmInParam));    
    memset(&m_struAlarmInSetup, 0, sizeof(m_struAlarmInSetup));
    m_fBatteryVoltage = 0;
	m_bInit = FALSE;
}


void CDlgProductAlarmHostCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProductAlarmHostCfg)
	DDX_Control(pDX, IDC_COMBO_ALARM_SENSITIVITY, m_cmSensitivity);
	DDX_Control(pDX, IDC_COMBOCOPYTIME, m_comboCopyTime);
	DDX_Control(pDX, IDC_COMBOWEEKDAY, m_comboWeekday);
	DDX_Control(pDX, IDC_LIST_SIREN_OUT, m_listSirenOut);
	DDX_Control(pDX, IDC_COMBO_DETECTOR, m_comboDetector);
	DDX_Control(pDX, IDC_COMBO_DEFENSE_TYPE, m_comboDefenseType);
	DDX_Control(pDX, IDC_LIST_ALARM_IN, m_listAlarmIn);
	DDX_Control(pDX, IDC_LIST_ALARM_OUT, m_listAlarmOut);
	DDX_Control(pDX, IDC_COMBO_ALARM_IN, m_comboAlarmIn);
	DDX_Text(pDX, IDC_EDIT_ALARMIN_NAME, m_csAlarmInName);
	DDV_MaxChars(pDX, m_csAlarmInName, 32);
	DDX_Text(pDX, IDC_EDIT_DELAY_TIME, m_iDelayTime);
	DDX_Text(pDX, IDC_EDITHOUR11, m_iHour11);
	DDX_Text(pDX, IDC_EDITHOUR12, m_iHour12);
	DDX_Text(pDX, IDC_EDITHOUR21, m_iHour21);
	DDX_Text(pDX, IDC_EDITHOUR22, m_iHour22);
	DDX_Text(pDX, IDC_EDITMIN11, m_iMin11);
	DDX_Text(pDX, IDC_EDITMIN12, m_iMin12);
	DDX_Text(pDX, IDC_EDITMIN21, m_iMin21);
	DDX_Text(pDX, IDC_EDITMIN22, m_iMin22);
	DDX_Text(pDX, IDC_EDITMIN31, m_iMin31);
	DDX_Text(pDX, IDC_EDITMIN32, m_iMin32);
	DDX_Text(pDX, IDC_EDITMIN41, m_iMin41);
	DDX_Text(pDX, IDC_EDITMIN42, m_iMin42);
	DDX_Text(pDX, IDC_EDITHOUR31, m_iHour31);
	DDX_Text(pDX, IDC_EDITHOUR32, m_iHour32);
	DDX_Text(pDX, IDC_EDITHOUR41, m_iHour41);
	DDX_Text(pDX, IDC_EDITHOUR42, m_iHour42);
	DDX_Check(pDX, IDC_CHECK_GROUPBYPASS, m_bGroupBypass);
	DDX_Check(pDX, IDC_CHECK_UPLOAD_ALARM_RECOVERY_REPORT, m_chUploadReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProductAlarmHostCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgProductAlarmHostCfg)
	ON_BN_CLICKED(IDC_BTN_SET_ALARMIN_PARAM, OnBtnSetAlarmInParam)
	ON_CBN_SELCHANGE(IDC_COMBO_ALARM_IN, OnSelchangeComboAlarmIn)
	ON_BN_CLICKED(IDC_BTN_SET_ALARM_CHAN, OnBtnSetAlarmChan)
	ON_BN_CLICKED(IDC_BTN_CLOSE_ALARM_CHAN, OnBtnCloseAlarmChan)
	ON_BN_CLICKED(IDC_BTN_STATUS_QUERY, OnBtnStatusQuery)
	ON_BN_CLICKED(IDC_BTN_ALARMOUT_CFG, OnBtnAlarmoutCfg)
	ON_BN_CLICKED(IDC_BTN_ALARMHOST_ABILITY, OnBtnAlarmhostAbility)
	ON_BN_CLICKED(IDC_BTN_SENSOR_CFG, OnBtnSensorCfg)
	ON_BN_CLICKED(IDC_BTN_RS485_CFG, OnBtnRs485Cfg)
	ON_BN_CLICKED(IDC_BTN_NET_USER, OnBtnNetUser)
	ON_BN_CLICKED(IDC_BTN_KEYBOARD_USER, OnBtnKeyboardUser)
	ON_BN_CLICKED(IDC_BTN_OPERATOR_USER, OnBtnOperatorUser)
	ON_BN_CLICKED(IDC_BTN_BATTARY_VOLTAGE, OnBtnBattaryVoltage)
	ON_BN_CLICKED(IDC_BTN_BYPASS, OnBtnBypass)
	ON_BN_CLICKED(IDC_BTN_UN_BYPASS, OnBtnUnBypass)
	ON_BN_CLICKED(IDC_BTN_ALARMHOST_ENABLECFG, OnBtnAlarmhostEnablecfg)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALARM_OUT, OnRclickListAlarmOut)
	ON_COMMAND(ID_MENU_SELECT_ALL, OnMenuSelectAll)
	ON_COMMAND(ID_MENU_CANCEL_ALL, OnMenuCancelAll)
	ON_BN_CLICKED(IDC_TIME_OK, OnTimeOk)
	ON_BN_CLICKED(IDC_TIME_COPY, OnTimeCopy)
	ON_CBN_SELCHANGE(IDC_COMBOWEEKDAY, OnSelchangeComboweekday)
	ON_CBN_SELCHANGE(IDC_COMBOCOPYTIME, OnSelchangeCombocopytime)
	ON_CBN_SELCHANGE(IDC_COMBO_DEFENSE_TYPE, OnSelchangeComboDefenseType)
	ON_BN_CLICKED(IDC_BTN_GATEWAY_CFG, OnBtnGatewayCfg)
	ON_BN_CLICKED(IDC_BUTTON_DIALPARAM, OnButtonDialparam)
	ON_BN_CLICKED(IDC_BTN_BYPASSSETUP, OnBtnBypasssetup)
	ON_BN_CLICKED(IDC_BTN_BYPASS_STOPSETUP, OnBtnBypassStopsetup)
	ON_BN_CLICKED(IDC_BTN_SUBSYSTEM_CFG, OnBtnSubsystemCfg)
	ON_BN_CLICKED(IDC_BTN_REPORTMODE, OnBtnReportmode)
	ON_BN_CLICKED(IDC_BTN_GPRSPARA, OnBtnGprspara)
	ON_BN_CLICKED(IDC_BTN_NETCFG, OnBtnNetcfg)
	ON_BN_CLICKED(IDC_BTN_ZOOM_CODE, OnBtnZoomCode)
	ON_BN_CLICKED(IDC_BTN_TRIGGER_CODE, OnBtnTriggerCode)
	ON_BN_CLICKED(IDC_BTN_FAULT_PROCESS, OnBtnFaultProcess)
	ON_BN_CLICKED(IDC_BTN_EVENT_TRIGGER, OnBtnEventTrigger)
	ON_BN_CLICKED(IDC_BTN_EXTERN_MODULE, OnBtnExternModule)
	ON_BN_CLICKED(IDC_BTN_LED, OnBtnLed)
	ON_BN_CLICKED(IDC_BTN_AUDIO_ASSOCIATE, OnBtnAudioAssociate)
	ON_BN_CLICKED(IDC_BTN_SAFETY_CABIN, OnBtnSafetyCabin)
	ON_BN_CLICKED(IDC_BTN_MODULE_CFG, OnBtnModuleCfg)
	ON_BN_CLICKED(IDC_BTN_EXTERNAL_DEVICE_STATE, OnBtnExternalDeviceState)
	ON_BN_CLICKED(IDC_BTN_EXTERNAL_DEVICE_LIMIT_VALUE, OnBtnExternalDeviceLimitValue)
	ON_BN_CLICKED(IDC_BTN_SENSOR_LINKAGE, OnBtnSensorLinkage)
	ON_BN_CLICKED(IDC_BTN_POINT_CFG, OnBtnPointCfg)
	ON_BN_CLICKED(IDC_BTN_UPLOAD_MODE, OnBtnUploadMode)
	ON_BN_CLICKED(IDC_BTN_HISTORY_DATA, OnBtnHistoryData)
	ON_BN_CLICKED(IDC_BTN_PRINTER, OnBtnPrinter)
	ON_BN_CLICKED(IDC_BUT_NETALARM_ALARM_LOG_SEARCH, OnButAlarmLogSearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProductAlarmHostCfg message handlers

BOOL CDlgProductAlarmHostCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
//     m_comboAlarmOut.SetCurSel(0);
//     OnSelchangeComboAlarmOut();
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgProductAlarmHostCfg::InitWnd()
{
    InitAlarmInCombo();
    InitAlarmOutList();
    InitAlarmInList();
    InitSirenOutList();
//    InitAlarmOutCombo();
    InitAlarmInDetectorCombo();
    IninAlarmInDefenseTypeCombo();
	InitWeekdayCombo();
	InitSensitivityCombo();
}

void CDlgProductAlarmHostCfg::InitAlarmInCombo()
{
    char szLan[128] = {0};
    m_comboAlarmIn.ResetContent();
    int i = 0;
	for ( i = 0; i < (m_struAlarmHostAbility.wTotalAlarmInNum); i++)
    {
        sprintf(szLan, "Alarm In %d", i + 1);
        m_comboAlarmIn.InsertString(i, szLan);
        m_comboAlarmIn.SetItemData(i, i);
    }
    
    if (i > 0)
    {
        m_comboAlarmIn.SetCurSel(0);
    }
}

void CDlgProductAlarmHostCfg::InitWeekdayCombo()
{
	char szLan[128] = {0};
	g_StringLanType(szLan, "整个星期", "Whole Week");
	m_comboCopyTime.AddString(szLan);
	
	g_StringLanType(szLan, "星期一", "Monday");
	m_comboWeekday.AddString(szLan);
	m_comboCopyTime.AddString(szLan);
	g_StringLanType(szLan, "星期二", "Tuesday");
	m_comboWeekday.AddString(szLan);
	m_comboCopyTime.AddString(szLan);
	g_StringLanType(szLan, "星期三", "Wednesday");
	m_comboWeekday.AddString(szLan);
	m_comboCopyTime.AddString(szLan);
	g_StringLanType(szLan, "星期四", "Thursday");
	m_comboWeekday.AddString(szLan);
	m_comboCopyTime.AddString(szLan);
	g_StringLanType(szLan, "星期五", "Friday");
	m_comboWeekday.AddString(szLan);
	m_comboCopyTime.AddString(szLan);
	g_StringLanType(szLan, "星期六", "Saturday");
	m_comboWeekday.AddString(szLan);
	m_comboCopyTime.AddString(szLan);
	g_StringLanType(szLan, "星期天", "Sunday");
	m_comboWeekday.AddString(szLan);
	m_comboCopyTime.AddString(szLan);
}


void CDlgProductAlarmHostCfg::InitSensitivityCombo()
{
	m_cmSensitivity.ResetContent();
	m_cmSensitivity.AddString("10ms");
	m_cmSensitivity.AddString("250ms");
	m_cmSensitivity.AddString("500ms");
	m_cmSensitivity.AddString("750ms");
}

void CDlgProductAlarmHostCfg::InitAlarmOutList()
{
    char szLan[128] = {0};

    m_listAlarmOut.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (int i = 0; i < (m_struAlarmHostAbility.wLocalAlarmOutNum + m_struAlarmHostAbility.wExpandAlarmOutNum); i++)
    {
        sprintf(szLan, "Alarm Out %d", (i+1));
        m_listAlarmOut.InsertItem(i, szLan);
    }
}

void CDlgProductAlarmHostCfg::InitAlarmInList()
{
    char szLan[128] = {0};
    
    m_listAlarmIn.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (int i = 0; i < (m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum); i++)
    {
        sprintf(szLan, "Alarm In %d", (i+1));
        m_listAlarmIn.InsertItem(i, szLan);
    }
}

void CDlgProductAlarmHostCfg::InitSirenOutList()
{
    char szLan[128] = {0};
    m_listSirenOut.SetExtendedStyle(LVS_EX_CHECKBOXES);
    for (int i = 0; i < m_struAlarmHostAbility.wSirenNum; i++)
    {
        sprintf(szLan, "Siren %d", (i+1));
        m_listSirenOut.InsertItem(i, szLan);
    }
}

void CDlgProductAlarmHostCfg::InitAlarmInDetectorCombo()
{
    char szLan[128] = {0};
    m_comboDetector.ResetContent();
    int nIndex = 0;

    g_StringLanType(szLan, "紧急开关", "Panic Button");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, PANIC_BUTTON);
    nIndex++;

    g_StringLanType(szLan, "门磁开关", "Magnetic Contact");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, MAGNETIC_CONTACT);
    nIndex++;

    g_StringLanType(szLan, "烟感探测器", "Smoke Detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, SMOKE_DETECTOR);
    nIndex++;
    
    g_StringLanType(szLan, "主动红外探测器", "Active Infrared Detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, ACTIVE_INFRARED_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "被动红外探测器", "Passive Infrared Detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, PASSIVE_INFRARED_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "玻璃破碎探测器", "Glass Break Detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, GLASS_BREAK_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "震动探测器", "Vibration detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, VIBRATION_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "双鉴移动探测器", "Dual Technology PIR detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, DUAL_TECHNOLOGY_PIR_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "三技术探测器", "Triple Technology PIR detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, TRIPLE_TECHNOLOGY_PIR_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "湿度探测器", "Humidity Dectector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, HUMIDITY_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "温感探测器", "Temperature Dectector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, TEMPERATURE_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "可燃气体探测器", "Combustible Gas Detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, COMBUSTIBLE_GAS_DETECTOR);
    nIndex++;

    g_StringLanType(szLan, "其他探测器", "Other Detector");
    m_comboDetector.InsertString(nIndex, szLan);
    m_comboDetector.SetItemData(nIndex, OTHER_DETECTOR);
    nIndex++;

}

void CDlgProductAlarmHostCfg::IninAlarmInDefenseTypeCombo()
{
    char szLan[128] = {0};
    m_comboDefenseType.ResetContent();
    g_StringLanType(szLan, "即时防区", "Instant zone");
    m_comboDefenseType.AddString(szLan);
    g_StringLanType(szLan, "24小时有声防区", "24-hour zone");
    m_comboDefenseType.AddString(szLan);
    g_StringLanType(szLan, "延时防区", "Delay zone");
    m_comboDefenseType.AddString(szLan);
    g_StringLanType(szLan, "内部防区", "Inter zone");
    m_comboDefenseType.AddString(szLan);
    g_StringLanType(szLan, "钥匙布撤防防区", "Key zone");
    m_comboDefenseType.AddString(szLan);

	g_StringLanType(szLan, "火警防区", "Fire zone");
    m_comboDefenseType.AddString(szLan);
    g_StringLanType(szLan, "周界防区", "perimeter zone");
    m_comboDefenseType.AddString(szLan);
    g_StringLanType(szLan, "24小时无声防区", "24-hour silent zone");
    m_comboDefenseType.AddString(szLan);
	g_StringLanType(szLan, "屏蔽防区", "shield zone");
    m_comboDefenseType.AddString(szLan);
}

BOOL CDlgProductAlarmHostCfg::GetAlarmHostAbility(NET_DVR_ALARMHOST_ABILITY& struAlarmHostAbility)
{
    BOOL bRet = FALSE;
    DWORD dwReturn = 0;
    if (NET_DVR_GetDeviceAbility(m_lServerID, ALARMHOST_ABILITY, NULL, 0, (char*)&struAlarmHostAbility, sizeof(struAlarmHostAbility)))
    {
        char szLan[128] = {0};
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "ALARMHOST_ABILITY");
        bRet = TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "ALARMHOST_ABILITY");
        bRet = FALSE;
    }
    return bRet;
}

BOOL CDlgProductAlarmHostCfg::SetAlarmInParam(NET_DVR_ALARMIN_PARAM& struAlarmInParam)
{
//     if (!NET_DVR_SetDVRConfig(m_lServerID, NET_DVR_SET_AlarmInParamCFG, i + m_lStartChan + sel * 64, &m_struAlarmInParamCfg, sizeof(m_struAlarmInParamCfg)))
    BOOL bRet = FALSE;
    LONG lAlarmIn = m_comboAlarmIn.GetItemData(m_comboAlarmIn.GetCurSel());
    if (NET_DVR_SetDVRConfig(m_lServerID, NET_DVR_SET_ALARMIN_PARAM, lAlarmIn, &struAlarmInParam, sizeof(struAlarmInParam)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_ALARMIN_PARAM");
        bRet = TRUE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_ALARMIN_PARAM");
        bRet = FALSE;
    }
    return bRet;
}

BOOL CDlgProductAlarmHostCfg::GetAlarmInParam(NET_DVR_ALARMIN_PARAM& struAlarmInParam)
{
    BOOL bRet = FALSE;
    LONG lAlarmIn = m_comboAlarmIn.GetItemData(m_comboAlarmIn.GetCurSel());
    DWORD dwReturn = 0;
    if (NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_ALARMIN_PARAM, lAlarmIn, &struAlarmInParam, sizeof(struAlarmInParam), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_ALARMIN_PARAM");
        bRet = TRUE;
    }
    else   
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_ALARMIN_PARAM");
        bRet = FALSE;
    }
    return bRet;
}

BOOL CDlgProductAlarmHostCfg::AlarmHostSetAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
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

BOOL CDlgProductAlarmHostCfg::AlarmHostCloseAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
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

// BOOL CDlgProductAlarmHostCfg::SetAlarmOutTimeCfg(NET_DVR_ALARMOUTTIMECFG &struAlarmOutTime)
// {
// // #define NET_DVR_GET_ALARMOUTTIMECFG		1122 //获取报警输出/警号时间参数
// // #define NET_DVR_SET_ALARMOUTTIMECFG		1123//设置报警输出/警号时间参数
//     BOOL bRet = FALSE;
//     int lAlarmOut = m_comboAlarmOut.GetItemData(m_comboAlarmOut.GetCurSel());
//     if (!NET_DVR_SetDVRConfig(m_lServerID, NET_DVR_SET_ALARMOUTTIMECFG, lAlarmOut, &struAlarmOutTime, sizeof(struAlarmOutTime)))
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_ALARMOUTTIMECFG");
//         bRet = FALSE;
//     }
//     else
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_ALARMOUTTIMECFG");
//         bRet = TRUE;
//     }   
//     return bRet;
// }
// 
// BOOL CDlgProductAlarmHostCfg::GetAlarmOutTimeCfg(NET_DVR_ALARMOUTTIMECFG &struAlarmOutTime)
// {
//     BOOL bRet = FALSE;
//     int lAlarmOut = m_comboAlarmOut.GetItemData(m_comboAlarmOut.GetCurSel());
//     DWORD dwReturn = 0;
//     if (!NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_ALARMOUTTIMECFG, lAlarmOut, &struAlarmOutTime, sizeof(struAlarmOutTime), &dwReturn))
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_ALARMOUTTIMECFG");
//         bRet = FALSE;
//     }
//     else
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_ALARMOUTTIMECFG");
//         bRet = TRUE;
//     }   
//     return bRet;
// }
// 
void CDlgProductAlarmHostCfg::SetAlarmInParamToWnd(NET_DVR_ALARMIN_PARAM& struAlarmInParam)
{
    int i = 0;
    for (i = 0; i < m_listAlarmOut.GetItemCount(); i++ )
    {
        if (1==  struAlarmInParam.byAssociateAlarmOut[i])
        {
            m_listAlarmOut.SetCheck(i, TRUE);
        }
        else
        {
            m_listAlarmOut.SetCheck(i, FALSE);
        }
    }

    for (i = 0; i < m_listSirenOut.GetItemCount(); i++)
    {
        if (1 == struAlarmInParam.byAssociateSirenOut[i])
        {
            m_listSirenOut.SetCheck(i, TRUE);
        }
        else
        {
            m_listSirenOut.SetCheck(i, FALSE);
        }
    }
    char szLan[40] = {0};
    strncpy(szLan, (char*)struAlarmInParam.byName, sizeof(struAlarmInParam.byName));
    m_csAlarmInName.Format("%s", szLan);
    for (i = 0; i < m_comboDetector.GetCount(); i++)
    {
        if (m_comboDetector.GetItemData(i) == struAlarmInParam.wDetectorType)
        {
            m_comboDetector.SetCurSel(i);
            break;
        }
    }
	if (struAlarmInParam.byType == 0xff)
	{
		m_comboDefenseType.SetCurSel(m_comboDefenseType.GetCount()-1);
	}
	else
	{
		m_comboDefenseType.SetCurSel(struAlarmInParam.byType);
	}
    m_comboDefenseType.SetCurSel(struAlarmInParam.byType);
    OnSelchangeComboDefenseType();
    m_iDelayTime = struAlarmInParam.dwParam;
    m_comboWeekday.SetCurSel(0);
    OnSelchangeComboweekday();
	m_cmSensitivity.SetCurSel(struAlarmInParam.bySensitivityParam);
	m_bGroupBypass = struAlarmInParam.byArrayBypass;
	m_chUploadReport = struAlarmInParam.byUploadAlarmRecoveryReport;
	UpdateData(FALSE);
}

void CDlgProductAlarmHostCfg::GetAlarmInParamFromWnd(NET_DVR_ALARMIN_PARAM& struAlarmInParam)
{
    int i = 0;
    for (i = 0; i < m_listAlarmOut.GetItemCount(); i++)
    {
        if (TRUE == m_listAlarmOut.GetCheck(i))
        {
            struAlarmInParam.byAssociateAlarmOut[i] = 1;
        }
        else
        {
            struAlarmInParam.byAssociateAlarmOut[i] = 0;
        }
    }
    
    for (i = 0; i < m_listSirenOut.GetItemCount(); i++)
    {
        if (TRUE == m_listSirenOut.GetCheck(i))
        {
            struAlarmInParam.byAssociateSirenOut[i] = 1;
        }
        else
        {
            struAlarmInParam.byAssociateSirenOut[i] = 0;
        }
    }
    strncpy((char*)struAlarmInParam.byName, m_csAlarmInName.GetBuffer(0), sizeof(struAlarmInParam.byName));
    struAlarmInParam.wDetectorType = m_comboDetector.GetItemData(m_comboDetector.GetCurSel());
    struAlarmInParam.byType = (m_comboDefenseType.GetCurSel()>7)?0xff:m_comboDefenseType.GetCurSel();
    struAlarmInParam.dwParam = m_iDelayTime;
    GetAlarmTimeFromWnd(&struAlarmInParam.struAlarmTime[0][0], m_comboWeekday.GetCurSel());

	struAlarmInParam.byArrayBypass = m_bGroupBypass;
	struAlarmInParam.bySensitivityParam = m_cmSensitivity.GetCurSel();
	struAlarmInParam.byUploadAlarmRecoveryReport = m_chUploadReport;
}

void CDlgProductAlarmHostCfg::GetAlarmInSetupFromWnd(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
{
    for (int i = 0; i < m_listAlarmIn.GetItemCount(); i++)
    {
        if (TRUE == m_listAlarmIn.GetCheck(i))
        {
            struAlarmInSetup.byAssiciateAlarmIn[i] = 1;
        }
        else
        {
            struAlarmInSetup.byAssiciateAlarmIn[i] = 0;
        }   
    }
}

void CDlgProductAlarmHostCfg::OnBtnSetAlarmInParam() 
{
    UpdateData(TRUE);
    GetAlarmInParamFromWnd(m_struAlarmInParam);
    if (!SetAlarmInParam(m_struAlarmInParam))
    {
        AfxMessageBox("Fail to set Alarm in associate");
    }
}

void CDlgProductAlarmHostCfg::OnSelchangeComboAlarmIn() 
{
    GetAlarmInParam(m_struAlarmInParam);
    SetAlarmInParamToWnd(m_struAlarmInParam);
    UpdateData(FALSE);
}


void CDlgProductAlarmHostCfg::OnBtnSetAlarmChan() 
{
    GetAlarmInSetupFromWnd(m_struAlarmInSetup);
    if (!AlarmHostSetAlarmChan(m_struAlarmInSetup))
    {
        AfxMessageBox("Fail to set alarm chan");
    }
}

void CDlgProductAlarmHostCfg::OnBtnCloseAlarmChan() 
{
    GetAlarmInSetupFromWnd(m_struAlarmInSetup);
    if (!AlarmHostCloseAlarmChan(m_struAlarmInSetup))
    {
        AfxMessageBox("Fail to clost alarm chan");
    }
}

// void CDlgProductAlarmHostCfg::OnBtnSetAlarmoutTime() 
// {
// /*    UpdateData(TRUE);*/
// //     m_struAlarmOutTimeCfg.wAlarmOutTime = m_iAlarmOutTime;
// //     SetAlarmOutTimeCfg(m_struAlarmOutTimeCfg);
// }


// void CDlgProductAlarmHostCfg::OnSelchangeComboAlarmOut() 
// {
// //     GetAlarmOutTimeCfg(m_struAlarmOutTimeCfg);
// //     m_iAlarmOutTime = (int)m_struAlarmOutTimeCfg.wAlarmOutTime;   
// //     UpdateData(FALSE);
// }

// void CDlgProductAlarmHostCfg::OnBtnOpenAlarmout() 
// {   
//     LONG lAlarmOutPort = m_comboAlarmOut.GetItemData(m_comboAlarmOut.GetCurSel());
//     if (!NET_DVR_SetAlarmOut(m_lServerID, lAlarmOutPort, 1))
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SetAlarmOut");
//         AfxMessageBox("Fail to set alarm out");
//     }
//     else
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SetAlarmOut");
//     }
// }

// void CDlgProductAlarmHostCfg::OnBtnCloseAlarmout() 
// {
//     LONG lAlarmOutPort = m_comboAlarmOut.GetItemData(m_comboAlarmOut.GetCurSel());
//     if (!NET_DVR_SetAlarmOut(m_lServerID, lAlarmOutPort, 0))
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SetAlarmOut");
//         AfxMessageBox("Fail to set alarm out");
//     }
//     else
//     {
//         g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SetAlarmOut");
//     }
// }


void CDlgProductAlarmHostCfg::OnBtnStatusQuery() 
{   
    CDlgAlarmHostStatus dlg;
    memcpy(&dlg.m_struAlarmHostAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_lServerID = m_lServerID;
    dlg.m_lStartChan = m_lStartChan;
    dlg.m_lChannel = m_lChannel;
    dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnAlarmoutCfg() 
{
    CDlgAlarmOutCfg dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    memcpy(&dlg.m_struAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));

    dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnAlarmhostAbility() 
{
    CDlgAlarmHostAbility dlg;
    memcpy(&dlg.m_struAlarmHostAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
	
}

void CDlgProductAlarmHostCfg::OnBtnSensorCfg() 
{
    CDlgSensorCfg dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_lChanNum = g_struDeviceInfo[m_iDevIndex].iAnalogChanNum;
    memcpy(&dlg.m_struDevAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnRs485Cfg() 
{
	CDlgAlarmHostRs485 dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    memcpy(&dlg.m_struDevAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
}


void CDlgProductAlarmHostCfg::OnBtnNetUser() 
{
    CDlgAlarmNetUser dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_dwNetUserNum = m_struAlarmHostAbility.wNetUserNum;
    dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnKeyboardUser() 
{
    CDlgAlarmKeyboardUser dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_dwKeyboardUserNum = m_struAlarmHostAbility.wKeyboardNum;
    dlg.m_dwAlarmInNum = (m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum);
    dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnOperatorUser() 
{
    CDlgAlarmOperatorUser dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_dwOperatorUserNum = m_struAlarmHostAbility.wOperatorUserNum;
    dlg.m_dwAlarmInNum = m_struAlarmHostAbility.wLocalAlarmInNum + m_struAlarmHostAbility.wExpandAlarmInNum;
    dlg.m_iSubSystemNum = m_struAlarmHostAbility.wSubSystemNum;
    dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnBattaryVoltage() 
{
    char szLan[128] = {0};
    if (GetBatteryVoltage())
    {
        sprintf(szLan, "Battery voltage %0.3f", m_fBatteryVoltage);
        AfxMessageBox(szLan);
    }
}

BOOL CDlgProductAlarmHostCfg::GetBatteryVoltage()
{
   if (NET_DVR_GetBatteryVoltage(m_lServerID, &m_fBatteryVoltage))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GetBatteryVoltage");
        return TRUE;
    } 
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetBatteryVoltage");
        return FALSE;
    }
}

BOOL CDlgProductAlarmHostCfg::BypassAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
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

BOOL CDlgProductAlarmHostCfg::UnBypassAlarmChan(NET_DVR_ALARMIN_SETUP &struAlarmInSetup)
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

void CDlgProductAlarmHostCfg::OnBtnBypass() 
{
    GetAlarmInSetupFromWnd(m_struAlarmInSetup);
    if (!BypassAlarmChan(m_struAlarmInSetup))
    {
        AfxMessageBox("Fail to bypass alarm chan");
    }

}

void CDlgProductAlarmHostCfg::OnBtnUnBypass() 
{
    GetAlarmInSetupFromWnd(m_struAlarmInSetup);
    if (!UnBypassAlarmChan(m_struAlarmInSetup))
    {
        AfxMessageBox("Fail to  Unbypass alarm chan");
    }
}

void CDlgProductAlarmHostCfg::OnBtnAlarmhostEnablecfg() 
{
    CDlgAlarmHostEnableCfg dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    memcpy(&dlg.m_struAbility, &m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility));
    dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnRclickListAlarmOut(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CMenu pMenu;
    
    if (!pMenu.LoadMenu(IDR_MENU_SELECT_MODE))
    {
        return;
	}
    CPoint point;
    GetCursorPos(&point);
    pMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}

void CDlgProductAlarmHostCfg::OnMenuSelectAll() 
{
	for (int i = 0; i < m_listAlarmOut.GetItemCount(); i++)
	{
        m_listAlarmOut.SetCheck(i, TRUE);
	}
}

void CDlgProductAlarmHostCfg::OnMenuCancelAll() 
{
    for (int i = 0; i < m_listAlarmOut.GetItemCount(); i++)
    {
        m_listAlarmOut.SetCheck(i, FALSE);
    }
	
}

void CDlgProductAlarmHostCfg::OnTimeOk() 
{
    UpdateData(TRUE);
//     if (!F_TimeTest())
//     {
//         return;
//     }
    int	i = m_comboWeekday.GetCurSel();	
    GetAlarmTimeFromWnd(&m_struAlarmInParam.struAlarmTime[0][0], i);
}

void CDlgProductAlarmHostCfg::OnTimeCopy() 
{
    UpdateData(TRUE);
    int j,k;
    int i = m_comboWeekday.GetCurSel();
    int nCopyTime = m_comboCopyTime.GetCurSel() - 1;
    
    if(nCopyTime == -1)
    {
        for(j=0; j<MAX_DAYS; j++)
        {
            if(j == i)
                continue;
            for(k=0; k<MAX_TIMESEGMENT; k++)
            {
                memcpy(&(m_struAlarmInParam.struAlarmTime[j][k]), &(m_struAlarmInParam.struAlarmTime[i][k]), sizeof(NET_DVR_SCHEDTIME));
            }
        }
    }
    else
    {
        for(k=0; k<MAX_TIMESEGMENT; k++)
        {
            memcpy(&(m_struAlarmInParam.struAlarmTime[nCopyTime][k]), &(m_struAlarmInParam.struAlarmTime[i][k]), sizeof(NET_DVR_SCHEDTIME));
        }
    }
}

BOOL CDlgProductAlarmHostCfg::F_TimeTest()
{
    int i, j;
    WORD wStartTime[4], wStopTime[4];
    
    UpdateData(TRUE);
    if( (m_iHour11 < 0) || (m_iHour11 > 24) || (m_iHour12 < 0) || (m_iHour12 > 24) ||	\
        (m_iHour21 < 0) || (m_iHour21 > 24) || (m_iHour22 < 0) || (m_iHour22 > 24) )
    {
        AfxMessageBox("Hours range 0 - 24 !");
        return FALSE;
    }
    if( (m_iMin11 < 0) || (m_iMin11 > 60) || (m_iMin12 < 0) || (m_iMin12 > 60) ||	\
        (m_iMin21 < 0) || (m_iMin21 > 60) || (m_iMin22 < 0) || (m_iMin22 > 60) )
    {
        AfxMessageBox("Minute range 0 - 60!");
        return FALSE;
    }
    wStartTime[0] = (WORD)(m_iHour11 * 100 + m_iMin11);
    wStartTime[1] = (WORD)(m_iHour21 * 100 + m_iMin21);
    wStopTime[0] = (WORD)(m_iHour12 * 100 + m_iMin12);
    wStopTime[1] = (WORD)(m_iHour22 * 100 + m_iMin22);
    for(i = 0; i < MAX_TIMESEGMENT; i++)
    {
        if( (wStartTime[i] > 2400) || (wStopTime[i] > 2400) )
        {
            char szLan[128] = {0};
            g_StringLanType(szLan, "布防时间不能超过24:00", "Fortify time can not be more than 24:00");
            AfxMessageBox(szLan);
            return FALSE;
        }
        if(wStartTime[i] > wStopTime[i])
        {
            char szLan[128] = {0};
            g_StringLanType(szLan, "布防起始时间不能大于停止时间", "Fortify start time can not be larger than the record to stop time");
            AfxMessageBox(szLan);
            return FALSE;
        }
        for(j = 0; j < MAX_TIMESEGMENT; j++)
        {
            if (i == j)
                continue;
            if( (wStartTime[i] > wStartTime[j]) && (wStartTime[i] < wStopTime[j]) ||	\
                (wStopTime[i] > wStartTime[j]) && (wStopTime[i] < wStopTime[j]) )
            {
                char szLan[128] = {0};
                g_StringLanType(szLan, "布防时间段不能重复!","Fortify time period can not be repeated!");
                AfxMessageBox(szLan);
                return FALSE;
            }
        }
    }
    return TRUE;
}

void CDlgProductAlarmHostCfg::SetAlarmTimeToWnd(NET_DVR_SCHEDTIME const *pAlarmTime, int nDay)
{
    m_iHour11 = pAlarmTime[nDay*4+0].byStartHour;
    m_iMin11 = pAlarmTime[nDay*4+0].byStartMin;
    m_iHour12 = pAlarmTime[nDay*4+0].byStopHour;
    m_iMin12 = pAlarmTime[nDay*4+0].byStopMin;
    m_iHour21 = pAlarmTime[nDay*4+1].byStartHour;
    m_iMin21 = pAlarmTime[nDay*4+1].byStartMin;
    m_iHour22 = pAlarmTime[nDay*4+1].byStopHour;
    m_iMin22 = pAlarmTime[nDay*4+1].byStopMin;
    m_iHour31 = pAlarmTime[nDay*4+2].byStartHour;
    m_iMin31 = pAlarmTime[nDay*4+2].byStartMin;
    m_iHour32 = pAlarmTime[nDay*4+2].byStopHour;
    m_iMin32 = pAlarmTime[nDay*4+2].byStopMin;
    m_iHour41 = pAlarmTime[nDay*4+3].byStartHour;
    m_iMin41 = pAlarmTime[nDay*4+3].byStartMin;
    m_iHour42 = pAlarmTime[nDay*4+3].byStopHour;
    m_iMin42 = pAlarmTime[nDay*4+3].byStopMin;

}

void CDlgProductAlarmHostCfg::GetAlarmTimeFromWnd(NET_DVR_SCHEDTIME *pAlarmTime, int nDay)
{
    pAlarmTime[nDay*4+0].byStartHour = (BYTE)m_iHour11;
    pAlarmTime[nDay*4+0].byStartMin = (BYTE)m_iMin11;
    pAlarmTime[nDay*4+0].byStopHour = (BYTE)m_iHour12;
    pAlarmTime[nDay*4+0].byStopMin = (BYTE)m_iMin12;

    pAlarmTime[nDay*4+1].byStartHour = (BYTE)m_iHour21;
    pAlarmTime[nDay*4+1].byStartMin = (BYTE)m_iMin21;
    pAlarmTime[nDay*4+1].byStopHour = (BYTE)m_iHour22;
    pAlarmTime[nDay*4+1].byStopMin = (BYTE)m_iMin22;

    pAlarmTime[nDay*4+2].byStartHour = (BYTE)m_iHour31;
    pAlarmTime[nDay*4+2].byStartMin = (BYTE)m_iMin31;
    pAlarmTime[nDay*4+2].byStopHour = (BYTE)m_iHour32;
    pAlarmTime[nDay*4+2].byStopMin = (BYTE)m_iMin32;

    pAlarmTime[nDay*4+3].byStartHour = (BYTE)m_iHour41;
    pAlarmTime[nDay*4+3].byStartMin = (BYTE)m_iMin41;
    pAlarmTime[nDay*4+3].byStopHour = (BYTE)m_iHour42;
    pAlarmTime[nDay*4+3].byStopMin = (BYTE)m_iMin42;
}


void CDlgProductAlarmHostCfg::OnSelchangeComboweekday() 
{
    int	i = m_comboWeekday.GetCurSel();	
    SetAlarmTimeToWnd(&m_struAlarmInParam.struAlarmTime[0][0], i);
    UpdateData(FALSE);

}

void CDlgProductAlarmHostCfg::OnSelchangeCombocopytime() 
{

}

void CDlgProductAlarmHostCfg::OnSelchangeComboDefenseType() 
{
    if (m_comboDefenseType.GetCurSel() == 2)
    {
        GetDlgItem(IDC_EDIT_DELAY_TIME)->EnableWindow(TRUE); 
    }
    else
    {
        GetDlgItem(IDC_EDIT_DELAY_TIME)->EnableWindow(FALSE);
    }
}

void CDlgProductAlarmHostCfg::OnBtnGatewayCfg() 
{
	CDlgGatewayCfg dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_dwGatawayNum = m_struAlarmHostAbility.wGatewayNum;
    dlg.DoModal();
}


#include "DlgAlarmHostDialParam.h"
void CDlgProductAlarmHostCfg::OnButtonDialparam() 
{
    CDlgAlarmHostDialParam dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
	
}

void CDlgProductAlarmHostCfg::OnBtnBypasssetup() 
{
	// TODO: Add your control notification handler code here
	if (!NET_DVR_AlarmHostArrayBypass(g_struDeviceInfo[m_iDevIndex].lLoginID))
	{
		char szLan[128] = {0};
		g_StringLanType(szLan, "组旁路失败" , "NET_DVR_AlarmHostArrayBypass FAILED");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AlarmHostArrayBypass FAILED");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AlarmHostArrayBypass SUCC");
	}
}



void CDlgProductAlarmHostCfg::OnBtnBypassStopsetup() 
{
	// TODO: Add your control notification handler code here
	char szLan[1024] = {0};
	if (!NET_DVR_AlarmHostArrayBypassResume(g_struDeviceInfo[m_iDevIndex].lLoginID))
	{
		g_StringLanType(szLan, "撤销组旁路失败" , "NET_DVR_AlarmHostArrayBypassResume FAILED");
		MessageBox(szLan);
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_AlarmHostArrayBypassResume FAILED");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_AlarmHostArrayBypassResume SUCC");
	}
}
#include "DlgAlarmhostSubsystem.h"
void CDlgProductAlarmHostCfg::OnBtnSubsystemCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmhostSubsystem dlg;
	dlg.DoModal();
}



void CDlgProductAlarmHostCfg::OnBtnReportmode() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostReportMode dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnGprspara() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostGPRS dlg;
	dlg.DoModal();
}


void CDlgProductAlarmHostCfg::OnBtnNetcfg() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostNetCfg dlg;
	dlg.DoModal();
}

#include "DlgAlarmHostZoomAddr.h"
void CDlgProductAlarmHostCfg::OnBtnZoomCode() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostZoneAddr dlg;
	dlg.DoModal();
}
#include "DlgAlarmHostTriggerCode.h"
void CDlgProductAlarmHostCfg::OnBtnTriggerCode() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostTriggerCode dlg;
	dlg.DoModal();
}

#include "DlgAlarmHostFaultProcess.h"
void CDlgProductAlarmHostCfg::OnBtnFaultProcess() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostFaultProcess dlg;
	dlg.DoModal();
}

#include "DlgAlarmHostEventTrigger.h"
void CDlgProductAlarmHostCfg::OnBtnEventTrigger() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostEventTrigger dlg;
	dlg.DoModal();
}

#include "DlgAlarmHostExternalModule.h"
void CDlgProductAlarmHostCfg::OnBtnExternModule() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostExternalModule dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnLed() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostLED dlg;
	dlg.DoModal();
}



void CDlgProductAlarmHostCfg::OnBtnAudioAssociate() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostAudioAssociateAlarm dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnSafetyCabin() 
{
	// TODO: Add your control notification handler code here
	CDlgSafetyCabin dlg;
	dlg.m_lUserID = m_lServerID;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnModuleCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmhostModuleCfg dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnExternalDeviceState() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostExternalDevState dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnExternalDeviceLimitValue() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmhostExternalDevLimitValue dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnSensorLinkage() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmhostSensorLinkage dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnPointCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmhostPointCfg dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnUploadMode() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmhostDataUploadMode dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnHistoryData() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmhostHistoryData dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::OnBtnPrinter() 
{
	// TODO: Add your control notification handler code here
	CDlgAlarmHostPrinter dlg;
	dlg.DoModal();
}

void CDlgProductAlarmHostCfg::UpdatePapam()
{
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex < 0)
    {
        return;
    }

    int iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (iChanIndex < 0)
    {
        iChanIndex = 0;//default to config channel one
    }
    m_iDevIndex = iDeviceIndex;
    m_lServerID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    m_lStartChan = g_struDeviceInfo[iDeviceIndex].iStartChan;
    m_lChannel= iChanIndex+g_struDeviceInfo[iDeviceIndex].iStartChan;
	
	if (!m_bInit)
	{
		if(TRUE == GetAlarmHostAbility(m_struAlarmHostAbility))
		{
			m_bInit = TRUE;
			InitWnd();
			if (m_struAlarmHostAbility.by3GNum > 0)
			{
				g_StringLanType(m_szLan, "3G参数配置", "3G Config");
				SetDlgItemText(IDC_BTN_GPRSPARA, m_szLan);
			}
			else
			{
				g_StringLanType(m_szLan, "GPRS参数配置", "GPRS Config");
				SetDlgItemText(IDC_BTN_GPRSPARA, m_szLan);
			}
			
			
			GetAlarmInParam(m_struAlarmInParam);
			SetAlarmInParamToWnd(m_struAlarmInParam);
		}
	}
    UpdateData(FALSE);
}

void CDlgProductAlarmHostCfg::OnButAlarmLogSearch() 
{
	// TODO: Add your control notification handler code here
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex == -1)
    {
        return;
    }
    CDlgAlarmHostLogSearch dlg;
    dlg.m_lServerID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    dlg.m_iDevIndex = iDeviceIndex;
    dlg.DoModal();
}
