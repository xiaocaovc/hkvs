// DlgVcaITCTriggerCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgVcaITCTriggerCfg.h"
#include "DrawFun.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_POLYGON_POINT_NUM ITC_MAX_POLYGON_POINT_NUM
#define MIN_POLYGON_POINT_NUM 3

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaITCTriggerCfg dialog

void CALLBACK g_DrawITCTriggerRgn(LONG lRealHandle, HDC hDc, DWORD dwUser)
{  
 	CDlgVcaITCTriggerCfg* pDlg = (CDlgVcaITCTriggerCfg*)dwUser;
 	if (pDlg != NULL)
 	{
 		pDlg->DrawRgnFun(hDc);
 	}
}

CDlgVcaITCTriggerCfg::CDlgVcaITCTriggerCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVcaITCTriggerCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVcaITCTriggerCfg)
	m_bFarmVehicleRecog = FALSE;
	m_bMicroPlateRecog = FALSE;
	m_bPlateRecog = FALSE;
	m_bTriggerCfg = FALSE;
	m_bVehicleColorRecog = FALSE;
	m_bVehicleLogoRecog = FALSE;
	m_bViaVtcoil = FALSE;
	m_strProvinceName = _T("");
	m_bFuzzyRecog = FALSE;
	m_bMotocarRecog = FALSE;
	m_nAssociateLaneNO = 0;
	m_nLaneNum = 0;
	//}}AFX_DATA_INIT

	memset(&m_struItcTriggerCfg,0,sizeof(m_struItcTriggerCfg));
    m_nCurDrawType         =     DRAW_TYPE_NONE;
	m_bFirstDrawLaneBoundary = FALSE;
	m_bFirstDrawLaneLine = FALSE;
	m_nCurLaneIndex = 0;
	m_nDrawPointNum = 0;
}


void CDlgVcaITCTriggerCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVcaITCTriggerCfg)
	DDX_Control(pDX, IDC_COMBO_VEHICLE_TYPE, m_comboVehicleType);
	DDX_Control(pDX, IDC_COMBO_TIME_TYPE, m_comboTimeType);
	DDX_Control(pDX, IDC_COMBO_SCENE_TYPE, m_comboSceneType);
	DDX_Control(pDX, IDC_COMBO_RECOG_TYPE, m_comboRecogType);
	DDX_Control(pDX, IDC_COMBO_RECOG_POS, m_comboRecogPos);
	DDX_Control(pDX, IDC_COMBO_LOCATE_TYPE, m_comboLocateType);
	DDX_Control(pDX, IDC_COMBO_LANE_USEAGETYPE, m_comboLaneUseageType);
	DDX_Control(pDX, IDC_COMBO_LANE_NUMBER, m_comboLaneNumber);
	DDX_Control(pDX, IDC_COMBO_LANE_DIRECTION, m_comboLaneDirection);
	DDX_Control(pDX, IDC_COMBO_DRIVE_DIRECTION, m_comboDriveDirection);
	DDX_Check(pDX, IDC_CHECK_FARM_VEHICLE_RECOG, m_bFarmVehicleRecog);
	DDX_Check(pDX, IDC_CHECK_MICRO_PLATE_RECOG, m_bMicroPlateRecog);
	DDX_Check(pDX, IDC_CHECK_PLATE_RECOG, m_bPlateRecog);
	DDX_Check(pDX, IDC_CHECK_TRIGGER_CFG, m_bTriggerCfg);
	DDX_Check(pDX, IDC_CHECK_VEHICLE_COLOR_RECOG, m_bVehicleColorRecog);
	DDX_Check(pDX, IDC_CHECK_VEHICLE_LOGO_RECOG, m_bVehicleLogoRecog);
	DDX_Check(pDX, IDC_CHECK_VIA_VTCOIL, m_bViaVtcoil);
	DDX_Text(pDX, IDC_EDIT_PROVINCE_NAME, m_strProvinceName);
	DDX_Check(pDX, IDC_CHECK_FUZZY_RECOG, m_bFuzzyRecog);
	DDX_Check(pDX, IDC_CHECK_MOTOCAR_RECOG, m_bMotocarRecog);
	DDX_Text(pDX, IDC_EDIT_ASSOCIATE_LANENO, m_nAssociateLaneNO);
	DDX_Text(pDX, IDC_EDIT_LANE_NUM, m_nLaneNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVcaITCTriggerCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgVcaITCTriggerCfg)
	ON_BN_CLICKED(IDC_BTN_DRAW_LANE_BOUNDARY_LINE, OnBtnDrawLaneBoundaryLine)
	ON_BN_CLICKED(IDC_BTN_DRAW_LANE_LINE, OnBtnDrawLaneLine)
	ON_BN_CLICKED(IDC_BTN_DRAW_PLATE_RECOG_RGN, OnBtnDrawPlateRecogRgn)
	ON_CBN_SELCHANGE(IDC_COMBO_LANE_NUMBER, OnSelchangeComboLaneNumber)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_END_DRAW, OnBtnEndDraw)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaITCTriggerCfg message handlers
