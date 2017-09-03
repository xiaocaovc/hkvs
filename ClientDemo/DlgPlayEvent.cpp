/**********************************************************
FileName:    DlgPlayEvent.cpp
Description: remote event play back      
Date:        2009/05/17
Note: 		  
Modification History:      
<version> <time>         <desc>
<1.0    > <2009/05/17>       <created>
***********************************************************/


#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgPlayEvent.h"


// CDlgPlayEvent dialog
//BOOL g_bRmtCapPic = FALSE;
CDlgPlayEvent* g_pDlgPlayEvent;

char szEventMap[][32] = 
{
	"TRAVERSE_PLANE", 
		"ENTER_AREA",				
		"EXIT_AREA",				
		"INTRUSION",			
		"LOITER",				 
		"LEFT_TAKE",	
		"PARKING",			
		"RUN",					
		"High DENSITY",			
		"STICK UP",
		"INSTALL SCANNER",
		"OPERATE OVERTIME",
		"FACE DETECT",
		"LEFT",
		"TAKE",
		"LEAVE_POSITION",
		"TRAIL",
		"NULL",
		"NULL",
		"FALL_DOWN",
		"OBJECT_PASTE",
		"FACE_CAPTURE",
		"MULTI_FACES",
		"AUDIO_ABNORMAL"
};
//#define CALLBACK_PLAY
/*********************************************************
Function:	PlayDataCallBack
Description:		
Input:		lPlayHandle, playback handle;dwDataType,data type,data head or stream;pBuffer,data buffer;dwBufSize,data size;dwUser,user data;
Output:	
Return:	
**********************************************************/
extern void CALLBACK PlayDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,DWORD dwUser);


/*********************************************************
Function:	GetEventThread
Desc:		
Input:		pParam, point to event dialog
Output:	
Return:	
**********************************************************/
UINT GetEventThread(LPVOID pParam)
{
	LONG lRet = -1;
	NET_DVR_SEARCH_EVENT_RET struEventInfo;
	char szTmp[36] = {0};
	char szTmp1[36] = {0};
	char szDetail[256] = {0};
	char szLan[1024] = {0};
	int iSize = 0;
    int i = 0;
    int iCount = 0;
	CString strFileIndex;
	
	while (g_pDlgPlayEvent->m_IsQuit)
	{
		memset(&struEventInfo, 0, sizeof(NET_DVR_SEARCH_EVENT_RET));
		lRet = NET_DVR_FindNextEvent(g_pDlgPlayEvent->m_lFileHandle, &struEventInfo);
		if (lRet == NET_DVR_FILE_SUCCESS)
		{		
			switch(struEventInfo.wMajorType)
			{
			case EVENT_MOT_DET:
				sprintf(szTmp, "Motdet");
				sprintf(szTmp1, "%d",struEventInfo.uSeniorRet.struMotionRet.dwMotDetNo);
				break;
			case EVENT_ALARM_IN:
				sprintf(szTmp, "AlarmIn ");
				sprintf(szTmp1, "%d",struEventInfo.uSeniorRet.struAlarmRet.dwAlarmInNo);
				break;
			case EVENT_VCA_BEHAVIOR:
                if (struEventInfo.wMinorType == 20)
                {
                    strncpy(szEventMap[struEventInfo.wMinorType], "Object Paste", sizeof(szEventMap[struEventInfo.wMinorType]));
                }
				sprintf(szTmp, "Vca Behavior %s", szEventMap[struEventInfo.wMinorType]);
				sprintf(szTmp1, "ch%d rule%d %s",struEventInfo.uSeniorRet.struVcaRet.dwChanNo, 
					struEventInfo.uSeniorRet.struVcaRet.byRuleID,\
					(char *)(struEventInfo.uSeniorRet.struVcaRet.byRuleName));
				break;
			case EVENT_STREAM_INFO:
				sprintf(szTmp, "Stream ID");
				sprintf(szTmp1, "type:%d size:%dMB lock flag:%d ", struEventInfo.uSeniorRet.struStreamIDRet.dwRecordType, 
					struEventInfo.uSeniorRet.struStreamIDRet.dwRecordLength/1048576, struEventInfo.uSeniorRet.struStreamIDRet.byLockFlag);
				strFileIndex.Format("%d", struEventInfo.uSeniorRet.struStreamIDRet.dwFileIndex);
				break;
			case EVENT_VCA_DETECTION:
				sprintf(szTmp, "Vca Detect");
				break;
			default:
				sprintf(szTmp, "Unknown Event%d", struEventInfo.wMajorType);
				break;
			}			
			g_pDlgPlayEvent->m_listRemoteFile.InsertItem(g_pDlgPlayEvent->m_iFileNum, szTmp,0);			
			g_pDlgPlayEvent->m_listRemoteFile.SetItemText(g_pDlgPlayEvent->m_iFileNum, 3, szTmp1);	
			sprintf(szTmp, "%04d%02d%02d%02d%02d%02d",struEventInfo.struStartTime.dwYear, \
				struEventInfo.struStartTime.dwMonth, struEventInfo.struStartTime.dwDay, \
				struEventInfo.struStartTime.dwHour, struEventInfo.struStartTime.dwMinute,\
				struEventInfo.struStartTime.dwSecond);
			g_pDlgPlayEvent->m_listRemoteFile.SetItemText(g_pDlgPlayEvent->m_iFileNum, 1, szTmp);
			sprintf(szTmp, "%04d%02d%02d%02d%02d%02d", struEventInfo.struEndTime.dwYear, struEventInfo.struEndTime.dwMonth,\
				struEventInfo.struEndTime.dwDay, struEventInfo.struEndTime.dwHour, \
				struEventInfo.struEndTime.dwMinute, struEventInfo.struEndTime.dwSecond);
			g_pDlgPlayEvent->m_listRemoteFile.SetItemText(g_pDlgPlayEvent->m_iFileNum, 2, szTmp);	
			g_pDlgPlayEvent->m_listRemoteFile.SetItemText(g_pDlgPlayEvent->m_iFileNum, 5, strFileIndex);
			memset(szDetail, 0, 256);
			iSize = 0;
			for (i=0; i<MAX_CHANNUM_V30; i++)
			{
				if (struEventInfo.byChan[i])
				{
					iSize++;
					sprintf(szDetail+iSize*3, "%2d,", i);
				}
			}
			g_pDlgPlayEvent->m_listRemoteFile.SetItemText(g_pDlgPlayEvent->m_iFileNum, 4, szDetail);
			g_pDlgPlayEvent->m_iFileNum++;
			TRACE("%d\n", g_pDlgPlayEvent->m_iFileNum);
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
				g_pDlgPlayEvent->GetDlgItem(IDC_BTN_EVENT_SEARCH_LIST)->SetWindowText(szLan);
				g_pDlgPlayEvent->m_bSearching = FALSE;
				(g_pDlgPlayEvent->GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE))->ShowWindow(SW_HIDE);
				if (g_pDlgPlayEvent->m_iFileNum > 0)
				{
					g_pMainDlg->AddLog(g_pDlgPlayEvent->m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_FindNextEvent file num[%d]", g_pDlgPlayEvent->m_iFileNum);
					g_StringLanType(szLan, "获取文件列表成功, 所有文件已列出", "Finish to get file list, and documents have all been listed");
					AfxMessageBox(szLan);
				}
				else
				{
					g_StringLanType(szLan, "搜索结束, 没有搜到文件", "Finish to get file list, and There is no record file");
					AfxMessageBox(szLan);
				}
				
				g_pDlgPlayEvent->m_iFileNum = 0;
				if (g_pDlgPlayEvent->m_lPlayHandle == -1)
				{
					g_pDlgPlayEvent->SetStopState();
					g_pDlgPlayEvent->SetStopStateIDTime();
					g_pDlgPlayEvent->GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->EnableWindow(FALSE);
					g_pDlgPlayEvent->GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->EnableWindow(FALSE);
				}
				break;
			}
			else
			{
				g_pDlgPlayEvent->GetDlgItem(IDC_BTN_EVENT_SEARCH_LIST)->SetWindowText("Search");
				g_pDlgPlayEvent->m_bSearching = FALSE;
				(g_pDlgPlayEvent->GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE))->ShowWindow(SW_HIDE);
				g_StringLanType(szLan, "由于服务器忙或网络故障, 搜索异常结束", "Since the server is busy, or network failure, abnormal termination of access file list");
				AfxMessageBox(szLan);
				g_pDlgPlayEvent->m_iFileNum = 0;
				if (g_pDlgPlayEvent->m_lPlayHandle == -1)
				{
					g_pDlgPlayEvent->SetStopState();
					g_pDlgPlayEvent->SetStopStateIDTime();
					g_pDlgPlayEvent->GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->EnableWindow(FALSE);
					g_pDlgPlayEvent->GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->EnableWindow(FALSE);
				}
				break;
			}
		}
	}
	NET_DVR_FindClose(g_pDlgPlayEvent->m_lFileHandle);
	//	TerminateThread(g_pDlgPlayEvent->m_hFileThread, 0);
	CloseHandle(g_pDlgPlayEvent->m_hFileThread);
	g_pDlgPlayEvent->m_hFileThread = NULL;	
	return 0;
}

