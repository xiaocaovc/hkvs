/**********************************************************
FileName:    DlgPTZCruise.cpp
Description: add cruise configuration     
Date:        2008/06/28
Note: 		 <global>struct, refer to GeneralDef.h, global variants and functions refer to ClientDemo.cpp 
Modification History:      
    <version> <time>         <desc>
    <1.0    > <2008/06/28>       <created>
***********************************************************/

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgPTZCruise.h"
#include ".\DlgPTZCruise.h"

extern BOOL PTZCruiseAll(LONG lRealHandle, int iDevIndex, int iChanIndex, DWORD dwPTZCruiseCmd,BYTE byCruiseRoute, BYTE byCruisePoint, WORD wInput, char *szCmdString);

// CDlgPTZCruise dialog
/*********************************************************
  Function:	CDlgPTZCruise
  Desc:		constructor
  Input:	
  Output:	
  Return:	
**********************************************************/
IMPLEMENT_DYNAMIC(CDlgPTZCruise, CDialog)
CDlgPTZCruise::CDlgPTZCruise(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPTZCruise::IDD, pParent)
	, m_lRealHandle(0)
	, m_iCruiseNum(1)
	, m_iSeqPoint(1)
	, m_iPresetNum(1)
	, m_iSeqDwell(2)
	, m_iSeqSpeed(30)
	, m_bPrivateIPMode(FALSE)
{
	memset(&m_struCruiseRet, 0, sizeof(NET_DVR_CRUISE_RET));
	memset(&m_struCruisePointCond, 0, sizeof(m_struCruisePointCond));
	memset(&m_struCruisePointV40, 0, sizeof(m_struCruisePointV40));
}

/*********************************************************
  Function:	~CDlgPTZCruise
  Desc:		destructor
  Input:	
  Output:	
  Return:	
**********************************************************/
CDlgPTZCruise::~CDlgPTZCruise()
{
}

