#if !defined(AFX_DLGALARMHOSTGPRS_H__07B0ED83_9CCE_4F2F_8253_905CC913986E__INCLUDED_)
#define AFX_DLGALARMHOSTGPRS_H__07B0ED83_9CCE_4F2F_8253_905CC913986E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAlarmHostGPRS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmHostGPRS dialog

class CDlgAlarmHostGPRS : public CDialog
{
// Construction
public:
	CDlgAlarmHostGPRS(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAlarmHostGPRS)
	enum { IDD = IDD_DLG_ALARMHOST_GPRS };
	CIPAddressCtrl	m_ipSIM;
	CIPAddressCtrl	m_ipCenter;
	CComboBox	m_cmProtocolType;
	CComboBox	m_cmCenter;
	CString	m_csAPNPassword;
	CString	m_csAPNUserName;
	CString	m_csAPNName;
	BYTE	m_byDetectLinkTime;
	CString	m_csDevID;
	BYTE	m_byOverTime;
	int		m_iPort;
	BYTE	m_byReconnectTime;
	CString	m_csSIMNum;
	CString	m_csDomain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlarmHostGPRS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iDeviceIndex;
	LONG m_lUserID;
	char szLan[1024];
	NET_DVR_ALARMHOST_WIRELESS_NETWORK_CFG m_struGPRSCfg;
	NET_DVR_ALARMHOST_ABILITY m_struAbility;
	void SetGPRSParaToDlg(LPNET_DVR_ALARMHOST_WIRELESS_NETWORK_CFG lpInter);
	void GetGPRSParaFromDlg();
	// Generated message map functions
	//{{AFX_MSG(CDlgAlarmHostGPRS)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnGet();
	afx_msg void OnSelchangeComboCenter();
	afx_msg void OnSelchangeComboProtocoltype();
	afx_msg void OnBtnSet();
	afx_msg void OnBtnSure();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGALARMHOSTGPRS_H__07B0ED83_9CCE_4F2F_8253_905CC913986E__INCLUDED_)
