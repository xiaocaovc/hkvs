#if !defined(AFX_SUBDLGDVRNVRCFG_H__40EF252C_40D9_4B8C_BF5B_49840F545BE9__INCLUDED_)
#define AFX_SUBDLGDVRNVRCFG_H__40EF252C_40D9_4B8C_BF5B_49840F545BE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SubDlgDVRNVRCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSubDlgDVRNVRCfg dialog

class CSubDlgDVRNVRCfg : public CDialog
{
// Construction
public:
	CSubDlgDVRNVRCfg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSubDlgDVRNVRCfg)
	enum { IDD = IDD_SUBDLG_PRODUCT_NVRDVR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubDlgDVRNVRCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSubDlgDVRNVRCfg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnNetService();
	afx_msg void OnBtnCert();
	afx_msg void OnBtnEventRecord();
	afx_msg void OnBtnAudioinput();
	afx_msg void OnBtnVcaDetection();
	afx_msg void OnBtmSmd();
	afx_msg void OnBtnImExportIpcCfgFile();
	afx_msg void OnBtnVqdCfg();
	afx_msg void OnBtnNvrcfg();
	afx_msg void OnBtnGetIPCStatus();
	afx_msg void OnBtnUpgradeIpc();
	afx_msg void OnBtnAtm();
	afx_msg void OnBtnInquest();
    afx_msg void OnBtnVehicleCfg();
	afx_msg void OnBtnWorkmode();
    afx_msg void OnBtnGetGopInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    void CurCfgUpdate();
public:
    int m_lUserID;
    int m_lChanNo;
    int m_iDevIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBDLGDVRNVRCFG_H__40EF252C_40D9_4B8C_BF5B_49840F545BE9__INCLUDED_)
