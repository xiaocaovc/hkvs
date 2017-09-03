// DlgLCDScreenDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgLCDScreenDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLCDScreenDisplay dialog


CDlgLCDScreenDisplay::CDlgLCDScreenDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLCDScreenDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLCDScreenDisplay)
	m_byBackLight = 0;
	m_byBrightnessLevel = 0;
	m_byContrastLevel = 0;
	m_bySharpnessLevel = 0;
	m_bySaturationLevel = 0;
	m_byHueLevel = 0;
	m_byRed = 0;
	m_byGreen = 0;
	m_byBlue = 0;
	m_byBlueOffset = 0;
	m_byGreenOffset = 0;
	m_byRedOffset = 0;
	m_byGainB = 0;
	m_byGainG = 0;
	m_byGainR = 0;
	m_byOffsetR = 0;
	m_byOffsetG = 0;
	m_byOffsetB = 0;
	m_byLeftEdge = 0;
	m_byLowerEdge = 0;
	m_byRightEdge = 0;
	m_byTopEdge = 0;
	//}}AFX_DATA_INIT

	m_lUserID = -1;
	m_iDevIndex = -1;
}


void CDlgLCDScreenDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLCDScreenDisplay)
	DDX_Control(pDX, IDC_COMBO_ENABLE_EDGE, m_comEdgeEnable);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_MODE, m_comDisplayMode);
	DDX_Control(pDX, IDC_COMBO_CFG_TYPE, m_comCfgType);
	DDX_Text(pDX, IDC_EDIT_BACKLIGHT, m_byBackLight);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_byBrightnessLevel);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_byContrastLevel);
	DDX_Text(pDX, IDC_EDIT_SHARPNESS, m_bySharpnessLevel);
	DDX_Text(pDX, IDC_EDIT_SATURATION, m_bySaturationLevel);
	DDX_Text(pDX, IDC_EDIT_HUE, m_byHueLevel);
	DDX_Text(pDX, IDC_EDIT_RED, m_byRed);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_byGreen);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_byBlue);
	DDX_Text(pDX, IDC_EDIT_BULEOFFSET, m_byBlueOffset);
	DDX_Text(pDX, IDC_EDIT_GREENOFFSET, m_byGreenOffset);
	DDX_Text(pDX, IDC_EDIT_REDOFFSET, m_byRedOffset);
	DDX_Text(pDX, IDC_EDIT_GAIN_B, m_byGainB);
	DDX_Text(pDX, IDC_EDIT_GAIN_G, m_byGainG);
	DDX_Text(pDX, IDC_EDIT_GAIN_R, m_byGainR);
	DDX_Text(pDX, IDC_EDIT_OFFSET_R, m_byOffsetR);
	DDX_Text(pDX, IDC_EDIT_OFFSET_G, m_byOffsetG);
	DDX_Text(pDX, IDC_EDIT_OFFSET_B, m_byOffsetB);
	DDX_Text(pDX, IDC_EDIT_LEFT_EDGE, m_byLeftEdge);
	DDX_Text(pDX, IDC_EDIT_LOWER_EDGE, m_byLowerEdge);
	DDX_Text(pDX, IDC_EDIT_RIGHT_EDGE, m_byRightEdge);
	DDX_Text(pDX, IDC_EDIT_TOP_EDGE, m_byTopEdge);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLCDScreenDisplay, CDialog)
	//{{AFX_MSG_MAP(CDlgLCDScreenDisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_CFG_TYPE, OnSelchangeComboCfgType)
	ON_BN_CLICKED(IDC_BTN_SAVE_CFG, OnBtnSaveCfg)
	ON_BN_CLICKED(IDC_BTN_SET_CFG, OnBtnSetCfg)
	ON_BN_CLICKED(IDC_BTN_GET_CFG, OnBtnGetCfg)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, OnSelchangeComboDisplayMode)
	ON_CBN_SELCHANGE(IDC_COMBO_ENABLE_EDGE, OnSelchangeComboEnableEdge)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLCDScreenDisplay message handlers

