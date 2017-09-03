// DlgStreamSrcCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgStreamSrcCfg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStreamSrcCfg dialog


CDlgStreamSrcCfg::CDlgStreamSrcCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStreamSrcCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStreamSrcCfg)
	m_bEnableChannel = FALSE;
	m_bEnableURL = FALSE;
	m_strChannel = _T("");
	m_strDomainName = _T("");
	m_strIPAddress = _T("");
	m_strPassword = _T("");
	m_strPort = _T("");
	m_strRelatedChannel = _T("");
	m_strStreamID = _T("");
	m_strStreamURL = _T("");
	m_strUserName = _T("");
	m_strStatus = _T("");
	//}}AFX_DATA_INIT
}


void CDlgStreamSrcCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStreamSrcCfg)
	DDX_Control(pDX, IDC_LIST_STREAM_SRC, m_lstStreamSrc);
	DDX_Control(pDX, IDC_COMBO_SRC_TYPE, m_cmbSrcType);
	DDX_Control(pDX, IDC_COMBO_STREAM_TYPE, m_cmbStreamtype);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL_TYPE, m_cmbProtocolType);
	DDX_Check(pDX, IDC_CHECK_ENABLE_CHANNLE, m_bEnableChannel);
	DDX_Check(pDX, IDC_CHECK_ENABLE_URL, m_bEnableURL);
	DDX_Text(pDX, IDC_EDIT_CHANNLE, m_strChannel);
	DDX_Text(pDX, IDC_EDIT_DOMAIN_NAME, m_strDomainName);
	DDX_Text(pDX, IDC_EDIT_IP_ADDRESS, m_strIPAddress);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDIT_PORT, m_strPort);
	DDX_Text(pDX, IDC_EDIT_RELATED_CHANNEL_, m_strRelatedChannel);
	DDX_Text(pDX, IDC_EDIT_STREAM_ID, m_strStreamID);
	DDX_Text(pDX, IDC_EDIT_STREAM_URL, m_strStreamURL);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_strStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStreamSrcCfg, CDialog)
	//{{AFX_MSG_MAP(CDlgStreamSrcCfg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAddStreamID)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDelStreamID)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STREAM_SRC, OnClickListStreamSrc)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGetStreamSrc)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSetStreamSrc)
	ON_CBN_SELCHANGE(IDC_COMBO_SRC_TYPE, OnSelchangeComboSrcType)
	ON_BN_CLICKED(IDC_BUTTON1, OnRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStreamSrcCfg message handlers

BOOL CDlgStreamSrcCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rc(0, 0, 0, 0);
	GetParent()->GetClientRect(&rc);
	((CTabCtrl*)GetParent())->AdjustRect(FALSE, &rc);
	MoveWindow(&rc);
//	OnRefresh();

	m_lstStreamSrc.SetExtendedStyle(m_lstStreamSrc.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	char szLan[128] = {0};
	g_StringLanType(szLan, "序号", "Serial No.");
	m_lstStreamSrc.InsertColumn(0, szLan, LVCFMT_LEFT, 50, -1);

	g_StringLanType(szLan, "流ID", "Stream ID");
	m_lstStreamSrc.InsertColumn(1, szLan, LVCFMT_LEFT, 100, -1);
	
	g_StringLanType(szLan, "关联通道", "Related channel");
	m_lstStreamSrc.InsertColumn(2, szLan, LVCFMT_LEFT, 60, -1);

	g_StringLanType(szLan, "来源方式", "Source type");
	m_lstStreamSrc.InsertColumn(3, szLan, LVCFMT_LEFT, 60, -1);
	
	g_StringLanType(szLan, "是否启用URL", "Enable URL");
	m_lstStreamSrc.InsertColumn(4, szLan, LVCFMT_LEFT, 100, -1);
	
	g_StringLanType(szLan, "URL", "URL");
	m_lstStreamSrc.InsertColumn(5, szLan, LVCFMT_LEFT, 150, -1);

	g_StringLanType(szLan, "是否启用直连通道", "Enable direct connect channel");
	m_lstStreamSrc.InsertColumn(6, szLan, LVCFMT_LEFT, 120, -1);
	
	g_StringLanType(szLan, "协议类型", "Protocol type");
	m_lstStreamSrc.InsertColumn(7, szLan, LVCFMT_LEFT, 60, -1);
	
	g_StringLanType(szLan, "用户名", "User name");
	m_lstStreamSrc.InsertColumn(8, szLan, LVCFMT_LEFT, 50, -1);
	
	g_StringLanType(szLan, "密码", "Password");
	m_lstStreamSrc.InsertColumn(9, szLan, LVCFMT_LEFT, 50, -1);
	
	g_StringLanType(szLan, "域名", "Domain");
	m_lstStreamSrc.InsertColumn(10, szLan, LVCFMT_LEFT, 50, -1);
	
	g_StringLanType(szLan, "IP地址", "IP address");
	m_lstStreamSrc.InsertColumn(11, szLan, LVCFMT_LEFT, 80, -1);

	g_StringLanType(szLan, "端口", "Port");
	m_lstStreamSrc.InsertColumn(12, szLan, LVCFMT_LEFT, 50, -1);
	
	g_StringLanType(szLan, "传输类型", "Stream type");
	m_lstStreamSrc.InsertColumn(13, szLan, LVCFMT_LEFT, 60, -1);
	
	g_StringLanType(szLan, "通道号", "Channel");
	m_lstStreamSrc.InsertColumn(14, szLan, LVCFMT_LEFT, 50, -1);

	g_StringLanType(szLan, "状态", "Status");
	m_lstStreamSrc.InsertColumn(15, szLan, LVCFMT_LEFT, 50, -1);


	m_cmbSrcType.SetCurSel(0);
	m_cmbSrcType.SetItemData(0, 0);
	m_cmbSrcType.SetItemData(1, 4);
	m_cmbStreamtype.SetCurSel(0);
	m_cmbProtocolType.SetCurSel(0);

	OnSelchangeComboSrcType();

	m_bEnableURL = FALSE;
	m_bEnableChannel = FALSE;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStreamSrcCfg::OnButtonAddStreamID() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szLan[128] = {0};
// 	if ("" == m_strStreamID)
// 	{
// 		g_StringLanType(szLan,"流ID为空","Stream ID is NULL");
// 		AfxMessageBox(szLan);
// 		return;
// 	}

	CString strTemp;	
	int iItemCount = m_lstStreamSrc.GetItemCount();
	if (iItemCount > 64)
	{
		g_StringLanType(szLan,"流ID超过64个","Stream ID is more than 64");
		AfxMessageBox(szLan);
		return;
	}
	strTemp.Format("%02d", iItemCount+1);
	m_lstStreamSrc.InsertItem(iItemCount, strTemp);
	m_lstStreamSrc.SetItemText(iItemCount, 1, m_strStreamID);
	m_lstStreamSrc.SetItemText(iItemCount, 2, m_strRelatedChannel);
	m_cmbSrcType.GetWindowText(strTemp);
	m_lstStreamSrc.SetItemText(iItemCount, 3, strTemp);
	if (0 == m_cmbSrcType.GetCurSel())
	{
		// 将url部分设为空
		m_lstStreamSrc.SetItemText(iItemCount, 4, "");
		m_lstStreamSrc.SetItemText(iItemCount, 5, "");

		if (TRUE == m_bEnableChannel)
		{
			m_lstStreamSrc.SetItemText(iItemCount, 6, "是");
		}
		else
		{
			m_lstStreamSrc.SetItemText(iItemCount, 6, "否");
		}

		m_cmbProtocolType.GetWindowText(strTemp);
		m_lstStreamSrc.SetItemText(iItemCount, 7, strTemp);
		m_lstStreamSrc.SetItemText(iItemCount, 8, m_strUserName);
		m_lstStreamSrc.SetItemText(iItemCount, 9, m_strPassword);
		m_lstStreamSrc.SetItemText(iItemCount, 10, m_strDomainName);
		m_lstStreamSrc.SetItemText(iItemCount, 11, m_strIPAddress);
		m_lstStreamSrc.SetItemText(iItemCount, 12, m_strPort);
		m_cmbStreamtype.GetWindowText(strTemp);
		m_lstStreamSrc.SetItemText(iItemCount, 13, strTemp);
		m_lstStreamSrc.SetItemText(iItemCount, 14, m_strChannel);
	}
	else
	{
		if (TRUE == m_bEnableURL)
		{
			m_lstStreamSrc.SetItemText(iItemCount, 4, "是");
		}
		else
		{
			m_lstStreamSrc.SetItemText(iItemCount, 4, "否");
		}

		m_lstStreamSrc.SetItemText(iItemCount, 5, m_strStreamURL);

		// 将直连部分全设为空
		for (int i=6; i<15; i++)
		{
			m_lstStreamSrc.SetItemText(iItemCount, i, "");
		}

	}
}

void CDlgStreamSrcCfg::OnButtonDelStreamID() 
{
	// TODO: Add your control notification handler code here
	while(m_lstStreamSrc.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int nItem = m_lstStreamSrc.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
		m_lstStreamSrc.DeleteItem(nItem);
	}
	
}

void CDlgStreamSrcCfg::OnClickListStreamSrc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nItem = 0;
	CString strChannel;
	
	POSITION pos = m_lstStreamSrc.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		nItem = m_lstStreamSrc.GetNextSelectedItem(pos);
		
		//显示信息
		m_strStreamID = m_lstStreamSrc.GetItemText(nItem, 1);
		m_strRelatedChannel = m_lstStreamSrc.GetItemText(nItem, 2);
		m_cmbSrcType.SetWindowText(m_lstStreamSrc.GetItemText(nItem, 3));
		if (0 == strcmp("是", m_lstStreamSrc.GetItemText(nItem, 4)))
		{
			m_bEnableURL = TRUE;
		}
		else
		{
			m_bEnableURL = FALSE;
		}

		m_strStreamURL = m_lstStreamSrc.GetItemText(nItem, 5);

		if (0 == strcmp("是", m_lstStreamSrc.GetItemText(nItem, 6)))
		{
			m_bEnableChannel = TRUE;
		}
		else
		{
			m_bEnableChannel = FALSE;
		}

		m_cmbProtocolType.SetWindowText(m_lstStreamSrc.GetItemText(nItem, 7));
		m_strUserName = m_lstStreamSrc.GetItemText(nItem, 8);
		m_strPassword = m_lstStreamSrc.GetItemText(nItem, 9);
		m_strDomainName = m_lstStreamSrc.GetItemText(nItem, 10);
		m_strIPAddress = m_lstStreamSrc.GetItemText(nItem, 11);
		m_strPort = m_lstStreamSrc.GetItemText(nItem, 12);
		m_cmbStreamtype.SetWindowText(m_lstStreamSrc.GetItemText(nItem, 13));
		m_strChannel = m_lstStreamSrc.GetItemText(nItem, 14);
		m_strStatus = m_lstStreamSrc.GetItemText(nItem, 15);
		
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CDlgStreamSrcCfg::OnButtonGetStreamSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    char szLan[128] = {0};
	DWORD dwReturn = 0;
	CString strTemp;

	LPNET_DVR_STREAM_SRC_INFO pStreamSrcInfo = NULL;

	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	LONG lLoginID = g_struDeviceInfo[iDeviceIndex].lLoginID;
	
	int iItemCount = m_lstStreamSrc.GetItemCount();
	
	LPNET_DVR_STREAM_INFO pstruStreamID = NULL;
	LPNET_DVR_STREAM_INFO pTempStreamID = NULL;
	NET_DVR_IPC_PROTO_LIST struIPCProtoList;
	memset(&struIPCProtoList,0,sizeof(NET_DVR_IPC_PROTO_LIST));
	m_cmbProtocolType.ResetContent();
	if(!NET_DVR_GetIPCProtoList(lLoginID,&struIPCProtoList))
	{
		g_StringLanType(szLan,"获取IPC协议失败","Fail to get IPC Proto list");
		g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "Fail to get Proto list");
		AfxMessageBox(szLan);
		return;
	}
	else
	{
		for (int i = 0; i < struIPCProtoList.dwProtoNum; i++)
		{
			//	char *strProto = ;
			sprintf(szLan, "%s", struIPCProtoList.struProto[i].byDescribe);
			m_cmbProtocolType.AddString(szLan);
			m_cmbProtocolType.SetItemData(i,struIPCProtoList.struProto[i].dwType);
		}
		m_cmbProtocolType.SetCurSel(0);
	}
	if (0 == iItemCount)
	{
// 		if ("" == m_strStreamID)
// 		{
// 			g_StringLanType(szLan,"流ID为空","Stream ID is NULL");
// 			AfxMessageBox(szLan);
// 			return;
// 		}
		pstruStreamID = new(std::nothrow) NET_DVR_STREAM_INFO;
		if (NULL == pstruStreamID)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pstruStreamID, 0, sizeof(NET_DVR_STREAM_INFO));
		pstruStreamID->dwSize = sizeof(NET_DVR_STREAM_INFO);
		pstruStreamID->dwChannel = atoi(m_strRelatedChannel);
        pstruStreamID->dwChannel = 0xffffffff;
		strncpy((char*)pstruStreamID->byID, m_strStreamID, STREAM_ID_LEN);

		pStreamSrcInfo = new(std::nothrow) NET_DVR_STREAM_SRC_INFO;
		if (NULL == pStreamSrcInfo)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pStreamSrcInfo, 0, sizeof(NET_DVR_STREAM_SRC_INFO));

		DWORD dwStatus = 0;

		BOOL bRet = NET_DVR_GetDeviceConfig(lLoginID, NET_DVR_GET_STREAM_SRC_INFO, iItemCount, pstruStreamID, sizeof(NET_DVR_STREAM_INFO), &dwStatus, pStreamSrcInfo, sizeof(NET_DVR_STREAM_SRC_INFO));
		
		if ( bRet && (0 == dwStatus) )
		{
			g_StringLanType(szLan,"获取流来源成功","Get source success");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_SUCC_T, "Get Stream Source success");
			AfxMessageBox(szLan);
		}
		else
		{        
			g_StringLanType(szLan,"获取流来源失败","Fail to Get stream source");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "Fail to set stream source");
			AfxMessageBox(szLan);
			return;
		}


		if (0 == pStreamSrcInfo->struStreamSrcInfo.byGetStreamType)
		{
			char szDomianName[MAX_DOMAIN_NAME] = {0};
			char szUserName[NAME_LEN] = {0};
			char szPassword[PASSWD_LEN] = {0};
			char szIP[16] = {0};

			m_cmbSrcType.SetCurSel(0);
			m_bEnableChannel = pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byEnable;
			m_cmbProtocolType.SetCurSel(0);  
			m_cmbStreamtype.SetCurSel(pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byStreamType);
			m_strChannel.Format("%d", pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.dwChannel);
			m_strPort.Format("%d", pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.wDVRPort);
			strncpy(szDomianName, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byDomain, MAX_DOMAIN_NAME-1);
			m_strDomainName.Format("%s", szDomianName);
			strncpy(szUserName, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sUserName, NAME_LEN);
			m_strUserName.Format("%s", szUserName);
			strncpy(szPassword, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sPassword, PASSWD_LEN);
			m_strPassword.Format("%s", szPassword);
			strncpy(szIP, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.struIP.sIpV4, 16);
			m_strIPAddress.Format("%s", szIP);

		}
		else if (4 == pStreamSrcInfo->struStreamSrcInfo.byGetStreamType)
		{
			char szURL[256] = {0};
			m_cmbSrcType.SetCurSel(1);
			m_bEnableURL = pStreamSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.byEnable;
			strncpy(szURL, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.strURL, 240);
			m_strStreamURL.Format("%s", szURL);
		}
		else
		{
			g_StringLanType(szLan,"源模式暂不支持","Not support this stream source type");
			AfxMessageBox(szLan);
			return;
		}

		m_strStatus.Format("%d", dwStatus);
	}
	else
	{
		// 申请iItemCount个NET_DVR_STREAM_INFO
		pstruStreamID = new(std::nothrow) NET_DVR_STREAM_INFO[iItemCount];
		if (NULL == pstruStreamID)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pstruStreamID, 0, iItemCount*sizeof(NET_DVR_STREAM_INFO));
		pTempStreamID = pstruStreamID;

		// 申请iItemCount个NET_DVR_STREAM_SRC_INFO
		pStreamSrcInfo = new(std::nothrow) NET_DVR_STREAM_SRC_INFO[iItemCount];
		if (NULL == pStreamSrcInfo)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pStreamSrcInfo, 0, iItemCount*sizeof(NET_DVR_STREAM_SRC_INFO));

		int i = 0; 
		for (i=0; i<iItemCount; i++)
		{
		 	pTempStreamID->dwSize = sizeof(NET_DVR_STREAM_INFO);
			strncpy((char*)pTempStreamID->byID, m_lstStreamSrc.GetItemText(i, 1), STREAM_ID_LEN);
		 	pTempStreamID->dwChannel = atoi(m_lstStreamSrc.GetItemText(i, 2));
			pTempStreamID++;
		}

		DWORD *pStatus = new DWORD[iItemCount];
		memset(pStatus, 0, sizeof(DWORD)*iItemCount);

		if (NET_DVR_GetDeviceConfig(lLoginID, NET_DVR_GET_STREAM_SRC_INFO, iItemCount, pstruStreamID, iItemCount*sizeof(NET_DVR_STREAM_INFO), pStatus, pStreamSrcInfo, iItemCount*sizeof(NET_DVR_STREAM_SRC_INFO)))
		{
			g_StringLanType(szLan,"获取流来源成功","Get stream source success");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_SUCC_T, "Get Stream Source");
			AfxMessageBox(szLan);
		}
		else
		{        
			g_StringLanType(szLan,"获取流来源失败","Fail to get stream source");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "Fail to get stream source");
			AfxMessageBox(szLan);
			return;
		}

		for (i=0; i<iItemCount; i++)
		{
			if (0 == pStreamSrcInfo->struStreamSrcInfo.byGetStreamType)
			{
				m_lstStreamSrc.SetItemText(i, 3, "直接取流");
				m_lstStreamSrc.SetItemText(i, 4, "");
				m_lstStreamSrc.SetItemText(i, 5, "");
				if (1 == pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byEnable)
				{
					m_lstStreamSrc.SetItemText(i, 6, "是");
				}
				else
				{
					m_lstStreamSrc.SetItemText(i, 6, "否");
				}

				m_lstStreamSrc.SetItemText(i, 7, "私有协议"); 
				m_lstStreamSrc.SetItemText(i, 8, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sUserName);
				m_lstStreamSrc.SetItemText(i, 9, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sPassword);
				m_lstStreamSrc.SetItemText(i, 10, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byDomain);
				m_lstStreamSrc.SetItemText(i, 11, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.struIP.sIpV4);
				strTemp.Format("%d", pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.wDVRPort);
				m_lstStreamSrc.SetItemText(i, 12, strTemp);
				
				if (0 == pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byStreamType)
				{
					m_lstStreamSrc.SetItemText(i, 13, "主码流");
				}
				else
				{
					m_lstStreamSrc.SetItemText(i, 13, "子码流");
				}

				strTemp.Format("%d", pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.dwChannel);
				m_lstStreamSrc.SetItemText(i, 14, strTemp);
			}
			else if (4 == pStreamSrcInfo->struStreamSrcInfo.byGetStreamType)
			{
				m_lstStreamSrc.SetItemText(i, 3, "URL");
				if (1 == pStreamSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.byEnable)
				{
					m_lstStreamSrc.SetItemText(i, 4, "是");
				}
				else
				{
					m_lstStreamSrc.SetItemText(i, 4, "否");
				}
				m_lstStreamSrc.SetItemText(i, 5, (const char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.strURL);
				for (int j=6; j<15; i++)
				{
					m_lstStreamSrc.SetItemText(i, j, "");
				}
			}
			else
			{
				g_pMainDlg->AddLog(iDeviceIndex, ALARM_INFO_T, "not support this stream source type");
			}
			
			CString strStatus;
			strStatus.Format("%d", *pStatus);
			m_lstStreamSrc.SetItemText(i, 15, strStatus);
			
			pStatus++;
			pStreamSrcInfo++;
		}
	}
	UpdateData(FALSE);
	
}

