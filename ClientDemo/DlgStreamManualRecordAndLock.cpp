// DlgStreamManualRecordAndLock.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgStreamManualRecordAndLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStreamManualRecordAndLock dialog


CDlgStreamManualRecordAndLock::CDlgStreamManualRecordAndLock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStreamManualRecordAndLock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStreamManualRecordAndLock)
	m_iChannel = 0;
	m_strStreamID = _T("");
	m_ctDateEnd = 0;
	m_ctDateStart = 0;
	m_ctTimeEnd = 0;
	m_ctTimeStart = 0;
	m_iLockDuration = 0;
	m_ctDateEndReturn = 0;
	m_ctDateStartReturn = 0;
	m_ctTimeEndReturn = 0;
	m_ctTimeStartReturn = 0;
	m_ctUnlockTimePointDate = 0;
	m_ctUnlockTimePointTime = 0;
	//}}AFX_DATA_INIT
}


void CDlgStreamManualRecordAndLock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStreamManualRecordAndLock)
	DDX_Control(pDX, IDC_COMBO_LOCK_RECORD_TYPE, m_cmbLockRecordType);
	DDX_Control(pDX, IDC_COMBO_RECORD_TYPE, m_cmbManualRecordType);
	DDX_Text(pDX, IDC_EDIT_CHANNEL, m_iChannel);
	DDX_Text(pDX, IDC_EDIT_ID, m_strStreamID);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_DATE_END, m_ctDateEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_DATE_START, m_ctDateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_TIME_END, m_ctTimeEnd);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_TIME_START, m_ctTimeStart);
	DDX_Text(pDX, IDC_EDIT_LOCK_DURATION, m_iLockDuration);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_DATE_END_RETURN, m_ctDateEndReturn);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_DATE_START_RETURN, m_ctDateStartReturn);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_TIME_END_RETURN, m_ctTimeEndReturn);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_LOCK_TIME_START_RETURN, m_ctTimeStartReturn);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_UNLOCK_TIME_POINT_DATE, m_ctUnlockTimePointDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_UNLOCK_TIME_POINT_TIME, m_ctUnlockTimePointTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStreamManualRecordAndLock, CDialog)
	//{{AFX_MSG_MAP(CDlgStreamManualRecordAndLock)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_LOCK, OnButtonLock)
	ON_BN_CLICKED(IDC_BUTTON_UNLOCK, OnButtonUnlock)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStreamManualRecordAndLock message handlers


