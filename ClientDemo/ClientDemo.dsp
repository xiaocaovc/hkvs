# Microsoft Developer Studio Project File - Name="ClientDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ClientDemo - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ClientDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ClientDemo.mak" CFG="ClientDemo - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ClientDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ClientDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ClientDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseVC60"
# PROP Intermediate_Dir "ReleaseVC60"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O1 /I "../../头文件/" /I "./" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "DEMO_LAN_CN" /FAcs /Fa"VC60/" /FR"VC60/" /Fo"VC60/" /Fd"VC60/" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "../../头文件/" /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /fo"Release/ClientDemo.res" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release/ClientDemo.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 HCNetSDK.lib PlayCtrl.lib ws2_32.lib DsSdk.lib winmm.lib GdiPlus.lib IPHlpApi.Lib /nologo /subsystem:windows /pdb:"../../库文件/ClientDemo.pdb" /machine:I386 /out:"../../库文件/ClientDemo.exe" /libpath:"../../库文件/"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ClientDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../头文件/" /I "./" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "DEMO_LAN_CN" /FR"DVC60/" /Fp"DVC60/ClientDemo.pch" /YX /Fo"DVC60/" /Fd"DVC60/" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "../../头文件/" /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GdiPlus.lib HCNetSDK.lib PlayCtrl.lib ws2_32.lib winmm.lib IPHlpApi.Lib /nologo /subsystem:windows /pdb:"../../库文件/ClientDemo.pdb" /debug /machine:I386 /out:"../../库文件/ClientDemo.exe" /libpath:"../../库文件/"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ClientDemo - Win32 Release"
# Name "ClientDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ClientDemo.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\ClientDemo.rc
# End Source File
# Begin Source File

SOURCE=.\ClientDemoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DecoderCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogMultiNetCard.cpp
# End Source File
# Begin Source File

