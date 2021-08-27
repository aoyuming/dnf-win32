#ifndef _QUIT_3_H_
#define _QUIT_3_H_

#include "SceneChange.h"

class CQuit3 : public CSceneChange
{
	int index[36];
	int w, h;
public:
	CQuit3();
	void Render(int CurFrame);
};

#endif