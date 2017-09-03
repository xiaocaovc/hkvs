// DlgSubIPCCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgSubIPCCfg.h"
#include "DlgCcdParam.h"
#include "DlgIPCSpecial.h"
#include "DlgIPCSpeclalEx.h"
#include "DlgUpgradeIPC.h"
#include "DlgImExportIPCCfgFile.h"
#include "DlgRoiCfg.h"
#include "DlgPtzScope.h"
#include "DlgIPCSimpIntellCfg.h"
#include "DlgDevServer.h"
#include "DlgGBT28181ProtocolAccess.h"
#include "DlgRemoteAlarmDetectFace.h"
#include "DlgCameraSetupCfg.h"
#include "DlgFishEye.h"
#include "DlgStreamAttachInfoCfg.h"
#include "DlgPtzCfgCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSubIPCCfg dialog


CDlgSubIPCCfg::CDlgSubIPCCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubIPCCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSubIPCCfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_lDeviceIndex = -1;
	m_lUserID = -1;
	m_lChannel = -1;
	m_lChanCount = -1;
	m_lAnaChanCount = -1;
	m_lDStartChannel = -1;
	m_lAlarmOutNum = -1;
	m_lStartChan = -1;
	m_lAlarmInNum = -1;
}


void CDlgSubIPCCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSubIPCCfg)
	DDX_Control(pDX, IDC_COMBO_ALARM_IN, m_comboAlarmIn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSubIPCCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgSubIPCCfg)
	ON_BN_CLICKED(IDC_BTN_CCDCONFIG, OnBtnCcdconfig)
	ON_BN_CLICKED(IDC_BTN_DETECT_FACE, OnBtnDetectFace)
	ON_BN_CLICKED(IDC_BTN_IPC_SPECIAL, OnBtnIpcSpecial)
	ON_BN_CLICKED(IDC_BTN_IPC_SPECIALEX, OnBtnIpcSpecialex)
	ON_BN_CLICKED(IDC_BTN_UPGRADE_IPC, OnBtnUpgradeIpc)
	ON_BN_CLICKED(IDC_BTN_IM_EXPORT_IPC_CFG_FILE, OnBtnImExportIpcCfgFile)
	ON_BN_CLICKED(IDC_BTN_ROI, OnBtnRoi)
	ON_BN_CLICKED(IDC_BTN_PTZ_SCOPE, OnBtnPtzScope)
	ON_BN_CLICKED(IDC_BTN_IPCINTELL, OnBtnIpcintell)
	ON_BN_CLICKED(IDC_BTN_DEVSERVER, OnBtnDevserver)
	ON_BN_CLICKED(IDC_BTN_GBT28181_PROTOCOL, OnBtnGbt28181Protocol)
	ON_BN_CLICKED(IDC_BTN_CAMERA_SETUPCFG, OnBtnCameraSetupcfg)
	ON_BN_CLICKED(IDC_BTN_FISHEYE, OnBtnFisheye)
	ON_BN_CLICKED(IDC_BTN_STREAM_ATTACHINFO_CFG, OnBtnStreamAttachinfoCfg)
	ON_BN_CLICKED(IDC_BTN_PTZCFGCTRL, OnBtnPtzcfgctrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSubIPCCfg message handlers
void CDlgSubIPCCfg::CurCfgUpdate()
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
	m_lAnaChanCount = g_struDeviceInfo[iDeviceIndex].iAnalogChanNum;
//	m_lDStartChannel = g_struDeviceInfo[iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan;
	m_lAlarmOutNum = g_struDeviceInfo[iDeviceIndex].iAlarmOutNum;
	m_lStartChan = g_struDeviceInfo[iDeviceIndex].iStartChan;
	m_lAlarmInNum = g_struDeviceInfo[iDeviceIndex].iAlarmInNum;

	if (!CheckInitParam())
	{
		return;
	}
}

BOOL CDlgSubIPCCfg::CheckInitParam()
{	
	int iAlarmInSel = 0;
	CString sTemp;
	char szLan[128] = {0};
	DWORD dwReturned = 0;
	m_comboAlarmIn.ResetContent();
	NET_DVR_ALARMINCFG_V40 struAlarmInCfgV40 = {0};

	for (int i=0; i<m_lAlarmInNum; i++)
	{	
		int iIPAlarmIndex = i;
            
        if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMINCFG_V40, iIPAlarmIndex, &struAlarmInCfgV40, sizeof(NET_DVR_ALARMINCFG_V40), &dwReturned))
        {
            g_pMainDlg->AddLog(m_lDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_ALARMINCFG_V40 chanindex[%d]", iIPAlarmIndex);	
            g_StringLanType(szLan, "获取报警输入参数失败", "Alarm input parameters to get the failure");
            AfxMessageBox(szLan);
			return FALSE;
        }
        else
        {
            g_pMainDlg->AddLog(m_lDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_ALARMINCFG_V40 chanindex[%d]", iIPAlarmIndex);	
            
        }
     			
		sTemp.Format(ALARM_IN_NAME, (1+i));

        m_comboAlarmIn.AddString(sTemp);
        m_comboAlarmIn.SetItemData(iAlarmInSel, iIPAlarmIndex);
        iAlarmInSel++;
    }

	m_comboAlarmIn.SetCurSel(0);

	return TRUE;
}

