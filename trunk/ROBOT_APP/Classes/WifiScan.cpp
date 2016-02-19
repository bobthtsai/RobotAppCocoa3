#include "wifiscan.h"
#include <wchar.h>
#include <UI_commonFun.h>
//#include <UI_RobotScanning.h>
// Lib paths
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

WIFI_SCAN m_WifiScan;
//extern UI_RobotScanning* m_pUIRobotSacnning; //因為有include winsock.h 跟winsock2.h 會衝突!

WIFI_SCAN::WIFI_SCAN()
{
	pIfList = NULL;
	pIfInfo = NULL;
	pBssList = NULL;
	pBssEntry = NULL;
	pWlanBssList=NULL;
	pdwNegotiatedVersion	=0;
	phClientHandle		=NULL;
	hResult				=ERROR_SUCCESS;
	pdwPrevNotifSource	= 0;
	EventWait =NULL;
	//guidInterface			={0};
}
//function that will for getting nw list
void FuncWlanGetNWList()
{

	m_WifiScan.hResult = WlanGetAvailableNetworkList(m_WifiScan.phClientHandle, 
		&m_WifiScan.guidInterface,
		WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_MANUAL_HIDDEN_PROFILES, 
		NULL, 
		&m_WifiScan.pBssList);   
}
//ies parsing
void FuncWlanParseIEs(PBYTE pBeaconframe,int SizeData)
{
	int len=SizeData;

	//printf("######## FuncWlanParseIEs--->########  \n \n");
	while(len>=2) //minimum length for ID and Length field
	{

		BYTE IEID;
		BYTE IELEN;
		IEID=*pBeaconframe++;
		IELEN=*pBeaconframe++;
		len-=2;

		if(IELEN>len)
		{
			//printf("######## FuncWlanParseIEs IELEN>len =%d =%d<--- ######## \n \n",IELEN,len);
			return;
		}

		switch(IEID)
		{
		case IEID_INTERWORKING:
			//printf("IEID_INTERWORKING---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("IEID_INTERWORKING<--- \n");	
			break;
		case IEID_ADVPROTOCOL:
			//printf("IEID_ADVPROTOCOL---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("IEID_ADVPROTOCOL<---  \n");
			break;	   	
		case IEID_EXPBANDREQ:
			//printf("IEID_EXPBANDREQ---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("IEID_EXPBANDREQ<---  \n");
			break;	   
		case IEID_QOSMAPSET:
			//printf("IEID_QOSMAPSET---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("IEID_QOSMAPSET<---  \n");
			break;	   
		case IEID_ROAMCONS:
			//printf("IEID_ROAMCONS---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("IEID_ROAMCONS<--- \n");
			break;	
		case IEID_EMERALERTID:
			//printf("IEID_EMERALERTID---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("IEID_EMERALERTID<---  \n");
			break;
		case	IEID_VENDORSPEC:
			//printf("IEID_VENDORSPEC---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("IEID_VENDORSPEC<---  \n");
			break;
		default:
			//printf("default IE---> \n");
			//FuncWlanIEPrint(IEID,IELEN,pBeaconframe);
			//printf("default IE<--- \n");
			break;

		}
		printf("\n");
		len-=IELEN;
		pBeaconframe+=IELEN;


	}
	printf("######## FuncWlanParseIEs <---########  \n \n");
}

