#ifndef _ENTER_3_H_
#define _ENTER_3_H_

#include "SceneChange.h"

class CEnter3 : public CSceneChange
{
	int index[36];
	int w, h;
public:
	CEnter3();
	void Render(int CurFrame);
};

#endif