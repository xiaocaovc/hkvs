// DlgSubITSCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgSubITSCfg.h"
#include "DlgTsSeries.h"
#include "DlgITSIOinCfg.h"
#include "DlgITSConfig.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubITSCfg dialog


CSubDlgITSCfg::CSubDlgITSCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgITSCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubITSCfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lDeviceIndex = -1;
	m_lUserID = -1;
	m_lChannel = -1;
	m_lChanCount = -1;
}


void CSubDlgITSCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubITSCfg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgITSCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgSubITSCfg)
	ON_BN_CLICKED(IDC_BTN_TS, OnBtnTs)
	ON_BN_CLICKED(IDC_BTN_ITS_IOIN_CFG, OnBtnItsIoinCfg)
	ON_BN_CLICKED(IDC_BTN_ITS_CONFIG, OnBtnItsConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubITSCfg message handlers

void CSubDlgITSCfg::CurCfgUpdate()
{
	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    if (iDeviceIndex < 0)
    {
		EnableWindow(FALSE);
        return;
    }
	
	EnableWindow(TRUE);
    int iChanIndex = g_pMainDlg->GetCurChanIndex();
    if (iChanIndex < 0)
    {
        iChanIndex = 0;//default to config channel one
    }
    m_lDeviceIndex = iDeviceIndex;
    m_lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    m_lChannel= iChanIndex+g_struDeviceInfo[iDeviceIndex].iStartChan;
	m_lChanCount = g_struDeviceInfo[iDeviceIndex].iDeviceChanNum;
}

void CSubDlgITSCfg::OnBtnTs() 
{
	// TODO: Add your control notification handler code here
	CDlgTsSeries dlg;
	dlg.m_lLoginID = m_lUserID;	
	dlg.m_iDeviceIndex = m_lDeviceIndex;			
	dlg.m_lChannel = m_lChannel;
	dlg.DoModal();
}

void CSubDlgITSCfg::OnBtnItsIoinCfg() 
{
	// TODO: Add your control notification handler code here
	CDlgITSIOinCfg dlg; 
    dlg.m_lUserID = m_lUserID;
    dlg.m_dwChannel = m_lChannel;
    dlg.m_iDevIndex = m_lDeviceIndex;
	dlg.DoModal();
}

void CSubDlgITSCfg::OnBtnItsConfig() 
{
	// TODO: Add your control notification handler code here
	CDlgITSConfig dlg;
	dlg.m_lServerID = m_lUserID;			
	dlg.m_iDevIndex = m_lDeviceIndex;			
	dlg.m_lChannel = m_lChannel;
	dlg.DoModal();	
}
