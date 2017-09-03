// DlgIPCSimpIntellCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgIPCSimpIntellCfg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIPCSimpIntellCfg dialog

void CALLBACK AudioDecibelCB(DWORD dwType, void* lpBuffer, DWORD dwBufLen, void* pUserData)
{
	int i = *((int*)lpBuffer);
	CDlgIPCSimpIntellCfg *pDlg = (CDlgIPCSimpIntellCfg *)pUserData;
	pDlg->SetAudioDecibel(i);
}

void CALLBACK DrawVideoEpoliceAlertShow(LONG lRealHandle, HDC hDc, DWORD dwUser)
{
    SetBkMode(hDc, TRANSPARENT);
    SetTextColor(hDc, RGB(255, 255, 255));

	CDlgIPCSimpIntellCfg *lp = (CDlgIPCSimpIntellCfg *)dwUser;
    lp->F_DrawFun(lRealHandle, hDc, dwUser);     
}

BOOL IsCrossLine( NET_VCA_POLYGON *struRuleRegion )
{
	//如果结构空，则不判断
	if(struRuleRegion == NULL)
	{
		return FALSE;
	}
	
	int i = 0, j = 0;
	DWORD dwPointTotal = 0;
	NET_VCA_POINT *pPointArray = NULL;
	bool    bCrossLine = FALSE;
	
	pPointArray = (NET_VCA_POINT *)struRuleRegion->struPos;
	dwPointTotal = struRuleRegion->dwPointNum;
	
    CString strTemp = "";
    if(dwPointTotal > 10)
    {
        strTemp.Format("dwPonitTotal is[%d]",dwPointTotal);
        ::AfxMessageBox(strTemp);
        return FALSE;
    }

	//如果小于最小个数3，不能构成框
	if (dwPointTotal == MIN_PNT_NUM)
	{
		bCrossLine = FALSE;
	}
	else
	{
		
		for(i=0; i<static_cast<int>(dwPointTotal-3); i++)
		{
			for(j=i+2; j<static_cast<int>(dwPointTotal-1); j++)
			{
				if(F_LineCrossDetect(pPointArray[i], pPointArray[i + 1], pPointArray[j], pPointArray[j + 1]))
				{
					bCrossLine = TRUE;
				}   
			}
		}
	}
	
	return bCrossLine;
}

BOOL F_LineCrossDetect( NET_VCA_POINT p1, NET_VCA_POINT p2, NET_VCA_POINT q1, NET_VCA_POINT q2 )
{
	NET_VCA_POINT p1_q1, q2_q1, p2_q1, q1_p1, p2_p1, q2_p1;       //Difference vector
    float z1, z2, z3, z4;
    BOOL  line_cross = FALSE;
    
    // p1 - q1;
    p1_q1.fX = p1.fX - q1.fX;
    p1_q1.fY = p1.fY - q1.fY;
    
    // q2 - q1;
    q2_q1.fX = q2.fX - q1.fX;
    q2_q1.fY = q2.fY - q1.fY;
    
    // p2 - q1;
    p2_q1.fX = p2.fX - q1.fX;
    p2_q1.fY = p2.fY - q1.fY;
    
    // 1 (p1 - q1) × (q2 - q1)；
    z1 = p1_q1.fX * q2_q1.fY - q2_q1.fX * p1_q1.fY; 
    
    // 2 (q2 - q1) × (p2 - q1)；
    z2 = q2_q1.fX * p2_q1.fY - p2_q1.fX * q2_q1.fY;
    
    // q1 - p1;
    q1_p1.fX = q1.fX - p1.fX;
    q1_p1.fY = q1.fY - p1.fY;
    
    // p2 - p1;
    p2_p1.fX = p2.fX - p1.fX;
    p2_p1.fY = p2.fY - p1.fY;
    
    // q2 - p1;
    q2_p1.fX = q2.fX - p1.fX;
    q2_p1.fY = q2.fY - p1.fY;
    
    // 3 (q1 - p1) × (p2 - p1)；
    z3 = q1_p1.fX * p2_p1.fY - p2_p1.fX * q1_p1.fY;
    
    //4 (p2 - p1) × (q2 - p1);
    z4 = p2_p1.fX * q2_p1.fY - q2_p1.fX * p2_p1.fY;
    
    
    //  check the span of two segment
    if( ((z1 >= 0.0f && z2 >= 0.0f) || (z1 < 0.0f && z2 < 0.0f)) && 
        ((z3 >= 0.0f && z4 >= 0.0f) || (z3 < 0.0f && z4 < 0.0f)))
    {
        line_cross = TRUE;	
    }else
    {
        line_cross = FALSE;
    }
    
    return line_cross;
}

BOOL IsValidArea( LPNET_VCA_POLYGON lpstruPolygon )
{
	if(lpstruPolygon == NULL)
    {
        return FALSE;
    }
    // 判断组成多边形的所有点是否在一条直线上
    if (F_IsStraightLine(lpstruPolygon))
    {
        char szLan[128] = {0};
        g_StringLanType(szLan, "所有点在同一直线上，不能组成多边形!",\
            "All points have been in the same line, not able to form the polygon");
        //AfxMessageBox(szLan);
        return FALSE;
    }
    // 判断组成多边形的终点和起点的连线与已有的直线是否有交叉
    if (F_IsACrossLine(lpstruPolygon))
    {
        char szLan[128] = {0};
        g_StringLanType(szLan,"终点和起点的连线与已有的直线相交，不能组成有效的多边形!", \
            "There are straight-line intersection, can not be composed of an effective polygon");
        //AfxMessageBox(szLan);
        return FALSE;
    }
    return TRUE;
}

BOOL F_IsStraightLine( NET_VCA_POLYGON *lpstruPolygon )
{
	if(lpstruPolygon == NULL)
    {
        return FALSE;
    }
    unsigned int i = 0;
    BOOL         bStraightLine;
    float        fSlope;
    float        fDistance;
    float        fCoefficient;
    float        fOffset;
    DWORD		 nPointTotal;
    NET_VCA_POINT *pPointArray;
    int          nXOffset;
    int          nYOffset;
    
    pPointArray = lpstruPolygon->struPos;
    nPointTotal = lpstruPolygon->dwPointNum;
    
    nXOffset = (int)(pPointArray[0].fX - pPointArray[1].fX);
    nYOffset = (int)(pPointArray[0].fY - pPointArray[1].fY);
    bStraightLine = TRUE;
    
    //Check if all points construct a vertical line.
    if (0 == nXOffset)
    {
        for (i = 2; i < nPointTotal; i++)
        {
            if (pPointArray[0].fX != pPointArray[i].fX)
            {
                bStraightLine = FALSE;
                break;
            }
        }
    }
    
    // Check if all points construct a horizontal line.
    if (0 == nYOffset)
    {
        for (i = 2; i < nPointTotal; i++)
        {
            if (pPointArray[0].fY != pPointArray[i].fY)
            {
                bStraightLine = FALSE;
                break;
            }                 
        }
    }
    
    // Check if all points construct a diagonal.
    if ((0 != nXOffset) && (0 != nYOffset))
    {
        // calculate slope
        fSlope  = 1.0f * nYOffset / nXOffset;
        // intercept 
        fOffset = pPointArray[0].fY - fSlope * pPointArray[1].fX;
        // the coefficient for calculating the distance between point and line. 
        fCoefficient = (float)sqrt(1.0 + fSlope * fSlope);
        
        for (i = 2; i < nPointTotal; i ++)
        {
            // Calculate the distance between other point and the line constructed by point 1 and point 2. 
            fDistance = (float)fabs(fSlope * pPointArray[i].fX - pPointArray[i].fY + fOffset)/fCoefficient;
            
            // if there is one distance of points that is more than 0, the polygon is valid.
            if ((fDistance - 0.0f) > 0.000001f)
            {
                bStraightLine = FALSE;
                break;
            }
        }		
    }
    
    return bStraightLine;	
}

BOOL F_IsACrossLine( NET_VCA_POLYGON *alarm_region )
{
	if(alarm_region == NULL)
    {
        return FALSE;
    }
    unsigned int i;
    DWORD	 nPointTotal;
    NET_VCA_POINT *pPointArray;
    NET_VCA_POINT *pPntArr;
    BOOL    bCrossLine;
    
    pPointArray = alarm_region->struPos;
    nPointTotal = alarm_region->dwPointNum;
    bCrossLine  = FALSE;
    
    
    if (nPointTotal == ITC_MAX_POLYGON_POINT_NUM)
    {
        bCrossLine = FALSE;
    }
    else
    {
        pPntArr = new NET_VCA_POINT[nPointTotal];
        if (pPntArr == NULL)
        {
            char szLan[128] = {0};
            g_StringLanType(szLan, "Allocate memory failed!","Memory allocation failure!");
            //AfxMessageBox(szLan);
            exit(EXIT_FAILURE);
        }
        
        for (i = 0; i < nPointTotal; i++)
        {
            pPntArr[i] = pPointArray[i];
        }
        //      pPntArr[nPointTotal] = pPointArray[0];
        
        for(i = 0; i < nPointTotal - 1; i++)
        {
            
            if (F_LineCrossDetect(pPntArr[0], pPntArr[nPointTotal - 1], pPntArr[i], pPntArr[i + 1]))
            {
                bCrossLine = TRUE;
            }			
        } 
        delete [] pPntArr;
    }
    return bCrossLine;
}

CDlgIPCSimpIntellCfg::CDlgIPCSimpIntellCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIPCSimpIntellCfg::IDD, pParent)
	, m_bChkInvokeAlarmOut(FALSE)
	, m_bChkInvokeJpegCapture(FALSE)
	, m_bChkPicToFtp(FALSE)
	, m_iHour11(0)
	, m_iMin11(0)
	, m_iHour12(0)
	, m_iMin12(0)
	, m_iHour21(0)
	, m_iMin21(0)
	, m_iHour22(0)
	, m_iMin22(0)
	, m_iHour31(0)
	, m_iMin31(0)
	, m_iHour32(0)
	, m_iMin32(0)
	, m_iHour41(0)
	, m_iMin41(0)
	, m_iHour42(0)
	, m_iMin42(0)
	, m_iHour51(0)
	, m_iMin51(0)
	, m_iHour52(0)
	, m_iMin52(0)
	, m_iHour61(0)
	, m_iMin61(0)
	, m_iHour62(0)
	, m_iMin62(0)
	, m_iHour71(0)
	, m_iMin71(0)
	, m_iHour72(0)
	, m_iMin72(0)
	, m_iHour81(0)
	, m_iMin81(0)
	, m_iHour82(0)
	, m_iMin82(0)
	, m_bChkUploadCenter(FALSE)
	, m_bChkMonitorAlarm(FALSE)
	, m_bChkVoiceAlarm(FALSE)
	, m_bChkEmapAlarmOut(FALSE)
	, m_bChkFocusAlarm(FALSE),
	m_lLoginID(-1),
	m_iDeviceIndex(-1),
	m_lChannel(-1),
	m_lPlayHandle(-1),
	m_iChanCount(0),
    m_iAnaChanCount(0),
	m_iDStartChannel(-1),
	m_lStartChannel(-1),
	m_bEnable(FALSE),
	m_bAudioAbnormal(FALSE),
	m_iAudioSen("0"),
	m_iAudioThreshold("0"),
	m_iLineSen("0"),
	m_iCross("0"),
	m_iFieldDuration("0"),
	m_iFieldRate("0"),
	m_iFieldSen("0"),
	m_iFieldNo(0),
	m_iLineNo(0),
	m_dwPosNum(0),
	m_bCloseIn(FALSE),
	m_bMouseMove(FALSE),
	m_bMouseMovePolygon(FALSE),
	m_lAudioHandle(-1),
    m_hRegisterDrawThread(NULL)
{
	//{{AFX_DATA_INIT(CDlgIPCSimpIntellCfg)
	m_bEnable = FALSE;
	m_bAudioAbnormal = FALSE;
	m_iAudioSen = "0";
	m_iAudioThreshold = "0";
	m_iMaxChan = 0;
	m_iUsedChan = 0;
	m_iLineSen = "0";
	m_iFieldDuration = "0";
	m_iFieldRate = "0";
	m_iFieldSen = "0";
	m_iCross = "0";
	m_iSceneSen = 0;
	m_bTraverseDualVca = FALSE;
	m_bFieldDetectionDualVca = FALSE;
	m_iDefocusSensortiveLevel = 0;
	m_bySensitivity = 0;
	m_bChkPTZAlarm = FALSE;
	//}}AFX_DATA_INIT
	memset(&m_struHolidayHandle, 0, sizeof(m_struHolidayHandle));
	memset(&m_struPtzLockCfg, 0, sizeof(m_struPtzLockCfg));
	memset(&m_struPtzLockInfoCond, 0, sizeof(m_struPtzLockInfoCond));
	memset(&m_struPtzLockInfo, 0, sizeof(m_struPtzLockInfo));
}


void CDlgIPCSimpIntellCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIPCSimpIntellCfg)
	DDX_Control(pDX, IDC_CMB_GROUPNO, m_cmbGroupNo);
	DDX_Control(pDX, IDC_COMBO_PTZ_LOCKSTATUS, m_comboPtzLockStatus);
	DDX_Control(pDX, IDC_COMBO_ENABLE, m_comEnable);
	DDX_Control(pDX, IDC_COMBO_REGION_ID, m_ComboRegionID);
	DDX_Control(pDX, IDC_COMBO_FIELDNO, m_Comnbo_FieldNo);
	DDX_Control(pDX, IDC_COMBO_LINENO, m_Combo_LineNo);
	DDX_Control(pDX, IDC_COMBO_CFG_TYPE, m_Combo_CfgType);
    DDX_Control(pDX, IDC_COMBO_CHANNEL, m_cmbChannel);
	DDX_Control(pDX, IDC_PLAYWND, m_wndPlay);
	DDX_Control(pDX, IDC_TREE_ALARM_OUT, m_treeAlarmOut);
	DDX_Control(pDX, IDC_TREE_CHAN, m_treeChan);
	DDX_Control(pDX, IDC_COMBO_ALARMIN_WEEKDAY, m_comboWeekday);
	DDX_Control(pDX, IDC_COMBO_ALARMIN_COPY_TIME, m_comboCopyTime);
	DDX_Check(pDX, IDC_CHK_ALARMIN_MONITOR, m_bChkMonitorAlarm);
	DDX_Check(pDX, IDC_CHK_ALARMIN_VOICE, m_bChkVoiceAlarm);
	DDX_Check(pDX, IDC_CHK_ALARMIN_UPLOAD_CENTER, m_bChkUploadCenter);
	DDX_Check(pDX, IDC_CHK_ALARMIN_INVOKE_ALARMOUT, m_bChkInvokeAlarmOut);
	DDX_Check(pDX, IDC_CHK_INVOKE_JPEG_CAPTURE, m_bChkInvokeJpegCapture);
	DDX_Check(pDX, IDC_CHK_ALARM_EMAP, m_bChkEmapAlarmOut);
	DDX_Check(pDX, IDC_CHK_ALARM_FOCUS, m_bChkFocusAlarm);	
	DDX_Text(pDX, IDC_EDIT_HOUR11, m_iHour11);
	DDX_Text(pDX, IDC_EDIT_MIN11, m_iMin11);
	DDX_Text(pDX, IDC_EDIT_HOUR12, m_iHour12);
	DDX_Text(pDX, IDC_EDIT_MIN12, m_iMin12);
	DDX_Text(pDX, IDC_EDIT_HOUR21, m_iHour21);
	DDX_Text(pDX, IDC_EDIT_MIN21, m_iMin21);
	DDX_Text(pDX, IDC_EDIT_HOUR22, m_iHour22);
	DDX_Text(pDX, IDC_EDIT_MIN22, m_iMin22);
	DDX_Text(pDX, IDC_EDIT_HOUR31, m_iHour31);
	DDX_Text(pDX, IDC_EDIT_MIN31, m_iMin31);
	DDX_Text(pDX, IDC_EDIT_HOUR32, m_iHour32);
	DDX_Text(pDX, IDC_EDIT_MIN32, m_iMin32);
	DDX_Text(pDX, IDC_EDIT_HOUR41, m_iHour41);
	DDX_Text(pDX, IDC_EDIT_MIN41, m_iMin41);
	DDX_Text(pDX, IDC_EDIT_HOUR42, m_iHour42);
	DDX_Text(pDX, IDC_EDIT_MIN42, m_iMin42);
	DDX_Text(pDX, IDC_EDIT_HOUR51, m_iHour51);
	DDX_Text(pDX, IDC_EDIT_MIN51, m_iMin51);
	DDX_Text(pDX, IDC_EDIT_HOUR52, m_iHour52);
	DDX_Text(pDX, IDC_EDIT_MIN52, m_iMin52);
	DDX_Text(pDX, IDC_EDIT_HOUR61, m_iHour61);
	DDX_Text(pDX, IDC_EDIT_MIN61, m_iMin61);
	DDX_Text(pDX, IDC_EDIT_HOUR62, m_iHour62);
	DDX_Text(pDX, IDC_EDIT_MIN62, m_iMin62);
	DDX_Text(pDX, IDC_EDIT_HOUR71, m_iHour71);
	DDX_Text(pDX, IDC_EDIT_MIN71, m_iMin71);
	DDX_Text(pDX, IDC_EDIT_HOUR72, m_iHour72);
	DDX_Text(pDX, IDC_EDIT_MIN72, m_iMin72);
	DDX_Text(pDX, IDC_EDIT_HOUR81, m_iHour81);
	DDX_Text(pDX, IDC_EDIT_MIN81, m_iMin81);
	DDX_Text(pDX, IDC_EDIT_HOUR82, m_iHour82);
	DDX_Text(pDX, IDC_EDIT_MIN82, m_iMin82);
	DDX_Check(pDX, IDC_CHK_PIC_FTP, m_bChkPicToFtp);
	DDX_Check(pDX, IDC_CHK_LINE_DETECTION, m_bEnable);
	DDX_Check(pDX, IDC_CHK_AUDIOABNORMAL, m_bAudioAbnormal);
	DDX_Text(pDX, IDC_EDT_AUDIOSEN, m_iAudioSen);
	DDX_Text(pDX, IDC_EDT_THRESHOLD, m_iAudioThreshold);
	DDX_Text(pDX, IDC_EDT_LINESEN, m_iLineSen);
	DDX_Text(pDX, IDC_EDT_FIELDDURATION, m_iFieldDuration);
	DDX_Text(pDX, IDC_EDT_FIELDRATE, m_iFieldRate);
	DDX_Text(pDX, IDC_EDT_FIELDSEN, m_iFieldSen);
	DDX_Text(pDX, IDC_EDT_CROSS, m_iCross);
	DDX_Text(pDX, IDC_EDT_SCENESEN, m_iSceneSen);
	DDX_Check(pDX, IDC_CHK_TRAVERSE_DUAL_VCA, m_bTraverseDualVca);
	DDX_Check(pDX, IDC_CHK_FIELD_DETECION_DUAL_VCA, m_bFieldDetectionDualVca);
	DDX_Text(pDX, IDC_EDIT_DEFOCUS_SENSORTIVE, m_iDefocusSensortiveLevel);
	DDX_Text(pDX, IDC_EDT_SENSITIVITY, m_bySensitivity);
	DDX_Check(pDX, IDC_CHK_ALARM_PTZ, m_bChkPTZAlarm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIPCSimpIntellCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgIPCSimpIntellCfg)
	ON_NOTIFY(NM_CLICK, IDC_TREE_CHAN, OnClickTreeChan)
	ON_BN_CLICKED(IDC_BTN_ALARM_IN_TIME_OK, OnBnClickedBtnAlarmInTimeOk)
	ON_CBN_SELCHANGE(IDC_COMBO_ALARMIN_WEEKDAY, OnCbnSelchangeComboAlarminWeekday)
	ON_BN_CLICKED(IDC_BTN_ALARMIN_TIME_COPY, OnBnClickedBtnAlarminTimeCopy)
	ON_BN_CLICKED(IDC_CHK_ALARMIN_INVOKE_ALARMOUT, OnBnClickedChkAlarminInvokeAlarmout)
	ON_NOTIFY(NM_CLICK, IDC_TREE_ALARM_OUT, OnClickTreeAlarmOut)
	ON_CBN_SELCHANGE(IDC_COMBO_CFG_TYPE, OnCfgTypeChanged)
    ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, OnChannelChanged)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SAVECFG, OnBtnSavecfg)
    ON_BN_CLICKED(IDC_BTN_CANCELANDEXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_REFRESH,OnBtnRefresh)
    ON_CBN_SELCHANGE(IDC_COMBO_LINENO, OnLineNoChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELDNO, OnFieldNoChanged)
	ON_BN_CLICKED(IDC_BTN_CLEARPIC, OnBtnClearpic)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CLICK, IDC_TREE_CHAN, OnClickTreeChan)
	ON_BN_CLICKED(IDC_BTN_PTZ_LOCKSTATUS_SET, OnBtnPtzLockstatusSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

DWORD  fRegisterDrawFun(LPVOID pParam)
{
    CDlgIPCSimpIntellCfg *pIPCSimpInterllCfg = (CDlgIPCSimpIntellCfg *)pParam;
    while (1)
    {
        
        if (!NET_DVR_RigisterDrawFun(pIPCSimpInterllCfg->m_lPlayHandle,  DrawVideoEpoliceAlertShow , (DWORD)pParam)) 
        {
            Sleep(100);
            continue;
            
        }
        else
        {
            
            break;
        }
    }
    return FALSE;
}

//************************************
// Method:    Play
// FullName:  CDlgIPCSimpIntellCfg::Play
// Access:    protected 
// Returns:   void
// Qualifier:
//************************************
void CDlgIPCSimpIntellCfg::Play()
{
	char szLan[128] = {0};
	char szTemp[256] = {0};
	NET_DVR_CLIENTINFO struPlayInfo; 
	memset(&struPlayInfo, 0, sizeof(NET_DVR_CLIENTINFO));
	
    struPlayInfo.hPlayWnd = GetDlgItem(IDC_PLAYWND)->GetSafeHwnd();

	struPlayInfo.lChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struPlayInfo.lLinkMode = 0;
	
    if (struPlayInfo.lChannel < 1)
    {
        struPlayInfo.lChannel = 1;
	}

	if (m_lPlayHandle >= 0)
	{
        NET_DVR_RigisterDrawFun(m_lPlayHandle,  NULL ,0);
		NET_DVR_StopRealPlay(m_lPlayHandle);
		m_lPlayHandle = -1;
	}
	
	m_lPlayHandle =  NET_DVR_RealPlay_V30(m_lLoginID, &struPlayInfo, NULL, NULL, FALSE); 

	if (m_lPlayHandle < 0)
	{
		g_StringLanType(szLan, "预览失败", "Realplay failed");
		sprintf(szTemp, "%s %s", szLan, NET_DVR_GetErrorMsg());
       // AfxMessageBox(szTemp);
		return ;
	}

//     while (1)
//     {
//         if (!NET_DVR_RigisterDrawFun(m_lPlayHandle, DrawVideoEpoliceAlertShow, (DWORD)this)) 
//         {
//             continue;
//         }
//         else
//         {
//             break;
//         }
//     }

    DWORD dwThreadId = 0;
    if (m_hRegisterDrawThread != NULL)
    {
        TerminateThread(m_hRegisterDrawThread, 0);
        CloseHandle(m_hRegisterDrawThread);
        m_hRegisterDrawThread = NULL;
    }
    
    if (m_hRegisterDrawThread == NULL)
    {
        m_hRegisterDrawThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(fRegisterDrawFun),this,0,&dwThreadId);
    }

   
	
// 	NET_DVR_RigisterDrawFun(m_lPlayHandle, DrawRoiDetectShow, 0);
	
}

BOOL CDlgIPCSimpIntellCfg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_PLAYWND)->GetWindowRect(&m_rcWnd);

	m_Combo_CfgType.SetCurSel(0);
	m_Combo_LineNo.SetCurSel(0);
	m_Comnbo_FieldNo.SetCurSel(0);
    
    //Add Channel Info 
    AddChanInfo();
    m_lChannel = m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
    AddGroupNoInfo();
	m_bCloseIn = TRUE;//Polygon is closed or not

	m_bMouseMove = FALSE;

	OnCfgTypeChanged();

//	Play();
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgIPCSimpIntellCfg message handlers

void CDlgIPCSimpIntellCfg::OnClickTreeChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	CPoint pt(0,0);
	CRect rc(0,0,0,0);
	GetCursorPos(&pt);
	GetDlgItem(IDC_TREE_CHAN)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	ScreenToClient(&pt);
	pt.x = pt.x - rc.left;
	pt.y = pt.y - rc.top;
	
	UINT uFlag = 0;
	HTREEITEM hSelect = m_treeChan.HitTest(pt, &uFlag);
	
	if (NULL == hSelect) 
	{
		return;
	}
	m_treeChan.SelectItem(hSelect);
	DWORD dwIndex = m_treeChan.GetItemData(hSelect)%1000;
	BOOL bCheck = m_treeChan.GetCheck(hSelect);
	m_bRecordChan[dwIndex - 1] = !bCheck;
	//switch checkbox status on click
	if (uFlag != LVHT_TOLEFT)
	{
		m_treeChan.SetCheck(hSelect, !bCheck);
	}
	else
	{
		m_treeChan.SetCheck(hSelect, bCheck);
	}
	
	*pResult = 0;
}

