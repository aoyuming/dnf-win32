#include "ManHuaScene.h"
#include "GameEngine.h"
#include "ButtonUI.h"
#include "BmpButtonUI.h"
#include "2.h"
#include <iostream>
#include <map>
#include <string>
#include "1.h"
#include "DNFprotocol.h"

void CManHuaScene::Init()
{

	filenameindex = 0;
}

void CManHuaScene::Enter()
{

}

void CManHuaScene::Quit()
{
	
}

void CManHuaScene::OutputRun()
{
	char buf[20];
	std::string FileName ;
	if(flag == 0)
		FileName = "pic\\角色漫画\\神枪手漫画\\";
	else
		FileName = "pic\\角色漫画\\鬼剑士漫画\\";
	itoa(filenameindex,buf,10);
	FileName += buf;
	FileName += ".png";

	CMyXFORM m;
	m.Scaling(1.48f,1.16f);
	CGameEngine::GetGE()->GetGO()->SetTransform(&m);
	CGameEngine::GetGE()->GetGO()->DrawPic1(FileName.data(),0, 0,1050, 556,0, 0);

	CGameEngine::GetGE()->GetGO()->EndTransform();
	
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_RETURN) == _KS_DC)
		filenameindex += 1;
	if(filenameindex >= 19)
	{
		Beep(500, 1000);
		CGameEngine::GetGE()->SetCurScene("选择房间");
	}
}

void CManHuaScene::LogicInputRun()
{

}

void CManHuaScene::OnUIMsg(const char* id, int UIMSG)
{
	
	
}
void CManHuaScene::OnNetEvent(_TC_NET_EVENT* ptne)
{
	switch (ptne->type)
	{
	case _TCNE_S_DISCONNECT:
	case _TCNE_C_DISCONNECT:
	case _TCNE_E_DISCONNECT:
		{
			//all_player.clear();
			break;
		}
	case _TCNE_DATA:
		{
			_GAME_PROTOCOL* pgp = (_GAME_PROTOCOL*)ptne->data;
			switch(pgp->id)
			{
			case _NICK_NAME_LEGALITY:
				{
	
					CGameEngine::GetGE()->SetCurScene("游戏场景");
					
				}break;
			
			}
			
		}break;
	}
}