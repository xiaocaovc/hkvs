/**********************************************************
FileName:    SubDlgDeviceCfg.cpp
Description: device info configuration    
Date:        2008/05/17
Note: 		<Global>struct, macro refer to GeneralDef.h, global variants and API refer to ClientDemo.cpp   
Modification History:      
    <version> <time>         <desc>
    <1.0    > <2008/05/17>       <created>
***********************************************************/


#include "stdafx.h"
#include "ClientDemo.h"
#include "SubDlgDeviceCfg.h"
#include ".\subdlgdevicecfg.h"
#include "DlgRemoteZone.h"
#include "DlgRtspConfig.h"
#include "DlgDeviceAudioTalkCode.h"
#include "DlgCcdParam.h"
#include "DlgJpegFTP.h"
#include "DlgAES.h"
#include "DlgAutoReboot.h"
#include "DlgCMSParam.h"
#include "DlgXmlAbility.h"

// CSubDlgDeviceCfg dialog
/*********************************************************
  Function:	CSubDlgDeviceCfg
  Desc:		Constructor
  Input:	
  Output:	
  Return:	
**********************************************************/
IMPLEMENT_DYNAMIC(CSubDlgDeviceCfg, CDialog)
CSubDlgDeviceCfg::CSubDlgDeviceCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgDeviceCfg::IDD, pParent)
	, m_csDeviceName(_T(""))
	, m_iHdNum(0)
	, m_iYKQID(0)
	, m_csDeviceSerialNo(_T(""))
	, m_csSoftwareVersion(_T(""))
	, m_csHardwareVersion(_T(""))
	, m_csPanelVersion(_T(""))
	, m_csDspVersion(_T(""))
	, m_iAlarmInNum(0)
	, m_iAlarmOutNum(0)
	, m_iChanNum(0)
	, m_bDeviceCfgSet(FALSE)
	, m_iDeviceIndex(-1)//CheckInitParam, iut is required that m_iDeviceIndex is initialed as -1
	, m_lLoginID(-1)
	, m_iChanCount(0)
	, m_lStartChannel(0)
	, m_dwUseZoom(0)
	, m_bSupScale(FALSE)
	, m_csDevTypeName(_T(""))
	, m_lIpTest(-1)
	, m_hIpTest(NULL)
	, m_iZeroChanNum(0)
{
	memset(&m_struDeviceCfg, 0, sizeof(m_struDeviceCfg));
	memset(&m_struMenuOutputMode, 0, sizeof(m_struMenuOutputMode));
	memset(&m_struIpTest, 0, sizeof(m_struIpTest));
}

/*********************************************************
  Function:	~CSubDlgDeviceCfg
  Desc:		destructor
  Input:	
  Output:	
  Return:	
**********************************************************/
CSubDlgDeviceCfg::~CSubDlgDeviceCfg()
{
}

