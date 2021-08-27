#include "chengzheng.h"
#include "GameEngine.h"
#include "ButtonUI.h"
#include "BmpButtonUI.h"
//#include "1.h"
#include <iostream>
#include <map>
#include <string>
#include "EditUI.h"
#include "DNFprotocol.h"

//void CChengZheng::TongBu()
//{
//
//}
bool g_fanhui = false;

void CChengZheng::Init()
{
	LoadUI("返回", new CButtonUI("返回", 0, 0, 90, 30));
	draw = false;
	LoadUI("显示框", new CEditUI(0,"", 860 - 60, 0, 185, 450,"pic\\选择角色界面\\0.png"));
	LoadUI("发送框", new CEditUI(0,"", 890 - 90, 460, 145, 30,"pic\\选择角色界面\\0.png"));
	LoadUI("发送按钮", new CButtonUI("发送", 870 - 60, 500, 90, 30));
	CEditUI* eb1 = (CEditUI*)GetUI("显示框");
	CEditUI* eb2 = (CEditUI*)GetUI("发送框");
	CGameEngine::GetGE()->GetGO()->LoadFont("城镇场景字体",10, 12, 100, false, false, false, "宋体");
	eb1->m_red = true;
	eb2->m_red = false;
	SuoYouWanJiaZhunBei = false;
	g_mapX = g_mapY = 0;
}

void CChengZheng::Enter()
{
	edit_string.clear();

	if(!g_fanhui)
	{
		char name[32];
		_C_S_GP13 csgp13;
		int len = 0;

		while(m_Nick_Name.c_str()[len])
			len++;
		for(int i = 0; i < len+1; i++)
		{
			name[i] = m_Nick_Name.c_str()[i];
			csgp13.pyr.name[i] = m_Nick_Name.c_str()[i];
		}
		m_Hero = new CHero(460,325,400000,400000,name,false,54,144);
		m_Hero->m_iScene = scene;
		if(FangZhu)
		{
			m_Hero->m_bFangZhu = true;
			m_Hero->m_bZhunBei = false;
		}
		m_player.insert(std::pair<std::string,CHero*>(m_Nick_Name,m_Hero));

		//发送玩家数据
		csgp13.pyr.hp = 400000;
		csgp13.fanhui = g_fanhui;
		csgp13.pyr.image = false;
		csgp13.pyr.mp = 400000;
		csgp13.pyr.PicBegin = 0;
		csgp13.pyr.PicEnd = 22;
		csgp13.pyr.scene = 0;
		csgp13.pyr.state = _HERO_STAND;
		csgp13.pyr.x = m_Hero->m_iX;
		csgp13.pyr.y = m_Hero->m_iY;
		csgp13.pyr.FangZhu = m_Hero->m_bFangZhu;
		csgp13.pyr.ZhunBei = m_Hero->m_bZhunBei;
		csgp13.pyr.scene = m_Hero->m_iScene;

		CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp13,sizeof(csgp13));
	}
	else
	{
		_C_S_GP13 csgp13;
		csgp13.fanhui = g_fanhui;
		csgp13.pyr.scene = scene;
		CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp13,sizeof(csgp13));

	}

	for(std::map<std::string,CHero*>::iterator it = m_player.begin();it != m_player.end(); it++)
	{
		it->second->m_iX  = 200;
		it->second->m_iY = 400;
		it->second->_WORLD_W_ = 940;
		it->second->_WORLD_H_ = 556;
	}

}

void CChengZheng::Quit()
{
	//std::map<std::string,CHero*>::iterator it = m_player.find(m_Hero->m_cName);
	//m_player.clear();
	//FangZhu = false;
}

