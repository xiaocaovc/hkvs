#if !defined(AFX_DLGPOSCHANFILTER_H__4272EF55_9569_470E_86FE_757CB3E4B397__INCLUDED_)
#define AFX_DLGPOSCHANFILTER_H__4272EF55_9569_470E_86FE_757CB3E4B397__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPosChanFilter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPosChanFilter dialog

class CDlgPosChanFilter : public CDialog
{
// Construction
public:
	CDlgPosChanFilter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPosChanFilter)
	enum { IDD = IDD_DLG_POS_CHAN_FLTERCFG };
	CComboBox	m_comboConnectMode;
	CComboBox	m_comboTheFilterNum;
	CComboBox	m_comboGroup;
	CComboBox	m_comboChannel;
	CString	m_csPosFilterName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPosChanFilter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPosChanFilter)
	afx_msg void OnBtnRefresh();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboGroup();
	afx_msg void OnBtnSet();
	afx_msg void OnBtnGet();
	afx_msg void OnSelchangeComboChannel();
	afx_msg void OnEditchangeComboThefilternum();
	afx_msg void OnSelchangeComboThefilternum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void GetFilterCfgInfo(UINT dwIndex);
	void AddChanInfo();
	void ShowPosFilterInfo( UINT dwTheFilterNum)  ; //dwTheFilterNum ��0��ʼ
public:
	int m_iUserID;
	int m_iDevIndex;
	DWORD m_dwCurselectIndx;
    NET_DVR_CHAN_FILTER_CFG m_struChanFilterCfg;
	NET_DVR_POS_FILTER_CFG m_struPosFilterCfg;
	NET_DVR_IPPARACFG_V40 m_struIPParaCfgV40; 

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPOSCHANFILTER_H__4272EF55_9569_470E_86FE_757CB3E4B397__INCLUDED_)