/*********************************************************
Function:	DoDataExchange
Desc:		the map between control and variable
Input:	
Output:	
Return:	
**********************************************************/
void CSubDlgDeviceCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgDeviceCfg)
	DDX_Control(pDX, IDC_COMBO_MENU_OUTPUT_MODE, m_cmbMenuOutputMode);
	DDX_Control(pDX, IDC_COMBO_STORAGE_MODE, m_comboStorageMode);
	DDX_Control(pDX, IDC_COMBO_IPC_PLUG, m_comboPlug);
	DDX_Control(pDX, IDC_COMBO_ESATA, m_comboEsata);
	DDX_Control(pDX, IDC_COMBO_DEV_SUPPORT, m_comboDevSupport);
	DDX_Control(pDX, IDC_COMBO_SUB_PROTOCOL, m_comboSubProtocol);
	DDX_Control(pDX, IDC_COMBO_MAIN_PROTOCOL, m_comboMainProtocol);
	DDX_Control(pDX, IDC_SUB_COMBO_MINOR_ZOOM, m_comboMinor);
	DDX_Text(pDX, IDC_SUB_EDIT_DEVICE_NAME, m_csDeviceName);
	DDX_Text(pDX, IDC_SUB_EDIT_HD_NUM, m_iHdNum);
	DDX_Text(pDX, IDC_SUB_EDIT_YKQID, m_iYKQID);
	DDX_Control(pDX, IDC_SUB_COMBO_USE_ZOOM, m_comboUseZoom);
	DDX_Text(pDX, IDC_SUB_EDIT_SERIAL, m_csDeviceSerialNo);
	DDX_Text(pDX, IDC_SUB_SOFTWARE_VERSION, m_csSoftwareVersion);
	DDX_Text(pDX, IDC_SUB_HARDWARE_VERSION, m_csHardwareVersion);
	DDX_Text(pDX, IDC_SUB_DSP_VERSION, m_csDspVersion);
	DDX_Text(pDX, IDC_SUB_ALARM_IN_NUM, m_iAlarmInNum);
	DDX_Text(pDX, IDC_SUB_ALARM_OUT_NUM, m_iAlarmOutNum);
	DDX_Text(pDX, IDC_SUB_EDIT_CHAN_NUM, m_iChanNum);
	DDX_Control(pDX, IDC_SUB_CYCLE_RECORD, m_comboCycleRec);
	DDX_Text(pDX, IDC_SUB_QMB_VERSION, m_csPanelVersion);
	DDX_Text(pDX, IDC_EDIT_ZEROCHAN_NUM, m_iZeroChanNum);
	DDX_Text(pDX, IDC_SUB_EDIT_DEV_TYPE_NAME, m_csDevTypeName);
	DDX_Text(pDX, IDC_SUB_LANGUAGE, m_csLanguageType);
	//}}AFX_DATA_MAP
}


/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CSubDlgDeviceCfg, CDialog)
	//{{AFX_MSG_MAP(CSubDlgDeviceCfg)	
	ON_BN_CLICKED(IDC_BTN_REMOTE_NET_OK, OnBnClickedBtnRemoteNetOk)
	ON_BN_CLICKED(IDC_BTN_ZONE, OnBtnZone)
	ON_BN_CLICKED(IDC_BTN_RTSP, OnBtnRtsp)
	ON_BN_CLICKED(IDC_BTN_AUDIOTALKCODET_CONFIG, OnBtnAudiotalkcodetConfig)
	ON_BN_CLICKED(IDC_BUTTON_JPEG_FTP, OnButtonJpegFtp)
	ON_BN_CLICKED(IDC_BUTTON_AES, OnButtonAes)
	ON_BN_CLICKED(IDC_BTN_AUTO_REBOOT, OnBtnAutoReboot)
	ON_BN_CLICKED(IDC_CMS_PARMA, OnCmsParma)
	ON_BN_CLICKED(IDC_BTN_DEV_ABILITY, OnBtnDevAbility)
	ON_BN_CLICKED(IDD_DLG_HOLIDAY, OnDlgHoliday)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CSubDlgDeviceCfg message handlers
/*********************************************************
Function:	OnInitDialog
Desc:		Initialize the dialog
Input:	
Output:	
Return:	
**********************************************************/
BOOL CSubDlgDeviceCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);

	return TRUE;
}

inline BOOL CSubDlgDeviceCfg::ValidIPv6(BYTE *ip)
{
	if (strlen((char *)ip) >= 2 && strchr((char *)ip, ':') != NULL)
	{
		return TRUE;
	}
	return FALSE;
}

