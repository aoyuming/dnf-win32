#ifndef BULLET_H
#define BULLET_H
#include "define_.h"

//�ӵ���
class CBullet
{
public:
	int m_iX;
	int m_iY;
	int m_PicBegin;
	int m_PicEnd;
	RECT m_rRect;
	bool m_bDlete;
	int m_iHarm;
	bool m_bImage;
	int m_iFuKongLi;
	int m_iPicW;//��ͼ��
	int m_iPicH;//��ͼ��
	int m_iyidongNum;
	int m_iyidongMax;

	CBullet(int x,int y,int h,int w,int picbeg,int picend,int harm,bool image);

	void Run();


};






#endif