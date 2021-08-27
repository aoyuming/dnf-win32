#ifndef _QUIT_2_H_
#define _QUIT_2_H_

#include "SceneChange.h"

class CQuit2 : public CSceneChange
{
	unsigned char m_Dark_Level[16];
public:
	CQuit2();
	void Render(int CurFrame);
};

#endif