//trying to print beacon or probe resp IE RAW data
DWORD FuncWlanBeaconFrame(PWLAN_BSS_ENTRY pWlanBssEntry,PBYTE pBeaconframe)
{

	PBYTE pBlob=NULL;
	ULONG i=0;

	//printf("######## FuncWlanBeaconFrame--->######## \n \n");
	//printf("offset of IE data blob =%d \n",pWlanBssEntry->ulIeOffset);
	//printf("Size of IE data blob =%d \n",pWlanBssEntry->ulIeSize);
	//printf("MAC ADRESS for AP  %02x:%02x:%02x:%02x:%02x:%02x\n",	pWlanBssEntry->dot11Bssid[0],
	//	pWlanBssEntry->dot11Bssid[1],
	//	pWlanBssEntry->dot11Bssid[2],
	//	pWlanBssEntry->dot11Bssid[3],
	//	pWlanBssEntry->dot11Bssid[4],
	//	pWlanBssEntry->dot11Bssid[5]);
	//printf("BSS Type %d \n ",pWlanBssEntry->dot11BssType);
	//printf("SSID %s \n ",pWlanBssEntry->dot11Ssid.ucSSID);
	//printf("Capability of beacon %d  \n \n ",pWlanBssEntry->usCapabilityInformation);
	//moving pointer the correct offset
	pBlob=(PBYTE)(pWlanBssEntry)+pWlanBssEntry->ulIeOffset;
	//IE data size
	memcpy((void*)pBeaconframe,(void*)pBlob,pWlanBssEntry->ulIeSize);
	//printf("*****Total IE data byte by byte Last Beacon and Probe Response****** \n");
	//do
	//{
	//	printf("%d\t",pBeaconframe[i]);
	//	i++;
	//	if((i!=0)&&(i%10)==0)
	//		printf("\n");

	//}while(i<pWlanBssEntry->ulIeSize);
	//printf("\n");
	//printf("*****Total IE data byte by byte Last Beacon and Probe Response****** \n \n");
	//printf("\n \n");
	FuncWlanParseIEs(&pBeaconframe[0],pWlanBssEntry->ulIeSize);

	//printf("######## FuncWlanBeaconFrame <---########  \n \n");
	return 0;
}

// function for bss list where we can read beacon with pointer to strcuture offset and length of response
int FuncWlanGetBSSList()
{
	m_WifiScan.hResult = WlanGetNetworkBssList(m_WifiScan.phClientHandle, 
		&m_WifiScan.guidInterface,
		NULL, 
		dot11_BSS_type_any,
		NULL,
		NULL,
		&m_WifiScan.pWlanBssList);   
	if(m_WifiScan.pWlanBssList->dwNumberOfItems==0)
	{
//		printf("########FuncWlanGetBSSList is empty<---######## \n \n");
		return 0;
	}

	for(unsigned int i=0;i<m_WifiScan.pWlanBssList->dwNumberOfItems;i++)
	{

		PWLAN_BSS_ENTRY pWlanBssEntry=&m_WifiScan.pWlanBssList->wlanBssEntries[i];
		void *beaconframe=malloc(pWlanBssEntry->ulIeSize);
		FuncWlanBeaconFrame(pWlanBssEntry,(PBYTE)beaconframe);

	}
	return m_WifiScan.pWlanBssList->dwNumberOfItems;

}
int bwait=1;
DWORD WINAPI FuncWlanThread(LPVOID lpParameter)
{
	UNREFERENCED_PARAMETER(lpParameter);
	DWORD WaitResult;

	WaitResult = WaitForSingleObject(m_WifiScan.EventWait,INFINITE);   
	bwait=1;
	switch (WaitResult) 
	{
		// Event object was signaled
	case WAIT_OBJECT_0: 
		m_WifiScan.hResult=WlanRegisterNotification(m_WifiScan.phClientHandle,
			WLAN_NOTIFICATION_SOURCE_NONE,
			TRUE,
			NULL,
			NULL,
			NULL,
			&m_WifiScan.pdwPrevNotifSource);

		if(m_WifiScan.hResult==ERROR_SUCCESS)
		{
			FuncWlanGetNWList(); ///取得連線列表
			FuncWlanGetBSSList();//working fine
		}

		break; 
		// An error occurred
	default: 
		return 0; 
	}
	bwait=0;
	return 1;
}

//function that creates thread and events and wait for unregistering the CB
int WIFI_SCAN::FuncWlanCreateThreadsAndEvents()
{
	EventWait=CreateEvent(NULL,
		FALSE,
		FALSE,
		L"WaitEvent");
	if (EventWait == NULL) 
	{ 
		return -1;
	}
	EventThread=CreateThread(NULL,
		0,
		FuncWlanThread,
		NULL,
		0,
		0);
	if (EventThread == NULL) 
	{ 
		return -1;
	}
	return 1;
}