BOOL CDlgVcaITCTriggerCfg::OnInitDialog()
{
	CDialog::OnInitDialog();

    GetDlgItem(IDC_PLAYWND)->GetWindowRect(&m_rcPlayWnd);
	ScreenToClient(&m_rcPlayWnd);
	StartRealPlay(GetDlgItem(IDC_PLAYWND)->GetSafeHwnd(),g_DrawITCTriggerRgn,this);
	
	InitCtrlState();

	GetITCTriggerCfg();
	LoadWndContent();

	return TRUE;
}

void CDlgVcaITCTriggerCfg::OnDestroy()
{   
	StopRealPlay();
	CDialog::OnDestroy();
}

void CDlgVcaITCTriggerCfg::OnBtnDrawLaneBoundaryLine() 
{
	NET_ITC_LINE& struLaneBoundaryLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneBoundaryLine;
	memset(&struLaneBoundaryLine,0,sizeof(struLaneBoundaryLine));
	m_bFirstDrawLaneBoundary = TRUE;
	m_nCurDrawType = DRAW_TYPE_LANE_BOUNDARY_LINE;
}

void CDlgVcaITCTriggerCfg::OnBtnDrawLaneLine() 
{
	NET_ITC_LINE& struLaneLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struLaneLine;
	memset(&struLaneLine,0,sizeof(struLaneLine));
	m_bFirstDrawLaneLine = TRUE;
	m_nCurDrawType = DRAW_TYPE_LANE_LINE;
}

void CDlgVcaITCTriggerCfg::OnBtnDrawPlateRecogRgn() 
{   
	NET_ITC_POLYGON&  struPlateRecog = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struPlateRecog;
	memset(&struPlateRecog,0,sizeof(struPlateRecog));
	m_nCurDrawType = DRAW_TYPE_PLATE_RECOG_RGN;
	m_nDrawPointNum = 0;
}

void CDlgVcaITCTriggerCfg::OnBtnEndDraw() 
{   
	if (m_nCurDrawType == DRAW_TYPE_PLATE_RECOG_RGN)
	{
		NET_ITC_POLYGON&  struPlateRecog = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struPlateRecog;
		if (m_nDrawPointNum >= MIN_POLYGON_POINT_NUM)
		{
			struPlateRecog.dwPointNum = m_nDrawPointNum;
		}
		else
		{
			
			memset(&struPlateRecog,0,sizeof(struPlateRecog));
		}
		m_nCurDrawType = DRAW_TYPE_NONE;
	}
}

void CDlgVcaITCTriggerCfg::OnSelchangeComboLaneNumber() 
{
	int nCurSel = m_comboLaneNumber.GetCurSel();
	if (nCurSel != CB_ERR)
	{   
		UpdateData(TRUE);
		SaveViaLaneWndContent(m_nCurLaneIndex);
		m_nCurLaneIndex = nCurSel;
		LoadViaLaneWndContent(nCurSel);
		UpdateData(FALSE);
	}
}

void CDlgVcaITCTriggerCfg::OnBtnSave() 
{
	SaveWndContent();
    
	char szError[32] = "\0";
	if (SetITCTriggerCfg())
	{
		g_StringLanType(szError, "保存成功","Saved successfully");
	}
	else
	{
		g_StringLanType(szError, "保存失败","Save failed");
	}
	AfxMessageBox(szError);
}


