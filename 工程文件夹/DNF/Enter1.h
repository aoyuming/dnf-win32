#ifndef _ENTER_1_H_
#define _ENTER_1_H_

#include "SceneChange.h"

class CEnter1 : public CSceneChange
{
	int w, h;
public:
	CEnter1();
	void Render(int CurFrame);
};

#endif