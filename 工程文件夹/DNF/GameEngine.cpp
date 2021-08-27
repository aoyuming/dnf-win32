#include <iostream>
#include <time.h>
#include "GameEngine.h"
#include "Scene.h"
#include "UI.h"
#include "SceneChange.h"
//#include "define_.h"

#define _SC_NONE 0 //没有切换
#define _SC_PRE 1 //准备切换
#define _SC_QUIT 2 //当前场景出切换
#define _SC_CHANGE 3 //场景切换
#define _SC_ENTER 4 //下一个场景入切换

CGameEngine* CGameEngine::m_pGameEngine = 0;
HWND CGameEngine::GetHwnd()
{
	return m_hWnd;
}
LRESULT CALLBACK CGameEngine::_WindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_ACTIVATEAPP:
		{
			GetGE()->m_Act = (BOOL)wParam;
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			POINT CurPos = {LOWORD(lParam), HIWORD(lParam)};

			//如果当前场景存在
			if (GetGE()->m_RunState == _SC_NONE && GetGE()->m_CurScene)
			{
				//遍历当前场景下面的所有控件
				std::map<std::string, CUI*>::iterator i;
				for (i = GetGE()->m_CurScene->m_UI.begin(); i != GetGE()->m_CurScene->m_UI.end(); ++i)
				{
					//必须可见
					if (i->second->GetView())
					{
						//必须激活
						if (i->second->GetAct())
						{
							//得到控件矩形
							int x, y, w, h;
							i->second->GetRect(&x, &y, &w, &h);
							RECT r = {x, y, x + w, y + h};

							//得到上次光标是否在矩形中
							BOOL b1 = PtInRect(&r, GetGE()->m_PrePos);

							//得到本次光标是否在矩形中
							BOOL b2 = PtInRect(&r, CurPos);

							//上次在、这次在就在内部移动
							if (b1 && b2)
								i->second->OnMsg(_UIM_C_MOVE_IN, CurPos.x - x, CurPos.y - y);
							//上次在、这次不在就是退出
							else if (b1 && !b2)
								i->second->OnMsg(_UIM_C_QUIT, CurPos.x - x, CurPos.y - y);
							//上次不在、这次在就是进入
							else if (!b1 && b2)
								i->second->OnMsg(_UIM_C_ENTER, CurPos.x - x, CurPos.y - y);
							//上次不在、这次不在就在外部移动
							else
								i->second->OnMsg(_UIM_C_MOVE_OUT, CurPos.x - x, CurPos.y - y);
						}
					}
				}
			}

			//本次判断的当前位置作为下次判断的前次位置
			GetGE()->m_PrePos = CurPos;

			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		{
			POINT CurPos = {LOWORD(lParam), HIWORD(lParam)};

			//如果当前场景存在
			if (GetGE()->m_RunState == _SC_NONE && GetGE()->m_CurScene)
			{
				//遍历当前场景下面的所有控件
				std::map<std::string, CUI*>::iterator i;
				for (i = GetGE()->m_CurScene->m_UI.begin(); i != GetGE()->m_CurScene->m_UI.end(); ++i)
				{
					//必须可见
					if (i->second->GetView())
					{
						//必须激活
						if (i->second->GetAct())
						{
							//得到控件矩形
							int x, y, w, h;
							i->second->GetRect(&x, &y, &w, &h);
							RECT r = {x, y, x + w, y + h};

							//得到本次光标是否在矩形中
							BOOL b = PtInRect(&r, CurPos);

							if (TRUE == b)
							{
								switch (uMsg)
								{
								case WM_LBUTTONDOWN: i->second->OnMsg(_UIM_C_LBD_IN, CurPos.x - x, CurPos.y - y); break;
								case WM_LBUTTONUP: i->second->OnMsg(_UIM_C_LBU_IN, CurPos.x - x, CurPos.y - y); break;
								case WM_RBUTTONDOWN: i->second->OnMsg(_UIM_C_RBD_IN, CurPos.x - x, CurPos.y - y); break;
								case WM_RBUTTONUP: i->second->OnMsg(_UIM_C_RBU_IN, CurPos.x - x, CurPos.y - y); break;
								}
							}
							else
							{
								switch (uMsg)
								{
								case WM_LBUTTONDOWN: i->second->OnMsg(_UIM_C_LBD_OUT, CurPos.x - x, CurPos.y - y); break;
								case WM_LBUTTONUP: i->second->OnMsg(_UIM_C_LBU_OUT, CurPos.x - x, CurPos.y - y); break;
								case WM_RBUTTONDOWN: i->second->OnMsg(_UIM_C_RBD_OUT, CurPos.x - x, CurPos.y - y); break;
								case WM_RBUTTONUP: i->second->OnMsg(_UIM_C_RBU_OUT, CurPos.x - x, CurPos.y - y); break;
								}
							}
						}
					}
				}
			}

			return 0;
		}
	case WM_KEYDOWN:
		{
			//如果当前场景存在
			if (GetGE()->m_RunState == _SC_NONE && GetGE()->m_CurScene)
			{
				//当前场景拥有键盘输入焦点控件
				if (GetGE()->m_CurScene->m_KeyUI)
				{
					//键盘输入焦点控件必须可见、激活
					if (GetGE()->m_CurScene->m_KeyUI->GetView() && GetGE()->m_CurScene->m_KeyUI->GetAct())
						GetGE()->m_CurScene->m_KeyUI->OnMsg(_UIM_K_D, (int)wParam, 0);
				}
			}

			return 0;
		}
	case WM_KEYUP:
		{
			//如果当前场景存在
			if (GetGE()->m_RunState == _SC_NONE && GetGE()->m_CurScene)
			{
				//当前场景拥有键盘输入焦点控件
				if (GetGE()->m_CurScene->m_KeyUI)
				{
					//键盘输入焦点控件必须可见、激活
					if (GetGE()->m_CurScene->m_KeyUI->GetView() && GetGE()->m_CurScene->m_KeyUI->GetAct())
						GetGE()->m_CurScene->m_KeyUI->OnMsg(_UIM_K_U, (int)wParam, 0);
				}
			}

			return 0;
		}
	case WM_CHAR:
		{
			//如果当前场景存在
			if (GetGE()->m_RunState == _SC_NONE && GetGE()->m_CurScene)
			{
				//当前场景拥有键盘输入焦点控件
				if (GetGE()->m_CurScene->m_KeyUI)
				{
					//键盘输入焦点控件必须可见、激活
					if (GetGE()->m_CurScene->m_KeyUI->GetView() && GetGE()->m_CurScene->m_KeyUI->GetAct())
					{
						if (GetGE()->m_DoubleByte)
						{
							char c[4] = {GetGE()->m_FirstByte, (char)wParam};
							GetGE()->m_CurScene->m_KeyUI->OnMsg(_UIM_K_CHAR, *((int*)c), 1);
							GetGE()->m_DoubleByte = false;
							return 0;
						}
					
						if (IsDBCSLeadByte((char)wParam))
						{
							GetGE()->m_FirstByte = (char)wParam;
							GetGE()->m_DoubleByte = true;
						}
						else
							GetGE()->m_CurScene->m_KeyUI->OnMsg(_UIM_K_CHAR, (int)wParam, 0);
						
						
					}
				}
			}

			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CGameEngine::_SceneChange()
{
	if (m_NextScene)
	{
		m_CurScene->Quit(); //多态
		m_CurScene = m_NextScene;
		m_CurScene->Enter(); //多态
		m_NextScene = 0;
	}
}

void CGameEngine::_Change()
{
	switch (m_RunState)
	{
	case _SC_PRE:
		{
			//得到当前场景的出切换
			std::map<std::string, CSceneChange*>::iterator j;
			j = m_SceneChange.find(m_CurScene->m_QuitChange);
			if (j == m_SceneChange.end())
				m_QuitChange = 0;
			else
			{
				m_QuitChange = j->second;
				m_QuitChange->m_CurFrame = 0;
			}

			//得到下一个场景的入切换
			j = m_SceneChange.find(m_NextScene->m_EnterChange);
			if (j == m_SceneChange.end())
				m_EnterChange = 0;
			else
			{
				m_EnterChange = j->second;
				m_EnterChange->m_CurFrame = 0;
			}

			//有出切换、入切换
			if (m_QuitChange && m_EnterChange)
			{
				m_RunState = _SC_QUIT;
			}
			//有出切换、无入切换
			else if (m_QuitChange && !m_EnterChange)
			{
				m_RunState = _SC_QUIT;
			}
			//无出切换、有入切换
			else if (!m_QuitChange && m_EnterChange)
			{
				_SceneChange();
				m_RunState = _SC_ENTER;
			}
			//无出切换、入切换
			else
			{
				_SceneChange();
				m_RunState = _SC_NONE;
				_SendMouseMove();
			}

			break;
		}
	case _SC_CHANGE:
		{
			_SceneChange();
			m_RunState = m_QuitChange ? _SC_ENTER : _SC_NONE;
			if (m_RunState == _SC_NONE)
				_SendMouseMove();

			break;
		}
	}
}

void CGameEngine::_SendMouseMove()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(m_hWnd, &p);
	SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELONG(p.x, p.y));
}

