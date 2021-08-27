
#include "GameEngine.h"
#include "bullet.h"
CBullet::CBullet(int x,int y,int h,int w,int picbeg,int picend,int harm,bool image)
:
m_iX(x),
m_iY(y),
m_iPicW(w),
m_iPicH(h),
m_PicBegin(picbeg),
m_PicEnd(picend),
m_iHarm(harm),
m_bImage(image),
m_bDlete(false),
m_iFuKongLi(20)
{
	m_iyidongNum = 0;
	m_iyidongMax = 200;


};

void CBullet::Run()
{
	//设置矩形范围
	m_rRect.top = m_iY;
	m_rRect.bottom = m_iY + m_iPicH;
	m_rRect.left = m_iX;
	m_rRect.right = m_iX + m_iPicW;
	
	if(m_bImage)
		m_iX -= 25;
	else
		m_iX += 25;
	
	if(m_iyidongNum >= m_iyidongMax)
		m_bDlete = true;
	else
		m_iyidongNum += 25;

	//贴图
	char buf[20];
	std::string s  = "pic\\格林子弹\\";
	itoa(m_PicBegin,buf,10);
	s += buf;
	s += ".png";
	std::map<std::string,File_W_H>::iterator j = g_FileName.find(s);
	int w = j->second.w;
	int h = j->second.h;

	//绘制人物
	CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),g_mapX + m_iX,m_iY,w,h,0,0,m_bImage);

	

};