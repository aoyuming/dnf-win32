#ifndef _SCENE_H_
#define _SCENE_H_

#include <map>
#include <string>
#include "TcpClient.h"

class CUI;

class CScene
{
	friend class CGameEngine;

	//入出切换名字
	std::string m_EnterChange;
	std::string	m_QuitChange;

	//控件管理
	std::map<std::string, CUI*> m_UI;
	CUI* m_KeyUI; //拥有当前焦点的控件

public:
	CScene();
	virtual ~CScene();

	virtual void Init();
	virtual void Enter();
	virtual void OutputRun(); //绘制
	virtual void LogicInputRun(); //逻辑、输入
	virtual void Quit();
	virtual void End();

	//切换相关
	void SetEnterChange(const char* id);
	void SetQuitChange(const char* id);

	//控件相关
	bool LoadUI(const char* id, CUI* pUI);
	bool ReleaseUI(const char* id);
	CUI* GetUI(const char* id);
	CUI* GetKeyUI();
	bool SetKeyUI(const char* id); //若id为0则设置当前没有焦点控件

	//当本场景中的控件发生事情本函数就会被调用
	virtual void OnUIMsg(const char* id, int UIMSG);

	//网络相关
	virtual void OnNetEvent(_TC_NET_EVENT* ptne);
};

#endif