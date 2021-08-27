#include "Enter2.h"
#include "GameEngine.h"

CEnter2::CEnter2()
:
CSceneChange(16)
{
	m_Dark_Level[0] = 255;
	m_Dark_Level[1] = 230;
	m_Dark_Level[2] = 210;
	m_Dark_Level[3] = 190;
	m_Dark_Level[4] = 170;
	m_Dark_Level[5] = 150;
	m_Dark_Level[6] = 130;
	m_Dark_Level[7] = 110;
	m_Dark_Level[8] = 100;
	m_Dark_Level[9] = 90;
	m_Dark_Level[10] = 80;
	m_Dark_Level[11] = 70;
	m_Dark_Level[12] = 50;
	m_Dark_Level[13] = 30;
	m_Dark_Level[14] = 15;
	m_Dark_Level[15] = 0;
}

void CEnter2::Render(int CurFrame)
{
	//1~16
	CGameEngine::GetGE()->GetGO()->SetDark(m_Dark_Level[CurFrame - 1]);
}