/*********************************************************
Function:	DoDataExchange
Desc:		the map between control and variable
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPTZCruise::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPTZCruise)
	DDX_Control(pDX, IDC_COMBO_CRUISE_PRESET, m_comboSeqPreset);
	DDX_Control(pDX, IDC_COMBO_CRUISE_TIME, m_comboSeqTime);
	DDX_Control(pDX, IDC_COMBO_CRUISE_SPEED, m_comboSeqSpeed);
	DDX_Control(pDX, IDC_COMBO_CRUISE_POINT, m_comboPoint);
	DDX_Control(pDX, IDC_COMBO_CRUISE_ROUTE, m_comboCruiseRoute);
    //}}AFX_DATA_MAP
}

/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CDlgPTZCruise, CDialog)
	//{{AFX_MSG_MAP(CDlgPTZCruise)	
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_CBN_SELCHANGE(IDC_COMBO_CRUISE_POINT, OnCbnSelchangeComboCruisePoint)
	ON_CBN_SELCHANGE(IDC_COMBO_CRUISE_ROUTE, OnCbnSelchangeComboCruiseRoute)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgPTZCruise message handlers
/*********************************************************
  Function:	OnInitDialog
  Desc:		Initialize the dialog
  Input:	
  Output:	
  Return:	
**********************************************************/
BOOL CDlgPTZCruise::OnInitDialog()
{
	CDialog::OnInitDialog();
	char szLan[128] = {0};
	int i=0;
	char szNum[32] = {0};
	for (i= 0; i<16+1; i++)//0 and the max number is just for test;
	{
		g_StringLanType(szLan, "Ñ²º½Â·¾¶", "Cruise route");
		sprintf(szNum, "%s%d", szLan, i);
		m_comboCruiseRoute.AddString(szNum);
	}
	m_comboCruiseRoute.SetCurSel(1);

	for (i=0; i<128/*32*/+1; i++)
	{
		sprintf(szNum, "%d", i);
		m_comboPoint.AddString(szNum);
	}
	m_comboPoint.SetCurSel(1);

	for (i=0; i<300/*MAX_PRESET_V30*/; i++)
	{
		sprintf(szNum, "%d", i+1);
		m_comboSeqPreset.AddString(szNum);
	}
	m_comboSeqPreset.SetCurSel(1);

	for (i=0; i<256+1; i++)
	{
		sprintf(szNum, "%d", i);
		m_comboSeqTime.AddString(szNum);
	}
	m_comboSeqTime.SetCurSel(2);

	for (i=0; i<256+1; i++)
	{
		sprintf(szNum, "%d", i);
		m_comboSeqSpeed.AddString(szNum);
	}
	m_comboSeqSpeed.SetCurSel(30);

	m_lRealHandle = g_dlgOutput[g_pMainDlg->m_iCurWndIndex].m_lPlayHandle;
	OnCbnSelchangeComboCruiseRoute();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*********************************************************
  Function:	OnBnClickedButtonAdd
  Desc:		add cruise route
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgPTZCruise::OnBnClickedButtonAdd()
{
	UpdateData(TRUE);
	BOOL bRet = FALSE;
	m_iCruiseNum = m_comboCruiseRoute.GetCurSel();

	m_iSeqPoint = m_comboPoint.GetCurSel();
	m_iPresetNum = m_comboSeqPreset.GetCurSel();
	m_iSeqDwell = m_comboSeqTime.GetCurSel();
	m_iSeqSpeed = m_comboSeqSpeed.GetCurSel();

	int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
	int iChanIndex = g_pMainDlg->GetCurChanIndex();
	bRet = PTZCruiseAll(m_lRealHandle,  iDevIndex,\
			iChanIndex, FILL_PRE_SEQ, BYTE(m_iCruiseNum), BYTE(m_iSeqPoint), (WORD)m_iPresetNum, "FILL_PRE_SEQ");
	if (bRet)
	{
		bRet = PTZCruiseAll(m_lRealHandle,iDevIndex,\
			iChanIndex, SET_SEQ_DWELL,BYTE(m_iCruiseNum), BYTE(m_iSeqPoint), (WORD)m_iSeqDwell, "SET_SEQ_DWELL");
		if (bRet)
		{
			bRet = PTZCruiseAll(m_lRealHandle, iDevIndex,\
			iChanIndex, SET_SEQ_SPEED, BYTE(m_iCruiseNum) ,BYTE(m_iSeqPoint), (WORD)m_iSeqSpeed, "SET_SEQ_SPEED");
		}
	}	
}

/*********************************************************
  Function:	OnBnClickedButtonDel
  Desc:		delete cruise route
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgPTZCruise::OnBnClickedButtonDel()
{	
	UpdateData(TRUE);
	m_iCruiseNum = m_comboCruiseRoute.GetCurSel();
	m_iSeqPoint = m_comboPoint.GetCurSel();
	int iDevIndex=g_pMainDlg->GetCurDeviceIndex();
	int iChanIndex = g_pMainDlg->GetCurChanIndex();
	PTZCruiseAll(m_lRealHandle,iDevIndex,\
			iChanIndex, CLE_PRE_SEQ, BYTE(m_iCruiseNum), BYTE(m_iSeqPoint), (WORD)m_iPresetNum, "CLE_PRE_SEQ");	
}

/*********************************************************
  Function:	OnCbnSelchangeComboCruisePoint
  Desc:		display cruise info
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgPTZCruise::OnCbnSelchangeComboCruisePoint()
{
	UpdateData(TRUE);
	m_iSeqPoint = m_comboPoint.GetCurSel();
	if (m_iSeqPoint < 1)
	{
		m_comboSeqPreset.SetCurSel(0);
		m_comboSeqTime.SetCurSel(0);
		m_comboSeqSpeed.SetCurSel(0);
		return;
	}

	if (m_bPrivateIPMode)
	{//only private ip dome support this interface

// 		g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, "NET_DVR_GetPTZCruise cruise[%d] point[%d]\
// 			preset[%d] time[%d]speed[%d]", m_iCruiseNum, m_iSeqPoint, m_struCruiseRet.struCruisePoint[m_iSeqPoint-1].PresetNum,\
// 			m_struCruiseRet.struCruisePoint[m_iSeqPoint-1].Dwell, m_struCruiseRet.struCruisePoint[m_iSeqPoint-1].Speed);
// 		m_comboSeqPreset.SetCurSel(m_struCruiseRet.struCruisePoint[m_iSeqPoint-1].PresetNum);
// 		m_comboSeqTime.SetCurSel(m_struCruiseRet.struCruisePoint[m_iSeqPoint-1].Dwell);
// 		m_comboSeqSpeed.SetCurSel(m_struCruiseRet.struCruisePoint[m_iSeqPoint-1].Speed);
		
		g_pMainDlg->AddLog(-1, OPERATION_SUCC_T, "NET_GET_CRUISEPOINT_V40 cruise[%d] point[%d]\
						   preset[%d] time[%d] speed[%d]", m_iCruiseNum, m_iSeqPoint, m_struCruisePointV40.struCruisePoint[m_iSeqPoint-1].wPresetNo,\
						   m_struCruisePointV40.struCruisePoint[m_iSeqPoint-1].wDwell, m_struCruisePointV40.struCruisePoint[m_iSeqPoint-1].bySpeed);
		m_comboSeqPreset.SetCurSel(m_struCruisePointV40.struCruisePoint[m_iSeqPoint-1].wPresetNo);
		m_comboSeqTime.SetCurSel(m_struCruisePointV40.struCruisePoint[m_iSeqPoint-1].wDwell);
				m_comboSeqSpeed.SetCurSel(m_struCruisePointV40.struCruisePoint[m_iSeqPoint-1].bySpeed);
	}
	else
	{
		//default
		m_comboSeqPreset.SetCurSel(1);
		m_comboSeqTime.SetCurSel(3);
		m_comboSeqSpeed.SetCurSel(29);
	}

	UpdateData(FALSE);
}

/*********************************************************
  Function:	OnCbnSelchangeComboCruiseRoute
  Desc:		get cruise route
  Input:	
  Output:	
  Return:	
**********************************************************/
void CDlgPTZCruise::OnCbnSelchangeComboCruiseRoute()
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	int iStatus = 0;
	m_iCruiseNum = m_comboCruiseRoute.GetCurSel();
	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_iSeqPoint = 1;
	m_comboPoint.SetCurSel(m_iSeqPoint);
	int iSeqPoint = m_iSeqPoint - 1;
	if (iDeviceIndex < 0)
	{
		return;
	}

	int iChanIndex = g_pMainDlg->GetCurChanIndex();

	if (iChanIndex == -1)
	{
		return;
	}

	/*
	if (!NET_DVR_GetPTZCruise(g_struDeviceInfo[iDeviceIndex].lLoginID, iChanIndex+g_struDeviceInfo[iDeviceIndex].iStartChan, m_iCruiseNum, &m_struCruiseRet))
	{
	m_bPrivateIPMode = FALSE;
	g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetPTZCruise %s cruise[%d] ", g_struDeviceInfo[iDeviceIndex].pStruChanInfo[iChanIndex].chChanName, m_iCruiseNum);
	}
	else
	{
	m_bPrivateIPMode = TRUE;
	g_pMainDlg->AddLog(iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GetPTZCruise cruise[%d] point[%d]preset[%d]dwell[%d]speed[%d] %s",\
	m_iCruiseNum, m_iSeqPoint,\
	m_struCruiseRet.struCruisePoint[iSeqPoint].PresetNum, m_struCruiseRet.struCruisePoint[iSeqPoint].Dwell, \
	m_struCruiseRet.struCruisePoint[iSeqPoint].Speed,\
	g_struDeviceInfo[iDeviceIndex].pStruChanInfo[iChanIndex].chChanName);
	m_comboSeqPreset.SetCurSel(m_struCruiseRet.struCruisePoint[iSeqPoint].PresetNum);
	m_comboSeqTime.SetCurSel(m_struCruiseRet.struCruisePoint[iSeqPoint].Dwell);
	m_comboSeqSpeed.SetCurSel(m_struCruiseRet.struCruisePoint[iSeqPoint].Speed);
	}
	*/
	
	m_struCruisePointCond.dwSize = sizeof(m_struCruisePointCond);
	m_struCruisePointCond.dwChan = iChanIndex+g_struDeviceInfo[iDeviceIndex].iStartChan;
	m_struCruisePointCond.wRouteNo = m_iCruiseNum;
	BOOL bRet = NET_DVR_GetDeviceConfig(g_struDeviceInfo[iDeviceIndex].lLoginID, NET_GET_CRUISEPOINT_V40, 1, &m_struCruisePointCond, \
		sizeof(m_struCruisePointCond), &iStatus, &m_struCruisePointV40, sizeof(m_struCruisePointV40));
	if (bRet && iStatus == 0)
	{
		m_bPrivateIPMode = TRUE;
		g_pMainDlg->AddLog(iDeviceIndex, OPERATION_SUCC_T, "NET_GET_CRUISEPOINT_V40 cruise[%d] point[%d]preset[%d]dwell[%d]speed[%d] %s",\
			m_iCruiseNum, m_iSeqPoint,\
			m_struCruisePointV40.struCruisePoint[iSeqPoint].wPresetNo, m_struCruisePointV40.struCruisePoint[iSeqPoint].wDwell, \
			m_struCruisePointV40.struCruisePoint[iSeqPoint].bySpeed, \
			g_struDeviceInfo[iDeviceIndex].pStruChanInfo[iChanIndex].chChanName);
		m_comboSeqPreset.SetCurSel(m_struCruisePointV40.struCruisePoint[iSeqPoint].wPresetNo);
		m_comboSeqTime.SetCurSel(m_struCruisePointV40.struCruisePoint[iSeqPoint].wDwell);
		m_comboSeqSpeed.SetCurSel(m_struCruisePointV40.struCruisePoint[iSeqPoint].bySpeed);
		
    }
    else
    {
		m_bPrivateIPMode = FALSE;
        sprintf(szLan,"err %d",iStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "NET_GET_CRUISEPOINT_V40");
    }
}

/*********************************************************
  Function:	OnBnClickedBtnExit
  Desc:		exit the dialog
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void CDlgPTZCruise::OnBnClickedBtnExit()
{
	CDialog::OnCancel();
}
