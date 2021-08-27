#ifndef _HCAN_H
#define _HCAN_H
#include "define_.h"
#include "GameEngine.h"


//英雄的状态
enum HcanState : char 
{
	_HCAN_STAND,//站立
	_HCAN_MOVE_SHOOT,//移动射击状态
};
class CHero;
class CHcan
{
	int m_iX,m_iY;//坐标
	int m_iPicBegin;//贴图开始的索引
	int m_iPicEnd;//贴图结束的索引
	int m_iPicW;//贴图高
	int m_iPicH;//贴图宽
	int m_bImage;//镜像
	int m_iState;//状态
	time_t m_tIntervalTimeBegin;//间隔时间开始
	time_t m_tIntervalTimeEnd;//间隔时间结束
	std::string m_FileName;//文件名称
public:
	CHcan(int x,int y,int begin,int end,int dir);
	void Run();
	//绘制动作
	void DrawMotion(int index);
	//待机
	void daiji();
	//移动射击状态
	void Move_Shooting_State();

	friend class CHero;
};



#endif