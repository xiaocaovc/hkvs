// DlgInquestEvent.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgInquestEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDlgInquestEvent *g_InquestEvent;

/////////////////////////////////////////////////////////////////////////////
// CDlgInquestEvent dialog


CDlgInquestEvent::CDlgInquestEvent(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInquestEvent::IDD, pParent)
	, m_bSearching(FALSE)
	, m_hFileThread(NULL)
	, m_lFileHandle(-1)
	, m_lLoginID(-1)
	, m_iDeviceIndex(-1)
	, m_byRoomIndex(-1)
	, m_bResuming(FALSE)
	, m_lResumeHandle(-1)
	, m_hResumeThread(NULL)
{
	//{{AFX_DATA_INIT(CDlgInquestEvent)
	m_DateStart = 0;
	m_DateStop = 0;
	m_TimeStart = 0;
	m_TimeStop = 0;
	//}}AFX_DATA_INIT
	memset(&m_struEventParam, 0, sizeof(m_struEventParam));
}


void CDlgInquestEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInquestEvent)
	DDX_Control(pDX, IDC_LIST_EVENT_FILE, m_listEvent);
	DDX_Control(pDX, IDC_COMBO_MINOR_EVENT, m_comboMinorEvent);
	DDX_Control(pDX, IDC_COMBO_MAJOR_EVENT, m_comboMajorEvent);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_DATE_START, m_DateStart);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_DATE_STOP, m_DateStop);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_TIME_START, m_TimeStart);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_TIME_STOP, m_TimeStop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInquestEvent, CDialog)
	//{{AFX_MSG_MAP(CDlgInquestEvent)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_RESUME, OnButtonResume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInquestEvent message handlers

BOOL CDlgInquestEvent::OnInitDialog() 
{
	CDialog::OnInitDialog();

	g_InquestEvent = this;

	m_comboMajorEvent.SetCurSel(0);
	m_comboMinorEvent.SetCurSel(0);
	
	char szLan[128] = {0};
	g_StringLanType(szLan, "主类型", "Major Type");
	m_listEvent.InsertColumn(0, szLan,LVCFMT_LEFT,80,-1);
	g_StringLanType(szLan, "次类型", "Minor Type");
	m_listEvent.InsertColumn(1, szLan,LVCFMT_LEFT,80,-1);
	g_StringLanType(szLan, "开始时间", "Start Time");
	m_listEvent.InsertColumn(2, szLan, LVCFMT_LEFT, 120, -1);	
	g_StringLanType(szLan, "结束时间", "Stop Time");
	m_listEvent.InsertColumn(3, szLan, LVCFMT_LEFT, 120, -1);
	g_StringLanType(szLan, "审讯室", "Room No");
	m_listEvent.InsertColumn(4, szLan, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLan, "刻录机", "Drive No");
	m_listEvent.InsertColumn(5, szLan, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLan, "片段编号", "Segment No");
	m_listEvent.InsertColumn(6, szLan, LVCFMT_LEFT, 60, -1);
	g_StringLanType(szLan, "片段大小", "Segment Size");
	m_listEvent.InsertColumn(7, szLan, LVCFMT_LEFT, 80, -1);
	g_StringLanType(szLan, "片段状态", "Segment State");
	m_listEvent.InsertColumn(8, szLan, LVCFMT_LEFT, 60, -1);

	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),0,0,0);
	CTime timeStop(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),23,59,59);
	m_DateStart = timeStart;
	m_TimeStart = timeStart;
	m_DateStop = timeStop;
	m_TimeStop = timeStop;
	
	CString csTemp;
	csTemp.Format("%d", m_byRoomIndex);
	GetDlgItem(IDC_STATIC_ROOM_INDEX)->SetWindowText(csTemp);

	UpdateData(FALSE);

	return TRUE;
}

void CDlgInquestEvent::OnCancelMode() 
{
	CDialog::OnCancelMode();	
}

