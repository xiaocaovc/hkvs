// DlgAlarmhostSensorLinkage.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAlarmhostSensorLinkage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmhostSensorLinkage dialog


CDlgAlarmhostSensorLinkage::CDlgAlarmhostSensorLinkage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarmhostSensorLinkage::IDD, pParent)
	,m_iDeviceIndex(-1)
	,m_lUserID(-1)
{
	//{{AFX_DATA_INIT(CDlgAlarmhostSensorLinkage)
	m_chJointByAlarmType = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgAlarmhostSensorLinkage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarmhostSensorLinkage)
	DDX_Control(pDX, IDC_LIST_SIREN_LINKAGE_BY_TYPE, m_listSirenType);
	DDX_Control(pDX, IDC_LIST_ALARMOUT_LINKAGE_BY_TYPE, m_listSensorType);
	DDX_Control(pDX, IDC_COMBO_SENSOR_JOINT_ALARMOUT, m_cmAlarmOutChan);
	DDX_Control(pDX, IDC_COMBO_SENSOR_INDEX, m_cmSensorIndex);
	DDX_Control(pDX, IDC_LIST_SIREN_LINKAGE, m_listSiren);
	DDX_Control(pDX, IDC_LIST_ALARMOUT_LINKAGE, m_listAlarmOut);
	DDX_Check(pDX, IDC_CHECK_JOINT_BY_ALARMTYPE, m_chJointByAlarmType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarmhostSensorLinkage, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarmhostSensorLinkage)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_CBN_SELCHANGE(IDC_COMBO_SENSOR_INDEX, OnSelchangeComboSensorIndex)
	ON_BN_CLICKED(IDC_CHECK_JOINT_BY_ALARMTYPE, OnCheckJointByAlarmtype)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_CBN_SELCHANGE(IDC_COMBO_SENSOR_JOINT_ALARMOUT, OnSelchangeComboSensorJointAlarmout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmhostSensorLinkage message handlers

BOOL CDlgAlarmhostSensorLinkage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lUserID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	memset(&m_struSensor, 0, sizeof(m_struSensor));
	DWORD dwReturn = 0;
    if (NET_DVR_GetDeviceAbility(m_lUserID, ALARMHOST_ABILITY, NULL, 0, (char*)&m_struAlarmHostAbility, sizeof(m_struAlarmHostAbility)))
    {
        char szLan[128] = {0};
        g_pMainDlg->AddLog(m_lUserID, OPERATION_SUCC_T, "ALARMHOST_ABILITY");
    }
    else
    {
        g_pMainDlg->AddLog(m_lUserID, OPERATION_FAIL_T, "ALARMHOST_ABILITY");
        return FALSE;
    }	

	CString csStr;
	int i=0;
	for (i=0; i<(m_struAlarmHostAbility.wLocalSensorNum+m_struAlarmHostAbility.wExpandSensorNum); i++)
	{
		csStr.Format("%d", i+1);
		m_cmSensorIndex.AddString(csStr);
	}

	for (i=0; i<(m_struAlarmHostAbility.wLocalAlarmOutNum + m_struAlarmHostAbility.wExpandAlarmOutNum); i++)
	{
		csStr.Format("%d", i+1);
		m_cmAlarmOutChan.AddString(csStr);
	}
	InitListWnd();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarmhostSensorLinkage::InitListWnd() 
{
	char szLan[128] = {0};
	int iBitIndex = 0;
	BOOL bBitState = 0;
    m_listAlarmOut.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listSiren.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listSensorType.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listSirenType.SetExtendedStyle(LVS_EX_CHECKBOXES);

	int i = 0; 
	for (i=0; i<(m_struAlarmHostAbility.wLocalAlarmOutNum + m_struAlarmHostAbility.wExpandAlarmOutNum); i++)
	{
		sprintf(szLan, "Alarmout %d", i+1);
		m_listAlarmOut.InsertItem(i, szLan);
	}

	for (i=0; i<m_struAlarmHostAbility.wSirenNum; i++)
	{
		sprintf(szLan, "Siren %d", i+1);
		m_listSiren.InsertItem(i, szLan);
	}
	
	int iIndex = 0;
	m_listSensorType.InsertItem(iIndex++, "Limit1");
	m_listSensorType.InsertItem(iIndex++, "Limit2");
	m_listSensorType.InsertItem(iIndex++, "Limit3");
	m_listSensorType.InsertItem(iIndex++, "Limit4");
	m_listSensorType.InsertItem(iIndex++, "Fault");

	iIndex = 0;
	m_listSirenType.InsertItem(iIndex++, "Limit1");
	m_listSirenType.InsertItem(iIndex++, "Limit2");
	m_listSirenType.InsertItem(iIndex++, "Limit3");
	m_listSirenType.InsertItem(iIndex++, "Limit4");
	m_listSirenType.InsertItem(iIndex++, "Fault");
}



void CDlgAlarmhostSensorLinkage::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwReturn = 0;
	int i = 0;
	int iIndex = m_cmSensorIndex.GetCurSel() + 1;
	m_struSensor.dwSize = sizeof(m_struSensor);

	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_ALARMHOST_SENSOR_JOINT_CFG, iIndex, &m_struSensor, sizeof(m_struSensor), &dwReturn))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Get sensor linkage failed");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Get sensor linkage successful");
	}

	//clear
	for (i=0; i<m_listAlarmOut.GetItemCount(); i++)
	{
		m_listAlarmOut.SetCheck(i, 0);
	}
	
	for (i=0; i<m_listSiren.GetItemCount(); i++)
	{
		m_listSiren.SetCheck(i, 0);
	}
	//set data
	for (i=0; i<m_listAlarmOut.GetItemCount(); i++)
	{
		m_listAlarmOut.SetCheck(i, m_struSensor.bySensorJointAlarmOut[i]);
	}

	for (i=0; i<m_listSiren.GetItemCount(); i++)
	{
		m_listSiren.SetCheck(i, m_struSensor.bySensorJointSiren[i]);
	}

	for (i=0; i<m_listSirenType.GetItemCount(); i++)
	{
		m_listSirenType.SetCheck(i, (BOOL)((m_struSensor.bySesorAlarmTypeJointSiren[0]>>i)&0x01));
	}

	m_cmAlarmOutChan.SetCurSel(0);
	UpdateData(FALSE);
	OnSelchangeComboSensorJointAlarmout();
	UpdateData(FALSE);
}

