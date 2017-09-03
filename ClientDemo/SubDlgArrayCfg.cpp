// SubDlgArrayCfg.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "SubDlgArrayCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubDlgArrayCfg dialog
#include "DlgCreateVD.h"

CSubDlgArrayCfg::CSubDlgArrayCfg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubDlgArrayCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubDlgArrayCfg)
	m_csArrayName = _T("");
	//}}AFX_DATA_INIT
    m_iDevIndex = -1;
    m_lServerID = -1;
    m_iSelectIndex = -1;
    memset(&m_struArrayList, 0, sizeof(m_struArrayList));
}


void CSubDlgArrayCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubDlgArrayCfg)
	DDX_Control(pDX, IDC_LIST_PD_SLOT, m_listPDSlot);
	DDX_Control(pDX, IDC_COMBO_RAID_MODE, m_comboRaidMode);
	DDX_Control(pDX, IDC_LIST_ARRAY_INFO, m_ListArrrayInfo);
	DDX_Text(pDX, IDC_EDIT_ARRAY_NAME, m_csArrayName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubDlgArrayCfg, CDialog)
	//{{AFX_MSG_MAP(CSubDlgArrayCfg)
	ON_BN_CLICKED(IDC_BTN_DEL_ARRAY, OnBtnDelArray)
	ON_BN_CLICKED(IDC_BTN_CREATE_VD, OnBtnCreateVd)
	ON_BN_CLICKED(IDC_BTN_REBUILD_ARRAY, OnBtnRebuildArray)
	ON_BN_CLICKED(IDC_BTN_MIGRATE_ARRAY, OnBtnMigrateArray)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ARRAY_INFO, OnItemchangedListArrayInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubDlgArrayCfg message handlers
