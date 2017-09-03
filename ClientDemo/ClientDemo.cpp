/**********************************************************
FileName:    ClientDemo.cpp
Description: the initiale class of the application, declare the global variable and function      
Date:        2008/05/17
Note: 		 define global variables, implement global function
Modification History:      
    <version> <time>         <desc>
    <1.0    > <2008/05/17>       <created>
***********************************************************/

#include "stdafx.h"
#include "ClientDemo.h"
#include "String.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*Declare all the global variables in ClientDemo.cpp, then add extern to them in ClientDemo.h. 
if other files need to call these global variables, just include ClientDemo.h file*/
LOCAL_SENSOR_ADD_PREVIEW g_struSensorAddPreview[MAX_ALARMHOST_ALARMIN_NUM] = {0};
//memset(&m_struSensorAddPreview, 0,sizeof(m_struSensorAddPreview));

LOCAL_FACE_PICTURE_INFO g_struFacePic[MAX_HUMAN_PICTURE_NUM];
LOCAL_FACE_MODEL_INFO g_struFaceModel[MAX_HUMAN_PICTURE_NUM];
LOCAL_DEVICE_INFO g_struDeviceInfo[MAX_DEVICES];
LOCAL_PARAM g_struLocalParam;		//demo local configure
CClientDemoDlg * g_pMainDlg = NULL;	//
CDlgSensorCfg* g_pDlgSensor = NULL;
BOOL g_bCapPic[MAX_OUTPUTS];//capture pic
BOOL g_bLocalCapPic = TRUE;
BOOL g_bInSide = TRUE;//FALSE; //callback decode
BOOL g_bExitDemo = FALSE;//control post message when exit, so can release the buffer.
BOOL g_bPtzMaualTrace = FALSE;

CDlgOutput g_dlgOutput[MAX_OUTPUTS];//video output dialog
BOOL g_bShareSound = FALSE;
HBRUSH	g_hPlayHardBgBrush = NULL;		//brush fo preview background, different color in hardware decode from in software decode
HBRUSH	g_hPlaySoftBgBrush =  NULL;
BOOL g_bCardInit = FALSE;			//card initialized successfully or not

LONG g_lVoiceHandle = -1;	//voice talk index
int g_nNetDelay = 2;

char g_szWorkDir[MAX_PATH] = {0};//demo application path

//parameters of all play or circulation play
PLAY_INFO g_struPlayAllInfo[MAX_PREVIEWS];
BOOL g_bPlayAllDevice = FALSE;		//play all the devices that have logged in
WORD g_nSurveillanceTotalNum = 0;	//surveillance points for preview(camera number)
int g_iNextSurveillanceIndex = 0;	//last surveillance point's next serial NO.
int g_pCycleTimer = 0;				//cycle timer index
BOOL g_bStandardCB = FALSE;
//support multi-VGA output
CRect g_rcWindow;				//entire area of dialog
DWORD g_dwOldDeviceNum = 0;		//display card number
int	g_iCurScreenWidth = 0;		//current screen width
int	g_iCurScreenHeight = 0;		//current screen height

BOOL g_bMustReboot = FALSE;		//reboot after modifying some configuration of the device


//2009-7-22 
DWORD g_dwStreamTransType = 0;
//2009-7-22

//demo global key
char g_sSecretKey[17] = "StreamNotEncrypt";

//the handle of DSSDK
HINSTANCE g_hDSSDK; 

//global function
void g_ReadDeviceConfig();

void g_SaveDeviceConfig();
void WriteLocalConfig();
void ReadLocalConfig();

void GetCurrentPath(char *path);
BOOL CheckIPStr(CString ip);
void g_CreateLocalDir();
void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn);
/////////////////////////////////////////////////////////////////////////////
// CClientDemoApp

/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		map between message and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CClientDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CClientDemoApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDemoApp construction

/*********************************************************
Function:	CClientDemoApp
Desc:		Constructor
Input:	
Output:	
Return:	
**********************************************************/
CClientDemoApp::CClientDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/*********************************************************
Function:	CClientDemoApp
Desc:		destructor
Input:	
Output:	
Return:	
**********************************************************/
CClientDemoApp::~CClientDemoApp()
{
	// TODO: add construction code here,
	for (int i = 0; i < MAX_DEVICES; i++)
	{
		if (g_struDeviceInfo[i].pStruIPParaCfgV40 != NULL)
		{
			delete[] g_struDeviceInfo[i].pStruIPParaCfgV40;
			g_struDeviceInfo[i].pStruIPParaCfgV40 = NULL;
		}
	}
    
	
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientDemoApp object

CClientDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClientDemoApp initialization
/*********************************************************
Function:	InitInstance
Desc:		initialize instance
Input:	
Output:	
Return:	
**********************************************************/
BOOL CClientDemoApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	g_hDSSDK = LoadLibrary("DsSdk.dll");
	NET_DVR_Init(); 
	NET_DVR_SDKLOCAL_CFG struSdkLocalCfg = {0};
	struSdkLocalCfg.byEnableAbilityParse = 1; //
	struSdkLocalCfg.byVoiceComMode = 0;   //0 use windows Api , 1 Use liabrary
    struSdkLocalCfg.byLoginWithSimXml = 1;
	NET_DVR_SetSDKLocalConfig(&struSdkLocalCfg);
	//NET_DVR_SetConnectTime(3000, 3);
	CClientDemoDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}



/*********************************************************
Function:	ExitInstance
Desc:	    exit instance
Input:	
Output:	
Return:	
**********************************************************/
int CClientDemoApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	NET_DVR_Cleanup(); 

    Gdiplus::GdiplusShutdown(m_gdiplusToken);

	return CWinApp::ExitInstance();
}

