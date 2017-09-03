// DlgPosChanFilter.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPosChanFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPosChanFilter dialog


CDlgPosChanFilter::CDlgPosChanFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPosChanFilter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPosChanFilter)
	m_csPosFilterName = _T("");
	m_iUserID = -1;
	m_iDevIndex = -1;

	//}}AFX_DATA_INIT
}


void CDlgPosChanFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPosChanFilter)
	DDX_Control(pDX, IDC_COMBO_CONNECT_MODE, m_comboConnectMode);
	DDX_Control(pDX, IDC_COMBO_THEFILTERNUM, m_comboTheFilterNum);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_comboChannel);
	DDX_Text(pDX, IDC_EDIT_POS_FILTERNAME, m_csPosFilterName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPosChanFilter, CDialog)
	//{{AFX_MSG_MAP(CDlgPosChanFilter)
	ON_BN_CLICKED(IDC_BTN_REFRESH, OnBtnRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUP, OnSelchangeComboGroup)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, OnSelchangeComboChannel)
	ON_CBN_EDITCHANGE(IDC_COMBO_THEFILTERNUM, OnEditchangeComboThefilternum)
	ON_CBN_SELCHANGE(IDC_COMBO_THEFILTERNUM, OnSelchangeComboThefilternum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPosChanFilter message handlers

BOOL CDlgPosChanFilter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_dwCurselectIndx = 0;
	// TODO: Add extra initialization here
	memset(&m_struChanFilterCfg, 0 ,sizeof(m_struChanFilterCfg));
	memset(&m_struPosFilterCfg, 0 ,sizeof(m_struPosFilterCfg));
	memset(&m_struIPParaCfgV40,0, sizeof(m_struIPParaCfgV40));

	GetFilterCfgInfo(0);
	AddChanInfo();
	m_comboChannel.SetCurSel(0);
	m_comboGroup.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgPosChanFilter::OnBtnRefresh() 
{
	// TODO: Add your control notification handler code here
	GetFilterCfgInfo(0);

}

void CDlgPosChanFilter::OnSelchangeComboGroup() 
{
	// TODO: Add your control notification handler code here
	char szLan[128] = {0};
	int iIndex = m_comboGroup.GetCurSel();
	if (iIndex == LB_ERR)
	{
		g_StringLanType(szLan,"请选择组号","Please select Group Number frist!");
		AfxMessageBox(szLan);
		return ;
	}
	GetFilterCfgInfo(iIndex);

}

void CDlgPosChanFilter::GetFilterCfgInfo(UINT dwIndex)
{
	UpdateData(TRUE);
	DWORD dwReturn = 0;
	char szLan[128] = {0};
	int i = 0;
	//获取过滤规则信息
	memset(&m_struPosFilterCfg, 0, sizeof(m_struPosFilterCfg));
	m_struPosFilterCfg.struVerHead.wLength = sizeof(m_struPosFilterCfg);
	m_struPosFilterCfg.dwMaxGroupNum = dwIndex;

	if (!NET_DVR_GetDVRConfig(m_iUserID,NET_DVR_GET_POS_FILTER_CFG,dwIndex,&m_struPosFilterCfg,sizeof(m_struPosFilterCfg), &dwReturn))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_POS_FILTER_CFG");
		return ;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_POS_FILTER_CFG");
	}
	
	//添加规则信息
	m_comboTheFilterNum.ResetContent();
	m_csPosFilterName = "";
	if (m_struPosFilterCfg.dwMaxRuleNum != 0)
	{
		for (i = 0; i < m_struPosFilterCfg.dwMaxRuleNum;i++)
		{
			sprintf(szLan,"%d",i+1);
			m_comboTheFilterNum.AddString(szLan);
		}
	}
	
	UpdateData(FALSE);
}

void CDlgPosChanFilter::ShowPosFilterInfo( UINT dwTheFilterNum)  
{
	m_csPosFilterName.Format("%s",m_struPosFilterCfg.struFilterCfg[dwTheFilterNum].sPosName);
	m_comboConnectMode.SetCurSel(m_struPosFilterCfg.struFilterCfg[dwTheFilterNum].dwProtocolType - 1);
}

