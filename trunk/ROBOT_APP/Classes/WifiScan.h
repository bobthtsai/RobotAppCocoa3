//Headers for WLAN SCAN
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#ifndef __WIFISCAN_H__
#define __WIFISCAN_H__

#include <windows.h>
#include <conio.h>
#include <atlbase.h>
#include <wlanapi.h>
#include <WinBase.h>
#include <malloc.h>

#include <string>
#include <vector>


using namespace std;

#define ETH_LEN 6
// Structure for Interworking element
	typedef struct _Interworking
	{
		BYTE ElementID;
		BYTE Length;
		//BYTE OUI[3];
		//BYTE TEST[50];
		BYTE ANO;      //Access Network Options
		BYTE VenueInfo[2];
		BYTE HESSID[ETH_LEN];
	
	}InterworkingIE,*pInterworkingIE;

	// general IE
	typedef struct _IE
	{
		UCHAR ElementID;
		UCHAR Length;
		UCHAR Data[ETH_LEN];

	}IES,*pIES;

	typedef struct _IeDataq
	{
		BYTE			InterIE[sizeof(InterworkingIE)];
		WLAN_RAW_DATA	pWRD;
		
	}IeDataq,*pIeDataq;



	// Information elements
#define IEID_INTERWORKING 107
#define IEID_ADVPROTOCOL  108
#define IEID_EXPBANDREQ   109
#define IEID_QOSMAPSET    110
#define IEID_ROAMCONS     111
#define IEID_EMERALERTID  112 
#define IEID_VENDORSPEC	  221	

class WIFI_SCAN
{
private:  
public:
	//variables used for WlanEnumInterfaces 
	PWLAN_INTERFACE_INFO_LIST	pIfList ;
	PWLAN_INTERFACE_INFO		pIfInfo ;
	//variables used for WlanGetAvailableNetworkList
	PWLAN_AVAILABLE_NETWORK_LIST	pBssList ;
	PWLAN_AVAILABLE_NETWORK			pBssEntry ;
	//VARIABLE USED FOR 
	PWLAN_BSS_LIST					pWlanBssList;
	// Variables for open handle
	DWORD pdwNegotiatedVersion	;
	HANDLE phClientHandle		;
	DWORD hResult				;
	DWORD pdwPrevNotifSource	;

	// GUID Variable
	GUID guidInterface			;

	//Handles for events and threads
	HANDLE EventThread;
	HANDLE EventWait;

	std::vector<int> WifiLinkIndex;
	std::vector<std::wstring> WifiCardName;
	std::vector<std::wstring> WifiAPName;
	std::vector<std::string> WifiSSIDName;
	std::vector<int> WifiLinkState;
	
public:
	WIFI_SCAN();
	~WIFI_SCAN();
	int MainSearch();
	
	int MainSearchNothread(vector<wstring>& APName,vector<string>& SSIDName ,vector<int>&iLink);//�D�n��� �^�ǬO�_���A�åB�N�o���ưO���U��.���}��THREAD

	bool IsVistaOrHigher() ;
	int FuncWlanOpenAndEnum();
	void FuncWlanPrintInterfaceNames();
	int FuncWlanScan(int ScanWithIE); //�j�MAP �M�w�O�_�n�w��SSID �j�M
	int FuncWlanCreateThreadsAndEvents();
	int WifiConnectAP(int NetWorkNumber); //�t�γs�u
	void FreeWifiScanMemory();	
	int WifiAPCheck(int NetWorkNumber);//�ˬd�O�_�w�g�s�u	
	int WifiDisconnectAP(int NetWorkNumber);//�_�u
	char* GetSSIDName(int NetWorkNumber);//���o���w�s�u��SSID�@�W��
};
//�]�����Ψ�C++ ���ŧi �ҥH����bUI_CommonFun.h
int GFUN_MainSearchNothread(vector<wstring>& APName,vector<string>& SSIDName, vector<int>&iLinkReady);

#endif __WIFISCAN_H__
#endif WIN32