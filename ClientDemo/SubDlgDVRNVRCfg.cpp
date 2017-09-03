// SubDlgDVRNVRCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "SubDlgDVRNVRCfg.h"
#include "DlgNetServiceCfg.h"
#include "DlgCertInfo.h"
#include "DlgNetraEventVedio.h"
#include "DlgVedioAudioInCfg.h"
#include "DlgVCADetionCfg.h"
#include "DlgIPCSimpIntellCfg.h"
#include "DlgImExportIPCCfgFile.h"
#include "DlgVQDCfg.h"
#include "DlgNVRCfg.h"
#include "DlgGetDigitalChannelStatus.h"
#include "DlgUpgradeIPC.h"
#include "DlgAtmFrameFormatV30.h"
#include "DlgInquestDvr.h"
#include "DlgVehicleCfg.h"
#include "DlgDevWorkMode.h"
#include "DlgGopInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgDVRNVRCfg dialog


CSubDlgDVRNVRCfg::CSubDlgDVRNVRCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgDVRNVRCfg::IDD, pParent)
    ,m_lUserID(-1)
    ,m_lChanNo(-1)
    ,m_iDevIndex(-1)
{
	//{{AFX_DATA_INIT(CSubDlgDVRNVRCfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSubDlgDVRNVRCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgDVRNVRCfg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgDVRNVRCfg, CDialog)
	//{{AFX_MSG_MAP(CSubDlgDVRNVRCfg)
	ON_BN_CLICKED(IDC_BTN_NET_SERVICE, OnBtnNetService)
	ON_BN_CLICKED(IDC_BTN_CERT, OnBtnCert)
	ON_BN_CLICKED(IDC_BTN_EVENT_RECORD, OnBtnEventRecord)
	ON_BN_CLICKED(IDC_BTN_AUDIOINPUT, OnBtnAudioinput)
	ON_BN_CLICKED(IDC_BTN_VCA_DETECTION, OnBtnVcaDetection)
	ON_BN_CLICKED(IDC_BTM_SMD, OnBtmSmd)
	ON_BN_CLICKED(IDC_BTN_IM_EXPORT_IPC_CFG_FILE, OnBtnImExportIpcCfgFile)
	ON_BN_CLICKED(IDC_BTN_VQD_CFG, OnBtnVqdCfg)
	ON_BN_CLICKED(IDC_BTN_NVRCFG, OnBtnNvrcfg)
	ON_BN_CLICKED(IDC_BTN_GET_IPCSTATIUS, OnBtnGetIPCStatus)
	ON_BN_CLICKED(IDC_BTN_UPGRADE_IPC, OnBtnUpgradeIpc)
	ON_BN_CLICKED(IDC_BTN_ATM, OnBtnAtm)
	ON_BN_CLICKED(IDC_BTN_INQUEST, OnBtnInquest)
    ON_BN_CLICKED(IDC_BTN_VEHICLECFG, OnBtnVehicleCfg)
	ON_BN_CLICKED(IDC_BTN_WORKMODE, OnBtnWorkmode)
    ON_BN_CLICKED(IDC_BTN_GOP_INFO, OnBtnGetGopInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgDVRNVRCfg message handlers

BOOL CSubDlgDVRNVRCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSubDlgDVRNVRCfg::OnBtnNetService() 
{
	// TODO: Add your control notification handler code here
    CDlgNetServiceCfg dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_lUserID = m_lUserID;
    dlg.m_iDevIndex = m_iDevIndex;
	dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnCert() 
{
	// TODO: Add your control notification handler code here
	CDlgCertInfo dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_lUserID = m_lUserID;
    dlg.m_iDevIndex = m_iDevIndex;
	dlg.DoModal();

}

void CSubDlgDVRNVRCfg::OnBtnEventRecord()
{
    CDlgNetraEventVedio dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_lLoginID = m_lUserID;
    dlg.m_lChannel = m_lChanNo;
    dlg.m_iDeviceIndex = m_iDevIndex;
    dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnAudioinput() 
{
	// TODO: Add your control notification handler code here
   	DlgVedioAudioInCfg dlg; 
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_VedioChannel  = m_lChanNo;
    dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnVcaDetection() 
{
	// TODO: Add your control notification handler code here
	DlgVCADetionCfg dlg;
    dlg.m_ChannelNo  = m_lChanNo;
    dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtmSmd() 
{
	// TODO: Add your control notification handler code here
    CDlgIPCSimpIntellCfg dlg;
    dlg.m_lLoginID = m_lUserID;	
    dlg.m_iDeviceIndex = m_iDevIndex;			
    dlg.m_lChannel = m_lChanNo;
    dlg.m_iChanCount = g_struDeviceInfo[m_iDevIndex].iDeviceChanNum;
    dlg.m_iAnaChanCount = g_struDeviceInfo[m_iDevIndex].iAnalogChanNum;
    dlg.m_iDStartChannel = g_struDeviceInfo[m_iDevIndex].pStruIPParaCfgV40[0].dwStartDChan;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
	dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnImExportIpcCfgFile() 
{
	// TODO: Add your control notification handler code here
    DlgImexporIPCCfgFile dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
	dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnVqdCfg() 
{
	// TODO: Add your control notification handler code here
    CDlgVQDCfg dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_lUserID = m_lUserID;	
    dlg.m_iDevIndex = m_iDevIndex;			
    //	dlg.m_lChannel = iChanIndex + g_struDeviceInfo[iDeviceIndex].iStartChan;
	dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnNvrcfg() 
{
	// TODO: Add your control notification handler code here
     
    CDlgNVRCfg dlg;

    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }

    dlg.m_lUserID = g_struDeviceInfo[m_iDevIndex].lLoginID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_lChannel = m_iDevIndex + g_struDeviceInfo[m_iDevIndex].iStartChan;
	dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnGetIPCStatus() 
{
	// TODO: Add your control notification handler code here
    // TODO: Add your command handler code here
   
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    
    CDlgGetDigitalChannelStatus dlg;
    dlg.m_lUserID = g_struDeviceInfo[m_iDevIndex].lLoginID;
    dlg.m_dwDevIndex = m_iDevIndex;
    
	dlg.DoModal();
}
void CSubDlgDVRNVRCfg::CurCfgUpdate()
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
    m_lChanNo= m_lChanNo;//iChanIndex+g_struDeviceInfo[iDeviceIndex].iStartChan;
   // m_lChanCount = g_struDeviceInfo[iDeviceIndex].iDeviceChanNum;
}
void CSubDlgDVRNVRCfg::OnBtnUpgradeIpc() 
{
	// TODO: Add your control notification handler code here
    CDlgUpgradeIPC dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_lUserID = m_lUserID;
    dlg.m_dwDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnAtm() 
{
	// TODO: Add your control notification handler code here
    
    CDlgAtmFrameFormatV30 dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_lServerID = m_lUserID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.m_iChannelNum = g_struDeviceInfo[m_iDevIndex].iDeviceChanNum;
    dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnInquest() 
{
	// TODO: Add your control notification handler code here
    CDlgInquestDvr dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_dwDeviceIndex = m_iDevIndex;
    dlg.m_lLoginID = m_lUserID;
	dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnVehicleCfg()
{
       if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    if (m_lChanNo < 0)
    {
        m_lChanNo = 0;//default to config channel one
    }
    
    CDlgVehicleCfg dlg;
    dlg.m_lServerID = m_lUserID;
    dlg.m_dwAlarmOutNum = g_struDeviceInfo[m_iDevIndex].iAlarmOutNum;
    dlg.m_dwDevIndex = m_iDevIndex;
    dlg.m_nChannel = g_struDeviceInfo[m_iDevIndex].iDeviceChanNum;
    dlg.m_lStartChan = g_struDeviceInfo[m_iDevIndex].iStartChan;
    dlg.m_iChannelIndex = m_iDevIndex + g_struDeviceInfo[m_iDevIndex].iStartChan;
 	dlg.DoModal();
}
void CSubDlgDVRNVRCfg::OnBtnWorkmode() 
{
	// TODO: Add your control notification handler code here
    CDlgDevWorkMode dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }
    dlg.m_lUserID = m_lUserID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}

void CSubDlgDVRNVRCfg::OnBtnGetGopInfo()
{
    CDlgGopInfo dlg;
    if (m_iDevIndex < 0)
    {
        AfxMessageBox("Login First!");
        return;
    }

    dlg.m_lUserID = m_lUserID;
    dlg.m_iDevIndex = m_iDevIndex;
    dlg.DoModal();
}