#if !defined(AFX_DLGLCDSCREENDISPLAY_H__0D96701C_F506_4BA4_B717_17682B102FBE__INCLUDED_)
#define AFX_DLGLCDSCREENDISPLAY_H__0D96701C_F506_4BA4_B717_17682B102FBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLCDScreenDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLCDScreenDisplay dialog

class CDlgLCDScreenDisplay : public CDialog
{
// Construction
public:
	CDlgLCDScreenDisplay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgLCDScreenDisplay)
	enum { IDD = IDD_DLG_SCREEN_DISPLAY };
	CComboBox	m_comEdgeEnable;
	CComboBox	m_comDisplayMode;
	CComboBox	m_comCfgType;
	BYTE	m_byBackLight;
	BYTE	m_byBrightnessLevel;
	BYTE	m_byContrastLevel;
	BYTE	m_bySharpnessLevel;
	BYTE	m_bySaturationLevel;
	BYTE	m_byHueLevel;
	BYTE	m_byRed;
	BYTE	m_byGreen;
	BYTE	m_byBlue;
	BYTE	m_byBlueOffset;
	BYTE	m_byGreenOffset;
	BYTE	m_byRedOffset;
	BYTE	m_byGainB;
	BYTE	m_byGainG;
	BYTE	m_byGainR;
	BYTE	m_byOffsetR;
	BYTE	m_byOffsetG;
	BYTE	m_byOffsetB;
	BYTE	m_byLeftEdge;
	BYTE	m_byLowerEdge;
	BYTE	m_byRightEdge;
	BYTE	m_byTopEdge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLCDScreenDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLCDScreenDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboCfgType();
	afx_msg void OnBtnSaveCfg();
	afx_msg void OnBtnSetCfg();
	afx_msg void OnBtnGetCfg();
	afx_msg void OnSelchangeComboDisplayMode();
	afx_msg void OnSelchangeComboEnableEdge();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void initDisplay();

	LONG m_lUserID;
	int m_iDevIndex;

	NET_DVR_SCREEN_DISPLAY_CFG m_struDisplay;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLCDSCREENDISPLAY_H__0D96701C_F506_4BA4_B717_17682B102FBE__INCLUDED_)
