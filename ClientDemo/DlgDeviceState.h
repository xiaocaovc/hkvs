#pragma once
#include "afxcmn.h"


// CDlgDeviceState dialog

class CDlgDeviceState : public CDialog
{
	DECLARE_DYNAMIC(CDlgDeviceState)

public:
	CDlgDeviceState(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDeviceState();

// Dialog Data
	

public:
	//{{AFX_DATA(CDlgDeviceState)
	enum { IDD = IDD_DLG_DEVICE_STATE };
	CString m_csLocalNodeName;
	CString m_csDeviceState;
	CListCtrl m_listChanState;
	CListCtrl m_listDiskState;
	CString m_csDeviceIP;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CDlgDeviceState)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgDeviceState)
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnLinkStatus();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
		
public:
	
	int m_iDeviceIndex;
	CString m_csRecState;
	CString m_csSignalState;
	CString m_csHardWareState;
	CString m_csLinkCount;
	CString m_csBitrate;
	CString m_csDiskVolume;
	CString m_csDiskFreeSpace;
	CString m_csDiskState;
	BOOL CheckInitParam(void);
	LONG m_lLoginID;
	
	
};
