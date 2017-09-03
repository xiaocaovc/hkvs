#if !defined(AFX_DLGCCDPARAM_H__7C2B1FE2_F49D_48CA_BF49_362F8D2BB82B__INCLUDED_)
#define AFX_DLGCCDPARAM_H__7C2B1FE2_F49D_48CA_BF49_362F8D2BB82B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCcdParam1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCcdParam dialog
                
class CDlgCcdParam : public CDialog
{
// Construction
public:
	CDlgCcdParam(CWnd* pParent = NULL);   // standard constructor

    NET_DVR_CAMERAPARAMCFG m_CcdParam;
	NET_DVR_CAMERAPARAMCFG_EX m_CcdParamEx;
    int m_iDeviceIndex;
	LONG m_lChannel;
	NET_DVR_CAMERA_DEHAZE_CFG m_struDehaze;
	NET_DVR_CORRIDOR_MODE m_struCorridorMode;
	NET_DVR_ISP_CAMERAPARAMCFG m_struISPCameraParamCfg;
// Dialog Data
	//{{AFX_DATA(CDlgCcdParam)
	enum { IDD = IDD_DLG_CCDPARAM };
	CComboBox	m_comboMirror;
	CComboBox	m_cmLaserTriggerMode;
	CComboBox	m_cmLaserControlMode;
	CComboBox	m_comboCaptureMode2;
	CComboBox	m_comboCaptureMode;
	CComboBox	m_comboPIrisMode;
	CComboBox	m_comboSmartIRMode;
	CComboBox	m_comElecteSwitch;
	CComboBox	m_comboDehazeMode;
	CComboBox	m_comboLocalOutPutGate;
	CComboBox	m_comboDayNightType;
	CComboBox	m_comboAlarmTrigMode;
	CComboBox	m_comboBackLightMode;
	CComboBox	m_comboWhiteBalanceMode;
	CComboBox	m_comboLightInhibitLevel;
	CComboBox	m_comboGrayLevel;
	CComboBox	m_comboDigitalZoom;
	CComboBox	m_comboPaletteMode;
	CComboBox	m_comboFocusSpeed;
	CComboBox	m_comboFilterSwitch;
	CComboBox	m_comboEnhanceMode;
	CComboBox	m_comboDimmerMode;
	CComboBox	m_comboInOutMode;
	CComboBox	m_comboNoiseMoveMode;
	CComboBox	m_ctrlIrisMode;
	int		m_iBrightness;
	int		m_iContrast;
	int		m_iGain;
	int		m_iSaturation;
	int		m_iSharpness;
	int		m_iExposureUserSet;
	int		m_iVedioExposure;
	int		m_iUserGain;
	BYTE	m_byNormalLevel;
	BYTE	m_bySpectralLevel;
	BYTE	m_byTemporalLevel;
	BYTE	m_byAutoCompInter;
	BOOL	m_bChkLightInhibitEn;
	BOOL	m_bChkIlluminationEn;
	BOOL	m_bChkSmartIREn;
	BYTE	m_byBGain;
	BYTE	m_byRGain;
	BYTE	m_byEndTimeHour;
	BYTE	m_byEndTimeMin;
	BYTE	m_byEndTimeSec;
	DWORD	m_dwBackLightX1;
	DWORD	m_dwBackLightX2;
	DWORD	m_dwBackLightY1;
	DWORD	m_dwBackLightY2;
	BYTE	m_byBeginTimeHour;
	BYTE	m_byBeginTimeMin;
	BYTE	m_byBeginTimeSec;
	BYTE	m_byDehazeLevel;
	BOOL	m_bChkCorridorMode;
	BYTE	m_byElectLevel;
	BYTE	m_byIRDistance;
	BYTE	m_byPIrisAperture;
	BOOL	m_bChkISPSet;
	BYTE	m_byLaserAngle;
	BYTE	m_byLaserBrightness;
	BYTE	m_byLaserSensitivity;
	BYTE	m_byLaserLimitBrightness;
	BYTE	m_byShortIRDistance;
	BYTE	m_byLongIRDistance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCcdParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCcdParam)
	afx_msg void OnBtnGet();
	afx_msg void OnBtnSet();
	afx_msg void OnBtnExit();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInfrareCfg();
	afx_msg void OnBtnSetCorridorMode();
	afx_msg void OnBtnGetCorridorMode();
	afx_msg void OnBtnGetEx();
	afx_msg void OnBtnSetEx();
	afx_msg void OnSelchangeComboIrismode();
	afx_msg void OnSelchangeComboPirisMode();
	afx_msg void OnSelchangeComboSmartirMode();
	afx_msg void OnBtnIspParamset();
	afx_msg void OnBtnSignallightsync();
	afx_msg void OnBtnEzvizAccesscfg();
	afx_msg void OnBtnIOoutCfg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void InitLocalOutPutGate();
	void SetNewInfoToWnd();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCCDPARAM_H__7C2B1FE2_F49D_48CA_BF49_362F8D2BB82B__INCLUDED_)