void CChengZheng::OutputRun()
{

	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\选择房间界面\\0.png",g_mapX, g_mapY, 890, 556,0,0);
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\选择角色界面\\0.png",860 - 60, 200, 185, 450,0,0);
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\PVP贴图\\0\\3.png",860 - 60 - 275, 0, 275, 398,0,0,0,200);

	//准备贴图
	if(m_Hero->m_bFangZhu)//是否是房主
		CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\PVP贴图\\1\\开始.png",860 - 60 - 192, 360, 119, 24,0, 0,0,200);
	else
	{
		if(m_Hero->m_bZhunBei)
			CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\PVP贴图\\1\\取消.png",860 - 60 - 192, 360, 119, 24,0, 0,0,200);
		else
			CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\PVP贴图\\1\\准备.png",860 - 60 - 192, 360, 119, 24,0, 0,0,200);
	}

	//聊天
	if(draw)
	{
		std::string s = "";
		for(std::vector<std::string>::iterator i = edit_string.begin();i != edit_string.end(); ++i)
		{
			s += (*i);
			s += "\n";
		}
		CEditUI* eb1 = (CEditUI*)GetUI("显示框");
		eb1->SetText(s.c_str());
		draw = false;
	}


	for(int i = 0, y = 120; i < 9; i++,y += 20)
		CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\PVP贴图\\1\\1.png",860 - 60 - 262, y - 5, 258, 22,0, 0);

	//遍历英雄表
	int y = 120;

	for(std::map<std::string,CHero*>::iterator it = m_player.begin(); it != m_player.end(); it++,y += 20)
	{
		it->second->Run();
		CGameEngine::GetGE()->GetGO()->DrawString1("城镇场景字体", 860 - 60 - 240, y,it->second->m_cName, RGB(255, 0, 0));
		if(it->second->m_bFangZhu)//房主
			CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\PVP贴图\\1\\8.png",860 - 60 - 260, y - 5, 17,19,0, 0);
		else if(it->second->m_bZhunBei)//不是房主
			CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\PVP贴图\\1\\7.png",860 - 60 - 42, y - 5, 42,20,0, 0);
	}



	double hp = m_Hero->GetHP();
	double mp = m_Hero->GetMP();
	double h = hp / 400000;
	double m = mp / 400000;
	double hph = 56 * h;
	double mph = 56 * m;

	int pichpy = 56 - hph;
	int picmpy = 56 - mph;

	int hpx = 0 + 13;
	int hpy = 556 - 75 + pichpy + 56 - hph - 1;
	int mpx = 875 - 56 - 13 - 87 + 13;
	int mpy = 556 - 75 + picmpy + 56 - mph - 1;
	//贴血量
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\血槽\\0.png",0,556 - 91,800,91,0,0);
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\血槽\\1.png",hpx,hpy,56,hph,0,pichpy);
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\血槽\\2.png",mpx,mpy,56,mph,0,picmpy);

	//绘制移动射击冷却时间
	char buf[20];
	itoa(600 - CSkill_Move_Shooting::m_dangqianCDTimer,buf,10);
	CGameEngine::GetGE()->GetGO()->DrawString1("城镇场景字体", 540, 500 + 20,buf, RGB(255, 255, 0));

	//绘制卫星射线冷却时间
	itoa(999 - m_Hero->m_dangqianCDTimer,buf,10);
	CGameEngine::GetGE()->GetGO()->DrawString1("城镇场景字体", 570, 500 + 20,buf, RGB(255, 255, 0));
}