/*********************************************************
Function:	CDlgPlayEvent
Desc:		Constructor
Input:	
Output:	
Return:	
**********************************************************/
IMPLEMENT_DYNAMIC(CDlgPlayEvent, CDialog)
CDlgPlayEvent::CDlgPlayEvent(CWnd* pParent /*=NULL*/)
: CDialog(CDlgPlayEvent::IDD, pParent)
, m_ctDateStart(0)
, m_ctTimeStart(0)
, m_ctDateStop(0)
, m_ctTimeStop(0)
, m_iDeviceIndex(-1)
, m_lLoginID(-1)
, m_iChanIndex(-1)
, m_lPlayHandle(-1)
, m_bDownloading(FALSE)
, m_bSearching(FALSE)
, m_bFullScreen(FALSE)
, m_hFileThread(NULL)
, m_lDeviceType(0)
, m_bGetMaxTime(FALSE)
, m_hPareTabWnd(NULL)
, m_hPareDlgWnd(NULL)
, m_lDownloadHandle(-1)
, m_bPause(FALSE)
, m_bSound(FALSE)
, m_hPlayWnd(NULL)
, m_iFileNum(0)
, m_iChanShowNum(0)
, m_iMajorType(0)
, m_lFileHandle(-1)
, m_nFileTime(0)
, m_nTotalFrames(0)
, m_nTotalSecond(0)
, m_nTotalMinute(0)
, m_nTotalHour(0)
, m_lStartChan(0)
, m_bSaveFile(FALSE)
, m_bSetFrame(FALSE)
, m_iChanNum(0)
, m_iItemSel(-1)
, m_lPort(-1)
, m_iTime(5)
, m_dwBackupVolume(0)
, m_iRelatedChannel(0)
, m_IsQuit(TRUE)
{
	memset(m_szFileName, 0, 200);
	memset(m_szDownLoadFileName, 0, 256);
    memset(&m_ctDateStart, 0, sizeof(m_ctDateStart));
    memset(&m_ctDateStop, 0, sizeof(m_ctDateStop));
	m_hPlayEnableIcon =			AfxGetApp()->LoadIcon(IDI_PLAY_ENABLE);
	m_hPlayDisableIcon =		AfxGetApp()->LoadIcon(IDI_PLAY_DISABLE);
	m_hPauseEnableIcon =		AfxGetApp()->LoadIcon(IDI_PAUSE_ENABLE);
	m_hPauseDisableIcon =		AfxGetApp()->LoadIcon(IDI_PAUSE_DISABLE);
	m_hStopEnableIcon =			AfxGetApp()->LoadIcon(IDI_STOP_ENABLE);
	m_hStopDisableIcon =		AfxGetApp()->LoadIcon(IDI_STOP_DISABLE);
	m_hSoundStartIcon=			AfxGetApp()->LoadIcon(IDI_SOUND_ENABLE);
	m_hSoundStopIcon=			AfxGetApp()->LoadIcon(IDI_SOUND_DISABLE);
	m_hGotoHeadDisableIcon =	AfxGetApp()->LoadIcon(IDI_GOTOSTART_DISABLE);
	m_hGotoTailDisableIcon =	AfxGetApp()->LoadIcon(IDI_GOTOEND_DISABLE);
	m_hFastDisableIcon =		AfxGetApp()->LoadIcon(IDI_FASTFORWARD_DISABLE);
	m_hSlowDisableIcon=			AfxGetApp()->LoadIcon(IDI_FASTBACKWARD_DISABLE);
	m_hGotoHeadEnableIcon =		AfxGetApp()->LoadIcon(IDI_GOTOSTART_ENABLE);
	m_hGotoTailEnableIcon =		AfxGetApp()->LoadIcon(IDI_GOTOEND_ENABLE);
	m_hFastEnableIcon =			AfxGetApp()->LoadIcon(IDI_FASTFORWARD_ENABLE);
	m_hSlowEnableIcon =			AfxGetApp()->LoadIcon(IDI_FASTBACKWARD_ENABLE);
	m_hStepEnableIcon=			AfxGetApp()->LoadIcon(IDI_STEP_ENABLE);
	m_hStepDisableIcon =		AfxGetApp()->LoadIcon(IDI_STEP_DISABLE);
	m_hStepBackEnableIcon =		AfxGetApp()->LoadIcon(IDI_STEPBACK_ENABLE);
	m_hStepBackDisableIcon =	AfxGetApp()->LoadIcon(IDI_STEPBACK_DISABLE);
	m_hCaptureIcon	=			AfxGetApp()->LoadIcon(IDI_PIC);	
}

/*********************************************************
Function:	~CDlgPlayEvent
Desc:		destructor
Input:	
Output:	
Return:	
**********************************************************/
CDlgPlayEvent::~CDlgPlayEvent()
{
}

/*********************************************************
Function:	DoDataExchange
Desc:		the map between control and variable
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPlayEvent)
	DDX_Control(pDX, IDC_COMBO_RULEID, m_comboRuleID);
	DDX_Control(pDX, IDC_LIST_CHAN, m_listChan);
	DDX_Control(pDX, IDC_COMBO_MINOR_EVENT, m_comboMinorEvent);
	DDX_Control(pDX, IDC_COMBO_MAJOR_EVENT, m_comboMajorType);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_DATE_START, m_ctDateStart);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_TIME_START, m_ctTimeStart);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_DATE_STOP, m_ctDateStop);
	DDX_DateTimeCtrl(pDX, IDC_COMBO_EVENT_TIME_STOP, m_ctTimeStop);
	DDX_Control(pDX, IDC_LIST_EVENT_FILE, m_listRemoteFile);
	DDX_Control(pDX, IDC_STATIC_EVENT_FILE_WND, m_staticPlayWnd);
	DDX_Control(pDX, IDC_SLIDER_EVENT_FILE_VOLUME, m_sliderVolume);
	DDX_Text(pDX, IDC_EDIT_TIME, m_iTime);
	DDV_MinMaxInt(pDX, m_iTime, 0, 60);
	DDX_Text(pDX, IDC_EDIT_STREAM_ID, m_strStreamID);
	DDX_Text(pDX, IDC_EDIT_RELATED_CHANNEL, m_iRelatedChannel);
	DDX_Text(pDX, IDC_EDIT_BACKUP_VOLUME, m_dwBackupVolume);
	DDX_Check(pDX, IDC_CHECK_DRAW_FRAME, m_bDrawFrame);
	DDX_Check(pDX, IDC_CHECK_BY_VALUE, m_bByValue);
	//}}AFX_DATA_MAP
}


/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CDlgPlayEvent, CDialog)
//{{AFX_MSG_MAP(CDlgPlayEvent)	
ON_BN_CLICKED(IDC_BTN_EVENT_SEARCH_LIST, OnBnClickedBtnRemoteSearchList)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_EVENT_FILE, OnNMDblclkListRemoteFile)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_PLAY, OnBnClickedBtnRemoteFilePlay)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_STOP, OnBnClickedBtnRemoteFileStop)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_STOP_SAVE, OnBnClickedBtnRemoteFileStopSave)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_SLOW, OnBnClickedBtnRemoteFileSlow)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_SNATCH_PIC, OnBnClickedBtnRemoteFileSnatchPic)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_STEP, OnBnClickedBtnRemoteFileStep)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_SOUND, OnBnClickedBtnRemoteFileSound)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_FAST, OnBnClickedBtnRemoteFileFast)
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_SAVE, OnBnClickedBtnRemoteFileSave)
ON_WM_LBUTTONDBLCLK()
ON_WM_TIMER()
ON_NOTIFY(NM_CLICK, IDC_LIST_EVENT_FILE, OnClickListRemoteFile)
ON_WM_DESTROY()
ON_WM_CTLCOLOR()
ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_EVENT_FILE_VOLUME, OnReleasedcaptureSliderRemoteFileVolume)
ON_NOTIFY(NM_CLICK, IDC_LIST_CHAN, OnClickListChan)
ON_CBN_SELCHANGE(IDC_COMBO_MAJOR_EVENT, OnSelchangeComboMajorEvent)
ON_CBN_SELCHANGE(IDC_COMBO_MINOR_EVENT, OnSelchangeComboMinorEvent)
ON_WM_HSCROLL()
ON_WM_PAINT()
ON_WM_MOVE()
ON_WM_CANCELMODE()
ON_WM_CAPTURECHANGED()
ON_BN_CLICKED(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME, OnBtnEventFilePlayByIdAndTime)
//}}AFX_MSG_MAP

END_MESSAGE_MAP()


// CDlgPlayEvent message handlers
/*********************************************************
Function:	SetParentWnd
Desc:		transfer config parent dialog handle
Input:	hhWnd,point of handle array, parent point number
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::SetParentWnd(HWND *hhWnd, int iParentNum)
{
	VERIFY(iParentNum==2);
	m_hPareDlgWnd=hhWnd[0];
	m_hPareTabWnd=hhWnd[1];
}

/*********************************************************
Function:	OnInitDialog
Desc:		Initialize the dialog
Input:	
Output:	
Return:	
**********************************************************/
BOOL CDlgPlayEvent::OnInitDialog()
{
	CDialog::OnInitDialog();
	
    m_listRemoteFile.SetExtendedStyle(m_listRemoteFile.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);
	InitUI();
	char szLan[128] = {0};
	g_StringLanType(szLan, "主类型", "Major Type");
	m_listRemoteFile.InsertColumn(0, szLan,LVCFMT_LEFT,120,-1);
	// 	g_StringLanType(szLan, "次类型", "Minor Type");
	// 	m_listRemoteFile.InsertColumn(1, szLan,LVCFMT_LEFT,50,-1);
	g_StringLanType(szLan, "开始时间", "Start time");
	m_listRemoteFile.InsertColumn(1, szLan, LVCFMT_LEFT, 120, -1);
	
	g_StringLanType(szLan, "结束时间", "Stop Time");
	m_listRemoteFile.InsertColumn(2, szLan, LVCFMT_LEFT, 120, -1);
	g_StringLanType(szLan, "高级参数", "Senior Param");
	m_listRemoteFile.InsertColumn(3, szLan,LVCFMT_LEFT,50,-1);
	g_StringLanType(szLan, "触发通道", "Invoke Chan");
	m_listRemoteFile.InsertColumn(4, szLan,LVCFMT_LEFT,50,-1);
	g_StringLanType(szLan, "文件索引", "File Index");
	m_listRemoteFile.InsertColumn(5, szLan, LVCFMT_LEFT, 50, -1);
	m_listChan.SetExtendedStyle(m_listChan.GetExtendedStyle()|LVS_EX_CHECKBOXES);
	CTime timeCur = CTime::GetCurrentTime();
	CTime timeStart(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),0,0,0);
	CTime timeStop(timeCur.GetYear(),timeCur.GetMonth(),timeCur.GetDay(),23,59,59);
	m_ctDateStart = timeStart;
	m_ctTimeStart = timeStart;
	m_ctDateStop = timeStop;
	m_ctTimeStop = timeStop;
	m_hPlayWnd = GetDlgItem(IDC_STATIC_EVENT_FILE_WND)->m_hWnd;
	m_sliderVolume.SetRange(0,100);
	m_sliderVolume.SetPos(50);
	
	GetDlgItem(IDC_STATIC_EVENT_FILE_WND)->GetWindowRect(m_rcWnd);
	ScreenToClient(m_rcWnd);
	g_pDlgPlayEvent=this;
	
	int index=0;
	m_comboRuleID.ResetContent();
	g_StringLanType(szLan, "全部", "All");
	m_comboRuleID.AddString(szLan);
	m_comboRuleID.SetItemData(index, 0xff);
	index++;
	
	m_comboRuleID.AddString("0");
	m_comboRuleID.SetItemData(index, 0);
	index++;
	
	m_comboRuleID.AddString("1");
	m_comboRuleID.SetItemData(index, 1);
	index++;
	
	m_comboRuleID.AddString("2");
	m_comboRuleID.SetItemData(index, 2);
	index++;
	
	m_comboRuleID.AddString("3");
	m_comboRuleID.SetItemData(index, 3);
	index++;
	
	m_comboRuleID.AddString("4");
	m_comboRuleID.SetItemData(index, 4);
	index++;
	
	m_comboRuleID.AddString("5");
	m_comboRuleID.SetItemData(index, 5);
	index++;
	
	m_comboRuleID.AddString("6");
	m_comboRuleID.SetItemData(index, 6);
	index++;
	
	m_comboRuleID.AddString("7");
	m_comboRuleID.SetItemData(index, 7);
	index++;
	//minor event, now for behavior analysis
	CheckInitParam();
	UpdateMinorEvent();
	m_comboMajorType.SetCurSel(0);
	m_comboMinorEvent.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;
}



