#ifndef _ENTER_2_H_
#define _ENTER_2_H_

#include "SceneChange.h"

class CEnter2 : public CSceneChange
{
	unsigned char m_Dark_Level[16];
public:
	CEnter2();
	void Render(int CurFrame);
};

#endif