void CDlgIPCSimpIntellCfg::OnBnClickedBtnAlarmInTimeOk()
{
    UpdateData(TRUE);
    if (!TimeTest())
    {
        return;
    }
    int	i = m_comboWeekday.GetCurSel();	
    if (i == 7)
    {
        m_struHolidayHandle.struAlarmTime[0].byStartHour = (BYTE)m_iHour11;
        m_struHolidayHandle.struAlarmTime[0].byStartMin = (BYTE)m_iMin11;
        m_struHolidayHandle.struAlarmTime[0].byStopHour = (BYTE)m_iHour12;
        m_struHolidayHandle.struAlarmTime[0].byStopMin = (BYTE)m_iMin12;
        m_struHolidayHandle.struAlarmTime[1].byStartHour = (BYTE)m_iHour21;
        m_struHolidayHandle.struAlarmTime[1].byStartMin = (BYTE)m_iMin21;
        m_struHolidayHandle.struAlarmTime[1].byStopHour = (BYTE)m_iHour22;
        m_struHolidayHandle.struAlarmTime[1].byStopMin = (BYTE)m_iMin22;
        m_struHolidayHandle.struAlarmTime[2].byStartHour = (BYTE)m_iHour31;
        m_struHolidayHandle.struAlarmTime[2].byStartMin = (BYTE)m_iMin31;
        m_struHolidayHandle.struAlarmTime[2].byStopHour = (BYTE)m_iHour32;
        m_struHolidayHandle.struAlarmTime[2].byStopMin = (BYTE)m_iMin32;
        m_struHolidayHandle.struAlarmTime[3].byStartHour = (BYTE)m_iHour41;
        m_struHolidayHandle.struAlarmTime[3].byStartMin = (BYTE)m_iMin41;
        m_struHolidayHandle.struAlarmTime[3].byStopHour = (BYTE)m_iHour42;
        m_struHolidayHandle.struAlarmTime[3].byStopMin = (BYTE)m_iMin42;
        
        m_struHolidayHandle.struAlarmTime[4].byStartHour = (BYTE)m_iHour51;
        m_struHolidayHandle.struAlarmTime[4].byStartMin = (BYTE)m_iMin51;
        m_struHolidayHandle.struAlarmTime[4].byStopHour = (BYTE)m_iHour52;
        m_struHolidayHandle.struAlarmTime[4].byStopMin = (BYTE)m_iMin52;
        m_struHolidayHandle.struAlarmTime[5].byStartHour = (BYTE)m_iHour61;
        m_struHolidayHandle.struAlarmTime[5].byStartMin = (BYTE)m_iMin61;
        m_struHolidayHandle.struAlarmTime[5].byStopHour = (BYTE)m_iHour62;
        m_struHolidayHandle.struAlarmTime[5].byStopMin = (BYTE)m_iMin62;
        m_struHolidayHandle.struAlarmTime[6].byStartHour = (BYTE)m_iHour71;
        m_struHolidayHandle.struAlarmTime[6].byStartMin = (BYTE)m_iMin71;
        m_struHolidayHandle.struAlarmTime[6].byStopHour = (BYTE)m_iHour72;
        m_struHolidayHandle.struAlarmTime[6].byStopMin = (BYTE)m_iMin72;
        m_struHolidayHandle.struAlarmTime[7].byStartHour = (BYTE)m_iHour81;
        m_struHolidayHandle.struAlarmTime[7].byStartMin = (BYTE)m_iMin81;
        m_struHolidayHandle.struAlarmTime[7].byStopHour = (BYTE)m_iHour82;
        m_struHolidayHandle.struAlarmTime[7].byStopMin = (BYTE)m_iMin82;
        
    }
    else
    {
        m_struAlarmTime[i][0].byStartHour = (BYTE)m_iHour11;
        m_struAlarmTime[i][0].byStartMin = (BYTE)m_iMin11;
        m_struAlarmTime[i][0].byStopHour = (BYTE)m_iHour12;
        m_struAlarmTime[i][0].byStopMin = (BYTE)m_iMin12;
        m_struAlarmTime[i][1].byStartHour = (BYTE)m_iHour21;
        m_struAlarmTime[i][1].byStartMin = (BYTE)m_iMin21;
        m_struAlarmTime[i][1].byStopHour = (BYTE)m_iHour22;
        m_struAlarmTime[i][1].byStopMin = (BYTE)m_iMin22;
        m_struAlarmTime[i][2].byStartHour = (BYTE)m_iHour31;
        m_struAlarmTime[i][2].byStartMin = (BYTE)m_iMin31;
        m_struAlarmTime[i][2].byStopHour = (BYTE)m_iHour32;
        m_struAlarmTime[i][2].byStopMin = (BYTE)m_iMin32;
        m_struAlarmTime[i][3].byStartHour = (BYTE)m_iHour41;
        m_struAlarmTime[i][3].byStartMin = (BYTE)m_iMin41;
        m_struAlarmTime[i][3].byStopHour = (BYTE)m_iHour42;
        m_struAlarmTime[i][3].byStopMin = (BYTE)m_iMin42;
        m_struAlarmTime[i][4].byStartHour = (BYTE)m_iHour51;
        m_struAlarmTime[i][4].byStartMin = (BYTE)m_iMin51;
        m_struAlarmTime[i][4].byStopHour = (BYTE)m_iHour52;
        m_struAlarmTime[i][4].byStopMin = (BYTE)m_iMin52;
        m_struAlarmTime[i][5].byStartHour = (BYTE)m_iHour61;
        m_struAlarmTime[i][5].byStartMin = (BYTE)m_iMin61;
        m_struAlarmTime[i][5].byStopHour = (BYTE)m_iHour62;
        m_struAlarmTime[i][5].byStopMin = (BYTE)m_iMin62;
        m_struAlarmTime[i][6].byStartHour = (BYTE)m_iHour71;
        m_struAlarmTime[i][6].byStartMin = (BYTE)m_iMin71;
        m_struAlarmTime[i][6].byStopHour = (BYTE)m_iHour72;
        m_struAlarmTime[i][6].byStopMin = (BYTE)m_iMin72;
        m_struAlarmTime[i][7].byStartHour = (BYTE)m_iHour81;
        m_struAlarmTime[i][7].byStartMin = (BYTE)m_iMin81;
        m_struAlarmTime[i][7].byStopHour = (BYTE)m_iHour82;
        m_struAlarmTime[i][7].byStopMin = (BYTE)m_iMin82;
    }
	GetDlgItem(IDC_BTN_ALARMIN_TIME_COPY)->EnableWindow(TRUE);
}

BOOL CDlgIPCSimpIntellCfg::TimeTest()
{
	int i = 0;
	int j = 0;
	WORD wStartTime[MAX_TIMESEGMENT_V30], wStopTime[MAX_TIMESEGMENT_V30];
	char szLan[1024] = {0};
	UpdateData(TRUE);
	if ((m_iHour11 < 0) || (m_iHour11 > 24) || (m_iHour12 < 0) || (m_iHour12 > 24) ||	\
		(m_iHour21 < 0) || (m_iHour21 > 24) || (m_iHour22 < 0) || (m_iHour22 > 24) ||	\
		(m_iHour31 < 0) || (m_iHour31 > 24) || (m_iHour32 < 0) || (m_iHour32 > 24) ||	\
		(m_iHour41 < 0) || (m_iHour41 > 24) || (m_iHour42 < 0) || (m_iHour42 > 24) ||   \
		(m_iHour51 < 0) || (m_iHour51 > 24) || (m_iHour52 < 0) || (m_iHour52 > 24) ||	\
		(m_iHour61 < 0) || (m_iHour61 > 24) || (m_iHour62 < 0) || (m_iHour62 > 24) ||	\
		(m_iHour71 < 0) || (m_iHour71 > 24) || (m_iHour72 < 0) || (m_iHour72 > 24) ||	\
		(m_iHour81 < 0) || (m_iHour81 > 24) || (m_iHour82 < 0) || (m_iHour82 > 24))
	{
		g_StringLanType(szLan, "小时范围在0 -- 24之间", "Hour between 0 - 24 ");
		AfxMessageBox(szLan);
		return FALSE;
	}
	if ((m_iMin11 < 0) || (m_iMin11 > 60) || (m_iMin12 < 0) || (m_iMin12 > 60) ||	\
		(m_iMin21 < 0) || (m_iMin21 > 60) || (m_iMin22 < 0) || (m_iMin22 > 60) ||	\
		(m_iMin31 < 0) || (m_iMin31 > 60) || (m_iMin32 < 0) || (m_iMin32 > 60) ||	\
		(m_iMin41 < 0) || (m_iMin41 > 60) || (m_iMin42 < 0) || (m_iMin42 > 60) ||   \
		(m_iMin51 < 0) || (m_iMin51 > 60) || (m_iMin52 < 0) || (m_iMin52 > 60) ||	\
		(m_iMin61 < 0) || (m_iMin61 > 60) || (m_iMin62 < 0) || (m_iMin62 > 60) ||	\
		(m_iMin71 < 0) || (m_iMin71 > 60) || (m_iMin72 < 0) || (m_iMin72 > 60) ||	\
		(m_iMin81 < 0) || (m_iMin81 > 60) || (m_iMin82 < 0) || (m_iMin82 > 60))
	{
		g_StringLanType(szLan, "分钟范围在0 -- 60之间", "Minute between 0 - 60 ");
		AfxMessageBox(szLan);
		return FALSE;
	}
	wStartTime[0] = (WORD)(m_iHour11 * 100 + m_iMin11);
	wStartTime[1] = (WORD)(m_iHour21 * 100 + m_iMin21);
	wStartTime[2] = (WORD)(m_iHour31 * 100 + m_iMin31);
	wStartTime[3] = (WORD)(m_iHour41 * 100 + m_iMin41);
	wStopTime[0] = (WORD)(m_iHour12 * 100 + m_iMin12);
	wStopTime[1] = (WORD)(m_iHour22 * 100 + m_iMin22);
	wStopTime[2] = (WORD)(m_iHour32 * 100 + m_iMin32);
	wStopTime[3] = (WORD)(m_iHour42 * 100 + m_iMin42);
	wStartTime[4] = (WORD)(m_iHour51 * 100 + m_iMin51);
	wStartTime[5] = (WORD)(m_iHour61 * 100 + m_iMin61);
	wStartTime[6] = (WORD)(m_iHour71 * 100 + m_iMin71);
	wStartTime[7] = (WORD)(m_iHour81 * 100 + m_iMin81);
	wStopTime[4] = (WORD)(m_iHour52 * 100 + m_iMin52);
	wStopTime[5] = (WORD)(m_iHour62 * 100 + m_iMin62);
	wStopTime[6] = (WORD)(m_iHour72 * 100 + m_iMin72);
	wStopTime[7] = (WORD)(m_iHour82 * 100 + m_iMin82);
	for (i = 0; i < MAX_TIMESEGMENT_V30; i++)
	{
		if ((wStartTime[i] > 2400) || (wStopTime[i] > 2400))
		{
			g_StringLanType(szLan, "布防时间不能超过24:00", "Fortify time can not be more than 24:00");
			AfxMessageBox(szLan);
			return FALSE;
		}
		if (wStartTime[i] > wStopTime[i])
		{
			g_StringLanType(szLan, "布防起始时间不能大于停止时间", "Fortify start time can not be larger than the record to stop time");
			AfxMessageBox(szLan);
			return FALSE;
		}
		for (j = 0; j < MAX_TIMESEGMENT_V30; j++)
		{
			if (i == j)
			{
				continue;
			}
			if ((wStartTime[i] > wStartTime[j]) && (wStartTime[i] < wStopTime[j]) ||	\
				(wStopTime[i] > wStartTime[j]) && (wStopTime[i] < wStopTime[j]))
            {
				g_StringLanType(szLan, "时间段不能重复", "time can not be repeated");
				AfxMessageBox(szLan);
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CDlgIPCSimpIntellCfg::OnCbnSelchangeComboAlarminWeekday()
{
	UpdateData(TRUE);
	int	i = m_comboWeekday.GetCurSel();	
    if (i == 7)
    {
        m_iHour11 = m_struHolidayHandle.struAlarmTime[0].byStartHour;
        m_iMin11 = m_struHolidayHandle.struAlarmTime[0].byStartMin;
        m_iHour12 = m_struHolidayHandle.struAlarmTime[0].byStopHour;
        m_iMin12 = m_struHolidayHandle.struAlarmTime[0].byStopMin;
        m_iHour21 = m_struHolidayHandle.struAlarmTime[1].byStartHour;
        m_iMin21 = m_struHolidayHandle.struAlarmTime[1].byStartMin;
        m_iHour22 = m_struHolidayHandle.struAlarmTime[1].byStopHour;
        m_iMin22 = m_struHolidayHandle.struAlarmTime[1].byStopMin;
        m_iHour31 = m_struHolidayHandle.struAlarmTime[2].byStartHour;
        m_iMin31 = m_struHolidayHandle.struAlarmTime[2].byStartMin;
        m_iHour32 = m_struHolidayHandle.struAlarmTime[2].byStopHour;
        m_iMin32 = m_struHolidayHandle.struAlarmTime[2].byStopMin;
        m_iHour41 = m_struHolidayHandle.struAlarmTime[3].byStartHour;
        m_iMin41 = m_struHolidayHandle.struAlarmTime[3].byStartMin;
        m_iHour42 = m_struHolidayHandle.struAlarmTime[3].byStopHour;
        m_iMin42 = m_struHolidayHandle.struAlarmTime[3].byStopMin;
        m_iHour51 = m_struHolidayHandle.struAlarmTime[4].byStartHour;
        m_iMin51 = m_struHolidayHandle.struAlarmTime[4].byStartMin;
        m_iHour52 = m_struHolidayHandle.struAlarmTime[4].byStopHour;
        m_iMin52 = m_struHolidayHandle.struAlarmTime[4].byStopMin;
        m_iHour61 = m_struHolidayHandle.struAlarmTime[5].byStartHour;
        m_iMin61 = m_struHolidayHandle.struAlarmTime[5].byStartMin;
        m_iHour62 = m_struHolidayHandle.struAlarmTime[5].byStopHour;
        m_iMin62 = m_struHolidayHandle.struAlarmTime[5].byStopMin;
        m_iHour71 = m_struHolidayHandle.struAlarmTime[6].byStartHour;
        m_iMin71 = m_struHolidayHandle.struAlarmTime[6].byStartMin;
        m_iHour72 = m_struHolidayHandle.struAlarmTime[6].byStopHour;
        m_iMin72 = m_struHolidayHandle.struAlarmTime[6].byStopMin;
        m_iHour81 = m_struHolidayHandle.struAlarmTime[7].byStartHour;
        m_iMin81 = m_struHolidayHandle.struAlarmTime[7].byStartMin;
        m_iHour82 = m_struHolidayHandle.struAlarmTime[7].byStopHour;
        m_iMin82 = m_struHolidayHandle.struAlarmTime[7].byStopMin;
    }
    else
    {
        m_iHour11 = m_struAlarmTime[i][0].byStartHour;
        m_iMin11 = m_struAlarmTime[i][0].byStartMin;
        m_iHour12 =m_struAlarmTime[i][0].byStopHour;
        m_iMin12 = m_struAlarmTime[i][0].byStopMin;
        m_iHour21 = m_struAlarmTime[i][1].byStartHour;
        m_iMin21 = m_struAlarmTime[i][1].byStartMin;
        m_iHour22 = m_struAlarmTime[i][1].byStopHour;
        m_iMin22 = m_struAlarmTime[i][1].byStopMin;
        m_iHour31 = m_struAlarmTime[i][2].byStartHour;
        m_iMin31 = m_struAlarmTime[i][2].byStartMin;
        m_iHour32 = m_struAlarmTime[i][2].byStopHour;
        m_iMin32 = m_struAlarmTime[i][2].byStopMin;
        m_iHour41 = m_struAlarmTime[i][3].byStartHour;
        m_iMin41 = m_struAlarmTime[i][3].byStartMin;
        m_iHour42 = m_struAlarmTime[i][3].byStopHour;
        m_iMin42 = m_struAlarmTime[i][3].byStopMin;
        m_iHour51 = m_struAlarmTime[i][4].byStartHour;
        m_iMin51 = m_struAlarmTime[i][4].byStartMin;
        m_iHour52 = m_struAlarmTime[i][4].byStopHour;
        m_iMin52 = m_struAlarmTime[i][4].byStopMin;
        m_iHour61 = m_struAlarmTime[i][5].byStartHour;
        m_iMin61 = m_struAlarmTime[i][5].byStartMin;
        m_iHour62 = m_struAlarmTime[i][5].byStopHour;
        m_iMin62 = m_struAlarmTime[i][5].byStopMin;
        m_iHour71 = m_struAlarmTime[i][6].byStartHour;
        m_iMin71 = m_struAlarmTime[i][6].byStartMin;
        m_iHour72 = m_struAlarmTime[i][6].byStopHour;
        m_iMin72 = m_struAlarmTime[i][6].byStopMin;
        m_iHour81 = m_struAlarmTime[i][7].byStartHour;
        m_iMin81 = m_struAlarmTime[i][7].byStartMin;
        m_iHour82 = m_struAlarmTime[i][7].byStopHour;
    	m_iMin82 = m_struAlarmTime[i][7].byStopMin;
    }
	
	UpdateData(FALSE);
}

void CDlgIPCSimpIntellCfg::ShowData()
{
	UpdateData(TRUE);
	int i = 0;
	m_bChkMonitorAlarm = m_struAlarmHandleType.dwHandleType &0x01;
	m_bChkVoiceAlarm = (m_struAlarmHandleType.dwHandleType>>1)&0x01;
	m_bChkUploadCenter = (m_struAlarmHandleType.dwHandleType>>2)&0x01;
	m_bChkInvokeAlarmOut = (m_struAlarmHandleType.dwHandleType>>3)&0x01;
	m_bChkInvokeJpegCapture = (m_struAlarmHandleType.dwHandleType>>4)&0x01;
	m_bChkEmapAlarmOut = (m_struAlarmHandleType.dwHandleType>>6)&0x01;
	m_bChkPicToFtp = (m_struAlarmHandleType.dwHandleType>>9)&0x01;
	m_bChkFocusAlarm = (m_struAlarmHandleType.dwHandleType>>10)&0x01;
	m_bChkPTZAlarm = (m_struAlarmHandleType.dwHandleType>>11)&0x01;

	m_treeAlarmOut.EnableWindow(m_bChkInvokeAlarmOut);
   
    for (i = 0; i < m_struAlarmHandleType.dwRelAlarmOutChanNum; i++)
    {
        m_bAlarmOut[m_struAlarmHandleType.dwRelAlarmOut[i]-1] = 1;
    }
 
	//以下是音频异常专有
	m_bAudioAbnormal = m_struAudioException.struAudioAbnormal.byEnable;
	m_iAudioSen.Format("%d", m_struAudioException.struAudioAbnormal.bySensitivity);
	m_iAudioThreshold.Format("%d", m_struAudioException.struAudioAbnormal.byThreshold);
//	m_iAudioSen = m_struAudioException.struAudioAbnormal.bySensitivity;
	//m_iAudioThreshold = m_struAudioException.struAudioAbnormal.byThreshold;

	m_iLineSen.Format("%d", m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].bySensitivity);
//	m_iLineSen = m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].bySensitivity;
	m_iCross.Format("%d", m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].dwCrossDirection);
//	m_iCross = m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].dwCrossDirection;
	
	//区域入侵
	m_iFieldSen.Format("%d", m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].bySensitivity);
//	m_iFieldSen = m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].bySensitivity;
	m_iFieldRate.Format("%d", m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].byRate);
//	m_iFieldRate = m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].byRate;
	m_iFieldDuration.Format("%d", m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].wDuration);