void CDlgVcaITCTriggerCfg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags,point);

	if (!m_rcPlayWnd.PtInRect(point))
	{
		return;
	}
	if (m_nCurDrawType == DRAW_TYPE_NONE)
	{
		return;
	}
	
	if (m_nCurDrawType == DRAW_TYPE_LANE_LINE)
	{
		NET_ITC_LINE& struLaneLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struLaneLine;
		
		if (m_bFirstDrawLaneLine)
		{
			struLaneLine.struLine.struStart.fX = (float)(point.x-m_rcPlayWnd.left)/(float)m_rcPlayWnd.Width();
            struLaneLine.struLine.struStart.fY = (float)(point.y-m_rcPlayWnd.top) /(float)m_rcPlayWnd.Height();
			struLaneLine.struLine.struEnd.fX   = struLaneLine.struLine.struStart.fX;
            struLaneLine.struLine.struEnd.fY   = struLaneLine.struLine.struStart.fY;
			m_bFirstDrawLaneLine= FALSE;
		}
		else
		{   
			struLaneLine.struLine.struEnd.fX = (float)(point.x-m_rcPlayWnd.left)/(float)m_rcPlayWnd.Width();
            struLaneLine.struLine.struEnd.fY = (float)(point.y-m_rcPlayWnd.top) /(float)m_rcPlayWnd.Height();
			m_nCurDrawType = DRAW_TYPE_NONE;
		}
	}
	else if (m_nCurDrawType == DRAW_TYPE_LANE_BOUNDARY_LINE)
	{   
		NET_ITC_LINE& struLaneBoundaryLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneBoundaryLine;
		
		if (m_bFirstDrawLaneBoundary)
		{
			struLaneBoundaryLine.struLine.struStart.fX = (float)(point.x-m_rcPlayWnd.left)/(float)m_rcPlayWnd.Width();
            struLaneBoundaryLine.struLine.struStart.fY = (float)(point.y-m_rcPlayWnd.top) /(float)m_rcPlayWnd.Height();
			struLaneBoundaryLine.struLine.struEnd.fX   = struLaneBoundaryLine.struLine.struStart.fX;
            struLaneBoundaryLine.struLine.struEnd.fY   = struLaneBoundaryLine.struLine.struStart.fY;
			m_bFirstDrawLaneBoundary = FALSE;
		}
		else
		{   
			struLaneBoundaryLine.struLine.struEnd.fX = (float)(point.x-m_rcPlayWnd.left)/(float)m_rcPlayWnd.Width();
            struLaneBoundaryLine.struLine.struEnd.fY = (float)(point.y-m_rcPlayWnd.top) /(float)m_rcPlayWnd.Height();
			m_nCurDrawType = DRAW_TYPE_NONE;
		}
	}
	else if (m_nCurDrawType == DRAW_TYPE_PLATE_RECOG_RGN)
	{
		if (m_nDrawPointNum >= MAX_POLYGON_POINT_NUM)
		{
			m_nCurDrawType = DRAW_TYPE_NONE;
		}
		else
		{   
			NET_ITC_POLYGON&  struPlateRecog = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struPlateRecog;
		    struPlateRecog.struPos[m_nDrawPointNum].fX  = (float)(point.x - m_rcPlayWnd.left) / (float)m_rcPlayWnd.Width();
		    struPlateRecog.struPos[m_nDrawPointNum].fY  = (float)(point.y - m_rcPlayWnd.top)  / (float)m_rcPlayWnd.Height();
		    m_nDrawPointNum++;
		}
	}
}

void CDlgVcaITCTriggerCfg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnRButtonDown(nFlags,point);
	
	if (!m_rcPlayWnd.PtInRect(point))
	{
		return;
	}
	if (m_nCurDrawType == DRAW_TYPE_NONE)
	{
		return;
	}
	
	if (m_nCurDrawType == DRAW_TYPE_LANE_BOUNDARY_LINE)
	{   
		m_bFirstDrawLaneBoundary = FALSE;
	}
	else if (m_nCurDrawType == DRAW_TYPE_LANE_LINE)
	{
		m_bFirstDrawLaneLine = FALSE;
	}
	else if (m_nCurDrawType == DRAW_TYPE_PLATE_RECOG_RGN)
	{   
		NET_ITC_POLYGON&  struPlateRecog = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struPlateRecog;
		
		if (m_nDrawPointNum < MIN_POLYGON_POINT_NUM)
		{
			memset(&struPlateRecog,0,sizeof(struPlateRecog));
		}
		else
		{   
			struPlateRecog.dwPointNum = m_nDrawPointNum;
		}
	}
	m_nCurDrawType = DRAW_TYPE_NONE;	
}