BOOL CDlgLCDScreenDisplay::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_comCfgType.SetCurSel(0);
	OnSelchangeComboCfgType(); 
	initDisplay();
	memset(&m_struDisplay, 0, sizeof(m_struDisplay));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLCDScreenDisplay::initDisplay()
{
	m_byBackLight = 0;

	m_comDisplayMode.SetCurSel(0);
	m_byBrightnessLevel = 0;
	m_byContrastLevel = 0;
	m_bySharpnessLevel = 0;
	m_bySaturationLevel = 0;
	m_byHueLevel = 0;

	m_byRed = 0;
	m_byGreen = 0;
	m_byBlue = 0;
	m_byRedOffset = 0;
	m_byGreenOffset = 0;
	m_byBlueOffset = 0;
	
	m_byGainR = 0;
	m_byGainG = 0;
	m_byGainB = 0;
	m_byOffsetR = 0;
	m_byOffsetG = 0;
	m_byOffsetB = 0;

	m_comEdgeEnable.SetCurSel(0);
	m_byLeftEdge = 0;
	m_byRightEdge = 0;
	m_byTopEdge = 0;
	m_byLowerEdge = 0;	
}

void CDlgLCDScreenDisplay::OnSelchangeComboCfgType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	initDisplay();
	int i = m_comCfgType.GetCurSel();
	if (0 == i)
	{
		GetDlgItem(IDC_EDIT_BACKLIGHT)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_DISPLAY_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_RED)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_REDOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREENOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BULEOFFSET)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_GAIN_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_B)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_ENABLE_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(FALSE);
	}
	else if (1 == i)
	{
		GetDlgItem(IDC_EDIT_BACKLIGHT)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_COMBO_DISPLAY_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RED)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_REDOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREENOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BULEOFFSET)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_GAIN_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_B)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_ENABLE_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(FALSE);

		m_byBackLight = m_struDisplay.byBackLight;
	} 
	else if (2 == i)
	{
		GetDlgItem(IDC_EDIT_BACKLIGHT)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_DISPLAY_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT_RED)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_REDOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREENOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BULEOFFSET)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_GAIN_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_B)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_ENABLE_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(FALSE);

		m_comDisplayMode.SetCurSel(0);
		if(0xff == m_struDisplay.struVideoOutCfg.byDisplayMode)
		{
			m_comDisplayMode.SetCurSel(0);
		}
		else
		{
			m_comDisplayMode.SetCurSel(m_struDisplay.struVideoOutCfg.byDisplayMode);
		}
		
		if (0 == m_comDisplayMode.GetCurSel())
		{
			m_byBrightnessLevel = m_struDisplay.struVideoOutCfg.byBrightnessLevel;
			m_byContrastLevel = m_struDisplay.struVideoOutCfg.byContrastLevel;
			m_bySharpnessLevel = m_struDisplay.struVideoOutCfg.bySharpnessLevel;
			m_bySaturationLevel = m_struDisplay.struVideoOutCfg.bySaturationLevel;
			m_byHueLevel = m_struDisplay.struVideoOutCfg.byHueLevel;

			GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_HUE)->EnableWindow(TRUE);
		}
		else
		{
			m_byBrightnessLevel = 0;
			m_byContrastLevel = 0;
			m_bySharpnessLevel = 0;
			m_bySaturationLevel = 0;
			m_byHueLevel = 0;

			GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_HUE)->EnableWindow(FALSE);
		}
		
	}
	else if (3 == i)
	{
		GetDlgItem(IDC_EDIT_BACKLIGHT)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_DISPLAY_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RED)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_REDOFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GREENOFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BULEOFFSET)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT_GAIN_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_B)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_ENABLE_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(FALSE);

		m_byRed = m_struDisplay.struColorTempCfg.byRed;
		m_byGreen = m_struDisplay.struColorTempCfg.byGreen;
		m_byBlue = m_struDisplay.struColorTempCfg.byBlue;
		m_byRedOffset = m_struDisplay.struColorTempCfg.byRedOffset;
		m_byGreenOffset = m_struDisplay.struColorTempCfg.byGreenOffset;
		m_byBlueOffset = m_struDisplay.struColorTempCfg.byBlueOffset;
	}
	else if (4 == i)
	{
		GetDlgItem(IDC_EDIT_BACKLIGHT)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_DISPLAY_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RED)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_REDOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREENOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BULEOFFSET)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_GAIN_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN_G)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_G)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_B)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_COMBO_ENABLE_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(FALSE);

		m_byGainR = m_struDisplay.struAdcCfg.byGainR;
		m_byGainG = m_struDisplay.struAdcCfg.byGainG;
		m_byGainB = m_struDisplay.struAdcCfg.byGainB;
		m_byOffsetR = m_struDisplay.struAdcCfg.byOffsetR;
		m_byOffsetG = m_struDisplay.struAdcCfg.byOffsetG;
		m_byOffsetB = m_struDisplay.struAdcCfg.byOffsetB;
	}
	else if (5 == i)
	{
		GetDlgItem(IDC_EDIT_BACKLIGHT)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_DISPLAY_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_RED)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_REDOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GREENOFFSET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BULEOFFSET)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_GAIN_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_R)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_G)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFSET_B)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_COMBO_ENABLE_EDGE)->EnableWindow(TRUE);
		m_comEdgeEnable.SetCurSel(m_struDisplay.struScreenEdgeCfg.byEnable);
		if (0 == m_struDisplay.struScreenEdgeCfg.byEnable)
		{
			m_byLeftEdge = 0;
			m_byRightEdge = 0;
			m_byTopEdge = 0;
			m_byLowerEdge = 0;
			GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(FALSE);
		}
		else
		{
			m_byLeftEdge = m_struDisplay.struScreenEdgeCfg.byLeftEdge;
			m_byRightEdge = m_struDisplay.struScreenEdgeCfg.byRightEdge;
			m_byTopEdge = m_struDisplay.struScreenEdgeCfg.byTopEdge;
			m_byLowerEdge = m_struDisplay.struScreenEdgeCfg.byLowerEdge;
			GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(TRUE);
		}
	}

	UpdateData(FALSE);
}