/*********************************************************
  Function:	GetCurrentPath
  Desc:		get current path of running application folder
  Input:	
  Output:	szPath, folder path
  Return:	
**********************************************************/
void GetCurrentPath(char *pPath)
{
	CString strPath;
	//get entire path of running application and its title
	GetModuleFileName(NULL,pPath,MAX_PATH);
	strPath.Format(pPath);
	int iPos=strPath.ReverseFind('\\');
	if ((unsigned)iPos == -1)
	{
		return ;
	}
	memset(pPath, 0, MAX_PATH);
	strncpy(pPath, strPath.Left(iPos).GetBuffer(0), iPos);
	pPath[iPos]='\0';
}

/*********************************************************
Function:	
Desc:		change the IP address type from CString to DWORD
Input:		strIP,CString IP address;
Output:	
Return:		DWORD IP address
**********************************************************/
DWORD CStringTodwIP(CString strIP)
{
	DWORD dwRet = 0;
	CString strIPTemp = _T("");
	CString strLeft = _T("");
	CString strRight= _T("");
	int iPos = 0,iLen = 0;

	strIPTemp.Format("%s",strIP.GetBuffer(0));
	strRight = strIPTemp;
	iPos=strRight.Find(".",0);
	iLen=strRight.GetLength();

	while (iPos>0)
	{
		strLeft.Format("%s",strRight.Left(iPos));
		dwRet=(dwRet<<8) + (unsigned)atoi(strLeft);
		strRight.Format("%s",strRight.Right(iLen - iPos - 1));
		iLen=strRight.GetLength();
		iPos=strRight.Find(".",0);
	}
	dwRet=(dwRet<<8) + (unsigned)atoi(strRight);
	return dwRet;
}

/*********************************************************
Function:	IPToStr
Desc:		change the IP address type from DWORD to CString
Input:		dwIP, DWORD;
Output:	
Return:		CString ip address;
**********************************************************/
CString IPToStr(DWORD dwIP)
{
	CString strIP = _T("");
	WORD add1,add2,add3,add4;

	add1=(WORD)(dwIP&255);
	add2=(WORD)((dwIP>>8)&255);
	add3=(WORD)((dwIP>>16)&255);
	add4=(WORD)((dwIP>>24)&255);
	strIP.Format("%d.%d.%d.%d",add4,add3,add2,add1);
	return strIP;
}

void g_CreateAlarmOutTree(CTreeCtrl &m_treeAlarmOut, int iDeviceIndex, BOOL *bAlarmOut)
{
	if (iDeviceIndex == -1)
	{
		return;
    }
    CString strTemp =_T("");
    CString strChanTmp = _T("");
    int i = 0;
    m_treeAlarmOut.DeleteAllItems();
	
	HTREEITEM hChanItem = NULL;
	HTREEITEM hFirstItem = NULL;
	
    BOOL m_bAlarmOut[MAX_ALARMOUT_V40];
	DWORD dwID = 0;
	/*  DWORD dwLoopNum = dwAlarmOutNum;*/
    memset(m_bAlarmOut,0, sizeof(BOOL)* MAX_ALARMOUT_V40);
	
	
    for (i = 0; i < /*g_struDeviceInfo[iDeviceIndex].iAlarmOutNum*/MAX_ALARMOUT_V40; i++)
    {
        m_bAlarmOut[i] = BOOL(*(bAlarmOut+i));
        if ((i <  g_struDeviceInfo[iDeviceIndex].iAlarmOutNum))
        {
            strTemp.Format(ALARM_OUT_FORMAT, i+1);
            hChanItem =  m_treeAlarmOut.InsertItem(strTemp, 0, 0, TVI_ROOT);
            if (hFirstItem == NULL)
            {
                hFirstItem = hChanItem;
            }
            m_treeAlarmOut.SetItemData(hChanItem, 0*1000 + i);	
            if (m_bAlarmOut[i])
            {
                m_treeAlarmOut.SetCheck(hChanItem, TRUE);
            }
            
        }
        else if (i >= MAX_ANALOG_ALARMOUT)
        {
            dwID = g_struDeviceInfo[iDeviceIndex].pStruIPAlarmOutCfgV40->struIPAlarmOutInfo[i-MAX_ANALOG_ALARMOUT].dwIPID;
            if (dwID != 0)
            {
                strTemp.Format(IP_ALARM_OUT_NAME, i+1-MAX_ANALOG_ALARMOUT);
                hChanItem =  m_treeAlarmOut.InsertItem(strTemp, 0, 0, TVI_ROOT);
                m_treeAlarmOut.SetItemData(hChanItem, 1*1000 + i);	
                if (m_bAlarmOut[i])
                {
                    m_treeAlarmOut.SetCheck(hChanItem, TRUE);
                }
            }
            
        }
    }
	
	//     for (i = MAX_ALARMOUT_V30; i < MAX_ALARMOUT_V40 ; i++)
	//     {
	// 		m_bAlarmOut[i] = FALSE;
//     }
	m_treeAlarmOut.SelectItem(hFirstItem);//avoid keeping clearing screen
	m_treeAlarmOut.Expand(m_treeAlarmOut.GetRootItem(),TVE_EXPAND);
}
//[add] by zja 2014-2-28 
//注： 升级IPC功能时，调用该接口得到的通道号是不正确的，
// 应该将 m_treeChannel.SetItemData(hChanItem, 0*1000 + i);
// 改为m_treeChannel.SetItemData(hChanItem, 0*1000 + g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChannelNO)
// 由于好几个地方调用该接口，并没有反映该问题，不做修改。
void g_CreateChannelTree(CTreeCtrl &m_treeChannel, int iDeviceIndex, BOOL *bChannelChecked)
{
	if (iDeviceIndex == -1)
	{
		return;
	}
	m_treeChannel.DeleteAllItems();
	CString strTemp =_T("");
	CString strChanTmp = _T("");
	int i = 0;
	HTREEITEM hChanItem = NULL;
	HTREEITEM hFirstItem = NULL;
	DWORD dwID = 0;
	
	BOOL bChanChecked[MAX_CHANNUM_V40] = {0};
	
	for(i = 0; i < g_struDeviceInfo[iDeviceIndex].iDeviceChanNum ; i++)
	{
		if(i < g_struDeviceInfo[iDeviceIndex].iAnalogChanNum)
		{
			if (g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].bEnable)
			{
				bChanChecked[i] = BOOL(*(bChannelChecked+i));
				strTemp.Format(ANALOG_C_FORMAT, i+1);
				hChanItem =  m_treeChannel.InsertItem(strTemp, 0, 0, TVI_ROOT);
				if (hFirstItem == NULL)
				{
					hFirstItem = hChanItem;
				}
				
				m_treeChannel.SetItemData(hChanItem, 0*1000 + i);
				if (bChannelChecked[g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChannelNO -1])
				{
					m_treeChannel.SetCheck(hChanItem, TRUE);
				}
			}
		}
		else
		{
			if (g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].bEnable)
			{
				bChanChecked[i] = BOOL(*(bChannelChecked+i));
				strTemp.Format(IP_CAMERA_NAME, i+1 - g_struDeviceInfo[iDeviceIndex].iAnalogChanNum);
				hChanItem =  m_treeChannel.InsertItem(strTemp, 0, 0, TVI_ROOT);
				if (hFirstItem == NULL)
				{
					hFirstItem = hChanItem;
				}
				m_treeChannel.SetItemData(hChanItem, 0*1000 + i);
				if (bChannelChecked[g_struDeviceInfo[iDeviceIndex].pStruChanInfo[i].iChannelNO -1])
				{
					m_treeChannel.SetCheck(hChanItem, TRUE);
				}
			}
		}
	}
	
	m_treeChannel.SelectItem(hFirstItem);//avoid keeping clearing screen
	m_treeChannel.Expand(m_treeChannel.GetRootItem(),TVE_EXPAND);
}