void CDlgVcaITCTriggerCfg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags,point);
	
	if (!m_rcPlayWnd.PtInRect(point))
	{
		return;
	}
	if (m_nCurDrawType == DRAW_TYPE_NONE)
	{
		return;
	}

	if (m_nCurDrawType == DRAW_TYPE_LANE_BOUNDARY_LINE)
	{   
		NET_ITC_LINE& struLaneBoundaryLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneBoundaryLine;
		
		if (!m_bFirstDrawLaneBoundary)
		{
			struLaneBoundaryLine.struLine.struEnd.fX = (float)(point.x-m_rcPlayWnd.left)/(float)m_rcPlayWnd.Width();
            struLaneBoundaryLine.struLine.struEnd.fY = (float)(point.y-m_rcPlayWnd.top) /(float)m_rcPlayWnd.Height();
		}
	}
	else if (m_nCurDrawType == DRAW_TYPE_LANE_LINE)
	{
		NET_ITC_LINE& struLaneLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struLaneLine;
		
		if (!m_bFirstDrawLaneLine)
		{
			struLaneLine.struLine.struEnd.fX = (float)(point.x-m_rcPlayWnd.left)/(float)m_rcPlayWnd.Width();
            struLaneLine.struLine.struEnd.fY = (float)(point.y-m_rcPlayWnd.top) /(float)m_rcPlayWnd.Height();
		}
	}
	else if (m_nCurDrawType == DRAW_TYPE_PLATE_RECOG_RGN)
	{   
        NET_ITC_POLYGON&  struPlateRecog = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[m_nCurLaneIndex].struPlateRecog;
		
		if (m_nDrawPointNum >= MAX_POLYGON_POINT_NUM || m_nDrawPointNum < 1)
		{
			return;
		}
		struPlateRecog.struPos[m_nDrawPointNum].fX  = (float)(point.x - m_rcPlayWnd.left) / (float)m_rcPlayWnd.Width();
		struPlateRecog.struPos[m_nDrawPointNum].fY  = (float)(point.y - m_rcPlayWnd.top)  / (float)m_rcPlayWnd.Height();
	    struPlateRecog.dwPointNum = m_nDrawPointNum + 1;
	}
}

