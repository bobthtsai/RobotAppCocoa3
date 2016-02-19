#ifndef __UICLENJOB_H__
#define __UICLENJOB_H__
#include "cocos2d.h"
using namespace cocos2d;
//頁面種類
enum _CLEANJOBPGETYPE_
{
	CLEJOBPG_MAIN,
	CLEJOBPG_CORNER,
	CLEJOBPG_CORNERHELP,
	CLEJOBPG_MULTI,
	CLEJOBPG_MULTIHELP,
	CLEJOBPG_MOVETO,
	CLEJOBPG_MOVETOHELP,
	CLEJOBPG_AUTO,
	CLEJOBPG_AUTOHELP,
	CLEJOBPG_SPIRAL,
	CLEJOBPG_SPIRALHELP,
	CLEJOBPG_WALL,
	CLEJOBPG_WALLHELP,

	CLEJOBPG_SIZE, //頁面數量
	CLEJOBPG_LAYER, //無顯示頁面
};

enum _CLEANJOBMAINITEM_
{
	CLEJOBMAIN_BK=0,
	CLEJOBMAIN_TITAL,
	CLEJOBMAIN_CORNER,
	CLEJOBMAIN_MULTI,
	CLEJOBMAIN_MOVE,
	CLEJOBMAIN_AUTO,
	CLEJOBMAIN_SPIRAL,
	CLEJOBMAIN_WALL,
	CLEJOBMAIN_SIZE,
    CLEJOBMAIN_LAB_CORNER =CLEJOBMAIN_SIZE,
    CLEJOBMAIN_LAB_MULTI,
    CLEJOBMAIN_LAB_MOVE,
    CLEJOBMAIN_LAB_AUTO,
    CLEJOBMAIN_LAB_SPIRAL,
    CLEJOBMAIN_LAB_WALL,
    CLEJOBMAIN_LAB_SIZE,
	CLEJOBMAIN_LAB_SPACE=CLEJOBMAIN_LAB_CORNER-CLEJOBMAIN_CORNER, //背景上LAB
};
enum _CLECORNERITEM_
{
	CLECORNER_BK=0,
	CLECORNER_HELP,
	CLECORNER_TITAL,	
	CLECORNER_ZOOMOUT,
	CLECORNER_ZOOMIN,	
	CLECORNER_ZOOMLAB,
	CLECORNER_OVAL,
	CLECORNER_RECT,
	CLECORNER_WALL,
	CLECORNER_UNDO,
	CLECORNER_DELETE,
	CLECORNER_GO,
	CLECORNER_ROBOT,
	CLECORNER_MAP,
	CLECORNER_SIZE,

};
enum _CLEMULTIITEM_
{
	CLEMULTI_BK=0,
	CLEMULTI_HELP,
	CLEMULTI_TITAL,	
	CLEMULTI_ZOOMOUT,
	CLEMULTI_ZOOMIN,
	CLEMULTI_ZOOMLAB,
	CLEMULTI_OVAL,
	CLEMULTI_RECT,
	CLEMULTI_WALL,
	CLEMULTI_UNDO,
	CLEMULTI_DELETE,
	CLEMULTI_GO,
	CLEMULTI_ROBOT,
	CLEMULTI_MAP,
	CLEMULTI_SIZE,
};
enum _CLEMOVETOITEM_
{
	CLEMOVETO_BK=0,
	CLEMOVETO_HELP,
	CLEMOVETO_TITAL,	
	CLEMOVETO_ZOOMOUT,
	CLEMOVETO_ZOOMIN,
	CLEMOVETO_ZOOMLAB,
	CLEMOVETO_WALL,
	CLEMOVETO_UNDO,
	CLEMOVETO_DELETE,
	CLEMOVETO_GO,
	CLEMOVETO_TARGET,
	CLEMOVETO_ROBOT,
	CLEMOVETO_MAP,
	CLEMOVETO_SIZE,
};
enum _CLEAUTOITEM_
{
	CLEAUTO_BK=0,
	CLEAUTO_HELP,
	CLEAUTO_TITAL,	
	CLEAUTO_ZOOMOUT,
	CLEAUTO_ZOOMIN,
	CLEAUTO_ZOOMLAB,
	CLEAUTO_WALL,
	CLEAUTO_UNDO,
	CLEAUTO_DELETE,
	CLEAUTO_GO,
	CLEAUTO_ROBOT,
	CLEAUTO_MAP,
	CLEAUTO_SIZE,
};