/*********************************************************
  Function:	g_CreateLocalDir
  Desc:		create the local directory
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void g_CreateLocalDir()
{
	CString csDir;
	if (g_struLocalParam.chDownLoadPath != NULL)
	{
		csDir.Format("%s\\", g_struLocalParam.chDownLoadPath);
		if (GetFileAttributes(g_struLocalParam.chDownLoadPath) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CreateDirectory(csDir, NULL);
		}
	}
	if (g_struLocalParam.chPictureSavePath != NULL)
	{
		csDir.Format("%s\\", g_struLocalParam.chPictureSavePath);
		if (GetFileAttributes(g_struLocalParam.chPictureSavePath) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CreateDirectory(csDir, NULL);
		}
	}
	if (g_struLocalParam.chRemoteCfgSavePath != NULL)
	{
		csDir.Format("%s\\", g_struLocalParam.chRemoteCfgSavePath);
		if (GetFileAttributes(g_struLocalParam.chRemoteCfgSavePath) != FILE_ATTRIBUTE_DIRECTORY)
		{
			CreateDirectory(csDir, NULL);
		}
	}
}

int g_buflen(char *buf)
{
	char *pos = NULL;
	int iBufLen = 0;
	pos = strchr(buf, '\r');
	if (pos != NULL)
	{
		iBufLen = pos - buf;
		return iBufLen;
	}
	pos = strchr(buf, '\n');
	if (pos != NULL)
	{
		iBufLen = pos - buf;
	}
	return iBufLen;
}

/*********************************************************
  Function:	g_ReadDeviceConfig
  Desc:		read configure of the devices and their channels from "DeviceCfg.dat"
  Input:	
  Output:	
  Return:	
**********************************************************/
void g_ReadDeviceConfig()
{
	int i, j;
	char buf[256];
	CStdioFile myFile;
	CString csFilename;

	csFilename.Format("%s\\DeviceCfg.dat", g_szWorkDir);
	if (myFile.Open(csFilename, CFile::modeRead) == FALSE)
	{
		myFile.Open(csFilename, CFile::modeCreate);
		return;
	}
	g_nSurveillanceTotalNum=0;

	while (myFile.ReadString(buf,256))
	{
		if (buf[1]==0x53)
		{
			myFile.ReadString(buf,256);//array index
			i=atoi(buf);

			if( g_struDeviceInfo[i].pStruChanInfo == NULL)
            {
                g_struDeviceInfo[i].pStruChanInfo =  new STRU_CHANNEL_INFO[MAX_CHANNUM_V40];
                if (g_struDeviceInfo[i].pStruChanInfo == NULL)
                {
                    break;
                }
            }
			
            if (g_struDeviceInfo[i].pStruIPAlarmInCfgV40 == NULL)
            {
				g_struDeviceInfo[i].pStruIPAlarmInCfgV40 = new NET_DVR_IPALARMINCFG_V40; 
                if (g_struDeviceInfo[i].pStruIPAlarmInCfgV40  == NULL)
                {
                    break;
                }
                memset(g_struDeviceInfo[i].pStruIPAlarmInCfgV40, 0, sizeof(NET_DVR_IPALARMINCFG_V40));
            }
			
			
            if (g_struDeviceInfo[i].pStruIPAlarmOutCfgV40  == NULL)
            {
                g_struDeviceInfo[i].pStruIPAlarmOutCfgV40 =  new NET_DVR_IPALARMOUTCFG_V40;
                
                if (g_struDeviceInfo[i].pStruIPAlarmOutCfgV40 == NULL)
                {
                    break;
                }
                memset(g_struDeviceInfo[i].pStruIPAlarmOutCfgV40, 0, sizeof(NET_DVR_IPALARMOUTCFG_V40));
            }

			g_struDeviceInfo[i].iDeviceIndex = i;
			//device basic info
			myFile.ReadString(buf,256);

			strncpy(g_struDeviceInfo[i].chSerialNumber, buf, g_buflen(buf)/*strlen(buf)-2*/);
			myFile.ReadString(buf,256);

			g_struDeviceInfo[i].iDeviceChanNum=atoi(buf);//channel number
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].iStartChan=atoi(buf);//start channel number
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].iAlarmInNum=atoi(buf);//alarm input number
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].iAlarmOutNum=atoi(buf);//alarm output number
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].iIPChanNum =atoi(buf);//alarm output number

			int dwStruIPParaCfgV40Num = 0;
            int iCurIPChanNum = g_struDeviceInfo[i].iIPChanNum;
            if (iCurIPChanNum > 0)
            {
                if (( iCurIPChanNum % MAX_CHANNUM_V30) == 0)
                {
                    dwStruIPParaCfgV40Num =    iCurIPChanNum/MAX_CHANNUM_V30;
                }
                else
                {
                    dwStruIPParaCfgV40Num =   iCurIPChanNum/MAX_CHANNUM_V30 + 1;
                }
                
                g_struDeviceInfo[i].pStruIPParaCfgV40 =new NET_DVR_IPPARACFG_V40[dwStruIPParaCfgV40Num];
				
                if (g_struDeviceInfo[i].pStruIPParaCfgV40 == NULL)
                {
                    char szLan[256] = {0};            
                    g_StringLanType(szLan, "申请内存失败", "Alloc memory  failed");            
                    AfxMessageBox(szLan);
                    break;
                }
                
                for (int k = 0; k < dwStruIPParaCfgV40Num; k++)
                {
                    memset(&g_struDeviceInfo[i].pStruIPParaCfgV40[k], 0, sizeof(NET_DVR_IPPARACFG_V40));
                }
                
            }

			myFile.ReadString(buf,256);
			strncpy(g_struDeviceInfo[i].chLocalNodeName, buf, g_buflen(buf)/*strlen(buf)-2*/);//device title
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].iDeviceType=atoi(buf);//device type
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].iDiskNum=atoi(buf);//hard disk number

			//network configuration
			myFile.ReadString(buf,256);
			strncpy(g_struDeviceInfo[i].chLoginUserName,buf, g_buflen(buf)/*strlen(buf)-2*/);//user name
			myFile.ReadString(buf,256);
			strncpy(g_struDeviceInfo[i].chLoginPwd,buf, g_buflen(buf)/*strlen(buf)-2*/);
			myFile.ReadString(buf,256);
			strncpy(g_struDeviceInfo[i].chDeviceIP, buf, g_buflen(buf)/*strlen(buf)-2*/);
			//myFile.ReadString(buf,256);
			//strncpy(g_struDeviceInfo[i].chDevNetCard1IP, buf, g_buflen(buf)/*strlen(buf)-2*/);
			myFile.ReadString(buf,256);
			strncpy(g_struDeviceInfo[i].chDeviceMultiIP, buf, g_buflen(buf)/*strlen(buf)-2*/);
			//myFile.ReadString(buf,256);
			//strncpy(g_struDeviceInfo[i].chIPServerIP, buf, g_buflen(buf)/*strlen(buf)-2*/);
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].lDevicePort = atoi(buf);//port
			//2011-12-29
			myFile.ReadString(buf,256);
			strncpy(g_struDeviceInfo[i].chDeviceName,buf,g_buflen(buf)/*strlen(buf)-2*/);
		
            g_struDeviceInfo[i].iAnalogChanNum = g_struDeviceInfo[i].iDeviceChanNum - g_struDeviceInfo[i].iIPChanNum;
