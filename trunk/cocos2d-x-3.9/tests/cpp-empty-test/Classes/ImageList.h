#ifndef __IMAGELIST_H__
#define __IMAGELIST_H__
#ifdef ROBOT_MAIN_CPP
#define EXTERN 
#else
#define EXTERN extern
#endif
#include "AppDefine.h"
#include "cocos2d.h"
using namespace cocos2d;

//#define ROBOT_DISABLE	//暫時關閉 ROBOT連線相關
//定義每個頁面的TAG
static const int TAG_UI_RobotRemote_Bk =10;
static const int TAG_UI_RobotScanning_Bk =11;
static const int TAG_UI_RobotMain_Bk =12;
static const int TAG_UI_RobotMap_Bk =13;
static const int TAG_UI_CleanJob_Bk =14;
static const int TAG_UI_Reservation_Bk =15;
static const int TAG_UI_RobotSetup_Bk =16;

static const int LAYER_UI_LOGO =10;
static const int LAYER_UI_Scanning =9;
static const int LAYER_UI_MAIN =9;

typedef Vector<Sprite* > SpriteVector;
typedef Vector<Label* > LabelVector;

#include "UI_commonFun.h"
#include "UI_RobotScanning.h"
#include "UI_RobotMain.h"
#include "UI_MAP.h"
#include "UI_RobotRemote.h"
#include "UI_Reservation.h"
#include "UI_CleanJob.h"
#include "UI_RobotSetup.h"
#include "Coco2dDialog.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//FOXME   逐步加入
#include "UI_SAMPLE.h"
#endif

//#define IMAGELIST_ROBOT_CONTROL "RobotAll.plist" //控制畫面
#define IMAGELIST_UPDATE_TIME_NUMBER "Image//JW_M_sk_bo_n3.png" //數字圖形 （更新倒數用）

//圖檔路徑
static const int IMAGELIST_M_Packet=2;
static const char IMAGELIST_M[IMAGELIST_M_Packet][128]=
{
	{"Image//RobotAll_356_634_0.plist"},
	{"Image//RobotAll_356_634_1.plist"}
};
static const int IMAGELIST_H_Packet=2;
static const char IMAGELIST_H[IMAGELIST_H_Packet][128]=
{
	{"Image//RobotAll_960_540_0.plist"},
	{"Image//RobotAll_960_540_1.plist"}
};
static const int IMAGELIST_XH_Packet=3;
static const char IMAGELIST_XH[IMAGELIST_XH_Packet][128]=
{
	{"Image//RobotAll_724_1286_0.plist"},
	{"Image//RobotAll_724_1286_1.plist"},
	{"Image//RobotAll_724_1286_2.plist"}
    
};
static const int IMAGELIST_XXH_Packet=6;
static const char IMAGELIST_XXH[IMAGELIST_XXH_Packet][128]=
{
	{"Image//RobotAll_1920_1080_0.plist"},
	{"Image//RobotAll_1920_1080_1.plist"},
	{"Image//RobotAll_1920_1080_2.plist"},
	{"Image//RobotAll_1920_1080_3.plist"},
	{"Image//RobotAll_1920_1080_4.plist"},
	{"Image//RobotAll_1920_1080_5.plist"}
	
};
static const int pIMAGELISTSize[4]=
{
	IMAGELIST_M_Packet,
	IMAGELIST_H_Packet,
	IMAGELIST_XH_Packet,
	IMAGELIST_XXH_Packet
};

static const char** pIMAGELIST[4]=
{
	( const char**)IMAGELIST_M,
	( const char**)IMAGELIST_H,
	( const char**)IMAGELIST_XH,
	( const char**)IMAGELIST_XXH
};
const int IMAGE_SIZE_TABLE_W[]=
{
	356,
	540,
	724,
	1080,	
};
const int IMAGE_SIZE_TABLE_H[]=
{
	634,
	960,
	1286,
	1920,	
};
enum _FRAME_SIZE_
{
	WINFRAME_M=0,
	WINFRAME_H,
	WINFRAME_XH,
	WINFRAME_XXH,
	WINFRAME_SIZE
};