BOOL CSubDlgArrayCfg::CheckInitParam()
{
    m_iDevIndex = g_pMainDlg->GetCurDeviceIndex();
    m_lServerID = g_struDeviceInfo[m_iDevIndex].lLoginID;
    
    if (!GetAdapterInfo())
    {
//         AfxMessageBox("Fail to get adapter info");
//         return FALSE;
    }

    m_comboRaidMode.ResetContent();
    int nIndex = 0;
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 0) & 0x01))
    {
        m_comboRaidMode.AddString("RAID0");
        m_comboRaidMode.SetItemData(nIndex, RAID0);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 1) & 0x01))
    {
        m_comboRaidMode.AddString("RAID1");
        m_comboRaidMode.SetItemData(nIndex, RAID1);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 2) & 0x01))
    {
        m_comboRaidMode.AddString("RAID10");
        m_comboRaidMode.SetItemData(nIndex, RAID10);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 3) & 0x01))
    {
        m_comboRaidMode.AddString("RAID1E");
        m_comboRaidMode.SetItemData(nIndex, RAID1E);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 4) & 0x01))
    {
        m_comboRaidMode.AddString("RAID5");
        m_comboRaidMode.SetItemData(nIndex, RAID5);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 5) & 0x01))
    {
        m_comboRaidMode.AddString("RAID6");
        m_comboRaidMode.SetItemData(nIndex, RAID6);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 6) & 0x01))
    {
        m_comboRaidMode.AddString("RAID50");
        m_comboRaidMode.SetItemData(nIndex, RAID50);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 7) & 0x01))
    {
        m_comboRaidMode.AddString("JBOD");
        m_comboRaidMode.SetItemData(nIndex, JBOD);
        nIndex++;
    }
    if (1 == ((m_struAdapterInfo.wSupportRaidType >> 8) & 0x01))
    {
        m_comboRaidMode.AddString("RAID60");
        m_comboRaidMode.SetItemData(nIndex, RAID60);
        nIndex++;
    }

    if (!GetPDList())
    {
        //AfxMessageBox("Fail to get PD");
    }
	else
	{
		char szLan[128] = {0};
		m_listPDSlot.DeleteAllItems();            
		m_listPDSlot.SetExtendedStyle(LVS_EX_CHECKBOXES);
		for (int i = 0; i < m_struPDList.dwCount; i++)
		{
			sprintf(szLan, "%d", m_struPDList.struPhyDiskInfo[i].wPhySlot);
			m_listPDSlot.InsertItem(i, szLan);
			m_listPDSlot.SetItemData(i, m_struPDList.struPhyDiskInfo[i].wPhySlot);
		}
	}


    return TRUE;
}
void CSubDlgArrayCfg::CurCfgUpdate()
{
    if (!CheckInitParam())
    {
        return;
    }
    
    if (!GetArrayList())
    {
        return;
    }

    m_ListArrrayInfo.DeleteAllItems();
    int nIndex = 0;
    char szLan[128] = {0};
    char szLanEn[128] = {0};
    char szLanCn[128] = {0};
    CString csTmp = "";
    for (int i = 0; i < m_struArrayList.dwCount; i++)
    {
        nIndex = 0;
        sprintf(szLan, "%d", i+1);   
        m_ListArrrayInfo.InsertItem(i, szLan,nIndex);
        nIndex++;

        sprintf(szLan, "%d",m_struArrayList.struArrayInfo[i].wArrayID);   
        m_ListArrrayInfo.SetItemText(i, nIndex, szLan);
        nIndex++;

        m_ListArrrayInfo.SetItemText(i, nIndex, (char*)m_struArrayList.struArrayInfo[i].byArrayName);
        nIndex++;
        
        unsigned __int64 dwCapacity = 0;
        unsigned __int64 dwFreeSpace = 0;
        *((DWORD*)&dwCapacity) = m_struArrayList.struArrayInfo[i].dwLCapacity;
        *((DWORD*)((char*)&dwCapacity + 4))  = m_struArrayList.struArrayInfo[i].dwHCapacity;
        *((DWORD*)&dwFreeSpace) = m_struArrayList.struArrayInfo[i].dwLFreeSpace;
        *((DWORD*)((char*)&dwFreeSpace+ 4))  = m_struArrayList.struArrayInfo[i].dwHFreeSpace;
        dwCapacity >>= 20;
        dwFreeSpace >>= 20;
        sprintf(szLan, "%I64u/%I64u Gb", dwFreeSpace, dwCapacity);
        m_ListArrrayInfo.SetItemText(i, nIndex, szLan);
        nIndex++;

        csTmp = "";
		int j = 0;
        for (j = 0; j < m_struArrayList.struArrayInfo[i].byPDCount; j++)
        {
            sprintf(szLan, "%d ", m_struArrayList.struArrayInfo[i].wPDSlots[j]);
            csTmp += szLan;
        }
        m_ListArrrayInfo.SetItemText(i, nIndex, csTmp.GetBuffer(0));
        nIndex++; //  物理磁盘

        csTmp = "";
        for (j = 0; j < m_struArrayList.struArrayInfo[i].bySpareCount; j++)
        {
            sprintf(szLan, "%d ", m_struArrayList.struArrayInfo[i].wSparePDSlots[j]);
            csTmp += szLan;
        }
        m_ListArrrayInfo.SetItemText(i, nIndex, csTmp.GetBuffer(0));
        nIndex++;    //  热备

        if (0 == m_struArrayList.struArrayInfo[i].byStatus)
        {
           g_StringLanType(szLan, "在线", "FUNCTIONAL");
       }
       else if (1 == m_struArrayList.struArrayInfo[i].byStatus)
       {
           g_StringLanType(szLan, "磁盘丢失", "Missing");
       }
       else if (2 == m_struArrayList.struArrayInfo[i].byStatus)
       {
           g_StringLanType(szLan, "不在线", "offline");
       }
	   else if(3 == m_struArrayList.struArrayInfo[i].byStatus)
       {
           g_StringLanType(szLan, "降级", "degrade");
       }
	   else if(4 == m_struArrayList.struArrayInfo[i].byStatus)
       {
           g_StringLanType(szLan, "异常", "abnormal");
       }
	   else if(5 == m_struArrayList.struArrayInfo[i].byStatus)
       {
           g_StringLanType(szLan, "次正常", "partially optimal");
       }
	   else if(6 == m_struArrayList.struArrayInfo[i].byStatus)
       {
           g_StringLanType(szLan, "外来盘", "foreign");
       }
	   else if(0xff == m_struArrayList.struArrayInfo[i].byStatus)
       {
           g_StringLanType(szLan, "不存在", "not exist");
       }
	   
       m_ListArrrayInfo.SetItemText(i, nIndex, szLan);
       nIndex++;


        switch (m_struArrayList.struArrayInfo[i].byRaidMode)
        {
        case RAID0:
            strcpy(szLan, "RAID0");
            break;
        case RAID1:
            strcpy(szLan, "RAID1");
            break;
        case RAID10:
            strcpy(szLan, "RAID10");
            break;
        case RAID1E:
            strcpy(szLan, "RAID1E");
            break;
        case RAID5:
            strcpy(szLan, "RAID5");
            break;
        case RAID6:
            strcpy(szLan, "RAID6");
            break;
        case RAID50:
            strcpy(szLan, "RAID50");
            break;
        case JBOD:
            strcpy(szLan, "JBOD");
            break;
        case RAID60:
            strcpy(szLan, "RAID60");
            break;
        default:
            break;
        }
        m_ListArrrayInfo.SetItemText(i, nIndex, szLan);
        nIndex++;

        ProcessBgaInfo(szLan, m_struArrayList.struArrayInfo[i].struBgaInfo);
        m_ListArrrayInfo.SetItemText(i, nIndex, szLan);
        nIndex++;
    }

    }

