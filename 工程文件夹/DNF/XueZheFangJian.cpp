#include "XueZheFangJian.h"
#include "GameEngine.h"
#include "ButtonUI.h"
#include "BmpButtonUI.h"
#include "EditUI.h"
#include "SelectUI.h"
#include "DNFprotocol.h"



void CXuanZhe::Init()
{


	CGameEngine::GetGE()->GetGO()->LoadFont("选择房间字体",15, 15, 100, false, false, false, "黑体");

	m_ChuangJianX = (940 - 223) / 2 - 331 / 2;
	m_ChuangJianY = 556 / 2 - 284 / 2;

	m_JinRuX = (940 - 223) / 2 - 329 / 2;;
	m_JinRuY = 556 / 2 - 218 / 2;


	LoadUI("创建房间", new CButtonUI("创建房间", 940 - 223, 556 - 55, 90, 30));
	LoadUI("自由练习", new CButtonUI("自由练习", 940 - 223 + 110, 556 - 55, 90, 30));
	LoadUI("确定", new CButtonUI("确定", 940 / 2 - 200, 556 / 2 + 100, 90, 30));
	LoadUI("取消", new CButtonUI("取消", 940 / 2 - 200 + 120 , 556 / 2 + 100, 90, 30));
	LoadUI("房间名称", new CEditUI(0, "",m_ChuangJianX + 104,m_ChuangJianY + 48, 200, 30,"pic\\选择角色界面\\28.png"));
	LoadUI("密码", new CEditUI(0, "",m_ChuangJianX + 104,m_ChuangJianY + 119, 200, 30,"pic\\选择角色界面\\28.png"));
	LoadUI("进入", new CButtonUI("进入",m_JinRuX + 52, m_JinRuY + 177,90, 30));
	LoadUI("不进入", new CButtonUI("不进入", m_JinRuX + 52 + 110,m_JinRuY + 177, 90, 30));
	for(int i = 1;i <= 16;i++)
	{
		char buf[20];
		itoa(i,buf,10);
		LoadUI(buf, new CButtonUI("", 940  - 223, (i-1) * 30, 223, 30));
		GetUI(buf)->SetView(false);
	}

	GetUI("确定")->SetView(false);
	GetUI("取消")->SetView(false);
	GetUI("房间名称")->SetView(false);
	GetUI("密码")->SetView(false);

	GetUI("进入")->SetView(false);
	GetUI("不进入")->SetView(false);
	GetUI("自由练习")->SetAct(false);


	draw = false;
	m_ChuangJian = false;
	m_JinRu = false;
	buttonI = 1;
	bufRoom_index = -1;

}

void CXuanZhe::Enter()
{
	_C_S_GP16 sc16;
	CGameEngine::GetGE()->GetTcpClient()->SendData((const char*)&sc16,sizeof(sc16));
	draw = false;
	m_ChuangJian = false;
	m_JinRu = false;
	GetUI("确定")->SetView(false);
	GetUI("取消")->SetView(false);
	GetUI("房间名称")->SetView(false);
	GetUI("密码")->SetView(false);
	GetUI("进入")->SetView(false);
	GetUI("不进入")->SetView(false);
}

void CXuanZhe::Quit()
{

	m_vFJXX.clear();
}

void CXuanZhe::OutputRun()
{
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\背景图2.png",-2, 0, 940, 701,0, 0);
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\房间列表2.png",940 - 223, 0, 223, 482,0,0);
	if(m_ChuangJian)
		CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\创建房间.png",m_ChuangJianX,m_ChuangJianY, 328, 284,0,0);
	else if(m_JinRu)
		CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\房间信息.png",m_JinRuX,m_JinRuY, 329, 218,0,0);

	if(draw)
	{
		CGameEngine::GetGE()->GetGO()->DrawString1("选择房间字体", m_JinRuX + 128, m_JinRuY + 25,fangjianhao, RGB(255, 0, 0));
		CGameEngine::GetGE()->GetGO()->DrawString1("选择房间字体", m_JinRuX + 128, m_JinRuY + 64,room_name, RGB(255, 0, 0));
		CGameEngine::GetGE()->GetGO()->DrawString1("选择房间字体", m_JinRuX + 128, m_JinRuY + 115,YXnum, RGB(255, 0, 0));
	}
	//if(draw)
	//{
	//	draw = false;
	//	int i = 1;
	//	for(std::map<std::string,FangJianXinXi>::iterator it = m_vFJXX.begin();  it !=  m_vFJXX.end(); ++it)
	//	{
	//		char buf[20];
	//		itoa(i++,buf,10);
	//		GetUI(buf)->SetView(true);
	//		CEditUI* eb1 = (CEditUI*)GetUI(buf);
	//		std::string s = "";
	//		s += it->second.room_name;
	//		eb1->SetText(s.c_str());
	//	}
	//}


}

void CXuanZhe::LogicInputRun()
{

}