//	m_iFieldDuration = m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].wDuration;

	//2013-07-17 场景更变侦测
	m_iSceneSen = m_struSceneDetection.bySensitiveLevel;

	CreateTree();
	UpdateData(FALSE);
	m_comboWeekday.SetCurSel(0);
	m_comboCopyTime.SetCurSel(0);
	OnCbnSelchangeComboAlarminWeekday();
//	GetDlgItem(IDC_BTN_ALARMIN_OK)->EnableWindow(TRUE);	

	if (m_lPlayHandle  < 0)
	{
		Play();
	}

	if (m_lAudioHandle < 0)
	{
		NET_DVR_INPUTVOLUME struInputVolume = {0};
		struInputVolume.dwSize = sizeof(NET_DVR_INPUTVOLUME);
		struInputVolume.byAudioInputChan = m_lChannel;
        m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	 	m_lAudioHandle = NET_DVR_StartRemoteConfig(m_lLoginID, NET_DVR_START_GET_INPUTVOLUME, &struInputVolume, sizeof(struInputVolume), AudioDecibelCB, this);
	}

	return;
}

void CDlgIPCSimpIntellCfg::OnBnClickedBtnAlarminTimeCopy()
{
	UpdateData(TRUE);
	int j,k;
	int i = m_comboWeekday.GetCurSel();
	int m_nCopyTime = m_comboCopyTime.GetCurSel() - 1;
	
	if (m_nCopyTime == -1)
	{
		for (j=0; j<MAX_DAYS; j++)
		{
			if (j == i)
			{
				continue;
			}
			for (k=0; k<MAX_TIMESEGMENT_V30; k++)
			{
				memcpy(&(m_struAlarmTime[j][k]), &(m_struAlarmTime[i][k]), sizeof(NET_DVR_SCHEDTIME));
			}
		}
	}
    if (m_nCopyTime == 7) // 假日情况
    {
        for (k = 0; k < MAX_TIMESEGMENT_V30; k++)
        {
            memcpy(&m_struHolidayHandle.struAlarmTime[k], &(m_struAlarmTime[i][k]), sizeof(NET_DVR_SCHEDTIME));
        }
//         if (!SetMotionHolidayHandle(m_struHolidayHandle))
//         {
//             AfxMessageBox("Fail to set motion holiday handle");
//         }
    }
	else
	{
		for (k=0; k<MAX_TIMESEGMENT_V30; k++)
		{
			memcpy(&(m_struAlarmTime[m_nCopyTime][k]), &(m_struAlarmTime[i][k]), sizeof(NET_DVR_SCHEDTIME));
		}
	}
}

void CDlgIPCSimpIntellCfg::OnBnClickedChkAlarminInvokeAlarmout()
{
	UpdateData(TRUE);
	m_treeAlarmOut.EnableWindow(m_bChkInvokeAlarmOut);
}

void CDlgIPCSimpIntellCfg::OnClickTreeAlarmOut( NMHDR* pNMHDR, LRESULT* pResult )
{
	UpdateData(TRUE);
	CPoint pt(0,0);
	CRect rc(0,0,0,0);
	GetCursorPos(&pt);
	GetDlgItem(IDC_TREE_ALARM_OUT)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	ScreenToClient(&pt);
	pt.x = pt.x - rc.left;
	pt.y = pt.y - rc.top;
	
	UINT uFlag = 0;
	HTREEITEM hSelect = m_treeAlarmOut.HitTest(pt, &uFlag);
	
	if (NULL == hSelect) 
	{
		return;
	}
	m_treeAlarmOut.SelectItem(hSelect);
	DWORD dwIndex = m_treeAlarmOut.GetItemData(hSelect)%1000;
	BOOL bCheck = m_treeAlarmOut.GetCheck(hSelect);
	m_bAlarmOut[dwIndex] = !bCheck;
	//switch checkbox status on click
	if (uFlag != LVHT_TOLEFT)
	{
		m_treeAlarmOut.SetCheck(hSelect, !bCheck);
	}
	else
	{
		m_treeAlarmOut.SetCheck(hSelect, bCheck);
	}
	
	*pResult = 0;
}

void CDlgIPCSimpIntellCfg::CreateTree()
{
	m_treeChan.DeleteAllItems();
	CString strTemp =_T("");
	CString strChanTmp = _T("");
	int i = 0;
	HTREEITEM hChanItem = NULL;
	if (m_iChanCount <= 0)
	{
		m_treeAlarmOut.EnableWindow(FALSE);
		m_treeChan.EnableWindow(FALSE);
	}
	
	int iChanShow = 0;
	
    for (i = 0; i < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; i++)
    {

        hChanItem =  m_treeChan.InsertItem(g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].chChanName, 0, 0, TVI_ROOT);
        m_treeChan.SetItemData(hChanItem, 0*1000 + g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].iChannelNO);	
        if (m_bRecordChan[g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].iChannelNO -1])
        {
            m_treeChan.SetCheck(hChanItem, TRUE);
        }
	}

/*	for (i = 0; i < g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum; i++)
	{
		iChanShow = g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].iChannelNO;
		
		if ((i < m_iAnaChanCount) && g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].bEnable)
		{
			strTemp.Format(ANALOG_C_FORMAT, iChanShow);
			hChanItem =  m_treeChan.InsertItem(strTemp, 0, 0, TVI_ROOT);
			m_treeChan.SetItemData(hChanItem, 0*1000 + i);	
			if (m_bRecordChan[iChanShow -1])
			{
				m_treeChan.SetCheck(hChanItem, TRUE);
			}
		}
		else if (i >= m_iAnaChanCount && g_struDeviceInfo[m_iDeviceIndex].pStruChanInfo[i].bEnable)
		{
			strTemp.Format(DIGITAL_C_FORMAT, iChanShow - m_iDStartChannel + 1);
			hChanItem =  m_treeChan.InsertItem(strTemp, 0, 0, TVI_ROOT);
			m_treeChan.SetItemData(hChanItem, 1*1000 + i);	
			if (m_bRecordChan[iChanShow - 1])
			{
				m_treeChan.SetCheck(hChanItem, TRUE);
			}
		}
	}
*/
    m_treeChan.SelectItem(hChanItem);
	m_treeChan.Expand(m_treeChan.GetRootItem(),TVE_EXPAND);	

	UpdateWindow();
	
	
	g_CreateAlarmOutTree(m_treeAlarmOut, m_iDeviceIndex, m_bAlarmOut);
}