enum _CLESPIRALITEM_
{
	CLESPIRAL_BK=0,
	CLESPIRAL_HELP,
	CLESPIRAL_TITAL,	
	CLESPIRAL_ZOOMOUT,
	CLESPIRAL_ZOOMIN,	
	CLESPIRAL_ZOOMLAB,
	CLESPIRAL_WALL,
	CLESPIRAL_UNDO,
	CLESPIRAL_DELETE,
	CLESPIRAL_GO,
	CLESPIRAL_ROBOT,
	CLESPIRAL_MAP,
	CLESPIRAL_SIZE,
};

enum _CLEWALLITEM_
{
	CLEWALL_BK=0,
	CLEWALL_HELP,
	CLEWALL_TITAL,	
	CLEWALL_ZOOMOUT,
	CLEWALL_ZOOMIN,
	CLEWALL_ZOOMLAB,
	CLEWALL_WALL,
	CLEWALL_UNDO,
	CLEWALL_DELETE,
	CLEWALL_GO,
	CLEWALL_ROBOT,
	CLEWALL_MAP,
	CLEWALL_SIZE,
};

//每個按鍵位置

enum _CLEANJOBPGIDXETYPE_
{
	JOBIDX_CORNER,
	JOBIDX_MULTI,
	JOBIDX_MOVETO,
	JOBIDX_AUTO,
	JOBIDX_SPIRAL,
	JOBIDX_WALL,
	JOBIDX_FUNCTIONNUM,	//頁面數量
};
//const int PAGEBK_IDX[JOBIDX_FUNCTIONNUM]=
//{
//	CLEJOBPG_CORNER,
//	CLEJOBPG_MULTI,
//	CLEJOBPG_MOVETO,
//	CLEJOBPG_AUTO,
//	CLEJOBPG_SPIRAL,
//	CLEJOBPG_WALL,
//};
const int MAPBK_IDX[JOBIDX_FUNCTIONNUM]=
{
	CLECORNER_MAP,
	CLEMULTI_MAP,
	CLEMOVETO_MAP,
	CLEAUTO_MAP,
	CLESPIRAL_MAP,
	CLEWALL_MAP,
};

const int ROBOTICON_IDX[JOBIDX_FUNCTIONNUM]=
{
	CLECORNER_ROBOT,
	CLEMULTI_ROBOT,
	CLEMOVETO_ROBOT,
	CLEAUTO_ROBOT,
	CLESPIRAL_ROBOT,
	CLEWALL_ROBOT,
};
const int STARTITEM_IDX[JOBIDX_FUNCTIONNUM]=
{
	CLECORNER_HELP,
	CLEMULTI_HELP,
	CLEMOVETO_HELP,
	CLEAUTO_HELP,
	CLESPIRAL_HELP,
	CLEWALL_HELP,
};
const int ENDITEM_IDX[JOBIDX_FUNCTIONNUM]=
{
	CLECORNER_SIZE,
	CLEMULTI_SIZE,
	CLEMOVETO_SIZE,
	CLEAUTO_SIZE,
	CLESPIRAL_SIZE,
	CLEWALL_SIZE,
};

