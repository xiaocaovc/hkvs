// DlgAbilityAutoDetect.cpp : implementation file
//

#include "stdafx.h"
#include "clientdemo.h"
#include "DlgAbilityAutoDetect.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAbilityAutoDetect dialog
#define XML_ABILITY_IN_LEN	1024
#define XML_ABILITY_OUT_LEN	3*1024*1024

CDlgAbilityAutoDetect::CDlgAbilityAutoDetect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAbilityAutoDetect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAbilityAutoDetect)
	m_strDevAbility = _T("");
	m_strLostAbility = _T("");
	m_dwLastError = 0;
	m_dwReturnValue = 0;
	m_lServerID = -1;
	m_pOutBuf = NULL;
	m_dwAbilityType = 0;
	m_strInputParam = _T("");
	//}}AFX_DATA_INIT
	memset(&m_struSdkLocalCfg, 0, sizeof(m_struSdkLocalCfg));
}


void CDlgAbilityAutoDetect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAbilityAutoDetect)
	DDX_Control(pDX, IDC_CMB_ABILITY_TYPE, m_cmbAbilityType);
	DDX_Text(pDX, IDC_EDT_ABILITY_SHOW, m_strDevAbility);
	DDX_Text(pDX, IDC_EDT_ABILITY_LOST, m_strLostAbility);
	DDX_Text(pDX, IDC_EDT_LAST_ERROR, m_dwLastError);
	DDX_Text(pDX, IDC_EDT_RETURN_VALUE, m_dwReturnValue);
	DDX_Text(pDX, IDC_EDT_INPUT_PARAM, m_strInputParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAbilityAutoDetect, CDialog)
	//{{AFX_MSG_MAP(CDlgAbilityAutoDetect)
	ON_BN_CLICKED(IDC_BTN_USE_SIM_ABILITY, OnBtnUseSimAbility)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CMB_ABILITY_TYPE, OnSelchangeCmbAbilityType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAbilityAutoDetect message handlers

void CDlgAbilityAutoDetect::OnBtnUseSimAbility() 
{
	// TODO: Add your control notification handler code here
	memset(&m_struSdkLocalCfg, 0 ,sizeof(m_struSdkLocalCfg));
	NET_DVR_GetSDKLocalConfig(&m_struSdkLocalCfg);
	
	if (1 == m_struSdkLocalCfg.byEnableAbilityParse)
	{
		m_struSdkLocalCfg.byEnableAbilityParse = 0;
		GetDlgItem(IDC_BTN_USE_SIM_ABILITY)->SetWindowText("不使用模拟能力");
	} 
	else
	{
		m_struSdkLocalCfg.byEnableAbilityParse = 1;
		GetDlgItem(IDC_BTN_USE_SIM_ABILITY)->SetWindowText("使用模拟能力");
	}
	
	if (!NET_DVR_SetSDKLocalConfig(&m_struSdkLocalCfg))
	{
		AfxMessageBox("fail!");
	}
}

void CDlgAbilityAutoDetect::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDT_ABILITY_SHOW)->SetWindowText("");
	GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("");
	UpdateData(TRUE);
	if (m_dwAbilityType == 0)
	{
		AfxMessageBox("请选择正确的能力集类型!");
		return ;
	}

	m_dwReturnValue = NET_DVR_GetDeviceAbility(m_lServerID, m_dwAbilityType, m_strInputParam.GetBuffer(0), m_strInputParam.GetLength(), m_pOutBuf, XML_ABILITY_OUT_LEN);
	m_strInputParam.ReleaseBuffer();
	m_dwLastError = NET_DVR_GetLastError();
	if (m_dwReturnValue != 0)
	{
		CXmlBase xmlBase;
		xmlBase.Parse(m_pOutBuf);
		xmlBase.SetRoot();
		m_strDevAbility = xmlBase.GetChildren().c_str();
		//	m_strDevAbility = m_pOutBuf;
		m_strDevAbility.Replace("\n", "\r\n");
		ParseAbility();
	}
	
	UpdateData(FALSE);
}

void CDlgAbilityAutoDetect::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pOutBuf != NULL)
	{
		delete []m_pOutBuf;
		m_pOutBuf = NULL;
	}
}