void CDlgIPCSimpIntellCfg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CreateTree();
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgIPCSimpIntellCfg::OnCfgTypeChanged()
{
    memset(&m_struLineDetection, 0, sizeof(m_struLineDetection));
    memset(&m_struFieldDetection, 0, sizeof(m_struFieldDetection));
    memset(&m_struDefousDetection, 0, sizeof(m_struDefousDetection));
    memset(&m_struAudioException, 0, sizeof(m_struAudioException));
    memset(&m_struSceneDetection, 0, sizeof(m_struSceneDetection));//场景变更侦测2013-07-17
    memset(&m_bRecordChan, 0,sizeof(m_bRecordChan));
    memset(&m_bAlarmOut, 0 , sizeof(m_bAlarmOut));
    memset(m_struHolidayHandle.struAlarmTime, 0, sizeof(NET_DVR_SCHEDTIME)*MAX_TIMESEGMENT_V30);
	int i = m_Combo_CfgType.GetCurSel();
	m_lChannel = m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
    if (i == 0)  
	{
		GetLineDetection();
		//
// 		for (int k = 0; k < MAX_CHANNUM_V30; k++)
// 		{
// 			m_bRecordChan[k] = m_struLineDetection.byRelRecordChan[k];
// 		}
        for (int k = 0; k < m_struLineDetection.dwRelRecordChanNum; k++)
        {
            if (m_struLineDetection.byRelRecordChan[k] != 0xffffffff)
            {
                m_bRecordChan[m_struLineDetection.byRelRecordChan[k]-1] = 1;
            }
        }
		memcpy(m_struAlarmTime, m_struLineDetection.struAlarmSched, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);
        memcpy(m_struHolidayHandle.struAlarmTime, m_struLineDetection.struHolidayTime, sizeof(NET_DVR_SCHEDTIME)*MAX_TIMESEGMENT_V30);
        m_struAlarmHandleType = m_struLineDetection.struHandleException;
		m_bEnable = m_struLineDetection.byEnable;
		m_bTraverseDualVca = m_struLineDetection.byEnableDualVca;
	}
	else if(i == 1) //当前配置区域入侵侦测
	{
		GetFieldDetection();
		//
		for (int k = 0; k < m_struFieldDetection.dwRelRecordChanNum; k++)
		{
            if(m_struFieldDetection.byRelRecordChan[k] != 0xffffffff)
            {
                m_bRecordChan[m_struFieldDetection.byRelRecordChan[k]-1] = 1;
            }
			
		}
 

		memcpy(m_struAlarmTime, m_struFieldDetection.struAlarmSched, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);
        memcpy(m_struHolidayHandle.struAlarmTime, m_struFieldDetection.struHolidayTime, sizeof(NET_DVR_SCHEDTIME)*MAX_TIMESEGMENT_V30);
		m_struAlarmHandleType = m_struFieldDetection.struHandleException;
		m_bEnable = m_struFieldDetection.byEnable;
		m_bFieldDetectionDualVca = m_struFieldDetection.byEnableDualVca;
	}
	else if(i == 2) //当前配置虚焦侦测
	{
		GetDefousDetection();
         
		m_struAlarmHandleType = m_struDefousDetection.struHandleException;		
		m_bEnable = m_struDefousDetection.byEnable;
		m_iDefocusSensortiveLevel = m_struDefousDetection.bySensitiveLevel;
	}
	else if (i == 3)
	{
		GetAudioException();
		m_bEnable = m_struAudioException.byEnableAudioInException;
		memcpy(m_struAlarmTime, m_struAudioException.struAlarmSched, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);
		m_struAlarmHandleType = m_struAudioException.struHandleException;
		//2014-03-21
		m_bySensitivity = m_struAudioException.struAudioSteepDrop.bySensitivity; 
		m_comEnable.SetCurSel(m_struAudioException.struAudioSteepDrop.byEnable); 
		for (int k = 0; k < m_struAudioException.dwRelRecordChanNum; k++)
		{
            if (m_struAudioException.byRelRecordChan[k] != 0xff)
            {
                m_bRecordChan[m_struAudioException.byRelRecordChan[k]-1]  =1;
            }
		}

	}
	else if (i == 4)//场景变更侦测2013-07-17
	{
		GetSceneDetection();
		m_bEnable = m_struSceneDetection.byEnable;
		memcpy(m_struAlarmTime, m_struSceneDetection.struAlarmSched, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);
		m_struAlarmHandleType = m_struSceneDetection.struHandleException;
		for (int k = 0; k < m_struSceneDetection.dwRelRecordChanNum; k++)
		{
            if ( m_struSceneDetection.dwRelRecordChan[k] != 0xffffffff)
            {
                m_bRecordChan[ m_struSceneDetection.dwRelRecordChan[k]-1] = 1;
            }			
		}
	}

	//GetDlgItem(IDC_CHK_LINE_DETECTION)->UpdateData(FALSE);
	UpdateData(FALSE);
	ShowData();
}

void CDlgIPCSimpIntellCfg::GetLineDetection()
{
	UpdateData(TRUE);
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struLineDetectionCond = {0};
	struLineDetectionCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struLineDetectionCond.dwGroup = m_cmbGroupNo.GetItemData(m_cmbGroupNo.GetCurSel());
	struLineDetectionCond.byID = m_Combo_LineNo.GetCurSel()+1/*m_ComboRegionID.GetCurSel()+1*/;
	struLineDetectionCond.dwSize = sizeof(struLineDetectionCond);
	int pStatus = 0;
    char szLan[128] = {0};
	BOOL bRet = NET_DVR_GetDeviceConfig(m_lLoginID, NET_DVR_GET_TRAVERSE_PLANE_DETECTION, iCount, &struLineDetectionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struLineDetection, sizeof(m_struLineDetection));
	if (bRet && pStatus == NET_DVR_NOERROR)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_TRAVERSE_PLANE_DETECTION");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);

        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_TRAVERSE_PLANE_DETECTION");
        return;
    }
}

void CDlgIPCSimpIntellCfg::SaveLineDetection()
{
	UpdateData(TRUE);
	int iCount = 0;
    char szLan[128] = {0};
	NET_DVR_CHANNEL_GROUP struLineDetectionCond = {0};
	struLineDetectionCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struLineDetectionCond.dwGroup = m_cmbGroupNo.GetItemData(m_cmbGroupNo.GetCurSel());
	struLineDetectionCond.byID = m_Combo_LineNo.GetCurSel()+1/*m_ComboRegionID.GetCurSel()+1*/;
	struLineDetectionCond.dwSize = sizeof(struLineDetectionCond);
	int pStatus = 0;
	m_struLineDetection.dwSize = sizeof(NET_VCA_TRAVERSE_PLANE_DETECTION);
	BOOL bRet = NET_DVR_SetDeviceConfig(m_lLoginID, NET_DVR_SET_TRAVERSE_PLANE_DETECTION, iCount, &struLineDetectionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struLineDetection, sizeof(m_struLineDetection));
	if (bRet && pStatus == NET_DVR_NOERROR)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_TRAVERSE_PLANE_DETECTION");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_TRAVERSE_PLANE_DETECTION");
    }
}

void CDlgIPCSimpIntellCfg::GetFieldDetection()
{
	UpdateData(TRUE);
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struFielDetectionCond = {0};
	struFielDetectionCond.dwChannel = m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struFielDetectionCond.dwGroup = m_cmbGroupNo.GetItemData(m_cmbGroupNo.GetCurSel());
	struFielDetectionCond.byID = m_Comnbo_FieldNo.GetCurSel() + 1/*m_ComboRegionID.GetCurSel()+1*/;
	struFielDetectionCond.dwSize = sizeof(struFielDetectionCond);
	int pStatus = 0;
    char szLan[128] = {0};
	BOOL bRet = NET_DVR_GetDeviceConfig(m_lLoginID, NET_DVR_GET_FIELD_DETECTION, iCount, &struFielDetectionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struFieldDetection, sizeof(m_struFieldDetection));
	if (bRet && pStatus == NET_DVR_NOERROR)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_FIELD_DETECTION");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_FIELD_DETECTION");
    }
}


void CDlgIPCSimpIntellCfg::SaveFieldDetection()
{
	UpdateData(TRUE);
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struFieldDetectionCond = {0};
	struFieldDetectionCond.dwChannel = m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struFieldDetectionCond.dwGroup = m_cmbGroupNo.GetItemData(m_cmbGroupNo.GetCurSel());
	struFieldDetectionCond.byID = m_Comnbo_FieldNo.GetCurSel() + 1/*m_ComboRegionID.GetCurSel()+1*/;
	struFieldDetectionCond.dwSize = sizeof(struFieldDetectionCond);
	int pStatus = 0;
	m_struFieldDetection.dwSize = sizeof(m_struFieldDetection);
    char szLan[128] = {0};
    BOOL bRet = NET_DVR_SetDeviceConfig(m_lLoginID, NET_DVR_SET_FIELD_DETECTION, iCount, &struFieldDetectionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struFieldDetection, sizeof(m_struFieldDetection));
	if (bRet && pStatus == NET_DVR_NOERROR)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_FIELD_DETECTION");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_FIELD_DETECTION");
    }
}

void CDlgIPCSimpIntellCfg::GetDefousDetection()
{
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struDefousCond = {0};
	struDefousCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struDefousCond.dwGroup = 1;
	struDefousCond.dwSize = sizeof(struDefousCond);
	int pStatus = 0;
    char szLan[128] = {0};
	m_struDefousDetection.dwSize = sizeof(NET_VCA_DEFOCUSPARAM);
	BOOL bRet = NET_DVR_GetDeviceConfig(m_lLoginID, NET_DVR_GET_DEFOCUSPARAM, iCount, &struDefousCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struDefousDetection, sizeof(m_struDefousDetection));
	if (bRet && pStatus == 0)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_DEFOCUSPARAM");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_DEFOCUSPARAM");
    }
}

void CDlgIPCSimpIntellCfg::SaveDefousDetection()
{
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struDefousCond = {0};
	struDefousCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struDefousCond.dwGroup = 0;
	struDefousCond.dwSize = sizeof(struDefousCond);
	m_struDefousDetection.dwSize = sizeof(m_struDefousDetection);
	int pStatus = 0;
    char szLan[128] = {0};
	BOOL bRet = NET_DVR_SetDeviceConfig(m_lLoginID, NET_DVR_SET_DEFOCUSPARAM, iCount, &struDefousCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struDefousDetection, sizeof(m_struDefousDetection));
	if (bRet && pStatus == 0)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_DEFOCUSPARAM");
    }
    else
    {
       sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);;
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_DEFOCUSPARAM");
    }
}

void CDlgIPCSimpIntellCfg::OnBtnSavecfg() 
{
	SaveUIData();
	int i = m_Combo_CfgType.GetCurSel();
	// TODO: Add your control notification handler code here
	if (i == 0)
	{
		m_struLineDetection.byEnable = m_bEnable;
		m_struLineDetection.byEnableDualVca = m_bTraverseDualVca;
		memcpy(m_struLineDetection.struAlarmSched, m_struAlarmTime, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);
        memcpy(m_struLineDetection.struHolidayTime, m_struHolidayHandle.struAlarmTime, sizeof(NET_DVR_SCHEDTIME)*MAX_TIMESEGMENT_V30);
		m_struLineDetection.dwRelRecordChanNum = 0;
        memset(m_struLineDetection.byRelRecordChan, 0xffffffff, sizeof(m_struLineDetection.byRelRecordChan));
        for (int k = 0; k < MAX_CHANNUM_V30*8; k++)
		{
            if(m_bRecordChan[k])
            {
                m_struLineDetection.byRelRecordChan[m_struLineDetection.dwRelRecordChanNum++] = k +1;
            }
		}

		m_struLineDetection.struHandleException = m_struAlarmHandleType;	
		
		SaveLineDetection();
	}
	else if (i == 1)
	{
        m_struFieldDetection.dwRelRecordChanNum = 0;
        memset(m_struFieldDetection.byRelRecordChan, 0xffffffff, sizeof(m_struFieldDetection.byRelRecordChan));
        for (int k = 0; k < MAX_CHANNUM_V30*8; k++)
        {
            if(m_bRecordChan[k])
            {
                m_struFieldDetection.byRelRecordChan[m_struFieldDetection.dwRelRecordChanNum++] = k +1;
            }
        }


		memcpy(m_struFieldDetection.struAlarmSched, m_struAlarmTime, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);
        memcpy(m_struFieldDetection.struHolidayTime, m_struHolidayHandle.struAlarmTime, sizeof(NET_DVR_SCHEDTIME)*MAX_TIMESEGMENT_V30);

		m_struFieldDetection.struHandleException = m_struAlarmHandleType;
		m_struFieldDetection.byEnable = m_bEnable;

		m_struFieldDetection.byEnableDualVca = m_bFieldDetectionDualVca;
		SaveFieldDetection();
	}
	else if (i == 2)
	{
		m_struDefousDetection.byEnable = m_bEnable;
		m_struDefousDetection.struHandleException = m_struAlarmHandleType;
		m_struDefousDetection.bySensitiveLevel = m_iDefocusSensortiveLevel;
		SaveDefousDetection();
	}
	else if (i == 3)
	{
		m_struAudioException.byEnableAudioInException = m_bEnable;
		memcpy(m_struAudioException.struAlarmSched, m_struAlarmTime, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);

        m_struAudioException.dwRelRecordChanNum = 0;
        memset(m_struAudioException.byRelRecordChan, 0xffffffff, sizeof(m_struAudioException.byRelRecordChan));
        for (int k = 0; k < MAX_CHANNUM_V30*8; k++)
        {
            if(m_bRecordChan[k])
            {
                m_struAudioException.byRelRecordChan[m_struAudioException.dwRelRecordChanNum++] = k +1;
            }
        }

		m_struAudioException.struHandleException = m_struAlarmHandleType;
		
		//2014-03-21
		m_struAudioException.struAudioSteepDrop.bySensitivity = m_bySensitivity; 
		m_struAudioException.struAudioSteepDrop.byEnable = m_comEnable.GetCurSel(); 
		
		SaveAudioException();
	}
	else if (i == 4)//场景变更侦测2013-07-17
	{
		m_struSceneDetection.byEnable = m_bEnable;
		memcpy(m_struSceneDetection.struAlarmSched, m_struAlarmTime, sizeof(NET_DVR_SCHEDTIME) * MAX_DAYS * MAX_TIMESEGMENT_V30);
		for (int k = 0; k < MAX_CHANNUM_V30; k++)
		{
			m_struSceneDetection.dwRelRecordChan[k] = m_bRecordChan[k] +1;
		}
		m_struSceneDetection.struHandleException = m_struAlarmHandleType;
		SaveSceneDetection();
	}
}

