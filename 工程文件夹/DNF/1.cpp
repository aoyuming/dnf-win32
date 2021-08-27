#include "1.h"
#include "GameEngine.h"
#include "ButtonUI.h"
#include "EditUI.h"
#include "SelectUI.h"
#include "DNFprotocol.h"


std::string m_Nick_Name;
std::map<std::string,CHero*> m_player;
CHero* m_Hero;//Ӣ��
bool FangZhu = false;
int scene = -1;

void C1::Init()
{
	CGameEngine::GetGE()->GetGO()->LoadDirectory("pic");

	CGameEngine::GetGE()->GetGO()->LoadFont("��ʼ��������",30, 40, 400, false, false, false, "����");

	SetEnterChange("��4");
	SetQuitChange("��4");

	LoadUI("��¼��ť", new CButtonUI("��¼", 150 + 200, 380, 90, 30));
	LoadUI("������ť", new CButtonUI("�˳���Ϸ", 250 + 200, 380, 90, 30));

	LoadUI("IP", new CEditUI(0, "IP", 150 + 200, 200, 200, 60,"pic\\ѡ���ɫ����\\28.png"));
	//GetUI("�༭��1")->SetAct(false);
	LoadUI("�ǳ�", new CEditUI(16, "�ǳ�", 150 + 200, 300, 200, 60,"pic\\ѡ���ɫ����\\28.png"));

}

void C1::Enter()
{

}

void C1::Quit()
{
	CGameEngine::GetGE()->GetAM()->StopLoopAudio("m1");
	//delete m_Hero;
}

void C1::OutputRun()
{

	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\����ͼ1.png",0, 0, 940, 701,0, 0);
	CGameEngine::GetGE()->GetGO()->DrawString1(
		"��ʼ��������", 100 + 200 - 20, 100, "���³�����ʿ", RGB(255, 0, 0));

}

void C1::LogicInputRun()
{
}

void C1::OnUIMsg(const char* id, int UIMSG)
{
	if (UIMSG == _BUTTON_CLICK)
	{
		if (strcmp(id, "��¼��ť") == 0)
		{
			CEditUI* eb1 = (CEditUI*)GetUI("IP");
			CEditUI* eb2 = (CEditUI*)GetUI("�ǳ�");
			if (strlen(eb1->GetText()) > 0 && strlen(eb2->GetText()) > 0)
			{
				if(strlen(eb2->GetText()) > 14)
				{
					MessageBox(CGameEngine::GetGE()->GetHwnd(),"�ǳƲ��ܴ���10�ֽ�","��ʾ",MB_OK);
				}
				//�õ��ǳ�
				else
				{
					m_Nick_Name = eb2->GetText() + 4;
					//����
					char buf[32];
					strcpy_s(buf,32, eb1->GetText() + 2);
					CGameEngine::GetGE()->GetTcpClient()->ConnectByIP(buf,12345,10, 10, 10);
	

				}
			}

		}
		else if (strcmp(id, "������ť") == 0)
			CGameEngine::GetGE()->ExitGame();
	}
	else if (UIMSG == _SELECT_NUM_CHANGE)
	{
		if (strcmp(id, "ѡ��1") == 0)
		{
			int cur = ((CSelectUI*)GetUI("ѡ��1"))->GetCur();
			CGameEngine::GetGE()->GetAM()->SetLoopVolume(cur);
		}
		else if (strcmp(id, "ѡ��2") == 0)
		{
			int cur = ((CSelectUI*)GetUI("ѡ��2"))->GetCur();
			CGameEngine::GetGE()->GetAM()->SetOnceVolume(cur);
		}
	}
}

void C1::OnNetEvent(_TC_NET_EVENT* ptne)
{
	switch (ptne->type)
	{
	case _TCNE_CONNECT_FAIL:
		{
			MessageBox(CGameEngine::GetGE()->GetHwnd(),"�޷����ӷ�����","��ʾ",MB_OK);
			break;
		}
	case _TCNE_CONNECT_OK:
		{
					
			//����1��Э��
			_C_S_GP1 csgp1;
			strcpy_s(csgp1.nick_name,32, m_Nick_Name.c_str());
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp1,sizeof(csgp1));
			break;
		}
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
			case _NICK_NAME_LAWLESS:
				{
					MessageBox(CGameEngine::GetGE()->GetHwnd(),"�ǳ��ѱ�ʹ��","��ʾ",MB_OK);
					CGameEngine::GetGE()->GetTcpClient()->Disconnect();
					break;
				}
			case _NICK_NAME_LEGALITY:
				{
					CGameEngine::GetGE()->SetCurScene("��Ϸ����");
				}break;
			}
			break;
		}
	}
}