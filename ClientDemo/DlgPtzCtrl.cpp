// DlgPtzCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPtzCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPtzCtrl dialog


CDlgPtzCtrl::CDlgPtzCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPtzCtrl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPtzCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPtzCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPtzCtrl)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPtzCtrl, CDialog)
	//{{AFX_MSG_MAP(CDlgPtzCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPtzCtrl message handlers

BOOL CDlgPtzCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btnPtzUp.SubclassDlgItem(IDC_BTN_PTZ_UP,this);
    m_btnPtzDown.SubclassDlgItem(IDC_BTN_PTZ_DOWN,this);
    m_btnPtzLeft.SubclassDlgItem(IDC_BTN_PTZ_LEFT,this);
    m_btnPtzRight.SubclassDlgItem(IDC_BTN_PTZ_RIGHT,this);	
    m_btnZoomOut.SubclassDlgItem(IDC_BTN_ZOOM_OUT,this);
    m_btnZoomIn.SubclassDlgItem(IDC_BTN_ZOOM_IN,this);
    m_btnFocusNear.SubclassDlgItem(IDC_BTN_FOCUS_NEAR,this);
    m_btnFocusFar.SubclassDlgItem(IDC_BTN_FOCUS_FAR,this);
    m_btnIrisOpen.SubclassDlgItem(IDC_BTN_IRIS_OPEN,this);
    m_btnIrisClose.SubclassDlgItem(IDC_BTN_IRIS_CLOSE,this);

    m_btnPtzUp.m_iPTZType = TILT_UP;
    m_btnPtzDown.m_iPTZType = TILT_DOWN;
    m_btnPtzLeft.m_iPTZType = PAN_LEFT;
    m_btnPtzRight.m_iPTZType = PAN_RIGHT;
    m_btnZoomOut.m_iPTZType = ZOOM_OUT;
    m_btnZoomIn.m_iPTZType = ZOOM_IN;
    m_btnFocusNear.m_iPTZType = FOCUS_NEAR;
    m_btnFocusFar.m_iPTZType = FOCUS_FAR;
    m_btnIrisOpen.m_iPTZType = IRIS_OPEN;
    m_btnIrisClose.m_iPTZType = IRIS_CLOSE;

    m_btnPtzUp.m_lPlayHandle = m_lPlayHandle;
    m_btnPtzDown.m_lPlayHandle = m_lPlayHandle;
    m_btnPtzLeft.m_lPlayHandle = m_lPlayHandle;
    m_btnPtzRight.m_lPlayHandle = m_lPlayHandle;
    m_btnZoomOut.m_lPlayHandle = m_lPlayHandle;
    m_btnZoomIn.m_lPlayHandle = m_lPlayHandle;	
    m_btnFocusNear.m_lPlayHandle = m_lPlayHandle;
    m_btnFocusFar.m_lPlayHandle = m_lPlayHandle;
    m_btnIrisOpen.m_lPlayHandle = m_lPlayHandle;
    m_btnIrisClose.m_lPlayHandle = m_lPlayHandle;


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
