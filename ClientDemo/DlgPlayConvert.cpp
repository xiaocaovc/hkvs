// DlgPlayConvert.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgPlayConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgPlayConvert dialog


DlgPlayConvert::DlgPlayConvert(CWnd* pParent /*=NULL*/)
	: CDialog(DlgPlayConvert::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgPlayConvert)
	m_IntervalFrameI = 0;
	m_bEnableSVC = FALSE;
	m_byENumber = 0;
	//}}AFX_DATA_INIT
	memset(&m_struCompressionInfoV30,0,sizeof(m_struCompressionInfoV30));
	m_pCompresscfgAbility = new NET_DVR_COMPRESSIONCFG_ABILITY;
	m_iFrameIInterval = 0;
	m_lPlayHandle = -1;
    m_dwCurChanNo = 1;
}


void DlgPlayConvert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgPlayConvert)
	DDX_Control(pDX, IDC_STREAM_TYPE, m_StreamType);
	DDX_Control(pDX, IDC_COM_FORMAT_TYPE, m_comFormatType);
	DDX_Control(pDX, IDC_COM_VIDEO_ENC_COMPLEXITY, m_comVideoEncComplexity);
	DDX_Control(pDX, IDC_COM_VEDIO_FRAME_RATE, m_comVideoFrameRate);
	DDX_Control(pDX, IDC_COM_VEDIO_ENCTYPE, m_comVideoEnctype);
	DDX_Control(pDX, IDC_COM_VEDIO_BITRATE, m_comVideoBitrate);
	DDX_Control(pDX, IDC_COM_RESOLUTION, m_comResolution);
	DDX_Control(pDX, IDC_COM_INTERVAL_BP_FRAME, m_comIntervalBPFrame);
	DDX_Control(pDX, IDC_COM_BITRATE_TYPE, m_comBitrateType);
	DDX_Control(pDX, IDC_COM_PIC_QUALITY, m_comPicQuality);
	DDX_Control(pDX, IDC_COM_AUDIO_ENCTYPE, m_comAudioEnctype);
	DDX_Text(pDX, IDC_INTERVAL_FRAME_I, m_IntervalFrameI);
	DDX_Check(pDX, IDC_ENABLE_SVC, m_bEnableSVC);
	DDX_Text(pDX, IDC_E_NUMBER, m_byENumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgPlayConvert, CDialog)
	//{{AFX_MSG_MAP(DlgPlayConvert)
	ON_BN_CLICKED(IDC_SET, OnSet)
	ON_CBN_SELCHANGE(IDC_COM_VEDIO_FRAME_RATE, OnSelchangeComVedioFrameRate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgPlayConvert message handlers
void DlgPlayConvert::Init(LONG &lplayhandle)
{
	m_lPlayHandle = lplayhandle;
}

BOOL DlgPlayConvert::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	DWORD iCurChanIndex = g_pMainDlg->GetCurChanIndex();
	 
	if (GetCompressCfgAbility(m_dwCurChanNo))
    {
        //GetStreamTypeAbility();
        GetResolutionAbility();
        GetBitrateAbility();
        GetFrameAbility();
    }
    else
    {
        AfxMessageBox("Get compression ability failed");
    }

    m_comResolution.SetCurSel(0);
    m_comVideoBitrate.SetCurSel(0);
    m_comVideoFrameRate.SetCurSel(0);

	return TRUE; 
}

void DlgPlayConvert::GetResolutionAbility()
{
    for (int nAbilityCount = 0; nAbilityCount < m_pCompresscfgAbility->dwAbilityNum; nAbilityCount++)
    {
        if (MAIN_RESOLUTION_ABILITY == m_pCompresscfgAbility->struAbilityNode[nAbilityCount].dwAbilityType)
        {
            m_comResolution.ResetContent();
            for (int i = 0; i < m_pCompresscfgAbility->struAbilityNode[nAbilityCount].dwNodeNum; i++)
            {
                m_comResolution.AddString((char*)m_pCompresscfgAbility->struAbilityNode[nAbilityCount].struDescNode[i].byDescribe);
                m_comResolution.SetItemData(i, m_pCompresscfgAbility->struAbilityNode[nAbilityCount].struDescNode[i].iValue);
            }
        }
    }
}

BOOL DlgPlayConvert::GetCompressCfgAbility(DWORD dwIPChanIndex)
{
    if (!NET_DVR_GetDeviceAbility(g_struDeviceInfo[m_iDeviceIndex].lLoginID, COMPRESSIONCFG_ABILITY, (char*)&dwIPChanIndex, sizeof(dwIPChanIndex), (char*)m_pCompresscfgAbility, sizeof(NET_DVR_COMPRESSIONCFG_ABILITY)))
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "COMPRESSIONCFG_ABILITY");
        return FALSE;    
    }
    else
    {
        g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "COMPRESSIONCFG_ABILITY");
       
        return TRUE;
    }
}

