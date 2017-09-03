// DlgDoTest.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgDoTest.h"
#include "DlgRecordStreamTransType.h"
#include "DlgAbilityAutoDetect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDoTest dialog

CDlgDoTest::CDlgDoTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDoTest::IDD, pParent)
{
	memset(&m_struPreviewCfg, 0, sizeof(NET_DVR_PREVIEWCFG));//_V30
	//{{AFX_DATA_INIT(CDlgDoTest)
	m_iPara1 = 0;
	m_iPara2 = 0;
	m_csPara3 = _T("");
	m_csPara4 = _T("");
	m_iVerHeadParam1 = 0;
	m_iVerHeadParam2 = 0;
	m_iVerHeadParam1_1 = 0;
	m_iVerHeadParam1_2 = 0;
	m_iVerHeadParam2_1 = 0;
	m_iVerHeadParam2_2 = 0;
	m_iVersion = 0;
	//}}AFX_DATA_INIT
    m_lDebugHandle = -1;
    m_bDebugVca = FALSE;
}


void CDlgDoTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDoTest)
	DDX_Control(pDX, IDC_COMBO_HEAD_VER, m_ComboHeadVer);
	DDX_Control(pDX, IDC_COMBO_HEAD_MODEL, m_ComboHeadModel);
	DDX_Control(pDX, IDC_COMBO_PARA4, m_combo4);
	DDX_Control(pDX, IDC_COMBO_PARA3, m_combo3);
	DDX_Control(pDX, IDC_COMBO_PARA2, m_combo2);
	DDX_Control(pDX, IDC_COMBO_PARA1, m_combo1);
	DDX_Text(pDX, IDC_EDIT_PARA1, m_iPara1);
	DDX_Text(pDX, IDC_EDIT_PARA2, m_iPara2);
	DDX_Text(pDX, IDC_EDIT_PARA3, m_csPara3);
	DDX_Text(pDX, IDC_EDIT_PARA4, m_csPara4);
	DDX_Text(pDX, IDC_EDIT_VER_HEARD_PARAM1, m_iVerHeadParam1);
	DDX_Text(pDX, IDC_EDIT_VER_HEARD_PARAM2, m_iVerHeadParam2);
	DDX_Text(pDX, IDC_EDIT_VER_HEARD_PARAM1_1, m_iVerHeadParam1_1);
	DDX_Text(pDX, IDC_EDIT_VER_HEARD_PARAM1_2, m_iVerHeadParam1_2);
	DDX_Text(pDX, IDC_EDIT_VER_HEARD_PARAM2_1, m_iVerHeadParam2_1);
	DDX_Text(pDX, IDC_EDIT_VER_HEARD_PARAM2_2, m_iVerHeadParam2_2);
	DDX_Text(pDX, IDC_EDIT_CFG_GLOBAL_VER, m_iVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDoTest, CDialog)
	//{{AFX_MSG_MAP(CDlgDoTest)
	ON_BN_CLICKED(IDC_BTN1, OnBtn1)
	ON_BN_CLICKED(IDC_BTN2, OnBtn2)
	ON_BN_CLICKED(IDC_BTN3, OnBtn3)
	ON_BN_CLICKED(IDC_BTN4, OnBtn4)
	ON_BN_CLICKED(IDC_BTN6, OnBtn6)
	ON_BN_CLICKED(IDC_BTN_ABILITY, OnBtnAbility)
	ON_BN_CLICKED(IDC_BTN_VCA_DEBUG, OnBtnVcaDebug)
	ON_BN_CLICKED(IDC_BTN_ABILITYPARSE, OnBtnAbilityparse)
	ON_BN_CLICKED(IDC_BTN_SET_HEAD_VERSION, OnBtnSetHeadVersion)
	ON_BN_CLICKED(IDC_BTN_VER_HEAD_GET, OnBtnVerHeadGet)
	ON_BN_CLICKED(IDC_BTN_VER_HEAD_SET, OnBtnVerHeadSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDoTest message handlers
BOOL CDlgDoTest::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (g_bInSide)
	{
		GetDlgItem(IDC_BTN4)->SetWindowText("DecodeInRealPlay");
		GetDlgItem(IDC_BTN6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN4)->SetWindowText("DecodeInCallBack");
		GetDlgItem(IDC_BTN6)->EnableWindow(TRUE);
	}
	if (g_bStandardCB)
	{
		GetDlgItem(IDC_BTN6)->SetWindowText("StandardCallBack");
	}
	else
	{
		GetDlgItem(IDC_BTN6)->SetWindowText("ConvertCallBack");
	}
	m_combo1.SetCurSel(1);

    m_ComboHeadModel.SetCurSel(0);
    m_ComboHeadVer.SetCurSel(0);
    m_iVersion = 100;
	return TRUE;
}

