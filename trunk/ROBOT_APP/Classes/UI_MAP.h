#ifndef __UIMAP_H__
#define __UIMAP_H__
#include "cocos2d.h"
using namespace cocos2d;
//頁面種類
enum _MAPPGETYPE_
{
	MAPPG_MAIN,
	MAPPG_SIZE, //頁面數量
	MAPPG_LAYER //無顯示頁面
};
enum _MAPSTATEITEM_
{
	MAPMAIN_BK=0,
	MAPMAIN_HISTORY,
	MAPMAIN_TITAL,	
	MAPMAIN_ZOOMOUT,
	MAPMAIN_ZOOMIN,
	MAPMAIN_ZOOMLAB,
	MAPMAIN_RETIRNZERO,
//#ifdef DEBUG
	MAPMAIN_RESTIME,
//#endif
	MAPMAIN_ROBOT,
	MAPMAIN_MAPBK,
	MAPMAIN_SIZE
};
class UI_MAP: public CCLayer
{
private:  
	int PageType;
	CCSize winSize;  
	bool bMoveKeep;
	float fZoomRate; //縮放倍率
	int iRelseTime; //更新時間
	CCPoint StartPoint_Mouse;
	CCPoint StartPoint_Map;
	CCPoint StartPoint_Target;
	CCTexture2D *pMapTexture; 

	CCArray* m_BtnArray[MAPMAIN_SIZE];//存放所有按鈕
	void InitUI();
	void InitUI_MAPMAIN();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void SetupUIBtn(CCSprite* pSprite,bool bOnOff);
	
;//設定畫面按鈕
public:
	CREATE_FUNC(UI_MAP); 
	virtual bool init();  
	UI_MAP();
	~UI_MAP();
	void Function_UIMAP(int iBtnIdx);
	void ShowLayer();
	void HideLayer();	
	void Hide_AllPage();
	void Show_MapMain();
	void Hide_MapMain();
	//地圖移動
	void MapMove(CCPoint nodePosition );
	void MAPZoomIn();
	void MAPZoomOut();
	void MAPZoomDefault();
	void MAPZoomLabUpdate(); //更新倍率字串
	float MAPReSetSize(float fSize); //設定MAP 大小
	void update(float dt);  
	
	void MAPRetunZero();//回歸原點

	void Function_UIMAP_START(int iBtnIdx);
	void ReloadRobotMap();	//重新載入地圖
	void UpdateMAPSchedule(float ftime); //更新地圖

};


#endif // __UIMAP_H__