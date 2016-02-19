#ifndef __UIMAP_H__
#define __UIMAP_H__
#include "cocos2d.h"
using namespace cocos2d;
//��������
enum _MAPPGETYPE_
{
	MAPPG_MAIN,
	MAPPG_SIZE, //�����ƶq
	MAPPG_LAYER //�L��ܭ���
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
	float fZoomRate; //�Y�񭿲v
	int iRelseTime; //��s�ɶ�
	CCPoint StartPoint_Mouse;
	CCPoint StartPoint_Map;
	CCPoint StartPoint_Target;
	CCTexture2D *pMapTexture; 

	CCArray* m_BtnArray[MAPMAIN_SIZE];//�s��Ҧ����s
	void InitUI();
	void InitUI_MAPMAIN();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void SetupUIBtn(CCSprite* pSprite,bool bOnOff);
	
;//�]�w�e�����s
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
	//�a�ϲ���
	void MapMove(CCPoint nodePosition );
	void MAPZoomIn();
	void MAPZoomOut();
	void MAPZoomDefault();
	void MAPZoomLabUpdate(); //��s���v�r��
	float MAPReSetSize(float fSize); //�]�wMAP �j�p
	void update(float dt);  
	
	void MAPRetunZero();//�^�k���I

	void Function_UIMAP_START(int iBtnIdx);
	void ReloadRobotMap();	//���s���J�a��
	void UpdateMAPSchedule(float ftime); //��s�a��

};


#endif // __UIMAP_H__