void CXuanZhe::OnUIMsg(const char* id, int UIMSG)
{
	if (UIMSG == _BUTTON_CLICK)
	{
		bool flag = false;
		for(int i = 1;i <= 16;i++)
		{
			char buf[20];
			itoa(i,buf,10);
			if(strcmp(id, buf) == 0)
			{
				flag = true;
				bufRoom_index = i;
				break;
			}
		}
		if(flag)
		{
			GetUI("确定")->SetView(false);
			GetUI("取消")->SetView(false);
			GetUI("房间名称")->SetView(false);
			GetUI("密码")->SetView(false);
			CEditUI* eb1 = (CEditUI*)GetUI("房间名称");
			CEditUI* eb2 = (CEditUI*)GetUI("密码");
			eb1->SetText("");
			eb2->SetText("");

			char buf[20];
			itoa(bufRoom_index,buf,10);
			CButtonUI* cb1 = (CButtonUI*)GetUI(buf);
			std::map<std::string,FangJianXinXi>::iterator it = m_vFJXX.find(cb1->GetText());
			m_JinRu = true;
			m_ChuangJian = false;
			GetUI("进入")->SetView(true);
			GetUI("不进入")->SetView(true);
			itoa(it->second.FangJianHao,buf,10);
			strcpy_s(fangjianhao,20,buf);
			itoa(it->second.PYRnum,buf,10);
			strcpy_s(YXnum,20,buf);
			strcpy_s(room_name,32,it->second.room_name);

			if(it->second.PYRnum >= 8 || it->second.ZhanDou)
			{
				GetUI("进入")->SetAct(false);
			}
			else
				GetUI("进入")->SetAct(true);


			draw = true;
			

		}
		else if (strcmp(id, "创建房间") == 0)
		{
			m_ChuangJian = true;
			m_JinRu = false;
			draw = false;
			GetUI("确定")->SetView(true);
			GetUI("取消")->SetView(true);
			GetUI("房间名称")->SetView(true);
			GetUI("密码")->SetView(true);
			GetUI("进入")->SetView(false);
			GetUI("不进入")->SetView(false);
		}
		else if(strcmp(id, "取消") == 0)
		{
			m_ChuangJian = false;
			m_JinRu = false;
			GetUI("确定")->SetView(false);
			GetUI("取消")->SetView(false);
			GetUI("房间名称")->SetView(false);
			GetUI("密码")->SetView(false);
			CEditUI* eb1 = (CEditUI*)GetUI("房间名称");
			CEditUI* eb2 = (CEditUI*)GetUI("密码");
			eb1->SetText("");
			eb2->SetText("");
		}
		else if (strcmp(id, "确定") == 0)
		{

			CEditUI* eb1 = (CEditUI*)GetUI("房间名称");
			CEditUI* eb2 = (CEditUI*)GetUI("密码");
			if(strcmp(eb1->GetText(), "") == 0)
			{
				MessageBox(CGameEngine::GetGE()->GetHwnd(),"请输入房间名称!","提示",MB_OK);
				return;
			}
			_C_S_GP5 cs5;//创建房间
			strcpy_s(cs5.room_name,32,eb1->GetText());
			strcpy_s(cs5.mima,32,eb2->GetText());
			CGameEngine::GetGE()->GetTcpClient()->SendData((const char*)&cs5,sizeof(cs5));

		}
		else if (strcmp(id, "进入") == 0)
		{
		
			for(std::map<std::string,FangJianXinXi>::iterator it = m_vFJXX.begin(); it != m_vFJXX.end();it++)
			{
				if(it->second.buttonI == bufRoom_index)
				{
					scene = it->second.FangJianHao;
					break;
				}
			}
			bool flag = true;
			for(std::map<std::string,FangJianXinXi>::iterator it = m_vFJXX.begin(); it != m_vFJXX.end();it++)
			{
				if(it->second.FangJianHao == scene && it->second.ZhanDou == true)
				{
					flag = false;
					break;
				}
			}
			if(flag)
			{
				Beep(500, 1000);
				CGameEngine::GetGE()->SetCurScene("城镇场景");
			}
			else
				MessageBox(CGameEngine::GetGE()->GetHwnd(),"暂时不能进入","提示",MB_OK);
		}
		else if (strcmp(id, "不进入") == 0)
		{
			m_ChuangJian = false;
			m_JinRu = false;
			draw = false;
			GetUI("进入")->SetView(false);
			GetUI("不进入")->SetView(false);
		}
		else if (strcmp(id, "自由练习") == 0) 
		{}
		else if (strcmp(id, "结束按钮") == 0)
			CGameEngine::GetGE()->ExitGame();
	}
	else if (UIMSG == _SELECT_NUM_CHANGE)
	{
		if (strcmp(id, "选择1") == 0)
		{
			int cur = ((CSelectUI*)GetUI("选择1"))->GetCur();
			CGameEngine::GetGE()->GetAM()->SetLoopVolume(cur);
		}
		else if (strcmp(id, "选择2") == 0)
		{
			int cur = ((CSelectUI*)GetUI("选择2"))->GetCur();
			CGameEngine::GetGE()->GetAM()->SetOnceVolume(cur);
		}
	}
}