void CDlgDoTest::OnBtn1() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int i=0;
	BOOL bRet = FALSE;
	char szInfo[1024] = {0};
	NET_DVR_ALARMOUTSTATUS_V30 struAlarmOutState;
	memset(&struAlarmOutState, 0, sizeof(NET_DVR_ALARMOUTSTATUS_V30));
	bRet = NET_DVR_GetAlarmOut_V30(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NULL);

	bRet = NET_DVR_GetAlarmOut_V30(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &struAlarmOutState);
	if (!bRet)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetAlarmOut_V30");
		return;
	}
	for (i=0; i<MAX_ALARMOUT_V30; i++)
	{
		if (struAlarmOutState.Output[i])
		{
			sprintf(szInfo, "%s %d[%d]", szInfo, i, struAlarmOutState.Output[i]);
		}
	}	
	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetAlarmOut_V30- %s", szInfo);
	g_dlgOutput[g_pMainDlg->m_iCurWndIndex].Restart();
}
WORD DecToHex(WORD wDec)
{
	return (wDec / 1000) * 4096 + ((wDec % 1000) / 100) * 256 + ((wDec % 100) / 10) * 16 + (wDec % 10);
}

WORD HexToDec(WORD wHex)
{
	return (wHex / 4096) * 1000 + ((wHex % 4096) / 256) * 100 + ((wHex % 256) / 16) * 10 + (wHex % 16);
}

	
void CDlgDoTest::OnBtn2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
//	int i=0;
	int j=0;
	BOOL bRet = FALSE;
	char szInfo[1024] = {0};
	DWORD dwReturned;
//	char szTmp[64] = {0};
	bRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
	if (!bRet)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_PTZPOS");
		return;
	}
	sprintf(szInfo, "%s a[%x]Pan[%x]t[%x]z[%x]",\
		szInfo, m_ptzPos.wAction, m_ptzPos.wPanPos, m_ptzPos.wTiltPos, m_ptzPos.wZoomPos);
	m_iPara1 = HexToDec(m_ptzPos.wPanPos);
	m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
	m_csPara3.Format("%x", m_ptzPos.wZoomPos);	
	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_PTZPOS %s", szInfo);
	UpdateData(FALSE);
// 	bRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_PREVIEWCFG, 0, &m_struPreviewCfg, sizeof(NET_DVR_PREVIEWCFG), &dwReturned);
// 	if (!bRet)
// 	{
// 		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_PREVIEWCFG_V30");
// 		return;
// 	}
// 	sprintf(szInfo, "%s EnableAudio[%d]PreviewNumber[%d]switchtime[%d]",\
// 		szInfo, m_struPreviewCfg.byEnableAudio, m_struPreviewCfg.byPreviewNumber, m_struPreviewCfg.wSwitchTime);
// // 	for (i=0; i< MAX_PREVIEW_MODE; i++)
// // 	{
// 		for (j=0;j<MAX_WINDOW;j++)
// 		{
// 			sprintf(szInfo, "%s [%d]", szInfo, m_struPreviewCfg.bySwitchSeq[j]);//[i]
// 		}
// //	}	
// 	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_PREVIEWCFG %s", szInfo);

	//
// 	bRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_AUXOUTCFG, 0, &m_struAuxOutCfg, sizeof(NET_DVR_AUXOUTCFG), &dwReturned);
// 	if (!bRet)
// 	{
// 		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_AUXOUTCFG");
// 		return;
// 	}
// 	sprintf(szInfo, "AuxOrder[%d]AuxSwitchTime[%d]dwAlarmOutChan[%d]AlarmChanSwitchTime[%d]switchtime[%d]", m_struAuxOutCfg.byAuxOrder[0][0], \
// 		m_struAuxOutCfg.dwAlarmOutChan, m_struAuxOutCfg.dwAuxSwitchTime[0], m_struAuxOutCfg.dwAlarmChanSwitchTime);	
// 	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_AUXOUTCFG %s", szInfo);
}

