// DlgPosConnectMode.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPosConnectMode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPosConnectMode dialog


CDlgPosConnectMode::CDlgPosConnectMode(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPosConnectMode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPosConnectMode)
	m_bEnable = FALSE;
	m_csNetMonitorIPAddr = _T("");
	m_iUserID = -1;
	m_iDevIndex = -1;
	m_wNetReceivePort = 0;
	//}}AFX_DATA_INIT
}


void CDlgPosConnectMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPosConnectMode)
	DDX_Control(pDX, IDC_COMBO_POSNAME, m_comboPosName);
	DDX_Control(pDX, IDC_COMBO_RS232_STOPBIT, m_comboRS232StopBit);
	DDX_Control(pDX, IDC_COMBO_RS232_PARITY, m_comboRS232Parity);
	DDX_Control(pDX, IDC_COMBO_RS232_FLOWCTRL, m_comboRS232FlowCtrl);
	DDX_Control(pDX, IDC_COMBO_RS232_DATABIT, m_comboRS232DataBit);
	DDX_Control(pDX, IDC_COMBO_RS232_BAUDRATE, m_comboRS232BaudRate);
	DDX_Control(pDX, IDC_COMBO_RS2232_WORKMODE, m_comboRS232WorkMode);
	DDX_Control(pDX, IDC_COMBO_CONNECT_MODE, m_comboConnectMode);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bEnable);
	DDX_Text(pDX, IDC_EDIT_NET_MONITOR_IPADDR, m_csNetMonitorIPAddr);
	DDX_Text(pDX, IDC_EDIT_NET_RECEIVE_PORT, m_wNetReceivePort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPosConnectMode, CDialog)
	//{{AFX_MSG_MAP(CDlgPosConnectMode)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_CBN_SELCHANGE(IDC_COMBO_CONNECT_MODE, OnSelchangeComboConnectMode)
	ON_EN_KILLFOCUS(IDC_EDIT_NET_RECEIVE_PORT, OnKillfocusEditNetReceivePort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPosConnectMode message handlers

BOOL CDlgPosConnectMode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	memset(&m_struConnectPosCfg,0,sizeof(m_struConnectPosCfg));
	memset(&m_struPosFilterRuleCfg, 0, sizeof(m_struPosFilterRuleCfg));
	ShowConnectInfo(MODE_NET_RECEIVE);
	m_comboConnectMode.SetCurSel(1);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPosConnectMode::ShowConnectInfo(UINT dwType)
{

	switch(dwType)
	{
	case MODE_NET_RECEIVE:
		ShowNetReceiveInfo(TRUE);
		ShowNetMonitorInfo(FALSE);
		ShowRS232Info(FALSE);
		break;
	case MODE_NET_MONITER:
		ShowNetReceiveInfo(FALSE);
		ShowNetMonitorInfo(TRUE);
		ShowRS232Info(FALSE);
		break;
	case MODE_RS232:
		ShowNetReceiveInfo(FALSE);
		ShowNetMonitorInfo(FALSE);
		ShowRS232Info(TRUE);
		break;
	default:
		ShowNetReceiveInfo(TRUE);
		ShowNetMonitorInfo(FALSE);
		ShowRS232Info(FALSE);
		break;
	}
}

//显示网络接受相关信息
void CDlgPosConnectMode::ShowNetReceiveInfo(BOOL bShow)
{
	GetDlgItem(IDC_STATIC_NET_RECEIVE)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_NET_RECEIVE_PORT)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_NET_RECEIVE_PORT)->ShowWindow(bShow);
}

//设置网络监听相关信息
void CDlgPosConnectMode::ShowNetMonitorInfo(BOOL bShow)
{
	GetDlgItem(IDC_STATIC_NET_MONITOR)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_NET_MONITOR_IPADDR)->ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_NET_MONITOR_IPADDR)->ShowWindow(bShow);
}

