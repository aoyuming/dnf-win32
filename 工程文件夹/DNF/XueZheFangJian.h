#ifndef _XUANZE_H_
#define _XUANZE_H_

#include "Scene.h"
#include "Hero.h"
#include "DNFprotocol.h"
extern int scene;
extern bool FangZhu;
extern CHero* m_Hero;//”¢–€
//extern std::map<std::string,CHero*> all_player;
class CXuanZhe: public CScene
{
	float a;
	int buttonI;
	bool m_ChuangJian;
	bool m_JinRu;
	int m_ChuangJianX;
	int m_ChuangJianY;
	int m_JinRuX;
	int m_JinRuY;
	char fangjianhao[20];
	char YXnum[20];
	char room_name[32];
	int bufRoom_index;
	std::map<std::string,FangJianXinXi> m_vFJXX;
	bool draw;
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