BOOL CDlgAbilityAutoDetect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_pOutBuf = new char[XML_ABILITY_OUT_LEN];
	memset(m_pOutBuf, 0, XML_ABILITY_OUT_LEN);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAbilityAutoDetect::OnSelchangeCmbAbilityType() 
{
	// TODO: Add your control notification handler code here
	CXmlBase xmlInput;

	switch (m_cmbAbilityType.GetCurSel())
	{
	case 0:	//软硬件能力
		m_dwAbilityType = DEVICE_SOFTHARDWARE_ABILITY;
		break;
	case 1:	//Wifi能力
		m_dwAbilityType = DEVICE_NETWORK_ABILITY;
		break;
	case 2: //编码能力
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY;
		break;
	case 3:	//当前编码能力
		m_dwAbilityType = DEVICE_ENCODE_CURRENT;
		xmlInput.Parse("<CurrentCompressInfo><ChannelNumber>1</ChannelNumber><VideoEncodeType>0</VideoEncodeType><VideoResolution>3</VideoResolution></CurrentCompressInfo>");
		break;
	case 4:	//前端参数能力
		m_dwAbilityType = IPC_FRONT_PARAMETER;
		break;
	case 5:	//鱼眼能力
		m_dwAbilityType = FISHEYE_ABILITY;
		break;
	case 6:	//Raid能力
		m_dwAbilityType = DEVICE_RAID_ABILITY;
		break;
	case 7: //编码能力2.0
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY_V20;
		xmlInput.Parse("<AudioVideoCompressInfo><AudioChannelNumber>1</AudioChannelNumber><VoiceTalkChannelNumber>1</VoiceTalkChannelNumber><VideoChannelNumber>1</VideoChannelNumber></AudioVideoCompressInfo>");
		break;
	case 8: //前段参数2.0
		m_dwAbilityType = IPC_FRONT_PARAMETER_V20;
		xmlInput.Parse("<CAMERAPARA><ChannelNumber>1</ChannelNumber></CAMERAPARA>");
		break;
	case 9: //解码器能力
		m_dwAbilityType = DECODER_ABILITY;
		xmlInput.Parse("<DecoderAbility version='2.0'></DecoderAbility>");
		break;
	case 10: //用户管理能力
		m_dwAbilityType = DEVICE_USER_ABILITY;
		xmlInput.Parse("<UserAbility version='2.0'></UserAbility>");
		break;
	case 11: //网络应用能力
		m_dwAbilityType = DEVICE_NETAPP_ABILITY;
		xmlInput.Parse("<NetAppAbility version='2.0'></NetAppAbility>");
		break;
	case 12: //视频图像能力
		m_dwAbilityType = DEVICE_VIDEOPIC_ABILITY;
		xmlInput.Parse("<VideoPicAbility version='2.0'>	<channelNO>1</channelNO></VideoPicAbility>");
		break;
	case 13: //JPEG抓图能力
		m_dwAbilityType = DEVICE_JPEG_CAP_ABILITY;
		xmlInput.Parse("<JpegCaptureAbility version='2.0'><channelNO>1</channelNO></JpegCaptureAbility>");
		break;
	case 14: //串口能力
		m_dwAbilityType = DEVICE_SERIAL_ABILITY;
		xmlInput.Parse("<SerialAbility version='2.0'></SerialAbility>");
		break;
	case 15: //视频综合平台能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 16: //解码子系统能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 17: //编码子系统能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 18: //级联输入子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 19: //级联输出子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 20: //码分子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 21: //网络报警主机
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<AlarmHostAbility version=\"2.0\"></AlarmHostAbility>");
		break;
	case 22: //平台级联子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 23: //录像参数能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<RecordAbility version='2.0'></RecordAbility>");
		break;
	case 24: //设备数字通道能力
		m_dwAbilityType = DEVICE_DYNCHAN_ABILITY;
		xmlInput.Parse("<DynChannelAbility version='2.0'><channelNO>1</channelNO></DynChanAbility>");
		break;
	case 25: //通道输入能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<ChannelInputAbility version='2.0'><channelNO>1</channelNO></ChannelInputAbility>");
		break;
	case 26: //审讯机能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<InquestAbility version='2.0'></InquestAbility>");
		break;
	case 27: //CVR能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CVRAbility version='2.0'></CVRAbility>");
		break;
	case 28: //设备事件能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<EventAbility version='2.0'><channelNO>1</channelNO></EventAbility>");
		break;
	case 29: //前端接入通道能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<GetAccessDeviceChannelAbility version='2.0'></GetAccessDeviceChannelAbility>");
		break;
	case 30: //本地预览切换能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<PreviewSwitchAbility version='2.0'></PreviewSwitchAbility>");
		break;
	case 31: //ROI能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<ROIAbility version='2.0'><channelNO>1</channelNO></ROIAbility>");
		break;
	case 32: //云台能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<PTZAbility><channelNO>1</channelNO></PTZAbility>");
		break;
	case 33: //VQD能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VQDAbility version='2.0'><channelNO>1</channelNO></VQDAbility>");
		break;
	case 34://智能交通能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<ITDeviceAbility version='2.0'><channelNO>1</channelNO></ITDeviceAbility>");
		break;
	case 35: //N+1能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<NPlusOneAbility  version='2.0'></NPlusOneAbility >");
		break;
	case 36://磁盘相关能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<HardDiskAbility version='2.0'></HardDiskAbility>");
		break;
	case 37://设备报警能力
		m_dwAbilityType = DEVICE_ALARM_ABILITY;
		xmlInput.Parse("<AlarmAbility version='2.0'><channelID>1</channelID></AlarmAbility>");
		break;
	case 38://IPC配置文件导入导出能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<IPAccessConfigFileAbility version='2.0'></IPAccessConfigFileAbility>");
		break;
	case 39://前端动态能力获取（曝光时间）
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><ExposureSetDynamicLinkTo><WDR><WDREnable>0</WDREnable></WDR><IrisMode><IrisType>0</IrisType></IrisMode></ExposureSetDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 40://前端动态能力获取（CaptureMode）
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><AudioVideoCompressInfoDynamicLinkTo><captureMode opt='640*480@30fps'/></AudioVideoCompressInfoDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 41://GBT28181协议
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<GBT28181AccessAbility version='2.0'><channelNO>1</channelNO></GBT28181AccessAbility>");
		break;
	case 42://日志搜索能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<SearchLogAbility version='2.0'><channelNO>1</channelNO></SearchLogAbility>");
		break;
	case 43://报警触发录像
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<AlarmTriggerRecordAbility version='2.0'><channelNO>1</channelNO></AlarmTriggerRecordAbility>");
		break;
	case 44://IP可视对讲分机能力
		m_dwAbilityType = IP_VIEW_DEV_ABILITY;
		break;
    case 45://矩阵能力
        m_dwAbilityType = MATRIX_ABILITY; 
        xmlInput.Parse("<MatrixAbility version='2.0'></MatrixAbility>"); 
        break; 
	case 46:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<IOAbility version='2.0'><channelNO>1</channelNO></IOAbility>");
		break;
	case 47:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<AccessProtocolAbility version='2.0'><channelNO>1</channelNO></AccessProtocolAbility>");
		break;
	case 48:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VcaDevAbility version='2.0'></VcaDevAbility>");
		break;
	case 49:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VcaCtrlAbility  version='2.0'><channelNO>1</channelNO></VcaCtrlAbility>");
		break;
	case 50:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<VcaChanAbility  version='2.0'><channelNO>1</channelNO></VcaChanAbility>");
		break;
	case 51:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		xmlInput.Parse("<CameraMountAbility  version='2.0'><channelNO>1</channelNO></CameraMountAbility>");
		break;
default:
		m_dwAbilityType = 0;
		break;
	}
	xmlInput.SetRoot();
	m_strInputParam = xmlInput.GetChildren().c_str();
	m_strInputParam.Replace("\n", "\r\n");
	UpdateData(FALSE);
}

