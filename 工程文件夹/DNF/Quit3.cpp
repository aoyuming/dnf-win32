#include "Quit3.h"
#include "GameEngine.h"

CQuit3::CQuit3()
:
CSceneChange(36)
{
	int tmp[]
	=
	{
		14,15,21,20,19,13,7,8,9,10,16,22,28,27,26,25,24,
		18,12,6,0,1,2,3,4,5,11,17,23,29,35,34,33,32,31,30
	};
	for (int i = 0; i < 36; ++i)
		index[i] = tmp[i];
	w = CGameEngine::GetGE()->GetW() / 6;
	h = CGameEngine::GetGE()->GetH() / 6;
}

void CQuit3::Render(int CurFrame)
{
	for (int i = 0; i < CurFrame; ++i)
	{
		int x = index[i] % 6;
		int y = index[i] / 6;
		CGameEngine::GetGE()->GetGO()->DrawRectangle2(
			x * w, y * h, (x + 1) * w, (y + 1) * h,
			RGB(0, 0, 0),
			RGB(0, 0, 0));
	}
}