/*********************************************************
Function:	CheckInitParam
Desc:		detect device parameters and update parameter display on the dialog box
Input:	
Output:	
Return:	TRUE,correct parameter, update paramerter display; FALSE, do not initial
**********************************************************/
BOOL CSubDlgDeviceCfg::CheckInitParam()
{
	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	DWORD dwReturned = 0;
	CString sTemp;
	char szLan[128] = {0};

	if (iDeviceIndex == -1)
	{
		return FALSE;
	}

	if (m_iDeviceIndex == iDeviceIndex)
	{
		return TRUE;
	}
	else
	{
		//update parameter display on the control
		m_iDeviceIndex = iDeviceIndex;
		m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
		m_lStartChannel = g_struDeviceInfo[m_iDeviceIndex].iStartChan;
		m_iChanCount = g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum;
	}

	m_csDeviceName = _T("");
	m_iHdNum = 0;
	m_iYKQID = 0;
	m_csDeviceSerialNo = _T("");
	m_csSoftwareVersion = _T("");
	m_csHardwareVersion = _T("");
	m_csPanelVersion = _T("");
	m_csDspVersion = _T("");
	m_iAlarmInNum = 0;
	m_iAlarmOutNum = 0;
	m_iChanNum = 0;
	m_csDevTypeName = _T("");
    m_csLanguageType = _T("");

	m_comboUseZoom.SetCurSel(-1);

	if (m_lLoginID < 0)
	{
		return FALSE;
	}

	m_bSetDeviceCfg = FALSE;
	memset(&m_struDeviceCfg, 0, sizeof(m_struDeviceCfg));
	m_dwUseZoom = 0;
	if (!NET_DVR_GetDVRConfig(m_lLoginID, NET_DVR_GET_DEVICECFG_V40, 0, &m_struDeviceCfg, sizeof(m_struDeviceCfg), &dwReturned))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_DEVICECFG_V40");
		g_StringLanType(szLan, "获取设备参数失败", "Get Device Configure failed");
		AfxMessageBox(szLan);
	//	return FALSE;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_DEVICECFG_V40");
	}


	if (!NET_DVR_GetScaleCFG_V30(m_lLoginID, &m_struScale))
	{
		m_bSupScale = FALSE;
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetScaleCFG_V30");
	}
	else
	{
		m_bSupScale = TRUE;	
	}

	if (!NET_DVR_GetDVRConfig(m_lLoginID, NET_DVR_GET_MEMU_OUTPUT_MODE, 0,&m_struMenuOutputMode, sizeof(NET_DVR_MENU_OUTPUT_MODE), &dwReturned))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_MEMU_OUTPUT_MODE");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_MEMU_OUTPUT_MODE");
		m_cmbMenuOutputMode.SetCurSel(m_struMenuOutputMode.byMenuOutputMode);
	}

	UpdateData(FALSE);
	return TRUE;
}

