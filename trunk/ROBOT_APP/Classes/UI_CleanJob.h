#ifndef __UICLENJOB_H__
#define __UICLENJOB_H__
#include "cocos2d.h"
using namespace cocos2d;
//��������
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

	CLEJOBPG_SIZE, //�����ƶq
	CLEJOBPG_LAYER, //�L��ܭ���
};

enum _CLEANJOBMAINITEM_
{
	CLEJOBMAIN_BK=0,
	CLEJOBMAIN_TITAL,
	CLEJOBMAIN_CORNER,
	CLEJOBMAIN_LABCORNER,
	CLEJOBMAIN_MULTI,
	CLEJOBMAIN_LABMULTI,
	CLEJOBMAIN_MOVE,
	CLEJOBMAIN_LABMOVE,
	CLEJOBMAIN_AUTO,
	CLEJOBMAIN_LABAUTO,
	CLEJOBMAIN_SPIRAL,
	CLEJOBMAIN_LABSPIRAL,
	CLEJOBMAIN_WALL,
	CLEJOBMAIN_LABWALL,
	CLEJOBMAIN_SIZE,
	CLEJOBMAIN_BTNSPACENUM=2, //�C�ӭI���W������ƶq
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

//�C�ӫ����m

enum _CLEANJOBPGIDXETYPE_
{
	JOBIDX_CORNER,
	JOBIDX_MULTI,
	JOBIDX_MOVETO,
	JOBIDX_AUTO,
	JOBIDX_SPIRAL,
	JOBIDX_WALL,
	JOBIDX_FUNCTIONNUM,	//�����ƶq
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
//�ǰe��robot server �a�Ϫ�index
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
//�e�ت��ܼ�
typedef struct stMapRangInfo
{	
	bool bUse;	//�Ŷ��Q�ϥ�	
	bool bSel;	//�Q�I�襤
	bool bReserver1;	//�w�ƺX��
	bool bReserver2;	//�w�ƺX��
	
	int ID;		//�s��
	int iType;	//�ϫ�����
	
	float fX;	//�ϧΥ��U��
	float fY;	
	float fEdgeLeft;	//��ؼƾ�
	float fEdgeTop;
	float fEdgeRight;
	float fEdgeDown;

	int iBefIdx;	//�W�@�B
	int iWidth;		//��
	int iHeight;	//�e
	int iAngle;		//����
}MapRangInfo,*pMapRangInfo;
const int G_DRAWPENTAG = 1;
class UI_CLEANJOB: public CCLayer
{
private:  
	int PageType;
	float fZoomRate;
	//����ܼ�
	bool bMoveKeep;
	CCPoint StartPoint_Mouse;
	CCPoint StartPoint_Map;
	CCPoint StartPoint_Rang;
	CCPoint StartPoint_Target;
	//CCPoint MinPenPoint;
	//CCPoint MaxPenPoint;
	int StartPoint_RangWidth;
	int StartPoint_RangHeight;
	//�I��d���ܼ�
	int iRangTagIdx;
	int iRangBtnType;
	int iMaxZord;
	int iChangRangFlag;
	CCSize winSize;  
	CCArray* m_BtnArray[CLEJOBMAIN_SIZE];//�s��Ҧ����s
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

	void BtnColorSetting(int iBtnIdx,bool bSel);//���s�ܦ�
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	
;//�]�w�e�����s
public:
	CREATE_FUNC(UI_CLEANJOB); 
	virtual bool init();  
	UI_CLEANJOB();
	~UI_CLEANJOB();
	void Function_CLEANJOBMAIN(int iBtnIdx);
	void Function_CORNER(int iBtnIdx);
	void Function_MULTI(int iBtnIdx);
	void Function_MOVETO(int iBtnIdx);
	void Function_AUTO(int iBtnIdx);
	void Function_SPIRAL(int iBtnIdx);
	void Function_WALL(int iBtnIdx);
	
	void CleanMAPRang(int iPagIdx);//�M�Ų{�b�e���W�Ҧ�����
	void MouseDown_Function(CCPoint  nodePosition);	
	void MAPRangItemCheck();//�ˬd�Хܽd���I��
	void TargetMove_ByMouse(CCSprite *pSprite,CCPoint MousePosition ); //�ϰv����
	void TargetMove(CCPoint  nodePosition);
	void TargetSet(CCPoint  nodePosition);
	void TargetPointSave();			//�����{�b�ϰv��m
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
//�a����ܽs�賡��
	void MAPZoomDefault(int iPagIdx);//�Y��^�_��w�]	
	void LoadGUIMAPData(CCSprite *pSpriteMap);//���J�a��
	float MAPReSetSize(CCSprite* pMag,float fSize); //�]�w�a���Y��
	void MAPRetunZero(int iPgIdx);		//�^�k���I
	void MAPZoomIn(int iPgIdx);
	void MAPZoomOut(int iPgIdx);
	void MAPZoomLabUpdate(int iPagIdx);
	void InitPageArge();
	void MapMove(CCPoint nodePosition );
	void MapRangMove(CCPoint MousePosition );
	int GetPageIDxFromPageType();
	void AddMAPRang_OVAL();	
	void AddMAPRang_RECL();
	int AddMAPRang_PEN(CCPoint StartSite);	// �W�[�e��
	int  SelMapRang(CCPoint); //�P�_�I�諸��m�O�_�O�b�d�򤸥�W
	void SelMapRang_DrawBlackBox(CCDrawNode *pRang);  //�h���I����� �אּ�®�
	void SelMapRang_DrawRedBox(CCDrawNode *pRang);	
	void SelMapRang_UnSel();//�ˬd�O�_���I�� �� �N��אּ���I��

	void  DeleteMapRang();	//�R�����w����
	void  UndoMapRang();	//�^��W�@�B���w����
	void  DrawMapRang(CCDrawNode *pRang);
	void  DrawMapRangByPEN(CCPoint StartSite);// �e�u
	void  DrawMapRang_RealRangByType(CCDrawNode *pRang,pMapRangInfo pRangInfo);//ø�s��ڽd��(������ϰ�)
	void  MapRangReRoation(CCPoint MousePosition );
	void  MapRangReSize_H(CCPoint MousePosition );
	void  MapRangReSize_V(CCPoint MousePosition );
	void  ResetContentSizeRang(CCDrawNode *pRang);//���s�]�w�I��d��
	void  RobotGo(int iScanType);	
	void  RobotGo_Target();//���ʨ���w�a�I

	//�d��O���޲z
	void InitRangInfo();	
	inline void InitOneRangInfo(int iIdx);
	int  GetNewRangSpace();//���o�s�������Ŷ�	
	void DelRangSpace(int iIdx);//�R���Ҧ��O��
	int  SaveNewRangInfo(int iWidth,int iHeight,int iAngle,CCPoint stPoint,int iType); // �إߤ@�շs�ƾ�
	int  GetMAXMAPZord(CCSprite *pSprite);
	void SaveRangStateToInfo();
	inline int GetMapRangTag();
	inline void InitMapRangTag();
	inline void SetMapRangTag(int iValue);
	inline bool IsMapRangTagReady();
	void MapRangToArray();	
	void MapPenToArray();
	

};


#endif // __UICLENJOB_H__