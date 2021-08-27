#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include "TcpClient.h"
#include <windows.h>
#include <string>
#include <map>
#include "GameInput.h"
#include "GameOutputD3D.h"
#include "GameOutputGDI.h"
#include "ReadFile.h"
#include "WriteFile.h"
#include "AudioManager.h"

class CScene;
class CSceneChange;

class CGameEngine
{
	//窗口相关
	WNDCLASSA m_WndClass;
	char m_WndClassName[32];
	HWND m_hWnd;
	int m_ClientW, m_ClientH;
	BOOL m_Act;
	int m_SleepTime;
	POINT m_PrePos; //刚才的光标位置
	bool m_DoubleByte; //双字节文字标识
	char m_FirstByte; //双字节文字首字节

	//工具相关
	CGameInput* m_GI;
	CGameOutput* m_GO;
	CReadFile m_RF;
	CWriteFile m_WF;
	CAudioManager m_AM;
	CTcpClient m_TcpClient;

	//场景相关
	std::map<std::string, CScene*> m_Scene; //场景
	CScene* m_CurScene; //当前场景
	CScene* m_NextScene; //下一个场景
	int m_RunState; //运行状态

	//场景切换相关
	CSceneChange* m_EnterChange;
	CSceneChange* m_QuitChange;
	std::map<std::string, CSceneChange*> m_SceneChange; //场景切换

	//消息处理函数
	static LRESULT CALLBACK _WindowProc(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam);

	//切换处理
	void _SceneChange();
	void _Change();
	void _SendMouseMove();

	//构造、析构、指针
	CGameEngine();
	~CGameEngine();
	static CGameEngine* m_pGameEngine;

public:

	//得到游戏引擎
	static CGameEngine* GetGE();
	 HWND GetHwnd();
	//初始化
	bool Init(
		int ClientW, //客户区宽
		int ClientH, //客户区高
		bool Advanced = false); //高级渲染

	//运行
	void Run();

	//场景相关函数
	bool LoadScene(const char* id, CScene* pScene);
	bool ReleaseScene(const char* id);
	bool SetInitScene(const char* id);
	bool SetCurScene(const char* id);
	CScene* GetScene(const char* id);

	//加载场景切换
	bool LoadSceneChange(const char* id, CSceneChange* pSceneChange);

	//工具函数
	CGameInput* GetGI();
	CGameOutput* GetGO();
	CReadFile* GetRF();
	CWriteFile* GetWF();
	CAudioManager* GetAM();
	CTcpClient* GetTcpClient();

	void SetTitle(const char* Title);
	bool SetSleepTime(int SleepTime);
	void ExitGame();
	int GetW();
	int GetH();
	static int GetRandomNum(int Min, int Max);
};

#endif