//显示RS232相关信息
void CDlgPosConnectMode::ShowRS232Info(BOOL bShow)
{
	GetDlgItem(IDC_STATIC_RS232)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_RS232_BAUDRATE)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_RS232_DATABIT)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_RS232_FLOWCTRL)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_RS232_PARITY)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_RS232_STOPBIT)->ShowWindow(bShow);
	GetDlgItem(IDC_STATIC_RS2232_WORKMODE)->ShowWindow(bShow);


	GetDlgItem(IDC_COMBO_RS232_BAUDRATE)->ShowWindow(bShow);
	GetDlgItem(IDC_COMBO_RS232_DATABIT)->ShowWindow(bShow);
	GetDlgItem(IDC_COMBO_RS232_FLOWCTRL)->ShowWindow(bShow);
	GetDlgItem(IDC_COMBO_RS232_PARITY)->ShowWindow(bShow);
	GetDlgItem(IDC_COMBO_RS232_STOPBIT)->ShowWindow(bShow);
	GetDlgItem(IDC_COMBO_RS2232_WORKMODE)->ShowWindow(bShow);
}

void CDlgPosConnectMode::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	memset(&m_struConnectPosCfg, 0, sizeof(m_struConnectPosCfg));
	m_struConnectPosCfg.struVerHead.wLength = sizeof(m_struConnectPosCfg);
	m_struConnectPosCfg.struVerHead.byVersion = 0;
	CString strLen = "";
//	m_comboPosName.SelectString(m_comboPosName.GetCurSel(),strLen);
    m_comboPosName.GetWindowText(strLen);
	memcpy(m_struConnectPosCfg.sPosName,strLen,strLen.GetLength());
	m_struConnectPosCfg.dwConnectMode = m_comboConnectMode.GetCurSel();
	m_struConnectPosCfg.byPosEnable = m_bEnable;

	switch(m_struConnectPosCfg.dwConnectMode)
	{
	case MODE_NET_RECEIVE:
		GetNetSeceiveInfo();
		break;
	case MODE_NET_MONITER:
		GetNetMonitorInfo();
		break;
	case MODE_RS232:
		GetRS232Info();
		break;			
	}
	
	if (NET_DVR_SetDVRConfig(m_iUserID, NET_DVR_SET_CONNECT_POS_CFG, NULL, &m_struConnectPosCfg, sizeof(m_struConnectPosCfg)))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_CONNECT_POS_CFG");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_CONNECT_POS_CFG");
	}
}

void CDlgPosConnectMode::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwReturn = 0;
	memset(&m_struConnectPosCfg, 0, sizeof(m_struConnectPosCfg));
	m_struConnectPosCfg.struVerHead.wLength = sizeof(m_struConnectPosCfg);
	m_struConnectPosCfg.struVerHead.byVersion = 0;

	if (NET_DVR_GetDVRConfig(m_iUserID, NET_DVR_GET_CONNECT_POS_CFG, NULL, &m_struConnectPosCfg, sizeof(m_struConnectPosCfg), &dwReturn))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_CONNECT_POS_CFG");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_CONNECT_POS_CFG");
		return;
	}

	//显示信息
	switch (m_struConnectPosCfg.dwConnectMode)
	{
	case MODE_NET_RECEIVE:
		SetNetSeceiveInfo();
		break;
	case MODE_NET_MONITER:
		SetNetMonitorInfo();
		break;
	case MODE_RS232:
		SetRS232Info();
		break;
	default:
		SetNetMonitorInfo();
		break;
	}

	//获取规则信息
	dwReturn = 0;
	if (NET_DVR_GetDVRConfig(m_iUserID, NET_DVR_GET_POS_FILTER_CFG, NULL, &m_struPosFilterRuleCfg, sizeof(m_struPosFilterRuleCfg), &dwReturn))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_CONNECT_POS_CFG");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_CONNECT_POS_CFG");
		return;
	}

	//先清空对应的列表
	m_comboPosName.ResetContent();

	//
	for (int i = 0 ; i < m_struPosFilterRuleCfg.dwMaxRuleNum ; i++)
	{
		m_comboPosName.AddString((LPCTSTR)m_struPosFilterRuleCfg.struFilterCfg[i].sPosName);
	}

	UpdateData(FALSE);

}

void CDlgPosConnectMode::GetNetSeceiveInfo()
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	if (m_iUserID < 0)
	{
		g_StringLanType(szLan,"请先登录设备!", "Please Login Frist!");
		AfxMessageBox(szLan);
		return;
	}

	if (m_wNetReceivePort <= 0)
	{
		g_StringLanType(szLan, "非法的端口号!", "illeage port!");
		AfxMessageBox(szLan);
		return;
	}
	m_struConnectPosCfg.struPosConnMode.struNetReceiveSet.wNetPort = m_wNetReceivePort;
	UpdateData(FALSE);
}