//			g_nDeviceNum++;
		}
		else if (buf[1] = 0x43)
		{
			myFile.ReadString(buf,256);//array index
			i=atoi(buf);
			myFile.ReadString(buf,256);//array index
			j=atoi(buf);

			if( g_struDeviceInfo[i].pStruChanInfo == NULL)
            {
                g_struDeviceInfo[i].pStruChanInfo =  new STRU_CHANNEL_INFO[MAX_CHANNUM_V40];
                if (g_struDeviceInfo[i].pStruChanInfo == NULL)
                {
                    break;
                }
            }
			
            if (g_struDeviceInfo[i].iIPChanNum >= 0 && g_struDeviceInfo[i].pStruIPParaCfgV40 == NULL)
            {
				g_struDeviceInfo[i].pStruIPParaCfgV40 =new NET_DVR_IPPARACFG_V40[g_struDeviceInfo[i].iIPChanNum/MAX_CHANNUM_V30 + 1];
				if (g_struDeviceInfo[i].pStruIPParaCfgV40 == NULL)
				{
					char szLan[256] = {0};            
					g_StringLanType(szLan, "申请内存失败", "Alloc memory  failed");            
					AfxMessageBox(szLan);
					break;
				}
				
				for (int k = 0; k < g_struDeviceInfo[i].iIPChanNum/MAX_CHANNUM_V30 + 1; k++)
				{
					memset(&g_struDeviceInfo[i].pStruIPParaCfgV40[k], 0, sizeof(NET_DVR_IPPARACFG_V40));
				}
            }
			
            if (g_struDeviceInfo[i].pStruIPAlarmInCfgV40 == NULL)
            {
				g_struDeviceInfo[i].pStruIPAlarmInCfgV40 = new NET_DVR_IPALARMINCFG_V40; 
                if (g_struDeviceInfo[i].pStruIPAlarmInCfgV40  == NULL)
                {
                    break;
                }
            }
            
            
            if (g_struDeviceInfo[i].pStruIPAlarmOutCfgV40  == NULL)
            {
                g_struDeviceInfo[i].pStruIPAlarmOutCfgV40 =  new NET_DVR_IPALARMOUTCFG_V40;
                
                if (g_struDeviceInfo[i].pStruIPAlarmOutCfgV40 == NULL)
                {
                    break;
                }
            }

// 			if (i == NULL||j == NULL)
// 			{
// 				continue;
// 			}
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].iDeviceIndex=atoi(buf);//device index

			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].iChanIndex=atoi(buf);//channel index

			myFile.ReadString(buf,256);
            memset(g_struDeviceInfo[i].pStruChanInfo[j].chChanName, 0, 100);
			strncpy(g_struDeviceInfo[i].pStruChanInfo[j].chChanName,buf, g_buflen(buf)/*strlen(buf)-2*/);//channel title
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].dwProtocol=atoi(buf);//protocol

			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].iPicResolution = atoi(buf);//pic resolution
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].iPicQuality = atoi(buf);//pic quality
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].bEnable = atoi(buf);//channel enable
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].dwPreviewMode = atoi(buf); //预览模式
			myFile.ReadString(buf,256);
			g_struDeviceInfo[i].pStruChanInfo[j].nPreviewProtocolType = atoi(buf); 
		}
	}

	myFile.Close();	
}