/*********************************************************
Function:	CheckInitParam
Desc:		Verify current device's parameters, and initialize the parameter of dialog
Input:	
Output:	
Return:	TRUE,Verify successfully and initialize parameters; FALSE,verify failed and won't initialize parameters;
**********************************************************/
BOOL CDlgPlayEvent::CheckInitParam()
{
	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	if (iDeviceIndex == -1)
	{
		UpdateData(FALSE);
		return FALSE;
	}
	if ( m_iDeviceIndex == -1 || m_iDeviceIndex != iDeviceIndex)
	{
		m_iChanIndex = -1;
	}
	int iChanIndex = g_pMainDlg->GetCurChanIndex();
	if (iChanIndex == -1)
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "set the channel index = 0");
		iChanIndex = 0;
		//AfxMessageBox("please select a channel!");
		//UpdateData(FALSE);
		//return FALSE;
	}
	
	UpdateData(TRUE);
	char szLan[128] = {0};
	
	m_iDeviceIndex = iDeviceIndex;
	m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
	m_iChanNum = g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum;
	m_lDeviceType = g_struDeviceInfo[m_iDeviceIndex].iDeviceType;
	m_lStartChan = g_struDeviceInfo[m_iDeviceIndex].iStartChan;
	
	//reinitialize the parameter when switching channel
	if (m_iChanIndex != iChanIndex)
	{
		m_iChanIndex = iChanIndex;
		m_iChanShowNum = g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].iChannelNO;
		if (m_lPlayHandle>=0)
		{
			StopPlay();
		}
		
		m_bDownloading=FALSE;
		m_bSearching = FALSE;
		m_lPlayHandle = -1;
		m_bSound=TRUE;
		m_bPause=FALSE;
		m_bSetFrame = FALSE;
		m_bGetMaxTime = FALSE;
		m_bFullScreen = FALSE;
		m_bSaveFile = FALSE;
		//m_hPareDlgWnd = NULL;
		//m_hPareTabWnd = NULL;
		
		m_hFileThread = NULL;
		m_iFileNum = 0;
		m_lDownloadHandle = -1;
		m_nFileTime = 0;
		m_nTotalFrames = 0;
		m_nTotalSecond = 0;
		m_nTotalMinute = 0;
		m_nTotalHour = 0;
		m_listRemoteFile.DeleteAllItems();
		memset(&m_struEventParam, 0, sizeof(NET_DVR_SEARCH_EVENT_PARAM));
		for (int i = 0; i < MAX_CHANNUM_V30 - 1; i++)
		{
			m_struEventParam.uSeniorParam.struVCADetectByValue.dwChanNo[i] = 0xffffffff;
		}
		
	}
	if (m_lLoginID < 0)
	{
		UpdateData(FALSE);
		return FALSE;
	}
	
	int iIndex = 0;
	m_comboMajorType.ResetContent();
	
	g_StringLanType(szLan, "移动侦测", "Motion Detect");
	m_comboMajorType.AddString(szLan);
	m_comboMajorType.SetItemData(iIndex, 0);
	iIndex++;
	
	g_StringLanType(szLan, "报警输入", "Alarm In");
	m_comboMajorType.AddString(szLan);
	m_comboMajorType.SetItemData(iIndex, 1);
	iIndex++;
	
	g_StringLanType(szLan, "行为分析", "Behavior Analysis");
	m_comboMajorType.AddString(szLan);
	m_comboMajorType.SetItemData(iIndex, 2);
	iIndex++;
	
	g_StringLanType(szLan, "智能侦测", "VCA Detect");
	m_comboMajorType.AddString(szLan);
	m_comboMajorType.SetItemData(iIndex, 4);
	iIndex++;
	
	
	g_StringLanType(szLan, "通道关联", "Channel Related"); 
	m_comboMajorType.AddString(szLan);
	m_comboMajorType.SetItemData(iIndex, 100);
	iIndex++;
	
	m_iMajorType = 0;
	m_comboMajorType.SetCurSel(m_iMajorType);
	UpdateMinorEvent();
	UpdateChanStatus();
	
	UpdateData(FALSE); 
	return TRUE;
}

