/**********************************************************
FileName:    DlgLogSearch.cpp
Description: search log     
Date:        2008/05/17
Note: 		 <global>struct, define refer to GeneralDef.h, global variables and functions refer to ClientDemo.cpp    
Modification History:      
<version> <time>         <desc>
<1.0    > <2008/05/17>       <created>
***********************************************************/

#include "stdafx.h"
#include "ClientDemo.h"
#include "DlgLogSearch.h"
#include ".\dlglogsearch.h"


// CDlgLogSearch dialog
CDlgLogSearch * pLogSearch = NULL;

void ParaTypeMap(NET_DVR_LOG_V30 stLogInfo, CString &szTemp)
{
    switch(stLogInfo.dwParaType)
    {
    case PARA_VIDEOOUT:
        szTemp.Format("VIDEOOUT");
        break;
    case PARA_IMAGE:
        szTemp.Format("IMAGE");
        break;
    case PARA_ENCODE:
        szTemp.Format("ENCODE");
        break;
    case PARA_NETWORK:
        szTemp.Format("NETWORK");
        break;
    case PARA_ALARM:
        szTemp.Format("ALARM");
        break;
    case PARA_EXCEPTION:
        szTemp.Format("EXCEPTION");
        break;
    case PARA_DECODER:
        szTemp.Format("DECODER");
        break;
    case PARA_RS232:
        szTemp.Format("RS232");
        break;
    case PARA_PREVIEW:
        szTemp.Format("PREVIEW");
        break;
    case PARA_SECURITY:
        szTemp.Format("SECURITY");
        break;
    case PARA_DATETIME:
        szTemp.Format("sys DATETIME");
        break;
    case PARA_FRAMETYPE:
        szTemp.Format("FRAMETYPE");
        break;
    case PARA_DETECTION:
        szTemp.Format("Detection");
        break;
    case PARA_VCA_RULE:
        szTemp.Format("VCA Rule");
        break;
    case PARA_VCA_CTRL:
        szTemp.Format("VCA Ctrl");
        break;
    case PARA_VCA_PLATE:
        szTemp.Format("VCA plate");
        break;
	//2010-04-13 
	case PARA_CODESPLITTER:
        szTemp.Format("CODESPLITTER");
        break;
    case PARA_RS485:
        szTemp.Format("RS485");
        break;
    case PARA_DEVICE:
        szTemp.Format("DEVICE");
        break;
    case PARA_HARDDISK:
        szTemp.Format("HARDDISK");
        break;
    case PARA_AUTOBOOT:
        szTemp.Format("AUTOBOOT");
        break;
    case PARA_HOLIDAY:
        szTemp.Format("HOLIDAY");
        break;
    case PARA_IPC	:
        szTemp.Format("IPC");
        break;
    default:
        break;
    }
}


/*********************************************************
Function:	AlarmMinorTypeMap
Desc:		主类型为报警的次类型映射
Input:	stLogInfo, log information; szTemp, log string;
Output:	none
Return:	none
**********************************************************/
void AlarmMinorTypeMap(NET_DVR_LOG_V30 stLogInfo, CString &szTemp)
{
    CString szParaType;
    switch(stLogInfo.dwMinorType)
    {
		//alarm
    case MINOR_ALARM_IN:
        szTemp.Format("ALARM_IN[%d]", stLogInfo.dwAlarmInPort);
        break;
    case MINOR_ALARM_OUT:
        szTemp.Format("ALARM_OUT[%d]",stLogInfo.dwAlarmOutPort);
        break;
    case MINOR_MOTDET_START:
        szTemp.Format("MOTDET_START[%d]", stLogInfo.dwChannel);
        break;
    case MINOR_MOTDET_STOP:
        szTemp.Format("MOTDET_STOP");
        break;
    case MINOR_HIDE_ALARM_START:
        szTemp.Format("HIDE_ALARM_START");
        break;
    case MINOR_HIDE_ALARM_STOP:
        szTemp.Format("HIDE_ALARM_STOP");
        break;
    case MINOR_VCA_ALARM_START:
        szTemp.Format("VCA_ALARM_START");
        break;
    case MINOR_VCA_ALARM_STOP:
        szTemp.Format("VCA_ALARM_STOP");
        break;
	case MINOR_VCA_SECNECHANGE_DETECTION://场景变更侦测日志查询2013-07-17
		szTemp.Format("VCA_SECNECHANGE_DETECTION");
		break;
	case MINOR_NETALARM_START:
        szTemp.Format("NETALARM_START");
        break;
	case MINOR_NETALARM_STOP:
        szTemp.Format("NETALARM_STOP");
        break;
	case MINOR_WIRELESS_ALARM_START:
        szTemp.Format("WIRELESS_ALARM_START");
        break;
	case MINOR_WIRELESS_ALARM_STOP:
        szTemp.Format("WIRELESS_ALARM_STOP");
        break;
	case MINOR_PIR_ALARM_START:
        szTemp.Format("PIR_ALARM_START");
        break;
	case MINOR_PIR_ALARM_STOP:
        szTemp.Format("PIR_ALARM_STOP");
        break;
	case MINOR_CALLHELP_ALARM_START:
        szTemp.Format("CALLHELP_ALARM_START");
        break;
	case MINOR_CALLHELP_ALARM_STOP:
        szTemp.Format("CALLHELP_ALARM_STOP");
        break;
	case MINOR_DETECTFACE_ALARM_START:
        szTemp.Format("DETECTFACE_ALARM_START");
        break;
	case MINOR_DETECTFACE_ALARM_STOP:
        szTemp.Format("DETECTFACE_ALARM_STOP");
        break;
	case MINOR_VQD_ALARM_START:
		szTemp.Format("VQD Alarm Start");
		break;
	case MINOR_VQD_ALARM_STOP:
		szTemp.Format("VQD Alarm Stop");
		break;
#ifdef NET_DVR_SET_TRAVERSE_PLANE_DETECTION
	case MINOR_VCA_ALARM_LINE_DETECTION_BEGIN:
		szTemp.Format("TRAVERSE_PLANE_DETECTION START");
		break;
	case MINOR_VCA_ALARM_LINE_DETECTION_END:
		szTemp.Format("TRAVERSE_PLANE_DETECTION Stop");
		break;
#endif //NET_DVR_SET_TRAVERSE_PLANE_DETECTION
	case MINOR_VCA_ALARM_INTRUDE_BEGIN:
		szTemp.Format("NTRUDE Start");
		break;
	case MINOR_VCA_ALARM_INTRUDE_END:
		szTemp.Format("NTRUDE Stop");
		break;
	case MINOR_VCA_ALARM_AUDIOINPUT:
		szTemp.Format("AudioInput Exception");
		break;
	case MINOR_VCA_ALARM_AUDIOABNORMAL:
		szTemp.Format("AudioInput Stop");
		break;
	case MINOR_VCA_DEFOCUS_DETECTION_BEGIN:
		szTemp.Format("defous detection Start");
		break;
	case MINOR_VCA_DEFOCUS_DETECTION_END:
		szTemp.Format("defous detection Stop");
		break;
	case MINOR_EXT_ALARM:
		szTemp.Format("external alarm");
		break;
	case MINOR_VCA_FACE_ALARM_BEGIN:
		szTemp.Format("SMD face alarm start");
		break;
	case MINOR_VCA_FACE_ALARM_END:
		szTemp.Format("SMD face alarm end");
		break;
	case MINOR_VCA_SCENE_CHANGE_ALARM_BEGIN:
		szTemp.Format("SMD scene change alarm start");
		break;
	case MINOR_VCA_SCENE_CHANGE_ALARM_END:
		szTemp.Format("SMD scene change alarm stop");
		break;
	case MINOR_VCA_ALARM_AUDIOINPUT_BEGIN:
		szTemp.Format("SMD alarm audio input start");
		break;
	case MINOR_VCA_ALARM_AUDIOINPUT_END:
		szTemp.Format("SMD alarm audio input stop");
		break;
	case MINOR_VCA_ALARM_AUDIOABNORMAL_BEGIN:
		szTemp.Format("SMD alarm audio abnormal start");
		break;
	case MINOR_VCA_ALARM_AUDIOABNORMAL_END:
		szTemp.Format("SMD alarm audio abnormal end");
		break;
	case MINOR_VCA_LECTURE_DETECTION_BEGIN:
		szTemp.Format("Lecture Start");
		break;
	case MINOR_VCA_LECTURE_DETECTION_END:
		szTemp.Format("Lecture End");
		break;
	case MINOR_VCA_ANSWER_DETECTION_BEGIN:
		szTemp.Format("Answering Questions Start");
		break;
	case MINOR_VCA_ANSWER_DETECTION_END:
		szTemp.Format("Answering Questions End");
		break;
	case MINOR_VCA_ALARM_AUDIOSTEEPDROP://2014-03-21
		szTemp.Format("ALARM_AUDIOSTEEPDROP");
		break;
    default:
        szTemp.Format("0x%x", stLogInfo.dwMinorType);
        break;
    }
}

/*********************************************************
Function:	ExceptionMinorTypeMap
Desc:		主类型为异常的次类型映射
Input:	stLogInfo, log information; szTemp, log string;
Output:	none
Return:	none
**********************************************************/
void ExceptionMinorTypeMap(NET_DVR_LOG_V30 stLogInfo, CString &szTemp)
{
    CString szParaType;
    switch(stLogInfo.dwMinorType)
    {
        //exception
	case MINOR_RAID_ERROR:
        szTemp.Format("RAID_ERROR");
        break;
    case MINOR_VI_LOST:
        szTemp.Format("VI_LOST");
        break;
    case MINOR_ILLEGAL_ACCESS:
        szTemp.Format("ILLEGAL_ACCESS");
        break;
    case MINOR_HD_FULL:
        szTemp.Format("HD_FULL[%d]", stLogInfo.dwDiskNumber);
        break;
    case MINOR_HD_ERROR:
        szTemp.Format("HD_ERROR[%d]", stLogInfo.dwDiskNumber);
        break;
    case MINOR_REC_ERROR:
        szTemp.Format("REC_ERROR");
        break;
    case MINOR_DCD_LOST:
        szTemp.Format("DCD_LOST");
        break;
    case MINOR_IPC_NO_LINK:
        szTemp.Format("IPC_NO_LINK");
        break;
    case MINOR_IP_CONFLICT:
        szTemp.Format("IP_CONFLICT");
        break;
    case MINOR_NET_BROKEN:
        szTemp.Format("NET_BROKEN");
        break;
    case MINOR_VI_EXCEPTION:
        szTemp.Format("VI_EXCEPTION");
        break;
    case MINOR_IPC_IP_CONFLICT:
        szTemp.Format("IPC_IP_CONFLICT");
        break;
    case MINOR_SENCE_EXCEPTION:
        szTemp.Format("SENCE_EXCEPTION");
        break;
	case MINOR_RESOLUTION_MISMATCH:
        szTemp.Format("RESOLUTION_MISMATCH");
        break;
	case MINOR_RECORD_OVERFLOW:
        szTemp.Format("RECORD_OVERFLOW");
        break;
	case MINOR_DSP_ABNORMAL:
        szTemp.Format("DSP_ABNORMAL");
        break;
    case MINOR_ANR_RECORD_FAIED:
        szTemp.Format("ANR_RECORD_FAIED");
        break;
	case MINOR_SPARE_WORK_DEVICE_EXCEPT:
        szTemp.Format("SPARE_WORK_DEVICE_EXCEPT");
        break;
	case MINOR_START_IPC_MAS_FAILED:
        szTemp.Format("START_IPC_MAS_FAILED");
        break;
	case MINOR_IPCM_CRASH:
        szTemp.Format("IPCM_ABNORMAL_RESTART");
        break;
    case MINOR_POE_POWER_EXCEPTION:
        szTemp.Format("POE_POWER_EXCEPTION");
        break;
    default:
        szTemp.Format("0x%x", stLogInfo.dwMinorType);
        break;
    }
}

