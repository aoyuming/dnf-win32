#ifndef _CMANHUA_SCENE_H_
#define _CMANHUA_SCENE_H_

#include "Scene.h"
extern int flag;
class CManHuaScene : public CScene
{
	int x, y;
	int filenameindex;

public:
	//virtual ~CScene();

	virtual void Init();
	virtual void Enter();
	virtual void OutputRun(); //����
	virtual void LogicInputRun(); //�߼�������
	virtual void Quit();
	//virtual void End();

	void OnUIMsg(const char* id, int UIMSG);
	void OnNetEvent(_TC_NET_EVENT* ptne);
};

#endif