/*********************************************************
Function:	CurCfgUpdate
Desc:		update parameter and refresh control display
Input:	
Output:	
Return:	
**********************************************************/
void CSubDlgDeviceCfg::CurCfgUpdate()
{
	if (!CheckInitParam())
	{
		UpdateData(FALSE);
		EnableWindow(FALSE);
		return;
	}
	EnableWindow(TRUE);
	
	char cTemp[200] = {0};
	if (m_bSupScale)
	{		
		GetDlgItem(IDC_STATICSCALE)->ShowWindow(SW_SHOW);
		m_comboUseZoom.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_MAJOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_MINOR)->ShowWindow(SW_SHOW);
		m_comboMinor.ShowWindow(SW_SHOW);
		m_comboUseZoom.SetCurSel(m_struScale.dwMajorScale);	
		m_comboMinor.SetCurSel(m_struScale.dwMinorScale);	
	}
	else
	{
		GetDlgItem(IDC_STATICSCALE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MAJOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_MINOR)->ShowWindow(SW_HIDE);
		m_comboUseZoom.ShowWindow(SW_HIDE);
		m_comboMinor.ShowWindow(SW_HIDE);
	}

    //2010-01-16
    char szLan[128] = {0};
    m_comboDevSupport.ResetContent();
    if (m_struDeviceCfg.bySupport & 0x01 == 1)
    {
        g_StringLanType(szLan, "支持移动侦测搜索", "Motion Search");
        m_comboDevSupport.AddString(szLan);
    }

    if (((m_struDeviceCfg.bySupport >> 1) & 0x01) == 1)
    {
        g_StringLanType(szLan, "支持备份", "Backup");
        m_comboDevSupport.AddString(szLan);
    }

    if (((m_struDeviceCfg.bySupport >> 2) & 0x01) == 1)
    {
        g_StringLanType(szLan, "支持压缩能力", "Compression Ability");
        m_comboDevSupport.AddString(szLan);
    }

    if (((m_struDeviceCfg.bySupport >> 4) & 0x01) == 1)
    {
        g_StringLanType(szLan, "远程SADP", "Remote SADP");
        m_comboDevSupport.AddString(szLan);
    }

    if (((m_struDeviceCfg.bySupport >> 5) & 0x01) == 1)
    {
        g_StringLanType(szLan, "Raid", "Raid");
        m_comboDevSupport.AddString(szLan);
    }

    if ((m_struDeviceCfg.bySupport >> 6 & 0x01) == 1)
    {
        m_comboDevSupport.AddString("IPSAN Search");
    }

	if ((m_struDeviceCfg.bySupport >> 7 & 0x01) == 1)
	{
		m_comboDevSupport.AddString("RTP Over RTSP");
    }

	if (m_struDeviceCfg.bySupport1 & 0x01 == 1)
    {
        m_comboDevSupport.AddString("SNMP V30");
    }

    if (((m_struDeviceCfg.bySupport >> 1) & 0x01) == 1)
    {
        g_StringLanType(szLan, "区分回放和下载", "distinguish playback from download");
        m_comboDevSupport.AddString(szLan);
    }

	m_comboEsata.ResetContent();
	g_StringLanType(szLan, "备份", "Backup");
	m_comboEsata.AddString(szLan);
	g_StringLanType(szLan, "录像", "Record");
	m_comboEsata.AddString(szLan);

    m_comboSubProtocol.SetCurSel(g_struDeviceInfo[m_iDeviceIndex].bySubProto);
    m_comboMainProtocol.SetCurSel(g_struDeviceInfo[m_iDeviceIndex].byMainProto);
	m_comboPlug.SetCurSel(m_struDeviceCfg.byIPCPlug);
	m_comboEsata.SetCurSel(m_struDeviceCfg.byEsataUseage);
    m_iZeroChanNum = m_struDeviceCfg.byZeroChanNum;

	m_comboCycleRec.SetCurSel(m_struDeviceCfg.dwRecycleRecord);
	ZeroMemory(cTemp, 200);
	memcpy(cTemp, m_struDeviceCfg.sDVRName, NAME_LEN);
	m_csDeviceName.Format("%s", cTemp);
	m_iYKQID = m_struDeviceCfg.dwDVRID;
	ZeroMemory(cTemp, 200);
	memcpy(cTemp, m_struDeviceCfg.sSerialNumber, SERIALNO_LEN);
	m_csDeviceSerialNo.Format("%s", cTemp);
	memcpy(cTemp,m_struDeviceCfg.byDevTypeName,sizeof(m_struDeviceCfg.byDevTypeName));
	m_csDevTypeName.Format("%s",cTemp);

	if (((m_struDeviceCfg.dwSoftwareVersion>>24)&0xFF) > 0)
	{
		m_csSoftwareVersion.Format("V%d.%d.%d build %02d%02d%02d",\
			(m_struDeviceCfg.dwSoftwareVersion>>24)&0xFF, (m_struDeviceCfg.dwSoftwareVersion>>16)&0xFF, m_struDeviceCfg.dwSoftwareVersion&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>16)&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>8)&0xFF,m_struDeviceCfg.dwSoftwareBuildDate&0xFF);
	}
	else
	{
		m_csSoftwareVersion.Format("V%d.%d build %02d%02d%02d",\
			(m_struDeviceCfg.dwSoftwareVersion>>16)&0xFFFF, m_struDeviceCfg.dwSoftwareVersion&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>16)&0xFFFF, (m_struDeviceCfg.dwSoftwareBuildDate>>8)&0xFF,m_struDeviceCfg.dwSoftwareBuildDate&0xFF);
	}
	m_csDspVersion.Format("V%d.%d build %02d%02d%02d", (m_struDeviceCfg.dwDSPSoftwareVersion>>16)&0xFFFF, m_struDeviceCfg.dwDSPSoftwareVersion&0xFFFF, (m_struDeviceCfg.dwDSPSoftwareBuildDate>>16)&0xFFFF-2000, (m_struDeviceCfg.dwDSPSoftwareBuildDate>>8)&0xFF,m_struDeviceCfg.dwDSPSoftwareBuildDate&0xFF);	
	m_csHardwareVersion.Format("0x%x", m_struDeviceCfg.dwHardwareVersion);
	m_csPanelVersion.Format("V%d", m_struDeviceCfg.dwPanelVersion);

	m_iAlarmInNum = m_struDeviceCfg.byAlarmInPortNum;
	m_iAlarmOutNum = m_struDeviceCfg.byAlarmOutPortNum;
	m_iChanNum = m_struDeviceCfg.byChanNum;	
    m_comboStorageMode.SetCurSel(m_struDeviceCfg.byStorageMode);
    if (m_iDeviceIndex > -1)
    {
        if ( g_struDeviceInfo[m_iDeviceIndex].byLanguageType & 0x1)
        {
            m_csLanguageType.Format("CN");
        }
        else if ( g_struDeviceInfo[m_iDeviceIndex].byLanguageType & 0x2)
        {
            m_csLanguageType.Format("EN");
        }
    }

    m_csDevTypeName = m_struDeviceCfg.byDevTypeName;

	m_iHdNum = m_struDeviceCfg.byDiskNum;

	UpdateData(FALSE);
}