//指定字型
static const char S_FONE[]="Arial";
//圖檔名稱
static const char S_LOGO_ICON[]			= "ic_launchscreen.png";
static const char S_LOGO_BKIMG[]			= "bg_launchscreen.png";

static const char S_TITAL_BKIMG[]		= "robot_action_bg.png";
static const char S_TITAL_BACK_ICON[]	="robot_action_ic_back.png";
static const char S_TITAL_FORWARD_ICON[]	="robot_connect_ic_next.png";

static const char S_STATE_SCANING_ICON[]	="voicecontrol_btn_speaking_working.png";
static const char S_ERROR_NODEVICE_ICON[]	="robot_fail_ic_nodevice.png";
static const char S_ERROR_CONNFAIL_ICON[]	="robot_dialog_ic_warn.png";
static const char S_WIFILIST_ICON[]	="robot_connect_ic_next.png";
//主畫面
static const char S_MAIN_BKIMG[]         = "robot_main_bg.png";
static const char S_MAIN_ROBOTICON[]	="robot_info_img_robot.png";
static const char S_MAIN_ALARM_ERROR_ICON[]	="robot_main_ic_error.png";
static const char S_MAIN_ALARM_WARN_ICON[]	="robot_main_ic_warn.png";
static const char S_MAIN_BTNBK[]	="robot_main_bg_line.png";
static const char S_MAIN_BTNBKONE[]	="robot_main_btn_press.png";
static const char S_MAIN_BTNSCHEDULED[]	="robot_main_ic_refresh.png";
static const char S_MAIN_BTNJOB[]	="robot_main_ic_job.png";
static const char S_MAIN_BTNREMOTE[]	="robot_main_ic_remote.png";
static const char S_MAIN_BTNMAP[]	="robot_main_ic_map.png";
static const char S_MAIN_BTNSETUP[]	="robot_main_ic_setting.png";

//通用背景
static const char S_ROBOT_BKIMG[]        = "robot_main_bg.png";
static const char S_BTN_BACKGROUNP[]   = "new_robot_btn_bg.png";
static const char S_BTN_BACKGROUNP2[]   = "new_robot_btn_bg2.png";
static const char S_CLEAN_MAP_BG[]   = "new_robot_map_bg.png";
static const char S_CLEAN_HELP_BG[]   = "new_robot_help_bg.png";
static const char S_RESERVATION_TIMESET_BK[]   = "new_robot_btn_TimeSet.png";
static const char S_MSGBOX_BK[]   = "new_robot_MsgBox.png";
static const char S_MSGBOX_BTN_BK[]   = "new_robot_MsgBox_btn.png";
static const char S_RESERVATION_SETTIME_BK[]   = "new_robot_reservation_set_time_bk.png";
static const char S_RESERVATION_SETTIME_BTN[]   = "new_robot_btn_TimeSet.png";
//控制畫面 
static const char S_ROBOT_BTN_UP_DEF[]   = "remote_btn_up_default.png";
static const char S_ROBOT_BTN_UP_PRE[]   = "remote_btn_up_pressed.png";
static const char S_ROBOT_BTN_GO_DEF[]   = "remote_btn_godock_default.png";
static const char S_ROBOT_BTN_GO_PRE[]   = "remote_btn_godock_pressed.png";
static const char S_ROBOT_BTN_RIGHT_DEF[]   = "remote_btn_right_default.png";
static const char S_ROBOT_BTN_RIGHT_PRE[]   = "remote_btn_right_pressed.png";
static const char S_ROBOT_BTN_LEFT_DEF[]   = "remote_btn_left_default.png";
static const char S_ROBOT_BTN_LEFT_PRE[]   = "remote_btn_left_pressed.png";
static const char S_ROBOT_BTN_STOP_DEF[]   = "remote_btn_stop_default.png";
static const char S_ROBOT_BTN_STOP_PRE[]   = "remote_btn_stop_pressed.png";