void CDlgDoTest::OnBtn3() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	BOOL bRet = FALSE;
	char szInfo[1024] = {0};
	m_ptzPos.wAction = m_combo1.GetCurSel();
	m_ptzPos.wPanPos = DecToHex(m_iPara1);
	m_ptzPos.wTiltPos = DecToHex(m_iPara2);
	m_ptzPos.wZoomPos = DecToHex(atoi(m_csPara3));
	bRet = NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS));
	if (!bRet)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_PTZPOS");
		return;
	}
	sprintf(szInfo, "%s action[%x] Pan[%x]t[%x]z[%x]",\
		szInfo, m_ptzPos.wAction, m_ptzPos.wPanPos, m_ptzPos.wTiltPos, m_ptzPos.wZoomPos);	
	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_PTZPOS %s", szInfo);
// 	m_struPreviewCfg.byPreviewNumber = (BYTE)m_iPara1;
// 
// 	bRet = NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_PREVIEWCFG, 0, &m_struPreviewCfg, sizeof(NET_DVR_PREVIEWCFG));
// 	if (!bRet)
// 	{
// 		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_PREVIEWCFG");
// 		return;
// 	}
// 	sprintf(szInfo, "%s EnableAudio[%d]PreviewNumber[%d]switchtime[%d]",\
// 		szInfo, m_struPreviewCfg.byEnableAudio, m_struPreviewCfg.byPreviewNumber, m_struPreviewCfg.wSwitchTime);	
// 	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_PREVIEWCFG %s", szInfo);
// 	
// 
// 	m_struAuxOutCfg.byAuxOrder[0][0] = (BYTE)m_iPara2;
// 	m_struAuxOutCfg.dwAlarmOutChan = m_combo1.GetCurSel();
// 	m_struAuxOutCfg.dwAuxSwitchTime[0] = m_combo2.GetCurSel();
// 	m_struAuxOutCfg.dwAlarmChanSwitchTime = m_combo3.GetCurSel();
// 	bRet = NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_AUXOUTCFG, 0, &m_struAuxOutCfg, sizeof(NET_DVR_AUXOUTCFG));
// 	if (!bRet)
// 	{
// 		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_AUXOUTCFG_V30");
// 		return;
// 	}
// 	sprintf(szInfo, "AuxOrder[%d]AuxSwitchTime[%d]dwAlarmOutChan[%d]AlarmChanSwitchTime[%d]switchtime[%d]", m_struAuxOutCfg.byAuxOrder[0][0], \
// 		m_struAuxOutCfg.dwAlarmOutChan, m_struAuxOutCfg.dwAuxSwitchTime[0], m_struAuxOutCfg.dwAlarmChanSwitchTime);	
// 
// 	g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_AUXOUTCFG %s", szInfo);
}

void CDlgDoTest::OnBtn4() 
{
	// TODO: Add your control notification handler code here
	g_bInSide = !g_bInSide;
	if (g_bInSide)
	{
		GetDlgItem(IDC_BTN4)->SetWindowText("DecodeInRealPlay");
		GetDlgItem(IDC_BTN6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN4)->SetWindowText("DecodeInCallBack");
		GetDlgItem(IDC_BTN6)->EnableWindow(TRUE);
	}
	WriteLocalConfig();
}

void CDlgDoTest::OnBtn6() 
{
	// TODO: Add your control notification handler code here
	g_bStandardCB = !g_bStandardCB;
	if (g_bStandardCB)
	{
		GetDlgItem(IDC_BTN6)->SetWindowText("StandardCallBack");
	}
	else
	{
		GetDlgItem(IDC_BTN6)->SetWindowText("ConvertCallBack");
	}
	WriteLocalConfig();
}

void CDlgDoTest::OnBtnAbility() 
{
	// TODO: Add your control notification handler code here
	CString tmp;
	NET_DVR_CODESYSTEMABILITY m_Ability;
	memset(&m_Ability, 0, sizeof(m_Ability));
    if (!NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, VIDEOPLATFORM_SBUCODESYSTEM_ABILITY, NULL, 0, (char*)&m_Ability, sizeof(m_Ability)))
    {
        tmp.Format("Error code : %d", NET_DVR_GetLastError());
        AfxMessageBox(tmp);
        return;
    }
}
void  CALLBACK VcaDebug(LONG lHandle, LONG lCHannel, char *pRecvDataBuffer, DWORD dwBufSize, void *pUser)
{
//     char *pBuf = new char[dwBufSize];

    TRACE(pRecvDataBuffer);

}

