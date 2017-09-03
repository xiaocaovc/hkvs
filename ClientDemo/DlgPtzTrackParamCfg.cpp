// DlgPtzTrackParamCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPtzTrackParamCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPtzTrackParamCfg dialog


CDlgPtzTrackParamCfg::CDlgPtzTrackParamCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPtzTrackParamCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPtzTrackParamCfg)
	m_nTrackTime = 0;
	//}}AFX_DATA_INIT
	m_lServerID = -1;
	m_lChannel = -1;
	m_iDevIndex = -1;
	memset(&m_struPtzTrackParam,0,sizeof(m_struPtzTrackParam));
	m_struPtzTrackParam.dwSize = sizeof(m_struPtzTrackParam);
}


void CDlgPtzTrackParamCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPtzTrackParamCfg)
	DDX_Control(pDX, IDC_COMBO_TRACKMODE, m_comboTrackMode);
	DDX_Text(pDX, IDC_EDIT_TRACKTIME, m_nTrackTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPtzTrackParamCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgPtzTrackParamCfg)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPtzTrackParamCfg message handlers


BOOL CDlgPtzTrackParamCfg::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitCtrlState();

	if (GetPtzTrackParam())
	{
		LoadWndContent();
	}

	return TRUE;
}

void CDlgPtzTrackParamCfg::InitCtrlState()
{
	m_comboTrackMode.ResetContent();
     
	char szLan[32] = "\0";
	g_StringLanType(szLan,"一直跟踪","Track Always");
	m_comboTrackMode.InsertString(0,szLan);
	m_comboTrackMode.SetItemData(0,PTZ_TRACK_MODE_ALWAYS);
	g_StringLanType(szLan,"跟踪指定时间","Track a Specified Time");
	m_comboTrackMode.InsertString(1,szLan);
	m_comboTrackMode.SetItemData(1,PTZ_TRACK_MODE_BYTIME);
	g_StringLanType(szLan,"跟踪下一目标","Track Next Target");
	m_comboTrackMode.InsertString(2,szLan);
	m_comboTrackMode.SetItemData(2,PTZ_TRACK_MODE_NEXT);
}

BOOL CDlgPtzTrackParamCfg::GetPtzTrackParam()
{
	BOOL bRet      = FALSE;
	DWORD dwReturn = 0;
	
    if (!NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_PTZ_TRACK_PARAM,m_lChannel,&m_struPtzTrackParam,
		sizeof(m_struPtzTrackParam),&dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_PTZ_TRACK_PARAM Chan[%d]", m_lChannel);
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_PTZ_TRACK_PARAM Chan[%d]", m_lChannel);
        bRet = TRUE;
    }
	return bRet;
}

BOOL CDlgPtzTrackParamCfg::SetPtzTrackParam()
{
	BOOL bRet      = FALSE;
	
    if (!NET_DVR_SetDVRConfig(m_lServerID, NET_DVR_SET_PTZ_TRACK_PARAM,m_lChannel,&m_struPtzTrackParam,
		sizeof(m_struPtzTrackParam)))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_SET_PTZ_TRACK_PARAM Chan[%d]", m_lChannel);
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_SET_PTZ_TRACK_PARAM Chan[%d]", m_lChannel);
        bRet = TRUE;
    }
	return bRet;
}

void CDlgPtzTrackParamCfg::OnBtnSave() 
{
	SaveWndContent();
	char szError[32] = "\0";
	if (SetPtzTrackParam())
	{
        g_StringLanType(szError, "保存成功","Saved successfully");
	}
	else
	{
		g_StringLanType(szError, "保存失败","Save failed");
	}
	AfxMessageBox(szError);
}


void CDlgPtzTrackParamCfg::LoadWndContent()
{
	m_nTrackTime = m_struPtzTrackParam.dwTrackTime;
	int nCount = m_comboTrackMode.GetCount();

	for (int i = 0; i < nCount; i++)
	{
		if (m_comboTrackMode.GetItemData(i) == m_struPtzTrackParam.byTrackMode)
		{
			m_comboTrackMode.SetCurSel(i);
			break;
		}
	}

	UpdateData(FALSE);
}

void CDlgPtzTrackParamCfg::SaveWndContent()
{
	UpdateData(TRUE);

	m_struPtzTrackParam.dwSize = sizeof(m_struPtzTrackParam);
	m_struPtzTrackParam.byTrackMode = m_comboTrackMode.GetItemData(m_comboTrackMode.GetCurSel());
	m_struPtzTrackParam.dwTrackTime = m_nTrackTime;
}