WIFI_SCAN::~WIFI_SCAN()
{

}
// to check the operating system to meention the type of client (vista,xp etc)
bool WIFI_SCAN::IsVistaOrHigher()  
{  
	OSVERSIONINFO osVersion; 
	ZeroMemory(&osVersion, sizeof(OSVERSIONINFO));  
	osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);  
	if(!GetVersionEx(&osVersion))  
	{
		return false; 
	}
	if(osVersion.dwMajorVersion >= 6)
	{
		return true;
	}
	return false;  

}
//function to open and enumerate
//開檔 取得網卡位置
int WIFI_SCAN::FuncWlanOpenAndEnum()
{
	DWORD dwClientVersion=(IsVistaOrHigher() ? 2 : 1);

	//creating session handle for the client to connect to server.
	hResult=WlanOpenHandle(dwClientVersion,NULL,&pdwNegotiatedVersion,&phClientHandle);
	if(hResult!=ERROR_SUCCESS)
	{
		return hResult;
	}
	//Enumerates all the wifi adapters currently enabled on PC.
	//Returns the list of interface list that are enabled on PC.
	hResult=WlanEnumInterfaces(phClientHandle,NULL,&pIfList);
	if(hResult!=ERROR_SUCCESS)
	{
		return hResult;
	}
	return hResult;
}
//取得網路卡名稱 設定搜尋的網卡名稱
void WIFI_SCAN::FuncWlanPrintInterfaceNames()
{
	WCHAR SGuid[256]={0};
	for(unsigned int i=0;(i < pIfList->dwNumberOfItems);i++)
	{
		WifiCardName.push_back(pIfList->InterfaceInfo[pIfList->dwIndex].strInterfaceDescription);
		StringFromGUID2(pIfList->InterfaceInfo[pIfList->dwIndex].InterfaceGuid,SGuid,256);
	}
	guidInterface=pIfList->InterfaceInfo[0].InterfaceGuid;
}


//planning to start scan operation
//主動掃描 附近的AP 當SCANWITHIE =1 表示指定SSID
int WIFI_SCAN::FuncWlanScan(int ScanWithIE)
{
	pInterworkingIE pInfoElem	=NULL;
	PWLAN_RAW_DATA	pIeData		=NULL;
	PBYTE			pData		=NULL;

	//creating threads and events
	hResult=FuncWlanCreateThreadsAndEvents();
	if(hResult==-1)	
	{
		return hResult;
	}


	if(ScanWithIE)
	{
		pIeData=(PWLAN_RAW_DATA)malloc(sizeof(InterworkingIE)+sizeof(DWORD)); 
		memset((void*)pIeData,0,(sizeof(InterworkingIE)+sizeof(DWORD)));		

		pIeData->dwDataSize=sizeof(InterworkingIE);
		pInfoElem =(pInterworkingIE)&pIeData->DataBlob;

		//working only with vvendor specific IE's for 
		//WIFI Adapter Description =Intel(R) Centrino(R) Advanced-N 6205 
		//WIFI Adapter GUID={0E7CAA2C-B007-4B75-A5BF-2B3ED571C635} 
		//May be intel driver filtering reserved element IDS(52-126)

		pInfoElem->ElementID=IEID_INTERWORKING; 	
		pInfoElem->Length=sizeof(InterworkingIE)-2;
		pInfoElem->ANO=1;
		//memcpy((void*)&pInfoElem->TEST,(void*)"INTERWORKING",13);


		hResult=WlanScan(phClientHandle,&guidInterface,NULL,pIeData,NULL);

		if(hResult!=ERROR_SUCCESS)
		{
			if(!SetEvent(EventWait))
			{
				return hResult;
			}

		}
	}
	else
	{
		//wlan scan without mentioning SSID. assuming PC has only one wifi card
		hResult=WlanScan(phClientHandle,&guidInterface,NULL,NULL,NULL);
		if(hResult!=ERROR_SUCCESS)
		{
			if(!SetEvent(EventWait))
			{
				return hResult;
			}
		}
	}
	return hResult;
}
void FuncWlanAcmNotify(PWLAN_NOTIFICATION_DATA data,PVOID context)
{
	if(data->NotificationCode==wlan_notification_acm_scan_complete)
	{
		//printf("Scan is completed with success  \n");
		//if(!SetEvent(EventWait))
		//	printf("setting event failed \n");
	}

	if(data->NotificationCode==wlan_notification_acm_scan_fail)
	{
		//printf("Scan is completed with Fail  \n");
		//if(!SetEvent(EventWait))
		//	printf(" setting event failed  \n");
	}
}
//檢查是否已經連線