void CSubDlgArrayCfg::ProcessBgaInfo(char *str, const NET_DVR_BGA_INFO &struBgaInfo)
{
    char szLan[128] = {0};
    char szLanCn[128] = {0};
    char szLanEn[128] = {0};
    if (struBgaInfo.byBgaState  == BGA_STATE_NONE)
    {
        strcpy(str, "No task");
    }
    else if (struBgaInfo.byBgaState == BGA_STATE_RUNNING)
    { 
        switch (struBgaInfo.byBga)
        {
        case BGA_REBUILD:
            sprintf(szLanCn, "重建:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLanEn, "Rebuild:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLan, szLanCn, szLanEn);
            break;
        case BGA_CONSISTENCY_FIX:
            sprintf(szLanCn, "一致性检查并修复:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLanEn, "consistency check and fix:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLan, szLanCn, szLanEn);
            break;
        case BGA_CONSISTENCY_CHECK:
            sprintf(szLanCn, "一致性检查:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLanEn, "consistency check:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLan, szLanCn, szLanEn);
            break;
        case BGA_INIT_QUICK:
            sprintf(szLanCn, "快速初始化:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLanEn, "Quick Init:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLan, szLanCn, szLanEn);
            break;
        case BGA_INIT_BACK:
            sprintf(szLanCn, "初始化:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLanEn, "Init:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLan, szLanCn, szLanEn);
            break;
        case BGA_MIGRATION:
            sprintf(szLanCn, "迁移:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLanEn, "Migration:%d%", struBgaInfo.wBgaPercentage);
            sprintf(szLan, szLanCn, szLanEn);
            break;
        case BGA_INIT_FORE:
            sprintf(szLan, "Init Fore:%d%", struBgaInfo.wBgaPercentage);
            break;
        case BGA_COPYBACK:
            sprintf(szLan, "CopyBack %d%",struBgaInfo.wBgaPercentage);
            break;
        default:
            sprintf(szLan, "bga[%d]bgaState[%d]wPercentage[%d]", struBgaInfo.byBga, struBgaInfo.byBgaState, struBgaInfo.wBgaPercentage);
            break;
        }
        strncpy(str, szLan, sizeof(szLan));
    }
    else if (struBgaInfo.byBgaState == BGA_STATE_ABORTED)
    {
        strcpy(str, "Task Aboted");
    }
    else if (struBgaInfo.byBgaState  == BGA_STATE_PAUSED)
    {
        strcpy(str, "Task pause");
    }
}

BOOL CSubDlgArrayCfg::GetArrayList()
{
    if (!NET_DVR_GetArrayList(m_lServerID, &m_struArrayList))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetArrayList");
        return FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GetArrayList");
        return TRUE;
    }
}