/*********************************************************
Function:	InitUI
Desc:		Initial control of dialog
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::InitUI()
{
	// Initial button
	CButton *pButton;
	
	m_rcScreen=CRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY);
	pButton->SetIcon(m_hPlayDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME);
	pButton->SetIcon(m_hPlayDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STOP);
	pButton->SetIcon(m_hStopDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_FAST);
	pButton->SetIcon(m_hFastDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SLOW);
	pButton->SetIcon(m_hSlowDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STEP);
	pButton->SetIcon(m_hStepDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SNATCH_PIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND);
	pButton->SetIcon(m_hSoundStopIcon);
	pButton->EnableWindow(FALSE);
}

/*********************************************************
Function:	SetStopState
Desc:		Change the control status when stop playback
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::SetStopState()
{
	CButton *pButton;
	
	m_sliderVolume.EnableWindow(FALSE);
	
	GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY);
	pButton->SetIcon(m_hPlayEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STOP);
	pButton->SetIcon(m_hStopDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_FAST);
	pButton->SetIcon(m_hFastDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SLOW);
	pButton->SetIcon(m_hSlowDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STEP);
	pButton->SetIcon(m_hStepDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SNATCH_PIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND);
	pButton->SetIcon(m_hSoundStopIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STEP);
	pButton->SetIcon(m_hStepDisableIcon);
	pButton->EnableWindow(FALSE);	
}

/*********************************************************
Function:	SetStopStateIDTime
Desc:		Change the control status when stop playback
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::SetStopStateIDTime()
{
	CButton *pButton;
	
	m_sliderVolume.EnableWindow(FALSE);
	
	GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME);
	pButton->SetIcon(m_hPlayEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STOP);
	pButton->SetIcon(m_hStopDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_FAST);
	pButton->SetIcon(m_hFastDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SLOW);
	pButton->SetIcon(m_hSlowDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STEP);
	pButton->SetIcon(m_hStepDisableIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SNATCH_PIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND);
	pButton->SetIcon(m_hSoundStopIcon);
	pButton->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STEP);
	pButton->SetIcon(m_hStepDisableIcon);
	pButton->EnableWindow(FALSE);	
}

/*********************************************************
Function:	ExitPlayBack
Desc:		Exit current diaglog and release resource
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::ExitPlayBack()
{
	if (m_hFileThread)
	{
		TerminateThread(m_hFileThread, 0);
		CloseHandle(m_hFileThread);
		m_hFileThread = NULL;
		NET_DVR_FindClose(m_lFileHandle);
	}	
	
	if (m_lPlayHandle!=-1)
	{
		NET_DVR_StopPlayBack(m_lPlayHandle);
		m_lPlayHandle = -1;
	}
	Sleep(100);
}
/*********************************************************
Function:	ConvertTime
Desc:		convert between time_t and NET_DVR_TIME
Input:	void
Output:	void
Return:	void
**********************************************************/
BOOL ConvertTime(time_t *lpInBuf, LPNET_DVR_TIME pOutBuf, BOOL bInToOut)
{
	if(pOutBuf == NULL || lpInBuf == NULL)
	{
		return FALSE;
	}
	
	if (bInToOut)
	{
		struct tm *newtime;
		newtime = localtime(lpInBuf);
		if (newtime == NULL)
		{
			return FALSE;
		}
		pOutBuf->dwYear = DWORD(newtime->tm_year+1900);
		pOutBuf->dwMonth = DWORD (newtime->tm_mon+1);
		pOutBuf->dwDay = DWORD(newtime->tm_mday);
		pOutBuf->dwHour = DWORD(newtime->tm_hour);
		pOutBuf->dwMinute = DWORD(newtime->tm_min);
		pOutBuf->dwSecond = DWORD(newtime->tm_sec);
	}
	else
	{
		struct tm newtime = {0};
		newtime.tm_year = pOutBuf->dwYear - 1900;
		newtime.tm_mon = pOutBuf->dwMonth - 1;
		newtime.tm_mday = pOutBuf->dwDay;
		newtime.tm_hour = pOutBuf->dwHour;
		newtime.tm_min = pOutBuf->dwMinute;
		newtime.tm_sec = pOutBuf->dwSecond;	
		//utc = local time + bias, local time = utc - bias;
		*lpInBuf = mktime(&newtime);
	}
	return TRUE;
}
/*********************************************************
Function:	PlayBack
Desc:		begin play back 
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::PlayBack()
{
	UpdateData(TRUE);
	
	int m_nVerifyret = -1;
	CString csFileName;
	int iFileSelPos = 0;
	HWND hPlayWnd=m_staticPlayWnd.GetSafeHwnd();
	POSITION  posItem = m_listRemoteFile.GetFirstSelectedItemPosition();
	NET_DVR_TIME struStartTime,struStopTime;
	char szLan[128] = {0};
	
	//	remoteplay_info.srcfilename=m_szFileName;
	if (m_lPlayHandle>=0)
	{
		if (NET_DVR_StopPlayBack(m_lPlayHandle))
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopPlayBack");
		}
		m_lPlayHandle = -1;
		Sleep(400);
	}
	if (m_lPlayHandle == -1)
	{
		m_iChanShowNum = g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].iChannelNO;
		time_t tmp;
		ConvertTime(&tmp, &m_struStartTime, FALSE);
		tmp -= m_iTime;
		ConvertTime(&tmp, &m_struStartTime, TRUE);
		ConvertTime(&tmp, &m_struStopTime, FALSE);
		tmp += m_iTime;
		ConvertTime(&tmp, &m_struStopTime, TRUE);
		memcpy(&struStartTime, &m_struStartTime, sizeof(NET_DVR_TIME));
		memcpy(&struStopTime, &m_struStopTime, sizeof(NET_DVR_TIME));	
		
		
		m_lPlayHandle = NET_DVR_PlayBackByTime(m_lLoginID, m_iChanShowNum, &struStartTime, &struStopTime, hPlayWnd);//NULL
		
		if (m_lPlayHandle == -1)
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PlayBackByTime ChanNum[%d]", m_iChanShowNum);
			g_StringLanType(szLan, "远程回放失败", "Fail to play back remote");
			AfxMessageBox(szLan);
			return;
		}
		int idx = NET_DVR_GetPlayBackPlayerIndex(m_lPlayHandle);
		
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PlayBackByTime ChanNum[%d]", m_iChanShowNum);
		NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTART, 0, NULL);
		
		if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTARTAUDIO, 0, NULL))
		{
			m_bSound=TRUE;
			NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYAUDIOVOLUME, (0xffff)/2, NULL);
			m_sliderVolume.SetPos(50);
			((CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND))->SetIcon(m_hSoundStartIcon);
		}
		else
		{
			m_bSound=FALSE;
			((CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND))->SetIcon(m_hSoundStopIcon);
		}
		SetPlayState();
		SetTimer(PLAYBYTIME_TIMER, 5000, NULL);
	}
	else
	{
		if (m_bPause)
		{
			if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYRESTART, 0, NULL))
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYRESTART");
				m_bPause=FALSE;
				SetPlayState();
			}
		}
		else
		{
			if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYPAUSE, 0, NULL))
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYPAUSE");
				m_bPause=TRUE;
				CButton *pButton;	
				pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY);
				pButton->SetIcon(m_hPlayEnableIcon);
				pButton->EnableWindow(TRUE);
			}
		}
	}
}

/*********************************************************
Function:	SetPlayState
Desc:		update the play state
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::SetPlayState() 
{
	m_sliderVolume.EnableWindow(TRUE);
	
	CButton *pButton;
	
	GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY);
	pButton->SetIcon(m_hPauseEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STOP);
	pButton->SetIcon(m_hStopEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_FAST);
	pButton->SetIcon(m_hFastEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SLOW);
	pButton->SetIcon(m_hSlowEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SNATCH_PIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND);
	if (m_bSound)
	{
		pButton->SetIcon(m_hSoundStartIcon);
	}
	else
	{
		pButton->SetIcon(m_hSoundStopIcon);
	}
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STEP);
	pButton->SetIcon(m_hStepEnableIcon);
	pButton->EnableWindow(TRUE);
}

/*********************************************************
Function:	SetPauseState
Desc:		update the pause button state
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::SetPauseState()
{
	CButton *pButton;	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY);
	pButton->SetIcon(m_hPlayEnableIcon);
	pButton->EnableWindow(TRUE);
	
	GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->EnableWindow(FALSE);
}

/*********************************************************
Function:	SetPlayStateIDTime
Desc:		update the play state
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::SetPlayStateIDTime() 
{
	m_sliderVolume.EnableWindow(TRUE);
	
	CButton *pButton;
	
	GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->EnableWindow(FALSE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME);
	pButton->SetIcon(m_hPauseEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STOP);
	pButton->SetIcon(m_hStopEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_FAST);
	pButton->SetIcon(m_hFastEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SLOW);
	pButton->SetIcon(m_hSlowEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SNATCH_PIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND);
	if (m_bSound)
	{
		pButton->SetIcon(m_hSoundStartIcon);
	}
	else
	{
		pButton->SetIcon(m_hSoundStopIcon);
	}
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_STEP);
	pButton->SetIcon(m_hStepEnableIcon);
	pButton->EnableWindow(TRUE);
}

/*********************************************************
Function:	SetPauseState
Desc:		update the pause button state
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::SetPauseStateIDTime()
{
	CButton *pButton;	
	pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME);
	pButton->SetIcon(m_hPlayEnableIcon);
	pButton->EnableWindow(TRUE);
	
	GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->EnableWindow(FALSE);
}

/*********************************************************
Function:	StopPlay
Desc:		Stop playback
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::StopPlay()
{
	if (m_lPlayHandle >= 0)
	{
		if (m_bSaveFile)
		{
			if (!NET_DVR_StopPlayBackSave(m_lPlayHandle))
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopPlayBackSave");
			}
			else
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopPlayBackSave");
				m_bSaveFile = FALSE;
			}
			
		}
		if (!NET_DVR_StopPlayBack(m_lPlayHandle))
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StopPlayBack");
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopPlayBack");
			m_bSaveFile = FALSE;
		}
		if (m_lPort >= 0)
		{
			PlayM4_FreePort(m_lPort);
			m_lPort = -1;
		}
		m_lPlayHandle = -1;
		KillTimer(REMOTE_PLAY_STATE_TIMER);
		UpdateData(FALSE);
		Invalidate(TRUE);
	}
	m_bPause = FALSE;
	m_bSetFrame = FALSE;
	
	if (EVENT_STREAM_INFO == m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel()))
	{
		SetStopStateIDTime();
	}
	else
	{
		SetStopState();
	}
}

/*********************************************************
Function:	EnableFlashWnd
Desc:		refresh the state
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::EnableFlashWnd(BOOL bEnable)
{
	if (bEnable)
	{
		//THE same to multiplay
		GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_STOP)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_SLOW)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_FAST)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_SOUND)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_SLIDER_EVENT_FILE_VOLUME)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_COMBO_EVENT_DATE_START)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_COMBO_EVENT_DATE_STOP)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_COMBO_EVENT_TIME_STOP)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_COMBO_EVENT_TIME_START)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_COMBO_MAJOR_EVENT)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_BTN_EVENT_SEARCH_LIST)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_SNATCH_PIC)->ModifyStyle(0,WS_VISIBLE,0);	
		//the only for single play
		GetDlgItem(IDC_BTN_EVENT_FILE_DOWNLOAD)->ModifyStyle(0,WS_VISIBLE,0);
		GetDlgItem(IDC_LIST_EVENT_FILE)->ModifyStyle(0,WS_VISIBLE,0);
		
		GetDlgItem(IDC_BTN_EVENT_FILE_STEP)->ModifyStyle(0,WS_VISIBLE,0);
	}
	else
	{
		//THE same to multiplay
		GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_STOP)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_SLOW)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_FAST)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_SOUND)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_SLIDER_EVENT_FILE_VOLUME)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_COMBO_EVENT_DATE_START)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_COMBO_EVENT_DATE_STOP)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_COMBO_EVENT_TIME_STOP)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_COMBO_EVENT_TIME_START)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_COMBO_MAJOR_EVENT)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_SEARCH_LIST)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_SNATCH_PIC)->ModifyStyle(WS_VISIBLE,0,0);
		//the only for single play
		GetDlgItem(IDC_LIST_EVENT_FILE)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_STEP)->ModifyStyle(WS_VISIBLE,0,0);
		GetDlgItem(IDC_BTN_EVENT_FILE_DOWNLOAD)->ModifyStyle(WS_VISIBLE,0,0);
	}
}

/*********************************************************
Function:	IsInWnd
Desc:		make sure the mouse is on this window or not
Input:	
Output:	
Return:	
**********************************************************/
BOOL CDlgPlayEvent::IsInWnd(CPoint point)
{
	CRect rc;
	GetDlgItem(IDC_STATIC_EVENT_FILE_WND)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	if (PtInRect(&rc,point))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*********************************************************
Function:	PlayBackFullScreen
Desc:		full screen
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::PlayBackFullScreen()
{
	EnableFlashWnd(FALSE);
	
	CRect rc,rcClient;
	::GetWindowRect(m_hPareDlgWnd,m_rcPareDlg);
	
	::GetWindowRect(m_hPareTabWnd,m_rcPareTab);
	
	::ScreenToClient(m_hPareDlgWnd,(POINT*)(&m_rcPareTab));
	::ScreenToClient(m_hPareDlgWnd,(POINT*)(&m_rcPareTab)+1);
	
	::GetWindowRect(m_hWnd,m_rcParent);
	::ScreenToClient(m_hPareTabWnd,(POINT*)(&m_rcParent));
	::ScreenToClient(m_hPareTabWnd,(POINT*)(&m_rcParent)+1);
	
	
	::SetWindowPos(m_hPareDlgWnd,NULL,m_rcScreen.left-SCREEN_EDGE,m_rcScreen.top-SCREEN_EDGE,m_rcScreen.Width()+2*SCREEN_EDGE,m_rcScreen.Height()+2*SCREEN_EDGE,SWP_SHOWWINDOW);
	
	::GetClientRect(m_hPareDlgWnd,rc);
	::SetWindowPos(m_hPareTabWnd,NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW);
	
	::GetClientRect(m_hPareTabWnd,rc);
	::SetWindowPos(m_hWnd,NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW);
	
	GetDlgItem(IDC_STATIC_EVENT_FILE_WND)->GetClientRect(rcClient);
	GetDlgItem(IDC_STATIC_EVENT_FILE_WND)->GetWindowRect(rc);
	int iEdge=(rc.Width()-rcClient.Width())/2;
	rc=m_rcScreen;
	rc.left-=iEdge;
	rc.right+=iEdge;
	rc.top-=iEdge;
	rc.bottom+=iEdge;
	this->ScreenToClient(rc);
	::SetWindowPos(GetDlgItem(IDC_STATIC_EVENT_FILE_WND)->m_hWnd,NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW);
	Invalidate(FALSE);
	UpdateWindow();
}

/*********************************************************
Function:	PlayBackShowNormal
Desc:		back to the normal show
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::PlayBackShowNormal()
{
	GetDlgItem(IDC_STATIC_EVENT_FILE_WND)->SetWindowPos(NULL,m_rcWnd.left,m_rcWnd.top,m_rcWnd.Width(),m_rcWnd.Height(),SWP_SHOWWINDOW);
	::SetWindowPos(m_hWnd,NULL,m_rcParent.left,m_rcParent.top,m_rcParent.Width(),m_rcParent.Height(),SWP_SHOWWINDOW);
	::SetWindowPos(m_hPareTabWnd,NULL,m_rcPareTab.left,m_rcPareTab.top,m_rcPareTab.Width(),m_rcPareTab.Height(),SWP_SHOWWINDOW);
	::SetWindowPos(m_hPareDlgWnd,NULL,m_rcPareDlg.left,m_rcPareDlg.top,m_rcPareDlg.Width(),m_rcPareDlg.Height(),SWP_SHOWWINDOW);
	EnableFlashWnd(TRUE);
	Invalidate(FALSE);
	UpdateWindow();
}

/*********************************************************
Function:	OnDestroy
Desc:		Destroy dialog when exiting
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnDestroy() 
{
	KillTimer(DOWNSTATE_TIMER);
	CDialog::OnDestroy();	
}

/*********************************************************
Function:	OnNMDblclkListRemoteFile
Desc:		play the selected file
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnNMDblclkListRemoteFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnBnClickedBtnRemoteFileStop();
	if (EVENT_STREAM_INFO == m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel()))
	{
		OnBtnEventFilePlayByIdAndTime();
	}
	else
	{
		OnBnClickedBtnRemoteFilePlay();
	}
	
	*pResult = 0;
}
/*********************************************************
Function:	OnLButtonDblClk
Desc:		Enlarge image by double click
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ((IsInWnd(point)) && (m_lPlayHandle>=0))
	{
		m_bFullScreen=!m_bFullScreen;
		g_pMainDlg->FullScreen(m_bFullScreen);//process of main frame
		if (m_bFullScreen)
		{
			PlayBackFullScreen();
		}
		else
		{
			PlayBackShowNormal();
		}
		
	}
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

/*********************************************************
Function:	OnTimer
Desc:		Function for Timer, show the status of download and playback
Input:	
Output:	
Return:	
**********************************************************/
#if (_MSC_VER >= 1500)	//vs2008
void CDlgPlayEvent::OnTimer(UINT_PTR nIDEvent)
#else
void CDlgPlayEvent::OnTimer(UINT nIDEvent) 
#endif
{
	DWORD nPos;
	char szLan[128] = {0};
	//	NET_DVR_TIME struOsdTime;
	if (nIDEvent == PLAYBYTIME_TIMER)
	{
		if (m_lPlayHandle >= 0)
		{
			if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYGETPOS, 0, &nPos))
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYGETPOS pos[%d]", nPos);
			}
			else
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYGETPOS");
			}
			
			// 			if (NET_DVR_GetPlayBackOsdTime(m_lPlayHandle, &struOsdTime))
			// 			{
			// 				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "OSDTime: %04d%02d%02d%02d%02d%02d", \
			// 					struOsdTime.dwYear, struOsdTime.dwMonth,struOsdTime.dwDay, struOsdTime.dwHour, struOsdTime.dwMinute, struOsdTime.dwSecond);
			// 			}
			// 			else
			// 			{
			// 				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetPlayBackOsdTime");
			// 			}
			
			if (nPos > 100)
			{
				StopPlay();
				g_StringLanType(szLan, "由于网络原因或DVR忙,回放异常终止!", " Due to network reasons or DVR is busy, playback abnormal termination");
				AfxMessageBox(szLan);
			}
			if (nPos == 100)
			{				
				StopPlay();
				g_StringLanType(szLan, "按时间回放结束", "playback by time over");
				AfxMessageBox(szLan);
			}		
		}
		
	}
	// 	else if (nIDEvent == DOWNBYTIME_TIMER)
	// 	{
	// 		if (m_bDown)
	// 		{
	// 			NET_DVR_PlayBackControl(m_lLoadHandle, NET_DVR_PLAYGETPOS, 0, &nPos);
	// 			if (nPos > 100)
	// 			{
	// 				m_bDown=FALSE;				
	// 				NET_DVR_StopGetFile(m_lLoadHandle);
	// 				g_StringLanType(szLan, "下载", "Download");
	// 				GetDlgItem(IDC_BTN_RT_DOWNLOAD)->SetWindowText(szLan);
	// 				g_StringLanType(szLan, "由于网络原因或DVR忙,下载异常终止!", " Due to network reasons or DVR is busy, download abnormal termination");
	// 				AfxMessageBox(szLan);
	// 			}
	// 			if (nPos == 100)
	// 			{
	// 				m_bDown=FALSE;				
	// 				NET_DVR_StopGetFile(m_lLoadHandle);
	// 				g_StringLanType(szLan, "下载", "Download");
	// 				GetDlgItem(IDC_BTN_RT_DOWNLOAD)->SetWindowText(szLan);
	// 				g_StringLanType(szLan, "按时间下载结束", "download by time over");
	// 				AfxMessageBox(szLan);
	// 			}		
	// 		}	
	// 	}	
	CDialog::OnTimer(nIDEvent);
}