int WIFI_SCAN::WifiAPCheck(int iIdx)
{
	//PWLAN_AVAILABLE_NETWORK_LIST	pTempBssList ;
	//WlanGetAvailableNetworkList(m_WifiScan.phClientHandle, 
	//	&m_WifiScan.guidInterface,
	//	WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_MANUAL_HIDDEN_PROFILES, 
	//	NULL, 
	//	&pTempBssList);
	if(pBssList==NULL)
		return false;
	int NetWorkNumber=WifiLinkIndex[iIdx];
	WLAN_AVAILABLE_NETWORK wlanAN = pBssList->Network[NetWorkNumber];//PLEASE CHECK THIS YOURSELF
	bool bLink=true;
	if(wlanAN.dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) // 已經連線
		bLink= true;
	else
		bLink= false;
	//WlanFreeMemory(pTempBssList);
	return bLink;


}
//斷線

int WIFI_SCAN::WifiDisconnectAP(int NetWorkNumber)
{

	DWORD dwResult = WlanDisconnect(phClientHandle, &pIfList->InterfaceInfo[0].InterfaceGuid,NULL);
	return dwResult;
}
char* WIFI_SCAN::GetSSIDName(int iIdx)
{
	char stemp[256];
	int NetWorkNumber=WifiLinkIndex[iIdx];
	WLAN_AVAILABLE_NETWORK wlanAN = m_WifiScan.pBssList->Network[NetWorkNumber];//PLEASE CHECK THIS YOURSELF
	//sprintf(stemp,"%s",wlanAN .dot11Ssid.ucSSID); //顯示SSID
	return (char*)wlanAN .dot11Ssid.ucSSID; //注意這裡有機會造成記憶體lost
}
//主要函數 回傳是否找到，並且將得到資料記錄下來

