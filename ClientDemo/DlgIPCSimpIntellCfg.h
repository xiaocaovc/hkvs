#if !defined(AFX_DLGIPCSIMPINTELLCFG_H__07C66E2C_FC33_40A0_B793_8D837845ACD9__INCLUDED_)
#define AFX_DLGIPCSIMPINTELLCFG_H__07C66E2C_FC33_40A0_B793_8D837845ACD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIPCSimpIntellCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIPCSimpIntellCfg dialog

class CDlgIPCSimpIntellCfg : public CDialog
{
// Construction
public:
	CDlgIPCSimpIntellCfg(CWnd* pParent = NULL);   // standard constructor
	~CDlgIPCSimpIntellCfg();

public:
	CRect   m_rcWnd;

	//设备相关参数
	LONG    m_lLoginID;
	int     m_iDeviceIndex;
	LONG    m_lChannel;

	LONG	m_lPlayHandle;

	int		m_iChanCount;
	int		m_iAnaChanCount;
	int     m_iDStartChannel;
	LONG	m_lStartChannel;
    HANDLE  m_hRegisterDrawThread;

// Dialog Data
	//{{AFX_DATA(CDlgIPCSimpIntellCfg)
	enum { IDD = IDD_DLG_IPC_SIMP_INTELL };
	CComboBox	m_cmbGroupNo;
	CComboBox	m_comboPtzLockStatus;
	CComboBox	m_comEnable;
	CComboBox	m_ComboRegionID;
	CComboBox	m_Comnbo_FieldNo;
	CComboBox	m_Combo_LineNo;
	CComboBox	m_Combo_CfgType;
    CComboBox   m_cmbChannel;
	CStatic	m_wndPlay;
	CTreeCtrl	m_treeAlarmOut;
	CTreeCtrl	m_treeChan;
	CComboBox	m_comboWeekday;
	CComboBox	m_comboCopyTime;
	BOOL	m_bChkMonitorAlarm;
	BOOL	m_bChkVoiceAlarm;
	BOOL	m_bChkUploadCenter;
	BOOL	m_bChkInvokeAlarmOut;
	BOOL	m_bChkInvokeJpegCapture;
	BOOL	m_bChkEmapAlarmOut;
	BOOL    m_bChkFocusAlarm;
	int m_iHour11;
	int m_iMin11;
	int m_iHour12;
	int m_iMin12;
	int m_iHour21;
	int m_iMin21;
	int m_iHour22;
	int m_iMin22;
	int m_iHour31;
	int m_iMin31;
	int m_iHour32;
	int m_iMin32;
	int m_iHour41;
	int m_iMin41;
	int m_iHour42;
	int m_iMin42;
	int m_iHour51;
	int m_iMin51;
	int m_iHour52;
	int m_iMin52;
	int m_iHour61;
	int m_iMin61;
	int m_iHour62;
	int m_iMin62;
	int m_iHour71;
	int m_iMin71;
	int m_iHour72;
	int m_iMin72;
	int m_iHour81;
	int m_iMin81;
	int m_iHour82;
	int m_iMin82;
	BOOL	m_bChkPicToFtp;
	BOOL	m_bEnable;
	BOOL	m_bAudioAbnormal;
	CString	m_iAudioSen;
	CString	m_iAudioThreshold;
	int		m_iMaxChan;
	int		m_iUsedChan;
	CString	m_iLineSen;
	CString	m_iFieldDuration;
	CString	m_iFieldRate;
	CString	m_iFieldSen;
	CString	m_iCross;
	int     m_iSceneSen;
	BOOL	m_bTraverseDualVca;
	BOOL	m_bFieldDetectionDualVca;
	UINT	m_iDefocusSensortiveLevel;
	BYTE	m_bySensitivity;
	BOOL	m_bChkPTZAlarm;
	//}}AFX_DATA

public:
	//结构体的部分成员
	BOOL m_bRecordChan[MAX_CHANNUM_V30 *8];
	NET_DVR_SCHEDTIME m_struAlarmTime[MAX_DAYS][MAX_TIMESEGMENT_V30]; 
	NET_DVR_HANDLEEXCEPTION_V40 m_struAlarmHandleType;
	BOOL m_bAlarmOut[MAX_ALARMOUT_V30];
    