/*********************************************************
Function:	OperationMinorTypeMap
Desc:		主类型为操作的次类型映射
Input:	stLogInfo, log information; szTemp, log string;
Output:	none
Return:	none
**********************************************************/
void OperationMinorTypeMap(NET_DVR_LOG_V30 stLogInfo, CString &szTemp)
{
    CString szParaType;
    switch(stLogInfo.dwMinorType)
    {
        //operation
    case MINOR_START_DVR:
        szTemp.Format("START_DVR");
        break;
    case MINOR_STOP_DVR:
        szTemp.Format("STOP_DVR");
        break;
    case MINOR_STOP_ABNORMAL:
        szTemp.Format("STOP_ABNORMAL");
        break;
    case MINOR_REBOOT_DVR:
        szTemp.Format("REBOOT_DVR");
        break;
    case MINOR_LOCAL_LOGIN:
        szTemp.Format("LOCAL_LOGIN");
        break;
    case MINOR_LOCAL_LOGOUT:
        szTemp.Format("%s", "LOCAL_LOGOUT");
        break;
    case MINOR_LOCAL_CFG_PARM:
        ParaTypeMap(stLogInfo, szParaType);
        szTemp.Format("%s-%s", "LOCAL_CFG_PARM", szParaType.GetBuffer(0));
        break;
    case MINOR_LOCAL_PLAYBYFILE:
        szTemp.Format("%s","LOCAL_PLAYBYFILE");
        break;
    case MINOR_LOCAL_PLAYBYTIME:
        szTemp.Format("%s", "LOCAL_PLAYBYTIME");
        break;
    case MINOR_LOCAL_START_REC:
        szTemp.Format("%s", "LOCAL_START_REC");
        break;
    case MINOR_LOCAL_STOP_REC:
        szTemp.Format("%s", "LOCAL_STOP_REC");
        break;
    case MINOR_LOCAL_PTZCTRL:
        szTemp.Format("%s", "LOCAL_PTZCTRL");
        break;
    case MINOR_LOCAL_PREVIEW:
        szTemp.Format("%s", "LOCAL_PREVIEW");
        break;
    case MINOR_LOCAL_MODIFY_TIME:
        szTemp.Format("%s", "LOCAL_MODIFY_TIME");
        break;
    case MINOR_LOCAL_UPGRADE:
        szTemp.Format("%s", "LOCAL_UPGRADE");
        break;
    case MINOR_LOCAL_COPYFILE:
        szTemp.Format("%s", "LOCAL_COPYFILE");
        break;
        
    case MINOR_LOCAL_LOCKFILE:
        szTemp.Format("%s", "LOCAL_LOCKFILE");
        break;
    case MINOR_LOCAL_UNLOCKFILE:
        szTemp.Format("%s", "LOCAL_UNLOCKFILE");
        break;
    case MINOR_LOCAL_FORMAT_HDD:
        szTemp.Format("%s", "LOCAL_FORMAT_HDD");
        break;
        
    case MINOR_REMOTE_LOGIN:
        szTemp.Format("%s", "REMOTE_LOGIN");
        break;
    case MINOR_REMOTE_LOGOUT:
        szTemp.Format("%s", "REMOTE_LOGOUT");
        break;
    case MINOR_REMOTE_START_REC:
        szTemp.Format("%s", "REMOTE_START_REC");
        break;
    case MINOR_REMOTE_STOP_REC:
        szTemp.Format("%s","REMOTE_STOP_REC");
        break;
    case MINOR_START_TRANS_CHAN:
        szTemp.Format("%s", "START_TRANS_CHAN");
        break;
    case MINOR_STOP_TRANS_CHAN:
        szTemp.Format("%s","STOP_TRANS_CHAN");
        break;
    case MINOR_REMOTE_GET_PARM:
        ParaTypeMap(stLogInfo, szParaType);
        szTemp.Format("%s-%s", "REMOTE_GET_PARM", szParaType.GetBuffer(0));
        break;
    case MINOR_REMOTE_CFG_PARM:
        ParaTypeMap(stLogInfo, szParaType);
        szTemp.Format("%s-%s", "REMOTE_CFG_PARM", szParaType.GetBuffer(0));
        break;
    case MINOR_REMOTE_GET_STATUS:
        szTemp.Format("%s", "REMOTE_GET_STATUS");
        break;
    case MINOR_REMOTE_ARM:
        szTemp.Format("%s", "REMOTE_ARM");
        break;
    case MINOR_REMOTE_DISARM:
        szTemp.Format("%s", "REMOTE_DISARM");
        break;
    case MINOR_REMOTE_REBOOT:
        szTemp.Format("%s", "REMOTE_REBOOT");
        break;
    case MINOR_START_VT:
        szTemp.Format("%s", "START_VT");
        break;
    case MINOR_STOP_VT:
        szTemp.Format("%s", "STOP_VT");
        break;
    case MINOR_REMOTE_UPGRADE:
        szTemp.Format("%s", "REMOTE_UPGRADE");
        break;
    case MINOR_REMOTE_PLAYBYFILE:
        szTemp.Format("%s", "REMOTE_PLAYBYFILE");
        break;
    case MINOR_REMOTE_PLAYBYTIME:
        szTemp.Format("%s", "REMOTE_PLAYBYTIME");
        break;
    case MINOR_REMOTE_PTZCTRL:
        szTemp.Format("%s", "REMOTE_PTZCTRL");
        break;
    case MINOR_REMOTE_STOP:
        szTemp.Format("%s", "REMOTE_STOP");
        break;
        
    case MINOR_REMOTE_LOCKFILE:
        szTemp.Format("%s", "REMOTE_LOCKFILE");
        break;
    case MINOR_REMOTE_CFGFILE_OUTPUT:
        szTemp.Format("%s", "REMOTE_CFGFILE_OUTPUT");
        break;
       
    case MINOR_REMOTE_CFGFILE_INTPUT:
        szTemp.Format("%s", "REMOTE_CFGFILE_INTPUT");
        break;
    case MINOR_REMOTE_FORMAT_HDD:
        szTemp.Format("%s", "REMOTE_FORMAT_HDD");
        break;
    case MINOR_REMOTE_IPC_ADD:
        szTemp.Format("%s", "REMOTE_IPC_ADD");
        break;
    case MINOR_REMOTE_IPC_DEL:
        szTemp.Format("%s", "REMOTE_IPC_DEL");
        break;

    case MINOR_REMOTE_IPC_SET:
        szTemp.Format("%s", "REMOTE_IPC_SET");
        break;
    case  MINOR_REBOOT_VCA_LIB:
        szTemp.Format("%s", "REBOOT_VCA_LIB");
        break;
    case MINOR_REMOTE_ADD_NAS:
        szTemp.Format("%s", "REMOTE_ADD_NAS");
        break;		
    case MINOR_REMOTE_DEL_NAS:
        szTemp.Format("%s", "REMOTE_DEL_NAS");
        break;
    case MINOR_REMOTE_SET_NAS:
        szTemp.Format("%s", "REMOTE_SET_NAS");
        break;
	case MINOR_LOCAL_OPERATE_LOCK :
		szTemp.Format("%s", "LOCAL_OPERATE_LOCK");
        break;
	case MINOR_LOCAL_OPERATE_UNLOCK:
		szTemp.Format("%s", "LOCAL_OPERATE_UNLOCK");
        break;
	case MINOR_REMOTE_DELETE_HDISK:
		szTemp.Format("%s", "REMOTE_DELETE_HDISK");
        break;
	case MINOR_REMOTE_LOAD_HDISK:
		szTemp.Format("%s", "REMOTE_LOAD_HDISK");
        break;
	case MINOR_REMOTE_UNLOAD_HDISK:
		szTemp.Format("%s", "REMOTE_UNLOAD_HDISK");
        break;
        //local
    case MINOR_LOCAL_CFGFILE_OUTPUT:
        szTemp.Format("%s", "LOCAL_CFGFILE_OUTPUT");
        break;
    case MINOR_LOCAL_CFGFILE_INPUT:
        szTemp.Format("%s", "LOCAL_CFGFILE_INPUT");
        break;
    case MINOR_LOCAL_DVR_ALARM:
        szTemp.Format("%s", "LOCAL_DVR_ALARM");
        break;
    case MINOR_REMOTE_DVR_ALARM:
        szTemp.Format("%s", "REMOTE_DVR_ALARM");
        break;
    case MINOR_IPC_ADD:
        szTemp.Format("%s", "IPC_ADD");
        break;
    case MINOR_IPC_DEL:
        szTemp.Format("%s", "IPC_DEL");
        break;
    case MINOR_IPC_SET:
        szTemp.Format("%s", "IPC_SET");
        break;
    case MINOR_LOCAL_START_BACKUP:
        szTemp.Format("%s", "LOCAL_START_BACKUP");
        break;
    case MINOR_LOCAL_STOP_BACKUP:
        szTemp.Format("%s", "LOCAL_STOP_BACKUP");
        break;
    case MINOR_LOCAL_COPYFILE_START_TIME:
        szTemp.Format("%s", "LOCAL_COPYFILE_START_TIME");
        break;
    case MINOR_LOCAL_COPYFILE_END_TIME:
        szTemp.Format("%s", "LOCAL_COPYFILE_START_TIME");
        break;
    case MINOR_LOCAL_ADD_NAS:
        szTemp.Format("%s", "LOCAL_ADD_NAS");
        break;
    case MINOR_LOCAL_DEL_NAS:
        szTemp.Format("%s", "LOCAL_DEL_NAS");
        break;
    case MINOR_LOCAL_SET_NAS:
        szTemp.Format("%s", "LOCAL_SET_NAS");
        break;
	//2010-04-13 
	case MINOR_FANABNORMAL:
        szTemp.Format("%s", "FANABNORMAL");
        break;
    case  MINOR_FANRESUME:
        szTemp.Format("%s", "FANRESUME");
        break;
    case MINOR_SUBSYSTEM_ABNORMALREBOOT:
        szTemp.Format("%s", "SUBSYSTEM_ABNORMALREBOOT");
        break;	 
    case MINOR_MATRIX_STARTBUZZER:
        szTemp.Format("%s", "MATRIX_STARTBUZZER");
        break;	 
    case MINOR_NET_ABNORMAL:
        szTemp.Format("%s", "NET_ABNORMAL");
        break;	 
	case MINOR_MEM_ABNORMAL:
        szTemp.Format("%s", "MEM_ABNORMAL");
        break;
    case MINOR_FILE_ABNORMAL:
        szTemp.Format("%s", "FILE_ABNORMAL");
        break;
    case MINOR_SUBSYSTEMREBOOT:
        szTemp.Format("%s", "SUBSYSTEMREBOOT");
        break;	 
    case MINOR_MATRIX_STARTTRANSFERVIDEO:
        szTemp.Format("%s", "MATRIX_STARTTRANSFERVIDEO");
        break;	 
    case MINOR_MATRIX_STOPTRANSFERVIDEO:
        szTemp.Format("%s", "MATRIX_STOPTRANSFERVIDEO");
        break;	 
	case MINOR_REMOTE_SET_ALLSUBSYSTEM:
        szTemp.Format("%s", "REMOTE_SET_ALLSUBSYSTEM");
        break;
    case MINOR_REMOTE_GET_ALLSUBSYSTEM:
        szTemp.Format("%s", "REMOTE_GET_ALLSUBSYSTEM");
        break;
    case MINOR_REMOTE_SET_PLANARRAY:
        szTemp.Format("%s", "REMOTE_SET_PLANARRAY");
        break;	 
    case MINOR_REMOTE_GET_PLANARRAY:
        szTemp.Format("%s", "REMOTE_GET_PLANARRAY");
        break;	 
    case MINOR_MATRIX_STARTTRANSFERAUDIO:
        szTemp.Format("%s", "MATRIX_STARTTRANSFERAUDIO");
        break;	 
	case MINOR_MATRIX_STOPRANSFERAUDIO:
        szTemp.Format("%s", "MATRIX_STOPRANSFERAUDIO");
        break;
    case MINOR_LOGON_CODESPITTER:
        szTemp.Format("%s", "LOGON_CODESPITTER");
        break;
    case MINOR_LOGOFF_CODESPITTER:
        szTemp.Format("%s", "LOGOFF_CODESPITTER");
        break;	 
    case MINOR_START_DYNAMIC_DECODE:
        szTemp.Format("%s", "START_DYNAMIC_DECODE");
        break;	 
    case MINOR_STOP_DYNAMIC_DECODE:
        szTemp.Format("%s", "STOP_DYNAMIC_DECODE");
        break;	 
	case MINOR_GET_CYC_CFG:
        szTemp.Format("%s", "GET_CYC_CFG");
        break;
    case MINOR_SET_CYC_CFG:
        szTemp.Format("%s", "SET_CYC_CFG");
        break;
    case MINOR_START_CYC_DECODE:
        szTemp.Format("%s", "START_CYC_DECODE");
        break;	 
    case MINOR_STOP_CYC_DECODE:
        szTemp.Format("%s", "STOP_CYC_DECODE");
        break;	 
    case MINOR_GET_DECCHAN_STATUS:
        szTemp.Format("%s", "GET_DECCHAN_STATUS");
        break;	 
	case MINOR_GET_DECCHAN_INFO:
        szTemp.Format("%s", "GET_DECCHAN_INFO");
        break;
    case MINOR_START_PASSIVE_DEC:
        szTemp.Format("%s", "START_PASSIVE_DEC");
        break;
    case MINOR_STOP_PASSIVE_DEC:
        szTemp.Format("%s", "STOP_PASSIVE_DEC");
        break;	 
    case MINOR_CTRL_PASSIVE_DEC:
        szTemp.Format("%s", "CTRL_PASSIVE_DEC");
        break;	 
    case MINOR_RECON_PASSIVE_DEC:
        szTemp.Format("%s", "RECON_PASSIVE_DEC");
        break;			
	case MINOR_GET_DEC_CHAN_SW:
        szTemp.Format("%s", "GET_DEC_CHAN_SW");
        break;
    case MINOR_SET_DEC_CHAN_SW:
        szTemp.Format("%s", "SET_DEC_CHAN_SW");
        break;	 
    case MINOR_CTRL_DEC_CHAN_SCALE:
        szTemp.Format("%s", "CTRL_DEC_CHAN_SCALE");
        break;	 
    case MINOR_SET_REMOTE_REPLAY:
        szTemp.Format("%s", "SET_REMOTE_REPLAY");
        break;	 
	case MINOR_GET_REMOTE_REPLAY:
        szTemp.Format("%s", "GET_REMOTE_REPLAY");
        break;
    case MINOR_CTRL_REMOTE_REPLAY:
        szTemp.Format("%s", "CTRL_REMOTE_REPLAY");
        break;
    case MINOR_SET_DISP_CFG:
        szTemp.Format("%s", "SET_DISP_CFG");
        break;	 
    case MINOR_GET_DISP_CFG:
        szTemp.Format("%s", "GET_DISP_CFG");
        break;	 
    case MINOR_SET_PLANTABLE:
        szTemp.Format("%s", "SET_PLANTABLE");
        break;	 
	case MINOR_GET_PLANTABLE:
        szTemp.Format("%s", "GET_PLANTABLE");
        break;
    case MINOR_START_PPPPOE:
        szTemp.Format("%s", "START_PPPPOE");
        break;	 
    case MINOR_STOP_PPPPOE:
        szTemp.Format("%s", "STOP_PPPPOE");
        break;	 
    case MINOR_UPLOAD_LOGO:
        szTemp.Format("%s", "UPLOAD_LOGO");
        break;	 
    case MINOR_LOCAL_CONF_REB_RAID:
        szTemp.Format("%s", "LOCAL_CONF_REB_RAID");
        break;
    case MINOR_LOCAL_CONF_SPARE:
        szTemp.Format("%s", "LOCAL_CONF_SPARE");
        break;
    case MINOR_LOCAL_ADD_RAID:
        szTemp.Format("%s", "LOCAL_ADD_RAID");
        break;
    case MINOR_LOCAL_DEL_RAID:
        szTemp.Format("%s", "LOCAL_DEL_RAID");
        break;
    case MINOR_LOCAL_MIG_RAID:
        szTemp.Format("%s", "LOCAL_MIG_RAID");
        break;
    case MINOR_LOCAL_REB_RAID:
        szTemp.Format("%s", "LOCAL_REB_RAID");
        break;
    case MINOR_LOCAL_QUICK_CONF_RAID:
        szTemp.Format("%s", "LOCAL_QUICK_CONF_RAID");
        break;
    case MINOR_LOCAL_ADD_VD:
        szTemp.Format("%s", "LOCAL_ADD_VD");
        break;
    case MINOR_LOCAL_DEL_VD:
        szTemp.Format("%s", "LOCAL_DEL_VD");
        break;
    case MINOR_LOCAL_RP_VD:
        szTemp.Format("%s", "LOCAL_RP_VD");
        break;
    case MINOR_LOCAL_FORMAT_EXPANDVD:
        szTemp.Format("%s", "LOCAL_FORMAT_EXPANDVD");
        break;
    case MINOR_LOCAL_RAID_UPGRADE:
        szTemp.Format("%s", "LOCAL_RAID_UPGRADE");
		break;
	case MINOR_LOCAL_STOP_RAID:
        szTemp.Format("%s", "LOCAL_STOP_RAID");
        break;
    case MINOR_REMOTE_CONF_REB_RAID:
        szTemp.Format("%s", "REMOTE_CONF_REB_RAID");
        break;
    case MINOR_REMOTE_CONF_SPARE:
        szTemp.Format("%s", "REMOTE_CONF_SPARE");
        break;
    case MINOR_REMOTE_ADD_RAID:
        szTemp.Format("%s", "REMOTE_ADD_RAID");
        break;
    case MINOR_REMOTE_DEL_RAID:
        szTemp.Format("%s", "REMOTE_DEL_RAID");
        break;
    case MINOR_REMOTE_MIG_RAID:
        szTemp.Format("%s", "REMOTE_MIG_RAID");
        break;
    case MINOR_REMOTE_REB_RAID:
        szTemp.Format("%s", "REMOTE_REB_RAID");
        break;
    case MINOR_REMOTE_QUICK_CONF_RAID:
        szTemp.Format("%s", "REMOTE_QUICK_CONF_RAID");
        break;
    case MINOR_REMOTE_ADD_VD:
        szTemp.Format("%s", "REMOTE_ADD_VD");
        break;
    case MINOR_REMOTE_DEL_VD:
        szTemp.Format("%s", "REMOTE_DEL_VD");
        break;
    case MINOR_REMOTE_RP_VD:
        szTemp.Format("%s", "REMOTE_RP_VD");
        break;
    case MINOR_REMOTE_FORMAT_EXPANDVD:
        szTemp.Format("%s", "REMOTE_FORMAT_EXPANDVD");
        break;
    case MINOR_REMOTE_RAID_UPGRADE:
        szTemp.Format("%s", "REMOTE_RAID_UPGRADE");
        break;
	case MINOR_REMOTE_STOP_RAID:
        szTemp.Format("%s", "REMOTE_STOP_RAID");
        break;
	case MINOR_LOCAL_START_PIC_REC:
        szTemp.Format("%s", "LOCAL_START_PIC_REC");
        break;
    case MINOR_LOCAL_STOP_PIC_REC:
        szTemp.Format("%s", "LOCAL_STOP_PIC_REC");
        break;
    case MINOR_LOCAL_SET_SNMP:
        szTemp.Format("%s", "LOCAL_SET_SNMP");
        break;
	case MINOR_LOCAL_TAG_OPT:
        szTemp.Format("%s", "LOCAL_TAG_OPT");
        break;
	case MINOR_REMOTE_START_PIC_REC:
        szTemp.Format("%s", "REMOTE_START_PIC_REC");
        break;
    case MINOR_REMOTE_STOP_PIC_REC:
        szTemp.Format("%s", "REMOTE_STOP_PIC_REC");
        break;
    case MINOR_REMOTE_SET_SNMP:
        szTemp.Format("%s", "REMOTE_SET_SNMP");
        break;
	case MINOR_REMOTE_TAG_OPT:
        szTemp.Format("%s", "REMOTE_TAG_OPT");
        break;
	case MINOR_LOCAL_VOUT_SWITCH:
        szTemp.Format("%s", "LOCAL_VOUT_SWITCH");
        break;
	case MINOR_STREAM_CABAC:
        szTemp.Format("%s", "STREAM_CABAC");
        break;
    case MINOR_LOCAL_SPARE_OPT:
        szTemp.Format("%s", "LOCAL_SPARE_OPT");
        break;
	case MINOR_REMOTE_SPARE_OPT:
        szTemp.Format("%s", "REMOTE_SPARE_OPT");
        break;
	case MINOR_LOCAL_IPCCFGFILE_OUTPUT:
        szTemp.Format("%s", "LOCAL_IPCCFGFILE_OUTPUT");
        break;
	case MINOR_LOCAL_IPCCFGFILE_INPUT:
        szTemp.Format("%s", "LOCAL_IPCCFGFILE_INPUT");
        break;
	case MINOR_LOCAL_IPC_UPGRADE:
        szTemp.Format("%s", "LOCAL_IPC_UPGRADE");
        break;
	case MINOR_REMOTE_IPCCFGFILE_OUTPUT:
        szTemp.Format("%s", "REMOTE_IPCCFGFILE_OUTPUT");
        break;
	case MINOR_REMOTE_IPCCFGFILE_INPUT:
        szTemp.Format("%s", "REMOTE_IPCCFGFILE_INPUT");
        break;
	case MINOR_REMOTE_IPC_UPGRADE:
        szTemp.Format("%s", "REMOTE_IPC_UPGRADE");
        break;
	case MINOR_LOCAL_LOAD_HDISK:
        szTemp.Format("%s", "LOCAL_LOAD_HDISK");
        break;
	case MINOR_LOCAL_DELETE_HDISK:
        szTemp.Format("%s", "LOCAL_DELETE_HDISK");
        break;
	case MINOR_LOCAL_MAIN_AUXILIARY_PORT_SWITCH:
		szTemp.Format("%s", "LOCAL_MAIN_AUXILIARY_PORT_SWITCH");
        break;
	case MINOR_LOCAL_HARD_DISK_CHECK:
		szTemp.Format("%s", "LOCAL_HARD_DISK_CHECK");
        break;
	case MINOR_LOCAL_PIN:
		szTemp.Format("%s", "LOCAL_PIN");
		break;
	case MINOR_LOCAL_DIAL:
		szTemp.Format("%s", "LOCAL_DIAL");
		break;
	case MINOR_SMS_CONTROL:
		szTemp.Format("%s", "SMS_CONTROL");
		break;
	case MINOR_CALL_ONLINE:
		szTemp.Format("%s", "CALL_ONLINE");
		break;
	case MINOR_REMOTE_PIN:
		szTemp.Format("%s", "REMOTE_PIN");
		break;
		
	case MINOR_REMOTE_DEVICE_CONTROL:
		szTemp.Format("%s", "REMOTE_DEVICE_CONTROL");
		break;
	case MINOR_REMOTE_SET_GATE_PARM_CFG:
		szTemp.Format("%s", "REMOTE_SET_GATE_PARM_CFG");
		break;
	case MINOR_REMOTE_GET_GATE_PARM_CFG:
		szTemp.Format("%s", "REMOTE_GET_GATE_PARM_CFG");
		break;
	case MINOR_REMOTE_SET_DATAUPLOAD_PARM_CFG:
		szTemp.Format("%s", "REMOTE_SET_DATAUPLOAD_PARM_CFG");
		break;
	case MINOR_REMOTE_GET_DATAUPLOAD_PARM_CFG:
		szTemp.Format("%s", "REMOTE_GET_DATAUPLOAD_PARM_CFG");
		break;
	case MINOR_REMOTE_GET_BASE_INFO:
		szTemp.Format("%s", "REMOTE_GET_BASE_INFO");
		break;
	case MINOR_REMOTE_GET_OVERLAP_CFG:
		szTemp.Format("%s", "REMOTE_GET_OVERLAP_CFG");
		break;
	case MINOR_REMOTE_SET_OVERLAP_CFG:
		szTemp.Format("%s", "REMOTE_SET_OVERLAP_CFG");
		break;
	case MINOR_REMOTE_GET_ROAD_INFO:
		szTemp.Format("%s", "REMOTE_GET_ROAD_INFO");
		break;
	case MINOR_REMOTE_START_TRANSCHAN:
		szTemp.Format("%s", "REMOTE_START_TRANSCHAN");
		break;
	case MINOR_REMOTE_GET_ECTWORKSTATE:
		szTemp.Format("%s", "REMOTE_GET_ECTWORKSTATE");
		break;
	case MINOR_REMOTE_GET_ECTCHANINFO:
		szTemp.Format("%s", "REMOTE_GET_ECTCHANINFO");
		break;
		//2013-11-19
	case MINOR_REMOTE_ADD_EXTERNAL_DEVICE_INFO:
		szTemp.Format("%s", "REMOTE_ADD_EXTERNAL_DEVICE_INFO");
		break;
	case MINOR_REMOTE_MOD_EXTERNAL_DEVICE_INFO:
		szTemp.Format("%s", "REMOTE_MOD_EXTERNAL_DEVICE_INFO");
		break;
	case MINOR_REMOTE_GET_ENDETCFG:
		szTemp.Format("%s", "REMOTE_GET_ENDETCFG");
		break;
	case MINOR_REMOTE_SET_ENDETCFG:
		szTemp.Format("%s", "REMOTE_SET_ENDETCFG");
		break;
	case MINOR_REMOTE_ENDEV_ISSUEDDATA:
		szTemp.Format("%s", "REMOTE_ENDEV_ISSUEDDATA");
		break;
	case MINOR_REMOTE_DEL_ENDEV_ISSUEDDATA:
		szTemp.Format("%s", "REMOTE_DEL_ENDEV_ISSUEDDATA");
		break;

	case MINOR_LOCAL_ADD_CAR_INFO:
		szTemp.Format("%s", "LOCAL_ADD_CAR_INFO");
		break;
	case MINOR_LOCAL_MOD_CAR_INFO:
		szTemp.Format("%s", "LOCAL_MOD_CAR_INFO");
		break;
	case MINOR_LOCAL_DEL_CAR_INFO:
		szTemp.Format("%s", "LOCAL_DEL_CAR_INFO");
		break;
	case MINOR_LOCAL_FIND_CAR_INFO:
		szTemp.Format("%s", "LOCAL_FIND_CAR_INFO");
		break;
	case MINOR_LOCAL_ADD_MONITOR_INFO:
		szTemp.Format("%s", "LOCAL_ADD_MONITOR_INFO");
		break;
	case MINOR_LOCAL_MOD_MONITOR_INFO:
		szTemp.Format("%s", "LOCAL_MOD_MONITOR_INFO");
		break;
	case MINOR_LOCAL_DEL_MONITOR_INFO:
		szTemp.Format("%s", "LOCAL_DEL_MONITOR_INFO");
		break;
	case MINOR_LOCAL_FIND_MONITOR_INFO:
		szTemp.Format("%s", "LOCAL_FIND_MONITOR_INFO");
		break;

	case MINOR_LOCAL_FIND_NORMAL_PASS_INFO:
		szTemp.Format("%s", "LOCAL_FIND_NORMAL_PASS_INFO");
		break;
	case MINOR_LOCAL_FIND_ABNORMAL_PASS_INFO:
		szTemp.Format("%s", "LOCAL_FIND_ABNORMAL_PASS_INFO");
		break;
	case MINOR_LOCAL_FIND_PEDESTRIAN_PASS_INFO:
		szTemp.Format("%s", "LOCAL_FIND_PEDESTRIAN_PASS_INFO");
		break;
	case MINOR_LOCAL_PIC_PREVIEW:
		szTemp.Format("%s", "LOCAL_PIC_PREVIEW");
		break;
	case MINOR_LOCAL_SET_GATE_PARM_CFG:
		szTemp.Format("%s", "LOCAL_SET_GATE_PARM_CFG");
		break;

	case MINOR_LOCAL_GET_GATE_PARM_CFG:
		szTemp.Format("%s", "LOCAL_GET_GATE_PARM_CFG");
		break;
	case MINOR_LOCAL_SET_DATAUPLOAD_PARM_CFG:
		szTemp.Format("%s", "LOCAL_SET_DATAUPLOAD_PARM_CFG");
		break;
	case MINOR_LOCAL_GET_DATAUPLOAD_PARM_CFG:
		szTemp.Format("%s", "LOCAL_GET_DATAUPLOAD_PARM_CFG");
		break;
		//2013-11-19
	case MINOR_LOCAL_DEVICE_CONTROL:
		szTemp.Format("%s", "LOCAL_DEVICE_CONTROL");
		break;
	case MINOR_LOCAL_ADD_EXTERNAL_DEVICE_INFO:
		szTemp.Format("%s", "LOCAL_ADD_EXTERNAL_DEVICE_INFO");
		break;
	case MINOR_LOCAL_MOD_EXTERNAL_DEVICE_INFO:
		szTemp.Format("%s", "LOCAL_MOD_EXTERNAL_DEVICE_INFO");
		break;
	case MINOR_LOCAL_DEL_EXTERNAL_DEVICE_INFO:
		szTemp.Format("%s", "LOCAL_DEL_EXTERNAL_DEVICE_INFO");
		break;
	case MINOR_LOCAL_FIND_EXTERNAL_DEVICE_INFO:
		szTemp.Format("%s", "LOCAL_FIND_EXTERNAL_DEVICE_INFO");
		break;
	case MINOR_LOCAL_ADD_CHARGE_RULE:
		szTemp.Format("%s", "LOCAL_ADD_CHARGE_RULE");
		break;	
	case MINOR_LOCAL_MOD_CHARGE_RULE:
		szTemp.Format("%s", "LOCAL_MOD_CHARGE_RULE");
		break;
	case MINOR_LOCAL_DEL_CHARGE_RULE:
		szTemp.Format("%s", "LOCAL_DEL_CHARGE_RULE");
		break;	
	case MINOR_LOCAL_FIND_CHARGE_RULE:
		szTemp.Format("%s", "LOCAL_FIND_CHARGE_RULE");
		break;
	case MINOR_LOCAL_COUNT_NORMAL_CURRENTINFO:
		szTemp.Format("%s", "LOCAL_COUNT_NORMAL_CURRENTINFO");
		break;	
	case MINOR_LOCAL_EXPORT_NORMAL_CURRENTINFO_REPORT:
		szTemp.Format("%s", "LOCAL_EXPORT_NORMAL_CURRENTINFO_REPORT");
		break;
	case MINOR_LOCAL_COUNT_ABNORMAL_CURRENTINFO:
		szTemp.Format("%s", "LOCAL_COUNT_ABNORMAL_CURRENTINFO");
		break;	
	case MINOR_LOCAL_EXPORT_ABNORMAL_CURRENTINFO_REPORT:
		szTemp.Format("%s", "LOCAL_EXPORT_ABNORMAL_CURRENTINFO_REPORT");
		break;
	case MINOR_LOCAL_COUNT_PEDESTRIAN_CURRENTINFO:
		szTemp.Format("%s", "LOCAL_COUNT_PEDESTRIAN_CURRENTINFO");
		break;
	case MINOR_LOCAL_EXPORT_PEDESTRIAN_CURRENTINFO_REPORT:
		szTemp.Format("%s", "LOCAL_EXPORT_PEDESTRIAN_CURRENTINFO_REPORT");
		break;
	case MINOR_LOCAL_FIND_CAR_CHARGEINFO:
		szTemp.Format("%s", "LOCAL_FIND_CAR_CHARGEINFO");
		break;
	case MINOR_LOCAL_COUNT_CAR_CHARGEINFO:
		szTemp.Format("%s", "LOCAL_COUNT_CAR_CHARGEINFO");
		break;
	case MINOR_LOCAL_EXPORT_CAR_CHARGEINFO_REPORT:
		szTemp.Format("%s", "LOCAL_EXPORT_CAR_CHARGEINFO_REPORT");
		break;
	case MINOR_LOCAL_FIND_SHIFTINFO:
		szTemp.Format("%s", "LOCAL_FIND_SHIFTINFO");
		break;
	case MINOR_LOCAL_FIND_CARDINFO:
		szTemp.Format("%s", "LOCAL_FIND_CARDINFO");
		break;
	case MINOR_LOCAL_ADD_RELIEF_RULE:
		szTemp.Format("%s", "LOCAL_ADD_RELIEF_RULE");
		break;
	case MINOR_LOCAL_MOD_RELIEF_RULE:
		szTemp.Format("%s", "LOCAL_MOD_RELIEF_RULE");
		break;
	case MINOR_LOCAL_DEL_RELIEF_RULE:
		szTemp.Format("%s", "LOCAL_DEL_RELIEF_RULE");
		break;
	case MINOR_LOCAL_FIND_RELIEF_RULE:
		szTemp.Format("%s", "LOCAL_FIND_RELIEF_RULE");
		break;
	case MINOR_LOCAL_GET_ENDETCFG:
		szTemp.Format("%s", "LOCAL_GET_ENDETCFG");
		break;
	case MINOR_LOCAL_SET_ENDETCFG:
		szTemp.Format("%s", "LOCAL_SET_ENDETCFG");
		break;
	case MINOR_LOCAL_SET_ENDEV_ISSUEDDATA:
		szTemp.Format("%s", "LOCAL_SET_ENDEV_ISSUEDDATA");
		break;
	case MINOR_LOCAL_DEL_ENDEV_ISSUEDDATA:
		szTemp.Format("%s", "LOCAL_DEL_ENDEV_ISSUEDDATA");
		break;
    default:
        szTemp.Format("0x%x", stLogInfo.dwMinorType);
        break;
    }
}