void CDlgVcaITCTriggerCfg::InitCtrlState()
{   
	char szText[32] = "\0";

	m_comboRecogPos.ResetContent();
    g_StringLanType(szText, "正向识别","Forward Plate");
	m_comboRecogPos.InsertString(0,szText);
	m_comboRecogPos.SetItemData(0,0);
    g_StringLanType(szText, "背向识别","Back Plate");
	m_comboRecogPos.InsertString(1,szText);
	m_comboRecogPos.SetItemData(1,1);
	m_comboRecogPos.SetCurSel(0);
    
	m_comboVehicleType.ResetContent();
	g_StringLanType(szText, "小车牌","Small Plate");
	m_comboVehicleType.InsertString(0,szText);
	m_comboVehicleType.SetItemData(0,0);
	g_StringLanType(szText, "大车牌","Big Plate");
	m_comboVehicleType.InsertString(1,szText);
	m_comboVehicleType.SetItemData(1,1);
	m_comboVehicleType.SetCurSel(0);
    
	m_comboTimeType.ResetContent();
	g_StringLanType(szText, "白天","Daytime");
	m_comboTimeType.InsertString(0,szText);
	m_comboTimeType.SetItemData(0,0);
	g_StringLanType(szText, "晚上","Night");
	m_comboTimeType.InsertString(1,szText);
	m_comboTimeType.SetItemData(1,1);
	m_comboTimeType.SetCurSel(0);
    
	m_comboSceneType.ResetContent();
	g_StringLanType(szText, "电警","Electrical Superintendent");
	m_comboSceneType.InsertString(0,szText);
	m_comboSceneType.SetItemData(0,0);
	g_StringLanType(szText, "卡口","Gate");
	m_comboSceneType.InsertString(1,szText);
	m_comboSceneType.SetItemData(1,1);
	m_comboSceneType.SetCurSel(0);
    
	m_comboLocateType.ResetContent();
	g_StringLanType(szText, "帧定位","Frame Locate");
	m_comboLocateType.InsertString(0,szText);
	m_comboLocateType.SetItemData(0,0);
	g_StringLanType(szText, "场定位","Field locate");
	m_comboLocateType.InsertString(1,szText);
	m_comboLocateType.SetItemData(1,1);
	m_comboLocateType.SetCurSel(0);
    
	m_comboRecogType.ResetContent();
	g_StringLanType(szText, "帧识别","Frame Recognition");
	m_comboRecogType.InsertString(0,szText);
	m_comboRecogType.SetItemData(0,0);
	g_StringLanType(szText, "场识别","Field Recognition");
	m_comboRecogType.InsertString(1,szText);
	m_comboRecogType.SetItemData(1,1);
	m_comboRecogType.SetCurSel(0);

	for (int i = 0; i < MAX_ITC_LANE_NUM; i++)
	{
		CString strLaneNumber  = "";
		strLaneNumber.Format("%d",i + 1);
		m_comboLaneNumber.InsertString(i,strLaneNumber);
		m_comboLaneNumber.SetItemData(i,i);
	}
	m_comboLaneNumber.SetCurSel(0);
    
	m_comboLaneDirection.ResetContent();
	g_StringLanType(szText, "未知", "Unknown");
	m_comboLaneDirection.InsertString(0, szText);
	m_comboLaneDirection.SetItemData(0, ITC_LANE_DIRECTION_UNKNOW);

	g_StringLanType(szText, "左转", "Left");
	m_comboLaneDirection.InsertString(1, szText);
	m_comboLaneDirection.SetItemData(1, ITC_LANE_LEFT);
	
	g_StringLanType(szText, "直行", "Straight");
	m_comboLaneDirection.InsertString(2, szText);
	m_comboLaneDirection.SetItemData(2, ITC_LANE_STRAIGHT);
	
	g_StringLanType(szText, "左转+直行", "Left and Straight");
	m_comboLaneDirection.InsertString(3, szText);
	m_comboLaneDirection.SetItemData(3, ITC_LANE_LEFT_STRAIGHT);
	
	g_StringLanType(szText, "右转", "Right");
	m_comboLaneDirection.InsertString(4, szText);
	m_comboLaneDirection.SetItemData(4, ITC_LANE_RIGHT);
	
	g_StringLanType(szText, "左转+右转", "Left and Right");
	m_comboLaneDirection.InsertString(5, szText);
	m_comboLaneDirection.SetItemData(5, ITC_LANE_LEFT_RIGHT);
	
	g_StringLanType(szText, "右转+直行", "Right and Straight");
	m_comboLaneDirection.InsertString(6, szText);
	m_comboLaneDirection.SetItemData(6, ITC_LANE_RIGHT_STRAIGHT);
	
	g_StringLanType(szText, "左转+右转+直行", "Left and Right and Straight");
	m_comboLaneDirection.InsertString(7, szText);
	m_comboLaneDirection.SetItemData(7, ITC_LANE_LEFT_RIGHT_STRAIGHT);
	
	g_StringLanType(szText, "左转待行", "Left Wait");
	m_comboLaneDirection.InsertString(8, szText);
	m_comboLaneDirection.SetItemData(8, ITC_LANE_LEFT_WAIT);
	
	g_StringLanType(szText, "直行待行", "Straight Wait");
	m_comboLaneDirection.InsertString(9, szText);
	m_comboLaneDirection.SetItemData(9, ITC_LANE_STRAIGHT_WAIT);

	g_StringLanType(szText, "正向行驶", "Forward");
	m_comboLaneDirection.InsertString(10, szText);
	m_comboLaneDirection.SetItemData(10, ITC_LANE_FORWARD);

	g_StringLanType(szText, "背向行驶", "Backward");
	m_comboLaneDirection.InsertString(11, szText);
	m_comboLaneDirection.SetItemData(11, ITC_LANE_BACKWARD);

	g_StringLanType(szText, "双向行驶", "Bothway");
	m_comboLaneDirection.InsertString(12, szText);
	m_comboLaneDirection.SetItemData(12, ITC_LANE_BOTHWAY);
	m_comboLaneDirection.SetCurSel(0);

	m_comboLaneUseageType.ResetContent();
	g_StringLanType(szText, "未知", "Unknown");
	m_comboLaneUseageType.InsertString(0, szText);
	m_comboLaneUseageType.SetItemData(0, ITC_LANE_USEAGE_UNKNOW);

	g_StringLanType(szText, "正常车道", "Ordinary lanes");
	m_comboLaneUseageType.InsertString(1, szText);
	m_comboLaneUseageType.SetItemData(1, ITC_LANE_CARRIAGEWAY);

	g_StringLanType(szText, "公交车专用道", "Bus lanes");
	m_comboLaneUseageType.InsertString(2, szText);
	m_comboLaneUseageType.SetItemData(2, ITC_LANE_BUS);

	g_StringLanType(szText, "快车道", "Fast lane");
	m_comboLaneUseageType.InsertString(3, szText);
	m_comboLaneUseageType.SetItemData(3, ITC_LANE_FAST);

	g_StringLanType(szText, "慢车道", "Slow lane");
	m_comboLaneUseageType.InsertString(4, szText);
	m_comboLaneUseageType.SetItemData(4, ITC_LANE_SLOW);

	g_StringLanType(szText, "摩托车道", "Motorcycle lane");
	m_comboLaneUseageType.InsertString(5, szText);
	m_comboLaneUseageType.SetItemData(5, ITC_LANE_MOTOR);

	g_StringLanType(szText, "非机动车道", "Non-motor vehicle lane");
	m_comboLaneUseageType.InsertString(6, szText);
	m_comboLaneUseageType.SetItemData(6, ITC_LANE_NONMOTOR);

	g_StringLanType(szText, "反向车道", "Reversed lane");
	m_comboLaneUseageType.InsertString(7, szText);
	m_comboLaneUseageType.SetItemData(7, ITC_LANE_REVERSE_LANE);

	g_StringLanType(szText, "禁止货车车道", "Ban trucks lane");
	m_comboLaneUseageType.InsertString(8, szText);
	m_comboLaneUseageType.SetItemData(8, ITC_LANE_BAN_TRUCKS);

	g_StringLanType(szText, "混合车道", "Mix lane");
	m_comboLaneUseageType.InsertString(9, szText);
	m_comboLaneUseageType.SetItemData(9, ITC_LANE_MIX);
	m_comboLaneUseageType.SetCurSel(0);

	m_comboDriveDirection.ResetContent();
    g_StringLanType(szText, "未知", "Unknown");
	m_comboDriveDirection.InsertString(0,szText);
	m_comboDriveDirection.SetItemData(0,ITC_LANE_DRIVE_UNKNOW);

	g_StringLanType(szText, "下行", "Up to down");
	m_comboDriveDirection.InsertString(1,szText);
	m_comboDriveDirection.SetItemData(1,ITC_LANE_DRIVE_UP_TO_DOWN);

	g_StringLanType(szText, "上行", "Down to up");
	m_comboDriveDirection.InsertString(2,szText);
	m_comboDriveDirection.SetItemData(2,ITC_LANE_DRIVE_DOWN_TO_UP);
	m_comboDriveDirection.SetCurSel(0);
}