static const char S_ROBOT_BTN_AUTO_DEF[]   = "robot_btn_auto.png";
static const char S_ROBOT_BTN_AUTO_FOCUS[]   = "robot_btn_auto_focus.png";
static const char S_ROBOT_BTN_AUTO_PRE[]   = "robot_btn_auto_press.png";
static const char S_ROBOT_BTN_EXPLORE_DEF[]   = "robot_btn_explore.png";
static const char S_ROBOT_BTN_EXPLORE_FOCUS[]   = "robot_btn_explore_focus.png";
static const char S_ROBOT_BTN_EXPLORE_PRE[]   = "robot_btn_explore_press.png";
static const char S_ROBOT_BTN_SPIRAL_DEF[]   = "robot_btn_spiral.png";
static const char S_ROBOT_BTN_SPIRAL_FOCUS[]   = "robot_btn_spiral_focus.png";
static const char S_ROBOT_BTN_SPIRAL_PRE[]   = "robot_btn_spiral_press.png";
static const char S_ROBOT_BTN_WALL_DEF[]   = "robot_btn_wall.png";
static const char S_ROBOT_BTN_WALL_FOCUS[]   = "robot_btn_wall_focus.png";
static const char S_ROBOT_BTN_WALL_PRE[]   = "robot_btn_wall_press.png";
static const char S_ROBOT_BTN_VOICE_DEF[]   = "robot_btn_voicemsg.png";
static const char S_ROBOT_BTN_VOICE_FOCUS[]   = "robot_btn_voicemsg_focus.png";
static const char S_ROBOT_BTN_VOICE_PRE[]   = "robot_btn_voicemsg_press.png";
//Clean JOB
static const char S_CLEAN_BTN_CORNER[]   = "robot_ic_turo_clean_corner.png";
static const char S_CLEAN_BTN_MULTI[]   = "robot_ic_turo_multiple.png";
static const char S_CLEAN_BTN_MOVE[]   = "robot_ic_turo_target.png";
static const char S_CLEAN_BTN_AUTO[]   = "robot_ic_turo_automode.png";
static const char S_CLEAN_BTN_SPIRAL[]   = "robot_ic_turo_spiral.png";
static const char S_CLEAN_BTN_WALL[]   = "robot_ic_turo_wall.png";
static const char S_CLEAN_ICON_HELP[]   = "robot_action_ic_help.png";
static const char S_CLEAN_ZOOM_OUT_DEF[]   = "robot_map_btn_out.png";
static const char S_CLEAN_ZOOM_OUT_PREE[]   = "robot_map_btn_out_press.png";
static const char S_CLEAN_ZOOM_IN_DEF[]   = "robot_map_btn_zoom.png";
static const char S_CLEAN_ZOOM_IN_PREE[]   = "robot_map_btn_zoom_press.png";
static const char S_CLEAN_BTN_EDIT_OVAL_DEF[]   = "robot_map_stamp_corner_oval.png";
static const char S_CLEAN_BTN_EDIT_OVAL_PRESS[]   = "robot_map_stamp_corner_oval_press.png";
static const char S_CLEAN_BTN_EDIT_RECT_DEF[]   = "robot_map_stamp_corner_rect.png";
static const char S_CLEAN_BTN_EDIT_RECT_PRESS[]   = "robot_map_stamp_corner_rect_press.png";
static const char S_CLEAN_BTN_EDIT_MUTIOVAL_DEF[]   = "robot_map_stamp_multiple_oval.png";
static const char S_CLEAN_BTN_EDIT_MUTIOVAL_PRESS[]   = "robot_map_stamp_multiple_oval_press.png";
static const char S_CLEAN_BTN_EDIT_MUTIRECT_DEF[]   = "robot_map_stamp_multiple_rect.png";
static const char S_CLEAN_BTN_EDIT_MUTIRECT_PRESS[]   = "robot_map_stamp_multiple_rect_press.png";
static const char S_CLEAN_BTN_EDIT_SPIRAL1_DEF[]   = "robot_map_stamp_spiral1.png";
static const char S_CLEAN_BTN_EDIT_SPIRAL1_PRESS[]   = "robot_map_stamp_spiral1_press.png";
static const char S_CLEAN_BTN_EDIT_SPIRAL2_DEF[]   = "robot_map_stamp_spiral2.png";
static const char S_CLEAN_BTN_EDIT_SPIRAL2_PRESS[]   = "robot_map_stamp_spiral2_press.png";
static const char S_CLEAN_BTN_EDIT_TARGET1_DEF[]   = "robot_map_stamp_target1.png";
static const char S_CLEAN_BTN_EDIT_TARGET1_PRESS[]   = "robot_map_stamp_target1_press.png";
static const char S_CLEAN_BTN_EDIT_TARGET2_DEF[]   = "robot_map_stamp_target2.png";
static const char S_CLEAN_BTN_EDIT_TARGET2_PRESS[]   = "robot_map_stamp_target2_press.png";
static const char S_CLEAN_BTN_EDIT_WALL1_DEF[]   = "robot_map_stamp_wall1.png";
static const char S_CLEAN_BTN_EDIT_WALL1_PRESS[]   = "robot_map_stamp_wall1_press.png";
static const char S_CLEAN_BTN_EDIT_WALL2_DEF[]   = "robot_map_stamp_wall2.png";
static const char S_CLEAN_BTN_EDIT_WALL2_PRESS[]   = "robot_map_stamp_wall2_press.png";
static const char S_CLEAN_BTN_EDIT_DELETE_DEF[]   = "robot_map_btn_delete.png";
static const char S_CLEAN_BTN_EDIT_DELETE_PRESS[]   = "robot_map_btn_delete_press.png";
static const char S_CLEAN_BTN_EDIT_UNDO_DEF[]   = "robot_map_btn_undo.png";
static const char S_CLEAN_BTN_EDIT_UNDO_PRESS[]   = "robot_map_btn_undo_press.png";
static const char S_CLEAN_BTN_EDIT_WALL_DEF[]   = "robot_map_btn_wall.png";
static const char S_CLEAN_BTN_EDIT_WALL_PRESS[]   = "robot_map_btn_wall_press.png";
static const char S_CLEAN_BTN_EDIT_WALL_FOCUS[]   = "robot_map_btn_wall_focus.png";
static const char S_CLEAN_BTN_HELP_STAMP_CORNER[]   = "robot__map_img_stamp_corner.png";
static const char S_CLEAN_BTN_HELP_STAMP_MUTLI[]   = "robot__map_img_stamp_multiple.png";
static const char S_CLEAN_BTN_HELP_TARGET1[]   = "robot_ic_turo_target1.png";
static const char S_CLEAN_BTN_HELP_ARROW[]   = "robot_map_img_arrow.png";
static const char S_CLEAN_BTN_HELP_PEN[]   = "robot_map_img_pen.png";
static const char S_CLEAN_MOVETO_TAGGET[]   = "robot_ic_turo_target1.png";

