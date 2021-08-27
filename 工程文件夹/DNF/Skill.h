#ifndef _SKILL_H
#define _SKILL_H
#include "GameEngine.h"
#include <time.h>

//技能父类
class CSkill
{
public:
	int m_iX,m_iY;//坐标
	int m_iPicW;//贴图高
	int m_iPicH;//贴图宽
	int m_iPicBegin;//贴图开始下标
	int m_iPicEnd;//贴图结束下标
	RECT m_Rect;//碰撞范围
	int m_iHarm;//技能伤害
	int m_bImage;//镜像
	//static bool m_iCD;//冷却时间
	CSkill(int x,int y,int begpic,int endpic,int harm,int image,int h,int w);
	RECT GetRect();
	virtual void Run();//运行
};

#endif