void CDlgVcaITCTriggerCfg::DrawRgnFun(HDC hdc)
{   
    for (int i = 0; i < MAX_ITC_LANE_NUM; i++)
    {   
		DRAW_POLYGON_PARAM_EX struPolygonParam;
		memset(&struPolygonParam,0,sizeof(struPolygonParam));
		memcpy(&struPolygonParam.rcWnd, &m_rcPlayWnd, sizeof(m_rcPlayWnd));

		DRAW_LINE_PARAM  struLaneLineParam;
        if (i == m_nCurLaneIndex) 
        {
			struLaneLineParam.color = RGB(255,0,0);
			struPolygonParam.color = RGB(255,0,0);
			
        }
        else
        {  
			struLaneLineParam.color = RGB(0,255,0);
			struPolygonParam.color = RGB(85,170,255);
        }
        
		NET_ITC_LINE& struLaneLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[i].struLaneLine;
		memcpy(&struLaneLineParam.rcWnd, &m_rcPlayWnd, sizeof(m_rcPlayWnd));
        struLaneLineParam.struVcaLine = struLaneLine.struLine;
		CDrawFun::DrawLine(hdc, &struLaneLineParam);

        NET_ITC_POLYGON&  struPlateRecog = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[i].struPlateRecog;
		struPolygonParam.pArryPt = struPlateRecog.struPos;
		struPolygonParam.dwPtNum = struPlateRecog.dwPointNum;
		CDrawFun::DrawPolygonEx(hdc,&struPolygonParam);
	}
    
	DRAW_LINE_PARAM  struLaneBoundaryParam;
	
	NET_ITC_LINE& struLaneBoundaryLine = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneBoundaryLine;
	struLaneBoundaryParam.color = RGB(255,255,0);
	memcpy(&struLaneBoundaryParam.rcWnd, &m_rcPlayWnd, sizeof(struLaneBoundaryParam.rcWnd));
	struLaneBoundaryParam.struVcaLine = struLaneBoundaryLine.struLine;
    CDrawFun::DrawLine(hdc, &struLaneBoundaryParam);
}

void CDlgVcaITCTriggerCfg::LoadViaLaneWndContent(int nLaneIndex)
{
	NET_DVR_VIA_LANE_PARAM& struViaLaneParam = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[nLaneIndex];
    
	const NET_ITC_LANE_LOGIC_PARAM& struLaneLogicParam = struViaLaneParam.struLogicParam;
	
	m_nAssociateLaneNO = struViaLaneParam.byLaneNO;
	
	int i = 0;
	for (i = 0; i < m_comboLaneNumber.GetCount(); i++)
	{
		if (m_comboLaneNumber.GetItemData(i) == nLaneIndex)
		{
			m_comboLaneNumber.SetCurSel(i);
			break;
		}
	}
	
	for (i = 0; i < m_comboLaneUseageType.GetCount(); i++)
	{
		if (m_comboLaneUseageType.GetItemData(i) == struLaneLogicParam.byUseageType)
		{
			m_comboLaneUseageType.SetCurSel(i);
			break;
		}
	}
	
	for (i = 0; i < m_comboLaneDirection.GetCount(); i++)
	{
		if (m_comboLaneDirection.GetItemData(i) == struLaneLogicParam.byDirectionType)
		{
			m_comboLaneDirection.SetCurSel(i);
			break;
		}
	}
	
	for (i = 0; i < m_comboDriveDirection.GetCount(); i++)
	{
		if (m_comboDriveDirection.GetItemData(i) == struLaneLogicParam.byCarDriveDirect)
		{
			m_comboDriveDirection.SetCurSel(i);
			break;
		}
	}
}

