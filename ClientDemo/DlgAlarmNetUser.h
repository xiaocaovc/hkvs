#if !defined(AFX_DLGALARMNETUSER_H__0BC9453B_F049_4B44_B726_A614A602D8B4__INCLUDED_)
#define AFX_DLGALARMNETUSER_H__0BC9453B_F049_4B44_B726_A614A602D8B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAlarmNetUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmNetUser dialog

class CDlgAlarmNetUser : public CDialog
{
// Construction
public:
	CDlgAlarmNetUser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAlarmNetUser)
	enum { IDD = IDD_DLG_ALARMHOST_NET_USER };
	CListCtrl	m_listChan;
	CComboBox	m_cmRightType;
	CComboBox	m_comboUserType;
	CComboBox	m_comboUserIndex;
	BOOL	m_bAlarmOff;
	BOOL	m_bAlarmOn;
	BOOL	m_bBypass;
	BOOL	m_bDefault;
	BOOL	m_bGetParam;
	BOOL	m_bLog;
	BOOL	m_bRestartShuntdown;
	BOOL	m_bSetParam;
	BOOL	m_bSiren;
	BOOL	m_bUpgrade;
	CString	m_csPassword;
	CString	m_csUserName;
	CString	m_csUserIP;
	CString	m_csMacAddr1;
	CString	m_csMacAddr2;
	CString	m_csMacAddr3;
	CString	m_csMacAddr4;
	CString	m_csMacAddr5;
	CString	m_csMacAddr6;
	BOOL	m_bAlarmoutCtrl;
	BOOL	m_bAudioTalk;
	BOOL	m_bDiskCfg;
	BOOL	m_bFormatDisk;
	BOOL	m_bGatewayCtrl;
	BOOL	m_bSensorCtrl;
	BOOL	m_bSerialCtrl;
	BOOL	m_bLocalCtrl;
	BOOL	m_bPtzControl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlarmNetUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlarmNetUser)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboUserIndex();
	afx_msg void OnBtnSetNetUser();
	afx_msg void OnSelchangeComboUsertype();
	afx_msg void OnSelchangeComboRightType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    LONG    m_lServerID;
    int     m_iDevIndex;
    DWORD   m_dwNetUserNum;

    LONG    m_lUserIndex;
    NET_DVR_ALARM_DEVICE_USER m_struDeviceUser;

public:
    void InitWnd();
    void InitUserIndexCombo();   
    void InitUserTypeCombo();

    BOOL GetDeviceUser(NET_DVR_ALARM_DEVICE_USER &struDeviceUser);
    BOOL SetDeviceUser(NET_DVR_ALARM_DEVICE_USER &struDeviceUser);


    void GetDeviceUserFromWnd(NET_DVR_ALARM_DEVICE_USER &struDeviceUser);
    void SetDeviceUserToWnd(NET_DVR_ALARM_DEVICE_USER &struDeviceUser);

    void UserTypeEnable();
protected:
	void InitRightTypeCombo();
	void InitRightList();
	void GetChanInfoFromWnd();
	void SetChanInfoToDlg();
	long m_lChanNum;
	int	 m_iCurChanIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGALARMNETUSER_H__0BC9453B_F049_4B44_B726_A614A602D8B4__INCLUDED_)