void CDlgDoTest::OnBtnVcaDebug() 
{
//     int lChannel =  1;
//     if (!m_bDebugVca)
//     {
//         m_lDebugHandle = NET_DVR_AlgoDebugStart(g_struDeviceInfo[m_iDeviceIndex].lLoginID, lChannel, VcaDebug, NULL);
//         if (m_lDebugHandle == -1)
//         {
//             g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_AlgoLibInfoOutput_Start");
//         }
//         else
//         {
//             g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_AlgoLibInfoOutput_Start");
//             m_bDebugVca = !m_bDebugVca;
//             GetDlgItem(IDC_BTN_VCA_DEBUG)->SetWindowText("Stop Debug");
//         }
//     }
//     else
//     {
//         m_bDebugVca = !m_bDebugVca;
//         GetDlgItem(IDC_BTN_VCA_DEBUG)->SetWindowText("Debug Vca");
//         if (m_lDebugHandle >= 0)
//         {
//             NET_DVR_AlgoDebugStop(m_lDebugHandle);
//         }
//     }
}

void CDlgDoTest::OnBtnAbilityparse() 
{
	// TODO: Add your control notification handler code here
	CDlgAbilityAutoDetect dlg;
	dlg.m_lServerID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	if (dlg.m_lServerID < 0)
	{
		AfxMessageBox("请先登录设备!");
	}
	dlg.DoModal();	
}

