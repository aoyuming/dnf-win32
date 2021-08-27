#ifndef _QUIT_4_H_
#define _QUIT_4_H_

#include "SceneChange.h"

class CQuit4 : public CSceneChange
{
	int index[36];
	int w, h;
public:
	CQuit4();
	void Render(int CurFrame);
};

#endif