const int ZOOMLAB_IDX[JOBIDX_FUNCTIONNUM]=
{
	CLECORNER_ZOOMLAB,
	CLEMULTI_ZOOMLAB,
	CLEMOVETO_ZOOMLAB,
	CLEAUTO_ZOOMLAB,
	CLESPIRAL_ZOOMLAB,
	CLEWALL_ZOOMLAB,
};
//傳送給robot server 地圖的index
const int VIRUALWALLMAP_IDX[JOBIDX_FUNCTIONNUM]=
{
	10,
	10,//11,
	10,//12,
	10,//13,
	10,//14,
	10,//15,
};
const int ZONEMAP_IDX[JOBIDX_FUNCTIONNUM]=
{
	30,
	30,//31,
	30,//32,
	30,//33,
	30,//34,
	30,//35,
};
enum MapRangType
{
	MRType_None,
	MRType_Circle,
	MRType_Square,
	MRType_Pen
};
enum MapRangEditBtn
{
	MRBTN_NONE,
	MRBTN_ROTATE,
	MRBTN_HORIZONTAL,
	MRBTN_VERTICAL,
	MRBTN_PEN,
};
//畫框的變數
typedef struct stMapRangInfo
{	
	bool bUse;	//空間被使用	
	bool bSel;	//被點選中
	bool bReserver1;	//預備旗標
	bool bReserver2;	//預備旗標
	
	int ID;		//編號
	int iType;	//圖型種類
	
	float fX;	//圖形左下角
	float fY;	
	float fEdgeLeft;	//邊框數據
	float fEdgeTop;
	float fEdgeRight;
	float fEdgeDown;

	int iBefIdx;	//上一步
	int iWidth;		//長
	int iHeight;	//寬
	int iAngle;		//角度
}MapRangInfo,*pMapRangInfo;

const int G_DRAWPENTAG = 1;

class UI_CLEANJOB: public Layer
{
private:  
    int PageType;
    Size winSize;
    Vec2 winOrigin;
    std::vector<SpriteVector* > m_SpriteVectors;    //需要控制的圖形元件
    std::vector<LabelVector* >  m_LabelVectors;     //需要控制的標籤元件
    std::map<int,long> m_itemIdx; //記住元件在陣列中的位置
    float fZoomRate;

	//拖拉變數
	bool bMoveKeep;
	Point StartPoint_Mouse;
	Point StartPoint_Map;
	Point StartPoint_Rang;
	Point StartPoint_Target;
	//Point MinPenPoint;
	//Point MaxPenPoint;
	int StartPoint_RangWidth;
	int StartPoint_RangHeight;
	//點選範圍變數
	int iRangTagIdx;
	int iRangBtnType;
	int iMaxZord;
	int iChangRangFlag;
	void InitUI();
	void InitUI_CLEANJOBMAIN();
	void InitUI_CORNER();
	void InitUI_CORNERHELP();
	void InitUI_MULTI();
	void InitUI_MULTIHELP();
	void InitUI_MOVETO();
	void InitUI_MOVETOHELP();
	void InitUI_AUTO();
	void InitUI_AUTOHELP();
	void InitUI_SPIRAL();
	void InitUI_SPIRALHELP();
	void InitUI_WALL();
	void InitUI_WALLHELP();

	void BtnColorSetting(int iBtnIdx,bool bSel);//按鈕變色

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
     SpriteVector* getSpriteArray(int pageType);
     LabelVector* getLabelArray(int pageType);
    void saveSprite(int Page,int iIDx,Sprite* pSprite);
    void saveLabel(int Page,int iIDx,Label* pSprite);
    
public:
	CREATE_FUNC(UI_CLEANJOB); 
	virtual bool init() override;
	UI_CLEANJOB();
	~UI_CLEANJOB();
	void Function_CLEANJOBMAIN(int iBtnIdx);
	void Function_CORNER(int iBtnIdx);
	void Function_MULTI(int iBtnIdx);
	void Function_MOVETO(int iBtnIdx);
	void Function_AUTO(int iBtnIdx);
	void Function_SPIRAL(int iBtnIdx);
	void Function_WALL(int iBtnIdx);
	
