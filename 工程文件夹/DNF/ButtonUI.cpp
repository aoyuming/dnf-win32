#include "ButtonUI.h"
#include "Scene.h"
#include "GameEngine.h"

CButtonUI::CButtonUI(const char* t, int x, int y, int w, int h, bool v, bool a)
:
CUI(x, y, w, h, v, a),
m_ButtonState(_BUTTON_STATE_CURSOR_OUT),
m_DownIn(false),
m_Text(t)
{}

void CButtonUI::ActRender()
{
	CGameOutput* pGO = CGameEngine::GetGE()->GetGO();

	RECT r[]
	=
	{
		{m_X, m_Y, m_X + m_W - 1, m_Y + m_H - 1},
		{m_X + 1, m_Y + 1, m_X + m_W, m_Y + m_H},
	};

	pGO->DrawRectangle2(
		m_X, m_Y, m_X + m_W, m_Y + m_H);

	switch (m_ButtonState)
	{
	case _BUTTON_STATE_CURSOR_OUT:
		{
			pGO->DrawRectangle2(
				r[0].left, r[0].top, r[0].right, r[0].bottom,
				RGB(0, 0, 0),
				RGB(175, 175, 175));

			break;
		}
	case _BUTTON_STATE_CURSOR_IN_UP:
		{
			pGO->DrawRectangle2(
				r[0].left, r[0].top, r[0].right, r[0].bottom);

			break;
		}
	case _BUTTON_STATE_CURSOR_IN_DOWN:
		{
			pGO->DrawRectangle2(
				r[1].left, r[1].top, r[1].right, r[1].bottom);
			break;
		}
	}

	pGO->DrawString3(
		0,
		m_X, m_Y, m_W, m_H,
		m_Text.c_str(),
		RGB(0, 0, 0));
}

void CButtonUI::UnactRender()
{
	CGameOutput* pGO = CGameEngine::GetGE()->GetGO();

	pGO->DrawRectangle2(
		m_X, m_Y, m_X + m_W, m_Y + m_H,
		RGB(0, 0, 0),
		RGB(175, 175, 175));
	pGO->DrawString3(
		0,
		m_X, m_Y, m_W, m_H,
		m_Text.c_str(),
		RGB(100, 100, 100));
}

void CButtonUI::OnMsg(int Msg, int p1, int p2)
{
	switch (Msg)
	{
	case _UIM_C_LBD_IN:
		{
			m_ButtonState = _BUTTON_STATE_CURSOR_IN_DOWN;
			m_DownIn = true;
			break;
		}
	case _UIM_C_LBU_IN:
		{
			if (m_DownIn)
			{
				m_ButtonState = _BUTTON_STATE_CURSOR_IN_UP;
				m_DownIn = false;
				m_Scene->OnUIMsg(m_Name.c_str(), _BUTTON_CLICK);
			}
			break;
		}
	case _UIM_C_LBU_OUT:
		{
			m_ButtonState = _BUTTON_STATE_CURSOR_OUT;
			m_DownIn = false;
			break;
		}
	case _UIM_C_ENTER:
	case _UIM_C_MOVE_IN:
		{
			m_ButtonState = m_DownIn ? _BUTTON_STATE_CURSOR_IN_DOWN : _BUTTON_STATE_CURSOR_IN_UP;
			break;
		}
	case _UIM_C_QUIT:
	case _UIM_C_MOVE_OUT:
		{
			m_ButtonState = m_DownIn ? _BUTTON_STATE_CURSOR_IN_UP : _BUTTON_STATE_CURSOR_OUT;
			break;
		}
	}
}

void CButtonUI::SetText(const char* t)
{
	m_Text = t;
}

const char* CButtonUI::GetText()
{
	return m_Text.c_str();
}