/*********************************************************
  Function:	CfgSetup
  Desc:		users need to click device info box and click "OK" on the network configurtaion area
  Input:	
  Output:	
  Return:	TRUE,setting succeed;FALSE, setting failed;
**********************************************************/
BOOL CSubDlgDeviceCfg::CfgSetup()
{
	CString sTemp;
	BOOL bRet = TRUE;
	char szLan[128] = {0};
	
	if (m_bSupScale)
	{
		m_struScale.dwMajorScale = m_comboUseZoom.GetCurSel();
		m_struScale.dwMinorScale = m_comboMinor.GetCurSel();
		if (!NET_DVR_SetScaleCFG_V30(m_lLoginID, &m_struScale))
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SetScaleCFG_V30");
			bRet = FALSE;
			//AfxMessageBox("enable scale failed!");		
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SetScaleCFG_V30");
		}
	}

	if (m_bDeviceCfgSet)
	{
		if (!NET_DVR_SetDVRConfig(m_lLoginID, NET_DVR_SET_DEVICECFG_V40, 0,&(m_struDeviceCfg), sizeof(m_struDeviceCfg)))
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_DEVICECFG_V40");
			//AfxMessageBox("save device info failed!");
			bRet = FALSE;
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_DEVICECFG_V40");
		}
	}


	m_struMenuOutputMode.dwSize = sizeof(m_struMenuOutputMode);
	if (!NET_DVR_SetDVRConfig(m_lLoginID, NET_DVR_SET_MEMU_OUTPUT_MODE, 0, &(m_struMenuOutputMode), sizeof(NET_DVR_MENU_OUTPUT_MODE)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_MEMU_OUTPUT_MODE");
		bRet = FALSE;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_MEMU_OUTPUT_MODE");
	}
	
	return bRet;
}

