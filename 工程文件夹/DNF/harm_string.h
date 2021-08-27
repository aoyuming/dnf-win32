#ifndef HARM_STRING_H
#define HARM_STRING_H
#include "define_.h"
#include "GameEngine.h"

//技能伤害字符串
class CHram_String
{
	int m_iX,m_iY;//坐标
	int m_iHarm;//伤害最小值
	int m_iHarmMax;//伤害最大值
public:
	int m_Y;
	bool m_delete;//是否销毁
	CHram_String(int x,int y,int harm);
	void Run();
};


#endif