BOOL CDlgStreamManualRecordAndLock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);

	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),0,0,0);
	CTime timeStop(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),23,59,59);

	m_ctDateStart = timeStart;
	m_ctTimeStart = timeStart;
	m_ctDateStartReturn = timeStart;
	m_ctTimeStartReturn = timeStart;
	m_ctDateEnd = timeStop;
	m_ctDateEndReturn = timeStop;
	m_ctTimeEnd = timeStop;
	m_ctTimeEndReturn = timeStop;

	m_ctUnlockTimePointDate = timeCur;
	m_ctUnlockTimePointTime = timeCur;

	m_cmbManualRecordType.SetCurSel(0);
	m_cmbLockRecordType.SetCurSel(8);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgStreamManualRecordAndLock::OnButtonLock() 
{
	// TODO: Add your control notification handler code here
	m_iLockDuration = -2;
	UpdateData(TRUE);
    char szLan[128] = {0};

// 	if ("" == m_strStreamID)
// 	{
// 		g_StringLanType(szLan,"流ID为空","Stream ID is NULL");
// 		AfxMessageBox(szLan);
// 		return;
// 	}

	NET_DVR_STREAM_TIME_LOCK struTimeLock;
	memset(&struTimeLock, 0, sizeof(NET_DVR_STREAM_TIME_LOCK));

	if (-1 == m_iLockDuration)
	{
		struTimeLock.dwLockDuration = 0xffffffff;
	}
	else if(m_iLockDuration >= 0)
	{
		struTimeLock.dwLockDuration = m_iLockDuration;
	}
	else
	{
		AfxMessageBox("Input err");
		return;
	}
	

	struTimeLock.dwSize = sizeof(NET_DVR_STREAM_TIME_LOCK);
	struTimeLock.struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
	struTimeLock.struStreamInfo.dwChannel = m_iChannel;
	strncpy((char*)struTimeLock.struStreamInfo.byID, m_strStreamID, STREAM_ID_LEN);

	int iRecordType = m_cmbLockRecordType.GetCurSel();
	if (8 == iRecordType)
	{
		iRecordType = 0xffffffff;
	}
	struTimeLock.dwRecordType = iRecordType;

	struTimeLock.strBeginTime.dwYear = (DWORD)m_ctDateStart.GetYear();
	struTimeLock.strBeginTime.dwMonth = (DWORD)m_ctDateStart.GetMonth();
	struTimeLock.strBeginTime.dwDay = (DWORD)m_ctDateStart.GetDay();
	struTimeLock.strBeginTime.dwHour = (DWORD)m_ctTimeStart.GetHour();
	struTimeLock.strBeginTime.dwMinute = (DWORD)m_ctTimeStart.GetMinute();
	struTimeLock.strBeginTime.dwSecond = (DWORD)m_ctTimeStart.GetSecond();
	
	struTimeLock.strEndTime.dwYear = (DWORD)m_ctDateEnd.GetYear();
	struTimeLock.strEndTime.dwMonth = (DWORD)m_ctDateEnd.GetMonth();
	struTimeLock.strEndTime.dwDay = (DWORD)m_ctDateEnd.GetDay();
	struTimeLock.strEndTime.dwHour = (DWORD)m_ctTimeEnd.GetHour();
	struTimeLock.strEndTime.dwMinute = (DWORD)m_ctTimeEnd.GetMinute();
    struTimeLock.strEndTime.dwSecond = (DWORD)m_ctTimeEnd.GetSecond();

	struTimeLock.strUnlockTimePoint.wYear = (WORD)m_ctUnlockTimePointDate.GetYear();
	struTimeLock.strUnlockTimePoint.byMonth = (BYTE)m_ctUnlockTimePointDate.GetMonth();
	struTimeLock.strUnlockTimePoint.byDay = (BYTE)m_ctUnlockTimePointDate.GetDay();
	struTimeLock.strUnlockTimePoint.byHour = (BYTE)m_ctUnlockTimePointTime.GetHour();
	struTimeLock.strUnlockTimePoint.byMinute = (BYTE)m_ctUnlockTimePointTime.GetMinute();
	struTimeLock.strUnlockTimePoint.bySecond = (BYTE)m_ctUnlockTimePointTime.GetSecond();

	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;

	NET_DVR_LOCK_RETURN struLockReturn;
	memset(&struLockReturn, 0, sizeof(NET_DVR_LOCK_RETURN));

	struLockReturn.dwSize = sizeof(NET_DVR_LOCK_RETURN);

	if (NET_DVR_LockStreamFileByTime(m_lLoginID, &struTimeLock, &struLockReturn))
	{

		if(struLockReturn.strBeginTime.dwDay < 1 || struLockReturn.strBeginTime.dwDay > 31)
		{
			AfxMessageBox("返回日期不对");
			return;
		}

		if(struLockReturn.strBeginTime.dwMonth < 1 || struLockReturn.strBeginTime.dwMonth > 12)
		{
			AfxMessageBox("返回月份不对");
			return;
		}

		if(struLockReturn.strBeginTime.dwYear < 1900)
		{
			AfxMessageBox("返回年份不对");
			return;
		}

		CTime tmStartReturn(struLockReturn.strBeginTime.dwYear, 
			struLockReturn.strBeginTime.dwMonth, 
			struLockReturn.strBeginTime.dwDay,
			struLockReturn.strBeginTime.dwHour,
			struLockReturn.strBeginTime.dwMinute,
			struLockReturn.strBeginTime.dwSecond);
		m_ctDateStartReturn = tmStartReturn;
		m_ctTimeStartReturn = tmStartReturn;

		CTime tmEndReturn(struLockReturn.strEndTime.dwYear, 
			struLockReturn.strEndTime.dwMonth, 
			struLockReturn.strEndTime.dwDay,
			struLockReturn.strEndTime.dwHour,
			struLockReturn.strEndTime.dwMinute,
			struLockReturn.strEndTime.dwSecond);
		m_ctDateEndReturn = tmEndReturn;
		m_ctTimeEndReturn = tmEndReturn;

		g_StringLanType(szLan,"录像段加锁成功","Success");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "File locked");
		AfxMessageBox(szLan);

	}
	else
	{        
		g_StringLanType(szLan,"录像段加锁失败","Fail to lock");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Lock file err");
		AfxMessageBox(szLan);
		return;
	}
	
	UpdateData(FALSE);

}