/*********************************************************
Function:	OnBnClickedBtnRemoteSearchList
Desc:		function for search video by time or card NO.
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteSearchList()
{
	UpdateData(TRUE);
	char szLan[128] = {0};
	if (!m_bSearching)
	{
		m_IsQuit = TRUE;
		m_iMajorType = m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel());
		m_struEventParam.wMajorType = m_iMajorType;
		
        int i = m_comboMinorEvent.GetCurSel();
		m_struEventParam.wMinorType = m_comboMinorEvent.GetItemData(i);
		m_struEventParam.struStartTime.dwYear = (WORD)m_ctDateStart.GetYear();
		m_struEventParam.struStartTime.dwMonth = (WORD)m_ctDateStart.GetMonth();
		m_struEventParam.struStartTime.dwDay = (WORD)m_ctDateStart.GetDay();
		m_struEventParam.struStartTime.dwHour = (char)m_ctTimeStart.GetHour();
		m_struEventParam.struStartTime.dwMinute = (char)m_ctTimeStart.GetMinute();
		m_struEventParam.struStartTime.dwSecond = (char)m_ctTimeStart.GetSecond();
		m_struEventParam.struEndTime.dwYear = (WORD)m_ctDateStop.GetYear();
		m_struEventParam.struEndTime.dwMonth = (WORD)m_ctDateStop.GetMonth();
		m_struEventParam.struEndTime.dwDay = (WORD)m_ctDateStop.GetDay();
		m_struEventParam.struEndTime.dwHour = (char)m_ctTimeStop.GetHour();
		m_struEventParam.struEndTime.dwMinute = (char)m_ctTimeStop.GetMinute();
		m_struEventParam.struEndTime.dwSecond = (char)m_ctTimeStop.GetSecond();
		
		//the state is signed in updateChanStatus
		m_lDeviceType = g_struDeviceInfo[m_iDeviceIndex].iDeviceType;
		switch(m_iMajorType)
		{
		case EVENT_ALARM_IN:
		case EVENT_MOT_DET:
			//		case EVENT_AUX_ALARM:
			break;
		case EVENT_VCA_BEHAVIOR:
			m_struEventParam.uSeniorParam.struVcaParam.byRuleID = m_comboRuleID.GetItemData(m_comboRuleID.GetCurSel());
			break;
		case EVENT_STREAM_INFO:
			m_struEventParam.uSeniorParam.struStreamIDParam.struIDInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
			strncpy((char*)m_struEventParam.uSeniorParam.struStreamIDParam.struIDInfo.byID, m_strStreamID.GetBuffer(0), STREAM_ID_LEN);
			m_struEventParam.uSeniorParam.struStreamIDParam.struIDInfo.dwChannel = m_iRelatedChannel;
			m_struEventParam.uSeniorParam.struStreamIDParam.byBackupVolumeNum = (BYTE)m_dwBackupVolume;
			break;
		default:
			break;
		}
		
		
		m_lFileHandle = NET_DVR_FindFileByEvent(m_lLoginID, &m_struEventParam);
		
		if (m_lFileHandle < 0)
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_FindFileByEvent");
			g_StringLanType(szLan, "获取文件列表失败!", "Fail to get file list");
			AfxMessageBox(szLan);
			return;
		}
		m_listRemoteFile.DeleteAllItems();
		DWORD dwThreadId;
		if (m_hFileThread == NULL)
		{
			m_hFileThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetEventThread),this,0,&dwThreadId);	
		}
		if (m_hFileThread == NULL)
		{
			g_StringLanType(szLan, "打开查找线程失败!", "Fail to open finding thread!");
			AfxMessageBox(szLan);
			return;
		}
		g_StringLanType(szLan, "停止查找", "Stop Searching");
		GetDlgItem(IDC_BTN_EVENT_SEARCH_LIST)->SetWindowText(szLan);
		m_bSearching = TRUE;
		GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE)->ShowWindow(SW_SHOW);
	}
	else
	{
		if (m_hFileThread)
		{
			m_IsQuit = FALSE;
			//TerminateThread(m_hFileThread, 0);
		}
		CloseHandle(m_hFileThread);
		m_hFileThread = NULL;
		NET_DVR_FindClose(m_lFileHandle);
		g_StringLanType(szLan, "查找", "Search");
		GetDlgItem(IDC_BTN_EVENT_SEARCH_LIST)->SetWindowText(szLan);
		m_bSearching = FALSE;
		GetDlgItem(IDC_STATIC_EVENT_SEARCH_STATE)->ShowWindow(SW_HIDE);
		m_iFileNum = 0;
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFilePlay
Desc:		Function for start playback
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFilePlay()
{
	if (m_lPlayHandle == -1)
	{
		PlayBack();
	}
	else
	{
		if (m_bPause)
		{
			if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYRESTART, 0, NULL))
			{
				m_bPause=FALSE;
				SetPlayState();
			}
		}
		else
		{
			if (m_bSetFrame || m_bChangeSpeed)
			{
				if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYNORMAL, 0, NULL))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYNORMAL");
					m_bSetFrame=FALSE;
					m_bChangeSpeed = FALSE;
					SetPlayState();
				}
			}
			else
			{
				if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYPAUSE, 0, NULL))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYPAUSE");
					m_bPause=TRUE;
					SetPauseState();
				}
				if (!NET_DVR_RefreshPlay(m_lPlayHandle))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_RefreshPlay");
				}
			}
			
		}
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileStop
Desc:		stop playback
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileStop()
{
	StopPlay();
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileSave
Desc:		downlaod remote file
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileSave()
{
	static char szFilter[]="All File(*.mp4)|*.*||";
	char m_sFile[256] = "c:\\Savefile.mp4";
	CString csFile;
	CFileDialog dlg(FALSE,"*.mp4","Savefile.mp4",OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,
		szFilter);
	if (dlg.DoModal()==IDOK)
	{
		csFile = dlg.GetPathName();
		UpdateData(FALSE);
		sprintf(m_sFile, "%s", csFile);
	}
	
	if (NET_DVR_PlayBackSaveData(m_lPlayHandle, m_sFile))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PlayBackSaveData[%s]", m_sFile);
		m_bSaveFile = TRUE;
	}
	else
	{
		char szLan[128] = {0};
		g_StringLanType(szLan, "文件保存失败", "Save the file failed");
		AfxMessageBox(szLan);
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileStopSave
Desc:		stop downlaod remote file
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileStopSave()
{
	
	if (m_bSaveFile)
	{
		NET_DVR_StopPlayBackSave(m_lPlayHandle);
		m_bSaveFile = FALSE;
		char szLan[128] = {0};
		g_StringLanType(szLan, "停止保存成功", "Save the file successfully");
		AfxMessageBox(szLan);
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileGotoBeginning
Desc:		return to the begion of current file and play,
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileGotoBeginning()
{
	if (NET_DVR_PlayBackControl(m_lPlayHandle,NET_DVR_PLAYSETPOS, 0, NULL))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYSETPOS goto beginning");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYSETPOS goto beginning");
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileFast
Desc:		 playback faster
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileFast()
{
	if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYFAST, 0, NULL))
	{	
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYFAST");
		m_bChangeSpeed = TRUE;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYFAST");
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileSlow
Desc:		playback slower
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileSlow()
{
	if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSLOW, 0, NULL))
	{	
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYSLOW");
		m_bChangeSpeed = TRUE;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYSLOW");
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileSnatchPic
Desc:		Take snapshot of current palyback file
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileSnatchPic()
{
	char cFilename[256] = {0};
	
	sprintf(cFilename, "%s\\%d_%d.bmp",g_struLocalParam.chPictureSavePath,m_iChanShowNum, GetTickCount());
	if (NET_DVR_PlayBackCaptureFile(m_lPlayHandle,cFilename))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "%s NET_DVR_PlayBackCaptureFile",cFilename);
		return;
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PlayBackCaptureFile");
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileStep
Desc:		playback single frame
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileStep()
{
	if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYFRAME, 0, NULL))
	{
		CButton *pButton;	
		
		if (EVENT_STREAM_INFO == m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel()))
		{
			pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME);
			pButton->SetIcon(m_hPlayEnableIcon);
			pButton->EnableWindow(TRUE);
		}
		else
		{
			pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY);
			pButton->SetIcon(m_hPlayEnableIcon);
			pButton->EnableWindow(TRUE);
		}
		
		m_bSetFrame = TRUE;
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYFRAME");
	}
}

/*********************************************************
Function:	OnBnClickedBtnRemoteFileSound
Desc:		open/close audio
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::OnBnClickedBtnRemoteFileSound()
{
	CButton *pButton;
	
	if (m_bSound)
	{
		if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTOPAUDIO, 0, NULL))
		{
			m_bSound=FALSE;
			pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND);
			pButton->SetIcon(m_hSoundStopIcon);
			GetDlgItem(IDC_SLIDER_EVENT_FILE_VOLUME)->EnableWindow(FALSE);
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYSTOPAUDIO %s", g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].chChanName); 
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYSTOPAUDIO %s", g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].chChanName); 
		}
	}
	else
	{		
		if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTARTAUDIO, 0, NULL))
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYSTOPAUDIO %s", g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].chChanName); 
			pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND);
			GetDlgItem(IDC_SLIDER_EVENT_FILE_VOLUME)->EnableWindow(TRUE);
			pButton->SetIcon(m_hSoundStartIcon);
			m_bSound=TRUE;
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYSTOPAUDIO %s", g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].chChanName); 
		}
	}
}

/*********************************************************
Function:	OnClickListRemoteFile
Desc:		resolve the time of the event
Input:	void
Output:	void
Return:	void
**********************************************************/
void CDlgPlayEvent::OnClickListRemoteFile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	POSITION  iPos = m_listRemoteFile.GetFirstSelectedItemPosition();
	if (iPos == NULL)
	{
		return;
	}
	char szTime[32] = {0};
	char szTmp[8] = {0};
	int iOffset = 0;
	m_iItemSel = m_listRemoteFile.GetNextSelectedItem(iPos);
	sprintf(szTime, "%s", m_listRemoteFile.GetItemText(m_iItemSel, 1));//start
	memcpy(szTmp, szTime, 4);
	m_struStartTime.dwYear = atoi(szTmp);
	
	iOffset+=4;
	memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);	
	m_struStartTime.dwMonth = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStartTime.dwDay = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStartTime.dwHour = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStartTime.dwMinute = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStartTime.dwSecond = atoi(szTmp);
	
	memset(szTime, 0, 32);
	sprintf(szTime, "%s", m_listRemoteFile.GetItemText(m_iItemSel, 2));//stop time
	iOffset=0;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 4);
	m_struStopTime.dwYear = atoi(szTmp);
	
	iOffset+=4;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStopTime.dwMonth = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStopTime.dwDay = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStopTime.dwHour = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStopTime.dwMinute = atoi(szTmp);
	
	iOffset+=2;memset(szTmp, 0, 8);
	memcpy(szTmp, szTime+iOffset, 2);
	m_struStopTime.dwSecond = atoi(szTmp);
	
	if (EVENT_STREAM_INFO == m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel()))
	{
		CButton * pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME);
		pButton->SetIcon(m_hPlayEnableIcon);
		pButton->EnableWindow(TRUE);
		
		GetDlgItem(IDC_BTN_EVENT_FILE_PLAY)->EnableWindow(FALSE);
	}
	else
	{
		CButton * pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY);
		pButton->SetIcon(m_hPlayEnableIcon);
		pButton->EnableWindow(TRUE);
		
		GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME)->EnableWindow(FALSE);
	}
	
	
	
	UpdateData(FALSE);
	*pResult = 0;
}

