#ifndef __UISAMPLE_H__
#define __UISAMPLE_H__
#include "cocos2d.h"
using namespace cocos2d;
//��������
enum _SAMPLEPGETYPE_
{
	SAMPLEPG_MAIN,
	SAMPLEPG_SIZE, //�����ƶq
	SAMPLEPG_LAYER //�L��ܭ���
};
enum _SAMPLEMAINITEM_
{
	SAMPLEMAIN_BK=0,
	SAMPLEMAIN_TITAL,
	SAMPLEMAIN_CENTERICON,
	SAMPLEMAIN_SIZE
};
class UI_SAMPLE: public CCLayer
{
private:  
	int PageType;
	CCSize winSize;  
	CCArray* m_BtnArray[SAMPLEMAIN_SIZE];//�s��Ҧ����s
	void InitUI();
	void InitUI_SAMPLEMAIN();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	void SetupUIBtn(CCSprite* pSprite,bool bOnOff);
	
;//�]�w�e�����s
public:
	CREATE_FUNC(UI_SAMPLE); 
	virtual bool init();  
	UI_SAMPLE();
	~UI_SAMPLE();
	void Function_UISAMPLE(int iBtnIdx);
	void ShowLayer();
	void HideLayer();	
	void Hide_AllPage();
	void Show_SAMPLEMAIN();
	void Hide_SAMPLEMAIN();
};


#endif // __UISAMPLE_H__