static const char S_CLEAN_RANG_ROTATE_DEF[]	="robot_map_btn_rotate.png";
static const char S_CLEAN_RANG_ROTATE_PRESS[]	="robot_map_btn_rotate_press.png";
static const char S_CLEAN_RANG_HORIZONTAL_DEF[]	="robot_map_btn_stretch.png";
static const char S_CLEAN_RANG_HORIZONTAL_PRESS[]	="robot_map_btn_stretch_press.png";
static const char S_CLEAN_RANG_VERTICAL_DEF[]	="robot_map_btn_stretch_vertical.png";
static const char S_CLEAN_RANG_VERTICAL_PRESS[]	="robot_map_btn_stretch_vertical_press.png";

//map
static const char S_MAP_HISTORY_DEF[]	="robot_btn_path.png";
static const char S_MAP_HISTORY_PRESS[]	="robot_btn_path_press.png";
static const char S_MAP_HISTORY_FOCUS[]	="robot_btn_path_focus.png";
static const char S_MAP_RETURN_ZERO_DEF[]	="robot_btn_map.png";
static const char S_MAP_RETURN_ZERO_PRESS[]	="robot_btn_map_press.png";
static const char S_MAP_ROBOT[]	="robot_map_ic_robot.png";



//reservation 
static const char S_RESERVATION_BTN_SINGAL[]   = "robot_btn_single_appointment.png";
static const char S_RESERVATION_BTN_DAILY[]   = "robot_btn_daily_cleaning.png";
static const char S_RESERVATION_SWITH_OFF[]   = "robot_btn_off.png";
static const char S_RESERVATION_SWITH_ON[]   = "robot_btn_on.png";