void CDlgIPCSimpIntellCfg::SaveUIData()
{
	UpdateData(TRUE);
	if (!TimeTest())
	{
		return;
	}
	int	i = m_comboWeekday.GetCurSel();	
    if (i == 7)
    {
        m_struHolidayHandle.struAlarmTime[0].byStartHour = (BYTE)m_iHour11;
        m_struHolidayHandle.struAlarmTime[0].byStartMin = (BYTE)m_iMin11;
        m_struHolidayHandle.struAlarmTime[0].byStopHour = (BYTE)m_iHour12;
        m_struHolidayHandle.struAlarmTime[0].byStopMin = (BYTE)m_iMin12;
        m_struHolidayHandle.struAlarmTime[1].byStartHour = (BYTE)m_iHour21;
        m_struHolidayHandle.struAlarmTime[1].byStartMin = (BYTE)m_iMin21;
        m_struHolidayHandle.struAlarmTime[1].byStopHour = (BYTE)m_iHour22;
        m_struHolidayHandle.struAlarmTime[1].byStopMin = (BYTE)m_iMin22;
        m_struHolidayHandle.struAlarmTime[2].byStartHour = (BYTE)m_iHour31;
        m_struHolidayHandle.struAlarmTime[2].byStartMin = (BYTE)m_iMin31;
        m_struHolidayHandle.struAlarmTime[2].byStopHour = (BYTE)m_iHour32;
        m_struHolidayHandle.struAlarmTime[2].byStopMin = (BYTE)m_iMin32;
        m_struHolidayHandle.struAlarmTime[3].byStartHour = (BYTE)m_iHour41;
        m_struHolidayHandle.struAlarmTime[3].byStartMin = (BYTE)m_iMin41;
        m_struHolidayHandle.struAlarmTime[3].byStopHour = (BYTE)m_iHour42;
        m_struHolidayHandle.struAlarmTime[3].byStopMin = (BYTE)m_iMin42;
        
        m_struHolidayHandle.struAlarmTime[4].byStartHour = (BYTE)m_iHour51;
        m_struHolidayHandle.struAlarmTime[4].byStartMin = (BYTE)m_iMin51;
        m_struHolidayHandle.struAlarmTime[4].byStopHour = (BYTE)m_iHour52;
        m_struHolidayHandle.struAlarmTime[4].byStopMin = (BYTE)m_iMin52;
        m_struHolidayHandle.struAlarmTime[5].byStartHour = (BYTE)m_iHour61;
        m_struHolidayHandle.struAlarmTime[5].byStartMin = (BYTE)m_iMin61;
        m_struHolidayHandle.struAlarmTime[5].byStopHour = (BYTE)m_iHour62;
        m_struHolidayHandle.struAlarmTime[5].byStopMin = (BYTE)m_iMin62;
        m_struHolidayHandle.struAlarmTime[6].byStartHour = (BYTE)m_iHour71;
        m_struHolidayHandle.struAlarmTime[6].byStartMin = (BYTE)m_iMin71;
        m_struHolidayHandle.struAlarmTime[6].byStopHour = (BYTE)m_iHour72;
        m_struHolidayHandle.struAlarmTime[6].byStopMin = (BYTE)m_iMin72;
        m_struHolidayHandle.struAlarmTime[7].byStartHour = (BYTE)m_iHour81;
        m_struHolidayHandle.struAlarmTime[7].byStartMin = (BYTE)m_iMin81;
        m_struHolidayHandle.struAlarmTime[7].byStopHour = (BYTE)m_iHour82;
        m_struHolidayHandle.struAlarmTime[7].byStopMin = (BYTE)m_iMin82;
    }
    else
    {
        m_struAlarmTime[i][0].byStartHour = (BYTE)m_iHour11;
        m_struAlarmTime[i][0].byStartMin = (BYTE)m_iMin11;
        m_struAlarmTime[i][0].byStopHour = (BYTE)m_iHour12;
        m_struAlarmTime[i][0].byStopMin = (BYTE)m_iMin12;
        m_struAlarmTime[i][1].byStartHour = (BYTE)m_iHour21;
        m_struAlarmTime[i][1].byStartMin = (BYTE)m_iMin21;
        m_struAlarmTime[i][1].byStopHour = (BYTE)m_iHour22;
        m_struAlarmTime[i][1].byStopMin = (BYTE)m_iMin22;
        m_struAlarmTime[i][2].byStartHour = (BYTE)m_iHour31;
        m_struAlarmTime[i][2].byStartMin = (BYTE)m_iMin31;
        m_struAlarmTime[i][2].byStopHour = (BYTE)m_iHour32;
        m_struAlarmTime[i][2].byStopMin = (BYTE)m_iMin32;
        m_struAlarmTime[i][3].byStartHour = (BYTE)m_iHour41;
        m_struAlarmTime[i][3].byStartMin = (BYTE)m_iMin41;
        m_struAlarmTime[i][3].byStopHour = (BYTE)m_iHour42;
        m_struAlarmTime[i][3].byStopMin = (BYTE)m_iMin42;
        
        m_struAlarmTime[i][4].byStartHour = (BYTE)m_iHour51;
        m_struAlarmTime[i][4].byStartMin = (BYTE)m_iMin51;
        m_struAlarmTime[i][4].byStopHour = (BYTE)m_iHour52;
        m_struAlarmTime[i][4].byStopMin = (BYTE)m_iMin52;
        m_struAlarmTime[i][5].byStartHour = (BYTE)m_iHour61;
        m_struAlarmTime[i][5].byStartMin = (BYTE)m_iMin61;
        m_struAlarmTime[i][5].byStopHour = (BYTE)m_iHour62;
        m_struAlarmTime[i][5].byStopMin = (BYTE)m_iMin62;
        m_struAlarmTime[i][6].byStartHour = (BYTE)m_iHour71;
        m_struAlarmTime[i][6].byStartMin = (BYTE)m_iMin71;
        m_struAlarmTime[i][6].byStopHour = (BYTE)m_iHour72;
        m_struAlarmTime[i][6].byStopMin = (BYTE)m_iMin72;
        m_struAlarmTime[i][7].byStartHour = (BYTE)m_iHour81;
        m_struAlarmTime[i][7].byStartMin = (BYTE)m_iMin81;
        m_struAlarmTime[i][7].byStopHour = (BYTE)m_iHour82;
    	m_struAlarmTime[i][7].byStopMin = (BYTE)m_iMin82;
    }

	
// 	for (i=0; i<MAX_CHANNUM_V30; i++)
// 	{
// 		m_lpAlarmIn->byRelRecordChan[i] = (BYTE)m_bRecordChan[i];
// 	}
	
	m_struAlarmHandleType.dwHandleType = 0;
	m_struAlarmHandleType.dwHandleType |= (m_bChkMonitorAlarm << 0);
	m_struAlarmHandleType.dwHandleType |= (m_bChkVoiceAlarm << 1);
	m_struAlarmHandleType.dwHandleType |= (m_bChkUploadCenter << 2);
	m_struAlarmHandleType.dwHandleType |= (m_bChkInvokeAlarmOut << 3);
	m_struAlarmHandleType.dwHandleType |= (m_bChkInvokeJpegCapture << 4);
	m_struAlarmHandleType.dwHandleType |= (m_bChkEmapAlarmOut<<6);
	m_struAlarmHandleType.dwHandleType |= (m_bChkPicToFtp << 9);
	m_struAlarmHandleType.dwHandleType |= (m_bChkFocusAlarm << 10);
	m_struAlarmHandleType.dwHandleType |= (m_bChkPTZAlarm << 11);
	int iAlarmOutCount = 0;
    m_struAlarmHandleType.dwRelAlarmOutChanNum = 0;
    memset(m_struAlarmHandleType.dwRelAlarmOut, 0xffffffff, sizeof(m_struAlarmHandleType.dwRelAlarmOut));
	for (i=0; i < MAX_ALARMOUT_V30; i++)
	{

		if (m_bAlarmOut[i])
		{
			m_struAlarmHandleType.dwRelAlarmOut[m_struAlarmHandleType.dwRelAlarmOutChanNum++] = i +1;
		}
	}
	 


// 	int iRecordCount = 0;
// 	for (int k = 0; k < MAX_CHANNUM_V30; k++)
// 	{
// 		if (m_bRecordChan[k])
// 		{
// 			iRecordCount++;
// 		}
// 		else
// 		{
// 			break; //是紧凑排列的只要找到空的
// 		}
// 	}

	//以下是音频异常专有
	m_struAudioException.struAudioAbnormal.byEnable = m_bAudioAbnormal;
	m_struAudioException.struAudioAbnormal.bySensitivity = atoi(m_iAudioSen);
	m_struAudioException.struAudioAbnormal.byThreshold = atoi(m_iAudioThreshold);
	//m_struAudioException.dwRelRecordChanNum = iRecordCount;

	m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].bySensitivity = atoi(m_iLineSen);
	m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].dwCrossDirection = (VCA_CROSS_DIRECTION)atoi(m_iCross);
	//m_struLineDetection.dwRelRecordChanNum = iRecordCount;
	
	//区域入侵
	m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].bySensitivity = atoi(m_iFieldSen);
	m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].byRate = atoi(m_iFieldRate);
	m_struFieldDetection.struIntrusion[m_Comnbo_FieldNo.GetCurSel()].wDuration = atoi(m_iFieldDuration);
	//m_struFieldDetection.dwRelRecordChanNum = iRecordCount;

	//场景变更侦测2013-07-17
	m_struSceneDetection.bySensitiveLevel = m_iSceneSen;
	//m_struSceneDetection.dwRelRecordChanNum = iRecordCount;
}

//************************************
// Method:    GetAudioException
// FullName:  CDlgIPCSimpIntellCfg::GetAudioException
// Access:    protected 
// Returns:   void
// Qualifier:
//************************************
void CDlgIPCSimpIntellCfg::GetAudioException()
{
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struAudioDetectionCond = {0};
	struAudioDetectionCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struAudioDetectionCond.dwGroup = 1;
	struAudioDetectionCond.dwSize = sizeof(struAudioDetectionCond);
	int pStatus = 0;
    char szLan[128] = {0};
	BOOL bRet = NET_DVR_GetDeviceConfig(m_lLoginID, NET_DVR_GET_AUDIOEXCEPTIONPARAM, iCount, &struAudioDetectionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struAudioException, sizeof(m_struAudioException));
	if (bRet && pStatus==0)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_AUDIOEXCEPTIONPARAM");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_AUDIOEXCEPTIONPARAM");
    }
}

void CDlgIPCSimpIntellCfg::SaveAudioException()
{
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struAudioExceptionCond = {0};
	struAudioExceptionCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struAudioExceptionCond.dwGroup = 1;
	struAudioExceptionCond.dwSize = sizeof(struAudioExceptionCond);
	int pStatus = 0;
    char szLan[128] = {0};
	m_struAudioException.dwSize = sizeof(m_struAudioException);
	BOOL bRet = NET_DVR_SetDeviceConfig(m_lLoginID, NET_DVR_SET_AUDIOEXCEPTIONPARAM, iCount, &struAudioExceptionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struAudioException, sizeof(m_struAudioException));
	if (bRet && pStatus == 0)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_AUDIOEXCEPTIONPARAM");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_AUDIOEXCEPTIONPARAM");
    }
}

void CDlgIPCSimpIntellCfg::GetSceneDetection()//场景变更侦测2013-07-17
{
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struSceneDetectionCond = {0};
	struSceneDetectionCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struSceneDetectionCond.dwGroup = 1;
	struSceneDetectionCond.dwSize = sizeof(struSceneDetectionCond);
	int pStatus = 0;
    char szLan[128] = {0};
	BOOL bRet = NET_DVR_GetDeviceConfig(m_lLoginID, NET_DVR_GET_SCENECHANGE_DETECTIONCFG, iCount, &struSceneDetectionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struSceneDetection, sizeof(m_struSceneDetection));
	if (bRet && pStatus == 0)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_GET_SCENECHANGE_DETECTIONCFG");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_SCENECHANGE_DETECTIONCFG");
    }
}