void CDlgStreamSrcCfg::OnButtonSetStreamSrc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    char szLan[128] = {0};

	LPNET_DVR_STREAM_SRC_INFO pStreamSrcInfo = NULL;
	LPNET_DVR_STREAM_SRC_INFO pTempSrcInfo = NULL;
	
	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	LONG lLoginID = g_struDeviceInfo[iDeviceIndex].lLoginID;
	
	int iItemCount = m_lstStreamSrc.GetItemCount();
	
	LPNET_DVR_STREAM_INFO pstruStreamID = NULL;
	LPNET_DVR_STREAM_INFO pTempStreamID = NULL;
	if (0 == iItemCount)
	{
// 		if ("" == m_strStreamID)
// 		{
// 			g_StringLanType(szLan,"流ID为空","Stream ID is NULL");
// 			AfxMessageBox(szLan);
// 			return;
// 		}
		pstruStreamID = new(std::nothrow) NET_DVR_STREAM_INFO;
		if (NULL == pstruStreamID)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pstruStreamID, 0, sizeof(NET_DVR_STREAM_INFO));
		pstruStreamID->dwSize = sizeof(NET_DVR_STREAM_INFO);
		pstruStreamID->dwChannel = atoi(m_strRelatedChannel);
		strncpy((char*)pstruStreamID->byID, m_strStreamID, STREAM_ID_LEN);

		pStreamSrcInfo = new(std::nothrow) NET_DVR_STREAM_SRC_INFO;
		if (NULL == pStreamSrcInfo)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pStreamSrcInfo, 0, sizeof(NET_DVR_STREAM_SRC_INFO));
		pStreamSrcInfo->dwSize = sizeof(NET_DVR_STREAM_SRC_INFO);
		pStreamSrcInfo->struStreamSrcInfo.byGetStreamType = m_cmbSrcType.GetItemData(m_cmbSrcType.GetCurSel());
		if (0 == pStreamSrcInfo->struStreamSrcInfo.byGetStreamType)
		{
			pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byEnable = m_bEnableChannel;
			pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byProType = m_cmbProtocolType.GetItemData(m_cmbProtocolType.GetCurSel());
			pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byStreamType = m_cmbStreamtype.GetCurSel();
			pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.dwChannel = atoi(m_strChannel);
			pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.wDVRPort = atoi(m_strPort);
			strncpy((char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byDomain, m_strDomainName, MAX_DOMAIN_NAME);
			strncpy((char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sUserName, m_strUserName, NAME_LEN);
			strncpy((char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sPassword, m_strPassword, PASSWD_LEN);
			strncpy(pStreamSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.struIP.sIpV4, m_strIPAddress, 16);
			
		}
		else if (4 == pStreamSrcInfo->struStreamSrcInfo.byGetStreamType)
		{
			pStreamSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.byEnable = m_bEnableURL;
			strncpy((char*)pStreamSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.strURL, m_strStreamURL, 240);
		}
		else
		{
			g_StringLanType(szLan,"源模式暂不支持","Not support this stream source type");
			AfxMessageBox(szLan);
			return;
		}

		DWORD dwStatus = 0xffffffff; 

		BOOL bRet = NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_SET_STREAM_SRC_INFO, iItemCount, pstruStreamID, sizeof(NET_DVR_STREAM_INFO), &dwStatus, pStreamSrcInfo, sizeof(NET_DVR_STREAM_SRC_INFO));
		
	
		if (bRet && (0 == dwStatus))
		{
			g_StringLanType(szLan,"设置流来源成功","Success");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_SUCC_T, "Set Stream Source");
			AfxMessageBox(szLan);
		}
		else
		{        
			g_StringLanType(szLan,"设置流来源失败","Fail to set stream source");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "Fail to set stream source");
			AfxMessageBox(szLan);
		}

		m_strStatus.Format("%d", dwStatus);
	}
	else
	{
		// 申请iItemCount个NET_DVR_STREAM_INFO
		pstruStreamID = new(std::nothrow) NET_DVR_STREAM_INFO[iItemCount];
		if (NULL == pstruStreamID)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pstruStreamID, 0, iItemCount*sizeof(NET_DVR_STREAM_INFO));
		pTempStreamID = pstruStreamID;

		// 申请iItemCount个NET_DVR_STREAM_SRC_INFO
		pStreamSrcInfo = new(std::nothrow) NET_DVR_STREAM_SRC_INFO[iItemCount];
		if (NULL == pStreamSrcInfo)
		{
			g_StringLanType(szLan,"申请内存失败","new memory failed");
			AfxMessageBox(szLan);
			return;
		}
		memset(pStreamSrcInfo, 0, iItemCount*sizeof(NET_DVR_STREAM_SRC_INFO));
		pTempSrcInfo = pStreamSrcInfo;

		int i = 0; 
		for (i=0; i<iItemCount; i++)
		{
		 	pTempStreamID->dwSize = sizeof(NET_DVR_STREAM_INFO);
			strncpy((char*)pTempStreamID->byID, m_lstStreamSrc.GetItemText(i, 1), STREAM_ID_LEN);
		 	pTempStreamID->dwChannel = atoi(m_lstStreamSrc.GetItemText(i, 2));
		 	
			pTempSrcInfo->dwSize = sizeof(NET_DVR_STREAM_SRC_INFO);
			if (0 == strcmp("直接取流", m_lstStreamSrc.GetItemText(i, 3)))
			{
				pTempSrcInfo->struStreamSrcInfo.byGetStreamType = 0;
				if (0 == strcmp("是", m_lstStreamSrc.GetItemText(i, 6)))
				{
					pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byEnable = 1;
				}
				else
				{
					pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byEnable = 0;
				}

				pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byProType = 0; 
				strncpy((char*)pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sUserName, m_lstStreamSrc.GetItemText(i, 8), NAME_LEN);
				strncpy((char*)pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.sPassword, m_lstStreamSrc.GetItemText(i, 9), PASSWD_LEN);
				strncpy((char*)pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byDomain, m_lstStreamSrc.GetItemText(i, 10), MAX_DOMAIN_NAME);
				strncpy((char*)pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.struIP.sIpV4, m_lstStreamSrc.GetItemText(i, 11), 16);
				pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.wDVRPort = atoi(m_lstStreamSrc.GetItemText(i, 12));
				if (0 == strcmp("主码流", m_lstStreamSrc.GetItemText(i, 13)))
				{
					pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byStreamType = 0;
				}
				else
				{
					pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.byStreamType = 1;
				}
				pTempSrcInfo->struStreamSrcInfo.uGetStream.struChanInfo.dwChannel = atoi(m_lstStreamSrc.GetItemText(i, 14));

			}
			else if (0 == strcmp("URL", m_lstStreamSrc.GetItemText(i, 3)))
			{
				pTempSrcInfo->struStreamSrcInfo.byGetStreamType = 4;
				if (0 == strcmp("是", m_lstStreamSrc.GetItemText(i, 4)))
				{
					pTempSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.byEnable = 1;
				}
				else
				{
					pTempSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.byEnable = 0;
				}
				strncpy((char*)pTempSrcInfo->struStreamSrcInfo.uGetStream.struStreamUrl.strURL, m_lstStreamSrc.GetItemText(i, 5), 240);
			}
			else
			{
				g_pMainDlg->AddLog(iDeviceIndex, ALARM_INFO_T, "not support this stream source type");
			}
			
			pTempStreamID++;
			pTempSrcInfo++;
		}

		DWORD *pStatus = new DWORD[iItemCount];
		memset(pStatus, 0, sizeof(DWORD)*iItemCount);


		if (NET_DVR_SetDeviceConfig(lLoginID, NET_DVR_SET_STREAM_SRC_INFO, iItemCount, pstruStreamID, iItemCount*sizeof(NET_DVR_STREAM_INFO), pStatus, pStreamSrcInfo, iItemCount*sizeof(NET_DVR_STREAM_SRC_INFO)))
		{
			g_StringLanType(szLan,"设置流来源成功","Success");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_SUCC_T, "Set Stream Source");
			AfxMessageBox(szLan);
		}
		else
		{        
			g_StringLanType(szLan,"设置流来源失败","Fail to set stream source");
			g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "Fail to set stream source");
			AfxMessageBox(szLan);
		}

		for (i=0; i<iItemCount; i++)
		{
			CString strStatus;
			strStatus.Format("%d", *pStatus);
			m_lstStreamSrc.SetItemText(i, 15, strStatus);
			pStatus++;
		}
	}

	UpdateData(FALSE);
}