/*********************************************************
Function:	GetEventThread
Desc:		
Input:		pParam, point to event dialog
Output:	
Return:	
**********************************************************/
UINT GetInquestEventThread(LPVOID pParam)
{
	LONG lRet = -1;
	NET_DVR_SEARCH_EVENT_RET struEventInfo = {0};
	char szLan[1024] = {0};
	int iFileNum = 0;

	while (1)
	{
		memset(&struEventInfo, 0, sizeof(NET_DVR_SEARCH_EVENT_RET));
		lRet = NET_DVR_FindNextEvent(g_InquestEvent->m_lFileHandle, &struEventInfo);
		if (lRet == NET_DVR_FILE_SUCCESS)
		{

			g_InquestEvent->m_listEvent.InsertItem(iFileNum, "", 0);	
			//major type
			if(struEventInfo.wMajorType = EVENT_INQUEST)
			{
				g_StringLanType(szLan, "审讯", "Inquest");
			}
			else
			{
				g_StringLanType(szLan, "未知", "UnKnow");
			}
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 0, szLan);

			//minor type
			switch (struEventInfo.wMinorType)
			{
			case INQUEST_START_INFO:
				g_StringLanType(szLan, "审讯开始", "Inquest Start");
				break;
			case INQUEST_STOP_INFO:
				g_StringLanType(szLan, "审讯停止", "Inquest Stop");
				break;
			case INQUEST_TAG_INFO:
				g_StringLanType(szLan, "重点标记", "Tag Info");
				break;
			case INQUEST_SEGMENT_INFO:
				g_StringLanType(szLan, "审讯片段", "Inquest Segment");
				break;
			default:
				g_StringLanType(szLan, "未知", "UnKnow");
				break;
			}
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 1, szLan);

			//Start time
			sprintf(szLan, "%04d%02d%02d%02d%02d%02d",struEventInfo.struStartTime.dwYear, \
				struEventInfo.struStartTime.dwMonth, struEventInfo.struStartTime.dwDay, \
				struEventInfo.struStartTime.dwHour, struEventInfo.struStartTime.dwMinute,\
				struEventInfo.struStartTime.dwSecond);
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 2, szLan);

			//Stop time
			sprintf(szLan, "%04d%02d%02d%02d%02d%02d", struEventInfo.struEndTime.dwYear, struEventInfo.struEndTime.dwMonth,\
				struEventInfo.struEndTime.dwDay, struEventInfo.struEndTime.dwHour, \
				struEventInfo.struEndTime.dwMinute, struEventInfo.struEndTime.dwSecond);
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 3, szLan);			

			//Inquest room number
			sprintf(szLan, "%d", struEventInfo.uSeniorRet.struInquestRet.byRoomIndex);
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 4, szLan);

			//Drive number
			sprintf(szLan, "%d", struEventInfo.uSeniorRet.struInquestRet.byDriveIndex);
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 5, szLan);

			//Segment No
			sprintf(szLan, "%d", struEventInfo.uSeniorRet.struInquestRet.dwSegmentNo);
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 6, szLan);	
			
			//Segment Size
			sprintf(szLan, "%d", struEventInfo.uSeniorRet.struInquestRet.wSegmetSize);
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 7, szLan);
			
			//Segment State
			switch (struEventInfo.uSeniorRet.struInquestRet.wSegmentState)
			{
			case 0:
				g_StringLanType(szLan, "刻录正常", "normal，1");
				break;
			case 1:
				g_StringLanType(szLan, "刻录异常", "exception，2");
				break;
			case 2:
				g_StringLanType(szLan, "不刻录审讯", "inquest w/o write");
				break;
			default:
				g_StringLanType(szLan, "未知", "UnKnow");
				break;
			}
			g_InquestEvent->m_listEvent.SetItemText(iFileNum, 8, szLan);
			
			//number ++
			iFileNum ++;
		}
		else
		{
			if (lRet == NET_DVR_ISFINDING)
			{
				Sleep(5);
				continue;
			}
			if ((lRet == NET_DVR_NOMOREFILE) || (lRet == NET_DVR_FILE_NOFIND))
			{
				g_StringLanType(szLan, "查找", "Search");
				g_InquestEvent->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
				g_InquestEvent->m_bSearching = FALSE;
				(g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE))->ShowWindow(SW_HIDE);
				if (iFileNum > 0)
				{
					g_pMainDlg->AddLog(g_InquestEvent->m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_FindNextEvent file num[%d]", iFileNum);
					g_StringLanType(szLan, "获取事件列表成功, 所有事件已列出", "Finish to get event list, and event have all been listed");
					AfxMessageBox(szLan);
				}
				else
				{
					g_StringLanType(szLan, "搜索结束, 没有搜到事件", "Finish to get file list, and There is no event file");
					AfxMessageBox(szLan);
				}
				
				break;
			}
			else
			{
				g_InquestEvent->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText("Search");
				g_InquestEvent->m_bSearching = FALSE;
				(g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE))->ShowWindow(SW_HIDE);
				g_StringLanType(szLan, "由于服务器忙或网络故障, 搜索异常结束", "Since the server is busy, or network failure, abnormal termination of access file list");
				AfxMessageBox(szLan);

				break;
			}
		}
	}
	NET_DVR_FindClose(g_InquestEvent->m_lFileHandle);
 	CloseHandle(g_InquestEvent->m_hFileThread);
 	g_InquestEvent->m_hFileThread = NULL;	
	return 0;
}

