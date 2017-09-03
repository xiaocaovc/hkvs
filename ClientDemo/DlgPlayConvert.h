#if !defined(AFX_DLGPLAYCONVERT_H__7DEA740B_CCC4_44F5_9259_6A768CD15839__INCLUDED_)
#define AFX_DLGPLAYCONVERT_H__7DEA740B_CCC4_44F5_9259_6A768CD15839__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlayConvert.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgPlayConvert dialog

class DlgPlayConvert : public CDialog
{
// Construction
public:
	DlgPlayConvert(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgPlayConvert)
	enum { IDD = IDD_DLG_PLAY_CONVERT };
	CComboBox	m_StreamType;
	CComboBox	m_comFormatType;
	CComboBox	m_comVideoEncComplexity;
	CComboBox	m_comVideoFrameRate;
	CComboBox	m_comVideoEnctype;
	CComboBox	m_comVideoBitrate;
	CComboBox	m_comResolution;
	CComboBox	m_comIntervalBPFrame;
	CComboBox	m_comBitrateType;
	CComboBox	m_comPicQuality;
	CComboBox	m_comAudioEnctype;
	short	m_IntervalFrameI;
	BOOL	m_bEnableSVC;
	BYTE	m_byENumber;
	//}}AFX_DATA

  	LONG m_lPlayHandle;
 	void Init(LONG &lplayhandle);
// 	LONG m_lLoginID;
// 	NET_DVR_VOD_PARA m_struVodPara;
// 	int m_iResult;
// 	HWND m_hPlayBackWnd;
// 	NET_DVR_PLAYCOND m_struPlayCon;
	void getComPressionInfoV30(NET_DVR_COMPRESSION_INFO_V30 *struCompressionInfoV30);
	NET_DVR_COMPRESSION_INFO_V30 m_struCompressionInfoV30;
	NET_DVR_COMPRESSIONCFG_ABILITY *m_pCompresscfgAbility;
	DWORD m_iDeviceIndex;
	BOOL OnInitDialog();
	BOOL GetCompressCfgAbility(DWORD dwIPChanIndex);
	void GetResolutionAbility();
	void GetFrameAbility();
	void GetBitrateAbility();
	DWORD FrameIIntervalMap(DWORD nSelFrameRate);
	DWORD m_iFrameIInterval;
    DWORD m_dwCurChanNo;
	char *m_pRecvBuf;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgPlayConvert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	
	// Generated message map functions
	//{{AFX_MSG(DlgPlayConvert)
	afx_msg void OnSet();
	afx_msg void OnEditchangeComResolution();
	afx_msg void OnEditchangeComVedioFrameRate();
	afx_msg void OnSelchangeComVedioFrameRate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLAYCONVERT_H__7DEA740B_CCC4_44F5_9259_6A768CD15839__INCLUDED_)
