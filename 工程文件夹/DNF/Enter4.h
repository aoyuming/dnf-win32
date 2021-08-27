#ifndef _ENTER_4_H_
#define _ENTER_4_H_

#include "SceneChange.h"

class CEnter4 : public CSceneChange
{
	int index[36];
	int w, h;
public:
	CEnter4();
	void Render(int CurFrame);
};

#endif