/*********************************************************
  Function:	g_ValidIPv6
  Desc:		Is ipv6? 
  Input:	ip
  Output:	none
  Return:	true, false
**********************************************************/
BOOL g_ValidIPv6(BYTE *ip)
{
	if (strlen((char *)ip) > 2 && strchr((char *)ip, ':') != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

void g_replace(char* pOldString, char* pNewString, char* pSearchString, char* pRepalceString)
{
	string temp2("");
	string searchString("");
	string replaceString("");
	temp2 = temp2.insert(0, pOldString);
	searchString = searchString.insert(0, pSearchString);
	replaceString = replaceString.insert(0, pRepalceString);

    string::size_type pos = 0;
	while ( (pos = temp2.find(searchString, pos)) != string::npos )
	{	
	    temp2.replace( pos, searchString.size(), replaceString);	
	    pos++;	
	}
	strncpy(pNewString,temp2.c_str(),temp2.length());
}

/*********************************************************
  Function:	g_SaveDeviceConfig
  Desc:		save the device configure in "DeivceCfg.dat" 
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void g_SaveDeviceConfig()
{
	int i, j;
	char buf[256];
	CStdioFile myFile;
	CString csFilename;

	csFilename.Format("%s\\DeviceCfg.dat", g_szWorkDir);
	if (myFile.Open(csFilename, CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		return;
	}
	for (i = 0; i < MAX_DEVICES; i++)
	{
		if (g_struDeviceInfo[i].iDeviceChanNum != -1)
		{
			sprintf(buf,"$S\r\n");
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",i);
			myFile.WriteString(buf);
			// device basic info
			sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chSerialNumber);		//device serial NO.
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].iDeviceChanNum);		//channel number
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].iStartChan);		//start channel
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].iAlarmInNum);		//alarm input number
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].iAlarmOutNum);		//alarm output number
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].iIPChanNum);		//alarm output number
			myFile.WriteString(buf);
			sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chLocalNodeName);				//device title
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].iDeviceType);				//device type
			myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].iDiskNum);				//hard disk number
			myFile.WriteString(buf);
			//network configuration
			sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chLoginUserName);		//user name
			myFile.WriteString(buf);
			sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chLoginPwd);		//passwork
			myFile.WriteString(buf);
			sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chDeviceIP);		 //IP address of the device in IPServer
			myFile.WriteString(buf);
			//sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chDevNetCard1IP);				//local IP address
			//myFile.WriteString(buf);
			sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chDeviceMultiIP);			//multiIP address
			myFile.WriteString(buf);
			//sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chIPServerIP);				//dnsip address
			//myFile.WriteString(buf);
			sprintf(buf,"%d\r\n",g_struDeviceInfo[i].lDevicePort);				//port number
			myFile.WriteString(buf);
			//2011-12-29
			sprintf(buf,"%s\r\n",g_struDeviceInfo[i].chDeviceName);       //device type name
			myFile.WriteString(buf);
			for (j=0; j< MAX_CHANNUM_V40; j++)
			{
				if (g_struDeviceInfo[i].pStruChanInfo[j].iChanIndex != -1)
				{
					sprintf(buf,"$C\r\n");
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",i);
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",j);
					myFile.WriteString(buf);
		
					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].iDeviceIndex);		//device index
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].iChanIndex);		//channel index
					myFile.WriteString(buf);
					sprintf(buf,"%s\r\n",g_struDeviceInfo[i].pStruChanInfo[j].chChanName);			//surveillance title
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].dwProtocol);		//protocole
					myFile.WriteString(buf);

					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].iPicResolution);//pic resolution
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].iPicQuality);//pic quality
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].bEnable);//channel enable
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].dwPreviewMode);			//priview mode
					myFile.WriteString(buf);
					sprintf(buf,"%d\r\n",g_struDeviceInfo[i].pStruChanInfo[j].nPreviewProtocolType);			//priview protocol type
					myFile.WriteString(buf);
				}
			}
		}
	}
	myFile.Close();
}

/*********************************************************
  Function:	ReadLocalConfig
  Desc:		read local configure in "DemoLocalCfg.dat"
  Input:	none
  Output:	none
  Return:	none
**********************************************************/
void ReadLocalConfig()
{
	int iTemp = 0;
	int iDay = 0;
	int i = 0;
	int j = 0;
	char buf[256];
	CStdioFile myFile;
	CString csFilename;
	CString csDir;

	csFilename.Format("%s\\DemoLocalCfg.dat", g_szWorkDir);
	if (myFile.Open(csFilename, CFile::modeRead) == FALSE)
	{
		myFile.Open(csFilename, CFile::modeCreate);
		return;
	}

	if (!myFile.ReadString(buf,256))//1
	{
		myFile.Close();
		return;
	}
	iTemp = atoi(buf);
	if (iTemp < 5)
	{
		g_struLocalParam.iCycleTime = 5;				//cycle play enduration
	}
	else
	{
		g_struLocalParam.iCycleTime = iTemp;
	}
	myFile.ReadString(buf,256);//2
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bCyclePlay = TRUE;				// whether cycle play
	}
	else
	{
		g_struLocalParam.bCyclePlay = FALSE;
	}
	myFile.ReadString(buf,256);//3
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bUseCard = TRUE;				//whether using decode card
	}
	else
	{
		g_struLocalParam.bUseCard = FALSE;
	}

	myFile.ReadString(buf,256);//4
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bNTSC = TRUE;				//hardware decode mode
	}
	else
	{
		g_struLocalParam.bNTSC = FALSE;
	}

	myFile.ReadString(buf,256);//5
	iTemp = atoi(buf);
	if (iTemp > 60)
	{
		g_struLocalParam.iRecordFileInterval = 60;		//record file packing interval
	}
	else
	{
		g_struLocalParam.iRecordFileInterval = iTemp;
	}
	myFile.ReadString(buf,256);//6
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bAutoRecord = TRUE;			//whether auto-record 
	}
	else
	{
		g_struLocalParam.bAutoRecord = FALSE;
	}
	myFile.ReadString(buf,256);//7
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bCycleRecord = TRUE;			//whether cycle record
	}
	else
	{
		g_struLocalParam.bCycleRecord = FALSE;
	}
	myFile.ReadString(buf, 256);//8
	g_struLocalParam.iStartRecordDriver = atoi(buf);	//start record driver
	myFile.ReadString(buf, 256);//9
	g_struLocalParam.iEndRecordDriver = atoi(buf);		//end record driver

	myFile.ReadString(buf,256);//10
	strncpy(g_struLocalParam.chDownLoadPath, buf, strlen(buf)-2);//download remote file path
	myFile.ReadString(buf,256);//11
	strncpy(g_struLocalParam.chPictureSavePath, buf, strlen(buf)-2);//picture save path

	
	myFile.ReadString(buf,256);//12
	strncpy(g_struLocalParam.chRemoteCfgSavePath, buf, strlen(buf)-2);//log file save path
	g_CreateLocalDir();

	myFile.ReadString(buf,256);//13
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bAutoCheckDeviceTime = TRUE;				//whether initialize auto check device time
	}
	else
	{
		g_struLocalParam.bAutoCheckDeviceTime = FALSE;
	}

	LONG lTemp;
	myFile.ReadString(buf,256);//14
	lTemp = atol(buf);
	if (lTemp < 0)
	{
		g_struLocalParam.lCheckDeviceTime = 0;					//check device time
	}
	else
	{
		g_struLocalParam.lCheckDeviceTime = lTemp;
	}
	myFile.ReadString(buf,256);//15
	g_struLocalParam.iAlarmDelayTime = atoi(buf);// alarm delay time
	myFile.ReadString(buf,256);//16
	g_struLocalParam.iAlarmListenPort = atoi(buf);//alarm listening port

	myFile.ReadString(buf,256);//17
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bAutoSaveLog = TRUE;				//whether auto save log
	}
	else
	{
		g_struLocalParam.bAutoSaveLog = FALSE;
	}

	myFile.ReadString(buf,256);//18
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bAlarmInfo = TRUE;				//whether save alarm info
	}
	else
	{
		g_struLocalParam.bAlarmInfo = FALSE;
	}

	myFile.ReadString(buf,256);//19
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bSuccLog = TRUE;				//whether save succ log
	}
	else
	{
		g_struLocalParam.bSuccLog = FALSE;
	}

	myFile.ReadString(buf,256);//20
	iTemp = atoi(buf);
	if (iTemp == 1)
	{
		g_struLocalParam.bFailLog = TRUE;				//whether save fail info
	}
	else
	{
		g_struLocalParam.bFailLog = FALSE;
	}

	myFile.ReadString(buf,256);//21
	//strncpy(g_struLocalParam.chIPServerIP, buf, strlen(buf)-2);//ipserver address

	myFile.ReadString(buf,256);//22
	if (buf != NULL)
	{
		iTemp = atoi(buf);
		if (iTemp == 1)
		{
			g_struLocalParam.bNoDecode = TRUE;				//whether decode or not
		}
		else
		{
			g_struLocalParam.bNoDecode = FALSE;
		}
	}

	myFile.ReadString(buf,256);//23
	if (buf != NULL)
	{
		iTemp = atoi(buf);
		if (iTemp == 1)
		{
			g_struLocalParam.bPreviewBlock = TRUE;				//whether block or not
		}
		else
		{
			g_struLocalParam.bPreviewBlock = FALSE;
		}
	}

	myFile.ReadString(buf,256);//24
	if (buf != NULL)
	{
		iTemp = atoi(buf);
		if (iTemp == 1)
		{
			g_struLocalParam.bOutputDebugString = TRUE;				//whether output the demo debug info
		}
		else
		{
			g_struLocalParam.bOutputDebugString = FALSE;
		}
	}
	
	//local record schedule
	while (myFile.ReadString(buf, 256))//25
	{
		g_struLocalParam.struLocalRecordTime[iDay][0].iStartTime = (WORD)atoi(buf);
		myFile.ReadString(buf, 256);//26
		g_struLocalParam.struLocalRecordTime[iDay][0].iStopTime = (WORD)atoi(buf);
		myFile.ReadString(buf, 256);//27
		g_struLocalParam.struLocalRecordTime[iDay][1].iStartTime = (WORD)atoi(buf);
		myFile.ReadString(buf, 256);//28
		g_struLocalParam.struLocalRecordTime[iDay][1].iStopTime = (WORD)atoi(buf);
		myFile.ReadString(buf, 256);//29
		g_struLocalParam.struLocalRecordTime[iDay][2].iStartTime = (WORD)atoi(buf);
		myFile.ReadString(buf, 256);//30
		g_struLocalParam.struLocalRecordTime[iDay][2].iStopTime = (WORD)atoi(buf);
		myFile.ReadString(buf, 256);//31
		g_struLocalParam.struLocalRecordTime[iDay][3].iStartTime = (WORD)atoi(buf);
		myFile.ReadString(buf, 256);//32
		g_struLocalParam.struLocalRecordTime[iDay][3].iStopTime = (WORD)atoi(buf);
		iDay++;
		if (iDay >= MAX_DAYS)
		{
			break;
		}
	}
	for (i = 0; i < MAX_DAYS; i++)
	{
		for (j = 0; j < 4; j++)
		{
			g_struLocalParam.struLocalRecordTime[i][j].iStartHour =(WORD)(g_struLocalParam.struLocalRecordTime[i][j].iStartTime / 100);
			g_struLocalParam.struLocalRecordTime[i][j].iStartMinute = (WORD)(g_struLocalParam.struLocalRecordTime[i][j].iStartTime % 100);
			g_struLocalParam.struLocalRecordTime[i][j].iStopHour = (WORD)(g_struLocalParam.struLocalRecordTime[i][j].iStopTime / 100);
			g_struLocalParam.struLocalRecordTime[i][j].iStopMinute = (WORD)(g_struLocalParam.struLocalRecordTime[i][j].iStopTime % 100);
		}
	}

	myFile.ReadString(buf,256);
	if (buf != NULL)
	{
		g_struLocalParam.dwBFrameNum = atoi(buf);
	}
	
	myFile.ReadString(buf,256);
	if (buf != NULL)
	{
		g_bInSide = atoi(buf);//only get stream or get and play stream 
	}
	myFile.ReadString(buf,256);
	if (buf != NULL)
	{
		g_struLocalParam.nLogLevel = atoi(buf);
	}
	myFile.ReadString(buf,256);
	if (buf != NULL)
	{
		g_bStandardCB = atoi(buf);//private or standard packect
	}
	myFile.Close();
}

