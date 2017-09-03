#if !defined(AFX_DLGVCAITCTRIGGERCFG_H__26EC1A7B_5A97_4F7A_83CA_340E7CACB8D0__INCLUDED_)
#define AFX_DLGVCAITCTRIGGERCFG_H__26EC1A7B_5A97_4F7A_83CA_340E7CACB8D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VcaCommonOperations.h"
// DlgVcaITCTriggerCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVcaITCTriggerCfg dialog

class CDlgVcaITCTriggerCfg : public CDialog,public CVcaCommonOperations
{
// Construction
public:
	CDlgVcaITCTriggerCfg(CWnd* pParent = NULL);   // standard constructor
    enum
	{
		DRAW_TYPE_NONE      = 0,          //不画图
	    DRAW_TYPE_LANE_BOUNDARY_LINE = 1, //车道边界线
        DRAW_TYPE_LANE_LINE   = 2,        //车道线
        DRAW_TYPE_PLATE_RECOG_RGN   = 3   //牌识区别
	};
// Dialog Data
	//{{AFX_DATA(CDlgVcaITCTriggerCfg)
	enum { IDD = IDD_DLG_VCA_ITC_TRIGGER_CFG };
	CComboBox	m_comboVehicleType;
	CComboBox	m_comboTimeType;
	CComboBox	m_comboSceneType;
	CComboBox	m_comboRecogType;
	CComboBox	m_comboRecogPos;
	CComboBox	m_comboLocateType;
	CComboBox	m_comboLaneUseageType;
	CComboBox	m_comboLaneNumber;
	CComboBox	m_comboLaneDirection;
	CComboBox	m_comboDriveDirection;
	BOOL	m_bFarmVehicleRecog;
	BOOL	m_bMicroPlateRecog;
	BOOL	m_bPlateRecog;
	BOOL	m_bTriggerCfg;
	BOOL	m_bVehicleColorRecog;
	BOOL	m_bVehicleLogoRecog;
	BOOL	m_bViaVtcoil;
	CString	m_strProvinceName;
	BOOL	m_bFuzzyRecog;
	BOOL	m_bMotocarRecog;
	int		m_nAssociateLaneNO;
	BYTE	m_nLaneNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVcaITCTriggerCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVcaITCTriggerCfg)
	afx_msg void OnDestroy();
	afx_msg void OnBtnDrawLaneBoundaryLine();
	afx_msg void OnBtnDrawLaneLine();
	afx_msg void OnBtnDrawPlateRecogRgn();
	afx_msg void OnSelchangeComboLaneNumber();
	afx_msg void OnBtnSave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);;
	afx_msg void OnBtnEndDraw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void DrawRgnFun(HDC hdc);
protected:
	void InitCtrlState();
protected:
	void LoadWndContent();
	void SaveWndContent();
    
	void LoadViaLaneWndContent(int nLaneIndex);
	void SaveViaLaneWndContent(int nLaneIndex);
	BOOL GetITCTriggerCfg();
	BOOL SetITCTriggerCfg();
private:
	CRect m_rcPlayWnd;
	NET_ITC_TRIGGERCFG m_struItcTriggerCfg;
	int m_nCurDrawType;
	int m_nCurLaneIndex;
	int m_nDrawPointNum;
	BOOL m_bFirstDrawLaneBoundary;
	BOOL m_bFirstDrawLaneLine;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVCAITCTRIGGERCFG_H__26EC1A7B_5A97_4F7A_83CA_340E7CACB8D0__INCLUDED_)