void CXuanZhe::OnNetEvent(_TC_NET_EVENT* ptne)
{
	switch (ptne->type)
	{
	case _TCNE_CONNECT_FAIL:
		{
			MessageBox(CGameEngine::GetGE()->GetHwnd(),"无法连接服务器","提示",MB_OK);
			
		}break;
	case _TCNE_CONNECT_OK:
		{

		}break;
	case _TCNE_S_DISCONNECT:
	case _TCNE_C_DISCONNECT:
	case _TCNE_E_DISCONNECT:
		{

		}break;
	case _TCNE_DATA:
		{
			_GAME_PROTOCOL* pgp = (_GAME_PROTOCOL*)ptne->data;
			switch (pgp->id)
			{
			case _RETURN_FOUND_ROOM://返回创建房间
				{
					_S_C_GP7* sc7 = (_S_C_GP7*)ptne->data;
					scene = sc7->room_index;
					FangZhu = true;
					CGameEngine::GetGE()->SetCurScene("城镇场景");
				}break;
			case _PLAYER_FOUND_ROOM://别人创建房间
				{
					_S_C_GP8* sc8 = (_S_C_GP8*)ptne->data;
					FangJianXinXi fjxx;
					fjxx.FangJianHao = sc8->FangJianHao;
					fjxx.PYRnum = sc8->num;
					strcpy_s(fjxx.mima,32,sc8->mima);
					strcpy_s(fjxx.room_name,32,sc8->room_name);
					fjxx.ZhanDou = sc8->ZhanDou;
					fjxx.buttonI = buttonI;
					m_vFJXX.insert(std::pair<std::string,FangJianXinXi>(fjxx.room_name,fjxx));

					char buf[20];
					itoa(buttonI,buf,10);
					GetUI(buf)->SetView(true);
					CButtonUI* cb1 = (CButtonUI*)GetUI(buf);
					cb1->SetText(fjxx.room_name);
					buttonI += 1;

				}break;
			case _RETURN_XUE_ZHE_FANG_JIAN://返回房间数据
				{
					_S_C_GP26* sc26 = (_S_C_GP26*)ptne->data;
				
					for(int i = 1;i < buttonI;i++)
					{
						char buf[20];
						itoa(i,buf,10);
						GetUI(buf)->SetView(false);
					}

					m_vFJXX.clear();
					buttonI = 1;
					for(int i = 0;i < sc26->num; i++)
					{
						FangJianXinXi fjxx;
						fjxx.FangJianHao = sc26->fjxx[i].FangJianHao;
						fjxx.PYRnum = sc26->fjxx[i].PYRnum;
						strcpy_s(fjxx.mima,32,sc26->fjxx[i].mima);
						strcpy_s(fjxx.room_name,32,sc26->fjxx[i].room_name);
						fjxx.ZhanDou = sc26->fjxx[i].ZhanDou;
						fjxx.buttonI = buttonI;
						m_vFJXX.insert(std::pair<std::string,FangJianXinXi>(fjxx.room_name,fjxx));

						char buf[20];
						itoa(buttonI,buf,10);
						GetUI(buf)->SetView(true);
						CButtonUI* cb1 = (CButtonUI*)GetUI(buf);
						cb1->SetText(fjxx.room_name);

						buttonI += 1;
					}

				}break;
			case _ROOM_NO_ENTER://房间不能进入
				{
					_S_C_GP27* sc27 = (_S_C_GP27*)ptne->data;
					
					for(std::map<std::string,FangJianXinXi>::iterator it = m_vFJXX.begin(); it != m_vFJXX.end();it++)
					{
						if(it->second.FangJianHao == sc27->fangjianhao)
						{
							it->second.ZhanDou = true;
							break;
						}
					}

				}break;
			case _ROOM_OK_ENTER://房间可以进入
				{
					_S_C_GP28* sc28 = (_S_C_GP28*)ptne->data;
					
					for(std::map<std::string,FangJianXinXi>::iterator it = m_vFJXX.begin(); it != m_vFJXX.end();it++)
					{
						if(it->second.FangJianHao == sc28->fangjianhao)
						{
							it->second.ZhanDou = false;
							break;
						}
					}
				}
			case _ROOM_RENSHU_BIAN_HUA://房间人数发生变化
				{
					_S_C_GP29* sc29 = (_S_C_GP29*)ptne->data;
					
					for(std::map<std::string,FangJianXinXi>::iterator it = m_vFJXX.begin(); it != m_vFJXX.end();it++)
					{
						if(it->second.FangJianHao == sc29->fangjianhao)
						{
							it->second.PYRnum = sc29->renshu;
							break;
						}
					}
					
				}break;
			case _RETURN_ENTER_ROOM://返回房间玩家数据
				{

				}break;
			}

		}break;
	}
}