void CSubDlgArrayCfg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CSubDlgArrayCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_ListArrrayInfo.SetExtendedStyle(m_ListArrrayInfo.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    
    int nIndex = 0;
    char szLan[128] = {0};

    g_StringLanType(szLan, "序号", "Index");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan,LVCFMT_LEFT,60,-1);
    nIndex++;

    g_StringLanType(szLan, "阵列ID", "Array ID");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan,LVCFMT_LEFT,60,-1);
    nIndex++;

    g_StringLanType(szLan, "名称", "Name");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan,LVCFMT_LEFT,60,-1);
    nIndex++;

    g_StringLanType(szLan, "容量/剩余容量", "capacity/free");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan,LVCFMT_LEFT,60,-1);
    nIndex++;

    g_StringLanType(szLan, "物理磁盘", "PD");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan,LVCFMT_LEFT,60,-1);
    nIndex++;

    g_StringLanType(szLan, "热备", "spare disk");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan,LVCFMT_LEFT,60,-1);
    nIndex++;

    g_StringLanType(szLan, "状态", "Status");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan, LVCFMT_LEFT, 60, -1);
    nIndex++;

    g_StringLanType(szLan, "类型", "Type");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan, LVCFMT_LEFT, 60, -1);
    nIndex++;

    g_StringLanType(szLan, "任务", "Task");
    m_ListArrrayInfo.InsertColumn(nIndex, szLan, LVCFMT_LEFT, 60, -1);
    nIndex++;

//     WORD    wArrayID; // 阵列ID
//     BYTE    byRaidMode; // raid模式  参照RAID_MODE
//     BYTE    byStatus;  // 0-正常 1-次正常 2-降级
//     HPR_UINT32 dwHCapacity;	// 阵列容量高32位
//     HPR_UINT32 dwLCapacity;	// 阵列容量低32位
//     HPR_UINT32	dwHFreeSpace;	// 阵列剩余空间高32位
//     HPR_UINT32	dwLFreeSpace;	// 阵列剩余空间低32位
//     BYTE    byArrayName[MAX_NAMELEN]; // 阵列名称
//     BYTE    byPDCount;  // 物理磁盘数目
//     BYTE    bySpareCount;   // 热备数目
//     BYTE    byRes1[2];
//     WORD    wPDSlots[SUPPORT_PD_NUM];	// 物理磁盘索引
//     WORD    wSparePDSlots[SUPPORT_PD_NUM];	// 热备磁盘索引
//     NET_DVR_BGA_INFO	struBgaInfo;	// 后台任务运行状态
//     BYTE    byRes2[80]; // 保留字节

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSubDlgArrayCfg::OnBtnDelArray() 
{
    char szLan[128] = {0};
    if ( -1 == m_iSelectIndex)
    {
        g_StringLanType(szLan, "请选择阵列", "please select Array");
        AfxMessageBox(szLan);
        return;
    }
    
    if (!DelArray(m_struArrayList.struArrayInfo[m_iSelectIndex].wArrayID))
    {
        AfxMessageBox("Fail to  del Array");
        return;
    }
    else
    {
        CurCfgUpdate();
    }
}

BOOL CSubDlgArrayCfg::DelArray(DWORD dwArrayID)
{
    if (!NET_DVR_DelArray(m_lServerID, dwArrayID))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_DelArray ArrayID[%d]", dwArrayID);
        return FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_DelArray ArrayID[%d]", dwArrayID);
        return TRUE;
    }
}

void CSubDlgArrayCfg::OnBtnCreateVd() 
{
    char szLan[128] = {0};
//     if ( -1 == m_iSelectIndex)
//     {
//         g_StringLanType(szLan, "请选择阵列", "please select Array");
//         AfxMessageBox(szLan);
//         return;
//     }
	CDlgCreateVD dlg;
    dlg.m_lServerID = m_lServerID;
    dlg.m_iDevIndex = m_iDevIndex;
	if (m_iSelectIndex > 0 && m_iSelectIndex < 8)
	{
		memcpy(&dlg.m_struArrayInfo, &m_struArrayList.struArrayInfo[m_iSelectIndex], sizeof(NET_DVR_ARRAY_INFO));
	}
    
    dlg.DoModal();
}

BOOL CSubDlgArrayCfg::GetPDList()
{
    if (!NET_DVR_GetPDList(m_lServerID, &m_struPDList))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GetPDList");
        return FALSE;
    }
    else
    {   
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GetPDList");
        return TRUE;
    }
}


