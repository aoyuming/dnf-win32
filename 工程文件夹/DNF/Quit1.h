#ifndef _QUIT_1_H_
#define _QUIT_1_H_

#include "SceneChange.h"

class CQuit1 : public CSceneChange
{
	int w, h;
public:
	CQuit1();
	void Render(int CurFrame);
};

#endif