void CDlgStreamSrcCfg::OnSelchangeComboSrcType() 
{
	// TODO: Add your control notification handler code here
	if (0 == m_cmbSrcType.GetCurSel())
	{
		GetDlgItem(IDC_EDIT_STREAM_URL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ENABLE_URL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STREAM_URL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_COMBO_PROTOCOL_TYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PROTOCOL_TYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_USER_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_USER_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PASSWORD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_CHANNLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CHANNLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_STREAM_TYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_STREAM_TYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_DOMAIN_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_DOMAIN_NAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_IP_ADDRESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_IP_ADDRESS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_ENABLE_CHANNLE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDIT_STREAM_URL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_ENABLE_URL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_STREAM_URL)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_COMBO_PROTOCOL_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PROTOCOL_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_USER_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_USER_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PASSWORD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CHANNLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CHANNLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_STREAM_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_STREAM_TYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_DOMAIN_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_DOMAIN_NAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_IP_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_IP_ADDRESS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ENABLE_CHANNLE)->ShowWindow(SW_HIDE);
	}
}

void CDlgStreamSrcCfg::OnRefresh() 
{
	// TODO: Add your control notification handler code here
	int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
	LONG lLoginID = g_struDeviceInfo[iDeviceIndex].lLoginID;
	char szLan[128] = {0};
	NET_DVR_IPC_PROTO_LIST struIPCProtoList;
	m_cmbProtocolType.ResetContent();
	memset(&struIPCProtoList,0,sizeof(NET_DVR_IPC_PROTO_LIST));
	if(!NET_DVR_GetIPCProtoList(lLoginID,&struIPCProtoList))
	{
		g_StringLanType(szLan,"获取IPC协议失败","Fail to get IPC Proto list");
		g_pMainDlg->AddLog(iDeviceIndex, OPERATION_FAIL_T, "Fail to get Proto list");
		AfxMessageBox(szLan);
		return;
	}
	else
	{
		for (int i = 0; i < struIPCProtoList.dwProtoNum; i++)
		{
			//	char *strProto = ;
			sprintf(szLan, "%s", struIPCProtoList.struProto[i].byDescribe);
			m_cmbProtocolType.AddString(szLan);
			m_cmbProtocolType.SetItemData(i,struIPCProtoList.struProto[i].dwType);
		}
		m_cmbProtocolType.SetCurSel(0);
	}
	UpdateData(FALSE);
}