SOURCE=.\DialogRecordTimeSpan.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg67DVSCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAbilityAutoDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAccessCameraCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAddDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAES.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmCamCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmExceptionHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHost.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostAudioAssociateAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostDataUploadMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostDialParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostEnableCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostEventTrigger.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostExternalDevLimitValue.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostExternalDevState.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostExternalModule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostFaultProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostGPRS.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostHistoryData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostLED.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostLEDBrightness.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostLogSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostModuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostNetCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostPointCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostPrinter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostReportMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostRs485.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostSensorLinkage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostSubsystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostTransparent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostTriggerCode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostZoomAddr.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmKeyboardUser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmNetUser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmOperatorUser.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmOutCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAtmFrameFormatV30.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAudioUploadDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAudioUploadDownload.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoReboot.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBatchConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBathcUserRightCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBroadCast.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCameraSetupCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCcdParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCertInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChanInputAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgChannelInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCheckTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCMSParam.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCodeSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCompressionV30.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgConfigAll.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgConfigFlash.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCreateVD.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCuntomProtocal.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCurrentEncodeAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCVR.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDecoderAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDecWinStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceAudioTalkCode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceLanguageEncode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceState.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDevServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDevWorkMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDiagnosticServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDiskQuota.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDoTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDynChanAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEctChanStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEctWorkState.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEncodeAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEncodeJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgESataMiniSasUsage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEthernetIpv6Cfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgEZVIZAccessCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFishEye.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyeCorrect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyeFECWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFishEyePtzControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyePtzPlayWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyeTrackCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFormatHD.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFortify.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGatewayCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGBT28181ProtocolAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGetAccessDeviceChannelInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGetDigitalChannelStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgGopInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHardDiskCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHeatMapCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgHolidayCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImExportIPCCfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfrareCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInquestDvr.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInquestEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInquestFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIOOutCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgDDNS.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgEasyDDNS.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgIPServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgStreamServer.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPAddrFilterCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPCFrontAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPCSimpIntellCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPCSpecial.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPCSpeclalEx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIPDomeLocalCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIpViewCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgIscsiCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgISPParamCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITCAblity.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSBaseInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSImageMergeCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSIOinCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSIpcCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSOverlapCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSRoadInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSUpload.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgITSWorkState.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgJpegCaptureCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgJpegFTP.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgJPEGSnatchConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLCDScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLCDScreenDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLEDTimerSwitch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLimitAngle.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLinkStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLocalConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLocalLogConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLocalRecordSchedule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLogSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMatDecBigScrSplice.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMatDecodeChanJoint.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMatUnitedSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMonitorInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetraEventVedio.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetraHttps.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetraRecordPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetraSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetServiceCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetworkAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetworkBonding.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNetworkFlowTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNVRCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOutputCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOutputScheduleRuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPassiveTransCode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPassiveWin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPdcCalibration.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPdcResult.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPdcRuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPdcRuleCfgEx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPDCSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayBack.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayDvrRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayLocalFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayLockFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayMotion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayRemoteFile.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPlayRemoteTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosChanFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosConnectMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosFilterEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosFilterRule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPositionTrackCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosManage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosRecvData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPOSRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPosSerialPortRecvData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPPPCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPreviewSwitchCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPreviewWin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProductAlarmHostCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProductCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProductionTest.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProductVcaCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPtzCfgCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPTZCruise.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPtzCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPtzParkActionCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPTZPreset.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPtzScope.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPTZTasks.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPtzTrackParamCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPUStream.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgQuickAddIpc.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRaidConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecordAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecordLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRecordStreamTransType.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRegionList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteAdvanceNetConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteAlarmDetectFace.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteAlarmInHandle.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteAlarmInPtzInvoke.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteControlAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteEmailAddReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteFanException.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteHideAlarm.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteHideArea.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteIPAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteKey.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteMotionDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteNetNFS.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemotePullException.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteRecordSchedule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteShowString.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteStreamRecordSchedule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteVideoInLost.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteVideoInput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteZone.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRoiCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRtspConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSafetyCabin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSceneTimeCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgScreenLogoCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSDIMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSenceControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSensorAddPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSensorCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSerialTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSignalLightSync.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSizeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSnmpCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSoftHardAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSplitterStartTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSplitterTransparent.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStreamAttachInfoCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStreamID.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStreamManualRecordAndLock.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStreamRecordInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStreamRecordStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgStreamSrcCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSubIPCCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSubITSCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSubsysNetCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgT1test.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTalkMREx.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTPSRuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTransAutoReboot.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTransCompression.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTransConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTransDevStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTransStreamSrcCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTsSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUpgrade.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUpgradeIPC.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUploadLogo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUpnpCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaAidRuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaAlarmTimeCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaAuxAreaList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBaselineScene.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBehaviorCalibrate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBehaviorCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBlackList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaChanCtrlCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaChannelWorkMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVCADetionCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDevAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDevAccessCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDevChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDrawMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaEnterRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFace.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceDataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceDetect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceMatchLogSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceSnapCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFindBackgroundPic.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFindSnapData.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaForensicsModeCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFunCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaITCTriggerCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaITSCalibrate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaITSCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaIvmsCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaIvmsSched.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaLaneCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaLF.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaMaskRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaObjectColor.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaParamkey.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaPositionRule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaRemoteChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaResetCounterCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaRuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSavePathConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneAidRuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneITSCalibrate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneLaneCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneRegionList.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneTpsRuleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneTrafficCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSingleProcImg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSnapDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaTrackCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaVQDAlarmPic.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaVQDEventRule.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVcaWorkStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVedioAudioInCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVehicleCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVehicleControlCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVehicleMvaCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVideoOut.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVoiceTalk.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVQDCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVqdPlan.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWall.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWallCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWallWin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWD1Enable.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWifi.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWinPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlAlarmAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlDecoderAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlEventAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlItcAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlJpegCapAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlLCDAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlNetAppAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlRaidAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlSerialAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlVideoPic.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgXmlWallAblity.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgZeroChanCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawFun.cpp
# End Source File
# Begin Source File

SOURCE=.\JPEGPicDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\MatCodeSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\MatCycLink.cpp
# End Source File
# Begin Source File

SOURCE=.\MatDecDnsCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\MatDecoderStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\MatDispChan.cpp
# End Source File
# Begin Source File

SOURCE=.\MatDispChanControl.cpp
# End Source File
# Begin Source File

SOURCE=.\MatDynamicTest.cpp
# End Source File
# Begin Source File

SOURCE=.\MatPassive.cpp
# End Source File
# Begin Source File

SOURCE=.\MatPlayBack.cpp
# End Source File
# Begin Source File

SOURCE=.\MatTrans.cpp
# End Source File
# Begin Source File

SOURCE=.\MatVedioPlatform.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\PTZButton.cpp
# End Source File
# Begin Source File

SOURCE=.\PTZButton1.cpp
# End Source File
# Begin Source File

SOURCE=.\PtzControl.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundIn.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgAdapterCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgAlarmCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgArrayCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgATMInfoCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgBTS.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgChanCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgCVRCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgDeviceCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgDVRNVRCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgExceptionCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgMatDecCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgNetworkCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgPDCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgPreviewCfgV30.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgSerialCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgUserCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgVDCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\SubDlgWndSequence.cpp
# End Source File
# Begin Source File

SOURCE=.\UserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VcaCommonOperations.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveSound.cpp
# End Source File
# Begin Source File

SOURCE=.\XmlUserAbility.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ClientDemo.h
# End Source File
# Begin Source File

SOURCE=.\ClientDemoDlg.h
# End Source File
# Begin Source File

SOURCE=.\DecoderCfg.h
# End Source File
# Begin Source File

SOURCE=.\DeviceDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialogMultiNetCard.h
# End Source File
# Begin Source File

