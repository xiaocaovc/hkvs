// DlgT1test.cpp : implementation file
//

#include "stdafx.h"

#include "clientdemo.h"
#include "DlgT1test.h"
#include "HCNetSDK.h"

#if _MSC_VER < 1500
#include "iphlpapi.h"
#include "ipifcons.h"
#else
#include <ifmib.h>
#endif

#ifdef _WIN64
DWORD WINAPI GetIfEntry(IN OUT PMIB_IFROW   pIfRow)
{
	return 0;
}

DWORD WINAPI GetIfTable(
		   OUT    PMIB_IFTABLE pIfTable,
		   IN OUT PULONG       pdwSize,
		   IN     BOOL         bOrder
		   )
{
	return 0;
}
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



void CALLBACK /*DATADOWNLOAD*/T1TestDownloadCallBack(LONG nDownloadHandle, DWORD dwDataType, void* pBuffer, DWORD dwBufSize, void *pUser)
{
	CDlgT1test* pT1Test = static_cast<CDlgT1test*>(pUser);
	pT1Test->appendInfo(pBuffer, dwBufSize);
//	T1Test->m_fileDown.Write(pBuffer, dwBufSize);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgT1test dialog

CDlgT1test::CDlgT1test(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgT1test::IDD, pParent)
	,m_lDownloadHandle(-1)
{
	memset(&m_struDownload, 0, sizeof(m_struDownload));
	//{{AFX_DATA_INIT(CDlgT1test)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}


void CDlgT1test::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgT1test)
	DDX_Control(pDX, IDC_EDIT3, m_editNetSpeed);
	DDX_Control(pDX, IDC_EDIT2, m_editInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgT1test, CDialog)
	//{{AFX_MSG_MAP(CDlgT1test)
	ON_BN_CLICKED(IDOK, OnStartT1TestClick)
	ON_BN_CLICKED(IDC_BTN_DEDAULT_SHUTDOWN, OnDefaultShutdownClick)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgT1test message handlers

void CDlgT1test::OnStartT1TestClick() 
{
	// TODO: Add your control notification handler code here
	GetNetWorkStatus();
	m_strTiTestInfo = "";
	CString csStr;
	m_struDownload.lpDataCallBack = T1TestDownloadCallBack;
	m_struDownload.pUserData = this;
	m_lDownloadHandle = NET_DVR_StartT1Test(g_struDeviceInfo[m_iDeviceIndex].lLoginID, &m_struDownload);
	if (m_lDownloadHandle == -1)
	{
		csStr.Format("%s", "NET_DVR_StartT1Test FAILED");
		MessageBox(csStr);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_StartT1Test FAILED");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_StartT1Test SUCC");
		GetDlgItem(IDOK)->EnableWindow(false);
	}
}

void CDlgT1test::OnDefaultShutdownClick()
{
	char *pContent = "<T1TestCmd type=\"0\"/>";
	if (NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_T1_TEST_CMD, 0, pContent, strlen(pContent)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_T1_TEST_CMD SUCC");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_T1_TEST_CMD FAILED");
	}
}

void CDlgT1test::appendInfo(void* pBuffer, DWORD dwBufSize)
{
	LONG stats = 0;
	BOOL bRet = NET_DVR_GetT1TestStatus(m_lDownloadHandle, &stats);
	if (!bRet)
	{
		CString csStr;
		csStr.Format("%s", "NET_DVR_GetT1TestStatus FAILED");
		MessageBox(csStr);
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_GetT1TestStatus FAILED");
		return;
	}
	if (stats != 2)
	{
		GetDlgItem(IDOK)->EnableWindow(true);
		return;
	}
    char *pTemp = NULL;

    TRY 
    {
        pTemp = new char[dwBufSize];
    }
    CATCH (CMemoryException, e)
    {
    	return;
    }
    END_CATCH
    if (pTemp != NULL)
    {
        memset(pTemp,0,dwBufSize);
        memcpy(pTemp, pBuffer,dwBufSize);
    }
    else
    {
        return;
    }

    char szLan[1024] = {0};

 	strncat(m_strTiTestInfo.GetBuffer(m_strTiTestInfo.GetLength() +1),  pTemp, dwBufSize);

	//m_strTiTestInfo += pTemp;
    m_editInfo.SetWindowText(GetLineAndReplace(m_strTiTestInfo));
//	GetDlgItem(IDC_EDIT2)->SetWindowText(GetLineAndReplace(m_strTiTestInfo));
	int rowCount = m_editInfo.GetLineCount();
	m_editInfo.LineScroll(rowCount);
    if (pTemp != NULL)
    {
        delete []pTemp;
        pTemp = NULL;
    }
}

