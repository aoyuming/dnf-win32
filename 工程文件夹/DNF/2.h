#ifndef _2_H_
#define _2_H_

#include "Scene.h"
class C2 : public CScene
{
	int x, y;
	int time;

public:
	//virtual ~CScene();

	virtual void Init();
	virtual void Enter();
	virtual void OutputRun(); //ªÊ÷∆
	virtual void LogicInputRun(); //¬ﬂº≠°¢ ‰»Î
	virtual void Quit();
	//virtual void End();

	void OnUIMsg(const char* id, int UIMSG);
	void OnNetEvent(_TC_NET_EVENT* ptne);
};

#endif