//指定的背景顏色
static const int BTN_Refresh_Disable_COLOR=200;
static const int BTN_Refresh_Enable_COLOR_R=143;
static const int BTN_Refresh_Enable_COLOR_G=196;
static const int BTN_Refresh_Enable_COLOR_B=76;
//按鈕字串
static const char STR_BTN_Refresh[]   = "Refresh";
static const int STR_BTN_COLOR=50;
static const int STR_BTN_SIZE_M		= 19;
static const int STR_BTN_SIZE_H		= 28;
static const int STR_BTN_SIZE_XH	= 37;
static const int STR_BTN_SIZE_XXH	= 56;
static const char STR_BTN_Canel[]   = "Cancel";
static const char STR_BTN_Save[]   = "Save";
static const char STR_BTN_Retry[]   = "Retry";
static const char STR_BTN_Ok[]   = "OK";
static const char STR_BTN_Reservation[]   = "Reservation";
static const char STR_BTN_CleanJob[]   = "Clean Job";
static const char STR_BTN_RemoteControl[]   = "Remote Control";
static const char STR_BTN_Map[]   = "Map";
static const int STR_CLEANJOB_SIZE_M	= 18;
static const int STR_CLEANJOB_SIZE_H	= 30;
static const int STR_CLEANJOB_SIZE_XH	= 40;
static const int STR_CLEANJOB_SIZE_XXH	= 60;
static const char STR_BTN_Cornercleaningjob[]   = "Corner cleaning job";
static const char STR_BTN_Multiroomsjob[]   = "Multi-rooms job";
static const char STR_BTN_Movetotargetjob[]   = "Move to target job";
static const char STR_BTN_Automode[]   = "Auto mode";
static const char STR_BTN_Spiral[]   = "Spiral";
static const char STR_BTN_Wallfollowing[]   = "Wall following";
static const char STR_BTN_SingleAppointment[]   = "Single Appointment";
static const char STR_BTN_Dailycleaning[]   = "Daily cleaning";
//常用字串
static const char STR_TITAL_FunRobot[]   = "FunRobot APP";
static const char STR_TITAL_RemoteControl[]   = "Remote Control";
static const char STR_TITAL_RobotInfomation[]   = "Robot Infomation";
static const char STR_TITAL_CoverageMap[]   = "Coverage Map";
static const char STR_TITAL_CleanJob[]   = "Job";
static const char STR_TITAL_Reservation[]   = "Reservation";
static const char STR_TITAL_Setting[]   = "Setting";
static const char STR_TITAL_EditCleanJob[]   = "Edit Clean Job";

static const int STR_TITAL_SIZE_M	= 27;
static const int STR_TITAL_SIZE_H	= 40;
static const int STR_TITAL_SIZE_XH	= 53;
static const int STR_TITAL_SIZE_XXH	= 80;
static const int STR_TITAL_COLOR_R=0x77;
static const int STR_TITAL_COLOR_G=0xB3;
static const int STR_TITAL_COLOR_B=0x20;

