#ifndef SKILL_MOVE_SHOOTING_H
#define SKILL_MOVE_SHOOTING_H
#include "Skill.h"
#include "define_.h"
#include <windows.h>

class CSkill_Move_Shooting : public CSkill
{

public:
	CSkill_Move_Shooting(int x,int y,int begpic,int endpic,int harm,int image,int h,int w);
	std::string m_FileName;//�ļ�����
	time_t m_tIntervalTimeBegin;//���ʱ�俪ʼ
	time_t m_tIntervalTimeEnd;//���ʱ�����
	static int m_iNum;//�ӵ�����
	static bool m_iCD;//CD�Ƿ����
	static int m_dangqianCDTimer;//��ǰCDʱ��
	RECT GetRect();//��ȡ������ײ��Χ
	void Run();
};


#endif