void CDlgIPCSimpIntellCfg::SaveSceneDetection()//场景变更侦测2013-07-17
{
	int iCount = 1;
	NET_DVR_CHANNEL_GROUP struSceneDetectionCond = {0};
	struSceneDetectionCond.dwChannel = m_lChannel;//m_cmbChannel.GetItemData(m_cmbChannel.GetCurSel());
	struSceneDetectionCond.dwGroup = 1;
	struSceneDetectionCond.dwSize = sizeof(struSceneDetectionCond);
	int pStatus = 0;
    char szLan[128] = {0};
	m_struSceneDetection.dwSize = sizeof(m_struSceneDetection);
	BOOL bRet = NET_DVR_SetDeviceConfig(m_lLoginID, NET_DVR_SET_SCENECHANGE_DETECTIONCFG, iCount, &struSceneDetectionCond, \
		sizeof(NET_DVR_CHANNEL_GROUP), &pStatus, &m_struSceneDetection, sizeof(m_struSceneDetection));
	if (bRet && pStatus == 0)
	{
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_SCENECHANGE_DETECTIONCFG");
    }
    else
    {
        sprintf(szLan,"err %d",pStatus);
        AfxMessageBox(szLan);
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_SCENECHANGE_DETECTIONCFG");
    }
}

BOOL CDlgIPCSimpIntellCfg::PreDrawPolygonMsg( MSG* pMsg, CPoint &pt )
{
	int i=0;
    BOOL bOneLine = TRUE,bXOneLine = TRUE,bYOneLine = TRUE;

    switch (pMsg->message)
    {
    case WM_LBUTTONDOWN:
        if ((GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0) && 
            PtInRect(&m_rcWnd,pt))
        {
       //     if(m_bNeedRedraw)
            {
				if (m_dwPosNum > 19)
				{
					m_bCloseIn = TRUE;
				}
                if(m_bCloseIn)
                {
                    m_dwPosNum = 0;
                    m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum = m_dwPosNum;
					memset(&m_struFieldDetection.struIntrusion[m_iFieldNo], 0, sizeof(NET_VCA_POLYGON));
                }
                if(m_dwPosNum > 19)
                {
                    char szLan[128] = {0};
                    g_StringLanType(szLan,"多边形顶点不能超过20个!","Apexes of polygon should less than 20");
                    AfxMessageBox(szLan);
                    return TRUE;
                }
				if((float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() <= m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum-1].fX+0.01 && (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() >= m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum-1].fX-0.01\
                    && (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() <= m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum-1].fY+0.01 && (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() >= m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum-1].fY-0.01)
                {
                    char szLan[128] = {0};
                    g_StringLanType(szLan, "不能连续在同一点上画区域", "Can't draw region on the point used");
                    AfxMessageBox(szLan);
                    return TRUE;
                }
                m_bCloseIn = FALSE;
                m_bMouseMovePolygon = FALSE;
                m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum].fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum].fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();
                m_dwPosNum++;
                m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum = m_dwPosNum;	
            }
            
        }
        break;
    case WM_MOUSEMOVE:
        if ((GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0) && 
            PtInRect(&m_rcWnd,pt))
        {
                if(PtInRect(&m_rcWnd,pt))
                {
                    if(m_dwPosNum > 19)
                    {
                        return TRUE;
                    }
                    if( !m_bCloseIn)
                    {
                        m_bMouseMovePolygon = TRUE;
                        m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum].fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
                        m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_dwPosNum].fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();
                        m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum = m_dwPosNum+1;				
                    }
                }
        }

        break;
    case WM_RBUTTONDOWN:
        if((GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0) 
            && PtInRect(&m_rcWnd,pt))
        {
                if(m_dwPosNum > 19)
                {
                    return TRUE;
                }
                for(i=0; i<m_dwPosNum; i++)
                {
                    if(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i].fX != m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i+1].fX)
                    {
                        bXOneLine = FALSE;
                        break;
                    }
                }
                for(i=0; i<m_dwPosNum; i++)
                {
                    if(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i].fY != m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i+1].fY)
                    {
                        bYOneLine = FALSE;
                        break;
                    }
                }
                bOneLine = bXOneLine||bYOneLine;
                if(bOneLine)
                {
                    char szlan[128] = {0};
                    g_StringLanType(szlan, "所画点成一线，无法构成区域!","The points is on a line and can't create a rectangle!");
                    AfxMessageBox(szlan);
                    return TRUE;
                }
                if( !m_bCloseIn)
                {
                    if(m_bMouseMovePolygon)
                    {
                        m_bMouseMovePolygon = FALSE;
                        m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum--;
                        m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum].fX = 0;
                        m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum].fY = 0;
                    }
                    m_bCloseIn = TRUE;
                }
		}
        break;
    }
     return TRUE;
}

void CDlgIPCSimpIntellCfg::OnLineNoChanged()
{
//	if (m_Combo_CfgType.GetCurSel() == 0)
	{
		UpdateData(TRUE);
		
		m_struLineDetection.struAlertParam[m_iLineNo].bySensitivity = atoi(m_iLineSen);
		m_struLineDetection.struAlertParam[m_iLineNo].dwCrossDirection = (VCA_CROSS_DIRECTION)atoi(m_iCross);
		
		//然后把当前值拷贝到页面
		m_iLineNo = m_Combo_LineNo.GetCurSel();
		m_iLineSen.Format("%d", m_struLineDetection.struAlertParam[m_iLineNo].bySensitivity);
	//	m_iLineSen = m_struLineDetection.struAlertParam[m_iLineNo].bySensitivity;
		m_iCross.Format("%d", m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].dwCrossDirection);
	//	m_iCross = m_struLineDetection.struAlertParam[m_Combo_LineNo.GetCurSel()].dwCrossDirection;
		UpdateData(FALSE);
	}
}

void CDlgIPCSimpIntellCfg::OnFieldNoChanged()
{
	//if (m_Combo_CfgType.GetCurSel() == 1)
	{
		UpdateData(TRUE);

		m_dwPosNum = 0;
		//区域入侵
		m_struFieldDetection.struIntrusion[m_iFieldNo].bySensitivity = atoi(m_iFieldSen);
		m_struFieldDetection.struIntrusion[m_iFieldNo].byRate = atoi(m_iFieldRate);
		m_struFieldDetection.struIntrusion[m_iFieldNo].wDuration = atoi(m_iFieldDuration);

		//然后把当前值拷贝到页面
		m_iFieldNo = m_Comnbo_FieldNo.GetCurSel();

		m_iFieldSen.Format("%d", m_struFieldDetection.struIntrusion[m_iFieldNo].bySensitivity);
	//	m_iFieldSen = m_struFieldDetection.struIntrusion[m_iFieldNo].bySensitivity;
		m_iFieldRate.Format("%d", m_struFieldDetection.struIntrusion[m_iFieldNo].byRate);
	//	m_iFieldRate = m_struFieldDetection.struIntrusion[m_iFieldNo].byRate;
		m_iFieldDuration.Format("%d", m_struFieldDetection.struIntrusion[m_iFieldNo].wDuration);
	//	m_iFieldDuration = m_struFieldDetection.struIntrusion[m_iFieldNo].wDuration;

		UpdateData(FALSE);
	}
}

BOOL CDlgIPCSimpIntellCfg::PreDrawLineMsg( MSG* pMsg, CPoint &pt )
{
    UpdateData(TRUE);
	CString strTemp = _T("");
    DWORD i=0;
    BOOL bOneLine = TRUE,bXOneLine = TRUE,bYOneLine = TRUE;
 
    switch(pMsg->message)  // Message of Draw
    {
    case WM_LBUTTONDOWN:
 
        if( (m_Combo_CfgType.GetCurSel() == 0) &&
            ((GetAsyncKeyState(VK_LCONTROL)< 0) || (GetAsyncKeyState(VK_RCONTROL) < 0)))//Press CTRL to draw
		{
			if(!m_bMouseMove) 
			{
				m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
				m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();		
				m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
				m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();			
				m_bMouseMove = TRUE;
			}
			else
			{
				if ((float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() <= m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fX+0.01 && (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width() >= m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fX-0.01\
					&& (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() <= m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fY+0.01 && (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height() >= m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fY-0.01)
				{
					char szLan[128] = {0};
					g_StringLanType(szLan, "不能连续在同一点上画区域", "Can't draw region on the point used");
					AfxMessageBox(szLan);
					return TRUE;
				}
				m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
				m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();			
				m_bMouseMove = FALSE;
             }
		}
        break;
    case WM_MOUSEMOVE:
        if( (m_Combo_CfgType.GetCurSel() == 0)  && (GetAsyncKeyState(VK_LCONTROL)<0 || GetAsyncKeyState(VK_RCONTROL)<0))
        {
			if(PtInRect(&m_rcWnd,pt))
            {
				if(m_bMouseMove)
				{
					m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fX = (float)(pt.x-m_rcWnd.left)/(float)m_rcWnd.Width();
					m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fY = (float)(pt.y-m_rcWnd.top)/(float)m_rcWnd.Height();			
                }
			}
        }
        break;
    case WM_RBUTTONDOWN://Click right mouse to close the rectangle. 
        break;
    default:
        break;
    }
	return TRUE;
}

BOOL CDlgIPCSimpIntellCfg::PreTranslateMessage( MSG* pMsg )
{
	CPoint pt(0, 0);
	GetDlgItem(IDC_PLAYWND)->GetWindowRect(&m_rcWnd);
	GetCursorPos(&pt);
	ScreenToClient(&m_rcWnd);
	ScreenToClient(&pt);
	
// 	if (m_bChkDrawPlate)
// 	{
// 		PreDrawPolygonMsg(pMsg, pt);
// 	}
	if (m_Combo_CfgType.GetCurSel() == 0)
	{
		PreDrawLineMsg(pMsg, pt);
	}
	else if(m_Combo_CfgType.GetCurSel() == 1)
	{
		PreDrawPolygonMsg(pMsg, pt);
	}
	
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgIPCSimpIntellCfg::F_DrawLine( HDC hDc )
{
	int iDirect = -1;
	int i = m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fX;
    int x1 = (int)(m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fX*m_rcWnd.Width());
    int y1 = (int)(m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struStart.fY*m_rcWnd.Height());
    int x2 = (int)(m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fX*m_rcWnd.Width());
    int y2 = (int)(m_struLineDetection.struAlertParam[m_iLineNo].struPlaneBottom.struEnd.fY*m_rcWnd.Height());
	
	COLORREF color = RGB(0,255,0); 
    SetTextColor(hDc,color);
    
    using namespace Gdiplus;
    Graphics graphics(hDc);
    Color DrawColor;
    DrawColor.SetValue(DrawColor.MakeARGB(50,GetRValue(color) ,GetGValue(color),GetBValue(color)));
    
    Pen pen(color, 4);
    pen.SetStartCap(LineCapFlat);
    pen.SetEndCap(LineCapFlat);
    
    CPen DrawPen;
    DrawPen.CreatePen(PS_SOLID, 2, color);
    
	//     bool bRect = false;
	//     int k = 0;
	//     //calculate slope
	//     if(x2 != x1)
	//     {
	//         k = (y2 - y1) / (x2 - x1);
	//         
	//         //if the absolute value of slope is more than 2, draw rectangle directly
	//         if(abs(k) >= 2)
	//         {
	//             bRect = true;
	//         }
	//     }
	//     else
	//     {
	//         bRect = true;
	//     }
	//     
	//     if(bRect)
	//     {
	//         pen.SetColor(DrawColor);
	//         if(k < 0)
	//         {
	//             graphics.DrawLine(&pen, x1-2, y1-1, x2-2, y2-1);
	//         }
	//         else
	//         {
	//             graphics.DrawLine(&pen, x1+2, y1-1, x2+2, y2-1);
	//         }
	//     }
	//     else
	//     {
	//         //Draw parallelogram, the vertical height is 8
	//         SolidBrush  brush(DrawColor);
	//         POINT point[4];
	//         point[0].x = x1;
	//         point[0].y = y1;
	//         point[1].x = x2;
	//         point[1].y = y2;
	//         point[2].x = x2;
	//         point[2].y = y2-8;
	//         point[3].x = x1;
	//         point[3].y = y1-8;
	//         graphics.FillPolygon(&brush, (Point *)point, 4);
	//     }
    
    
    HGDIOBJ pOldPen = SelectObject(hDc, DrawPen);
    MoveToEx(hDc, x1, y1, NULL);
    LineTo(hDc, x2, y2);
    
    //Draw arrow
	//    F_DrawArrowhead(hDc, lpDrawTaversePlane);
    //Draw label
    
    SelectObject(hDc, pOldPen);
	DeleteObject(DrawPen);
}

void CDlgIPCSimpIntellCfg::F_DrawFun( long lPlayHandle, HDC hDc, DWORD dwUser )
{
	if (m_Combo_CfgType.GetCurSel() == 0)
	{
		F_DrawLine(hDc);
	}
	else if (m_Combo_CfgType.GetCurSel() == 1)
	{
		F_DrawPolygon(hDc);
	}
}

void CDlgIPCSimpIntellCfg::F_DrawPolygon( HDC hDc)
{
	using namespace Gdiplus;
    Graphics graphics(hDc);
    SolidBrush  brush(Color(50, 0, 0, 0));
	COLORREF color = RGB(0,255,0); 
    brush.SetColor(Color(50, GetRValue(color), GetGValue(color), GetBValue(color)));
    
    POINT point[ITC_MAX_POLYGON_POINT_NUM] = {0};
    
    CPen DrawPen;
    unsigned int i;
    
    DrawPen.CreatePen(PS_SOLID, 2, color);
    SetTextColor(hDc,color);
    SetBkMode(hDc, TRANSPARENT);
    
    HGDIOBJ pOldPen = SelectObject(hDc, DrawPen);
    MoveToEx(hDc, (int)(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[0].fX*m_rcWnd.Width()), \
        (int)(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[0].fY*m_rcWnd.Height()), NULL);
    
    for(i=1; i<m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum && i < VCA_MAX_POLYGON_POINT_NUM + 1; i++)
    {
        
        LineTo(hDc, (int)(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i].fX*m_rcWnd.Width()),\
            (int)(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i].fY*m_rcWnd.Height()));
        //Redraw the polygon if the lines are intersectant.
		
		if (i>=MIN_PNT_NUM)
		{
			if (!m_bMouseMove)
			{
				if (IsCrossLine(&(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion)))
				{
					{
						char szLan[128] = {0};
						g_StringLanType(szLan,"多边形不相邻的边有相交情况存在,请重新输入!","Edges of Polygon can be intersectant, please redraw.!");
						//		AfxMessageBox(szLan);
						m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum--;
						m_dwPosNum = m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum;
						memset(&m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum], 0, sizeof(NET_VCA_POINT));
						break;
					}
				}
			}
		}
		//        if(i>=MIN_PNT_NUM && !lpDrawPolygon->bMouseMove && IsCrossLine(&lpDrawPolygon->struVcaPolygon))
		//         {
		//             char szLan[128] = {0};
		//             g_StringLanType(szLan,"多边形不相邻的边有相交情况存在,请重新输入!","Edges of Polygon can be intersectant, please redraw.!");
		//             //AfxMessageBox(szLan);
		//             lpDrawPolygon->struVcaPolygon.dwPointNum--;
		//             break;
		//         }
    }
    
    for (i=0; i<(int)m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum ; i++)
    {
        if (i == ITC_MAX_POLYGON_POINT_NUM)
        {
            break;
        }
        
        point[i].x = (int)(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i].fX*m_rcWnd.Width());
        point[i].y = (int)(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.struPos[i].fY*m_rcWnd.Height());
    }
    //After drawing the 10th point, connect it with the 1st point.
    if(ITC_MAX_POLYGON_POINT_NUM == i && !m_bMouseMove)
    {
        if (IsValidArea(&m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion))
        {
            graphics.FillPolygon(&brush, (Point *)point, m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum);
            
            LineTo(hDc, point[0].x, point[0].y);
           // TextOut(hDc, point[0].x, point[0].y, lpDrawPolygon->chName, strlen(lpDrawPolygon->chName));
			
            m_bCloseIn = TRUE;
        }else//redraw
        {
			
        }
        //check if the alarm region is intact
    }
    else if(m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum>2 && m_bCloseIn)
    {
        if (IsValidArea(&m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion))
        {
            graphics.FillPolygon(&brush, (Point *)point, m_struFieldDetection.struIntrusion[m_iFieldNo].struRegion.dwPointNum);
            
            LineTo(hDc, point[0].x, point[0].y);
			
         //   TextOut(hDc, point[0].x, point[0].y, lpDrawPolygon->chName, strlen(lpDrawPolygon->chName));
			//            F_AddLabel(hDc, AlarmArea, dwRuleID);
        }else
        {
			//             m_bNeedRedraw[dwRuleID] = TRUE;
			//             m_bCloseIn[dwRuleID] = FALSE;
        }
    }
    
    SelectObject(hDc, pOldPen);
    DeleteObject(DrawPen);
}

void CDlgIPCSimpIntellCfg::SetAudioDecibel( const int &i )
{
	CString str;
	str.Format("%d", i);
	GetDlgItem(IDC_STATIC_AUDIODECIBEL)->SetWindowText(str);
}

CDlgIPCSimpIntellCfg::~CDlgIPCSimpIntellCfg()
{
	if (m_lPlayHandle > -1)
	{
		NET_DVR_StopRealPlay(m_lPlayHandle);
	}

	if (m_lAudioHandle > -1)
	{
		NET_DVR_StopRemoteConfig(m_lAudioHandle);
	}

    if (m_hRegisterDrawThread != NULL)
    {
        TerminateThread(m_hRegisterDrawThread, 0);
        CloseHandle(m_hRegisterDrawThread);
        m_hRegisterDrawThread = NULL;
    }
}

void CDlgIPCSimpIntellCfg::OnBtnClearpic() 
{
	// TODO: Add your control notification handler code here
	if (m_Combo_CfgType.GetCurSel() == 0)
	{
		memset(&(m_struLineDetection.struAlertParam[m_iLineNo]), 0, sizeof(NET_VCA_TRAVERSE_PLANE));
	}
	else if (m_Combo_CfgType.GetCurSel() == 1)
	{
		memset(&(m_struFieldDetection.struIntrusion[m_iFieldNo]), 0, sizeof(NET_VCA_INTRUSION));
	}
}

void CDlgIPCSimpIntellCfg::OnChannelChanged()
{
    Play();
    OnCfgTypeChanged();
    return;
}

void CDlgIPCSimpIntellCfg::AddChanInfo()
{
    m_cmbChannel.ResetContent();
    char szLan[128] = {0};
    CString csStr;
    int iIndex = 0;
    int i;
    for(i=0; i< (g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum - g_struDeviceInfo[m_iDeviceIndex].iIPChanNum) ; i++)
    {
        csStr.Format("%s %d", "Camera", g_struDeviceInfo[m_iDeviceIndex].iStartChan + i);
        m_cmbChannel.AddString(csStr);
        m_cmbChannel.SetItemData(iIndex++, g_struDeviceInfo[m_iDeviceIndex].iStartChan + i);
    }
    
    for(i=0; i<g_struDeviceInfo[m_iDeviceIndex].iIPChanNum; i++)
    {
         if (g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].struIPDevInfo[i].byEnable)
        {
            g_StringLanType(szLan, "IPCamera", "IPChannel");
            csStr.Format("%s %d", szLan, 1 + i);
            m_cmbChannel.AddString(csStr);
            m_cmbChannel.SetItemData(iIndex++, g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan + i);
        }
        
    }
    
	m_cmbChannel.SetCurSel(0);
}