CGameEngine::CGameEngine()
:
m_hWnd(0),
m_ClientW(0),
m_ClientH(0),
m_Act(TRUE),
m_SleepTime(33),
m_GI(0),
m_GO(0),
m_CurScene(0),
m_NextScene(0),
m_RunState(_SC_NONE),
m_EnterChange(0),
m_QuitChange(0),
m_DoubleByte(false)
{
	srand((unsigned int)time(0));

}

CGameEngine::~CGameEngine()
{}

CGameEngine* CGameEngine::GetGE()
{
	if (0 == m_pGameEngine)
		m_pGameEngine = new CGameEngine;
	return m_pGameEngine;
}

bool CGameEngine::Init(int ClientW, int ClientH, bool Advanced)
{
	//参数检查
	if (ClientW < 1 || ClientH < 1)
		return false;

	//得到宽高
	m_ClientW = ClientW;
	m_ClientH = ClientH;

	//填充窗口类别结构体
	m_WndClass.style = CS_HREDRAW | CS_VREDRAW;
	m_WndClass.lpfnWndProc = _WindowProc;
	m_WndClass.cbClsExtra = 0;
	m_WndClass.cbWndExtra = 0;
	m_WndClass.hInstance = GetModuleHandle(0);
	m_WndClass.hIcon = LoadIconA(0, IDI_APPLICATION);
	m_WndClass.hCursor = LoadCursorA(0, IDC_ARROW);
	m_WndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	m_WndClass.lpszMenuName = 0;
	for (int i = 0; i < 31; ++i)
		m_WndClassName[i] = rand() % 26 + 'a';
	m_WndClassName[31] = 0;
	m_WndClass.lpszClassName = m_WndClassName;

	//注册窗口
	RegisterClassA(&m_WndClass);

	//得到窗口应该的矩形
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	RECT r = {(sw - m_ClientW) / 2, (sh - m_ClientH) / 2, r.left + m_ClientW, r.top + m_ClientH};
	AdjustWindowRect(&r, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

	//创建窗口
	m_hWnd = CreateWindowA(
		m_WndClass.lpszClassName,
		0,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		r.left,
		r.top,
		r.right - r.left,
		r.bottom - r.top,
		HWND_DESKTOP,
		0,
		m_WndClass.hInstance,
		0);

	m_GI = new CGameInput(m_hWnd);
	//m_GO = new CGameOutputGDI(m_hWnd);
	m_GO = new CGameOutputD3D(m_hWnd);

	//进入消息循环之前得到一次光标位置
	GetCursorPos(&m_PrePos);
	ScreenToClient(m_hWnd, &m_PrePos);

	return true;
}

void CGameEngine::Run()
{
	std::map<std::string, CScene*>::iterator i;

	//场景初始化
	for (i = m_Scene.begin(); i != m_Scene.end(); ++i)
	{
		i->second->Init(); //多态

		//初始化本场景中所有的控件
		std::map<std::string, CUI*>::iterator k;
		for (k = i->second->m_UI.begin(); k != i->second->m_UI.end(); ++k)
			k->second->Init(); //多态
	}

	//最初的场景入
	if (m_CurScene)
		m_CurScene->Enter(); //多态

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		int bt = GetTickCount();

		//输出
		m_GO->Begin();
		if (m_CurScene)
		{
			m_CurScene->OutputRun(); //多态

			//渲染当前场景控件
			std::map<std::string, CUI*>::iterator k;
			for (k = m_CurScene->m_UI.begin(); k != m_CurScene->m_UI.end(); ++k)
			{
				//看得到
				if (k->second->GetView())
				{
					if (k->second->GetAct())
						k->second->ActRender(); //多态
					else
						k->second->UnactRender(); //多态
				}
			}

			switch (m_RunState)
			{
			case _SC_QUIT:
				{
					if (m_QuitChange)
					{
						m_QuitChange->Render(m_QuitChange->m_CurFrame += 1);
						if (m_QuitChange->m_CurFrame == m_QuitChange->m_AllFrame)
							m_RunState = _SC_CHANGE;
					}
					break;
				}
			case _SC_ENTER:
				{
					if (m_EnterChange)
					{
						m_EnterChange->Render(m_EnterChange->m_CurFrame += 1);
						if (m_EnterChange->m_CurFrame == m_EnterChange->m_AllFrame)
						{
							m_RunState = _SC_NONE;
							_SendMouseMove();
						}
					}
					break;
				}
			}
		}
		m_GO->End();

		//逻辑、输入
		m_GI->Run();
		if (m_CurScene && m_RunState == _SC_NONE)
			m_CurScene->LogicInputRun(); //多态
		
		//网络部分运行
		m_TcpClient.Run();
		_TC_NET_EVENT tne;
		while (m_TcpClient.GetTCNetEvent(&tne))
		{
			//场景响应网络事件：多态
			m_CurScene->OnNetEvent(&tne);
			m_TcpClient.ReleaseTSNetEvent(&tne);
		}

		int at = GetTickCount() - bt;
		Sleep((m_SleepTime > at) ? (m_SleepTime - at) : 1);
		

		_Change();
	}

	//最后的场景出
	if (m_CurScene)
		m_CurScene->Quit(); //多态

	//结束、删除所有场景
	for (i = m_Scene.begin(); i != m_Scene.end(); ++i)
	{
		//结束本场景中所有的控件
		std::map<std::string, CUI*>::iterator k;
		for (k = i->second->m_UI.begin(); k != i->second->m_UI.end(); ++k)
		{
			k->second->End(); //多态
			delete k->second;
		}

		i->second->End(); //多态
		delete i->second;
	}

	//删除所有场景切换
	std::map<std::string, CSceneChange*>::iterator j;
	for (j = m_SceneChange.begin(); j != m_SceneChange.end(); ++j)
		delete j->second;

	delete m_GO;
	delete m_GI;
	delete m_pGameEngine;
}

