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
	MAPMAIN_RETIRNZERO,
	MAPMAIN_ROBOT,
	MAPMAIN_MAPBK,
	MAPMAIN_SIZE,

    MAPMAIN_LAB_ZOOMLAB=MAPMAIN_SIZE,
    MAPMAIN_LAB_RESTIME,
    MAPMAIN_LAB_SIZE,
};
//地圖通用函數
void WaitRobotMapUpdate();
int WaittryRobotMapUpdate();
void RobotMapUpdate();
bool IsRobotMapReady();

class UI_MAP: public Layer
{
private:  
	int PageType;
	Size winSize;
    Vec2 winOrigin;
	bool bMoveKeep;
    std::vector<SpriteVector* > m_SpriteVectors;    //需要控制的圖形元件
    std::vector<LabelVector* >  m_LabelVectors;     //需要控制的標籤元件
    std::map<int,long> m_itemIdx; //記住元件在陣列中的位置
    
	float fZoomRate;
	int iRelseTime;
    
	Point StartPoint_Mouse;
	Point StartPoint_Map;
	Point StartPoint_Target;
	Texture2D *pMapTexture;

	void InitUI();
	void InitUI_MAPMAIN();
	void SetupUIBtn(Sprite* pSprite,bool bOnOff);
	
protected: //touch 修改
    bool touchEnabled;
    EventListenerTouchOneByOne* touchListener;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
    bool isTouchEnabled() const;
    virtual void setTouchEnabled(bool enabled);
    Sprite* getSpritePoint(int iPageTyep,int iPageIdx);
    Label* getLabelPoint(int iPageTyep,int iPageIdx);
    
public:
	CREATE_FUNC(UI_MAP); 
	virtual bool init() override;
	UI_MAP();
	~UI_MAP();
	void Function_UIMAP(int iBtnIdx);
	void ShowLayer();
	void HideLayer();	
	void Hide_AllPage();
	void Show_MapMain();
	void Hide_MapMain();
	//¶aπœ≤æ∞ 
	void MapMove(Vec2 nodePosition );
	void MAPZoomIn();
	void MAPZoomOut();
	void MAPZoomDefault();
	void MAPZoomLabUpdate(); //ßÛ∑s≠ø≤v¶r¶Í
	float MAPReSetSize(float fSize); //≥]©wMAP §j§p
	void update(float dt) override;  
	
	void MAPRetunZero();//¶^¬k≠Ï¬I

	void Function_UIMAP_START(int iBtnIdx);
	void ReloadRobotMap();	//≠´∑s∏¸§J¶aπœ
	void UpdateMAPSchedule(float ftime); //ßÛ∑s¶aπœ
    

};


#endif // __UIMAP_H__