void CChengZheng::LogicInputRun()
{
	//上下左右按键标记
	static bool falg = true;
	static bool movefalg = true;


	HWND hwnd = CGameEngine::GetGE()->GetHwnd();
	//玩家准备与取消
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_X) == _KS_DC && GetForegroundWindow() == hwnd)
	{
		if(m_Hero->m_bFangZhu)//是否是房主
		{
			bool kaishi = true;
			for(std::map<std::string,CHero*>::iterator it = m_player.begin(); it != m_player.end(); ++it)
			{
				if(it->second->m_bZhunBei == false && it->second != m_Hero)
				{
					kaishi = false;
					break;
				}
			}
			if(kaishi && m_player.size() >= 2)
			{
				_C_S_GP15 cs15;
				cs15.scene = m_Hero->m_iScene;
				CGameEngine::GetGE()->GetTcpClient()->SendData((const char*)&cs15,sizeof(cs15));
			}

		}
		else
		{
			m_Hero->m_bZhunBei = m_Hero->m_bZhunBei ? false : true;
			//发送准备协议
			_C_S_GP6 cs6;
			cs6.master = m_Hero->m_bFangZhu;
			cs6.prepare = m_Hero->m_bZhunBei;
			cs6.scene = m_Hero->m_iScene;
			CGameEngine::GetGE()->GetTcpClient()->SendData((const char*)&cs6,sizeof(cs6));
		}

	}
	//上
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_UP) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{

		if(falg)
		{
			m_Hero->m_bMove = true;
			falg = false;
		}
		m_Hero->m_Hero_State.state = _HERO_MOVE_;
		m_Hero->m_iMoveDir = _MOVE_UP;
		m_Hero->Heroyidong(_MOVE_UP);
		//发送移动协议
		_C_S_GP4 csgp4;
		csgp4.bMove = m_Hero->m_bMove;
		csgp4.MoveDir = _MOVE_UP;
		csgp4.state = _HERO_MOVE_;
		csgp4.x = m_Hero->GetX();
		csgp4.y = m_Hero->GetY();
		csgp4.image = m_Hero->GetPicDir();
		CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp4,sizeof(csgp4));

	}
	//下
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_DOWN) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{

		if(falg)
		{
			m_Hero->m_bMove = true;
			falg = false;
		}
		m_Hero->m_Hero_State.state =_HERO_MOVE_;
		m_Hero->m_iMoveDir = _MOVE_DOWN;
		m_Hero->Heroyidong(_MOVE_DOWN);

		//发送移动协议
		_C_S_GP4 csgp4;
		csgp4.bMove = m_Hero->m_bMove;
		csgp4.MoveDir = _MOVE_DOWN;
		csgp4.state = _HERO_MOVE_;
		csgp4.x = m_Hero->GetX();
		csgp4.y = m_Hero->GetY();
		csgp4.image = m_Hero->GetPicDir();
		CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp4,sizeof(csgp4));


	}
	//左
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_LEFT) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{

		if(falg)
		{
			m_Hero->m_bMove = true;
			falg = false;
		}
		m_Hero->m_Hero_State.state =_HERO_MOVE_;
		m_Hero->m_iMoveDir = _MOVE_LEFT;
		m_Hero->Heroyidong(_MOVE_LEFT);


		//发送移动协议
		_C_S_GP4 csgp4;
		csgp4.bMove = m_Hero->m_bMove;
		csgp4.MoveDir = _MOVE_LEFT;
		csgp4.state = _HERO_MOVE_;
		csgp4.x = m_Hero->GetX();
		csgp4.y = m_Hero->GetY();
		csgp4.image = m_Hero->GetPicDir();
		CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp4,sizeof(csgp4));


	}
	//右
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_RIGHT) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{

		if(falg)
		{
			m_Hero->m_bMove = true;
			falg = false;
		}
		m_Hero->m_Hero_State.state = _HERO_MOVE_;
		m_Hero->m_iMoveDir = _MOVE_RIGHT;
		m_Hero->Heroyidong(_MOVE_RIGHT);


		//发送移动协议
		_C_S_GP4 csgp4;
		csgp4.bMove = m_Hero->m_bMove;
		csgp4.MoveDir = _MOVE_RIGHT;
		csgp4.state = _HERO_MOVE_;
		csgp4.x = m_Hero->GetX();
		csgp4.y = m_Hero->GetY();
		csgp4.image = m_Hero->GetPicDir();
		CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp4,sizeof(csgp4));


	}


	//没有按键盘
	if((CGameEngine::GetGE()->GetGI()->Get(_GI_K_UP) == _KS_UH)  &&
		(CGameEngine::GetGE()->GetGI()->Get(_GI_K_DOWN) == _KS_UH) &&
		(CGameEngine::GetGE()->GetGI()->Get(_GI_K_LEFT) == _KS_UH) &&
		(CGameEngine::GetGE()->GetGI()->Get(_GI_K_RIGHT) == _KS_UH)&& 
		(CGameEngine::GetGE()->GetGI()->Get(_GI_K_A) == _KS_UH)    && 
		(CGameEngine::GetGE()->GetGI()->Get(_GI_K_X) == _KS_UH)	 &&
		(CGameEngine::GetGE()->GetGI()->Get(_GI_K_Z) == _KS_UH) && 
		!m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath)
	{

		m_Hero->m_Hero_State.state = _HERO_STAND;
		if(m_Hero->m_Inputflag)
		{
			falg = true;
			movefalg = true;
			m_Hero->m_bStand = true;
			m_Hero->m_Inputflag = false;
			//发送待机状态消息
			_C_S_GP17 cs17;
			cs17.bStand = m_Hero->m_bStand;
			cs17.image = m_Hero->m_bPicdir;
			cs17.state = _HERO_STAND;
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs17,sizeof(cs17));

		}



	}


}

