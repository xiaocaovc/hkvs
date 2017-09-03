// DlgEZVIZAccessCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgEZVIZAccessCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEZVIZAccessCfg dialog


CDlgEZVIZAccessCfg::CDlgEZVIZAccessCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEZVIZAccessCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEZVIZAccessCfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEZVIZAccessCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEZVIZAccessCfg)
	DDX_Control(pDX, IDC_COMBO_ENABLE, m_cbEnable);
	DDX_Control(pDX, IDC_COMBO_DEV_STATUS, m_cbDeviceStatus);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEZVIZAccessCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgEZVIZAccessCfg)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEZVIZAccessCfg message handlers

void CDlgEZVIZAccessCfg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	char szLan[64]={0};
	g_StringLanType(szLan,"通道非法","channel invalid");
	if (m_iCurChanNO<0 || m_iCurChanNO>1024)
	{
		g_StringLanType(szLan,"通道非法","channel invalid");
		MessageBox(szLan);
		return;
	}
	
	DWORD dwRet=0;
	NET_DVR_EZVIZ_ACCESS_CFG struEZVIZCfg={0};
	struEZVIZCfg.dwSize = sizeof(struEZVIZCfg);
	CString strErr;
	if (!NET_DVR_GetDVRConfig(m_lUserID,NET_DVR_GET_EZVIZ_ACCESS_CFG,m_iCurChanNO,&struEZVIZCfg,sizeof(struEZVIZCfg),&dwRet))
	{
		strErr.Format("NET_DVR_GET_EZVIZ_ACCESS_CFG chan[%d] failed! error code[%d]", m_iCurChanNO, NET_DVR_GetLastError());
		MessageBox(strErr);
		return;
	}
	
	MessageBox("NET_DVR_GET_EZVIZ_ACCESS_CFG suss!");	
	m_cbEnable.SetCurSel(struEZVIZCfg.byEnable);
	if (struEZVIZCfg.byDeviceStatus > 0) //2014-03-21
	{
		m_cbDeviceStatus.SetCurSel(struEZVIZCfg.byDeviceStatus - 1);
	}
	UpdateData(FALSE);

}

BOOL CDlgEZVIZAccessCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	int iCurChanIndex = g_pMainDlg->GetCurChanIndex();
	m_iCurChanNO = g_struDeviceInfo[iDeviceIndex].pStruChanInfo[iCurChanIndex].iChannelNO; 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEZVIZAccessCfg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here

	char szLan[64]={0};
	if (m_iCurChanNO<0 || m_iCurChanNO>1024)
	{
		g_StringLanType(szLan,"通道非法","channel invalid");
		MessageBox(szLan);
		return;
	}

	UpdateData(TRUE);

	DWORD dwRet=0;
	NET_DVR_EZVIZ_ACCESS_CFG struEZVIZCfg={0};
	struEZVIZCfg.dwSize = sizeof(struEZVIZCfg);
	struEZVIZCfg.byEnable = m_cbEnable.GetCurSel();
	struEZVIZCfg.byDeviceStatus = m_cbDeviceStatus.GetCurSel()+1;//2014-03-21

	CString strErr;

	if (!NET_DVR_SetDVRConfig(m_lUserID,NET_DVR_SET_EZVIZ_ACCESS_CFG,m_iCurChanNO,&struEZVIZCfg,sizeof(struEZVIZCfg)))
	{
		strErr.Format("NET_DVR_SET_EZVIZ_ACCESS_CFG chan[%d] failed! error code[%d]", m_iCurChanNO, NET_DVR_GetLastError());
		MessageBox(strErr);
		return;
	}
	
	MessageBox("NET_DVR_SET_EZVIZ_ACCESS_CFG suss!");	
}
