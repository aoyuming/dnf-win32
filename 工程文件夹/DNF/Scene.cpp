#include "GameEngine.h"
#include "Scene.h"
#include "UI.h"

CScene::CScene()
:
m_KeyUI(0)
{
	m_EnterChange[0] = 0;
	m_QuitChange[0] = 0;
}
CScene::~CScene(){}
void CScene::Init(){}
void CScene::Enter(){}
void CScene::OutputRun(){}
void CScene::LogicInputRun(){}
void CScene::Quit(){}
void CScene::End(){}

void CScene::SetEnterChange(const char* id)
{
	if (id)
		m_EnterChange = id;
	else
		m_EnterChange = "";
}

void CScene::SetQuitChange(const char* id)
{
	if (id)
		m_QuitChange = id;
	else
		m_QuitChange = "";
}

bool CScene::LoadUI(const char* id, CUI* pUI)
{
	std::string s = id;
	if (m_UI.find(s) != m_UI.end())
		return false;

	//为UI设置场景指针、名字
	pUI->m_Scene = this;
	pUI->m_Name = id;

	//装入UI
	m_UI.insert(std::pair<std::string, CUI*>(s, pUI));

	return true;
}

bool CScene::ReleaseUI(const char* id)
{
	std::string s = id;
	std::map<std::string, CUI*>::iterator i = m_UI.find(s);
	if (i == m_UI.end())
		return false;

	i->second->End();
	delete i->second;
	m_UI.erase(i);

	return true;
}

CUI* CScene::GetUI(const char* id)
{
	std::string s = id;
	std::map<std::string, CUI*>::iterator i = m_UI.find(s);
	return i == m_UI.end() ? 0 : i->second;
}

CUI* CScene::GetKeyUI()
{
	return m_KeyUI;
}

bool CScene::SetKeyUI(const char* id)
{
	if (0 == id)
	{
		m_KeyUI = 0;
		return true;
	}
	else
	{
		std::string s = id;
		std::map<std::string, CUI*>::iterator i = m_UI.find(s);
		if (i == m_UI.end())
			return false;

		m_KeyUI = i->second;
		return true;
	}
}

void CScene:: OnUIMsg(const char* id, int UIMSG){}
void CScene::OnNetEvent(_TC_NET_EVENT* ptne){}