void CDlgLCDScreenDisplay::OnBtnSaveCfg() 
{
	// TODO: Add your control notification handler code here
}

void CDlgLCDScreenDisplay::OnBtnSetCfg() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
	memset(&m_struDisplay, 0, sizeof(m_struDisplay));
	m_struDisplay.dwSize = sizeof(m_struDisplay);
	m_struDisplay.byCfgType = m_comCfgType.GetCurSel();
	if (0 == m_struDisplay.byCfgType)
	{
		return;
	}
	else if (1 == m_struDisplay.byCfgType)
	{
		m_struDisplay.byBackLight = m_byBackLight;
	} 
	else if (2 == m_struDisplay.byCfgType)
	{
		if (0 == m_comDisplayMode.GetCurSel())
		{
			m_struDisplay.struVideoOutCfg.byDisplayMode = 0xff;
			m_struDisplay.struVideoOutCfg.byBrightnessLevel = m_byBrightnessLevel;
			m_struDisplay.struVideoOutCfg.byContrastLevel = m_byContrastLevel;
			m_struDisplay.struVideoOutCfg.bySharpnessLevel = m_bySharpnessLevel;
			m_struDisplay.struVideoOutCfg.bySaturationLevel = m_bySaturationLevel;
			m_struDisplay.struVideoOutCfg.byHueLevel = m_byHueLevel;
		}
		else
		{
			m_struDisplay.struVideoOutCfg.byDisplayMode = m_comDisplayMode.GetCurSel();
		}
	}
	else if (3 == m_struDisplay.byCfgType)
	{
		m_struDisplay.struColorTempCfg.byRed = m_byRed;
		m_struDisplay.struColorTempCfg.byGreen = m_byGreen;
		m_struDisplay.struColorTempCfg.byBlue = m_byBlue;
		m_struDisplay.struColorTempCfg.byRedOffset = m_byRedOffset;
		m_struDisplay.struColorTempCfg.byGreenOffset = m_byGreenOffset;
		m_struDisplay.struColorTempCfg.byBlueOffset = m_byBlueOffset;
	}
	else if (4 == m_struDisplay.byCfgType)
	{
		m_struDisplay.struAdcCfg.byGainR = m_byGainR ;
		m_struDisplay.struAdcCfg.byGainG = m_byGainG;
		m_struDisplay.struAdcCfg.byGainB = m_byGainB;
		m_struDisplay.struAdcCfg.byOffsetR = m_byOffsetR;
		m_struDisplay.struAdcCfg.byOffsetG = m_byOffsetG;
		m_struDisplay.struAdcCfg.byOffsetB = m_byOffsetB;
	}
	else if (5 == m_struDisplay.byCfgType)
	{
		m_struDisplay.struScreenEdgeCfg.byEnable = m_comEdgeEnable.GetCurSel();
		if (1 == m_struDisplay.struScreenEdgeCfg.byEnable )
		{
			m_struDisplay.struScreenEdgeCfg.byLeftEdge = m_byLeftEdge;
			m_struDisplay.struScreenEdgeCfg.byRightEdge = m_byRightEdge;
			m_struDisplay.struScreenEdgeCfg.byTopEdge = m_byTopEdge;
			m_struDisplay.struScreenEdgeCfg.byLowerEdge = m_byLowerEdge;
		}
	}
	
	if (NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_SCREEN_DISPLAY_CFG, -1, &m_struDisplay, sizeof(m_struDisplay)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_SCREEN_DISPLAY_CFG");
		UpdateData(FALSE);
        return;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_SCREEN_DISPLAY_CFG");
		UpdateData(FALSE);
        return;
    }
}

