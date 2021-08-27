#ifndef _1_H_
#define _1_H_

#include "Scene.h"

#include "Hero.h"


//extern std::map<std::string,CHero*> all_player;
class C1 : public CScene
{
	float a;
	CHero* m_Hero;
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