CString CDlgT1test::GetLineAndReplace(CString csStr)
{
	CString csStrResult = "";
	for (int count = 0; count < csStr.GetLength(); count++)
	{
		TCHAR c = csStr.GetAt(count);
		if ('\n' == c)
		{
			csStrResult += '\r';
		}
		csStrResult += c;
	}
	return csStrResult;
}

void CDlgT1test::GetNetWorkStatus()
{
	// Declare and initialize variables.

    // Declare and initialize variables.
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    int i;

    /* variables used for GetIfTable and GetIfEntry */
    MIB_IFTABLE *pIfTable;
    MIB_IFROW *pIfRow;

    // Allocate memory for our pointers.
    pIfTable = (MIB_IFTABLE *) malloc(sizeof (MIB_IFTABLE));
    if (pIfTable == NULL) {
        printf("Error allocating memory needed to call GetIfTable\n");
    }
    // Before calling GetIfEntry, we call GetIfTable to make
    // sure there are entries to get and retrieve the interface index.

    // Make an initial call to GetIfTable to get the
    // necessary size into dwSize
    dwSize = sizeof (MIB_IFTABLE);
    if (GetIfTable(pIfTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) {
        free(pIfTable);
        pIfTable = (MIB_IFTABLE *) malloc(dwSize);
        if (pIfTable == NULL) {
            printf("Error allocating memory\n");
            return;
        }
    }
    // Make a second call to GetIfTable to get the actual
    // data we want.
    if ((dwRetVal = GetIfTable(pIfTable, &dwSize, 0)) == NO_ERROR) {
        if (pIfTable->dwNumEntries > 0) {
            pIfRow = (MIB_IFROW *) malloc(sizeof (MIB_IFROW));
            if (pIfRow == NULL) {
                printf("Error allocating memory\n");
                if (pIfTable != NULL) {
                    free(pIfTable);
                    pIfTable = NULL;
                }
				return;
            }

			int count =  (int) pIfTable->dwNumEntries;

            for (i = 0; i < (int) pIfTable->dwNumEntries; i++) {
                pIfRow->dwIndex = pIfTable->table[i].dwIndex;
                if ((dwRetVal = GetIfEntry(pIfRow)) == NO_ERROR) {
					//if(pIfRow->dwOperStatus == MIB_IF_OPER_STATUS_CONNECTED)
					//if(pIfRow->dwAdminStatus == 1)
					if(pIfRow->dwPhysAddrLen != 0)
					{
						int dwSpeed = (pIfRow->dwSpeed) / 1000 / 1000;
						CString strSpeed;
						strSpeed.Format("%d Mbps", dwSpeed);
						m_editNetSpeed.SetWindowText(strSpeed);
					}
                }
            }
        } 

    }
}

void CDlgT1test::OnButton1() 
{
	// TODO: Add your control notification handler code here
	char *pContent = "<T1TestCmd type=\"1\"/>";
	if (NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_T1_TEST_CMD, 0, pContent, strlen(pContent)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_T1_TEST_CMD SUCC");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_T1_TEST_CMD FAILED");
	}
}

void CDlgT1test::OnButton2() 
{
	// TODO: Add your control notification handler code here
	char *pContent = "<T1TestCmd type=\"2\"/>";
	if (NET_DVR_SetDVRConfig(g_struDeviceInfo[m_iDeviceIndex].lLoginID, NET_DVR_T1_TEST_CMD, 0, pContent, strlen(pContent)))
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_SUCC_T, "NET_DVR_T1_TEST_CMD SUCC");
	}
	else
	{
		g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_T1_TEST_CMD FAILED");
	}
}
