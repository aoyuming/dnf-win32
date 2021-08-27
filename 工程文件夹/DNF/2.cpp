#include "2.h"
#include "GameEngine.h"
#include "ButtonUI.h"
#include "BmpButtonUI.h"
#include "1.h"
#include <iostream>
#include <map>
#include <string>
#include "DNFprotocol.h"

int flag;

void C2::Init()
{

	LoadUI("��ǹ��", new CBmpButtonUI(
		"pic\\ѡ���ɫ����\\37.png",
		"pic\\ѡ���ɫ����\\38.png",
		"pic\\ѡ���ɫ����\\39.png",
		"pic\\ѡ���ɫ����\\39.png",
		200,
		400,
		60,
		60,
		true));

		LoadUI("��ʿ", new CBmpButtonUI(
		"pic\\ѡ���ɫ����\\11.png",
		"pic\\ѡ���ɫ����\\12.png",
		"pic\\ѡ���ɫ����\\13.png",
		"pic\\ѡ���ɫ����\\14.png",
		500 + 200,
		400,
		60,
		60,
		true));
		CGameEngine::GetGE()->GetAM()->PushLoopAudio("m1", "sound\\DNF��¼��������.mp3");
		GetUI("��ʿ")->SetAct(false);
		SetEnterChange("��4");
        SetQuitChange("��4");

}

void C2::Enter()
{
	CGameEngine::GetGE()->GetAM()->PlayLoopAudio("m1");
	
}

void C2::Quit()
{
	CGameEngine::GetGE()->GetAM()->StopLoopAudio("m1");
}

void C2::OutputRun()
{
	CMyXFORM m;
	m.Scaling(1.6f,1.2f);
	CGameEngine::GetGE()->GetGO()->SetTransform(&m);
	CGameEngine::GetGE()->GetGO()->DrawPic1("pic\\ѡ���ɫ����\\0.png",0, 0, 640, 480,0, 0);
	//CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\ѡ���ɫ����\\1.png",150,70, 342,342,_MIRROR_NONE,0);
	CGameEngine::GetGE()->GetGO()->DrawPic1("pic\\���Ѳ�ͼ\\��ǹ���Ѳ�ͼ\\0.png",-20, 0, 332, 301,0, 0);
	CGameEngine::GetGE()->GetGO()->DrawPic1("pic\\���Ѳ�ͼ\\���۾��Ѳ�ͼ\\0.png",556 - 330 + 100, 0, 310, 300,0, 0,_MIRROR_LR);
	CGameEngine::GetGE()->GetGO()->EndTransform();
}

void C2::LogicInputRun()
{
	
}

void C2::OnUIMsg(const char* id, int UIMSG)
{
	if (UIMSG == _BUTTON_CLICK)
	{
		if (strcmp(id, "���ذ�ť") == 0)
		{
			CGameEngine::GetGE()->SetCurScene("��ʼ����");
		}
		if (strcmp(id, "��ǹ��") == 0)
		{
			Beep(500, 1000);
			flag = 0;
			CGameEngine::GetGE()->SetCurScene("��������");
		}
		if (strcmp(id, "��ʿ") == 0)
		{
			Beep(500, 1000);
			flag = 1;
			CGameEngine::GetGE()->SetCurScene("��������");
		}
	}
	
}
void C2::OnNetEvent(_TC_NET_EVENT* ptne)
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
			switch (pgp->id)
			{
			case _NICK_NAME_LEGALITY:
				{
					_S_C_GP3* p = (_S_C_GP3*)ptne->data;
					//��ʼ����Ϸ��ͼ����
					//��ȡ������ҵ�����	
					//for( int i = 0; i < p->player_num; ++i )
					//{
					//	//std::string s = p->player[i].nick_name;
					//	//CHero* hero = new CHero(p->player[i].x,p->player[i].y,p->player[i].hp,p->player[i].mp,p->player[i].nick_name,_MIRROR_NONE,75,141);

					////	all_player.insert(std::pair<std::string,CHero*>(s,hero));
					//}
					CGameEngine::GetGE()->SetCurScene("��Ϸ����");

					break;
				}
			}
			break;
		}
	}
}