/*********************************************************
Function:	InfoMinorTypeMap
Desc:		get the string of the log minor type 
Input:	stLogInfo, log information; szTemp, log string;
Output:	none
Return:	none
**********************************************************/
void InfoMinorTypeMap(NET_DVR_LOG_V30 stLogInfo, CString &szTemp)
{
    CString szParaType;
    switch(stLogInfo.dwMinorType)
    {
        //information
    case MINOR_HDD_INFO:
        szTemp.Format("%s", "HDD_INFO");
        break;
    case MINOR_SMART_INFO:
        szTemp.Format("%s", "SMART_INFO");
        break;
    case MINOR_REC_START:
        szTemp.Format("%s", "REC_START");
        break;
    case MINOR_REC_STOP:
        szTemp.Format("%s", "REC_STOP");
        break;
    case MINOR_REC_OVERDUE:
        szTemp.Format("%s", "REC_OVERDUE");
        break;	 
    case MINOR_LINK_START:
        szTemp.Format("%s", "LINK_START");
        break;	 
    case MINOR_LINK_STOP:
        szTemp.Format("%s", "LINK_STOP");
        break;	 
	case MINOR_NET_DISK_INFO:
		szTemp.Format("%s", "NET_DISK_INFO");
        break;
    case MINOR_RAID_INFO:
        szTemp.Format("%s", "RAID_INFO");
        break;
	case MINOR_RUN_STATUS_INFO:
		szTemp.Format("%s","RUN_STATUS_INFO");
		break;
	case MINOR_SPARE_START_BACKUP:
        szTemp.Format("%s", "SPARE_START_BACKUP");
        break;	 
    case MINOR_SPARE_STOP_BACKUP:
        szTemp.Format("%s", "SPARE_STOP_BACKUP");
        break;	 
	case MINOR_SPARE_CLIENT_INFO:
		szTemp.Format("%s", "SPARE_CLIENT_INFO");
        break;
    case MINOR_ANR_RECORD_START:
        szTemp.Format("%s", "ANR_RECORD_START");
        break;
	case MINOR_ANR_RECORD_END:
		szTemp.Format("%s","ANR_RECORD_END");
		break;
	case MINOR_ANR_ADD_TIME_QUANTUM:
        szTemp.Format("%s", "MINOR_ANR_ADD_TIME_QUANTUM");
        break;
	case MINOR_ANR_DEL_TIME_QUANTUM:
		szTemp.Format("%s","MINOR_ANR_DEL_TIME_QUANTUM");
		break;
    default:
        szTemp.Format("0x%x", stLogInfo.dwMinorType);
        break;
    }
}
/*********************************************************
Function:	GetLogFileThread
Desc:		get log file thread
Input:		pParam, parameter pointer
Output:	
Return:	
**********************************************************/
UINT GetLogFileThread(LPVOID pParam)
{
    UNREFERENCED_PARAMETER(pParam);
    
    LONG bRet = -1;
	
	NET_DVR_LOG_MATRIX struFileInfoMatrix = {0};
	
	NET_DVR_LOG_V30 struFileInfo;	
	
    CString csTmp;
    char szLan[128] = {0};
	char szNetUser[17] = {0};
	char szPanelUser[17] = {0};
    while (1) 
    {
		if (pLogSearch->m_bVideoPlatLog)
		{
			bRet = NET_DVR_FindNextLog_MATRIX(pLogSearch->m_lLogHandle, &struFileInfoMatrix);
			if (bRet == NET_DVR_FILE_SUCCESS)
			{
				csTmp.Format("%d", pLogSearch->m_lLogNum+1);
				pLogSearch->m_listDeviceLog.InsertItem(pLogSearch->m_lLogNum, csTmp,0);
				csTmp.Format("%04d-%02d-%02d %02d:%02d:%02d", struFileInfoMatrix.strLogTime.dwYear, struFileInfoMatrix.strLogTime.dwMonth, struFileInfoMatrix.strLogTime.dwDay, \
					struFileInfoMatrix.strLogTime.dwHour,struFileInfoMatrix.strLogTime.dwMinute, struFileInfoMatrix.strLogTime.dwSecond);
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 1, csTmp);	
				switch(struFileInfoMatrix.dwMajorType)
				{
				case 1:
					g_StringLanType(szLan, "报警", "Alarm");
					csTmp.Format("%s",szLan);
					break;
				case 2:
					g_StringLanType(szLan, "异常", "Abnormal");
					csTmp.Format("%s",szLan);
					break;
				case 3:
					g_StringLanType(szLan, "操作", "Operation");
					csTmp.Format("%s",szLan);
					break;
				case 4:
					g_StringLanType(szLan, "附加信息", "Additional information");
					csTmp.Format("%s",szLan);
					break;
				default:
					g_StringLanType(szLan, "未知类型", "Unknown");
					csTmp.Format("%s",szLan);
					break;
				}
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 2, csTmp);
				memcpy(&struFileInfo, &struFileInfoMatrix, sizeof(struFileInfoMatrix)); 
				if (MAJOR_INFORMATION == struFileInfo.dwMajorType)
				{
					InfoMinorTypeMap(struFileInfo, csTmp);
				}
				else if(MAJOR_OPERATION == struFileInfo.dwMajorType)
				{
					OperationMinorTypeMap(struFileInfo, csTmp);
				}
				else if(MAJOR_ALARM == struFileInfo.dwMajorType)
				{
					AlarmMinorTypeMap(struFileInfo, csTmp);
				}
				else if(MAJOR_EXCEPTION == struFileInfo.dwMajorType)
				{
					ExceptionMinorTypeMap(struFileInfo, csTmp);
				}
				
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 3, csTmp);
				
				memset(szNetUser,0,17);
				memset(szPanelUser,0,17);
				memcpy(szNetUser, struFileInfoMatrix.sNetUser,MAX_NAMELEN);
				memcpy(szPanelUser, struFileInfoMatrix.sPanelUser, MAX_NAMELEN);
				sprintf(szLan, "[%s-%s]", szNetUser, szPanelUser);
				if (struFileInfoMatrix.dwChannel > 0)
				{
					sprintf(szLan, "%schan[%d]", szLan, struFileInfoMatrix.dwChannel);
				}
				
				if (struFileInfoMatrix.dwAlarmInPort != 0)
				{
					sprintf(szLan, "%sAlarmIn[%d]", szLan, struFileInfoMatrix.dwAlarmInPort);
				}
				
				if (struFileInfoMatrix.dwAlarmOutPort != 0)
				{
					sprintf(szLan, "%sAlarmOut[%d]", szLan, struFileInfoMatrix.dwAlarmOutPort);
				}
				
				if (pLogSearch->ValidIPv6(struFileInfoMatrix.struRemoteHostAddr.byIPv6))
				{
					csTmp.Format("%s-%s",struFileInfoMatrix.struRemoteHostAddr.byIPv6, szLan);
				}
				else
				{
					csTmp.Format("%s-%s",struFileInfoMatrix.struRemoteHostAddr.sIpV4, szLan);
				}
				
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 4, csTmp);
				
				if (struFileInfoMatrix.dwInfoLen > 0)
				{
					csTmp.Format("%s",struFileInfoMatrix.sInfo);
				}
				else
				{
					csTmp.Format("");
				}
				
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 5, csTmp);
				
				pLogSearch->m_lLogNum++;
			}
			else
			{
				if (bRet == NET_DVR_ISFINDING)
				{
					pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_SHOW);
					pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText("日志搜索中......");
					Sleep(5);
					continue;
				}
				if ((bRet == NET_DVR_NOMOREFILE) || (bRet == NET_DVR_FILE_NOFIND))
				{
					g_StringLanType(szLan, "搜索日志", "Search Log");
					pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
					pLogSearch->m_bSearch = FALSE;
					pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_HIDE);
					g_StringLanType(szLan, "搜索日志结束!", "Search log Ending");
					AfxMessageBox(szLan);
					break;
				}
				else
				{
					g_StringLanType(szLan, "搜索日志", "Search Log");
					pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
					pLogSearch->m_bSearch = FALSE;
					g_StringLanType(szLan, "由于服务器忙,或网络故障,搜索日志异常终止!",\
						"Since the server is busy, or network failure, search the log abnormal termination");
					AfxMessageBox(szLan);
					break;
				}
			}
		}
		else
		{
			bRet = NET_DVR_FindNextLog_V30(pLogSearch->m_lLogHandle, &struFileInfo);
			if (bRet == NET_DVR_FILE_SUCCESS)
			{
				csTmp.Format("%d", pLogSearch->m_lLogNum+1);
				pLogSearch->m_listDeviceLog.InsertItem(pLogSearch->m_lLogNum, csTmp,0);
				csTmp.Format("%04d-%02d-%02d %02d:%02d:%02d", struFileInfo.strLogTime.dwYear, struFileInfo.strLogTime.dwMonth, struFileInfo.strLogTime.dwDay, \
					struFileInfo.strLogTime.dwHour,struFileInfo.strLogTime.dwMinute, struFileInfo.strLogTime.dwSecond);
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 1, csTmp);	
				switch(struFileInfo.dwMajorType)
				{
				case 1:
					g_StringLanType(szLan, "报警", "Alarm");
					csTmp.Format("%s",szLan);
					break;
				case 2:
					g_StringLanType(szLan, "异常", "Abnormal");
					csTmp.Format("%s",szLan);
					break;
				case 3:
					g_StringLanType(szLan, "操作", "Operation");
					csTmp.Format("%s",szLan);
					break;
				case 4:
					g_StringLanType(szLan, "附加信息", "Additional information");
					csTmp.Format("%s",szLan);
					break;
				default:
					g_StringLanType(szLan, "未知类型", "Unknown");
					csTmp.Format("%s",szLan);
					break;
				}
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 2, csTmp);
				
				if (MAJOR_INFORMATION == struFileInfo.dwMajorType)
				{
					InfoMinorTypeMap(struFileInfo, csTmp);
				}
				else if(MAJOR_OPERATION == struFileInfo.dwMajorType)
				{
					OperationMinorTypeMap(struFileInfo, csTmp);
				}
				else if(MAJOR_ALARM == struFileInfo.dwMajorType)
				{
					AlarmMinorTypeMap(struFileInfo, csTmp);
				}
				else if(MAJOR_EXCEPTION == struFileInfo.dwMajorType)
				{
					ExceptionMinorTypeMap(struFileInfo, csTmp);
				}
				
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 3, csTmp);
				
				memset(szNetUser,0,17);
				memset(szPanelUser,0,17);
				memcpy(szNetUser, struFileInfo.sNetUser,MAX_NAMELEN);
				memcpy(szPanelUser, struFileInfo.sPanelUser, MAX_NAMELEN);
				memset(szLan, 0, 128);
				sprintf(szLan, "[%s-%s]", szNetUser, szPanelUser);
				if (struFileInfo.dwChannel > 0)
				{
					sprintf(szLan, "%schan[%d]", szLan, struFileInfo.dwChannel);
				}
				
				if (struFileInfo.dwAlarmInPort != 0)
				{
					sprintf(szLan, "%sAlarmIn[%d]", szLan, struFileInfo.dwAlarmInPort);
				}
				
				if (struFileInfo.dwAlarmOutPort != 0)
				{
					sprintf(szLan, "%sAlarmOut[%d]", szLan, struFileInfo.dwAlarmOutPort);
				}
				
				if (pLogSearch->ValidIPv6(struFileInfo.struRemoteHostAddr.byIPv6))
				{
					csTmp.Format("%s-%s",struFileInfo.struRemoteHostAddr.byIPv6, szLan);
				}
				else
				{
					csTmp.Format("%s-%s",struFileInfo.struRemoteHostAddr.sIpV4, szLan);
				}

				//csTmp.Format("%s-%s",struFileInfo.struRemoteHostAddr.sIpV4, szLan);
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 4, csTmp);
				
				if (struFileInfo.dwInfoLen > 0)
				{
					csTmp.Format("%s",struFileInfo.sInfo);
				}
				else
				{
					csTmp.Format("");
				}
				
				pLogSearch->m_listDeviceLog.SetItemText(pLogSearch->m_lLogNum, 5, csTmp);
				
				pLogSearch->m_lLogNum++;
			}
			else
			{
				if (bRet == NET_DVR_ISFINDING)
				{
					pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_SHOW);
					pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->SetWindowText("日志搜索中......");
					Sleep(5);
					continue;
				}
				if ((bRet == NET_DVR_NOMOREFILE) || (bRet == NET_DVR_FILE_NOFIND))
				{
					g_StringLanType(szLan, "搜索日志", "Search Log");
					pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
					pLogSearch->m_bSearch = FALSE;
					pLogSearch->GetDlgItem(IDC_STATIC_SEARCH)->ShowWindow(SW_HIDE);
					g_StringLanType(szLan, "搜索日志结束!", "Search log Ending");
					AfxMessageBox(szLan);
					break;
				}
				else
				{
					g_StringLanType(szLan, "搜索日志", "Search Log");
					pLogSearch->GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
					pLogSearch->m_bSearch = FALSE;
					g_StringLanType(szLan, "由于服务器忙,或网络故障,搜索日志异常终止!",\
						"Since the server is busy, or network failure, search the log abnormal termination");
					AfxMessageBox(szLan);
					break;
				}
			}
		}
	}
    CloseHandle(pLogSearch->m_hFileThread);
    pLogSearch->m_hFileThread = NULL;
    NET_DVR_FindLogClose_V30(pLogSearch->m_lLogHandle);
    return 0;
}