SOURCE=.\DialogRecordTimeSpan.h
# End Source File
# Begin Source File

SOURCE=.\Dlg67DVSCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAbilityAutoDetect.h
# End Source File
# Begin Source File

SOURCE=.\DlgAccessCameraCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAddDevice.h
# End Source File
# Begin Source File

SOURCE=.\DlgAES.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmCamCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmExceptionHandle.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHost.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostAudioAssociateAlarm.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostDataUploadMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostDialParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostEnableCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostEventTrigger.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostExternalDevLimitValue.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostExternalDevState.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostExternalModule.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostFaultProcess.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostGPRS.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostHistoryData.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostLED.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostLEDBrightness.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostLogSearch.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostModuleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostNetCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostPointCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostPrinter.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostReportMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostRs485.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostSensorLinkage.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmhostSubsystem.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostTransparent.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostTriggerCode.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmHostZoomAddr.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmKeyboardUser.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmNetUser.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmOperatorUser.h
# End Source File
# Begin Source File

SOURCE=.\DlgAlarmOutCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAtmFrameFormatV30.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoReboot.h
# End Source File
# Begin Source File

SOURCE=.\DlgBatchConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgBathcUserRightCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgBroadCast.h
# End Source File
# Begin Source File

SOURCE=.\DlgCameraSetupCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgCcdParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgCertInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgChanInputAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgChannelInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgCheckTime.h
# End Source File
# Begin Source File

SOURCE=.\DlgCMSParam.h
# End Source File
# Begin Source File

SOURCE=.\DlgCodeSplitter.h
# End Source File
# Begin Source File

SOURCE=.\DlgCompressionV30.h
# End Source File
# Begin Source File

SOURCE=.\DlgConfigAll.h
# End Source File
# Begin Source File

SOURCE=.\DlgConfigFlash.h
# End Source File
# Begin Source File

SOURCE=.\DlgCreateVD.h
# End Source File
# Begin Source File

SOURCE=.\DlgCuntomProtocal.h
# End Source File
# Begin Source File

SOURCE=.\DlgCurrentEncodeAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgCVR.h
# End Source File
# Begin Source File

SOURCE=.\DlgDecoderAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgDecWinStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceAudioTalkCode.h
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceLanguageEncode.h
# End Source File
# Begin Source File

SOURCE=.\DlgDeviceState.h
# End Source File
# Begin Source File

SOURCE=.\DlgDevServer.h
# End Source File
# Begin Source File

SOURCE=.\DlgDevWorkMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgDiagnosticServer.h
# End Source File
# Begin Source File

SOURCE=.\DlgDiskQuota.h
# End Source File
# Begin Source File

SOURCE=.\DlgDoTest.h
# End Source File
# Begin Source File

SOURCE=.\DlgDynChanAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgEctChanStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgEctWorkState.h
# End Source File
# Begin Source File

SOURCE=.\DlgEncodeAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgEncodeJoint.h
# End Source File
# Begin Source File

SOURCE=.\DlgESataMiniSasUsage.h
# End Source File
# Begin Source File

SOURCE=.\DlgEthernetIpv6Cfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgEZVIZAccessCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgFishEye.h
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyeCorrect.h
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyeFECWnd.h
# End Source File
# Begin Source File

SOURCE=.\DlgFishEyePtzControl.h
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyePtzPlayWnd.h
# End Source File
# Begin Source File

SOURCE=.\DlgFisheyeTrackCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgFormatHD.h
# End Source File
# Begin Source File

SOURCE=.\DlgFortify.h
# End Source File
# Begin Source File

SOURCE=.\DlgGatewayCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgGBT28181ProtocolAccess.h
# End Source File
# Begin Source File

SOURCE=.\DlgGetAccessDeviceChannelInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgGetDigitalChannelStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgGopInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgHardDiskCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgHeatMapCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgHolidayCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgImExportIPCCfgFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfrareCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgInquestDvr.h
# End Source File
# Begin Source File

SOURCE=.\DlgInquestEvent.h
# End Source File
# Begin Source File

SOURCE=.\DlgInquestFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgIOOutCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgDDNS.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgEasyDDNS.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgIPServer.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPAccessCfgStreamServer.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPAddrFilterCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPCFrontAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPCSimpIntellCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPCSpecial.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPCSpeclalEx.h
# End Source File
# Begin Source File

SOURCE=.\DlgIPDomeLocalCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgIscsiCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgISPParamCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgITCAblity.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSBaseInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSImageMergeCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSIOinCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSIpcCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSOverlapCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSRoadInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSUpload.h
# End Source File
# Begin Source File

SOURCE=.\DlgITSWorkState.h
# End Source File
# Begin Source File

SOURCE=.\DlgJpegCaptureCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgJpegFTP.h
# End Source File
# Begin Source File