void CDlgInquestEvent::OnBtnSearch() 
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	if (!m_bSearching)
	{
		m_struEventParam.wMajorType = EVENT_INQUEST;
		m_struEventParam.uSeniorParam.struInquestParam.byRoomIndex = m_byRoomIndex;
		m_struEventParam.wMinorType = m_comboMinorEvent.GetCurSel() + INQUEST_START_INFO;
		m_struEventParam.struStartTime.dwYear = m_DateStart.GetYear();
		m_struEventParam.struStartTime.dwMonth = m_DateStart.GetMonth();
		m_struEventParam.struStartTime.dwDay = m_DateStart.GetDay();
		m_struEventParam.struStartTime.dwHour = m_TimeStart.GetHour();
		m_struEventParam.struStartTime.dwMinute = m_TimeStart.GetMinute();
		m_struEventParam.struStartTime.dwSecond = m_TimeStart.GetSecond();
		m_struEventParam.struEndTime.dwYear = m_DateStop.GetYear();
		m_struEventParam.struEndTime.dwMonth = m_DateStop.GetMonth();
		m_struEventParam.struEndTime.dwDay = m_DateStop.GetDay();
		m_struEventParam.struEndTime.dwHour = m_TimeStop.GetHour();
		m_struEventParam.struEndTime.dwMinute = m_TimeStop.GetMinute();
		m_struEventParam.struEndTime.dwSecond = m_TimeStop.GetSecond();
		
		m_lFileHandle = NET_DVR_FindFileByEvent(m_lLoginID, &m_struEventParam);
		
		if (m_lFileHandle < 0)
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_FindFileByEvent");
			g_StringLanType(szLan, "获取文件列表失败!", "Fail to get file list");
			AfxMessageBox(szLan);
			return;
		}

		m_listEvent.DeleteAllItems();
		DWORD dwThreadId;
		if (m_hFileThread == NULL)
		{
			m_hFileThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetInquestEventThread),this,0,&dwThreadId);	
		}
		if (m_hFileThread == NULL)
		{
			g_StringLanType(szLan, "打开查找线程失败!", "Fail to open searching thread!");
			AfxMessageBox(szLan);
			return;
		}
		g_StringLanType(szLan, "停止", "Stop");
		GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
		m_bSearching = TRUE;
		GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE)->ShowWindow(SW_SHOW);
	}
	else
	{
		if (m_hFileThread)
		{
			TerminateThread(m_hFileThread, 0);
		}
		CloseHandle(m_hFileThread);
		m_hFileThread = NULL;
		NET_DVR_FindClose(m_lFileHandle);
		g_StringLanType(szLan, "查找", "Search");
		GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
		m_bSearching = FALSE;
		GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE)->ShowWindow(SW_HIDE);
	}
}

void CDlgInquestEvent::OnButtonExit() 
{
	if (m_hFileThread)
	{
		TerminateThread(m_hFileThread, 0);
		CloseHandle(m_hFileThread);
		m_hFileThread = NULL;
		NET_DVR_FindClose(m_lFileHandle);
	}

	CDialog::OnCancel();
}