void CDlgVcaITCTriggerCfg::SaveViaLaneWndContent(int nLaneIndex)
{
	NET_DVR_VIA_LANE_PARAM& struViaLaneParam = m_struItcTriggerCfg.struTriggerParam.uTriggerParam.struViaVtCoil.struLaneParam[nLaneIndex];
    struViaLaneParam.byLaneNO = m_nAssociateLaneNO;
	NET_ITC_LANE_LOGIC_PARAM& struLaneLogicParam = struViaLaneParam.struLogicParam;
	struLaneLogicParam.byUseageType = m_comboLaneUseageType.GetItemData(m_comboLaneUseageType.GetCurSel());
	struLaneLogicParam.byDirectionType = m_comboLaneDirection.GetItemData(m_comboLaneDirection.GetCurSel());
	struLaneLogicParam.byCarDriveDirect = m_comboDriveDirection.GetItemData(m_comboDriveDirection.GetCurSel());
}

void CDlgVcaITCTriggerCfg::LoadWndContent()
{
	const NET_ITC_SINGLE_TRIGGERCFG&  struTriggerParam = m_struItcTriggerCfg.struTriggerParam;
	m_bTriggerCfg = struTriggerParam.byEnable;
	
	const NET_DVR_VIA_VTCOIL_PARAM& struViaVtcoiParam = struTriggerParam.uTriggerParam.struViaVtCoil;
	m_bViaVtcoil = struViaVtcoiParam.byEnable;
	m_nLaneNum = struViaVtcoiParam.byLaneNum;
	
    const NET_ITC_PLATE_RECOG_PARAM& struPlateRecogParam = struViaVtcoiParam.struPlateRecog;
	m_strProvinceName = struPlateRecogParam.byDefaultCHN;
    m_bPlateRecog = struPlateRecogParam.byEnable;
    m_bVehicleLogoRecog = struPlateRecogParam.byVehicleLogoRecog;
    
	DWORD dwRecogMode = struPlateRecogParam.dwRecogMode;
	
	m_comboRecogPos.SetCurSel(dwRecogMode & 0x00000001 ? 1 : 0);
    m_comboVehicleType.SetCurSel(dwRecogMode & 0x00000002 ? 1 : 0);
	m_bVehicleColorRecog = (dwRecogMode & 0x00000004) ? TRUE : FALSE;
	m_bFarmVehicleRecog = (dwRecogMode & 0x00000008) ? TRUE : FALSE;
	m_bFuzzyRecog = (dwRecogMode & 0x00000010) ? TRUE : FALSE;
	m_comboLocateType.SetCurSel(dwRecogMode & 0x00000020 ? 1 : 0);
	m_comboRecogType.SetCurSel(dwRecogMode & 0x00000040 ? 1 : 0);
	m_comboTimeType.SetCurSel(dwRecogMode & 0x00000080 ? 1 : 0);
    m_bMotocarRecog = (dwRecogMode & 0x00000100) ? TRUE : FALSE;
	m_comboSceneType.SetCurSel(dwRecogMode & 0x00000200 ? 1 : 0);
	m_bMicroPlateRecog = (dwRecogMode & 0x00000400)? TRUE : FALSE;

	LoadViaLaneWndContent(m_nCurLaneIndex);

	UpdateData(FALSE);
}