int WIFI_SCAN::WifiConnectAP(int iIdx)
{
	int NetWorkNumber=WifiLinkIndex[iIdx];
	WLAN_AVAILABLE_NETWORK wlanAN = m_WifiScan.pBssList->Network[NetWorkNumber];//PLEASE CHECK THIS YOURSELF
	WLAN_CONNECTION_PARAMETERS wlanConnPara;
	wlanConnPara.wlanConnectionMode =wlan_connection_mode_discovery_unsecure ; //wlan_connection_mode_discovery_unsecure ;//wlan_connection_mode_profile ; //WINXP sp2 限定 wlan_connection_mode_profile
	wlanConnPara.strProfile = NULL;//wlanAN.strProfileName;	// set the profile name
	wlanConnPara.pDot11Ssid = &wlanAN.dot11Ssid;		// SET SSID
	wlanConnPara.dot11BssType = wlanAN.dot11BssType;//dot11_BSS_type_independent;//;//dot11_BSS_type_infrastructure;		//dot11_BSS_type_any,I do not need it this time.	    
	wlanConnPara.pDesiredBssidList = NULL;							// the desired BSSID list is empty
	wlanConnPara.dwFlags = wlanAN.dwFlags;//WLAN_CONNECTION_ADHOC_JOIN_ONLY;//;//WLAN_CONNECTION_HIDDEN_NETWORK;			//it works on my WIN7\8 //Windows XP with SP3 and Wireless LAN API for Windows XP with SP2:  This member must be NULL.
	DWORD dwResult =ERROR_SUCCESS;
	if(wlanAN.dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED) // 已經連線 不再連線
		dwResult =ERROR_SUCCESS;
	else
		dwResult=WlanConnect(phClientHandle,&pIfList->InterfaceInfo[0].InterfaceGuid,&wlanConnPara ,NULL);
	return dwResult;// ERROR_SUCCESS
}
//主要函數 回傳是否找到，並且將得到資料記錄下來
int WIFI_SCAN::MainSearchNothread(vector<wstring>& APName,vector<string>& SSIDName, vector<int>&iLink)
{
	FreeWifiScanMemory();
	//openin handle for client to communicate with adapter and enumerating the wifi interfaces
	hResult=FuncWlanOpenAndEnum(); 
	if(hResult!=ERROR_SUCCESS)
	{
		WlanCloseHandle(phClientHandle,NULL);
		return hResult;
	}
	// trying to print all the wifi interfaces present and storing GUID for first adapter
	FuncWlanPrintInterfaceNames(); //取得網路卡名稱 設定搜尋的網卡名稱
	if(EventWait==NULL)
	{
		EventWait=CreateEvent(NULL,
			FALSE,
			FALSE,
			L"WaitEvent");
	}
	WaitForSingleObject(m_WifiScan.EventWait,2000); //等待3秒
	//wlan scan without mentioning SSID. assuming PC has only one wifi card
	hResult=WlanScan(phClientHandle,&guidInterface,NULL,NULL,NULL);
	if(hResult!=ERROR_SUCCESS)
	{
		WlanCloseHandle(phClientHandle,NULL);
		WlanFreeMemory(pIfList);
		return hResult;
	}
	//確認網卡準備OK
	hResult=WlanRegisterNotification(phClientHandle,
		WLAN_NOTIFICATION_SOURCE_ACM,
		TRUE,
		(WLAN_NOTIFICATION_CALLBACK)FuncWlanAcmNotify,
		NULL,
		NULL,
		&pdwPrevNotifSource);

	if(hResult!=ERROR_SUCCESS)
	{
		return hResult;
	}
	
	//檢查是否有網卡
	hResult=WlanRegisterNotification(phClientHandle,
		WLAN_NOTIFICATION_SOURCE_NONE,
		TRUE,
		NULL,
		NULL,
		NULL,
		&pdwPrevNotifSource);

	int iGetListType=0;
	if(m_WifiScan.hResult==ERROR_SUCCESS)
	{//預設只有有抓取第一張網卡SSID
		//FuncWlanGetNWList(); ///只能得到ssid，和信號質量
		//日後改回去
//		int iNum=FuncWlanGetBSSList();//可以得到ssid，bssid（MAC），信號質量，時間戳
//		if(iNum==0) //時間太短 或是訊號不好 再延長 然後只有取SSID 
		{
			WifiSearchIcon_AddSpeed();//搜尋圖示旋轉加快
			WaitForSingleObject(m_WifiScan.EventWait,3000); //再等待3秒
			FuncWlanGetNWList(); ///只能得到ssid，和信號質量
			iGetListType=1;
		}
	}
	if(iGetListType==0)
	{

		//將WIFI 名稱寫入 std:string 中
		char stemp[256];
		WCHAR wstemp[256];
		WifiLinkIndex.clear();
		WifiAPName.clear();
		WifiSSIDName.clear();
		WifiLinkState.clear();
		for(unsigned int i=0;i<pWlanBssList->dwNumberOfItems;i++)
		{
			swprintf(wstemp,255,L"%s",pBssList->Network[i].strProfileName);			
			sprintf(stemp,"%s",pBssList->Network[i].dot11Ssid.ucSSID); //顯示SSID
			if(wcsstr(wstemp, L"Robot") != 0 || strstr(stemp, "Robot")!=0)
			{
				WifiLinkIndex.push_back(i);  // Network index
				WifiAPName.push_back(wstemp);//顯示AP NAME							
				WifiSSIDName.push_back(stemp);
				if(pBssList->Network[i].dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
					WifiLinkState.push_back(1); //已經連線
				else
					WifiLinkState.push_back(0);
			}
		}
	}
	else
	{
		//將WIFI 名稱寫入 std:string 中
		char stemp[256];
		WCHAR wstemp[256];
		WifiLinkIndex.clear();
		WifiAPName.clear();
		WifiSSIDName.clear();
		WifiLinkState.clear();
		for(unsigned int i=0;i<pBssList->dwNumberOfItems;i++)
		{
			swprintf(wstemp,255,L"%s",pBssList->Network[i].strProfileName);
			sprintf(stemp,"%s",pBssList->Network[i].dot11Ssid.ucSSID); //顯示SSID
			if(wcsstr(wstemp, L"Robot") != 0 || strstr(stemp, "Robot")!=0)
			{
				WifiLinkIndex.push_back(i);  // Network index
				WifiAPName.push_back(wstemp);	//顯示AP NAME
				WifiSSIDName.push_back(stemp);
				if(pBssList->Network[i].dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED)
					WifiLinkState.push_back(1); //已經連線
				else
					WifiLinkState.push_back(0);

			}
		}

	}

	//NetworkLinkIndex=WifiLinkIndex;
	APName=WifiAPName;
	SSIDName=WifiSSIDName;
	iLink=WifiLinkState;

	return ERROR_SUCCESS;
}
void WIFI_SCAN::FreeWifiScanMemory()
{
	if(pIfList!=NULL)
	{
		WlanFreeMemory(pIfList);
		pIfList=NULL;
	}
	if(pBssList!=NULL)
	{
		WlanFreeMemory(pBssList);
		pBssList=NULL;
	}
	if(phClientHandle!=NULL)
	{
		WlanCloseHandle(phClientHandle,NULL);
		phClientHandle=NULL;
	}
	


}
//主要函數 回傳是否找到，並且將得到資料記錄下來
int WIFI_SCAN::MainSearch()
{
	//openin handle for client to communicate with adapter and enumerating the wifi interfaces
	hResult=FuncWlanOpenAndEnum();
	if(hResult!=ERROR_SUCCESS)
	{
		WlanCloseHandle(phClientHandle,NULL);
		return hResult;
	}
	// trying to print all the wifi interfaces present and storing GUID for first adapter

	FuncWlanPrintInterfaceNames();
	//if(ScanWithIE==1) //指定SSID
	//{
	//	hResult=FuncWlanScan(ScanWithIE);
	//	if(hResult!=ERROR_SUCCESS)
	//	{
	//		printf("failed FuncWlanScan with IE=%d \n",hResult);
	//		printf("######## MAIN()<---######## \n");
	//		WlanCloseHandle(phClientHandle,NULL);
	//		WlanFreeMemory(pIfList);
	//		CloseHandle(EventWait);
	//		return hResult;
	//	}
	//}
	//else if(ScanWithIE==0)
	//{
		
		hResult=FuncWlanScan(0);
		if(hResult!=ERROR_SUCCESS)
		{
			WlanCloseHandle(phClientHandle,NULL);
			WlanFreeMemory(pIfList);
			CloseHandle(EventWait);
			return hResult;
		}


		//等待搜尋
		while(bwait)
		{
			Sleep(1000);
		}

	//}
		char stemp[256];
		for(unsigned int i=0;i<pBssList->dwNumberOfItems;i++)
		{
			WifiAPName.push_back(pBssList->Network[i].strProfileName);
			sprintf(stemp,"%s",pBssList->Network[i].dot11Ssid.ucSSID);
			WifiSSIDName.push_back(stemp);
		}

		WlanFreeMemory(pIfList);
		WlanFreeMemory(pBssList);
		WlanCloseHandle(phClientHandle,NULL);
		return hResult;
}
//給外部呼交的公用函數
int GFUN_WifiConnectAP(int iIdx)
{
	return m_WifiScan.WifiConnectAP(iIdx);
}
int GFUN_WifiAPCheck(int iIdx)
{
	return m_WifiScan.WifiAPCheck(iIdx);
}
int GFUN_WifiDisconnectAP(int iIdx)
{
	return m_WifiScan.WifiDisconnectAP(iIdx);
}
void GFUN_WaitWifiEvenSingle(int iTime)
{
	WaitForSingleObject(m_WifiScan.EventWait,iTime);
}
void GFUN_FreeWifiScanMemory()
{
	m_WifiScan.FreeWifiScanMemory();
}

void GFUN_CreadWifi()
{
	m_WifiScan =WIFI_SCAN::WIFI_SCAN();
}
int GFUN_MainSearchNothread(vector<wstring>& APName,vector<string>& SSIDName, vector<int>&iLinkReady)
{
	return m_WifiScan.MainSearchNothread(APName,SSIDName,iLinkReady);
}