SOURCE=.\DlgJPEGSnatchConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgLCDScreen.h
# End Source File
# Begin Source File

SOURCE=.\DlgLCDScreenDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DlgLEDTimerSwitch.h
# End Source File
# Begin Source File

SOURCE=.\DlgLimitAngle.h
# End Source File
# Begin Source File

SOURCE=.\DlgLinkStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgLocalConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgLocalLogConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgLocalRecordSchedule.h
# End Source File
# Begin Source File

SOURCE=.\DlgLogSearch.h
# End Source File
# Begin Source File

SOURCE=.\DlgMatDecBigScrSplice.h
# End Source File
# Begin Source File

SOURCE=.\DlgMatDecodeChanJoint.h
# End Source File
# Begin Source File

SOURCE=.\DlgMatUnitedSystem.h
# End Source File
# Begin Source File

SOURCE=.\DlgMonitorInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetraEventVedio.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetraHttps.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetraRecordPackage.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetraSeries.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetServiceCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetworkAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetworkBonding.h
# End Source File
# Begin Source File

SOURCE=.\DlgNetworkFlowTest.h
# End Source File
# Begin Source File

SOURCE=.\DlgNVRCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgOutput.h
# End Source File
# Begin Source File

SOURCE=.\DlgOutputCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgOutputScheduleRuleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPassiveTransCode.h
# End Source File
# Begin Source File

SOURCE=.\DlgPassiveWin.h
# End Source File
# Begin Source File

SOURCE=.\DlgPdcCalibration.h
# End Source File
# Begin Source File

SOURCE=.\DlgPdcResult.h
# End Source File
# Begin Source File

SOURCE=.\DlgPdcRuleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPdcRuleCfgEx.h
# End Source File
# Begin Source File

SOURCE=.\DlgPDCSearch.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayBack.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayConvert.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayDvrRecord.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayLocalFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayLockFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayMotion.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayRemoteFile.h
# End Source File
# Begin Source File

SOURCE=.\DlgPlayRemoteTime.h
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPosChanFilter.h"
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPosConnectMode.h"
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPosFilterEffect.h"
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPosFilterRule.h"
# End Source File
# Begin Source File

SOURCE=.\DlgPositionTrackCfg.h
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPosManage.h"
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPosRecvData.h"
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPOSRegion.h"
# End Source File
# Begin Source File

SOURCE="E:\brunch\美分7204ST\代码\code\win32\ClientDemo\DlgPosSerialPortRecvData.h"
# End Source File
# Begin Source File

SOURCE=.\DlgPPPCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPreviewSwitchCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPreviewWin.h
# End Source File
# Begin Source File

SOURCE=.\DlgProductAlarmHostCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgProductCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgProductionTest.h
# End Source File
# Begin Source File

SOURCE=.\DlgProductVcaCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPtzCfgCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgPTZCruise.h
# End Source File
# Begin Source File

SOURCE=.\DlgPtzCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgPtzParkActionCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPTZPreset.h
# End Source File
# Begin Source File

SOURCE=.\DlgPtzScope.h
# End Source File
# Begin Source File

SOURCE=.\DlgPTZTasks.h
# End Source File
# Begin Source File

SOURCE=.\DlgPtzTrackParamCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgPUStream.h
# End Source File
# Begin Source File

SOURCE=.\DlgQuickAddIpc.h
# End Source File
# Begin Source File

SOURCE=.\DlgRaidConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecordAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecordLabel.h
# End Source File
# Begin Source File

SOURCE=.\DlgRecordStreamTransType.h
# End Source File
# Begin Source File

SOURCE=.\DlgRegionList.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteAlarmDetectFace.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteAlarmInHandle.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteAlarmInPtzInvoke.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteControlAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteEmailAddReceiver.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteFanException.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteHideAlarm.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteHideArea.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteIPAccess.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteKey.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteMotionDetect.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteNetNFS.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemotePullException.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteRecordSchedule.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteShowString.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteStreamRecordSchedule.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteVideoInLost.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteVideoInput.h
# End Source File
# Begin Source File

SOURCE=.\DlgRemoteZone.h
# End Source File
# Begin Source File

SOURCE=.\DlgRoiCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgRtspConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgSafetyCabin.h
# End Source File
# Begin Source File

SOURCE=.\DlgSceneTimeCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgScreenLogoCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSDIMatrix.h
# End Source File
# Begin Source File

SOURCE=.\DlgSenceControl.h
# End Source File
# Begin Source File

SOURCE=.\DlgSensorAddPreview.h
# End Source File
# Begin Source File

SOURCE=.\DlgSensorCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSerialTrans.h
# End Source File
# Begin Source File

SOURCE=.\DlgSignalLightSync.h
# End Source File
# Begin Source File

SOURCE=.\DlgSizeFilter.h
# End Source File
# Begin Source File

SOURCE=.\DlgSnmpCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSoftHardAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgSplitterStartTrans.h
# End Source File
# Begin Source File

