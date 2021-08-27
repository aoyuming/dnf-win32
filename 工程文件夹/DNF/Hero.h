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

//英雄的状态
enum HeroState : char 
{
	_HERO_STAND,//站立
	_HERO_MOVE_,//移动
	_HERO_MOVE_SHOOTING_MOVE,//移动射击状态下移动
	_HERO_MOVE_SHOOT,//移动射击
	_HERO_MOVE_ATTACK,//移动射击攻击
	_HERO_END_MOVESHOOTINGK,//停止移动射击
	_HERO_MOVE_SHOOTING_DIR,//变换移动射击方向
	_HERO_RUSH,//奔跑
	_HERO_WEI_XING_SHE_XIAN,//卫星射线
	_HERO_JIANG_ZHI,//僵直状态
	_HERO_DEATH,//死亡状态
	_HERO_PUTONGGONGJI//普通攻击
};

//设置贴图与状态
struct State_PicBeg_PicEnd
{
	bool SetPic;
	bool state;
};

//所有角色的父类
class CHero
{
	
	//英雄的状态
	struct STATE
	{
		int	state;//状态
		bool flag;//上一次状态是否还是state
	};


public:
	static int m_HeroNum;//英雄个数
	double m_iHp,m_iMp;//血量，蓝量
	int m_iX,m_iY;//坐标
	char m_cName[32];//角色名字
	int m_bPicdir;//贴图方向  
	int m_iPicW;//贴图高
	int m_iPicH;//贴图宽
	RECT m_rRect;//碰撞范围
	time_t m_tIntervalTimeBegin;//间隔时间开始
	time_t m_tIntervalTimeEnd;//间隔时间结束
	std::string m_FileName;//文件名称
	CHcan m_Hcan;//shou pao
	int m_iWXSXpicBegin;//卫星射线贴图开始
	int m_iWXSXpicEnd;//卫星射线贴图开始

	bool m_bFangZhu;//是否是房主
	bool m_bZhunBei;//是否准备
	bool m_bDeath;//是否死亡
	int m_iPicbegin;
	int m_iPicEnd;

	int _WORLD_W_;
	int _WORLD_H_;
	int m_iScene;//场景
	int m_iNum;//子弹数量
	bool m_bWXSXCD;//CD是否结束
	int m_dangqianCDTimer;//当前CD时间
	bool m_bRush;//是否处于奔跑状态
	STATE m_Hero_State;//英雄的状态
	bool m_Inputflag;//待机输入标记
	int m_iMoveDir;//移动方向
	bool m_YiDongSheJiAnXia;//移动射击是否按下
	bool m_bStand;//待机状态
	bool m_bMove;//移动状态
	bool m_bShooting_move[3];//移动射击状态移动 0 表示进入移动射击 1表示移动射击状态下移动 2表示移动射击状态攻击
	bool m_WXSX_anxia;//卫星射线是否按下
	bool m_WXSX_luoxia;
	int  m_WXSX_X;//卫星射线x
	int	 m_WXSX_Y;//卫星射线y
	bool m_iJiangZhiZT;//僵直状态
	int  m_iJiangZhiDu;//僵直度
	int	 m_iJiangZhiDuBegin;//僵直时间
	State_PicBeg_PicEnd m_PuTongGongJi;//普通攻击

	std::vector<CSkill_Move_Shooting*> m_Move_Shooting;//移动射击子弹表
	

	CHero(int x,int y,int hp,int mp,char* name,int dir,int w,int h);//,int scene);
	~CHero();

	//普通攻击
	void PuTongGongJi();

	//死亡
	void death();
	//僵直状态
	void JiangZhiZhuangTai();
	//英雄奔跑
	void Rush();
	//卫星射线
	void WeiXingSheXian();
	//英雄移动
	void Heroyidong(int dir);
	//绘制动作
	void DrawMotion(int index);
	//移动
	void Move(int dir);
	//运行
	void Run();
	//发动移动射击
	void Move_Shooting();
	//移动射击状态
	void Move_Shooting_State(int dir);
	//移动射击状态
	void Move_Shooting_Dir();
	//移动射击攻击
	void MoveShootingAttack();
	//停止移动射击
	void End_MoveShootingk();
	//待机站立状态
	void Standby();
	//贴血量蓝量
	void TieHP_MP(int x,int y);

	////获取移动射击攻击范围
	//RECT GetMoveShootingAttackRect();
	//获取血量
	double GetHP();
	//设置血量
	void SetHP(double hp);
	//获取蓝量
	double GetMP();
	//设置蓝量
	void SetMP(double mp);
	//获取坐标X
	int GetX();
	//设置坐标X
	void SetX(int x);
	//获取坐标Y
	int GetY();
	//设置坐标X
	void SetY(int y);
	//获取名字
	char* GetName();
	//设置名字
	void SetName(char* name);
	//获取贴图方向
	int GetPicDir();
	//设置贴图方向
	void SetPicDir(bool dir);
	//获取贴图宽
	int GetPicW();
	//设置贴图宽
	void SetPicW(int w);
	//获取贴图高
	int GetPicH();
	//设置贴图高
	void SetPicH(int  h);
	//获取贴图碰撞范围
	RECT GetPicRect();




};

#endif