DWORD  ResumeEventThread(LPVOID pParam)
{
    DWORD dwState = 0;
    char szLan[256] = {0};
    while (1)
    {
        NET_DVR_InquestGetResumeProgress(g_InquestEvent->m_lResumeHandle, &dwState);
		if (dwState == 100)
        {
            g_StringLanType(szLan, "回复时间成功", "Resume event succeed");
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(szLan);
            NET_DVR_InquestStopResume(g_InquestEvent->m_lResumeHandle);
            g_StringLanType(szLan, "恢复事件", "Resume Event");
            g_InquestEvent->GetDlgItem(IDC_BUTTON_RESUME)->SetWindowText(szLan);
            g_InquestEvent->m_bResuming = FALSE;
			Sleep(1000);
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->ShowWindow(SW_HIDE);
            break;
        }
        else if (dwState == 400)
        {
            g_StringLanType(szLan, "恢复事件过程出现异常", "Resume event exception");
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(szLan);
            NET_DVR_InquestStopResume(g_InquestEvent->m_lResumeHandle);
            g_StringLanType(szLan, "恢复事件", "Resume Event");
            g_InquestEvent->GetDlgItem(IDC_BUTTON_RESUME)->SetWindowText(szLan);
            g_InquestEvent->m_bResuming = FALSE;
			Sleep(1000);
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->ShowWindow(SW_HIDE);
            break;
        }
        else if (dwState == 500)
        {
            g_StringLanType(szLan, "恢复事件失败", "Failed to resume event");
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(szLan);
            NET_DVR_InquestStopResume(g_InquestEvent->m_lResumeHandle);
            g_StringLanType(szLan, "恢复事件", "Resume Event");
            g_InquestEvent->GetDlgItem(IDC_BUTTON_RESUME)->SetWindowText(szLan);
            g_InquestEvent->m_bResuming = FALSE;
			Sleep(1000);
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->ShowWindow(SW_HIDE);
            break;
        }
		//进度值
        else if (dwState >= 0 && dwState < 100)
        { 
			char sTemp[24] = {0};
			g_StringLanType(szLan, "正在恢复事件", "Resuming");
            sprintf(sTemp, "%s[%d]", szLan, dwState);

            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(sTemp);
        }
		else if(dwState ==  BACKUP_CHANGE_DEVICE)
		{
			g_StringLanType(szLan, "设备已满, 请更换设备继续恢复", "Device of resume is full, change another device and continue resuming");
			g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(szLan);
			NET_DVR_StopBackup(g_InquestEvent->m_lResumeHandle);
            g_StringLanType(szLan, "恢复事件", "Resume Event");
			g_InquestEvent->GetDlgItem(IDC_BUTTON_RESUME)->SetWindowText(szLan);
			g_InquestEvent->m_bResuming = FALSE;
			Sleep(1000);
			g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->ShowWindow(SW_HIDE);
			break;
		}
		//中间过程
		else if(dwState == BACKUP_SEARCH_DEVICE)
		{
            g_StringLanType(szLan, "正在搜索备份设备", "searching backup device");
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(szLan);
		}
		else if(dwState == BACKUP_SEARCH_FILE)
		{
            g_StringLanType(szLan, "正在搜索录像文件", "searching record files");
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(szLan);
		}
		//错误值
		else if(dwState >= BACKUP_TIME_SEG_NO_FILE)
		{
            char szLanCn[128] = {0};
            char szLanEn[128] = {0};
            sprintf(szLanCn, "恢复事件失败, 错误值[%d]", dwState);
            sprintf(szLanEn, "Backup failed, ErrorCode[%d]", dwState);
			g_StringLanType(szLan, szLanCn, szLanEn);
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->SetWindowText(szLan);

            NET_DVR_InquestStopResume(g_InquestEvent->m_lResumeHandle);
            g_StringLanType(szLan, "恢复事件", "Resume Event");
            g_InquestEvent->GetDlgItem(IDC_BUTTON_RESUME)->SetWindowText(szLan);
            g_InquestEvent->m_bResuming = FALSE;
			Sleep(2000);
            g_InquestEvent->GetDlgItem(IDC_STATIC_EVENT_RESUME)->ShowWindow(SW_HIDE);
            break;
		}
		
        Sleep(1000);
    }
    CloseHandle(g_InquestEvent->m_hResumeThread);
    g_InquestEvent->m_hResumeThread = NULL;

    return 0;
}