/*********************************************************
  Function:	WriteLocalConfig
  Desc:		save local configuration, LocalCfg.dat is called when application exit
  Input:	
  Output:	
  Return:	
**********************************************************/
void WriteLocalConfig()
{
	int i, j;
	char buf[256];
	CStdioFile myFile;
	CString csFilename;

	csFilename.Format("%s\\DemoLocalCfg.dat", g_szWorkDir);
	if (myFile.Open(csFilename, CFile::modeCreate|CFile::modeWrite) == FALSE)
	{
		return;
	}
	sprintf(buf,"%d\r\n",g_struLocalParam.iCycleTime);
	myFile.WriteString(buf);
	if (g_struLocalParam.bCyclePlay)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	if (g_struLocalParam.bUseCard)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	if (g_struLocalParam.bNTSC)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);
	
	sprintf(buf, "%d\r\n", g_struLocalParam.iRecordFileInterval);
	myFile.WriteString(buf);

	if (g_struLocalParam.bAutoRecord)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);
	if (g_struLocalParam.bCycleRecord)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}

	myFile.WriteString(buf);
	sprintf(buf, "%d\r\n", g_struLocalParam.iStartRecordDriver);
	myFile.WriteString(buf);
	sprintf(buf, "%d\r\n", g_struLocalParam.iEndRecordDriver);
	myFile.WriteString(buf);

	sprintf(buf, "%s\r\n", g_struLocalParam.chDownLoadPath);
	myFile.WriteString(buf);
	sprintf(buf, "%s\r\n", g_struLocalParam.chPictureSavePath);
	myFile.WriteString(buf);
	sprintf(buf, "%s\r\n", g_struLocalParam.chRemoteCfgSavePath);
	myFile.WriteString(buf);

	sprintf(buf, "%d\r\n", g_struLocalParam.bAutoCheckDeviceTime);
	myFile.WriteString(buf);

	if (g_struLocalParam.lCheckDeviceTime)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	sprintf(buf,"%d\r\n",g_struLocalParam.iAlarmDelayTime);//alarm delay time, check whether alarm overtime
	myFile.WriteString(buf);
	sprintf(buf,"%d\r\n",g_struLocalParam.iAlarmListenPort);//local alarm listen port
	myFile.WriteString(buf);


	if (g_struLocalParam.bAutoSaveLog)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	if (g_struLocalParam.bAlarmInfo)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	if (g_struLocalParam.bSuccLog)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	if (g_struLocalParam.bFailLog)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	sprintf(buf, "%s\r\n", g_struLocalParam.chPictureSavePath);
	myFile.WriteString(buf);

	if (g_struLocalParam.bNoDecode)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	if (g_struLocalParam.bPreviewBlock)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	if (g_struLocalParam.bOutputDebugString)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	for (i = 0; i < 7; i++)
	{
		for (j = 0; j < 4; j++)
		{
			sprintf(buf, "%d\r\n", g_struLocalParam.struLocalRecordTime[i][j].iStartTime);
			myFile.WriteString(buf);
			sprintf(buf, "%d\r\n", g_struLocalParam.struLocalRecordTime[i][j].iStopTime);
			myFile.WriteString(buf);
		}
	}
	sprintf(buf, "%d\r\n", g_struLocalParam.dwBFrameNum);
	myFile.WriteString(buf);

	if (g_bInSide)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);

	sprintf(buf,"%d\r\n", g_struLocalParam.nLogLevel);
	myFile.WriteString(buf);
	if (g_bStandardCB)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);
	sprintf(buf,"%d\r\n", g_struLocalParam.nTimeout);
	myFile.WriteString(buf);
	if (g_bStandardCB)
	{
		sprintf(buf,"1\r\n");
	}
	else
	{
		sprintf(buf,"0\r\n");
	}
	myFile.WriteString(buf);
	g_CreateLocalDir();

	myFile.Close();
}