void CDlgStreamManualRecordAndLock::OnButtonUnlock() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    char szLan[128] = {0};

// 	if ("" == m_strStreamID)
// 	{
// 		g_StringLanType(szLan,"流ID为空","Stream ID is NULL");
// 		AfxMessageBox(szLan);
// 		return;
// 	}
	
	NET_DVR_STREAM_TIME_LOCK struTimeLock;
	memset(&struTimeLock, 0, sizeof(NET_DVR_STREAM_TIME_LOCK));
	
	struTimeLock.dwSize = sizeof(NET_DVR_STREAM_TIME_LOCK);
	struTimeLock.struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
	struTimeLock.struStreamInfo.dwChannel = m_iChannel;
	strncpy((char*)struTimeLock.struStreamInfo.byID, m_strStreamID, STREAM_ID_LEN);
	
	struTimeLock.dwLockDuration = m_iLockDuration;
	int iRecordType = m_cmbLockRecordType.GetCurSel();
	if (8 == iRecordType)
	{
		iRecordType = 0xff;
	}
	struTimeLock.dwRecordType = iRecordType;
	
	struTimeLock.strBeginTime.dwYear = (WORD)m_ctDateStart.GetYear();
	struTimeLock.strBeginTime.dwMonth = (WORD)m_ctDateStart.GetMonth();
	struTimeLock.strBeginTime.dwDay = (WORD)m_ctDateStart.GetDay();
	struTimeLock.strBeginTime.dwHour = (char)m_ctTimeStart.GetHour();
	struTimeLock.strBeginTime.dwMinute = (char)m_ctTimeStart.GetMinute();
	struTimeLock.strBeginTime.dwSecond = (char)m_ctTimeStart.GetSecond();
	
	struTimeLock.strEndTime.dwYear = (WORD)m_ctDateEnd.GetYear();
	struTimeLock.strEndTime.dwMonth = (WORD)m_ctDateEnd.GetMonth();
	struTimeLock.strEndTime.dwDay = (WORD)m_ctDateEnd.GetDay();
	struTimeLock.strEndTime.dwHour = (char)m_ctTimeEnd.GetHour();
	struTimeLock.strEndTime.dwMinute = (char)m_ctTimeEnd.GetMinute();
    struTimeLock.strEndTime.dwSecond = (char)m_ctTimeEnd.GetSecond();
	
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	
	NET_DVR_LOCK_RETURN struLockReturn;
	memset(&struLockReturn, 0, sizeof(NET_DVR_LOCK_RETURN));

	struLockReturn.dwSize = sizeof(NET_DVR_LOCK_RETURN);
	
	if (NET_DVR_UnlockStreamFileByTime(m_lLoginID, &struTimeLock, &struLockReturn))
	{

		if(struLockReturn.strBeginTime.dwDay < 1 || struLockReturn.strBeginTime.dwDay > 31)
		{
			AfxMessageBox("返回日期不对");
			return;
		}
		
		if(struLockReturn.strBeginTime.dwMonth < 1 || struLockReturn.strBeginTime.dwMonth > 12)
		{
			AfxMessageBox("返回月份不对");
			return;
		}
		
		if(struLockReturn.strBeginTime.dwYear < 1900)
		{
			AfxMessageBox("返回年份不对");
			return;
		}
		CTime tmStartReturn(struLockReturn.strBeginTime.dwYear, 
			struLockReturn.strBeginTime.dwMonth, 
			struLockReturn.strBeginTime.dwDay,
			struLockReturn.strBeginTime.dwHour,
			struLockReturn.strBeginTime.dwMinute,
			struLockReturn.strBeginTime.dwSecond);
		m_ctDateStartReturn = tmStartReturn;
		m_ctTimeStartReturn = tmStartReturn;
		
		CTime tmEndReturn(struLockReturn.strEndTime.dwYear, 
			struLockReturn.strEndTime.dwMonth, 
			struLockReturn.strEndTime.dwDay,
			struLockReturn.strEndTime.dwHour,
			struLockReturn.strEndTime.dwMinute,
			struLockReturn.strEndTime.dwSecond);
		m_ctDateEndReturn = tmEndReturn;
		m_ctTimeEndReturn = tmEndReturn;

		g_StringLanType(szLan,"录像段解锁成功","Success");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "File locked");
		AfxMessageBox(szLan);
	}
	else
	{        
		g_StringLanType(szLan,"录像段解锁失败","Fail to lock");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "Lock file err");
		AfxMessageBox(szLan);
		return;
	}

	UpdateData(FALSE);
}


