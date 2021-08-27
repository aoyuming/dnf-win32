#include "Enter1.h"
#include "GameEngine.h"

CEnter1::CEnter1()
:
CSceneChange(16)
{
	w = CGameEngine::GetGE()->GetW();
	h = CGameEngine::GetGE()->GetH();
}

void CEnter1::Render(int CurFrame)
{
	if (1 == CurFrame)
		CGameEngine::GetGE()->GetGO()->SetDark(0);

	CGameEngine::GetGE()->GetGO()->DrawRectangle2(
		0, 0, w - (w / 16) * CurFrame, h,
		RGB(0, 0, 0),
		RGB(0, 0, 0));
}