void CChengZheng::OnUIMsg(const char* id, int UIMSG)
{
	if (UIMSG == _BUTTON_CLICK)
	{
		if (strcmp(id, "发送按钮") == 0)
		{
			CEditUI* eb1 = (CEditUI*)GetUI("发送框");
			std::string s = eb1->GetText();

			int len = strlen(s.c_str());
			if (len > 0)
			{
				if(edit_string.size() >= 26)
				{
					std::vector<std::string>::iterator i = edit_string.begin();
					edit_string.erase(i);
				}
				std::string s = m_Nick_Name;
				s += "说:";
				s += eb1->GetText();
				edit_string.push_back(s);
				_C_S_GP3 csgp3;
				strcpy_s(csgp3.str,128,s.c_str());
				//	csgp3.scene = m_Hero->m_iScene;
				csgp3.scene = m_Hero->m_iScene;
				CGameEngine::GetGE()->GetTcpClient()->SendData((const char*)&csgp3,sizeof(csgp3));// + strlen(csgp3.str) + 1);
				eb1->SetText("");

				draw = true;
			}
			else
			{
				MessageBox(CGameEngine::GetGE()->GetHwnd(),"请输入内容!","提示",MB_OK);
			}
		}
		else if (strcmp(id, "结束按钮") == 0)
			CGameEngine::GetGE()->ExitGame();
		else if (strcmp(id, "返回") == 0)
		{
			//发送退出房间协议
			_C_S_GP10 sc10;
			sc10.room_index = m_Hero->m_iScene;
			sc10.fangzhu = m_Hero->m_bFangZhu;
			CGameEngine::GetGE()->GetTcpClient()->SendData((const char*)&sc10,sizeof(sc10));
			CGameEngine::GetGE()->SetCurScene("选择房间");
			FangZhu = false;
			g_fanhui = false;
			m_player.clear();
		}
	}
}
void CChengZheng::OnNetEvent(_TC_NET_EVENT* ptne)
{
	switch (ptne->type)
	{
	case _TCNE_S_DISCONNECT:
	case _TCNE_C_DISCONNECT:
	case _TCNE_E_DISCONNECT:
		{
			//all_m_player.clear();

		}break;
	case _TCNE_DATA:
		{
			_GAME_PROTOCOL* pgp = (_GAME_PROTOCOL*)ptne->data;
			switch (pgp->id)
			{
			case _NICK_NAME_LEGALITY:
				{
					_S_C_GP3* p = (_S_C_GP3*)ptne->data;
					Beep(500, 1000);
					CGameEngine::GetGE()->SetCurScene("游戏场景");

				}break;
			case _PLAYER_ENTER_TOWN://进入城镇
				{
					_S_C_GP22* p = (_S_C_GP22*)ptne->data;
					CHero* hero = new CHero(p->pyr.x,p->pyr.y,p->pyr.hp,p->pyr.mp,p->pyr.name,p->pyr.image,54,144);
					
					hero->m_bFangZhu = p->pyr.FangZhu;
					hero->m_bZhunBei = p->pyr.ZhunBei;
					m_player.insert(std::pair<std::string,CHero*>(p->pyr.name,hero));
					if(edit_string.size() >= 26)
					{
						std::vector<std::string>::iterator i = edit_string.begin();
						edit_string.erase(i);
					}
					std::string s = p->pyr.name;
					s += ":加入游戏。";
					edit_string.push_back(s);
					draw = true;

				}break;
			case _RETURN_PLAYER_TOWN://返回玩家数据
				{
					_S_C_GP23* p = (_S_C_GP23*)ptne->data;
					for(int i = 0;i < p->num; i++)
					{
						CHero* hero = new CHero(p->pyr[i].x,p->pyr[i].y,p->pyr[i].hp,p->pyr[i].mp,p->pyr[i].name,p->pyr[i].image,54,144);
						hero->m_bFangZhu = p->pyr[i].FangZhu;
						hero->m_bZhunBei = p->pyr[i].ZhunBei;
						m_player.insert(std::pair<std::string,CHero*>(hero->m_cName,hero));

					}

				}break;
			case _PLAYER_WALK://移动
				{
					_S_C_GP6* scgp6 = (_S_C_GP6*)ptne->data;
					std::map<std::string,CHero*>::iterator it =  m_player.find(scgp6->pyr.name);
					
					if(it == m_player.end())
						return;

					
					it->second->m_Hero_State.state = scgp6->pyr.state;
					it->second->m_bPicdir = scgp6->pyr.image;
					it->second->m_iX = scgp6->pyr.x;
					it->second->m_iY = scgp6->pyr.y;
					it->second->m_bMove = scgp6->pyr.bMove;
					it->second->m_iMoveDir = scgp6->pyr.bMovedir;

				}break;
			case _PLAYER_QUIT_GAME://退出	
				{
					_S_C_GP19* sc19 = (_S_C_GP19*)ptne->data;
					std::map<std::string,CHero*>::iterator ik = m_player.find(sc19->name);

					if(ik != m_player.end())
						m_player.erase(ik);

					std::string s = sc19->name;
					s += "退出游戏";
					edit_string.push_back(s);
					draw = true;

				}break;
			case _PLAYER_TALK://玩家聊天
				{
					if(edit_string.size() >= 26)
					{
						std::vector<std::string>::iterator i = edit_string.begin();
						edit_string.erase(i);
					}		
					_S_C_GP5* p = (_S_C_GP5*)ptne->data;
					std::string s = p->str;
					edit_string.push_back(s);
					draw = true;

				}break;
			case _PLAYER_PREPARE://玩家准备与否
				{
					_S_C_GP9* sc9 = (_S_C_GP9*)ptne->data;
					std::map<std::string,CHero*>::iterator ik = m_player.find(sc9->name);
					
					if(ik == m_player.end())
						return;

					
					ik->second->m_bZhunBei = sc9->prepare;

				}break;
			case _JIN_RU_JUE_DOU://进入决斗场景
				{
					_S_C_GP25* sc25 = (_S_C_GP25*)ptne->data;
					for(int i = 0; i < sc25->num; ++i)
					{
						std::map<std::string,CHero*>::iterator it = m_player.find(sc25->pyr[i].name);
						
						if(it == m_player.end())
							return;

					
						it->second->m_Hero_State.state = sc25->pyr[i].state;
						it->second->m_bPicdir = sc25->pyr[i].image;
						it->second->m_iPicbegin = sc25->pyr[i].PicBegin;
						it->second->m_iPicEnd = sc25->pyr[i].PicEnd;
						it->second->m_iX = sc25->pyr[i].x;
						it->second->m_iY = sc25->pyr[i].y;
						it->second->m_bZhunBei = false;
						it->second->m_bFangZhu = sc25->pyr[i].FangZhu;
						it->second->_WORLD_W_ = 1559;
						it->second->_WORLD_H_ = 556;
					}
					Beep(500, 1000);
					CGameEngine::GetGE()->SetCurScene("决斗场景");

				}break;
			case _PLAYER_QUIT_ROOM://玩家退出房间
				{
					_S_C_GP16* sc16 = (_S_C_GP16*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc16->name);
					
					if(it == m_player.end())
						return;

					
					m_player.erase(it);

				}break;
			case _SET_FANG_ZHU://设置新房主
				{
					_S_C_GP30* sc30 = (_S_C_GP30*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc30->name);
					
					if(it == m_player.end())
						return;

					
					it->second->m_bFangZhu = true;

				}break;
			case _ZHUN_FA_DAI_JI://转发待机状态
				{
					_S_C_GP31* sc31 = (_S_C_GP31*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc31->pyr.name);
					
					if(it == m_player.end())
						return;

					
					it->second->m_bStand = sc31->pyr.bStand;
					it->second->m_Hero_State.state = sc31->pyr.state;
					it->second->m_bPicdir = sc31->pyr.image;
		
				}break;
			case _FANHUI_PLY://返回玩家数据
				{
					_S_C_GP34* p = (_S_C_GP34*)ptne->data;
					for(int i = 0;i < p->num; i++)
					{
						CHero* hero = new CHero(p->pyr[i].x,p->pyr[i].y,p->pyr[i].hp,p->pyr[i].mp,p->pyr[i].name,p->pyr[i].image,54,144);
						hero->m_bFangZhu = p->pyr[i].FangZhu;
						hero->m_bZhunBei = p->pyr[i].ZhunBei;
						hero->m_iScene = p->pyr[i].scene;
						m_player.insert(std::pair<std::string,CHero*>(hero->m_cName,hero));

						if(strcmp(p->pyr[i].name, m_Nick_Name.c_str()) == 0)
						{
							m_Hero = hero;
							scene = p->pyr[i].scene;
						}

					}
				}break;

			}
			//case _RETURN_TONG_BU://同步数据
			//	{
			//		_S_C_GP24* sc24 = (_S_C_GP24*)ptne->data;
			//		for(int i = 0;i < sc24->num; ++i)
			//		{
			//			std::map<std::string,CHero*>::iterator it =  m_player.find(sc24->pyr[i].name);
			//			it->second->m_Hero_State.state = sc24->pyr[i].state;
			//			it->second->m_bPicdir = sc24->pyr[i].image;
			//			it->second->m_iPicbegin = sc24->pyr[i].PicBegin;
			//			it->second->m_iPicEnd = sc24->pyr[i].PicEnd;
			//			it->second->m_iX = sc24->pyr[i].x;
			//			it->second->m_iY = sc24->pyr[i].y;
			//		}
			//	}break;
		}break;

	}

}