void DlgPlayConvert::GetFrameAbility()
{
    for (int nAbilityCount = 0; nAbilityCount < m_pCompresscfgAbility->dwAbilityNum; nAbilityCount++)
    {
        if (FRAME_ABILITY == m_pCompresscfgAbility->struAbilityNode[nAbilityCount].dwAbilityType)
        {
            m_comVideoFrameRate.ResetContent();
            for (int i = 0; i < m_pCompresscfgAbility->struAbilityNode[nAbilityCount].dwNodeNum; i++)
            {
                m_comVideoFrameRate.AddString((char*)m_pCompresscfgAbility->struAbilityNode[nAbilityCount].struDescNode[i].byDescribe);
                m_comVideoFrameRate.SetItemData(i, m_pCompresscfgAbility->struAbilityNode[nAbilityCount].struDescNode[i].iValue);
            }
        }
    }
}

void DlgPlayConvert::GetBitrateAbility()
{
    for (int nAbilityCount = 0; nAbilityCount < m_pCompresscfgAbility->dwAbilityNum; nAbilityCount++)
    {
        if (BITRATE_ABILITY == m_pCompresscfgAbility->struAbilityNode[nAbilityCount].dwAbilityType)
        {
            m_comVideoBitrate.ResetContent();
            for (int i = 0; i < m_pCompresscfgAbility->struAbilityNode[nAbilityCount].dwNodeNum; i++)
            {
                m_comVideoBitrate.AddString((char*)m_pCompresscfgAbility->struAbilityNode[nAbilityCount].struDescNode[i].byDescribe);
                m_comVideoBitrate.SetItemData(i, m_pCompresscfgAbility->struAbilityNode[nAbilityCount].struDescNode[i].iValue);
            }
        }
    }
}

/*********************************************************
  Function:	FrameIIntervalMap
  Desc:		frame interval
  Input:	
  Output:	
  Return:	
**********************************************************/
DWORD DlgPlayConvert::FrameIIntervalMap(DWORD nSelFrameRate)
{
	DWORD nRet = 0;
	switch (nSelFrameRate)
	{
	case 0:
		nRet = 25;
		break;
	case 6:
		nRet = 2;
		break;
	case 7:
		nRet = 4;
		break;
	case 8:
		nRet = 6;
		break;
	case 9:
		nRet = 8;
		break;
	case 10:
		nRet = 10;
		break;
	case 11:
		nRet = 12;
		break;
	case 12:
		nRet = 16;
		break;
	case 13:
		nRet = 20;
		break;
	case 14:
		nRet = 15;
		break;
	case 15:
		nRet = 18;
		break;
	case 16:
		nRet = 22;
		break;
	default:
		nRet = 1;
	}
	nRet *= 4;
	return nRet;
}

void DlgPlayConvert::OnSet() 
{
	// TODO: Add your control notification handler code here
	
	m_struCompressionInfoV30.byAudioEncType = (BYTE)m_comAudioEnctype.GetCurSel();
	m_struCompressionInfoV30.byBitrateType = (BYTE)m_comBitrateType.GetCurSel();
	m_struCompressionInfoV30.byEnableSvc = m_bEnableSVC;
	
	if (m_comFormatType.GetCurSel() == 10)
	{
		m_struCompressionInfoV30.byFormatType = 0xff;
	}
	else
	{
		m_struCompressionInfoV30.byFormatType = (BYTE)m_comFormatType.GetCurSel();
	}
	if (m_comIntervalBPFrame.GetCurSel() == 3)
	{
		m_struCompressionInfoV30.byIntervalBPFrame = 0xfe;
	}
	else
	{
		m_struCompressionInfoV30.byIntervalBPFrame = (BYTE)m_comIntervalBPFrame.GetCurSel();
	}
	
	
	m_struCompressionInfoV30.byPicQuality = (BYTE)m_comPicQuality.GetCurSel();
	m_struCompressionInfoV30.byResolution = (BYTE)m_comResolution.GetItemData(m_comResolution.GetCurSel());
	m_struCompressionInfoV30.byStreamType = (BYTE)m_StreamType.GetCurSel();
	m_struCompressionInfoV30.byVideoEncComplexity = (BYTE)m_comVideoEncComplexity.GetCurSel();
	m_struCompressionInfoV30.byVideoEncType = (BYTE)m_comVideoEnctype.GetCurSel();
	m_struCompressionInfoV30.dwVideoBitrate = m_comVideoBitrate.GetItemData(m_comVideoBitrate.GetCurSel());
	m_struCompressionInfoV30.dwVideoFrameRate = m_iFrameIInterval;
	m_struCompressionInfoV30.wIntervalFrameI = (WORD)m_IntervalFrameI;

	char szLan[128] = {0};

    CDialog::OnOK();
}

void DlgPlayConvert::getComPressionInfoV30(NET_DVR_COMPRESSION_INFO_V30 *struCompressionInfoV30)
{
	*struCompressionInfoV30 = m_struCompressionInfoV30;
}

void DlgPlayConvert::OnSelchangeComVedioFrameRate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	WORD nSel = (WORD)m_comVideoFrameRate.GetCurSel();
	m_iFrameIInterval = FrameIIntervalMap(nSel);
	UpdateData(FALSE);
}
