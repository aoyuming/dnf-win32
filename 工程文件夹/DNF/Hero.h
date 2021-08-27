#ifndef _HERO_H
#define _HERO_H
#include "GameEngine.h"
#include "define_.h"
#include <time.h>
#include "Skill_Move_Shooting.h"
#include "hcan.h"
#include "harm_string.h"
#include <vector>
#include "Vector2D.h"


#define _CLIENT_W_ 940
#define _CLIENT_H_ 556

//#define _WORLD_W_ (890)
//#define _WORLD_H_ (556)

//Ӣ�۵�״̬
enum HeroState : char 
{
	_HERO_STAND,//վ��
	_HERO_MOVE_,//�ƶ�
	_HERO_MOVE_SHOOTING_MOVE,//�ƶ����״̬���ƶ�
	_HERO_MOVE_SHOOT,//�ƶ����
	_HERO_MOVE_ATTACK,//�ƶ��������
	_HERO_END_MOVESHOOTINGK,//ֹͣ�ƶ����
	_HERO_MOVE_SHOOTING_DIR,//�任�ƶ��������
	_HERO_RUSH,//����
	_HERO_WEI_XING_SHE_XIAN,//��������
	_HERO_JIANG_ZHI,//��ֱ״̬
	_HERO_DEATH,//����״̬
	_HERO_PUTONGGONGJI//��ͨ����
};

//������ͼ��״̬
struct State_PicBeg_PicEnd
{
	bool SetPic;
	bool state;
};

//���н�ɫ�ĸ���
class CHero
{
	
	//Ӣ�۵�״̬
	struct STATE
	{
		int	state;//״̬
		bool flag;//��һ��״̬�Ƿ���state
	};


public:
	static int m_HeroNum;//Ӣ�۸���
	double m_iHp,m_iMp;//Ѫ��������
	int m_iX,m_iY;//����
	char m_cName[32];//��ɫ����
	int m_bPicdir;//��ͼ����  
	int m_iPicW;//��ͼ��
	int m_iPicH;//��ͼ��
	RECT m_rRect;//��ײ��Χ
	time_t m_tIntervalTimeBegin;//���ʱ�俪ʼ
	time_t m_tIntervalTimeEnd;//���ʱ�����
	std::string m_FileName;//�ļ�����
	CHcan m_Hcan;//shou pao
	int m_iWXSXpicBegin;//����������ͼ��ʼ
	int m_iWXSXpicEnd;//����������ͼ��ʼ

	bool m_bFangZhu;//�Ƿ��Ƿ���
	bool m_bZhunBei;//�Ƿ�׼��
	bool m_bDeath;//�Ƿ�����
	int m_iPicbegin;
	int m_iPicEnd;

	int _WORLD_W_;
	int _WORLD_H_;
	int m_iScene;//����
	int m_iNum;//�ӵ�����
	bool m_bWXSXCD;//CD�Ƿ����
	int m_dangqianCDTimer;//��ǰCDʱ��
	bool m_bRush;//�Ƿ��ڱ���״̬
	STATE m_Hero_State;//Ӣ�۵�״̬
	bool m_Inputflag;//����������
	int m_iMoveDir;//�ƶ�����
	bool m_YiDongSheJiAnXia;//�ƶ�����Ƿ���
	bool m_bStand;//����״̬
	bool m_bMove;//�ƶ�״̬
	bool m_bShooting_move[3];//�ƶ����״̬�ƶ� 0 ��ʾ�����ƶ���� 1��ʾ�ƶ����״̬���ƶ� 2��ʾ�ƶ����״̬����
	bool m_WXSX_anxia;//���������Ƿ���
	bool m_WXSX_luoxia;
	int  m_WXSX_X;//��������x
	int	 m_WXSX_Y;//��������y
	bool m_iJiangZhiZT;//��ֱ״̬
	int  m_iJiangZhiDu;//��ֱ��
	int	 m_iJiangZhiDuBegin;//��ֱʱ��
	State_PicBeg_PicEnd m_PuTongGongJi;//��ͨ����

	std::vector<CSkill_Move_Shooting*> m_Move_Shooting;//�ƶ�����ӵ���
	

	CHero(int x,int y,int hp,int mp,char* name,int dir,int w,int h);//,int scene);
	~CHero();

	//��ͨ����
	void PuTongGongJi();

	//����
	void death();
	//��ֱ״̬
	void JiangZhiZhuangTai();
	//Ӣ�۱���
	void Rush();
	//��������
	void WeiXingSheXian();
	//Ӣ���ƶ�
	void Heroyidong(int dir);
	//���ƶ���
	void DrawMotion(int index);
	//�ƶ�
	void Move(int dir);
	//����
	void Run();
	//�����ƶ����
	void Move_Shooting();
	//�ƶ����״̬
	void Move_Shooting_State(int dir);
	//�ƶ����״̬
	void Move_Shooting_Dir();
	//�ƶ��������
	void MoveShootingAttack();
	//ֹͣ�ƶ����
	void End_MoveShootingk();
	//����վ��״̬
	void Standby();
	//��Ѫ������
	void TieHP_MP(int x,int y);

	////��ȡ�ƶ����������Χ
	//RECT GetMoveShootingAttackRect();
	//��ȡѪ��
	double GetHP();
	//����Ѫ��
	void SetHP(double hp);
	//��ȡ����
	double GetMP();
	//��������
	void SetMP(double mp);
	//��ȡ����X
	int GetX();
	//��������X
	void SetX(int x);
	//��ȡ����Y
	int GetY();
	//��������X
	void SetY(int y);
	//��ȡ����
	char* GetName();
	//��������
	void SetName(char* name);
	//��ȡ��ͼ����
	int GetPicDir();
	//������ͼ����
	void SetPicDir(bool dir);
	//��ȡ��ͼ��
	int GetPicW();
	//������ͼ��
	void SetPicW(int w);
	//��ȡ��ͼ��
	int GetPicH();
	//������ͼ��
	void SetPicH(int  h);
	//��ȡ��ͼ��ײ��Χ
	RECT GetPicRect();




};

#endif