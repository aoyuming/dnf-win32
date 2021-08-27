#include "SelectUI.h"
#include "Scene.h"
#include "GameEngine.h"

#define _LR_W 25

CSelectUI::CSelectUI(const char* t, int min, int max, int cur, int x, int y, int w, int h, bool v, bool a)
:
CUI(x, y, w, h, v, a),
m_Min(min),
m_Max(max),
m_Cur(cur),
m_Text(t),
m_In(false),
m_Down(false),
m_Pos(0)
{}

//virtual void Init();
void CSelectUI::ActRender()
{
	CGameOutput* pGO = CGameEngine::GetGE()->GetGO();

	if (m_In)
	{
		pGO->DrawRectangle2(
			m_X + _LR_W,
			m_Y,
			m_X + m_W - _LR_W,
			m_Y + m_H);
		pGO->DrawLine(m_X, m_Y + m_H / 2, m_X + _LR_W, m_Y);
		pGO->DrawLine(m_X, m_Y + m_H / 2, m_X + _LR_W, m_Y + m_H);
		pGO->DrawLine(m_X + m_W, m_Y + m_H / 2, m_X + m_W - _LR_W, m_Y);
		pGO->DrawLine(m_X + m_W, m_Y + m_H / 2, m_X + m_W - _LR_W, m_Y + m_H);

		char buf[256];
		sprintf_s(buf, 256, "%s %d", m_Text.c_str(), m_Cur);
		pGO->DrawString3(0, m_X + _LR_W, m_Y, m_W - 2 * _LR_W, m_H, buf, RGB(0, 0, 0));
	}
	else
	{
		pGO->DrawRectangle2(
			m_X + _LR_W,
			m_Y,
			m_X + m_W - _LR_W,
			m_Y + m_H,
			RGB(0, 0, 0),
			RGB(175, 175, 175));
		pGO->DrawLine(m_X, m_Y + m_H / 2, m_X + _LR_W, m_Y);
		pGO->DrawLine(m_X, m_Y + m_H / 2, m_X + _LR_W, m_Y + m_H);
		pGO->DrawLine(m_X + m_W, m_Y + m_H / 2, m_X + m_W - _LR_W, m_Y);
		pGO->DrawLine(m_X + m_W, m_Y + m_H / 2, m_X + m_W - _LR_W, m_Y + m_H);

		char buf[256];
		sprintf_s(buf, 256, "%s %d", m_Text.c_str(), m_Cur);
		pGO->DrawString3(0, m_X + _LR_W, m_Y, m_W - 2 * _LR_W, m_H, buf, RGB(0, 0, 0));
	}

	//渲染函数是每次游戏循环都会调用的,在特殊的控件中可能
	//会被一些逻辑代码写在这个地方
	if (m_In && m_Down)
	{
		if (-1 == m_Pos)
		{
			if (m_Cur > m_Min)
			{
				m_Cur -= 1;
				m_Scene->OnUIMsg(m_Name.c_str(), _SELECT_NUM_CHANGE);
			}
		}
		else if (1 == m_Pos)
		{
			if (m_Cur < m_Max)
			{
				m_Cur += 1;
				m_Scene->OnUIMsg(m_Name.c_str(), _SELECT_NUM_CHANGE);
			}
		}
	}
}

void CSelectUI::UnactRender()
{
	CGameOutput* pGO = CGameEngine::GetGE()->GetGO();

	pGO->DrawRectangle2(
		m_X + _LR_W,
		m_Y,
		m_X + m_W - _LR_W,
		m_Y + m_H,
		RGB(0, 0, 0),
		RGB(175, 175, 175));
	pGO->DrawLine(m_X, m_Y + m_H / 2, m_X + _LR_W, m_Y);
	pGO->DrawLine(m_X, m_Y + m_H / 2, m_X + _LR_W, m_Y + m_H);
	pGO->DrawLine(m_X + m_W, m_Y + m_H / 2, m_X + m_W - _LR_W, m_Y);
	pGO->DrawLine(m_X + m_W, m_Y + m_H / 2, m_X + m_W - _LR_W, m_Y + m_H);

	char buf[256];
	sprintf_s(buf, 256, "%s %d", m_Text.c_str(), m_Cur);
	pGO->DrawString3(0, m_X + _LR_W, m_Y, m_W - 2 * _LR_W, m_H, buf, RGB(100, 100, 100));
}

void CSelectUI::OnMsg(int Msg, int p1, int p2)
{
	switch (Msg)
	{
	case _UIM_C_ENTER:
	case _UIM_C_MOVE_IN:
		{
			m_In = true;
			break;
		}
	case _UIM_C_QUIT:
	case _UIM_C_MOVE_OUT:
		{
			m_In = false;
			break;
		}
	case _UIM_C_LBD_IN:
		{
			m_Down = true;

			m_Pos = 0;
			if (p1 < _LR_W)
				m_Pos = -1;
			else if (p1 > m_W - _LR_W)
				m_Pos = 1;

			break;
		}
	case _UIM_C_LBU_IN:
	case _UIM_C_LBU_OUT:
		{
			m_Down = false;
			break;
		}
	}
}

void CSelectUI::SetText(const char* t)
{
	m_Text = t;
}

const char* CSelectUI::GetText()
{
	return m_Text.c_str();
}

bool CSelectUI::SetCur(int cur)
{
	if (cur < m_Min || cur > m_Max)
		return false;

	m_Cur = cur;
	return true;
}

int CSelectUI::GetCur()
{
	return m_Cur;
}