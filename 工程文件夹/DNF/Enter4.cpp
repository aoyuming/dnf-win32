#include "Enter4.h"
#include "GameEngine.h"

CEnter4::CEnter4()
:
CSceneChange(36)
{
	w = CGameEngine::GetGE()->GetW() / 6;
	h = CGameEngine::GetGE()->GetH() / 6;
}

void CEnter4::Render(int CurFrame)
{
	if (1 == CurFrame)
	{
		int tmp[36];
		for (int i = 0; i < 36; ++i)
			tmp[i] = i;

		for (int i = 35; i > 0; --i)
		{
			int j = CGameEngine::GetRandomNum(0, i);
			int k = tmp[j];
			tmp[j] = tmp[i];
			tmp[i] = k;
		}

		for (int i = 0; i < 36; ++i)
			index[i] = tmp[i];
	}

	CurFrame = 36 - CurFrame;
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