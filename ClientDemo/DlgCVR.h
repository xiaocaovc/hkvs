#if !defined(AFX_DLGCVR_H__6C7FA515_0BBA_4CA7_9DFF_E4B253A226DC__INCLUDED_)
#define AFX_DLGCVR_H__6C7FA515_0BBA_4CA7_9DFF_E4B253A226DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCVR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCVR dialog

#define ONE_KEY_CONFIG_TIMER		WM_USER+100		//timer for one key config

class CDlgCVR : public CDialog
{
// Construction
public:
	CDlgCVR(CWnd* pParent = NULL);   // standard constructor

	inline BOOL ValidIPv6(BYTE *ip);

// Dialog Data
	//{{AFX_DATA(CDlgCVR)
	enum { IDD = IDD_DLG_CVR };
	CComboBox	m_cmbWorkMode;
	CListCtrl	m_lstAddedMachine;
	CListCtrl	m_lstFC;
	CComboBox	m_cmbAccModeIscsi;
	CComboBox	m_cmbFCAccMode;
	CComboBox	m_cmbDelType;
	CComboBox	m_cmbAddType;
	CString	m_strStatus;
	UINT	m_dwMaxPCNum;
	BOOL	m_bDelData;
	BOOL	m_bEnableDataCallback;
	CString	m_strIP;
	CString	m_strCardSN;
	CString	m_strClientIP;
	INT		m_iFCChannel;
	UINT	m_dwFCID;
	UINT	m_dwLunIDFC;
	UINT	m_dwLunIDIscsi;
	UINT	m_dwTargetID;
	UINT	m_dwLunIDNas;
	CString	m_strPassword;
	BOOL	m_bDisableBackup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCVR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCVR)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRestartService();
	afx_msg void OnButtonOneKeyCfg();
#if (_MSC_VER >= 1500)	//vs2008
    afx_msg void OnTimer(UINT_PTR nIDEvent);
#else
    afx_msg void OnTimer(UINT nIDEvent);
#endif
	afx_msg void OnButtonGetMaxPcNum();
	afx_msg void OnButtonSetMaxPcNum();
	afx_msg void OnButtonGetDataCallback();
	afx_msg void OnButtonSetDataCallback();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonOpenIscsi();
	afx_msg void OnButtonCloseIscsi();
	afx_msg void OnButtonOpenFc();
	afx_msg void OnButtonCloseFc();
	afx_msg void OnButtonCreateNas();
	afx_msg void OnButtonDelNas();
	afx_msg void OnButtonGetFc();
	afx_msg void OnButtonGetAddedMachine();
	afx_msg void OnButtonGetWorkMode();
	afx_msg void OnButtonSetWorkMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	LONG m_lUserID;
	int m_iDevIndex;

	LONG m_lOneKeyCfgHandle;
	LONG m_lOneKeyCfgTimer;

	NET_DVR_MACHINE_MAX_NUM_CFG m_struMachineMaxNum;
	NET_DVR_DATA_CALLBACK_CFG	m_struDataCallback;

	BOOL m_bFCGeting;
	BOOL m_bAddedMachineGeting;
	LONG m_hRemoteConfig;
	LONG m_hAddedMachineRemoteConfig;
	HANDLE m_hThread;
	HANDLE m_hAddedMachineThread;
	int m_iFCNum;
	int m_iMachineNum;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCVR_H__6C7FA515_0BBA_4CA7_9DFF_E4B253A226DC__INCLUDED_)
