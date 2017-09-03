#if !defined(AFX_DLGINQUESTEVENT_H__77C34137_8643_4323_91A7_E2F10EE18B33__INCLUDED_)
#define AFX_DLGINQUESTEVENT_H__77C34137_8643_4323_91A7_E2F10EE18B33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInquestEvent.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInquestEvent dialog

class CDlgInquestEvent : public CDialog
{
// Construction
public:
	CDlgInquestEvent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInquestEvent)
	enum { IDD = IDD_DLG_INQUEST_EVENT };
	CListCtrl	m_listEvent;
	CComboBox	m_comboMinorEvent;
	CComboBox	m_comboMajorEvent;
	CTime	m_DateStart;
	CTime	m_DateStop;
	CTime	m_TimeStart;
	CTime	m_TimeStop;
	//}}AFX_DATA

	BOOL m_bSearching;
	BOOL m_bResuming;
	HANDLE m_hFileThread;
	LONG m_lFileHandle;
	NET_DVR_SEARCH_EVENT_PARAM m_struEventParam;
	LONG m_lLoginID;
	int m_iDeviceIndex;
	BYTE m_byRoomIndex;

	LONG m_lResumeHandle;
	HANDLE m_hResumeThread;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInquestEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInquestEvent)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnBtnSearch();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonResume();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINQUESTEVENT_H__77C34137_8643_4323_91A7_E2F10EE18B33__INCLUDED_)