/*********************************************************
  Function:	CheckIPStr
  Desc:		check IP address available
  Input:	ip, 
  Output:	
  Return:	TRUE,IP address correct;FALSE,IP address fault;
**********************************************************/
BOOL CheckIPStr(CString ip)
{	//check IP address correct
	char str[20];
	strcpy(str,ip);
    BOOL bIPV4 =  TRUE; 
	char a;
	int dot=0;
	int a3,a2,a1,a0,i = 0;
	a3 = a2 = a1 = a0 = -1;
	if (strlen(str)==0)
		return TRUE;
	while (a=str[i++])
	{
		if ((a==' ')||(a=='.')||((a>='0')&&(a<='9')))
		{
			if (a=='.')
				dot++;
		}
		else
        {
           bIPV4 = FALSE;
        }
			 
	}

    if (!bIPV4)
    {
        
        strcpy(str,ip);
        return g_ValidIPv6((BYTE*)str);
    }
	if (dot!=3)
	{
		return FALSE;
	}
	else
	{
		sscanf(str,"%d.%d.%d.%d",&a3,&a2,&a1,&a0);
		if ((a0>255)||(a1>255)||(a2>255)||(a3>255))
		{
			return FALSE;
		}
		if ((a0 < 0) || (a1 < 0) || (a2 < 0) || (a3 < 0))
		{
			return FALSE;
		}
	}
	return TRUE;
}

