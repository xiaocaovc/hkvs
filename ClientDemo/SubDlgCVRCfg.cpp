// SubDlgCVRCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "SubDlgCVRCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgCVRCfg dialog


CSubDlgCVRCfg::CSubDlgCVRCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgCVRCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgCVRCfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSubDlgCVRCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgCVRCfg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgCVRCfg, CDialog)
	//{{AFX_MSG_MAP(CSubDlgCVRCfg)
	ON_BN_CLICKED(IDC_BTN_DEV_CFG, OnBtnDevCfg)
	ON_BN_CLICKED(IDC_BTM_STREAMID, OnBtmStreamid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgCVRCfg message handlers

BOOL CSubDlgCVRCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    
//    char szLan[100] = "\0";
//     
//    g_StringLanType(szLan, "设备配置", "CVR Cfg");
//    m_tabStreamID.InsertItem(0, szLan);
//    m_dlgCVRCfg.m_lUserID = m_lUserID;
//    m_dlgCVRCfg.m_iDevIndex = m_iDevIndex;
//    m_dlgCVRCfg.Create(IDD_DLG_CVR, &m_tabStreamID);
//    m_dlgCVRCfg.ShowWindow(SW_HIDE);
// 
//    	g_StringLanType(szLan, "流来源", "Stream source");
//     m_tabStreamID.InsertItem(1, szLan);
//     m_dlgStreamSrc.Create(IDD_DLG_STREAM_SRC_CFG, &m_tabStreamID);
//     m_dlgStreamSrc.ShowWindow(SW_HIDE);
//     
//     g_StringLanType(szLan, "流录像状态", "Stream record status");
//     m_tabStreamID.InsertItem(2, szLan);
//     m_dlgStreamRecordStatus.Create(IDD_DLG_STREAM_RECORD_STATUS, &m_tabStreamID);
//     m_dlgStreamRecordStatus.ShowWindow(SW_HIDE);
//     
//     g_StringLanType(szLan, "流录像计划", "Stream record information");
//     m_tabStreamID.InsertItem(3, szLan);
//     m_dlgStreamRecordInfo.Create(IDD_DLG_STREAM_RECORD_INFO, &m_tabStreamID);
//     m_dlgStreamRecordInfo.ShowWindow(SW_HIDE);
//     
//     g_StringLanType(szLan, "流手动录像和录像段加锁", "Stream manual record and lock");
//     m_tabStreamID.InsertItem(4, szLan);
//     m_dlgStreamManualRecordAndLock.Create(IDD_DLG_STREAM_MANUAL_RECORD_AND_LOCK, &m_tabStreamID);
// 	m_dlgStreamManualRecordAndLock.ShowWindow(SW_HIDE);
// 
//    
//     
//     CRect rcClient;
//     GetClientRect(&rcClient);
//     m_tabStreamID.MoveWindow(rcClient);
//     
//     CRect rcChildRgn = rcClient;
//     rcChildRgn.DeflateRect(100,300,0,0);
//     
//     m_dlgCVRCfg.MoveWindow(rcChildRgn);
//     m_dlgStreamSrc.MoveWindow(rcChildRgn);    
//     m_dlgStreamRecordStatus.MoveWindow(rcChildRgn);    
//     m_dlgStreamRecordInfo.MoveWindow(rcChildRgn);    
//     m_dlgStreamManualRecordAndLock.MoveWindow(rcChildRgn); 
//     
//     ShowChildWnd(SUB_WND_CVR_CFG);

    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSubDlgCVRCfg::OnBtnDevCfg() 
{
    // TODO: Add your control notification handler code here
    CDlgCVR dlg;
    dlg.m_lUserID = m_lUserID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CSubDlgCVRCfg::OnBtmStreamid() 
{
	// TODO: Add your control notification handler code here
	CDlgStreamID dlg;
    dlg.DoModal();
}

void CSubDlgCVRCfg::CurCfgUpdate()
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
    m_iDevIndex = iDeviceIndex;
    m_lUserID = g_struDeviceInfo[iDeviceIndex].lLoginID;
    m_lChanNo= iChanIndex+g_struDeviceInfo[iDeviceIndex].iStartChan;
}