static const char STR_ERR_NODevice[]   = "NO Device";
static const int STR_ERROR_SIZE_M   = 27;
static const int STR_ERROR_SIZE_H   = 40;
static const int STR_ERROR_SIZE_XH  = 53;
static const int STR_ERROR_SIZE_XXH = 80;
static const int STR_ERROR_COLOR_R=62;
static const int STR_ERROR_COLOR_G=58;
static const int STR_ERROR_COLOR_B=57;
static const char STR_ERR_ConnFail[]   = "Connection Failed";
static const int STR_ERROR2_COLOR_R=233;
static const int STR_ERROR2_COLOR_G=98;
static const int STR_ERROR2_COLOR_B=124;
//錯誤說明 日後須將訊息移動到另外一檔案 方可支援多語言
//CONNECT_Fail_NoAP,
//CONNECT_Fail_NoIP,
//CONNECT_Fail_NoRobotAck,
//CONNECT_Fail_NoProxy,
static const char STR_ERR_ConnFail_NoAP_ENG[]   = "Can't find AP";
static const char STR_ERR_ConnFail_NoIP_ENG[]   = "No IP";
static const char STR_ERR_ConnFail_NoRobotAck_ENG[]   = "Robot No ACK";
static const char STR_ERR_ConnFail_NoProxy_ENG[]   = "Create proxy fail";
static const char STR_ERR_ConnFail_RobotNameRegFail_ENG[]   = "Robot name fail to get";
static const char STR_ERR_ConnFail_NoRobotTime_ENG[] ="Robot time Synchronize fail ";
static const char STR_ERR_ConnFail_MAPINIT_ENG[]   = "MAP INIT ERROR! must close programe";
static const char STR_ERR_ConnFail_MAPUPDATE_ENG[]   = "MAP Lost link! must close programe";
static const char STR_ERR_ConnFail_NoRobotPOWER_ENG[] ="Robot Power Unknow";
static const char STR_MSG_TimeSynchronize_ENG[] ="Time Synchronize";

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//日後需要將多語系移到另外檔案
static const char STR_ERR_ConnFail_NoAP_CHT[]   = "找尋不到AP";
static const char STR_ERR_ConnFail_NoIP_CHT[]   = "沒有取得IP";
static const char STR_ERR_ConnFail_NoRobotAck_CHT[]   = "機器人沒有回應";
static const char STR_ERR_ConnFail_NoProxy_CHT[]   = "伺服器建立失敗";
static const char STR_ERR_ConnFail_NoRobotTime_CHT[] ="無法取得機器時間";
static const char STR_ERR_ConnFail_RobotNameRegFail_CHT[]   = "機台名稱取得失敗!須重開";
static const char STR_ERR_ConnFail_MAPINIT_CHT[]   = "地圖初始失敗 程式重新啟動";
static const char STR_ERR_ConnFail_NoRobotPOWER_CHT[] ="無法取得機台電量";
static const char STR_ERR_ConnFail_MAPUPDATE_CHT[]   = "地圖無法更新!請重開";
static const char STR_MSG_TimeSynchronize_CHT[] ="時間同步";

#endif
static const char STR_STATE_Scaning[]   = "Scaning";
static const int STR_STATE_SIZE_M   = 24;
static const int STR_STATE_SIZE_H   = 40;
static const int STR_STATE_SIZE_XH  = 53;
static const int STR_STATE_SIZE_XXH = 80;
static const char STR_STATE_Welcome[]   = "Welcome";
static const char STR_STATE_PlzSel[]   = "Please Select Device";
static const int STR_WIFILIST_SIZE_M   = 13;
static const int STR_WIFILIST_SIZE_H   = 20;
static const int STR_WIFILIST_SIZE_XH  = 27;
static const int STR_WIFILIST_SIZE_XXH = 40;
static const char STR_STATE_Connecting[]   = "Connecting";

static const int STR_ROBOTMAIN_LABSIZE_M   = 12;
static const int STR_ROBOTMAIN_LABSIZE_H   = 18;
static const int STR_ROBOTMAIN_LABSIZE_XH  = 24;
static const int STR_ROBOTMAIN_LABSIZE_XXH = 36;
static const char STR_RobotMain_Status[]   = "Status";
static const char STR_RobotMain_charging[]   = "charging";
static const char STR_RobotMain_standby[]   = "stand-by";
static const char STR_RobotMain_cleaning[]   = "cleaning";
static const char STR_RobotMain_Backtodock[]   = "Back to dock";
static const char STR_RobotMain_Power[]   = "Power";
static const char STR_RobotMain_Everything[]   = "Everything is alright.";
static const char STR_RobotMain_Enjoy[]   = "Enjoy it !";
static const char STR_RobotMain_Go[]   = "Go";
static const char STR_RobotMain_Date[]   = "%.4d/%.02d/%.02d";
static const char STR_RobotMain_Time[]   = "%.02d:%.02d";

