#ifndef _SCENE_CHANGE_H_
#define _SCENE_CHANGE_H_

class CSceneChange
{
	friend class CGameEngine;
	int m_AllFrame; //总帧数
	int m_CurFrame; //当前帧数

public:
	CSceneChange(int AllFrame);
	virtual void Render(int CurFrame);
};

#endif