SOURCE=.\DlgSplitterTransparent.h
# End Source File
# Begin Source File

SOURCE=.\DlgStreamAttachInfoCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgStreamID.h
# End Source File
# Begin Source File

SOURCE=.\DlgStreamManualRecordAndLock.h
# End Source File
# Begin Source File

SOURCE=.\DlgStreamRecordInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgStreamRecordStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgStreamSrcCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSubIPCCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSubITSCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgSubsysNetCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgT1test.h
# End Source File
# Begin Source File

SOURCE=.\DlgTalkMREx.h
# End Source File
# Begin Source File

SOURCE=.\DlgTPSRuleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgTransAutoReboot.h
# End Source File
# Begin Source File

SOURCE=.\DlgTransCompression.h
# End Source File
# Begin Source File

SOURCE=.\DlgTransConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgTransDevStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgTransStreamSrcCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgTsSeries.h
# End Source File
# Begin Source File

SOURCE=.\DlgUpgrade.h
# End Source File
# Begin Source File

SOURCE=.\DlgUpgradeIPC.h
# End Source File
# Begin Source File

SOURCE=.\DlgUploadLogo.h
# End Source File
# Begin Source File

SOURCE=.\DlgUpnpCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaAidRuleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaAlarmTimeCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaAuxAreaList.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBaselineScene.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBehaviorCalibrate.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBehaviorCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaBlackList.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaChanCtrlCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaChannelWorkMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDataBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgVCADetionCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDevAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDevAccessCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDevChannel.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaDrawMode.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaEnterRegion.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFace.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceDataBase.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceDetect.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceMatchLogSearch.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFaceSnapCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFindBackgroundPic.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFindSnapData.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaForensicsModeCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaFunCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaITCTriggerCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaITSCalibrate.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaITSCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaIvmsCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaIvmsSched.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaLaneCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaLF.h
# End Source File
# Begin Source File

SOURCE=.\DlgvcaMaskRegion.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaObjectColor.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaParamkey.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaPositionRule.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaRemoteChannel.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaResetCounterCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaRuleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSavePathConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneITSCalibrate.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneLaneCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneRegionList.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSceneTpsRuleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSingleProcImg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaSnapDatabase.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaTrackCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaVQDAlarmPic.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaVQDEventRule.h
# End Source File
# Begin Source File

SOURCE=.\DlgVcaWorkStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgVedioAudioInCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVehicleCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVehicleControlCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVehicleMvaCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVideoOut.h
# End Source File
# Begin Source File

SOURCE=.\DlgVoiceTalk.h
# End Source File
# Begin Source File

SOURCE=.\DlgVQDCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgVqdPlan.h
# End Source File
# Begin Source File

SOURCE=.\DlgWall.h
# End Source File
# Begin Source File

SOURCE=.\DlgWallCfg.h
# End Source File
# Begin Source File

SOURCE=.\DlgWallWin.h
# End Source File
# Begin Source File

SOURCE=.\DlgWD1Enable.h
# End Source File
# Begin Source File

SOURCE=.\DlgWifi.h
# End Source File
# Begin Source File

SOURCE=.\DlgWinPreview.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlAlarmAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlDecoderAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlEventAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlItcAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlJpegCapAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlLCDAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlNetAppAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlRaidAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlSerialAbility.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlVideoPic.h
# End Source File
# Begin Source File

SOURCE=.\DlgXmlWallAblity.h
# End Source File
# Begin Source File

SOURCE=.\DlgZeroChanCfg.h
# End Source File
# Begin Source File

SOURCE=.\DrawFun.h
# End Source File
# Begin Source File

SOURCE=.\GeneralDef.h
# End Source File
# Begin Source File

SOURCE=..\..\src\export\header\InterfaceBase.h
# End Source File
# Begin Source File

SOURCE=.\IPExport.h
# End Source File
# Begin Source File

SOURCE=.\IPHlpApi.h
# End Source File
# Begin Source File

SOURCE=.\IPTypes.h
# End Source File
# Begin Source File

SOURCE=.\JPEGPicDownload.h
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# Begin Source File

SOURCE=.\MatCodeSplitter.h
# End Source File
# Begin Source File

SOURCE=.\MatCycLink.h
# End Source File
# Begin Source File

SOURCE=.\MatDecDnsCfg.h
# End Source File
# Begin Source File

SOURCE=.\MatDecoderStatus.h
# End Source File
# Begin Source File

SOURCE=.\MatDispChan.h
# End Source File
# Begin Source File

SOURCE=.\MatDispChanControl.h
# End Source File
# Begin Source File

SOURCE=.\MatDynamicTest.h
# End Source File
# Begin Source File

SOURCE=.\MatPassive.h
# End Source File
# Begin Source File

SOURCE=.\MatPlayBack.h
# End Source File
# Begin Source File

SOURCE=.\MatTrans.h
# End Source File
# Begin Source File