static const int STR_CLEAN_LABSIZE_M   = 12;
static const int STR_CLEAN_LABSIZE_H   = 24;
static const int STR_CLEAN_LABSIZE_XH  = 32;
static const int STR_CLEAN_LABSIZE_XXH = 48;
static const char STR_Clean_CleanStamps[]="Clean   Stamps";
static const char STR_Clean_VirtualWall[]="Virtual Wall";
static const char STR_Clean_Undo[]="Undo";
static const char STR_Clean_Delete[]="Delete";
static const char STR_Clean_CleanNow[]="Clean Now";
static const char STR_Clean_Gotit[]="Got it";
static const char STR_Clean_Cornercleaningjob[]="Corner cleaning job";
static const char STR_Clean_Cleaningstickers[]="Cleaning stickers";
static const char STR_Clean_cleaningarea[]="To big to be small inclinations Set focus on cleaning area";
static const char STR_Clean_NoEntry[]="FunRobot! No Entry!";

static const int STR_RESERV_LABSIZE_M   = 16;
static const int STR_RESERV_LABSIZE_H   = 24;
static const int STR_RESERV_LABSIZE_XH  = 32;
static const int STR_RESERV_LABSIZE_XXH = 48;
static const char STR_Reservation_SingleAppointment[]   = "Single Appointment";
static const char STR_Reservation_Choosetime[]   = "Choose custom start time cleaning";
static const char STR_Reservation_SetStartTime[]   = "Set Start Time";
static const char STR_Reservation_Robotwillstartcleaning[]   = "Robot will start cleaning";
//static const char STR_Reservation_ValueTimePM[]   = "last %d PM";
//static const char STR_Reservation_ValueTimeAM[]   = "last %d AM";
static const char STR_Reservation_ValueTime[]   = "later %.02d : %.02d";
static const char STR_Reservation_Dailycleaning[]   = "Daily cleaning";


static const char STR_HELP_PlzWait[]   = "Please wait";
static const char STR_HELP_AutoSearch[]   = "Automatically search robot... ";
static const char STR_HELP_PlzConf[]   = "Please confirm";
static const char STR_HELP_RobotConn[]   = "1.Robot connection status and power";
static const char STR_HELP_TheMobile[]   = "2.The mobile phone network connection";
static const char STR_HELP_Target[]   = "Target";
static const char STR_HELP_Setatarget[]   = "Set a target position, Also put forward the goal all along";
static const char STR_HELP_Automatic[]   = "Automatic cleaning house in a spiral pattern in every corner";
static const char STR_HELP_Automatic2[]   = "As long as there is a virtual wall brush You can easily draw the prohibited area!";
//static const char STR_HELP_Target[]   = "Target";
//static const char STR_HELP_Target[]   = "Target";
//static const char STR_HELP_Target[]   = "Target";
//static const char STR_HELP_Target[]   = "Target";
//static const char STR_HELP_Target[]   = "Target";
//static const char STR_HELP_Target[]   = "Target";

//setup page
static const char S_RobotSetup_RobotModel[]   = "Model";
static const char S_RobotSetup_RobotVersion[]   = "Version";
static const char S_RobotSetup_ApplicationVersion[]   = "Application Version";
static const char S_RobotSetup_Name[]   = "Name";
static const char S_RobotSetup_HomeAP[]   = "Setup Robot Connect to Home AP";
static const char S_RobotSetup_SyncTime[]   = "Synchronization Time";
static const char S_RobotSetup_Notification[]   = "Notification";
static const char S_RobotSetup_Log[]   = "Log";
static const char S_RobotSetup_Help[]   = "Help";
static const char S_RobotSetup_Legal[]   = "Legal";
static const char S_RobotSetup_Disconnection[]   = "Disconnection";
static const char S_RobotSetupMsg_SyncTime[]   = "Synchronization Time OK";
static const char S_RobotSetup_Disconnection_Fail[]   = "Disconnection Fail";
static const char S_RobotSetupNotif_TIPSYN[]   = "Tip synchronization time zone";
static const char S_RobotSetupNotif_STOPINTO[]   = "Stop into the Clean Job Tips";