/*********************************************************
  Function:	g_StringLanType
  Desc:		get the current operation language string type
  Input:	szLanCn, Chinese string; szLanEn, English string;
  Output:	szDstLan, current string
  Return:	none
**********************************************************/
void g_StringLanType(char *szDstLan, char *szLanCn, char *szLanEn)
{
#ifdef DEMO_LAN_CN
		sprintf(szDstLan, "%s", szLanCn);	
#else	
		sprintf(szDstLan, "%s", szLanEn);
#endif
	return;
    WORD wLangID=(WORD)(PRIMARYLANGID(::GetSystemDefaultLangID()));
    switch(wLangID)
    {
        case LANG_ENGLISH:
			sprintf(szDstLan, "%s", szLanEn);
			break;
		case LANG_CHINESE:
			sprintf(szDstLan, "%s", szLanCn);
			break;
		default:
			sprintf(szDstLan, "%s", szLanEn);
			break;
	}	
}

char szDstLan[1024] = {0};
/*********************************************************
  Function:	g_StringLanType
  Desc:		get the current operation language string type
  Input:	szLanCn, Chinese string; szLanEn, English string;
  Output:	szDstLan, current string
  Return:	none
**********************************************************/
char *gg_StringLanType(char *szLanCn, char *szLanEn, const char* format, ...)
{
	va_list arglist;
	va_start(arglist,format);
	vsprintf(szDstLan, format, arglist); 
	va_end(arglist);
#ifdef DEMO_LAN_CN
		sprintf(szDstLan, "%s%s", szLanCn, szDstLan);	
#else	
		sprintf(szDstLan, "%s%s", szLanEn, szDstLan);
#endif
	return szDstLan;
}

void g_ReadLocalReadSensorAdd()
{
    CFile myFile;
    CString csFilename;

    csFilename.Format("%s\\LocalSensorAdd.dat", g_szWorkDir);
    if (myFile.Open(csFilename, CFile::modeRead) == FALSE)
    {
        myFile.Open(csFilename, CFile::modeCreate);
        return;
	}

    myFile.Read(g_struSensorAddPreview, sizeof(g_struSensorAddPreview));
       
    myFile.Close();
}

void g_SaveLocalReadSensorAdd()
{
    CFile myFile;
    CString csFilename;
    
    csFilename.Format("%s\\LocalSensorAdd.dat", g_szWorkDir);
    if (myFile.Open(csFilename, CFile::modeRead|CFile::modeWrite) == FALSE)
    {
        myFile.Open(csFilename, CFile::modeCreate);
        return;
    }

    myFile.Write(g_struSensorAddPreview, sizeof(g_struSensorAddPreview));
    
    myFile.Close();	
}

int g_DelFromArray(DWORD *dwArray, int iMaxArrayIndex, int iCurIndex)
{
    // TODO: Add your control notification handler code here
    if (dwArray == NULL)
    {
        return FALSE;
    }
    
    DWORD *pArrayTemp = (DWORD *)dwArray;
    int i;
    for (i = 0; i < iMaxArrayIndex -1; i++)
    {
        if (iCurIndex <= i)
        {
            *(pArrayTemp + i) = *(pArrayTemp + i+1);
        }
    }
    *(pArrayTemp + iMaxArrayIndex -1) = 0 ;
    return TRUE;	
}

int g_FindFromArray(DWORD *dwArray, int iMaxArrayIndex, DWORD dwFindValue)
{
    if (dwArray == NULL)
    {
        return -1;
    }
    
    for (int i = 0; i < iMaxArrayIndex ; i++)
    {
        if (dwFindValue == *(dwArray +i))
        {
            return i;
        }
    }
    
    return -1;
}