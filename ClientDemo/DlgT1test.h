#if !defined(AFX_DLGT1TEST_H__59E27DF9_2934_4949_87FF_50832267D3A8__INCLUDED_)
#define AFX_DLGT1TEST_H__59E27DF9_2934_4949_87FF_50832267D3A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgT1test.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgT1test dialog

class CDlgT1test : public CDialog
{
// Construction
public:
	CDlgT1test(CWnd* pParent = NULL);   // standard constructor
	int m_iDeviceIndex;
	void appendInfo(void* pBuffer, DWORD dwBufSize);

// Dialog Data
	//{{AFX_DATA(CDlgT1test)
	enum { IDD = IDD_DLG_T1TEST };
	CEdit	m_editNetSpeed;
	CEdit	m_editInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgT1test)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgT1test)
	afx_msg void OnStartT1TestClick();
	afx_msg void OnDefaultShutdownClick();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	NET_DVR_ALARMHOST_DOWNLOAD_PARAM m_struDownload;
	LONG m_lDownloadHandle;
	CString m_strTiTestInfo;

private:
	CString GetLineAndReplace(CString csStr);
	void GetNetWorkStatus();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGT1TEST_H__59E27DF9_2934_4949_87FF_50832267D3A8__INCLUDED_)