void CDlgLCDScreenDisplay::OnBtnGetCfg() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DWORD dwReturn = 0;
	initDisplay();
	memset(&m_struDisplay, 0, sizeof(m_struDisplay));
    if (NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_SCREEN_DISPLAY_CFG, -1, &m_struDisplay, sizeof(m_struDisplay), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_SCREEN_DISPLAY_CFG");
		m_comCfgType.SetCurSel(m_struDisplay.byCfgType);

	//	if (1 == m_struDisplay.byCfgType)
		{
			m_byBackLight = m_struDisplay.byBackLight;
		} 
	//	else if (2 == m_struDisplay.byCfgType)
		{
			if (0xff == m_struDisplay.struVideoOutCfg.byDisplayMode)
			{
				m_comDisplayMode.SetCurSel(0);
			}
			else
			{
				m_comDisplayMode.SetCurSel(m_struDisplay.struVideoOutCfg.byDisplayMode);
			}
			m_byBrightnessLevel = m_struDisplay.struVideoOutCfg.byBrightnessLevel;
			m_byContrastLevel = m_struDisplay.struVideoOutCfg.byContrastLevel;
			m_bySharpnessLevel = m_struDisplay.struVideoOutCfg.bySharpnessLevel;
			m_bySaturationLevel = m_struDisplay.struVideoOutCfg.bySaturationLevel;
			m_byHueLevel = m_struDisplay.struVideoOutCfg.byHueLevel;
		}
	//	else if (3 == m_struDisplay.byCfgType)
		{
			m_byRed = m_struDisplay.struColorTempCfg.byRed;
			m_byGreen = m_struDisplay.struColorTempCfg.byGreen;
			m_byBlue = m_struDisplay.struColorTempCfg.byBlue;
			m_byRedOffset = m_struDisplay.struColorTempCfg.byRedOffset;
			m_byGreenOffset = m_struDisplay.struColorTempCfg.byGreenOffset;
			m_byBlueOffset = m_struDisplay.struColorTempCfg.byBlueOffset;
		}
	//	else if (4 == m_struDisplay.byCfgType)
		{
			m_byGainR = m_struDisplay.struAdcCfg.byGainR;
			m_byGainG = m_struDisplay.struAdcCfg.byGainG;
			m_byGainB = m_struDisplay.struAdcCfg.byGainB;
			m_byOffsetR = m_struDisplay.struAdcCfg.byOffsetR;
			m_byOffsetG = m_struDisplay.struAdcCfg.byOffsetG;
			m_byOffsetB = m_struDisplay.struAdcCfg.byOffsetB;
		}
	//	else if (5 == m_struDisplay.byCfgType)
		{
			m_comEdgeEnable.SetCurSel(m_struDisplay.struScreenEdgeCfg.byEnable);
			if (1 == m_struDisplay.struScreenEdgeCfg.byEnable)
			{
				m_byLeftEdge = m_struDisplay.struScreenEdgeCfg.byLeftEdge;
				m_byRightEdge = m_struDisplay.struScreenEdgeCfg.byRightEdge;
				m_byTopEdge = m_struDisplay.struScreenEdgeCfg.byTopEdge;
				m_byLowerEdge = m_struDisplay.struScreenEdgeCfg.byLowerEdge;
			}
		}
//		OnSelchangeComboCfgType();
		UpdateData(FALSE);
		GetDlgItem(IDC_EDIT_BACKLIGHT)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_COMBO_DISPLAY_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT_RED)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GREEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BLUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_REDOFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GREENOFFSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BULEOFFSET)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_EDIT_GAIN_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN_G)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_R)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_G)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFSET_B)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_COMBO_ENABLE_EDGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(TRUE);

 		OnSelchangeComboDisplayMode();
 		OnSelchangeComboEnableEdge();
		
		return;
	}
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_SCREEN_DISPLAY_CFG");	
		m_comCfgType.SetCurSel(0);
		UpdateData(FALSE);
        return;
    }
}