    NET_DVR_HOLIDAY_HANDLE m_struHolidayHandle;

	//完整的参数
	NET_VCA_TRAVERSE_PLANE_DETECTION m_struLineDetection;
	NET_VCA_FIELDDETECION m_struFieldDetection;
	NET_VCA_DEFOCUSPARAM m_struDefousDetection;
	NET_DVR_AUDIO_EXCEPTION m_struAudioException;
	NET_DVR_SCENECHANGE_DETECTION m_struSceneDetection;//场景变更侦测2013-07-17

	LONG m_lHandle;
	void OnGetPtzLockInfo();
	NET_DVR_PTZ_LOCKCFG m_struPtzLockCfg;
	NET_DVR_PTZLOCKINFO_COND m_struPtzLockInfoCond;
	NET_DVR_PTZLOCKINFO m_struPtzLockInfo;
	LPNET_DVR_PTZLOCKINFO m_pStruPtzLockInfo;
	static DWORD WINAPI GetPtzLockInfoThread(LPVOID lpPtzLockInfo);
	void SetPtzLockTime(const int i);

	//保存当前的索引值
	int m_iLineNo;
	int m_iFieldNo;

	//播放
	void Play();
	void F_DrawFun(long lPlayHandle, HDC hDc, DWORD dwUser);

	//填写当前声音强度
	void SetAudioDecibel(const int &i);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIPCSimpIntellCfg)
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	//当前已经画的点的个数
	DWORD m_dwPosNum; 
	BOOL m_bCloseIn;
	BOOL m_bMouseMovePolygon;
	BOOL m_bMouseMove;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIPCSimpIntellCfg)
	afx_msg void OnClickTreeChan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnAlarmInTimeOk();
	afx_msg void OnCbnSelchangeComboAlarminWeekday();
	afx_msg void OnBnClickedBtnAlarminTimeCopy();
	afx_msg void OnBnClickedChkAlarminInvokeAlarmout();
	afx_msg void OnClickTreeAlarmOut(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCfgTypeChanged();
    afx_msg void OnChannelChanged();
	afx_msg void OnPaint();
	afx_msg void OnBtnSavecfg();
    afx_msg void OnBtnExit();
    afx_msg void OnBtnRefresh();
	afx_msg void OnLineNoChanged();
	afx_msg void OnFieldNoChanged();
	afx_msg void OnBtnClearpic();
	afx_msg void OnBtnPtzLockstatusSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	BOOL TimeTest();
	void CreateTree();

	//显示数据
	void ShowData();
	//保存界面数据
	void SaveUIData();

    //add channel Info
    void AddChanInfo();
    void AddGroupNoInfo();
	//获取参数
	void GetLineDetection();
	void GetFieldDetection();
	void GetDefousDetection();
	void GetAudioException();
	void GetSceneDetection();
	void SaveLineDetection();
	void SaveFieldDetection();
	void SaveDefousDetection();
	void SaveAudioException();
	void SaveSceneDetection();
	//
	BOOL PreDrawPolygonMsg(MSG* pMsg, CPoint &pt);
	BOOL PreDrawLineMsg(MSG* pMsg, CPoint &pt);

	void F_DrawLine(HDC hDc);
	void F_DrawPolygon(HDC hDc);

	//保存音频实时上传连接的handle
	int m_lAudioHandle;
};

BOOL IsCrossLine(NET_VCA_POLYGON *struRuleRegion);
BOOL F_LineCrossDetect(NET_VCA_POINT p1, NET_VCA_POINT p2, NET_VCA_POINT q1, NET_VCA_POINT q2);
BOOL IsValidArea(LPNET_VCA_POLYGON lpstruPolygon);
BOOL F_IsStraightLine(NET_VCA_POLYGON *lpstruPolygon);
BOOL F_IsACrossLine(NET_VCA_POLYGON *alarm_region);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIPCSIMPINTELLCFG_H__07C66E2C_FC33_40A0_B793_8D837845ACD9__INCLUDED_)