void CDlgInquestEvent::OnButtonResume() 
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	NET_DVR_INQUEST_RESUME_EVENT strResumeEvent = {0};

	if (!m_bResuming)
	{
		POSITION pos = m_listEvent.GetFirstSelectedItemPosition();
		if (pos == NULL)
		{
			g_StringLanType(szLan, "请选择要恢复的事件", "Please select the event for resume");
			AfxMessageBox(szLan);
			return;
		}
			
		int iItemCount = 0;
		int iIndex = 0;
		CString csTemp = _T("");
		DWORD dwTemp = 0;

		while (pos != NULL && iItemCount < MAX_RESUME_SEGMENT)
		{
			iIndex = m_listEvent.GetNextSelectedItem(pos);

			csTemp = m_listEvent.GetItemText(iIndex, 2);
			sscanf(csTemp, "%04d%02d%02d%02d%02d%02d", &(strResumeEvent.struResumeSegment[iItemCount].struStartTime.dwYear),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStartTime.dwMonth),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStartTime.dwDay),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStartTime.dwHour),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStartTime.dwMinute),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStartTime.dwSecond));

			csTemp = m_listEvent.GetItemText(iIndex, 3);
			sscanf(csTemp, "%04d%02d%02d%02d%02d%02d", &(strResumeEvent.struResumeSegment[iItemCount].struStopTime.dwYear),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStopTime.dwMonth),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStopTime.dwDay),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStopTime.dwHour),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStopTime.dwMinute),\
			&(strResumeEvent.struResumeSegment[iItemCount].struStopTime.dwSecond));

			csTemp = m_listEvent.GetItemText(iIndex, 4);
			sscanf(csTemp, "%d", &dwTemp);
			strResumeEvent.struResumeSegment[iItemCount].byRoomIndex = (BYTE)dwTemp;

			csTemp = m_listEvent.GetItemText(iIndex, 5);
			sscanf(csTemp, "%d", &dwTemp);
			strResumeEvent.struResumeSegment[iItemCount].byDriveIndex = (BYTE)dwTemp;
			
			csTemp = m_listEvent.GetItemText(iIndex, 6);
			sscanf(csTemp, "%d", &(strResumeEvent.struResumeSegment[iItemCount].dwSegmentNo));

			csTemp = m_listEvent.GetItemText(iIndex, 7);
			sscanf(csTemp, "%d", &dwTemp);
			strResumeEvent.struResumeSegment[iItemCount].wSegmetSize = (WORD)dwTemp;
			
			++ iItemCount;
		}

		strResumeEvent.dwResumeNum = iItemCount;

		m_lResumeHandle = NET_DVR_InquestResumeEvent(m_lLoginID, &strResumeEvent);
        if (m_lResumeHandle == -1)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_InquestResumeEvent");
            return;
        }
        else
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_InquestResumeEvent");
        }     
        
        DWORD dwThreadId = 0;
        if (m_hResumeThread == NULL)
        {
            m_hResumeThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(ResumeEventThread),this,0,&dwThreadId);		
        }
        if (m_hResumeThread  == NULL)
        {
            char szLan[256] = {0};
            g_StringLanType(szLan, "打开恢复事件线程失败!", "Fail to open resume event thread!");
            AfxMessageBox(szLan);
            return;
        }
        g_StringLanType(szLan, "停止", "Stop");
        GetDlgItem(IDC_BUTTON_RESUME)->SetWindowText(szLan);
        m_bResuming = TRUE;
        GetDlgItem(IDC_STATIC_EVENT_RESUME)->ShowWindow(SW_SHOW);
    }
    else
    {
        if (m_hResumeThread)
        {
            TerminateThread(m_hResumeThread, 0);
        }
		
        CloseHandle(m_hResumeThread);
        m_hResumeThread = NULL;
        NET_DVR_InquestStopResume(m_lResumeHandle);
        g_StringLanType(szLan, "恢复事件", "Resume Event");
        GetDlgItem(IDC_BUTTON_RESUME)->SetWindowText(szLan);
        m_bResuming = FALSE;
        GetDlgItem(IDC_STATIC_EVENT_RESUME)->ShowWindow(SW_HIDE);
    }
}