/*********************************************************
  Function:	OnBnClickedBtnRemoteNetOk
  Desc:		Confirm network parameter settings
  Input:	
  Output:	
  Return:	
**********************************************************/
void CSubDlgDeviceCfg::OnBnClickedBtnRemoteNetOk()
{
	UpdateData(TRUE);
	char szLan[128] = {0};

    m_struDeviceCfg.byIPCPlug = m_comboPlug.GetCurSel();
    m_struDeviceCfg.byEsataUseage = m_comboEsata.GetCurSel(); 
	m_struDeviceCfg.byZeroChanNum = m_iZeroChanNum;
    m_struDeviceCfg.byStorageMode = m_comboStorageMode.GetCurSel();

	m_struDeviceCfg.dwDVRID = m_iYKQID;
	memcpy(m_struDeviceCfg.sDVRName, m_csDeviceName, NAME_LEN);	
	m_struDeviceCfg.dwRecycleRecord = m_comboCycleRec.GetCurSel();
	if (m_bSupScale)
	{
		m_dwUseZoom = m_comboUseZoom.GetCurSel();	
	}
	m_bDeviceCfgSet = TRUE;
	g_bMustReboot = g_pMainDlg->IsNeedRebootDevType();
}

/*********************************************************
  Function:	OnBtnZone
  Desc:		to configure the zone and dst
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CSubDlgDeviceCfg::OnBtnZone() 
{
	// TODO: Add your control notification handler code here
	CDlgRemoteZone dlg;
	dlg.m_iDeviceIndex = m_iDeviceIndex;
	dlg.DoModal();
}

void CSubDlgDeviceCfg::OnBtnRtsp() 
{
	// TODO: Add your control notification handler code here
	CDlgRtspConfig dlg;
	dlg.m_iDeviceIndex = m_iDeviceIndex;
	dlg.DoModal();
}

void CSubDlgDeviceCfg::OnBtnAudiotalkcodetConfig() 
{
	// TODO: Add your control notification handler code here
	CDlgDeviceAudioTalkCode dlg;
	dlg.m_iDeviceIndex = m_iDeviceIndex;
	dlg.DoModal();
}

// void CSubDlgDeviceCfg::OnBtnCCDConfig()
// {
//     CDlgCcdParam dlg;
// 	dlg.m_iDeviceIndex = m_iDeviceIndex;
// 	dlg.DoModal();
// }

void CSubDlgDeviceCfg::OnButtonJpegFtp() 
{
	CJpegFTP dlg;
	dlg.m_lServerID = m_lLoginID;
	DWORD m_dwDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	dlg.m_lStartChannel = g_struDeviceInfo[m_dwDeviceIndex].iStartChan;
	dlg.DoModal();
}

void CSubDlgDeviceCfg::OnButtonAes() 
{
	CDlgAES dlg;
	dlg.m_lUSerID = m_lLoginID;
    dlg.m_dwDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    dlg.DoModal();	
}

void CSubDlgDeviceCfg::OnBtnAutoReboot() 
{
	// TODO: Add your control notification handler code here
	CDlgAutoReboot dlg;
	dlg.m_lUserID = m_lLoginID;
	dlg.m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	dlg.DoModal();
}

void CSubDlgDeviceCfg::OnCmsParma() 
{
	// TODO: Add your control notification handler code here
	DlgCMSParam dlg;
	dlg.DoModal();
}

void CSubDlgDeviceCfg::OnBtnDevAbility() 
{
	CDlgXmlAbility dlg;
    dlg.m_lServerID = m_lLoginID;
    dlg.m_iDevIndex = g_pMainDlg->GetCurDeviceIndex();
    dlg.DoModal();
}
#include "DlgHolidayCfg.h"
void CSubDlgDeviceCfg::OnDlgHoliday() 
{
	// TODO: Add your control notification handler code here
    CDlgHolidayCfg dlg;
    dlg.m_lServerID = m_lLoginID;
    dlg.m_iDevIndex = g_pMainDlg->GetCurDeviceIndex();
    dlg.DoModal();
}