void CDlgLCDScreenDisplay::OnSelchangeComboDisplayMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (0 == m_comDisplayMode.GetCurSel())
	{
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(TRUE);
		
// 		m_byBrightnessLevel = m_struDisplay.struVideoOutCfg.byBrightnessLevel;
// 		m_byContrastLevel = m_struDisplay.struVideoOutCfg.byContrastLevel;
// 		m_bySharpnessLevel = m_struDisplay.struVideoOutCfg.bySharpnessLevel;
// 		m_bySaturationLevel = m_struDisplay.struVideoOutCfg.bySaturationLevel;
// 		m_byHueLevel = m_struDisplay.struVideoOutCfg.byHueLevel;
	}
	else
	{
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CONTRAST)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHARPNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SATURATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HUE)->EnableWindow(FALSE);
		
// 		m_byBrightnessLevel = 0;
// 		m_byContrastLevel = 0;
// 		m_bySharpnessLevel = 0;
// 		m_bySaturationLevel = 0;
// 		m_byHueLevel = 0;
	}
	UpdateData(FALSE);
}

void CDlgLCDScreenDisplay::OnSelchangeComboEnableEdge() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (0 == m_comEdgeEnable.GetCurSel())
	{
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(FALSE);
// 		m_byLeftEdge = 0;
// 		m_byRightEdge = 0;
// 		m_byTopEdge = 0;
// 		m_byLowerEdge = 0;
	}
	else
	{
		GetDlgItem(IDC_EDIT_LEFT_EDGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT_EDGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TOP_EDGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LOWER_EDGE)->EnableWindow(TRUE);

// 		m_byLeftEdge = m_struDisplay.struScreenEdgeCfg.byLeftEdge;
// 		m_byRightEdge = m_struDisplay.struScreenEdgeCfg.byRightEdge;
// 		m_byTopEdge = m_struDisplay.struScreenEdgeCfg.byTopEdge;
// 		m_byLowerEdge = m_struDisplay.struScreenEdgeCfg.byLowerEdge;
	}
	UpdateData(FALSE);
}