/*********************************************************
Function:	CDlgLogSearch
Desc:		Constructor
Input:	
Output:	
Return:	
**********************************************************/
IMPLEMENT_DYNAMIC(CDlgLogSearch, CDialog)
CDlgLogSearch::CDlgLogSearch(CWnd* pParent /*=NULL*/)
: CDialog(CDlgLogSearch::IDD, pParent)
, m_ctTimeStart(0)
, m_ctDateStart(0)
, m_ctDateStop(0)
, m_ctTimeStop(0)
, m_iDeviceIndex(-1)
, m_lLoginID(-1)
, m_hFileThread(NULL)
, m_iMode(0)
, m_iMajType(0)
, m_iMinType(0)
, m_bSearch(FALSE)
, m_lLogHandle(-1)
, m_lLogNum(0)
, m_bChkSmart(FALSE)
{
}
/*********************************************************
Function:	~CDlgLogSearch
Desc:		destructor
Input:	
Output:	
Return:	
**********************************************************/
CDlgLogSearch::~CDlgLogSearch()
{
    if (m_hFileThread != NULL)
    {
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
    }
}

/*********************************************************
Function:	DoDataExchange
Desc:		the map between control and variable
Input:	
Output:	
Return:	
**********************************************************/
void CDlgLogSearch::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgLogSearch)
	DDX_Control(pDX, IDC_COMBO_SEQUENCE, m_comboSequence);
    DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
    DDX_Control(pDX, IDC_COMBO_MAJOR_TYPE, m_comboMajorType);
    DDX_Control(pDX, IDC_COMBO_MINOR_TYPE, m_comboMinorType);
    DDX_DateTimeCtrl(pDX, IDC_TIME_LOG_START, m_ctTimeStart);
    DDX_DateTimeCtrl(pDX, IDC_DATE_LOG_START, m_ctDateStart);
    DDX_DateTimeCtrl(pDX, IDC_DATE_LOG_STOP, m_ctDateStop);
    DDX_DateTimeCtrl(pDX, IDC_TIME_LOG_STOP, m_ctTimeStop);
    DDX_Control(pDX, IDC_LIST_DEVICE_LOG, m_listDeviceLog);
    DDX_Check(pDX, IDC_CHK_SMART, m_bChkSmart);
	DDX_Check(pDX, IDC_CHECK_VIDEOPLAT, m_bVideoPlatLog);
	//}}AFX_DATA_MAP
}