BOOL CSubDlgArrayCfg::GetAdapterInfo()
{
    DWORD dwReturn = 0;
    if (!NET_DVR_GetDVRConfig(m_lServerID, NET_DVR_GET_RAID_ADAPTER_INFO, 0, &m_struAdapterInfo, sizeof(m_struAdapterInfo), &dwReturn))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_GET_RAID_ADAPTER_INFO");
        return FALSE;
    }   
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_GET_RAID_ADAPTER_INFO");
        return TRUE;
    }
}

void CSubDlgArrayCfg::OnBtnRebuildArray() 
{
    GetOperateArrayData();
    if (!RebuildArray())
    {
        AfxMessageBox("Fail to rebuild array");
    }
}

void CSubDlgArrayCfg::OnBtnMigrateArray() 
{
    GetOperateArrayData();
	if (!MigrateArray())
	{
        AfxMessageBox("Fail to migrate array");
	}
}

BOOL CSubDlgArrayCfg::GetOperateArrayData()
{
    UpdateData(TRUE);
    memset(&m_struOperateArray, 0, sizeof(m_struOperateArray));
    strncpy((char*)m_struOperateArray.byName, m_csArrayName.GetBuffer(0), sizeof(m_struOperateArray.byName));
    m_struOperateArray.wArrayID =  m_struArrayList.struArrayInfo[m_iSelectIndex].wArrayID;
    
    for (DWORD i = 0; i < m_listPDSlot.GetItemCount(); i++)
    {
        if (m_listPDSlot.GetCheck(i))
        {
            m_struOperateArray.wPDSlots[m_struOperateArray.byPDCount] = m_struPDList.struPhyDiskInfo[i].wPhySlot;
            m_struOperateArray.byPDCount++;
        }
    }
    
    m_struOperateArray.byRaidMode = m_comboRaidMode.GetItemData(m_comboRaidMode.GetCurSel());
    return TRUE;
}

BOOL CSubDlgArrayCfg::MigrateArray()
{
    BOOL bRet = FALSE;
    if (!NET_DVR_MigrateArray(m_lServerID, &m_struOperateArray))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_MigrateArray");
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_MigrateArray");
        bRet = TRUE;
    }
    return bRet;
}

BOOL CSubDlgArrayCfg::RebuildArray()
{
    BOOL bRet = FALSE;
    if (!NET_DVR_RebuildArray(m_lServerID, &m_struOperateArray))
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_FAIL_T, "NET_DVR_RebuildArray");
        bRet = FALSE;
    }
    else
    {
        g_pMainDlg->AddLog(m_iDevIndex, OPERATION_SUCC_T, "NET_DVR_RebuildArray");
        bRet = TRUE;
    }
    return bRet;
}


void CSubDlgArrayCfg::OnItemchangedListArrayInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    
    POSITION posArray = m_ListArrrayInfo.GetFirstSelectedItemPosition();
    
    m_iSelectIndex = m_ListArrrayInfo.GetNextSelectedItem(posArray);

    if (-1 == m_iSelectIndex)
    {
       return;
    }
    else
    {
         m_csArrayName = m_struArrayList.struArrayInfo[m_iSelectIndex].byArrayName;
		 int i = 0;
         for (i = 0; i < m_comboRaidMode.GetCount(); i++)
         {
             if (m_struArrayList.struArrayInfo[m_iSelectIndex].byRaidMode == m_comboRaidMode.GetItemData(i))
             {
                 m_comboRaidMode.SetCurSel(i);
             }
         }

		 int j = 0;
         for (j = 0; j < m_struArrayList.struArrayInfo[m_iSelectIndex].byPDCount; j++)
         {
             for (i = 0; i < m_listPDSlot.GetItemCount(); i++)
             {
                 if (m_struArrayList.struArrayInfo[m_iSelectIndex].wPDSlots[j] == m_listPDSlot.GetItemData(i))
                {
                    m_listPDSlot.SetCheck(i, TRUE);
                    break;
                }
             }
         }
         UpdateData(FALSE);
    }
	
	*pResult = 0;
}
