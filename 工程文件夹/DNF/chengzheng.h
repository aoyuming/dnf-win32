#ifndef _CHENGZHENG_H
#define _CHENGZHENG_H


#include "Scene.h"
#include "hero.h"
#include <vector>
#include "harm_string.h"
#include "skill_SatelliteRay.h"
#include "define_.h"
#include "bullet.h"

extern std::map<std::string,CHero*> m_player;
extern CHero* m_Hero;//”¢–€
extern bool FangZhu;
extern int scene;
class CChengZheng  : public CScene
{
	bool draw;
	bool SuoYouWanJiaZhunBei;
	CHero* h;
	std::vector<std::string> edit_string;
	std::vector<CSkill_Satellite_Ray*> m_WXSX;//Œ¿–«…‰œﬂ
	std::vector<CHram_String*> m_vHram_String;//…À∫¶ ˝◊÷±Ì
	std::vector<CBullet*> m_Vbullet;
	
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
	void TongBu();
};





#endif