void CDlgAbilityAutoDetect::ParseAbility()
{
	CXmlBase xmlAll;
	char szPath[MAX_PATH] = {0};
	GetCurrentPath(szPath);
	sprintf(szPath, "%s\\SDK_ABILITY.xml", szPath);
	if(!xmlAll.LoadFile(szPath))
	{
		GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("本地文件加载错误");
		return;
	}
	xmlAll.SetRoot();
	xmlAll.IntoElem();

	switch (m_cmbAbilityType.GetCurSel())
	{
	case 0:	//软硬件能力
		m_dwAbilityType = DEVICE_SOFTHARDWARE_ABILITY;
		ParseSimpleAbility(xmlAll, "BasicCapability");
		break;
	case 1:	//Wifi能力
		m_dwAbilityType = DEVICE_NETWORK_ABILITY;
		ParseSimpleAbility(xmlAll, "NetworkSetting");
		break;
	case 2: //编码能力
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY;
		break;
	case 3:	//当前编码能力
		m_dwAbilityType = DEVICE_ENCODE_CURRENT;
		break;
	case 4:	//前端参数能力
		m_dwAbilityType = IPC_FRONT_PARAMETER;
		break;
	case 5:	//鱼眼能力
		m_dwAbilityType = FISHEYE_ABILITY;
		break;
	case 6:	//Raid能力
		m_dwAbilityType = DEVICE_RAID_ABILITY;
		ParseSimpleAbility(xmlAll, "RAID");
		break;
	case 7: //编码能力2.0
		m_dwAbilityType = DEVICE_ENCODE_ALL_ABILITY_V20;
		ParseSimpleAbility(xmlAll, "AudioVideoCompressInfo");
		break;
	case 8: //前段参数2.0
		m_dwAbilityType = IPC_FRONT_PARAMETER_V20;
		ParseSimpleAbility(xmlAll, "CAMERAPARA");
		break;
	case 9: //解码器能力
		m_dwAbilityType = DECODER_ABILITY;
		ParseSimpleAbility(xmlAll, "DecoderAbility");
		break;
	case 10: //用户管理能力
		m_dwAbilityType = DEVICE_USER_ABILITY;
		ParseSimpleAbility(xmlAll, "UserAbility");
		break;
	case 11: //网络应用能力
		m_dwAbilityType = DEVICE_NETAPP_ABILITY;
		ParseSimpleAbility(xmlAll, "NetAppAbility");
		break;
	case 12: //视频图像能力
		m_dwAbilityType = DEVICE_VIDEOPIC_ABILITY;
		ParseSimpleAbility(xmlAll, "VideoPicAbility");
		break;
	case 13: //JPEG抓图能力
		m_dwAbilityType = DEVICE_JPEG_CAP_ABILITY;
		ParseSimpleAbility(xmlAll, "JpegCaptureAbility");
		break;
	case 14: //串口能力
		m_dwAbilityType = DEVICE_SERIAL_ABILITY;
		ParseSimpleAbility(xmlAll, "SerialAbility");
		break;
	case 15: //视频综合平台能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 16: //解码子系统能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 17: //编码子系统能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 18: //级联输入子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 19: //级联输出子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 20: //码分子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 21: //网络报警主机
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "AlarmHostAbility");
		break;
	case 22: //平台级联子系统
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		break;
	case 23:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "RecordAbility");
		break;
	case 24:
		m_dwAbilityType = DEVICE_DYNCHAN_ABILITY;
		ParseSimpleAbility(xmlAll, "DynChannelAbility");
		break;
	case 25:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ChannelInputAbility");
		break;
	case 26: //审讯机能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "InquestAbility");
		break;
	case 27: //CVR能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "CVRAbility");
		break;
	case 28: //设备事件能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "EventAbility");
		break;
	case 29: //前端接入通道能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "GetAccessDeviceChannelAbility");
		break;
	case 30: //本地预览切换能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "PreviewSwitchAbility ");
		break;
	case 31: //ROI能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ROIAbility");
		break;
	case 32: //云台能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "PTZAbility");
		break;
	case 33: //VQD能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VQDAbility");
		break;
	case 34://智能交通
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ITDeviceAbility");
		break;
	case 35: //N+1能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "NPlusOneAbility");
		break;
	case 36://磁盘相关能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "HardDiskAbility");
		break;
	case 37://设备报警能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "DeviceAlarmAbility");
		break;
	case 38://IPC配置文件导入导出能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "IPAccessConfigFileAbility");
		break;
	case 39://前端动态能力获取（曝光时间）
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "ExposureSetDynamicAbility");
		//xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><ExposureSetDynamicLinkTo><WDR><WDREnable>0</WDREnable></WRD><IrisMode><IrisType>0</IrisType></IrisMode></ExposureSetDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 40://前端动态能力获取（CaptureMode）
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "CaptureModeDynamicAbility");
		//xmlInput.Parse("<CameraParaDynamicAbility version='2.0'><channelNO>1</channelNO><AudioVideoCompressInfoDynamicLinkTo><captureMode opt='640*480@30fps'/></AudioVideoCompressInfoDynamicLinkTo></CameraParaDynamicAbility>");
		break;
	case 41://GBT28181协议
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "GBT28181AccessAbility");
		//xmlInput.Parse("<GBT28181AccessAbility version='2.0'><channelNO>1</channelNO></GBT28181AccessAbility>");
		break;
	case 42://日志搜索能力
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "SearchLogAbility");
		//xmlInput.Parse("<SearchLogAbility version='2.0'><channelNO>1</channelNO></SearchLogAbility>");
		break;
	case 43://报警触发录像
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "AlarmTriggerRecordAbility");
		//xmlInput.Parse("<AlarmTriggerRecordAbility version='2.0'><channelNO>1</channelNO></AlarmTriggerRecordAbility>");
		break;
	case 44:
		m_dwAbilityType = IP_VIEW_DEV_ABILITY;
		ParseSimpleAbility(xmlAll, "IpViewDevAbility");
		break;
	case 48:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VcaDevAbility");
		break;
	case 49:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VcaCtrlAbility");
		break;
	case 50:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "VcaChanAbility");
		break;
	case 51:
		m_dwAbilityType = DEVICE_ABILITY_INFO;
		ParseSimpleAbility(xmlAll, "CameraMountAbility");
		break;
	default:
		m_dwAbilityType = 0;
		break;
	}
}