void CDlgAlarmhostSensorLinkage::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	OnBtnSave();
	UpdateData(TRUE);
	int i = 0;	
	for (i=0; i<m_listAlarmOut.GetItemCount(); i++)
	{
		m_struSensor.bySensorJointAlarmOut[i] = m_listAlarmOut.GetCheck(i);
	}

	for (i=0; i<m_listSiren.GetItemCount(); i++)
	{
		m_struSensor.bySensorJointSiren[i] = m_listSiren.GetCheck(i);
	}

	int iIndex = m_cmSensorIndex.GetCurSel() + 1;
	m_struSensor.dwSize = sizeof(m_struSensor);
	if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_ALARMHOST_SENSOR_JOINT_CFG, iIndex, &m_struSensor, sizeof(m_struSensor)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Set sensor linkage failed");
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Set sensor linkage successful");
	}
}

void CDlgAlarmhostSensorLinkage::OnSelchangeComboSensorIndex() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	OnBtnGet();
}

void CDlgAlarmhostSensorLinkage::OnCheckJointByAlarmtype() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(1 == m_chJointByAlarmType)
	{
		GetDlgItem(IDC_LIST_ALARMOUT_LINKAGE_BY_TYPE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_CHAN_NO)->ShowWindow(TRUE);
		GetDlgItem(IDC_COMBO_SENSOR_JOINT_ALARMOUT)->ShowWindow(TRUE);
		g_StringLanType(m_szTemp, "模拟量类型联动触发器", "Sensor Type");
		SetDlgItemText(IDC_STATIC1, m_szTemp);
		GetDlgItem(IDC_LIST_ALARMOUT_LINKAGE)->ShowWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE)->ShowWindow(TRUE);

		g_StringLanType(m_szTemp, "模拟量类型联动警号", "Sensor Type");
		SetDlgItemText(IDC_STATIC2, m_szTemp);
		GetDlgItem(IDC_LIST_SIREN_LINKAGE)->ShowWindow(FALSE);
		GetDlgItem(IDC_LIST_SIREN_LINKAGE_BY_TYPE)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_LIST_ALARMOUT_LINKAGE_BY_TYPE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_CHAN_NO)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO_SENSOR_JOINT_ALARMOUT)->ShowWindow(FALSE);
		g_StringLanType(m_szTemp, "报警输出（触发器）", "Alarmout");
		SetDlgItemText(IDC_STATIC1, m_szTemp);
		GetDlgItem(IDC_LIST_ALARMOUT_LINKAGE)->ShowWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVE)->ShowWindow(FALSE);

		g_StringLanType(m_szTemp, "警号", "Siren");
		SetDlgItemText(IDC_STATIC2, m_szTemp);
		GetDlgItem(IDC_LIST_SIREN_LINKAGE_BY_TYPE)->ShowWindow(FALSE);
		GetDlgItem(IDC_LIST_SIREN_LINKAGE)->ShowWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDlgAlarmhostSensorLinkage::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iAlarmoutNo = m_cmAlarmOutChan.GetCurSel();
	BYTE bySensorType = 0;
	m_struSensor.bySensorAlarmTypeJointAlarmOut[iAlarmoutNo] = 0;
	int i=0;
	for (i=0; i<m_listSensorType.GetItemCount(); i++)
	{
		bySensorType = m_listSensorType.GetCheck(i);
		bySensorType = bySensorType << i;
		m_struSensor.bySensorAlarmTypeJointAlarmOut[iAlarmoutNo] |= bySensorType;
	}

	m_struSensor.bySesorAlarmTypeJointSiren[0] = 0;
	for (i=0; i<m_listSirenType.GetItemCount(); i++)
	{
		bySensorType = m_listSirenType.GetCheck(i) << i;
		m_struSensor.bySesorAlarmTypeJointSiren[0] |= bySensorType;
	}
	UpdateData(FALSE);
}

void CDlgAlarmhostSensorLinkage::OnSelchangeComboSensorJointAlarmout() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iAlarmoutNo = m_cmAlarmOutChan.GetCurSel();
	BYTE bySensorType = 0;
	int i=0;
	for (i=0; i<m_listSensorType.GetItemCount(); i++)
	{
		m_listSensorType.SetCheck(i, FALSE);
	}

	for (i=0; i<m_listSensorType.GetItemCount(); i++)
	{
		m_listSensorType.SetCheck(i, (BOOL)((m_struSensor.bySensorAlarmTypeJointAlarmOut[iAlarmoutNo]>>i) & 0x01));
	}
	UpdateData(FALSE);
}