bool CGameEngine::LoadScene(const char* id, CScene* pScene)
{
	std::string s = id;
	if (m_Scene.find(s) != m_Scene.end())
		return false;

	m_Scene.insert(std::pair<std::string, CScene*>(s, pScene));
	return true;
}

bool CGameEngine::ReleaseScene(const char* id)
{
	std::string s = id;
	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	if (i == m_Scene.end())
		return false;

	i->second->End(); //多态
	delete i->second;
	m_Scene.erase(i);

	return true;
}

bool CGameEngine::SetInitScene(const char* id)
{
	std::string s = id;
	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	if (i == m_Scene.end())
		return false;

	m_CurScene = i->second;
	return true;
}

bool CGameEngine::SetCurScene(const char* id)
{
	std::string s = id;
	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	if (i == m_Scene.end())
		return false;

	//得到下一个场景的指针
	m_NextScene = i->second;

	//修改运行状态为准备切换
	m_RunState = _SC_PRE;

	return true;
}

CScene* CGameEngine::GetScene(const char* id)
{
	std::string s = id;
	std::map<std::string, CScene*>::iterator i = m_Scene.find(s);
	return i == m_Scene.end() ? 0 : i->second;
}

bool CGameEngine::LoadSceneChange(const char* id, CSceneChange* pSceneChange)
{
	std::string s = id;
	if (m_SceneChange.find(s) != m_SceneChange.end())
		return false;

	m_SceneChange.insert(std::pair<std::string, CSceneChange*>(s, pSceneChange));
	return true;
}

void CGameEngine::SetTitle(const char* Title)
{
	SetWindowTextA(m_hWnd, Title);
}

bool CGameEngine::SetSleepTime(int SleepTime)
{
	if (SleepTime < 1)
		return false;

	m_SleepTime = SleepTime;
	return true;
}

void CGameEngine::ExitGame()
{
	PostQuitMessage(0);
}

CGameInput* CGameEngine::GetGI()
{
	return m_GI;
}

CGameOutput* CGameEngine::GetGO()
{
	return m_GO;
}

CReadFile* CGameEngine::GetRF()
{
	return &m_RF;
}

CTcpClient* CGameEngine::GetTcpClient()
{
	return &m_TcpClient;
}


CWriteFile* CGameEngine::GetWF()
{
	return &m_WF;
}

CAudioManager* CGameEngine::GetAM()
{
	return &m_AM;
}

int CGameEngine::GetW()
{
	return m_ClientW;
}

int CGameEngine::GetH()
{
	return m_ClientH;
}

int CGameEngine::GetRandomNum(int Min, int Max)
{
	return rand() % (Max - Min + 1) + Min;
}