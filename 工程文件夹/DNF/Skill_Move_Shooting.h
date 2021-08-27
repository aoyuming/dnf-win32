#ifndef SKILL_MOVE_SHOOTING_H
#define SKILL_MOVE_SHOOTING_H
#include "Skill.h"
#include "define_.h"
#include <windows.h>

class CSkill_Move_Shooting : public CSkill
{

public:
	CSkill_Move_Shooting(int x,int y,int begpic,int endpic,int harm,int image,int h,int w);
	std::string m_FileName;//文件名称
	time_t m_tIntervalTimeBegin;//间隔时间开始
	time_t m_tIntervalTimeEnd;//间隔时间结束
	static int m_iNum;//子弹数量
	static bool m_iCD;//CD是否结束
	static int m_dangqianCDTimer;//当前CD时间
	RECT GetRect();//获取矩形碰撞范围
	void Run();
};


#endif