void CDlgVcaITCTriggerCfg::SaveWndContent()
{
	UpdateData(TRUE);
    
    m_struItcTriggerCfg.dwSize = sizeof(m_struItcTriggerCfg);

	SaveViaLaneWndContent(m_nCurLaneIndex);
	
	NET_ITC_SINGLE_TRIGGERCFG&  struTriggerParam = m_struItcTriggerCfg.struTriggerParam;
	struTriggerParam.byEnable = m_bTriggerCfg;
	struTriggerParam.dwTriggerType = ITC_VIA_VIRTUALCOIL_TYPE;
	NET_DVR_VIA_VTCOIL_PARAM& struViaVtcoiParam = struTriggerParam.uTriggerParam.struViaVtCoil;
	
	struViaVtcoiParam.byEnable = m_bViaVtcoil;
	struViaVtcoiParam.byLaneNum = m_nLaneNum;
	
    NET_ITC_PLATE_RECOG_PARAM& struPlateRecogParam = struViaVtcoiParam.struPlateRecog;
	strncpy((char*)struPlateRecogParam.byDefaultCHN,m_strProvinceName,MAX_CHJC_NUM);
    struPlateRecogParam.byEnable = m_bPlateRecog;
	struPlateRecogParam.byVehicleLogoRecog = m_bVehicleLogoRecog;
    
	struPlateRecogParam.dwRecogMode = 0;
	struPlateRecogParam.dwRecogMode |= (m_comboRecogPos.GetItemData(m_comboRecogPos.GetCurSel())       == 0 ? 0x00000000 : 0x00000001);
	struPlateRecogParam.dwRecogMode |= (m_comboVehicleType.GetItemData(m_comboVehicleType.GetCurSel()) == 0 ? 0x00000000 : 0x00000002);
	struPlateRecogParam.dwRecogMode |= m_bVehicleColorRecog ? 0x00000004 : 0x00000000;
	struPlateRecogParam.dwRecogMode |= m_bFarmVehicleRecog  ? 0x00000008 : 0x00000000;
	struPlateRecogParam.dwRecogMode |= m_bFuzzyRecog        ? 0x00000010 : 0x00000000;
	struPlateRecogParam.dwRecogMode |= (m_comboLocateType.GetItemData(m_comboLocateType.GetCurSel())   == 0 ? 0x00000000 : 0x00000020);
	struPlateRecogParam.dwRecogMode |= (m_comboRecogType.GetItemData(m_comboRecogType.GetCurSel())     == 0 ? 0x00000000 : 0x00000040);
	struPlateRecogParam.dwRecogMode |= (m_comboTimeType.GetItemData(m_comboTimeType.GetCurSel())       == 0 ? 0x00000000 : 0x00000080);
	struPlateRecogParam.dwRecogMode |= m_bMotocarRecog      ? 0x00000100 : 0x00000000;
	struPlateRecogParam.dwRecogMode |= (m_comboSceneType.GetItemData(m_comboSceneType.GetCurSel())     == 0 ? 0x00000000 : 0x00000200);
	struPlateRecogParam.dwRecogMode |= m_bMicroPlateRecog   ? 0x00000400 : 0x00000000;
}

BOOL CDlgVcaITCTriggerCfg::GetITCTriggerCfg()
{
	NET_DVR_TRIGGER_COND struTriggerCond = {0};
    struTriggerCond.dwSize = sizeof(struTriggerCond);
	struTriggerCond.dwChannel = m_lChannel;
	struTriggerCond.dwTriggerMode = ITC_VIA_VIRTUALCOIL_TYPE;
	
	DWORD dwStatus = 0;
	BOOL bRet      = FALSE;
	
    if (!NET_DVR_GetDeviceConfig(m_lServerID, NET_DVR_GET_TRIGGEREX_CFG, 1,(LPVOID)&struTriggerCond, sizeof(struTriggerCond),
		&dwStatus,&m_struItcTriggerCfg, sizeof(m_struItcTriggerCfg)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_TRIGGEREX_CFG Chan[%d]", m_lChannel);
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_TRIGGEREX_CFG Chan[%d]", m_lChannel);
        bRet = TRUE;
    }
	return bRet;
}

BOOL CDlgVcaITCTriggerCfg::SetITCTriggerCfg()
{
	NET_DVR_TRIGGER_COND struTriggerCond = {0};
	struTriggerCond.dwSize = sizeof(struTriggerCond);
	struTriggerCond.dwChannel = m_lChannel;
	struTriggerCond.dwTriggerMode = ITC_VIA_VIRTUALCOIL_TYPE;

	m_struItcTriggerCfg.struTriggerParam.dwTriggerType = ITC_VIA_VIRTUALCOIL_TYPE;
	
    DWORD dwStatus = 0;
	BOOL bRet      = FALSE;
	
    if (!NET_DVR_SetDeviceConfig(m_lServerID, NET_DVR_SET_TRIGGEREX_CFG,1,(LPVOID)&struTriggerCond, sizeof(struTriggerCond),
		&dwStatus,&m_struItcTriggerCfg, sizeof(m_struItcTriggerCfg)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_TRIGGEREX_CFG Chan[%d]", m_lChannel);
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_TRIGGEREX_CFG Chan[%d]", m_lChannel);
        bRet = TRUE;
    }
    return bRet;
}