#include "BmpButtonUI.h"
#include "Scene.h"
//#include "GameEngine.h"

CBmpButtonUI::CBmpButtonUI(const char* bmp_act_cursor_out,
						   const char* bmp_act_cursor_in_up,
						   const char* bmp_act_cursor_in_down,
						   const char* bmp_unact,
						   int x,
						   int y,
						   int w,
						   int h,
						   bool t,
						   bool v,
						   bool a)
						   :
CButtonUI("", x, y, w, h, v, a),
m_Transparent(t)
{
	s[0] = bmp_act_cursor_out;
	s[1] = bmp_act_cursor_in_up;
	s[2] = bmp_act_cursor_in_down;
	s[3] = bmp_unact;
}

void CBmpButtonUI::ActRender()
{
	CGameOutput* pGO = CGameEngine::GetGE()->GetGO();

	switch (m_ButtonState)
	{
	case _BUTTON_STATE_CURSOR_OUT:
		{
			if (m_Transparent)
				pGO->DrawPic1(s[0].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
			else
				pGO->DrawPic2(s[0].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
			break;
		}
	case _BUTTON_STATE_CURSOR_IN_UP:
		{
			if (m_Transparent)
				pGO->DrawPic1(s[1].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
			else
				pGO->DrawPic2(s[1].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
			break;
		}
	case _BUTTON_STATE_CURSOR_IN_DOWN:
		{
			if (m_Transparent)
				pGO->DrawPic1(s[2].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
			else
				pGO->DrawPic2(s[2].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
			break;
		}
	}
}

void CBmpButtonUI::UnactRender()
{
	CGameOutput* pGO = CGameEngine::GetGE()->GetGO();

	if (m_Transparent)
		pGO->DrawPic1(s[3].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
	else
		pGO->DrawPic2(s[3].c_str(), m_X, m_Y, m_W, m_H, 0, 0, _MIRROR_NONE);
}