/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CDlgLogSearch, CDialog)
//{{AFX_MSG_MAP(CDlgLogSearch)	
ON_BN_CLICKED(IDC_BTN_SEARCH, OnBnClickedBtnSearch)
ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEVICE_LOG, OnDblclkListDeviceLog)
ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
ON_CBN_SELCHANGE(IDC_COMBO_MINOR_TYPE, OnSelchangeComboMinorType)
	ON_BN_CLICKED(IDC_CHECK_VIDEOPLAT, OnCheckVideoplat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CDlgLogSearch message handlers
/*********************************************************
Function:	OnInitDialog
Desc:		Initialize the dialog
Input:	
Output:	
Return:	
**********************************************************/
BOOL CDlgLogSearch::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    CenterWindow();
    m_bSearch = FALSE;
    m_hFileThread = NULL;
    
    pLogSearch = this;
    char szLan[128] = {0};
    
	CString tmp;
    tmp.Format("全部日志");
    m_comboSequence.InsertString(0, tmp);
    tmp.Format("78K");
    m_comboSequence.InsertString(1, tmp);
    for (int i = 0; i < 80; i++)
    {
        tmp.Format("Slot:%d", i + 1);
        m_comboSequence.InsertString(i + 2, tmp);
    }

    m_listDeviceLog.SetExtendedStyle(m_listDeviceLog.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES);
    g_StringLanType(szLan, "序号", "No.");
    m_listDeviceLog.InsertColumn(0, szLan, LVCFMT_RIGHT, 60, -1);
    g_StringLanType(szLan, "日志时间", "Log Time");
    m_listDeviceLog.InsertColumn(1, szLan, LVCFMT_LEFT, 140, -1);
    g_StringLanType(szLan, "主类型", "Major Type");
    m_listDeviceLog.InsertColumn(2, szLan, LVCFMT_LEFT,80, -1);
    g_StringLanType(szLan, "次类型", "Minor Type");
    m_listDeviceLog.InsertColumn(3, szLan, LVCFMT_LEFT, 150, -1);
    g_StringLanType(szLan, "远程主机", "Remote Host");
    m_listDeviceLog.InsertColumn(4, szLan, LVCFMT_LEFT, 100, -1);
    g_StringLanType(szLan, "描述", "Description");
    m_listDeviceLog.InsertColumn(5, szLan, LVCFMT_LEFT, 251, -1);
    
    m_lLogNum = 0;
    m_lLogHandle = -1;	
    CTime ctCurTime = CTime::GetCurrentTime();
    CTime time(ctCurTime.GetYear(),ctCurTime.GetMonth(),ctCurTime.GetDay(),0,0,0);
    CTime time0(ctCurTime.GetYear(),ctCurTime.GetMonth(),ctCurTime.GetDay(),23,59,59);
    m_ctDateStart = time;
    m_ctTimeStart = time;
    m_ctDateStop = ctCurTime;
    m_ctTimeStop = time0;
    m_iMode = 2;
    m_iMajType = 0;
    m_iMinType = 0;
    m_comboMode.SetCurSel(m_iMode);
    m_comboMajorType.SetCurSel(m_iMajType);
    m_comboMinorType.SetCurSel(m_iMinType);
    AddMinorString();
    CheckInitParam();
    UpdateData(FALSE); 
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

inline BOOL CDlgLogSearch::ValidIPv6(BYTE *ip)
{
	if (strlen((char *)ip) > 2 && strchr((char *)ip, ':') != NULL)
	{
		return TRUE;
	}
	return FALSE;
}

/*********************************************************
Function:	CheckInitParam
Desc:		check current device parameters, and initialize log search box corresponding parameters
Input:	
Output:	
Return:	TRUE,correct parameters; FALSE,wrong parameters;
**********************************************************/
BOOL CDlgLogSearch::CheckInitParam()
{
    int iDeviceIndex = g_pMainDlg->GetCurDeviceIndex();
    EnableCtrl(TRUE);
    if (iDeviceIndex == -1)
    {
        m_lLoginID = -1;
        EnableCtrl(FALSE);
        return FALSE;
    }
    
    if (m_iDeviceIndex == iDeviceIndex)
    {
        return TRUE;
    }
    else
    {
        m_iDeviceIndex = iDeviceIndex;
        m_listDeviceLog.DeleteAllItems();
    }
    
    m_lLoginID = g_struDeviceInfo[m_iDeviceIndex].lLoginID;
    
    if (m_lLoginID < 0)
    {
        EnableCtrl(FALSE);
        return FALSE;
    }
    GetDlgItem(IDC_STATIC_DEVNAME)->SetWindowText(g_struDeviceInfo[m_iDeviceIndex].chLocalNodeName);
    GetDlgItem(IDC_STATIC_DEV_STATE_IP)->SetWindowText(g_struDeviceInfo[m_iDeviceIndex].chDeviceIP);
    EnableWindow(TRUE);
    return TRUE;
}

/*********************************************************
Function:	AddMinorString
Desc:		add the minor type to combo
Input:	none
Output:	none
Return:	none
**********************************************************/
void CDlgLogSearch::AddMinorString()
{
    CString szTemp;
    int iSel = 0;
    m_comboMinorType.ResetContent();
    m_comboMinorType.AddString("All");
    m_comboMinorType.SetItemData(iSel, 0);
    m_comboMinorType.SetCurSel(0);
    
    szTemp.Format("ALARM_IN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ALARM_IN);
    
    szTemp.Format("ALARM_OUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ALARM_OUT);
    
    szTemp.Format("MOTDET_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MOTDET_START);
    
    szTemp.Format("MOTDET_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_MOTDET_STOP);
    
    szTemp.Format("HIDE_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HIDE_ALARM_START);
    
    szTemp.Format("HIDE_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HIDE_ALARM_STOP);
    
    szTemp.Format("VCA_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_START);
    
    szTemp.Format("VCA_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_STOP);

	//场景变更侦测日志查询2013-07-17
	szTemp.Format("VCA_SECNECHANGE_DETECTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_SECNECHANGE_DETECTION);

	szTemp.Format("WIRELESS_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_WIRELESS_ALARM_START);
	
	szTemp.Format("WIRELESS_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_WIRELESS_ALARM_STOP);
	
	szTemp.Format("PIR_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PIR_ALARM_START);
	
	szTemp.Format("PIR_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_PIR_ALARM_STOP);
	
	szTemp.Format("CALLHELP_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CALLHELP_ALARM_START);
	
	szTemp.Format("CALLHELP_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_CALLHELP_ALARM_STOP);
    
	szTemp.Format("DETECTFACE_ALARM_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DETECTFACE_ALARM_START);
	
	szTemp.Format("DETECTFACE_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DETECTFACE_ALARM_STOP);

	szTemp.Format("DETECTFACE_ALARM_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DETECTFACE_ALARM_STOP);

#ifdef NET_DVR_SET_TRAVERSE_PLANE_DETECTION
	szTemp.Format("TRAVERSE_PLANE_DETECTION start");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_LINE_DETECTION_BEGIN );
	
	szTemp.Format("TRAVERSE_PLANE_DETECTION end");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_LINE_DETECTION_END);
#endif //NET_DVR_SET_TRAVERSE_PLANE_DETECTION
	
	szTemp.Format("field start");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_INTRUDE_BEGIN);
	
	szTemp.Format("field end");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_INTRUDE_END);
	
	szTemp.Format("Audio Exception");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_AUDIOINPUT);
	
	szTemp.Format("Voice ABNORMAL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_AUDIOABNORMAL);
	
	szTemp.Format("defous detection start");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_DEFOCUS_DETECTION_BEGIN);
	
	szTemp.Format("defous detection end");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VCA_DEFOCUS_DETECTION_END);

	szTemp.Format("external alarm");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_EXT_ALARM);

	szTemp.Format("ALARM_VCA_LECTURE_DETECTION_BEGIN");//2014-03-21
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_VCA_LECTURE_DETECTION_BEGIN);
	
	szTemp.Format("ALARM_VCA_LECTURE_DETECTION_END");//2014-03-21
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_VCA_LECTURE_DETECTION_END);
	
	szTemp.Format("ALARM_AUDIOSTEEPDROP");//2014-03-21
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_VCA_ALARM_AUDIOSTEEPDROP);
	
	szTemp.Format("ALARM_VCA_ANSWER_DETECTION_BEGIN");//2014-03-21
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_VCA_ANSWER_DETECTION_BEGIN);
	
	szTemp.Format("ALARM_VCA_ANSWER_DETECTION_END");//2014-03-21
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_VCA_ANSWER_DETECTION_END);

    
    //exception
    szTemp.Format("---Excp----------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);
    
	szTemp.Format("RAID_ERROR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RAID_ERROR);

    szTemp.Format("VI_LOST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VI_LOST);
    
    szTemp.Format("ILLEGAL_ACCESS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_ILLEGAL_ACCESS);
    
    szTemp.Format("HD_FULL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HD_FULL);
    
    szTemp.Format("HD_ERROR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HD_ERROR);
    
    szTemp.Format("REC_ERROR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_ERROR);
    
    szTemp.Format("DCD_LOST");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_DCD_LOST);
    
    szTemp.Format("IPC_NO_LINK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_NO_LINK);
    
    szTemp.Format("IP_CONFLICT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IP_CONFLICT);
    
    szTemp.Format("NET_BROKEN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_NET_BROKEN);
    
    szTemp.Format("VI_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_VI_EXCEPTION);

    szTemp.Format("IPC_IP_CONFLICT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_IP_CONFLICT);

    szTemp.Format("SENCE_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SENCE_EXCEPTION);

	szTemp.Format("RESOLUTION_MISMATCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RESOLUTION_MISMATCH);

	szTemp.Format("RECORD_OVERFLOW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RECORD_OVERFLOW);
    
    szTemp.Format("POE_POWER_EXCEPTION");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_POE_POWER_EXCEPTION);
    
    //operation
    szTemp.Format("%s", "---Oper----------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);
    szTemp.Format("START_DVR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_START_DVR);
    
    szTemp.Format("STOP_DVR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_DVR);
    
    szTemp.Format("STOP_ABNORMAL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_ABNORMAL);
    
    szTemp.Format("REBOOT_DVR");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REBOOT_DVR);
    
    szTemp.Format("LOCAL_LOGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOGIN);
    
    szTemp.Format("%s", "LOCAL_LOGOUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOGOUT);
    
    szTemp.Format("LOCAL_CFG_PARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFG_PARM);
    
    szTemp.Format("%s","LOCAL_PLAYBYFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PLAYBYFILE);
    
    szTemp.Format("%s", "LOCAL_PLAYBYTIME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PLAYBYTIME);
    
    szTemp.Format("%s", "LOCAL_START_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_START_REC);
    
    szTemp.Format("%s", "LOCAL_STOP_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_STOP_REC);
    
    szTemp.Format("%s", "LOCAL_PTZCTRL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PTZCTRL);
    
    szTemp.Format("%s", "LOCAL_PREVIEW");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PREVIEW);
    
    szTemp.Format("%s", "LOCAL_MODIFY_TIME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MODIFY_TIME);
    
    szTemp.Format("%s", "LOCAL_UPGRADE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_UPGRADE);
    
    szTemp.Format("%s", "LOCAL_COPYFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COPYFILE);
    
    
    szTemp.Format("%s", "LOCAL_LOCKFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOCKFILE);
    
    szTemp.Format("%s", "LOCAL_UNLOCKFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_UNLOCKFILE);
    
    szTemp.Format("%s", "LOCAL_FORMAT_HDD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FORMAT_HDD);
    
    szTemp.Format("%s", "LOCAL_CFGFILE_OUTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFGFILE_OUTPUT);
    
    szTemp.Format("%s", "LOCAL_CFGFILE_INPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_CFGFILE_INPUT);
    
    szTemp.Format("%s", "LOCAL_DVR_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DVR_ALARM);
    szTemp.Format("%s", "IPC_ADD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_ADD);
    
    szTemp.Format("%s", "IPC_DEL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_DEL);
    
    szTemp.Format("%s", "IPC_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_IPC_SET);
    
    m_comboMinorType.AddString("LOCAL_START_BACKUP");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_START_BACKUP);
    m_comboMinorType.AddString("LOCAL_STOP_BACKUP");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_STOP_BACKUP);
    m_comboMinorType.AddString("LOCAL_COPYFILE_START_TIME");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COPYFILE_START_TIME);
    m_comboMinorType.AddString("LOCAL_COPYFILE_END_TIME");
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COPYFILE_END_TIME);
	//m_comboMinorType.AddString("LOCAL_COPYFILE_END_TIME");
    
	szTemp.Format("%s", "LOCAL_OPERATE_LOCK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_OPERATE_LOCK );
	
	szTemp.Format("%s", "LOCAL_OPERATE_UNLOCK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_OPERATE_UNLOCK );	
    
	szTemp.Format("%s", "LOCAL_TAG_OPT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_TAG_OPT);
	
	szTemp.Format("%s", "LOCAL_VOUT_SWITCH");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_VOUT_SWITCH);
    
	szTemp.Format("%s", "STREAM_CABAC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STREAM_CABAC);
	
	szTemp.Format("%s", "LOCAL_LOAD_HDISK");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_LOAD_HDISK);
	
	szTemp.Format("%s", "LOCAL_DELETE_HDISK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DELETE_HDISK);

	szTemp.Format("%s", "LOCAL_MAIN_AUXILIARY_PORT_SWITCH");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MAIN_AUXILIARY_PORT_SWITCH);

	szTemp.Format("%s", "LOCAL_HARD_DISK_CHECK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_HARD_DISK_CHECK);

	szTemp.Format("%s", "LOCAL_PIN");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PIN);

	szTemp.Format("%s", "LOCAL_DIAL");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DIAL);

	//ITS的操作日志 bg
	szTemp.Format("%s", "LOCAL_ADD_CAR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_CAR_INFO);

	szTemp.Format("%s", "LOCAL_MOD_CAR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_CAR_INFO);

	szTemp.Format("%s", "LOCAL_DEL_CAR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_CAR_INFO);

	szTemp.Format("%s", "LOCAL_FIND_CAR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CAR_INFO);

	szTemp.Format("%s", "LOCAL_ADD_MONITOR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_MONITOR_INFO);

	szTemp.Format("%s", "LOCAL_MOD_MONITOR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_MONITOR_INFO);

	szTemp.Format("%s", "LOCAL_DEL_MONITOR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_MONITOR_INFO);

	szTemp.Format("%s", "LOCAL_FIND_MONITOR_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_MONITOR_INFO);

	szTemp.Format("%s", "LOCAL_FIND_NORMAL_PASS_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_NORMAL_PASS_INFO);

	szTemp.Format("%s", "LOCAL_FIND_ABNORMAL_PASS_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_ABNORMAL_PASS_INFO);

	szTemp.Format("%s", "LOCAL_FIND_PEDESTRIAN_PASS_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_PEDESTRIAN_PASS_INFO);

	szTemp.Format("%s", "LOCAL_PIC_PREVIEW");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_PIC_PREVIEW);

	szTemp.Format("%s", "LOCAL_SET_GATE_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_GATE_PARM_CFG);

	szTemp.Format("%s", "LOCAL_GET_GATE_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_GET_GATE_PARM_CFG);

	szTemp.Format("%s", "LOCAL_SET_DATAUPLOAD_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_DATAUPLOAD_PARM_CFG);

	szTemp.Format("%s", "LOCAL_GET_DATAUPLOAD_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_GET_DATAUPLOAD_PARM_CFG);

	//2013-11-19
	szTemp.Format("%s", "LOCAL_DEVICE_CONTROL");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEVICE_CONTROL);

	szTemp.Format("%s", "LOCAL_ADD_EXTERNAL_DEVICE_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_EXTERNAL_DEVICE_INFO);

	szTemp.Format("%s", "LOCAL_MOD_EXTERNAL_DEVICE_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_EXTERNAL_DEVICE_INFO);

	szTemp.Format("%s", "LOCAL_DEL_EXTERNAL_DEVICE_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_EXTERNAL_DEVICE_INFO);

	szTemp.Format("%s", "LOCAL_FIND_EXTERNAL_DEVICE_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_EXTERNAL_DEVICE_INFO);

	szTemp.Format("%s", "LOCAL_ADD_CHARGE_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_CHARGE_RULE);

	szTemp.Format("%s", "LOCAL_MOD_CHARGE_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_CHARGE_RULE);

	szTemp.Format("%s", "LOCAL_DEL_CHARGE_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_CHARGE_RULE);

	szTemp.Format("%s", "LOCAL_FIND_CHARGE_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CHARGE_RULE);

	szTemp.Format("%s", "LOCAL_COUNT_NORMAL_CURRENTINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_NORMAL_CURRENTINFO);

	szTemp.Format("%s", "LOCAL_EXPORT_NORMAL_CURRENTINFO_REPORT");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_NORMAL_CURRENTINFO_REPORT);

	szTemp.Format("%s", "LOCAL_COUNT_ABNORMAL_CURRENTINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_ABNORMAL_CURRENTINFO);

	szTemp.Format("%s", "LOCAL_EXPORT_ABNORMAL_CURRENTINFO_REPORT");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_ABNORMAL_CURRENTINFO_REPORT);

	szTemp.Format("%s", "LOCAL_COUNT_PEDESTRIAN_CURRENTINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_PEDESTRIAN_CURRENTINFO);

	szTemp.Format("%s", "LOCAL_EXPORT_PEDESTRIAN_CURRENTINFO_REPORT");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_PEDESTRIAN_CURRENTINFO_REPORT);

	szTemp.Format("%s", "LOCAL_FIND_CAR_CHARGEINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CAR_CHARGEINFO);

	szTemp.Format("%s", "LOCAL_COUNT_CAR_CHARGEINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_COUNT_CAR_CHARGEINFO);

	szTemp.Format("%s", "LOCAL_EXPORT_CAR_CHARGEINFO_REPORT");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_EXPORT_CAR_CHARGEINFO_REPORT);
	
	szTemp.Format("%s", "LOCAL_FIND_SHIFTINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_SHIFTINFO);

	szTemp.Format("%s", "LOCAL_FIND_CARDINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_CARDINFO);

	szTemp.Format("%s", "LOCAL_ADD_RELIEF_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_ADD_RELIEF_RULE);

	szTemp.Format("%s", "LOCAL_MOD_RELIEF_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MOD_RELIEF_RULE);

	szTemp.Format("%s", "LOCAL_DEL_RELIEF_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_RELIEF_RULE);
	
	szTemp.Format("%s", "LOCAL_FIND_RELIEF_RULE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_FIND_RELIEF_RULE);

	szTemp.Format("%s", "LOCAL_GET_ENDETCFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_GET_ENDETCFG);

	szTemp.Format("%s", "LOCAL_SET_ENDETCFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_ENDETCFG);

	szTemp.Format("%s", "LOCAL_SET_ENDEV_ISSUEDDATA");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_SET_ENDEV_ISSUEDDATA);

	szTemp.Format("%s", "LOCAL_DEL_ENDEV_ISSUEDDATA");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DEL_ENDEV_ISSUEDDATA);
	//ITS的操作日志 ed

    ////remote 
    szTemp.Format("%s", "---Remote----------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);
    
    szTemp.Format("%s", "REMOTE_LOGIN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOGIN);
    
    szTemp.Format("%s", "REMOTE_LOGOUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOGOUT);
    
    szTemp.Format("%s", "REMOTE_START_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_START_REC);
    
    szTemp.Format("%s","REMOTE_STOP_REC");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_STOP_REC);
    
    szTemp.Format("%s", "START_TRANS_CHAN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_START_TRANS_CHAN);
    
    szTemp.Format("%s","STOP_TRANS_CHAN");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_TRANS_CHAN);
    
    szTemp.Format("REMOTE_GET_PARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_PARM);
    
    szTemp.Format("REMOTE_CFG_PARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFG_PARM);
    
    szTemp.Format("%s", "REMOTE_GET_STATUS");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_STATUS);
    
    szTemp.Format("%s", "REMOTE_ARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ARM);
    
    szTemp.Format("%s", "REMOTE_DISARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DISARM);
    
    szTemp.Format("%s", "REMOTE_REBOOT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_REBOOT);
    
    szTemp.Format("%s", "START_VT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_START_VT);
    
    szTemp.Format("%s", "STOP_VT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_STOP_VT);
    
    szTemp.Format("%s", "REMOTE_UPGRADE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_UPGRADE);
    
    szTemp.Format("%s", "REMOTE_PLAYBYFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PLAYBYFILE);
    
    szTemp.Format("%s", "REMOTE_PLAYBYTIME");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PLAYBYTIME);
    
    szTemp.Format("%s", "REMOTE_PTZCTRL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PTZCTRL);
    
    szTemp.Format("%s", "REMOTE_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_STOP);
    
    
    szTemp.Format("%s", "REMOTE_LOCKFILE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOCKFILE);
    
    szTemp.Format("%s", "REMOTE_CFGFILE_OUTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFGFILE_OUTPUT);
    
    
    szTemp.Format("%s", "REMOTE_CFGFILE_INTPUT");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_CFGFILE_INTPUT);
    
    szTemp.Format("%s", "REMOTE_FORMAT_HDD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_FORMAT_HDD);
    szTemp.Format("%s", "REMOTE_DVR_ALARM");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DVR_ALARM);
    szTemp.Format("%s", "REMOTE_IPC_ADD");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IPC_ADD);    
    
    szTemp.Format("%s", "REMOTE_IPC_DEL");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IPC_DEL);
    
    szTemp.Format("%s", "REMOTE_IPC_SET");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_IPC_SET);
    
    szTemp.Format("%s", "REBOOT_VCA_LIB");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REBOOT_VCA_LIB);
	
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_DELETE_HDISK);

	szTemp.Format("%s", "LOCAL_MAIN_AUXILIARY_PORT_SWITCH");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_MAIN_AUXILIARY_PORT_SWITCH);
	
	szTemp.Format("%s", "LOCAL_HARD_DISK_CHECK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_LOCAL_HARD_DISK_CHECK);

	szTemp.Format("%s", "REMOTE_DELETE_HDISK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DELETE_HDISK);
	
	szTemp.Format("%s", "REMOTE_LOAD_HDISK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_LOAD_HDISK  );
	
	szTemp.Format("%s", "REMOTE_UNLOAD_HDISK");
    m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_UNLOAD_HDISK );

	szTemp.Format("%s", "SMS_CONTROL");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_SMS_CONTROL);

	szTemp.Format("%s", "CALL_ONLINE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_CALL_ONLINE);

	szTemp.Format("%s", "REMOTE_PIN");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_PIN);
    
	//ITS操作日志类型 Remote bg
	szTemp.Format("%s", "REMOTE_DEVICE_CONTROL");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DEVICE_CONTROL);

	szTemp.Format("%s", "REMOTE_SET_GATE_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_GATE_PARM_CFG);

	szTemp.Format("%s", "REMOTE_GET_GATE_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_GATE_PARM_CFG);

	szTemp.Format("%s", "REMOTE_SET_DATAUPLOAD_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_DATAUPLOAD_PARM_CFG);

	szTemp.Format("%s", "REMOTE_GET_DATAUPLOAD_PARM_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_DATAUPLOAD_PARM_CFG);

	szTemp.Format("%s", "REMOTE_GET_BASE_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_BASE_INFO);

	szTemp.Format("%s", "REMOTE_GET_OVERLAP_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_OVERLAP_CFG);

	szTemp.Format("%s", "REMOTE_SET_OVERLAP_CFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_OVERLAP_CFG);

	szTemp.Format("%s", "REMOTE_GET_ROAD_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ROAD_INFO);

	szTemp.Format("%s", "REMOTE_START_TRANSCHAN");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_START_TRANSCHAN);

	szTemp.Format("%s", "REMOTE_GET_ECTWORKSTATE");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ECTWORKSTATE);

	szTemp.Format("%s", "REMOTE_GET_ECTCHANINFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ECTCHANINFO);

	//2013-11-19
	szTemp.Format("%s", "REMOTE_ADD_EXTERNAL_DEVICE_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ADD_EXTERNAL_DEVICE_INFO);
	
	szTemp.Format("%s", "REMOTE_MOD_EXTERNAL_DEVICE_INFO");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_MOD_EXTERNAL_DEVICE_INFO);
	
	szTemp.Format("%s", "REMOTE_GET_ENDETCFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_GET_ENDETCFG);
	
	szTemp.Format("%s", "REMOTE_SET_ENDETCFG");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_SET_ENDETCFG);
	
	szTemp.Format("%s", "REMOTE_ENDEV_ISSUEDDATA");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ENDEV_ISSUEDDATA);
	
	szTemp.Format("%s", "REMOTE_DEL_ENDEV_ISSUEDDATA");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_DEL_ENDEV_ISSUEDDATA);

	//日志类型（新增）2013-06-09
	szTemp.Format("%s", "REMOTE_ON_CTRL_LAMP");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_ON_CTRL_LAMP);
	
	szTemp.Format("%s", "REMOTE_OFF_CTRL_LAMP");
	m_comboMinorType.AddString(szTemp);
	iSel++;
	m_comboMinorType.SetItemData(iSel, MINOR_REMOTE_OFF_CTRL_LAMP );
	//ITS操作日志类型 Remote ed


    //information
    szTemp.Format("%s", "--INFOR-------------");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, 0);
    
    szTemp.Format("%s", "HDD_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_HDD_INFO);
    
    szTemp.Format("%s", "SMART_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_SMART_INFO);
    
    szTemp.Format("%s", "REC_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_START);
    
    szTemp.Format("%s", "REC_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_STOP);
    
    szTemp.Format("%s", "REC_OVERDUE");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_REC_OVERDUE);
    
    szTemp.Format("%s", "LINK_START");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LINK_START);
    
    szTemp.Format("%s", "LINK_STOP");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_LINK_STOP);
    
    szTemp.Format("%s", "NET_DISK_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_NET_DISK_INFO);

    szTemp.Format("%s", "RAID_INFO");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RAID_INFO);

	szTemp.Format("%s", "RUN_STATUS_INFO ");
    m_comboMinorType.AddString(szTemp);
    iSel++;
    m_comboMinorType.SetItemData(iSel, MINOR_RUN_STATUS_INFO );
}
/*********************************************************
Function:	OnBnClickedBtnSearch
Desc:		search log
Input:	
Output:	
Return:	
**********************************************************/
void CDlgLogSearch::OnBnClickedBtnSearch()
{
    UpdateData(TRUE);
    char szLan[128] = {0};
    if (!m_bSearch)
    {
        NET_DVR_TIME StartTime;
        NET_DVR_TIME StopTime;
        
        m_iMode = m_comboMode.GetCurSel();
        m_iMajType = m_comboMajorType.GetCurSel();
        m_iMinType = m_comboMinorType.GetItemData(m_comboMinorType.GetCurSel());
        StartTime.dwYear = (WORD)m_ctDateStart.GetYear();
        StartTime.dwMonth = (WORD)m_ctDateStart.GetMonth();
        StartTime.dwDay = (WORD)m_ctDateStart.GetDay();
        StartTime.dwHour = (char)m_ctTimeStart.GetHour();
        StartTime.dwMinute = (char)m_ctTimeStart.GetMinute();
        StartTime.dwSecond = (char)m_ctTimeStart.GetSecond();
        
        StopTime.dwYear = (WORD)m_ctDateStop.GetYear();
        StopTime.dwMonth = (WORD)m_ctDateStop.GetMonth();
        StopTime.dwDay = (WORD)m_ctDateStop.GetDay();
        StopTime.dwHour = (char)m_ctTimeStop.GetHour();
        StopTime.dwMinute = (char)m_ctTimeStop.GetMinute();
        StopTime.dwSecond = (char)m_ctTimeStop.GetSecond();
        if (m_bVideoPlatLog)
        {
			NET_DVR_VEDIOPLATLOG struVedioPlat = {0};
			struVedioPlat.byDevSequence = m_comboSequence.GetCurSel();
			if (m_comboSequence.GetCurSel() == 0)
			{
				struVedioPlat.byDevSequence = 0xff;
			}
			else if (m_comboSequence.GetCurSel() == 1)
			{
				struVedioPlat.byDevSequence = 0xfe;
			}
			else
			{
				struVedioPlat.byDevSequence = m_comboSequence.GetCurSel() - 2;
			}
			
			struVedioPlat.bySearchCondition = 0;
			m_lLogHandle = NET_DVR_FindDVRLog_Matrix(m_lLoginID, m_iMode, m_iMajType, m_iMinType, &struVedioPlat, &StartTime, &StopTime);
		}
		else
		{
			m_lLogHandle = NET_DVR_FindDVRLog_V30(m_lLoginID, m_iMode, m_iMajType, m_iMinType, &StartTime, &StopTime, m_bChkSmart);
		}
		if (m_lLogHandle < 0)
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_FindDVRLog_V30");
            
            g_StringLanType(szLan, "搜索日志失败", "Search log failed");
            AfxMessageBox(szLan);
            return;
        }
        m_bSearch = TRUE;
        m_lLogNum = 0;
        m_listDeviceLog.DeleteAllItems();
        DWORD dwThreadId;
        if (m_hFileThread == NULL)
        {
            m_hFileThread = CreateThread(NULL,0,LPTHREAD_START_ROUTINE(GetLogFileThread),this,0,&dwThreadId);		
        }
        if (m_hFileThread == NULL)
        {
            g_StringLanType(szLan, "打开线程失败", "Open thread failed");
            AfxMessageBox(szLan);
            return;
        }
        g_StringLanType(szLan, "停止搜索", "Stop Search");
        GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
        m_bSearch = TRUE;
    }
    else
    {
        if (!NET_DVR_FindLogClose_V30(m_lLogHandle))
        {
            g_pMainDlg->AddLog(m_iDeviceIndex, OPERATION_FAIL_T, "NET_DVR_FindLogClose_V30");
        }
        m_lLogHandle = -1;
        g_StringLanType(szLan, "搜索日志", "Search Log");
        GetDlgItem(IDC_BTN_SEARCH)->SetWindowText(szLan);
        m_bSearch = FALSE;
        m_lLogNum = 0;
        if (m_hFileThread)
        {
            TerminateThread(m_hFileThread, 0);
            CloseHandle(m_hFileThread);
            m_hFileThread = NULL;
        }
        
        m_hFileThread = NULL;
    }
}

/*********************************************************
Function:	DoHide()
Desc:		release resource while hiding search interface
Input:	
Output:	
Return:	
**********************************************************/
void CDlgLogSearch::DoHide()
{
    m_listDeviceLog.DeleteAllItems();
    if (m_hFileThread)
    {
        TerminateThread(m_hFileThread, 0);
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
        NET_DVR_FindLogClose_V30(m_lLogHandle);
    }
    ShowWindow(SW_HIDE);
}

/*********************************************************
Function: OnDblclkListDeviceLog	
Desc:		show the detail information
Input:	none
Output:	none
Return:	none
**********************************************************/
void CDlgLogSearch::OnDblclkListDeviceLog(NMHDR* pNMHDR, LRESULT* pResult) 
{
    UNREFERENCED_PARAMETER(pNMHDR);
    //	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    int iItemSel = 0;
    char* pTxt = new char[11024];
    
    POSITION  iPos = m_listDeviceLog.GetFirstSelectedItemPosition();
    if (iPos == NULL)
    {
        return;
    }
    iItemSel = m_listDeviceLog.GetNextSelectedItem(iPos);//%s\t%s\tm_listAllLog.GetItemText(iItemSel,1),\m_listAllLog.GetItemText(iItemSel,2)
    //csTxt.Format("%s",m_listDeviceLog.GetItemText(iItemSel,5));
    m_listDeviceLog.GetItemText(iItemSel, 5, pTxt, 11024);
    MessageBox(pTxt);
    
    if (OpenClipboard())   
    {   
        HGLOBAL   clipbuffer;   
        char   *   buffer;   
        EmptyClipboard();   
        clipbuffer   =   GlobalAlloc(GMEM_DDESHARE,  11024);   
        buffer   =   (char*)GlobalLock(clipbuffer);   
        strcpy(buffer,   LPCSTR(pTxt));   
        GlobalUnlock(clipbuffer);   
        SetClipboardData(CF_TEXT,clipbuffer);   
        CloseClipboard();   
    } 
    delete[] pTxt; 
    *pResult = 0;	
}

void CDlgLogSearch::OnBtnExit() 
{
    // TODO: Add your control notification handler code here
    if (m_hFileThread)
    {
        TerminateThread(m_hFileThread, 0);
        CloseHandle(m_hFileThread);
        m_hFileThread = NULL;
        NET_DVR_FindLogClose_V30(m_lLogHandle);
    }
    CDialog::OnCancel();
}

/*********************************************************
Function:	EnableCtrl
Desc:		make the control enable or not
Input:	bEnable, TRUE/FALSE
Output:	none
Return:	none
**********************************************************/
void CDlgLogSearch::EnableCtrl(BOOL bEnable)
{
    GetDlgItem(IDC_BTN_SEARCH)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_MAJOR_TYPE)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_MINOR_TYPE)->EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_MODE)->EnableWindow(bEnable);
}

void CDlgLogSearch::OnSelchangeComboMinorType() 
{
    // TODO: Add your control notification handler code here
    DWORD dwIndex = 0;
    dwIndex = m_comboMinorType.GetItemData(m_comboMinorType.GetCurSel());
    TRACE("----MINOR INDEX key--- %x\n", dwIndex);
}

void CDlgLogSearch::OnCheckVideoplat() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_bVideoPlatLog)
	{
		GetDlgItem(IDC_STATIC_SEQUENCE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_SEQUENCE)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_SEQUENCE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_SEQUENCE)->ShowWindow(SW_HIDE);
	}
	UpdateData(FALSE);
}