SOURCE=.\MatVedioPlatform.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\PTZButton.h
# End Source File
# Begin Source File

SOURCE=.\PTZButton1.h
# End Source File
# Begin Source File

SOURCE=.\PtzControl.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SoundIn.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgAdapterCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgAlarmCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgArrayCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgATMInfoCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgBTS.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgChanCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgCVRCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgDeviceCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgDVRNVRCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgExceptionCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgMatDeccfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgNetworkCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgPDCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgPreviewCfgV30.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgSerialCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgUserCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgVDCfg.h
# End Source File
# Begin Source File

SOURCE=.\SubDlgWndSequence.h
# End Source File
# Begin Source File

SOURCE=.\UserDlg.h
# End Source File
# Begin Source File

SOURCE=.\VcaCommonOperations.h
# End Source File
# Begin Source File

SOURCE=.\WaveSound.h
# End Source File
# Begin Source File

SOURCE=.\XmlUserAbility.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\audio.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\camera.bmp
# End Source File
# Begin Source File

SOURCE=.\res\camera.ico
# End Source File
# Begin Source File

SOURCE=.\res\ClientDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\ClientDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\dev_alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fortify.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fortify_alarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon8.ico
# End Source File
# Begin Source File

SOURCE=.\res\IPChan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\login.bmp
# End Source File
# Begin Source File

SOURCE=.\res\logout.bmp
# End Source File
# Begin Source File

SOURCE=.\res\p_r_a.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PAUSE_ENABLE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\play.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PLAY_DISABLE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\PLAY_ENABLE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\playAndAlarm.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rec.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rec_play.bmp
# End Source File
# Begin Source File

SOURCE=.\res\step_disable.ico
# End Source File
# Begin Source File

SOURCE=.\res\STOP.ICO
# End Source File
# Begin Source File

SOURCE=.\res\STOP_DISABLE.ICO
# End Source File
# Begin Source File

SOURCE=.\res\tree.bmp
# End Source File
# End Group
# Begin Group "includeCn"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\incCn\DataType.h
# End Source File
# Begin Source File

SOURCE=..\incCn\DecodeCardSdk.h
# End Source File
# Begin Source File

SOURCE=..\incCn\HCNetSDK.h
# End Source File
# Begin Source File

SOURCE=..\incCn\plaympeg4.h
# End Source File
# End Group
# Begin Group "xml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xml\tinystr.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\tinystr.h
# End Source File
# Begin Source File

SOURCE=.\xml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=.\xml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\tinyxmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\XmlBase.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\XmlBase.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DlgAlarmExceptionHandle.asp
# End Source File
# Begin Source File

