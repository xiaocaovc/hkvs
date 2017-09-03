// DlgAlarmHostNetCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmHostNetCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostNetCfg dialog


CDlgAlarmHostNetCfg::CDlgAlarmHostNetCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmHostNetCfg::IDD, pParent)
	, m_iDeviceIndex(-1)
	, m_lUserID(-1)
{
	//{{AFX_DATA_INIT(CDlgAlarmHostNetCfg)
	m_csDeviceID = _T("");
	m_iPort = 0;
	//}}AFX_DATA_INIT
	memset(&m_struNetCfg, 0, sizeof(m_struNetCfg));
	memset(&m_struAbility, 0, sizeof(m_struAbility));
}


void CDlgAlarmHostNetCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmHostNetCfg)
	DDX_Control(pDX, IDC_IPADDRESS_IPV4, m_ipCenter);
	DDX_Control(pDX, IDC_COMBO_PROTOCOLTYPE, m_cmProtocolType);
	DDX_Control(pDX, IDC_COMBO_CENTER, m_cmCenter);
	DDX_Text(pDX, IDC_EDIT_DEVICEID, m_csDeviceID);
	DDX_Text(pDX, IDC_EDIT_PORT, m_iPort);
	DDV_MinMaxInt(pDX, m_iPort, 0, 65535);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmHostNetCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmHostNetCfg)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTOCOLTYPE, OnSelchangeComboProtocoltype)
	ON_CBN_SELCHANGE(IDC_COMBO_CENTER, OnSelchangeComboCenter)
	ON_BN_CLICKED(IDC_BTN_SURE, OnBtnSure)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostNetCfg message handlers

BOOL CDlgAlarmHostNetCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	m_struAbility.dwSize = sizeof(m_struAbility);
	if (!NET_DVR_GetDeviceAbility(m_lUserID, ALARMHOST_ABILITY, NULL, 0, (char*)&m_struAbility, sizeof(m_struAbility)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Get Device Ability Failed");
	}
	else
	{
		CString csStr;
		char szLan[128] = {0};
		for (int i=0; i<m_struAbility.byNetNum; i++)
		{
			g_StringLanType(szLan, "中心", "CENTER");
			csStr.Format("%s%d", szLan, i+1);
			m_cmCenter.AddString(csStr);
		}
	}
	OnBtnGet();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmHostNetCfg::OnSelchangeComboProtocoltype() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwCenterIndex = m_cmCenter.GetCurSel();
	m_csDeviceID.Format("%s", m_struNetCfg.struNetCenter[dwCenterIndex].byDevID);
	UpdateData(FALSE);
}

void CDlgAlarmHostNetCfg::OnSelchangeComboCenter() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	NET_DVR_ALARMHOST_NETPARAM struNetParam = {0};

	memcpy(&struNetParam, &m_struNetCfg.struNetCenter[m_cmCenter.GetCurSel()], sizeof(struNetParam));

	DWORD dwIP = ntohl(inet_addr(struNetParam.struIP.sIpV4));
	m_ipCenter.SetAddress(dwIP);
	m_iPort = struNetParam.wPort;
	m_cmProtocolType.SetCurSel(struNetParam.byReportProtocol - 1);
	UpdateData(FALSE);
	OnSelchangeComboProtocoltype();
}


void CDlgAlarmHostNetCfg::OnBtnSure() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	NET_DVR_ALARMHOST_NETPARAM struNetParam = {0};
	DWORD dwIP = 0;
	m_ipCenter.GetAddress(dwIP);
	memcpy(struNetParam.struIP.sIpV4, IPToStr(dwIP), 16);
	struNetParam.wPort = m_iPort;
	struNetParam.byReportProtocol = m_cmProtocolType.GetCurSel() + 1;
	memcpy(struNetParam.byDevID, m_csDeviceID, ACCOUNTNUM_LEN);
	memcpy(&m_struNetCfg.struNetCenter[m_cmCenter.GetCurSel()], &struNetParam, sizeof(struNetParam));
}

void CDlgAlarmHostNetCfg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	DWORD dwReturn = 0;
	m_struNetCfg.dwSize = sizeof(NET_DVR_ALARMHOST_NETCFG);
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMHOST_NETCFG, 0, &m_struNetCfg, sizeof(m_struNetCfg), &dwReturn))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Get Net Parameter Failed");
		g_StringLanType(szLan, "获取网络参数失败", "Get Net Parameter Failed");
		MessageBox(szLan);
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_ALARMHOST_NETCFG SUCC");
		m_cmCenter.SetCurSel(0);
		UpdateData(FALSE);
		OnSelchangeComboCenter();
	}
}

void CDlgAlarmHostNetCfg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	OnBtnSure();
	m_struNetCfg.dwSize = sizeof(m_struNetCfg);
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMHOST_NETCFG, 0, &m_struNetCfg, sizeof(m_struNetCfg)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Set Net Parameter Failed");
		g_StringLanType(szLan, "设置网络参数失败", "Set Net Parameter Failed");
		MessageBox(szLan);
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set Net Parameter SUCC");
	}
}