/*********************************************************
Function:	OnCtlColor
Desc:		refresh the window
Input:	void
Output:	void
Return:	void
**********************************************************/
HBRUSH CDlgPlayEvent::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (m_lPlayHandle < 0 || !m_bPause)
	{
		return hbr;
	}
	if (!NET_DVR_RefreshPlay(m_lPlayHandle))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_RefreshPlay");
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

/*********************************************************
Function:	OnReleasedcaptureSliderRemoteFileVolume
Desc:		responding to mouse up message of volume progress
Input:	pNMHDR, point to NMHDR; 
pResult, point to LRESULT;
Output:	none
Return:	none
**********************************************************/
void CDlgPlayEvent::OnReleasedcaptureSliderRemoteFileVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	WORD temp = (0xffff)/100;
	WORD dwVolume = 0;
	char chPos = 0;
	chPos =(char)m_sliderVolume.GetPos();
	dwVolume =(WORD) (chPos*temp);
	m_sliderVolume.SetPos(chPos);
	if (m_lPlayHandle >= 0)
	{
		if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYAUDIOVOLUME, dwVolume, NULL))
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYAUDIOVOLUME %s", g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].chChanName); 
		}
		else
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PLAYAUDIOVOLUME %s", g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[m_iChanIndex].chChanName); 
		}		
	}
	*pResult = 0;
}

/*********************************************************
Function:	OnClickListChan
Desc:		refresh the channels's selected state
Input:	void
Output:	void
Return:	void
**********************************************************/
void CDlgPlayEvent::OnClickListChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	static int iAlarmInNo = 0,iMotDetChanNo = 0,iVacChanNo = 0,iVcaDetectNo = 0;
	m_struEventParam.byValue = (BYTE)m_bByValue;
	
	//POSITION  iPos = m_listChan.GetFirstSelectedItemPosition();//is not useful for small icon list
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	
	m_listChan.ScreenToClient(&point);
	
	UINT uFlag = 0;
	int iSel = m_listChan.HitTest(point, &uFlag);//
	if (iSel < 0)
	{
		return;
	}
	
	CString csTmp;
	int iData = m_listChan.GetItemData(iSel) + 1;
// 	if(g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum == 0)
// 	{
// 		iData = iData - 31;
// 	}
	BOOL bRet = m_listChan.GetCheck(iSel);
	if (iData == 0xffff||iData == 0xffdf||iData == 0xffe0)
	{
		OnChkAllChan(!bRet);
		TRACE("uFlag = %d, bRet = %d\n", uFlag, bRet);
		if (uFlag == LVHT_ONITEMLABEL)//uFlag | LVHT_ONITEM > 0
		{
			m_listChan.SetCheck(iSel, !bRet);			
		}
		return;
	}
	switch(m_struEventParam.wMajorType)
	{
	case 0:    
		if (bRet)
		{
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struMotionParam.byMotDetChanNo[iData] = 0;
			}
		}
		else
		{
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struMotionParam.byMotDetChanNo[iData] = 1;
			}
			else
			{
				m_struEventParam.uSeniorParam.struMotionParamByValue.wMotDetChanNo[iMotDetChanNo++] = iData;
			}
		}
		if (uFlag == LVHT_ONITEMLABEL)
		{
			m_listChan.SetCheck(iSel, !bRet);
		}		
		break;
	case 1: 
		if (bRet)
		{
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struAlarmParam.byAlarmInNo[iData] = 0;
			}
		}
		else
		{
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struAlarmParam.byAlarmInNo[iData] = 1;
			}
			else
			{
				m_struEventParam.uSeniorParam.struAlarmParamByValue.wAlarmInNo[iAlarmInNo++] = iData;
			}
		}
		
		if (uFlag == LVHT_ONITEMLABEL)
		{
			m_listChan.SetCheck(iSel, !bRet);
		}
		break;
    case 2:  
        if (bRet)
        {
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struVcaParam.byChanNo[iData] = 0;
			}      
        }
        else
        {
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struVcaParam.byChanNo[iData] = 1;
			} 
			else
			{
				m_struEventParam.uSeniorParam.struVcaParamByValue.wChanNo[iVacChanNo++] = iData;
			}
        }
        
        if (uFlag == LVHT_ONITEMLABEL)//
        {
            m_listChan.SetCheck(iSel, bRet);
        }
        break;
	case 4://智能侦测
//		m_struEventParam.byValue = 0;
        if (bRet)
        {
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struVCADetectByBit.byChan[iData] = 0;
			}
        }
        else
        {
			if (m_struEventParam.byValue == 0)
			{
				m_struEventParam.uSeniorParam.struVCADetectByBit.byChan[iData] = 1;
			}
			else
			{
				m_struEventParam.uSeniorParam.struVCADetectByValue.dwChanNo[iVcaDetectNo++] = iData;
			}
        }
        
        if (uFlag == LVHT_ONITEMLABEL)//
        {
            m_listChan.SetCheck(iSel, bRet);
        }
		break;
	default:
		break;
	}
	
	UpdateData(FALSE);
	*pResult = 0;
}