static const int STR_HELP_SIZE_M   = 19;
static const int STR_HELP_SIZE_H   = 28;
static const int STR_HELP_SIZE_XH  = 37;
static const int STR_HELP_SIZE_XXH = 56;
static const int STR_HELP_COLOR=50;
static const int STR_DATE_SIZE_M   = 15;
static const int STR_DATE_SIZE_H   = 22;
static const int STR_DATE_SIZE_XH  = 29;
static const int STR_DATE_SIZE_XXH = 44;
static const int STR_TIME_SIZE_M   = 40;
static const int STR_TIME_SIZE_H   = 60;
static const int STR_TIME_SIZE_XH  = 80;
static const int STR_TIME_SIZE_XXH = 120;

static const char STR_LINK_TrouShoot[]   = "Trouble shooting";
static const int STR_LINK_SIZE_M   = 19;
static const int STR_LINK_SIZE_H   = 28;
static const int STR_LINK_SIZE_XH  = 37;
static const int STR_LINK_SIZE_XXH = 56;
static const int STR_SETUP_SIZE_M   = 16;
static const int STR_SETUP_SIZE_H   = 24;
static const int STR_SETUP_SIZE_XH  = 32;
static const int STR_SETUP_SIZE_XXH = 48;

static const int STR_TOAST_SIZE_M   = 13;
static const int STR_TOAST_SIZE_H   = 20;
static const int STR_TOAST_SIZE_XH  = 27;
static const int STR_TOAST_SIZE_XXH = 40;

static const int STR_DIALOG_SIZE_M   = 19;
static const int STR_DIALOG_SIZE_H   = 28;
static const int STR_DIALOG_SIZE_XH  = 37;
static const int STR_DIALOG_SIZE_XXH = 56;


//常用顏色定義
static const Color3B COLOR_STRING_WHITE=Color3B(254,254,254);
static const Color3B COLOR_STRING_ORIGIN=Color3B(255,255,255);

static const Color3B COLOR_STRING_GREEN=Color3B(BTN_Refresh_Enable_COLOR_R,BTN_Refresh_Enable_COLOR_G,BTN_Refresh_Enable_COLOR_B);

static const Color3B COLOR_STRING_GRAY=Color3B(BTN_Refresh_Disable_COLOR,BTN_Refresh_Disable_COLOR,BTN_Refresh_Disable_COLOR);
static const Color3B COLOR_STRING_LIGHTGREY=Color3B(0xF2,0xF2,0xF2);
static const Color3B COLOR_STRING_BLACK2=Color3B(0,0,0);
static const Color3B COLOR_STRING_BLACK=Color3B(STR_HELP_COLOR,STR_HELP_COLOR,STR_HELP_COLOR);
static const Color3B COLOR_STRING_LIGHTBLACK=Color3B(153,153,153);
static const Color3B COLOR_STRING_RED=Color3B(STR_ERROR2_COLOR_R,STR_ERROR2_COLOR_G,STR_ERROR2_COLOR_B);
static const Color3B COLOR_STRING_ERRBLACK=Color3B(STR_ERROR_COLOR_R,STR_ERROR_COLOR_G,STR_ERROR_COLOR_B);
static const Color3B COLOR_STRING_TITAL=Color3B(STR_TITAL_COLOR_R, STR_TITAL_COLOR_G, STR_TITAL_COLOR_B);



//切換狀態 加快圖示
void WifiSearchIcon_AddSpeed();

EXTERN  float *pSTR_CLEANJOB_SIZE;
EXTERN  float *pSTR_TITAL_SIZE;
EXTERN  float *pSTR_ERROR_SIZE;
EXTERN  float *pSTR_STATE_SIZE;
EXTERN  float *pSTR_WIFILIST_SIZE;
EXTERN  float *pSTR_ROBOTMAIN_LABSIZE;
EXTERN  float *pSTR_CLEAN_LABSIZE;
EXTERN  float *pSTR_RESERV_LABSIZE;
EXTERN  float *pSTR_HELP_SIZE;
EXTERN  float *pSTR_DATE_SIZE;
EXTERN  float *pSTR_TIME_SIZE;
EXTERN  float *pSTR_LINK_SIZE;
EXTERN  float *pSTR_SETUP_SIZE;
EXTERN  float *pSTR_BTN_SIZE;
EXTERN  float *pSTR_TOAST_SIZE;
EXTERN  float *pSTR_DIALOG_SIZE;
#endif // __IMAGELIST_H__