SOURCE=.\Modification.txt
# End Source File
# End Target
# End Project
# Section ClientDemo : {6F074DA6-14D7-4D83-A83D-38AA1D52F1AB}
# 	1:16:IDD_DLG_CCDPARAM:122
# 	2:16:Resource Include:resource.h
# 	2:19:CLASS: CDlgCcdParam:CDlgCcdParam
# 	2:16:IDD_DLG_CCDPARAM:IDD_DLG_CCDPARAM
# 	2:10:ENUM: enum:enum
# 	2:13:DlgCcdParam.h:DlgCcdParam1.h
# 	2:15:DlgCcdParam.cpp:DlgCcdParam1.cpp
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {255AB1D5-5EA3-4B7F-BA48-2955C4A94F9D}
# 	1:37:IDD_DLG_STREAM_MANUAL_RECORD_AND_LOCK:125
# 	2:16:Resource Include:resource.h
# 	2:30:DlgStreamManualRecordAndLock.h:DlgStreamManualRecordAndLock.h
# 	2:37:IDD_DLG_STREAM_MANUAL_RECORD_AND_LOCK:IDD_DLG_STREAM_MANUAL_RECORD_AND_LOCK
# 	2:36:CLASS: CDlgStreamManualRecordAndLock:CDlgStreamManualRecordAndLock
# 	2:10:ENUM: enum:enum
# 	2:32:DlgStreamManualRecordAndLock.cpp:DlgStreamManualRecordAndLock.cpp
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {61059D25-AF2F-4EA6-82DB-245B90D34285}
# 	1:24:IDD_DLG_SUBSYSTEM_NETCFG:157
# 	2:16:Resource Include:resource.h
# 	2:23:CLASS: CDlgSubsysNetCfg:CDlgSubsysNetCfg
# 	2:19:DlgSubsysNetCfg.cpp:DlgSubsysNetCfg.cpp
# 	2:10:ENUM: enum:enum
# 	2:17:DlgSubsysNetCfg.h:DlgSubsysNetCfg.h
# 	2:24:IDD_DLG_SUBSYSTEM_NETCFG:IDD_DLG_SUBSYSTEM_NETCFG
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {B6614ED6-E53C-4F7D-ADB2-6A681F10E9CF}
# 	1:11:IDD_DLG_VQD:153
# 	2:16:Resource Include:resource.h
# 	2:11:DlgVQDCfg.h:DlgVQDCfg.h
# 	2:10:ENUM: enum:enum
# 	2:17:CLASS: CDlgVQDCfg:CDlgVQDCfg
# 	2:11:IDD_DLG_VQD:IDD_DLG_VQD
# 	2:19:Application Include:clientdemo.h
# 	2:13:DlgVQDCfg.cpp:DlgVQDCfg.cpp
# End Section
# Section ClientDemo : {899081F1-0F6D-47DB-8E05-A07F279FA049}
# 	1:31:IDD_DLG_REMOTE_RECORD_SCHEDULE1:150
# 	2:16:Resource Include:resource.h
# 	2:33:DlgRemoteStreamRecordSchedule.cpp:DlgRemoteStreamRecordSchedule.cpp
# 	2:31:IDD_DLG_REMOTE_RECORD_SCHEDULE1:IDD_DLG_REMOTE_RECORD_SCHEDULE1
# 	2:10:ENUM: enum:enum
# 	2:37:CLASS: CDlgRemoteStreamRecordSchedule:CDlgRemoteStreamRecordSchedule
# 	2:19:Application Include:clientdemo.h
# 	2:31:DlgRemoteStreamRecordSchedule.h:DlgRemoteStreamRecordSchedule.h
# End Section
# Section ClientDemo : {EE72C470-D6EE-4928-AFC9-2232EEC719FD}
# 	1:25:IDD_DIALOG_PTZ_PARKACTION:152
# 	2:16:Resource Include:resource.h
# 	2:21:DlgPtzParkActionCfg.h:DlgPtzParkActionCfg.h
# 	2:27:CLASS: CDlgPtzParkActionCfg:CDlgPtzParkActionCfg
# 	2:10:ENUM: enum:enum
# 	2:23:DlgPtzParkActionCfg.cpp:DlgPtzParkActionCfg.cpp
# 	2:25:IDD_DIALOG_PTZ_PARKACTION:IDD_DIALOG_PTZ_PARKACTION
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {C4D53BEA-6961-4DBF-A97C-BECE76F174E2}
# 	1:23:IDD_DLG_ITS_OVERLAP_CFG:117
# 	2:16:Resource Include:resource.h
# 	2:18:DlgITSOverlapCfg.h:DlgITSOverlapCfg.h
# 	2:23:IDD_DLG_ITS_OVERLAP_CFG:IDD_DLG_ITS_OVERLAP_CFG
# 	2:10:ENUM: enum:enum
# 	2:24:CLASS: CDlgITSOverlapCfg:CDlgITSOverlapCfg
# 	2:19:Application Include:clientdemo.h
# 	2:20:DlgITSOverlapCfg.cpp:DlgITSOverlapCfg.cpp
# End Section
# Section ClientDemo : {C8BA8E49-5578-4D46-A065-BCC5B4576783}
# 	1:22:IDD_DLG_STREAM_SRC_CFG:129
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:22:IDD_DLG_STREAM_SRC_CFG:IDD_DLG_STREAM_SRC_CFG
# 	2:23:CLASS: CDlgStreamSrcCfg:CDlgStreamSrcCfg
# 	2:17:DlgStreamSrcCfg.h:DlgStreamSrcCfg.h
# 	2:19:DlgStreamSrcCfg.cpp:DlgStreamSrcCfg.cpp
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {C8899C02-75CC-4505-AB77-313BC90B61BF}
# 	1:11:IDD_DLG_CVR:147
# 	2:16:Resource Include:resource.h
# 	2:11:IDD_DLG_CVR:IDD_DLG_CVR
# 	2:10:DlgCVR.cpp:DlgCVR.cpp
# 	2:10:ENUM: enum:enum
# 	2:14:CLASS: CDlgCVR:CDlgCVR
# 	2:8:DlgCVR.h:DlgCVR.h
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {0AB6E01E-87F8-4D59-B775-181DA72B2C1C}
# 	1:21:IDD_DLG_IPADDR_FILTER:123
# 	2:16:Resource Include:resource.h
# 	2:20:DlgIPAddrFilterCfg.h:DlgIPAddrFilterCfg.h
# 	2:10:ENUM: enum:enum
# 	2:26:CLASS: CDlgIPAddrFilterCfg:CDlgIPAddrFilterCfg
# 	2:22:DlgIPAddrFilterCfg.cpp:DlgIPAddrFilterCfg.cpp
# 	2:19:Application Include:clientdemo.h
# 	2:21:IDD_DLG_IPADDR_FILTER:IDD_DLG_IPADDR_FILTER
# End Section
# Section ClientDemo : {7EC61CD5-5FDE-438A-A525-023F0124B4EA}
# 	1:26:IDD_DLG_STREAM_RECORD_INFO:126
# 	2:16:Resource Include:resource.h
# 	2:23:DlgStreamRecordInfo.cpp:DlgStreamRecordInfo.cpp
# 	2:27:CLASS: CDlgStreamRecordInfo:CDlgStreamRecordInfo
# 	2:26:IDD_DLG_STREAM_RECORD_INFO:IDD_DLG_STREAM_RECORD_INFO
# 	2:10:ENUM: enum:enum
# 	2:21:DlgStreamRecordInfo.h:DlgStreamRecordInfo.h
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {4A699F8F-19B5-4BEE-8864-ECD1131BB9DB}
# 	1:18:IDD_DLG_ITS_UPLOAD:119
# 	2:16:Resource Include:resource.h
# 	2:18:IDD_DLG_ITS_UPLOAD:IDD_DLG_ITS_UPLOAD
# 	2:14:DlgITSUpload.h:DlgITSUpload.h
# 	2:20:CLASS: CDlgITSUpload:CDlgITSUpload
# 	2:16:DlgITSUpload.cpp:DlgITSUpload.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {E54C74B0-DD75-49AF-A78B-0337531F5594}
# 	1:24:IDD_DLG_ITS_ROADINFO_GET:118
# 	2:16:Resource Include:resource.h
# 	2:16:DlgITSRoadInfo.h:DlgITSRoadInfo.h
# 	2:22:CLASS: CDlgITSRoadInfo:CDlgITSRoadInfo
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:clientdemo.h
# 	2:24:IDD_DLG_ITS_ROADINFO_GET:IDD_DLG_ITS_ROADINFO_GET
# 	2:18:DlgITSRoadInfo.cpp:DlgITSRoadInfo.cpp
# End Section
# Section ClientDemo : {6F468570-AD65-43E5-9091-746055444805}
# 	1:21:IDD_DLG_ITS_WORKSTATE:120
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:17:DlgITSWorkState.h:DlgITSWorkState.h
# 	2:19:DlgITSWorkState.cpp:DlgITSWorkState.cpp
# 	2:23:CLASS: CDlgITSWorkState:CDlgITSWorkState
# 	2:21:IDD_DLG_ITS_WORKSTATE:IDD_DLG_ITS_WORKSTATE
# 	2:19:Application Include:clientdemo.h
# End Section
# Section ClientDemo : {CC235959-24D2-45A3-BE5F-92BA470B268B}
# 	1:24:IDD_DLG_ITS_GET_BASEINFO:113
# 	2:16:Resource Include:resource.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:clientdemo.h
# 	2:16:DlgITSBaseInfo.h:DlgITSBaseInfo.h
# 	2:18:DlgITSBaseInfo.cpp:DlgITSBaseInfo.cpp
# 	2:22:CLASS: CDlgITSBaseInfo:CDlgITSBaseInfo
# 	2:24:IDD_DLG_ITS_GET_BASEINFO:IDD_DLG_ITS_GET_BASEINFO
# End Section
# Section ClientDemo : {34AAB9D9-263E-406A-91C7-53822DF1CDB5}
# 	1:27:IDD_DLG_XML_ITC_ITS_ABILITY:121
# 	2:16:Resource Include:resource.h
# 	2:27:IDD_DLG_XML_ITC_ITS_ABILITY:IDD_DLG_XML_ITC_ITS_ABILITY
# 	2:24:CLASS: CDlgXmlItcAbility:CDlgXmlItcAbility
# 	2:20:DlgXmlItcAbility.cpp:DlgXmlItcAbility.cpp
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:clientdemo.h
# 	2:18:DlgXmlItcAbility.h:DlgXmlItcAbility.h
# End Section
# Section ClientDemo : {5521B324-A91C-4869-8564-FF5D1F3F19C6}
# 	1:23:IDD_DLG_LANGUAGE_ENCODE:154
# 	2:16:Resource Include:resource.h
# 	2:23:IDD_DLG_LANGUAGE_ENCODE:IDD_DLG_LANGUAGE_ENCODE
# 	2:19:DlgLanguageEncode.h:DlgLanguageEncode.h
# 	2:10:ENUM: enum:enum
# 	2:19:Application Include:clientdemo.h
# 	2:25:CLASS: CDlgLanguageEncode:CDlgLanguageEncode
# 	2:21:DlgLanguageEncode.cpp:DlgLanguageEncode.cpp
# End Section
# Section ClientDemo : {D2305316-2798-4705-BDA2-1C0C71A687D5}
# 	1:21:IDD_DLG_VCA_BLACKLIST:104
# 	2:16:Resource Include:resource.h
# 	2:19:DlgVcaBlackList.cpp:DlgVcaBlackList.cpp
# 	2:17:DlgVcaBlackList.h:DlgVcaBlackList.h
# 	2:10:ENUM: enum:enum
# 	2:21:IDD_DLG_VCA_BLACKLIST:IDD_DLG_VCA_BLACKLIST
# 	2:23:CLASS: CDlgVcaBlackList:CDlgVcaBlackList
# 	2:19:Application Include:clientdemo.h
# End Section