/*********************************************************
Function:	UpdateChanStatus
Desc:		update the status of all channels
Input:	none
Output:	none
Return:	none
**********************************************************/
void CDlgPlayEvent::UpdateChanStatus()
{
	int iIndex = 0;
	int i = 0;
	CString csTemp;
	m_listChan.DeleteAllItems();
	//get the whole state of all channels
	switch (m_struEventParam.wMajorType)
	{
	case EVENT_MOT_DET:
	case EVENT_VCA_BEHAVIOR:
		//	case EVENT_AUX_ALARM:
		//insert all channel node
		m_iIPChanNum = 0;
		m_listChan.InsertItem(iIndex, "All Chans");
		m_listChan.SetItemData(iIndex, 0xffff);
		iIndex ++;	
		for (i=0; i<g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; i++)
		{
			if (i<g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum)
			{
				csTemp.Format(ANALOG_C_FORMAT, g_struDeviceInfo[m_iDeviceIndex].iStartChan+i);
				m_listChan.InsertItem(iIndex, csTemp);
				m_listChan.SetItemData(iIndex, i + g_struDeviceInfo[m_iDeviceIndex].iStartChan);
				iIndex ++;	
			}
			else 
			{
				csTemp.Format(DIGITAL_C_FORMAT, i+ 1 - g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum/*g_struDeviceInfo[m_iDeviceIndex].iStartChan-g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan*/);
				m_listChan.InsertItem(iIndex, csTemp);
				m_listChan.SetItemData(iIndex, i- g_struDeviceInfo[m_iDeviceIndex].iAnalogChanNum + g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan);
				m_iIPChanNum ++;
				iIndex ++;	
			}
		}
		break;
	case EVENT_ALARM_IN:
		m_iIPAlarmIn = 0;
		m_listChan.InsertItem(iIndex, "All AlarmIn");
		m_listChan.SetItemData(iIndex, 0xffff);
		iIndex ++;	
		for (i=0; i<MAX_IP_ALARMIN_V40; i++)
		{
			if (i<g_struDeviceInfo[m_iDeviceIndex].iAlarmInNum)
			{
				csTemp.Format(ALARM_IN_NAME, 1+i);
				m_listChan.InsertItem(iIndex, csTemp);
				m_listChan.SetItemData(iIndex, i);
				iIndex ++;	
			}
			else if (i>=MAX_ANALOG_ALARMIN && g_struDeviceInfo[m_iDeviceIndex].pStruIPAlarmInCfgV40->struIPAlarmInInfo[i -MAX_ANALOG_ALARMIN].dwIPID != 0)
			{
				csTemp.Format(IP_ALARM_IN_NAME, 1+i-MAX_ANALOG_ALARMIN);
				m_listChan.InsertItem(iIndex, csTemp);
				m_listChan.SetItemData(iIndex, i);
				m_iIPAlarmIn ++;
				iIndex ++;				
			}
		}
		break;
	case EVENT_VCA_DETECTION:
		m_iIPChanNum = 0;
		m_listChan.InsertItem(iIndex, "All Chans");
		m_listChan.SetItemData(iIndex, 0xffff);	
		iIndex ++;	
		for (i=0; i<256; i++)
		{
			if (i<MAX_ANALOG_CHANNUM&&i < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum-g_struDeviceInfo[m_iDeviceIndex].iIPChanNum)
			{
				csTemp.Format(ANALOG_C_FORMAT, g_struDeviceInfo[m_iDeviceIndex].iStartChan+i);
				m_listChan.InsertItem(iIndex, csTemp);
				m_listChan.SetItemData(iIndex, i);
				iIndex ++;	
			}
			else if (i>=MAX_ANALOG_CHANNUM && i < g_struDeviceInfo[m_iDeviceIndex].iIPChanNum+MAX_ANALOG_CHANNUM)
			{
				csTemp.Format(DIGITAL_C_FORMAT, i+g_struDeviceInfo[m_iDeviceIndex].iStartChan-MAX_ANALOG_CHANNUM);
				m_listChan.InsertItem(iIndex, csTemp);
				m_listChan.SetItemData(iIndex, i);
				m_iIPChanNum ++;
				iIndex ++;	
			}
		}
		break;
	default:
		break;
	}
}

/*********************************************************
Function:	OnChkAllChan
Desc:		select all channels or not	
Input:	void
Output:	void
Return:	void
**********************************************************/
void CDlgPlayEvent::OnChkAllChan(BOOL bAll) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString csTmp;
	int iSel = 0;
	int iChanIndex = 0;
	int i = 0;
	
	switch(m_struEventParam.wMajorType)
	{
	case EVENT_MOT_DET:
        for (i = 0; i<g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; i++)//MAX_CHANNUM_V30
        {
            iSel++;
            m_listChan.SetCheck(iSel, bAll);
            iChanIndex = m_listChan.GetItemData(iSel);
			if (m_bByValue) // find Data By Value
            {
                if (bAll)
                {
                    m_struEventParam.uSeniorParam.struMotionParamByValue.wMotDetChanNo[i] = iChanIndex;
                }
                else
                {
                    m_struEventParam.uSeniorParam.struMotionParamByValue.wMotDetChanNo[i] = 0;
                }
			}
			else
			{                
				if (bAll)
				{
					m_struEventParam.uSeniorParam.struMotionParam.byMotDetChanNo[iChanIndex] = 1;
				}
				else
				{
					m_struEventParam.uSeniorParam.struMotionParam.byMotDetChanNo[iChanIndex] = 0;
				}
				
            }
            if (iChanIndex == MAX_CHANNUM_V30-1)
            {
                break;
            }
        }
        break;
		
	case EVENT_VCA_BEHAVIOR:
		for (i = 0; i<g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum+m_iIPChanNum; i++)//MAX_CHANNUM_V30
		{
			iSel++;
			m_listChan.SetCheck(iSel, bAll);
			iChanIndex = m_listChan.GetItemData(iSel);
			if (bAll)
			{
				if (m_bByValue) // find Data By Value
                {
                    m_struEventParam.uSeniorParam.struVcaParamByValue.wChanNo[i] = iChanIndex;
                }
                else
                { 
                    m_struEventParam.uSeniorParam.struVcaParam.byChanNo[iChanIndex] = 1;
                }
			}
			else
			{
                if (m_bByValue) // find Data By Value
                {
                    m_struEventParam.uSeniorParam.struVcaParamByValue.wChanNo[i] = 0;
                }
                else
                { 
                    m_struEventParam.uSeniorParam.struVcaParam.byChanNo[iChanIndex] = 0;
				}
			}
			if (iChanIndex == MAX_CHANNUM_V30-1)
			{
				break;
			}
		}
		break;
	case EVENT_ALARM_IN:
		for (i = 0; i<g_struDeviceInfo[m_iDeviceIndex].iAlarmInNum+m_iIPAlarmIn; i++)
		{
			iSel++;
			m_listChan.SetCheck(iSel, bAll);
			iChanIndex = m_listChan.GetItemData(iSel);
			if (m_bByValue)
            {
                if (bAll)
                {
                    m_struEventParam.uSeniorParam.struAlarmParamByValue.wAlarmInNo[i] = iChanIndex;
                }
                else
                {
                    m_struEventParam.uSeniorParam.struAlarmParamByValue.wAlarmInNo[i] = 0;
                }
                if (iChanIndex == 128 -1)
                {
                    break;
                }
            }
            else
            {
                if (bAll)
                {
                    m_struEventParam.uSeniorParam.struAlarmParam.byAlarmInNo[iChanIndex] = 1;
                }
                else
                {
                    m_struEventParam.uSeniorParam.struAlarmParam.byAlarmInNo[iChanIndex] = 0;
                }
                if (iChanIndex == MAX_ALARMIN_V30-1)
                {
                    break;
                }
            }
			
		}
		break;
	case EVENT_VCA_DETECTION:
//		m_struEventParam.byValue = 0;
		for (i = 0; i < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; i++)
		{
            iSel++;
            m_listChan.SetCheck(iSel, bAll);
            iChanIndex = m_listChan.GetItemData(iSel);
			if (m_bByValue)
            {
                if (bAll)
                {
					m_struEventParam.uSeniorParam.struVCADetectByValue.byAll = 1;
                    m_struEventParam.uSeniorParam.struVCADetectByValue.dwChanNo[i] = iChanIndex;
                }
                else
                {
                    m_struEventParam.uSeniorParam.struVCADetectByValue.dwChanNo[i] = 0xffffffff;
                }
                if (iChanIndex == 64 -1)
                {
                    break;
                }
            }
			else
			{
				if (bAll)
				{
					m_struEventParam.uSeniorParam.struVCADetectByBit.byChan[iChanIndex] = 1;
				}
				else
				{
					m_struEventParam.uSeniorParam.struVCADetectByBit.byChan[iChanIndex] = 0;
				}
				if (iChanIndex == 255)
				{
					break;
				}
			}
		}
		break;
	default:
		break;
	}
	
	
	UpdateData(FALSE);	
}

/*********************************************************
Function:	OnSelchangeComboMajorEvent
Desc:		select the major event
Input:	void
Output:	void
Return:	void
**********************************************************/
void CDlgPlayEvent::OnSelchangeComboMajorEvent() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_iMajorType != m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel()))
	{
		m_iMajorType = m_comboMajorType.GetItemData(m_comboMajorType.GetCurSel());
		m_struEventParam.wMajorType = m_iMajorType;
		UpdateMinorEvent();
		UpdateChanStatus();
	}
	
	UpdateData(FALSE);
}