void CDlgStreamManualRecordAndLock::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    char szLan[128] = {0};

// 	if ("" == m_strStreamID)
// 	{
// 		g_StringLanType(szLan,"流ID为空","Stream ID is NULL");
// 		AfxMessageBox(szLan);
// 		return;
// 	}
	
	NET_DVR_MANUAL_RECORD_PARA struRecordPara;
	memset(&struRecordPara, 0, sizeof(NET_DVR_MANUAL_RECORD_PARA));
	struRecordPara.lRecordType = m_cmbManualRecordType.GetCurSel();
	struRecordPara.struStreamInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
	struRecordPara.struStreamInfo.dwChannel = m_iChannel;
	strncpy((char*)struRecordPara.struStreamInfo.byID, m_strStreamID, STREAM_ID_LEN);

	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	if (NET_DVR_StartManualRecord(m_lLoginID, &struRecordPara))
	{
	   g_StringLanType(szLan,"手动开始录像成功","Success");
	   g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Start manual record");
	   AfxMessageBox(szLan);
	}
	else
	{        
	   g_StringLanType(szLan,"手动开始录像失败","Fail to set");
	   g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "fail to start manual record");
	   AfxMessageBox(szLan);
	}
	
}

void CDlgStreamManualRecordAndLock::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    char szLan[128] = {0};

// 	if ("" == m_strStreamID)
// 	{
// 		g_StringLanType(szLan,"流ID为空","Stream ID is NULL");
// 		AfxMessageBox(szLan);
// 		return;
// 	}
	
	NET_DVR_STREAM_INFO struStreamID;
	memset(&struStreamID, 0, sizeof(NET_DVR_STREAM_INFO));
	struStreamID.dwSize = sizeof(NET_DVR_STREAM_INFO);
	struStreamID.dwChannel = m_iChannel;
	strncpy((char*)struStreamID.byID, m_strStreamID, STREAM_ID_LEN);
	
	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	if (NET_DVR_StopManualRecord(m_lLoginID, &struStreamID))
	{
		g_StringLanType(szLan,"手动停止录像成功","Success");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "Stop manual record");
		AfxMessageBox(szLan);
	}
	else
	{        
		g_StringLanType(szLan,"手动停止录像失败","Fail to set");
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "fail to stop manual record");
		AfxMessageBox(szLan);
	}
}