void CDlgAbilityAutoDetect::ParseSimpleAbility(CXmlBase &xmlAll, char* szAbilityName)
{	
	CXmlBase xmlDev;
	
	if (!xmlAll.FindElem(szAbilityName))
	{
		GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("本地文件中没有当前能力");
		return ;
	}
	xmlAll.Parse(xmlAll.GetChildren().c_str());
	xmlAll.SetRoot();
	
	xmlDev.Parse(m_pOutBuf);
	if (!xmlDev.FindElem(szAbilityName))
	{
		GetDlgItem(IDC_EDT_ABILITY_LOST)->SetWindowText("数据格式错误,无法分析");
		return ;
	}

	ParseAbilityRecursive(xmlAll, xmlDev);

	UpdateData(FALSE);
}

void CDlgAbilityAutoDetect::ParseAbilityRecursive(CXmlBase &xmlAll, CXmlBase &xmlDev)
{
 	string strNode = xmlAll.GetNodeName();
 
 	if (!xmlDev.FindElem(strNode.c_str()))
 	{
 		m_strLostAbility += "LOST:";
		m_strLostAbility += strNode.c_str();
 		m_strLostAbility += "\r\n";
		if (xmlAll.NextSibElem())
		{
			ParseAbilityRecursive(xmlAll, xmlDev);
		}
		return;
 	}
 	
 	if (xmlAll.IntoElem())
	{
		if(xmlDev.IntoElem())
		{
			ParseAbilityRecursive(xmlAll, xmlDev);
			xmlDev.OutOfElem();
		}
		else
		{
			if (xmlDev.GetNodeName() == strNode && xmlDev.GetData() == "" && xmlDev.GetFirstAttributeValue() == "")
			{
				m_strLostAbility += "NULL:";
				m_strLostAbility += xmlDev.GetNodeName().c_str();
				m_strLostAbility += "\r\n";
			}			
			ParseAbilityRecursive(xmlAll, xmlDev);
		}
		xmlAll.OutOfElem();
	}
	else if (xmlDev.GetNodeName() == strNode && xmlDev.GetData() == "" && xmlDev.GetFirstAttributeValue() == "")
	{
		m_strLostAbility += "NULL:";
		m_strLostAbility += xmlDev.GetNodeName().c_str();
		m_strLostAbility += "\r\n";
	}

	if (xmlAll.NextSibElem())
	{
		if (!xmlDev.NextSibElem())	//加这个判断是为了处理相同名称节点存在多个的情况
		{
			do 
			{
				m_strLostAbility += "LOST:";
				m_strLostAbility += xmlAll.GetNodeName().c_str();
 				m_strLostAbility += "\r\n";
			} while (xmlAll.NextSibElem());
			return;
		}
		ParseAbilityRecursive(xmlAll, xmlDev);
	}
}