	void CleanMAPRang(int iPagIdx);//清空現在畫面上所有元件
	void MouseDown_Function(Point  nodePosition);	
	void MAPRangItemCheck();//檢查標示範圍點選
	void TargetMove_ByMouse(Sprite *pSprite,Point MousePosition ); //圖釘移動
	void TargetMove(Point  nodePosition);
	void TargetSet(Point  nodePosition);
	void TargetPointSave();			//紀錄現在圖釘位置
	void ShowLayer();
	void HideLayer();	
	void Hide_AllPage();
	void Show_CLEANJOBMAIN();
	void Hide_CLEANJOBMAIN();
	void Show_CORNER();
	void Hide_CORNER();
	void Show_CORNERHELP();
	void Hide_CORNERHELP();
	void Show_MULTI();
	void Hide_MULTI();
	void Show_MULTIHELP();
	void Hide_MULTIHELP();
	void Show_MOVETO();
	void Hide_MOVETO();
	void Show_MOVETOHELP();
	void Hide_MOVETOHELP();
	void Show_AUTO();
	void Hide_AUTO();
	void Show_AUTOHELP();
	void Hide_AUTOHELP();
	void Show_SPIRAL();
	void Hide_SPIRAL();
	void Show_SPIRALHELP();
	void Hide_SPIRALHELP();
	void Show_WALL();
	void Hide_WALL();
	void Show_WALLHELP();
	void Hide_WALLHELP();
//地圖顯示編輯部分
	void MAPZoomDefault(int iPagIdx);//縮放回復到預設	
	void LoadGUIMAPData(Sprite *pSpriteMap);//載入地圖
	float MAPReSetSize(Sprite* pMag,float fSize); //設定地圖縮放
	void MAPRetunZero(int iPgIdx);		//回歸原點
	void MAPZoomIn(int iPgIdx);
	void MAPZoomOut(int iPgIdx);
	void MAPZoomLabUpdate(int iPagIdx);
	void InitPageArge();
	void MapMove(Point nodePosition );
	void MapRangMove(Point MousePosition );
	int GetPageIDxFromPageType();
	void AddMAPRang_OVAL();	
	void AddMAPRang_RECL();
	int AddMAPRang_PEN(Point StartSite);	// 增加畫筆
	int  SelMapRang(Point); //判斷點選的位置是否是在範圍元件上
	void SelMapRang_DrawBlackBox(DrawNode *pRang);  //去除點選紅框 改為黑框
	void SelMapRang_DrawRedBox(DrawNode *pRang);	
	void SelMapRang_UnSel();//檢查是否有點選 框 將其改為未點選

	void  DeleteMapRang();	//刪除指定元件
	void  UndoMapRang();	//回到上一步指定元件
	void  DrawMapRang(DrawNode *pRang);
	void  DrawMapRangByPEN(Point StartSite);// 畫線
	void  DrawMapRang_RealRangByType(DrawNode *pRang,pMapRangInfo pRangInfo);//繪製實際範圍(粉紅色區域)
	void  MapRangReRoation(Point MousePosition );
	void  MapRangReSize_H(Point MousePosition );
	void  MapRangReSize_V(Point MousePosition );
	void  ResetContentSizeRang(DrawNode *pRang);//重新設定點選範圍
	void  RobotGo(int iScanType);	
	void  RobotGo_Target();//移動到指定地點

	//範圍記錄管理
	void InitRangInfo();	
	inline void InitOneRangInfo(int iIdx);
	int  GetNewRangSpace();//取得新的紀錄空間	
	void DelRangSpace(int iIdx);//刪除所有記錄
	int  SaveNewRangInfo(int iWidth,int iHeight,int iAngle,Point stPoint,int iType); // 建立一組新數據
	int  GetMAXMAPZord(Sprite *pSprite);
	void SaveRangStateToInfo();
	inline int GetMapRangTag();
	inline void InitMapRangTag();
	inline void SetMapRangTag(int iValue);
	inline bool IsMapRangTagReady();
	void MapRangToArray();	
	void MapPenToArray();
	

};


#endif // __UICLENJOB_H__