void CDlgPosConnectMode::GetNetMonitorInfo()
{
	UpdateData(TRUE);
	char szLan[256] = {0};
	char szDeviceIP[128] = {0};

	if (m_csNetMonitorIPAddr.GetLength() <= 0 || m_csNetMonitorIPAddr.GetLength() > 128)
	{
		char szLan[256] = {0};
		g_StringLanType(szLan, "设备地址长度为1~128!", "please input the device address in 1 and 128 characters!");
		AfxMessageBox(szLan);
		return;
	}

	sprintf(szDeviceIP, "%s", m_csNetMonitorIPAddr.GetBuffer(m_csNetMonitorIPAddr.GetLength()));

	if (m_csNetMonitorIPAddr.Find(":") != -1)
	{
		memcpy(m_struConnectPosCfg.struPosConnMode.struNetMonitorSet.struNetIp.byIPv6, m_csNetMonitorIPAddr.GetBuffer(m_csNetMonitorIPAddr.GetLength()), 128);
	}
	else
	{
		memcpy(m_struConnectPosCfg.struPosConnMode.struNetMonitorSet.struNetIp.sIpV4, m_csNetMonitorIPAddr.GetBuffer(m_csNetMonitorIPAddr.GetLength()), 16);
	}	

	UpdateData(FALSE);
}


void CDlgPosConnectMode::GetRS232Info()
{
	UpdateData(TRUE);

    m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.dwBaudRate = m_comboRS232BaudRate.GetCurSel();
	m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byDataBit = m_comboRS232DataBit.GetCurSel();
	m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byStopBit = m_comboRS232StopBit.GetCurSel();
	m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byFlowcontrol = m_comboRS232FlowCtrl.GetCurSel();
	m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byParity = m_comboRS232Parity.GetCurSel();
	m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.dwWorkMode = m_comboRS232WorkMode.GetCurSel();
	UpdateData(FALSE);

}

void CDlgPosConnectMode::SetNetSeceiveInfo()
{
	UpdateData(TRUE);	
   	m_wNetReceivePort = m_struConnectPosCfg.struPosConnMode.struNetReceiveSet.wNetPort;
	UpdateData(FALSE);
}

void CDlgPosConnectMode::SetNetMonitorInfo()
{
// 	UpdateData(TRUE);
// 	string
// 		if (strcmp(m_struConnectPosCfg.struPosConnMode.struNetMonitorSet.struNetIp.byIPv6, ":" )!= -1)
// 		{
// 			memcpy(m_struConnectPosCfg.struPosConnMode.struNetMonitorSet.struNetIp.byIPv6, m_csNetMonitorIPAddr.GetBuffer(m_csNetMonitorIPAddr.GetLength()), 128);
// 		}
// 		else
// 		{
// 			memcpy(m_struConnectPosCfg.struPosConnMode.struNetMonitorSet.struNetIp.sIpV4, m_csNetMonitorIPAddr.GetBuffer(m_csNetMonitorIPAddr.GetLength()), 16);
// 		}
// 		UpdateData(FALSE);
}


void CDlgPosConnectMode::SetRS232Info()
{
	UpdateData(TRUE);	
    m_comboRS232BaudRate.SetCurSel(m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.dwBaudRate);
	m_comboRS232DataBit.SetCurSel(m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byDataBit);
	m_comboRS232StopBit.SetCurSel(m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byStopBit);
	m_comboRS232FlowCtrl.SetCurSel(m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byFlowcontrol);
	m_comboRS232Parity.SetCurSel(m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.byParity);
	m_comboRS232WorkMode.SetCurSel(m_struConnectPosCfg.struPosConnMode.struSingleRS232Set.dwWorkMode);
	UpdateData(FALSE);
}

void CDlgPosConnectMode::OnSelchangeComboConnectMode() 
{
	// TODO: Add your control notification handler code here
	UINT dwSelectIndex = 0;
	dwSelectIndex = m_comboConnectMode.GetCurSel();
// 	switch(dwSelectIndex)
// 	{
// 
// 	}
	ShowConnectInfo(dwSelectIndex);
	
}

void CDlgPosConnectMode::OnKillfocusEditNetReceivePort() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128]= {0};
	if (m_wNetReceivePort <10000)
	{
		g_StringLanType(szLan,"网路监听端口为10000--65535!","Internet listening port between 10000 and 65535!");
		AfxMessageBox(szLan);
		return;
	}
}
