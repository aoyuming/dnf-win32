#include "harm_string.h"

CHram_String::CHram_String(int x,int y,int hram)
	:
	m_iX(x),
	m_iY(y),
	m_iHarm(hram),
	m_delete(false)
{
	m_iHarmMax =  m_iHarm;
	m_Y = m_iY;
}
void CHram_String::Run()
{
	char buf[32];
	itoa(m_iHarmMax,buf,10);
	//绘制名字
	CGameEngine::GetGE()->GetGO()->DrawString1("决斗场景字体",g_mapX +  m_iX, m_iY,buf, RGB(255, 255, 255));

	
	m_iY -= 3;
	if(m_iY < m_Y - 100)
		m_delete = true;

	
}