/*********************************************************
  Function:	CDlgDoTest::OnBtnVerHeadSet
  Desc:		Set the parameter of the version head struction.
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CDlgDoTest::OnBtnVerHeadSet() 
{
    UpdateData(TRUE);
    NET_DVR_TEST_VERSION_HEAD struTestVersionHead = {0};
    struTestVersionHead.dwSize = sizeof(NET_DVR_TEST_VERSION_HEAD);
    struTestVersionHead.dwParam1 = m_iVerHeadParam1;
    struTestVersionHead.byParam2 = m_iVerHeadParam2;

    BOOL bRet = FALSE;
    if (m_ComboHeadModel.GetCurSel() == 0)
    {
        //仅有一个版本的情况
        bRet = NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_TEST_VERSION_HEAD_ONLY_0, 0, &struTestVersionHead, sizeof(struTestVersionHead));
    } 
    else
    {
        //多个版本
        switch (m_ComboHeadVer.GetCurSel())
        {
        case 0:
            bRet = NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_TEST_VERSION_HEAD, 0, &struTestVersionHead, sizeof(struTestVersionHead));
        	break;
        case 1:
            {
                NET_DVR_TEST_VERSION_HEAD_V1 struTestVersionHeadV1 = {0};
                struTestVersionHeadV1.dwSize = sizeof(struTestVersionHeadV1);
                struTestVersionHeadV1.dwParam1 = m_iVerHeadParam1;
                struTestVersionHeadV1.byParam2 = m_iVerHeadParam2;
                struTestVersionHeadV1.dwParam1_1 = m_iVerHeadParam1_1;
                struTestVersionHeadV1.byParam1_2 = m_iVerHeadParam1_2;
                bRet = NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_TEST_VERSION_HEAD_V1, 0, &struTestVersionHeadV1, sizeof(struTestVersionHeadV1));
            }
            break;
        case 2:
            {
                NET_DVR_TEST_VERSION_HEAD_V2 struTestVersionHeadV2 = {0};
                struTestVersionHeadV2.dwSize = sizeof(struTestVersionHeadV2);
                struTestVersionHeadV2.dwParam1 = m_iVerHeadParam1;
                struTestVersionHeadV2.byParam2 = m_iVerHeadParam2;
                struTestVersionHeadV2.dwParam1_1 = m_iVerHeadParam1_1;
                struTestVersionHeadV2.byParam1_2 = m_iVerHeadParam1_2;
                struTestVersionHeadV2.dwParam2_1 = m_iVerHeadParam2_1;
                struTestVersionHeadV2.byParam2_2 = m_iVerHeadParam2_2;
                bRet = NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_SET_TEST_VERSION_HEAD_V2, 0, &struTestVersionHeadV2, sizeof(struTestVersionHeadV2));
            }
            break;
        default:
            break;
        }   
    }

    if (bRet)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "SET TEST_VERSION_HEAD");
    }
    else   
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "SET TEST_VERSION_HEAD");
	}

}

/*********************************************************
  Function:	CDlgDoTest::OnBtnVerHeadSet
  Desc:		Get the parameter of the version head struction.
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CDlgDoTest::OnBtnVerHeadGet() 
{
    DWORD dwRet = 0;
    BOOL bRet = FALSE;

    if (m_ComboHeadModel.GetCurSel() == 0)
    {
        NET_DVR_TEST_VERSION_HEAD struTestVersionHead = {0};
        //仅有一个版本的情况
        bRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_TEST_VERSION_HEAD_ONLY_0,0, &struTestVersionHead, sizeof(struTestVersionHead), &dwRet);
        m_iVerHeadParam1 = struTestVersionHead.dwParam1;
        m_iVerHeadParam2 = struTestVersionHead.byParam2;
    } 
    else
    {
        //多个版本
        switch (m_ComboHeadVer.GetCurSel())
        {
        case 0:
            {
                NET_DVR_TEST_VERSION_HEAD struTestVersionHead = {0};
                bRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_TEST_VERSION_HEAD,0, &struTestVersionHead, sizeof(struTestVersionHead), &dwRet);
                m_iVerHeadParam1 = struTestVersionHead.dwParam1;
                m_iVerHeadParam2 = struTestVersionHead.byParam2;
            }
            break;
        case 1:
            {
                NET_DVR_TEST_VERSION_HEAD_V1 struTestVersionHeadV1 = {0};
                bRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_TEST_VERSION_HEAD_V1,0, &struTestVersionHeadV1, sizeof(struTestVersionHeadV1), &dwRet);
                m_iVerHeadParam1 = struTestVersionHeadV1.dwParam1;
                m_iVerHeadParam2 = struTestVersionHeadV1.byParam2;
                m_iVerHeadParam1_1 = struTestVersionHeadV1.dwParam1_1;
                m_iVerHeadParam1_2 = struTestVersionHeadV1.byParam1_2;
            }
            break;
        case 2:
            {
                NET_DVR_TEST_VERSION_HEAD_V2 struTestVersionHeadV2 = {0};
                bRet = NET_DVR_GetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_GET_TEST_VERSION_HEAD_V2,0, &struTestVersionHeadV2, sizeof(struTestVersionHeadV2), &dwRet);
                m_iVerHeadParam1 = struTestVersionHeadV2.dwParam1;
                m_iVerHeadParam2 = struTestVersionHeadV2.byParam2;
                m_iVerHeadParam1_1 = struTestVersionHeadV2.dwParam1_1;
                m_iVerHeadParam1_2 = struTestVersionHeadV2.byParam1_2;
                m_iVerHeadParam2_1 = struTestVersionHeadV2.dwParam2_1;
                m_iVerHeadParam2_2 = struTestVersionHeadV2.byParam2_2;
            }
            break;
        default:
            break;
        }   
    }
    
    if (bRet)
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_TEST_VERSION_HEAD");
    }
    else   
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_TEST_VERSION_HEAD");
	}

	UpdateData(FALSE);
}

/*********************************************************
  Function:	CDlgDoTest::OnBtnVerHeadSet
  Desc:		Set global version of parameter configuration.
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CDlgDoTest::OnBtnSetHeadVersion()
{
    UpdateData(TRUE);

    NET_DVR_LOCAL_CFG_VERSION struCfgVer = {0};
    struCfgVer.byVersion = m_iVersion;
    
    if(!NET_DVR_SetSDKLocalCfg(NET_SDK_LOCAL_CFG_TYPE_CFG_VERSION, &struCfgVer))
    {
        g_pMainDlg->AddLog(-1, OPERATION_FAIL_T, "NET_DVR_SetSDKLocalCfg NET_SDK_LOCAL_CFG_TYPE_MEM_POOL");
    }
    else
    {
        g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, "NET_DVR_SetSDKLocalCfg NET_SDK_LOCAL_CFG_TYPE_MEM_POOL");
	}
}