/*********************************************************
Function: UpdateMinorEvent	
Desc:		update the minor event	
Input:	void
Output:	void
Return:	void
**********************************************************/
void CDlgPlayEvent::UpdateMinorEvent()
{
	int index = 0;
	m_iMinorType = 0xffff;
	m_comboMinorEvent.ResetContent();
	char szLan[128] = {0};
	switch(m_iMajorType)
	{
	case EVENT_MOT_DET:
	case EVENT_ALARM_IN:
		//	case EVENT_AUX_ALARM:
		g_StringLanType(szLan, "全部", "All");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, 0xffff);
		index++;
		GetDlgItem(IDC_STATIC_ID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_RULEID)->ShowWindow(SW_HIDE);
		break;
	case EVENT_VCA_BEHAVIOR:
		g_StringLanType(szLan, "全部", "All");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, 0xffff);
		index++;
		
		g_StringLanType(szLan, "穿越警戒面", "Traverse Plane");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_TRAVERSE_PLANE);
		index++;
		
		g_StringLanType(szLan, "进入区域", "Enter Area");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_ENTER_AREA);
		index++;
		
		g_StringLanType(szLan, "离开区域", "Exit Area");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_EXIT_AREA);
		index++;
		
		g_StringLanType(szLan, "入侵", "Intrusion");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_INTRUSION);
		index++;
		
		g_StringLanType(szLan, "徘徊", "Loiter");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_LOITER);
		index++;
		
		g_StringLanType(szLan, "物品遗留拿取", "Left and Take");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_LEFT_TAKE);
		index++;
		
		g_StringLanType(szLan, "停车", "Parking");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_PARKING);
		index++;
		
		g_StringLanType(szLan, "快速移动", "Running");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_RUN);
		index++;
		
		g_StringLanType(szLan, "人员聚集", "Staff Density");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index,  EVENT_HIGH_DENSITY);
        index++;
		
        g_StringLanType(szLan, "贴纸条", "Stick up");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_STICK_UP);
        index++;
		
        g_StringLanType(szLan, "安装读卡器", "Install Scanner");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_INSTALL_SCANNER);
        index++;
		
        g_StringLanType(szLan, "操作超时", "over time");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_OPERATE_OVER_TIME);
        index++;
		
        g_StringLanType(szLan, "异常人脸", "Face Detect");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_FACE_DETECT);
        index++;
		
        g_StringLanType(szLan, "物品遗留", "Left");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_LEFT);
        index++;
        
        g_StringLanType(szLan, "物品拿取", "Take");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_TAKE);
        index++;
		
		g_StringLanType(szLan, "离岗事件", "Leave Position");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_LEAVE_POSITION);
        index++;
		
		g_StringLanType(szLan, "尾随", "Trail");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_TRAIL_INFO);
        index++;
		
		g_StringLanType(szLan, "倒地", "Fall Down");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_FALL_DOWN_INFO);
        index++;
		
		g_StringLanType(szLan, "正常人脸", "Normal");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_FACE_CAPTURE_INFO);
        index++;
		
		g_StringLanType(szLan, "多张人脸", "Multi");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_MULTI_FACES_INFO);
        index++;
		
		g_StringLanType(szLan, "声强突变", "Audio Abnormal");
        m_comboMinorEvent.AddString(szLan);
        m_comboMinorEvent.SetItemData(index,  EVENT_AUDIO_ABNORMAL_INFO);
        index++;
		
		//rule id
		GetDlgItem(IDC_STATIC_ID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_RULEID)->ShowWindow(SW_SHOW);
		m_comboRuleID.SetCurSel(0);
		break;
	case EVENT_STREAM_INFO:
		g_StringLanType(szLan, "流ID", "Stream ID");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, 0);
		index++;
		
		g_StringLanType(szLan, "定时录像", "Timing Record");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_TIMING);
		index++;
		
		g_StringLanType(szLan, "移动侦测", "Motion Detect");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_MOTION_DETECT);
		index++;
		
		g_StringLanType(szLan, "报警录像", "Alarm");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_ALARM);
		index++;
		
		g_StringLanType(szLan, "报警或移动录像", "Alarm or Motion Detect");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_ALARM_OR_MOTION_DETECT);
		index++;
		
		g_StringLanType(szLan, "报警和移动录像", "Alarm and Motion Detect");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_ALARM_AND_MOTION_DETECT);
		index++;
		
		g_StringLanType(szLan, "命令触发", "Command");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_COMMAND_TRIGGER);
		index++;
		
		g_StringLanType(szLan, "手动录像", "Manual");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_MANNUAL);
		index++;
		
		g_StringLanType(szLan, "存档卷录像", "Backup volume");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_BACKUP_VOLUME);
		GetDlgItem(IDC_STATIC_ID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_RULEID)->ShowWindow(SW_HIDE);
		break;
	case EVENT_VCA_DETECTION:
		g_StringLanType(szLan, "全部", "All");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_ALL);
		index++;
		g_StringLanType(szLan, "越警侦测", "Traverse Plane");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_VCA_TRAVERSE_PLANE);
		index++;
		g_StringLanType(szLan, "区域入侵", "field detect");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_FIELD_DETECTION);
		index++;
		g_StringLanType(szLan, "人脸识别", "face detect");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_FACE_DETECTION);
		index++;
		g_StringLanType(szLan, "音频输入异常", "Audio Unnormal");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_AUDIO_INPUT_ALARM);
		index++;
		g_StringLanType(szLan, "声强突变侦测", "Audio Detect");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_SOUND_INTENSITY_ALARM);
		index++;
		g_StringLanType(szLan, "虚焦侦测", "virtual focus alarm");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_VIRTUAL_FOCUS_ALARM);
		index++;
		g_StringLanType(szLan, "场景变更侦测", "scene change alarm");
		m_comboMinorEvent.AddString(szLan);
		m_comboMinorEvent.SetItemData(index, EVENT_SCENE_CHANGE_ALARM);
		index++;
		break;
	default:
		break;
	}
	m_comboMinorEvent.SetCurSel(0);
}

/*********************************************************
Function:	OnSelchangeComboMinorEvent
Desc:		
Input:	void
Output:	void
Return:	void
**********************************************************/
void CDlgPlayEvent::OnSelchangeComboMinorEvent() 
{
	// TODO: Add your control notification handler code here
	
}

/*********************************************************
Function:	PlayBack
Desc:		begin play back 
Input:	
Output:	
Return:	
**********************************************************/
void CDlgPlayEvent::PlayBackByIDAndTime()
{
	UpdateData(TRUE);
	
	int m_nVerifyret = -1;
	int iFileSelPos = 0;
	HWND hPlayWnd=m_staticPlayWnd.GetSafeHwnd();
	POSITION  posItem = m_listRemoteFile.GetFirstSelectedItemPosition();
	char szLan[128] = {0};
	if (posItem == NULL)
	{
		g_StringLanType(szLan, "请选择要播放的文件!", "Please select the file to play");
		AfxMessageBox(szLan); 
		return;
	}
	
	iFileSelPos = m_listRemoteFile.GetNextSelectedItem(posItem);
	DWORD dwFileIndex = atoi(m_listRemoteFile.GetItemText(iFileSelPos,5));
	
	
	NET_DVR_TIME struStartTime,struStopTime;
	NET_DVR_VOD_PARA struVODPara;
	memset(&struVODPara, 0, sizeof(NET_DVR_VOD_PARA));
	
	//	remoteplay_info.srcfilename=m_szFileName;
	if (m_lPlayHandle>=0)
	{
		if (NET_DVR_StopPlayBack(m_lPlayHandle))
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StopPlayBack");
		}
		m_lPlayHandle = -1;
		Sleep(400);
	}
	if (m_lPlayHandle == -1)
	{
		m_iChanShowNum = m_iChanIndex+m_lStartChan;
		time_t tmp;
		ConvertTime(&tmp, &m_struStartTime, FALSE);
		//tmp -= m_iTime;
		ConvertTime(&tmp, &m_struStartTime, TRUE);
		
		ConvertTime(&tmp, &m_struStopTime, FALSE);
		//tmp += m_iTime;
		ConvertTime(&tmp, &m_struStopTime, TRUE);
		memcpy(&struStartTime, &m_struStartTime, sizeof(NET_DVR_TIME));
		memcpy(&struStopTime, &m_struStopTime, sizeof(NET_DVR_TIME));	
		
		// 给struVODPara赋值
		struVODPara.dwSize = sizeof(NET_DVR_VOD_PARA);
		struVODPara.struIDInfo.dwSize = sizeof(NET_DVR_STREAM_INFO);
		strncpy((char*)struVODPara.struIDInfo.byID, m_strStreamID.GetBuffer(0), STREAM_ID_LEN);
		struVODPara.struIDInfo.dwChannel = m_iRelatedChannel;
		struVODPara.hWnd = hPlayWnd;
		memcpy(&struVODPara.struBeginTime, &struStartTime, sizeof(NET_DVR_TIME));
		memcpy(&struVODPara.struEndTime, &struStopTime, sizeof(NET_DVR_TIME));
		if (m_comboMinorEvent.GetItemData(m_comboMinorEvent.GetCurSel()) == EVENT_BACKUP_VOLUME)
		{
			struVODPara.byVolumeType = 1; //回放存档卷
		}
		else
		{
			struVODPara.byVolumeType = 0; //回放普通卷
		}
		
		struVODPara.byVolumeNum = m_dwBackupVolume;
		struVODPara.dwFileIndex = dwFileIndex;
		struVODPara.byDrawFrame = m_bDrawFrame;
		
		m_lPlayHandle = NET_DVR_PlayBackByTime_V40(m_lLoginID, &struVODPara);
		
		if (m_lPlayHandle == -1)
		{
			g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_PlayBackByTime_V40");
			g_StringLanType(szLan, "通过ID和时间远程回放失败", "Fail to play back remote by ID and time");
			AfxMessageBox(szLan);
			return;
		}
		int idx = NET_DVR_GetPlayBackPlayerIndex(m_lPlayHandle);
		
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PlayBackByTime_V40");
		NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTART, 0, NULL);
		
		if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYSTARTAUDIO, 0, NULL))
		{
			m_bSound=TRUE;
			NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYAUDIOVOLUME, (0xffff)/2, NULL);
			m_sliderVolume.SetPos(50);
			((CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND))->SetIcon(m_hSoundStartIcon);
		}
		else
		{
			m_bSound=FALSE;
			((CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_SOUND))->SetIcon(m_hSoundStopIcon);
		}
		SetPlayStateIDTime();
		SetTimer(PLAYBYTIME_TIMER, 5000, NULL);
	}
	else
	{
		if (m_bPause)
		{
			if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYRESTART, 0, NULL))
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYRESTART");
				m_bPause=FALSE;
				SetPlayStateIDTime();
			}
		}
		else
		{
			if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYPAUSE, 0, NULL))
			{
				g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYPAUSE");
				m_bPause=TRUE;
				CButton *pButton;	
				pButton = (CButton *)GetDlgItem(IDC_BTN_EVENT_FILE_PLAY_BY_ID_AND_TIME);
				pButton->SetIcon(m_hPlayEnableIcon);
				pButton->EnableWindow(TRUE);
			}
		}
	}
}

void CDlgPlayEvent::OnBtnEventFilePlayByIdAndTime() 
{
	// TODO: Add your control notification handler code here
	if (m_lPlayHandle == -1)
	{
		PlayBackByIDAndTime();
	}
	else
	{
		if (m_bPause)
		{
			if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYRESTART, 0, NULL))
			{
				m_bPause=FALSE;
				SetPlayStateIDTime();
			}
		}
		else
		{
			if (m_bSetFrame || m_bChangeSpeed)
			{
				if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYNORMAL, 0, NULL))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYNORMAL");
					m_bSetFrame=FALSE;
					m_bChangeSpeed = FALSE;
					SetPlayStateIDTime();
				}
			}
			else
			{
				if (NET_DVR_PlayBackControl(m_lPlayHandle, NET_DVR_PLAYPAUSE, 0, NULL))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_PLAYPAUSE");
					m_bPause=TRUE;
					SetPauseStateIDTime();
				}
				if (!NET_DVR_RefreshPlay(m_lPlayHandle))
				{
					g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_RefreshPlay");
				}
			}
			
		}
	}
}