void CDlgIPCSimpIntellCfg::AddGroupNoInfo()
{
    m_cmbGroupNo.ResetContent();
    char szLan[128] = {0};
    CString csStr;
    int iIndex = 0;
    int i;
    int iMaxChanNo = 1;
    int iMaxIPAlarmOutNo = 0;  
    if (g_struDeviceInfo[m_iDeviceIndex].iIPChanNum == 0)
    {
        iMaxChanNo = g_struDeviceInfo[m_iDeviceIndex].iStartChan + g_struDeviceInfo[m_iDeviceIndex].iDeviceChanNum ;
    }
    else
    {
         iMaxChanNo = g_struDeviceInfo[m_iDeviceIndex].iIPChanNum + g_struDeviceInfo[m_iDeviceIndex].pStruIPParaCfgV40[0].dwStartDChan -1 ;
    }

    if (g_struDeviceInfo[m_iDeviceIndex].pStruIPAlarmOutCfgV40 == NULL)
    {
        iMaxIPAlarmOutNo = g_struDeviceInfo[m_iDeviceIndex].iAlarmOutNum;
    }
    else
    {
        if (g_struDeviceInfo[m_iDeviceIndex].byStartIPAlarmOutNo == 0)
        {
            iMaxIPAlarmOutNo = g_struDeviceInfo[m_iDeviceIndex].pStruIPAlarmOutCfgV40[0].dwCurIPAlarmOutNum +32;
        }
        else
        {
            iMaxIPAlarmOutNo = g_struDeviceInfo[m_iDeviceIndex].pStruIPAlarmOutCfgV40[0].dwCurIPAlarmOutNum +g_struDeviceInfo[m_iDeviceIndex].byStartIPAlarmOutNo;
        }
    }

    DWORD dwMaxGroupNo = (iMaxChanNo > iMaxIPAlarmOutNo)?  (iMaxChanNo/64 + 1):(iMaxIPAlarmOutNo/64 + 1);
    for(i=0; i< dwMaxGroupNo; i++)
    {
        csStr.Format("Group%d", i +1);
        m_cmbGroupNo.AddString(csStr);
        m_cmbGroupNo.SetItemData(iIndex++, i);
    }
    
   
	m_cmbGroupNo.SetCurSel(0);   
}
void CDlgIPCSimpIntellCfg::OnBtnExit()
{
    if (m_hRegisterDrawThread != NULL)
    {
        TerminateThread(m_hRegisterDrawThread, 0);
        CloseHandle(m_hRegisterDrawThread);
        m_hRegisterDrawThread = NULL;
    }
    CDialog::OnCancel();
}
 
void CDlgIPCSimpIntellCfg::OnBtnRefresh()
{
     OnChannelChanged();
}

void CDlgIPCSimpIntellCfg::OnBtnPtzLockstatusSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	
	m_struPtzLockCfg.dwSize = sizeof(m_struPtzLockCfg);
	m_struPtzLockCfg.byWorkMode = m_comboPtzLockStatus.GetCurSel();
	
	if (!NET_DVR_SetDVRConfig(m_lLoginID, NET_DVR_SET_PTZLOCKCFG, m_lChannel,&m_struPtzLockCfg, sizeof(m_struPtzLockCfg)))	
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_SET_PTZLOCKCFG %d",NET_DVR_GetLastError());
	}
	else
	{
		if (m_struPtzLockCfg.byWorkMode == 1)
		{
			OnGetPtzLockInfo();
		} 		
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_SET_PTZLOCKCFG Success!");
	}
	return;
}


void CALLBACK fPtzLockInfoCallback(DWORD dwType, void *lpBuffer, DWORD dwBufLen, void *pUserData)
{
	CDlgIPCSimpIntellCfg* pPtzLockInfo = (CDlgIPCSimpIntellCfg*)pUserData;
	if (pPtzLockInfo == NULL)
	{
		return;
	}
    
	CString strItem = "";
	
	switch (dwType)
	{
	case NET_SDK_CALLBACK_TYPE_DATA:
		if (lpBuffer != NULL)
		{
			NET_DVR_PTZLOCKINFO* pCallbackData = (NET_DVR_PTZLOCKINFO*)lpBuffer;
			pPtzLockInfo->SetPtzLockTime(pCallbackData->dwRemainingSec);
		}
		break;
	case NET_SDK_CALLBACK_TYPE_STATUS:
		{
			if (lpBuffer != NULL)
			{
				DWORD dwStatus = *(DWORD*)lpBuffer;
				if (dwStatus == NET_SDK_CALLBACK_STATUS_FAILED)
				{	
					strItem = "Fail";
					pPtzLockInfo->GetDlgItem(IDC_STATIC_PTZLOCKTIME)->SetWindowText(strItem);
				}
				else if(dwStatus == NET_SDK_CALLBACK_STATUS_SUCCESS)
				{
					strItem = "Succ";
					pPtzLockInfo->GetDlgItem(IDC_STATIC_PTZLOCKTIME)->SetWindowText(strItem);	
				}
			}
		}
		break;
	}
}

void CDlgIPCSimpIntellCfg::OnGetPtzLockInfo()
{
	char szLan[128] = {0};
	
	if (m_lHandle >= 0)
	{
		if (!NET_DVR_StopRemoteConfig(m_lHandle))
		{
			g_StringLanType(szLan, "云台锁定时间获取失败", "Get PTZ Lock Info Failed");
			AfxMessageBox(szLan);
			return;
		}
	} 
	m_struPtzLockInfoCond.dwSize = sizeof(m_struPtzLockInfoCond);
	m_struPtzLockInfoCond.dwChannel = m_lChannel;
	m_lHandle = NET_DVR_StartRemoteConfig(m_lLoginID, NET_DVR_GET_PTZLOCKINFO, &m_struPtzLockInfoCond, sizeof(m_struPtzLockInfoCond), fPtzLockInfoCallback, this);
	if (m_lHandle >= 0)
	{
		g_pMainDlg->AddLog(m_lLoginID, OPERATION_SUCC_T, "NET_DVR_StartRemoteConfig NET_DVR_GET_PTZLOCKINFO");
		// 		m_bGetNext = TRUE;
		// 		DWORD dwThreadId;
		// 		//m_lstPdcResult.DeleteAllItems();
		// 		m_hGetInfoThread = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(GetPtzLockInfoThread), this, 0, &dwThreadId);
	}
	else
	{
		g_pMainDlg->AddLog(m_lLoginID, OPERATION_FAIL_T, "NET_DVR_StartRemoteConfig NET_DVR_GET_PTZLOCKINFO");
		//m_bGetNext = FALSE;
		// 		g_StringLanType(szLan, "云台锁定时间获取失败", "Get PTZ Lock Info Failed");
		// 		AfxMessageBox(szLan);
		// 		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GET_PTZLOCKINFO");	
	}
	return;
}

void CDlgIPCSimpIntellCfg::SetPtzLockTime(const int i)
{
	CString str;
	str.Format("%d", i);
	GetDlgItem(IDC_STATIC_PTZLOCKTIME)->SetWindowText(str);
}