void CDlgSubIPCCfg::OnBtnCcdconfig() 
{
	// TODO: Add your control notification handler code here
	CDlgCcdParam dlg;
	dlg.m_iDeviceIndex = m_lDeviceIndex;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnDetectFace() 
{
	// TODO: Add your control notification handler code here
	CDlgRemoteAlarmDetectFace dlg;
	//	dlg.m_lpAlarmIn = &m_struAlarmInCfg[m_iAlarmInIndex];
	dlg.m_iChanCount = m_lChanCount;
	dlg.m_iAnaChanCount = m_lAnaChanCount;
	dlg.m_iDStartChannel = m_lDStartChannel;
	dlg.m_dwDevIndex = m_lDeviceIndex;
	dlg.m_lStartChannel = m_lStartChan;
	dlg.m_dwAlarmOutNum = m_lAlarmOutNum;
	dlg.m_dwAlarmInNum = m_lAlarmInNum;
    dlg.m_lAlarmInIndex = m_comboAlarmIn.GetItemData(m_comboAlarmIn.GetCurSel());
    dlg.m_lServerID = m_lUserID;
	
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnIpcSpecial() 
{
	// TODO: Add your control notification handler code here

	CDlgIPCSpecial dlg;
	dlg.m_lServerID = m_lUserID;
    dlg.m_iDevIndex = m_lDeviceIndex;
	dlg.m_lChannel = m_lChannel;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnIpcSpecialex() 
{
	// TODO: Add your control notification handler code here
	
	CDlgIPCSpeclalEx dlg;
	dlg.m_lLoginID = m_lUserID;	
	dlg.m_iDeviceIndex = m_lDeviceIndex;			
	dlg.m_lChannel = m_lChannel;
	dlg.m_iChanCount = m_lChanCount;
	dlg.m_iAnaChanCount = m_lAnaChanCount;
	dlg.m_iDStartChannel = m_lDStartChannel;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnUpgradeIpc() 
{
	// TODO: Add your control notification handler code here
	CDlgUpgradeIPC dlg;
    dlg.m_lUserID = m_lUserID;
    dlg.m_dwDevIndex = m_lDeviceIndex;
    dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnImExportIpcCfgFile() 
{
	// TODO: Add your control notification handler code here
	DlgImexporIPCCfgFile dlg;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnRoi() 
{
	// TODO: Add your control notification handler code here
	CDlgRoiCfg dlg;
	dlg.m_lLoginID = m_lUserID;	
	dlg.m_iDeviceIndex = m_lDeviceIndex;			
	dlg.m_lChannel = m_lChannel;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnPtzScope() 
{
	// TODO: Add your control notification handler code here
	CDlgPtzScope dlg;
    dlg.m_iDevIndex = m_lDeviceIndex;
    dlg.m_lServerID = m_lUserID;
    dlg.m_lChannel= m_lChannel;
    dlg.DoModal();	
}

void CDlgSubIPCCfg::OnBtnIpcintell() 
{
	// TODO: Add your control notification handler code here
	CDlgIPCSimpIntellCfg dlg;
	dlg.m_lLoginID = m_lUserID;	
	dlg.m_iDeviceIndex = m_lDeviceIndex;			
	dlg.m_lChannel = m_lChannel;
	dlg.m_iChanCount = m_lChanCount;
	dlg.m_iAnaChanCount = m_lAnaChanCount;
	dlg.m_iDStartChannel = m_lDStartChannel;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnDevserver() 
{
	// TODO: Add your control notification handler code here
	CDlgDevServer dlg;
	dlg.m_lServerID = m_lUserID;
	dlg.m_dwAlarmOutNum = m_lAlarmOutNum;
	dlg.m_dwDevIndex = m_lDeviceIndex;
	dlg.m_nChannel = m_lChannel;
	dlg.m_lStartChan = m_lStartChan;
	dlg.m_dwAlarmInNum = m_lAlarmInNum;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnGbt28181Protocol() 
{
	// TODO: Add your control notification handler code here
	CDlgGBT28181ProtocolAccess dlg;
	dlg.m_lServerID = m_lUserID;
	dlg.m_dwAlarmOutNum = m_lAlarmOutNum;
	dlg.m_dwDevIndex = m_lDeviceIndex;
	dlg.m_nChannel = m_lChannel;
	dlg.m_lStartChan = m_lStartChan;
	dlg.m_dwAlarmInNum = m_lAlarmInNum;
	dlg.DoModal();
}

BOOL CDlgSubIPCCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgSubIPCCfg::OnBtnCameraSetupcfg() 
{
	// TODO: Add your control notification handler code here
	CDlgCameraSetupCfg dlg ;
	dlg.m_lLoginID = m_lUserID;
	dlg.m_lChannel = m_lChannel;
	dlg.m_iDeviceIndex = m_lDeviceIndex ;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnFisheye() 
{
	CDlgFishEye dlg;
	dlg.m_lServerID = m_lUserID;
	dlg.m_lChannel = m_lChannel;
	dlg.m_iDevIndex = m_lDeviceIndex;
	dlg.DoModal();
}

void CDlgSubIPCCfg::OnBtnStreamAttachinfoCfg() 
{
	CDlgStreamAttachInfoCfg dlg;
	dlg.m_lServerID = m_lUserID;
	dlg.m_lChannel = m_lChannel;
	dlg.m_iDeviceIndex = m_lDeviceIndex;
	dlg.DoModal();
	
}

void CDlgSubIPCCfg::OnBtnPtzcfgctrl() 
{
	// TODO: Add your control notification handler code here
	CDlgPtzCfgCtrl dlg;
	dlg.m_lLoginID = m_lUserID;
	dlg.m_lChannel = m_lChannel;
	dlg.m_iDeviceIndex = m_lDeviceIndex;
	dlg.DoModal();
}