void CDlgPosChanFilter::AddChanInfo()
{
	UpdateData(TRUE);
	DWORD dwReturn = 0;
	CString strTemp =_T("");
	DWORD dwChanNum = 0;
	int iIndex = 0;
	DWORD dwIPChanIndex = 0;
	int i = 0;

	int iMaxSimulateChannelNO =  g_struDeviceInfo[m_iDevIndex].iAnalogChanNum;
	//***模拟通道***//
	for (i = 0; i < g_struDeviceInfo[m_iDevIndex].iAnalogChanNum; i++)
	{
		dwChanNum = i +1;
		if ((dwChanNum) > iMaxSimulateChannelNO)
		{
			break;
		}
		
		strTemp.Format(ANALOG_C_FORMAT, dwChanNum);
		m_comboChannel.AddString(strTemp);
	}
	
	//***IP channel***//
	int iMaxIPChannelNO =  g_struDeviceInfo[m_iDevIndex].iIPChanNum;
	for (i = 0; i < iMaxIPChannelNO; i++)
	{
		dwChanNum = i + m_struIPParaCfgV40.dwStartDChan + 1;
		if ((dwChanNum ) > iMaxIPChannelNO)
		{
			break;
		}
		
		strTemp.Format(IP_CAMERA_NAME, dwChanNum);
		m_comboChannel.AddString(strTemp);		
	}	
	
	UpdateData(FALSE);
}

void CDlgPosChanFilter::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	char szLan[2] = {0};
	m_struChanFilterCfg.struVerHead.wLength = sizeof(m_struChanFilterCfg);
	m_struChanFilterCfg.struVerHead.byVersion = 0;
	m_struChanFilterCfg.dwFilterID = m_comboTheFilterNum.GetCurSel() + m_comboGroup.GetCurSel() * FILTERRULE_NUM +1;

	DWORD dwChannelNum = 0;
	dwChannelNum = m_comboChannel.GetCurSel() +1;

	if (NET_DVR_SetDVRConfig(m_iUserID, NET_DVR_SET_CHAN_FILTER_CFG, dwChannelNum, &m_struChanFilterCfg, sizeof(m_struChanFilterCfg)))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_CHAN_FILTER_CFG");
		
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_CHAN_FILTER_CFG");
		return;
	}
}

void CDlgPosChanFilter::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
	DWORD dwGroupNum = 0;
	char szLan[2] = {0};
	DWORD dwReturn = 0;
	
	
// 	m_comboGroup.GetWindowText(szLan, m_comboChannel.GetCurSel());
// 	dwGroupNum = atoi(szLan);

	memset(&m_struChanFilterCfg, 0, sizeof(m_struChanFilterCfg));
	m_struChanFilterCfg.struVerHead.wLength = sizeof(m_struChanFilterCfg);
	m_struChanFilterCfg.struVerHead.byVersion = 0;
	 
	UINT32 dwChannelNum = 0 ;
	dwChannelNum = m_comboChannel.GetCurSel() + 1;
	if (NET_DVR_GetDVRConfig(m_iUserID, NET_DVR_GET_CHAN_FILTER_CFG,dwChannelNum , &m_struChanFilterCfg, sizeof(m_struChanFilterCfg),&dwReturn))
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_CHAN_FILTER_CFG");	
	}
	else
	{
		g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_CHAN_FILTER_CFG");
		return;
	}

	//首先计算组号
    if (m_struChanFilterCfg.dwFilterID >0)
    {
		dwGroupNum = (m_struChanFilterCfg.dwFilterID -1)/FILTERRULE_NUM; 
    }
	else   //表示目前未设置通道与规则的关联信息
	{
		dwGroupNum = 0;   
	}
	

	//Get Filter Cfg Info
 	GetFilterCfgInfo(dwGroupNum);
	
	//set show Info
	if (m_struChanFilterCfg.dwFilterID >0)
    {
		m_comboTheFilterNum.SetCurSel(m_struChanFilterCfg.dwFilterID - 1 - dwGroupNum * FILTERRULE_NUM);
		ShowPosFilterInfo(m_struChanFilterCfg.dwFilterID - 1 - dwGroupNum * FILTERRULE_NUM);
	}
	UpdateData(FALSE);	
}


void CDlgPosChanFilter::OnSelchangeComboChannel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(FALSE);
	int iSelectChan = -1;
	char szLan[128] = {0};
	iSelectChan = m_comboChannel.GetCurSel();
	sprintf(szLan, "%s", m_struPosFilterCfg.struFilterCfg[iSelectChan].sPosName);
	UpdateData(FALSE);
}

void CDlgPosChanFilter::OnEditchangeComboThefilternum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwTheFilterNum = m_comboTheFilterNum.GetCurSel();
	ShowPosFilterInfo(dwTheFilterNum);
	UpdateData(FALSE);

	
}

void CDlgPosChanFilter::OnSelchangeComboThefilternum() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//Get FilterNum
	DWORD dwSelectFilter = m_comboTheFilterNum.GetCurSel();
	ShowPosFilterInfo(dwSelectFilter);
	
	UpdateData(FALSE);
}
