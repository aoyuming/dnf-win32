#ifndef _SKILL_H
#define _SKILL_H
#include "GameEngine.h"
#include <time.h>

//���ܸ���
class CSkill
{
public:
	int m_iX,m_iY;//����
	int m_iPicW;//��ͼ��
	int m_iPicH;//��ͼ��
	int m_iPicBegin;//��ͼ��ʼ�±�
	int m_iPicEnd;//��ͼ�����±�
	RECT m_Rect;//��ײ��Χ
	int m_iHarm;//�����˺�
	int m_bImage;//����
	//static bool m_iCD;//��ȴʱ��
	CSkill(int x,int y,int begpic,int endpic,int harm,int image,int h,int w);
	RECT GetRect();
	virtual void Run();//����
};

#endif
