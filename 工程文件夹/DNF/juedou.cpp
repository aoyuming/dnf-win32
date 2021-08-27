#include "juedou.h"
#include "GameEngine.h"
#include "ButtonUI.h"
#include "BmpButtonUI.h"
//#include "1.h"
#include <iostream>
#include <map>
#include <string>
#include "EditUI.h"
#include "DNFprotocol.h"


void CJueDou::Init()
{
	draw = false;
	CGameEngine::GetGE()->GetGO()->LoadFont("决斗场景字体",18, 18, 100, false, false, false, "黑体");
	LoadUI("显示框", new CEditUI(0,"", 860 - 60, 0, 185, 450,"pic\\选择角色界面\\0.png"));
	LoadUI("发送框", new CEditUI(20,"", 890 - 90, 460, 145, 30,"pic\\选择角色界面\\0.png"));
	LoadUI("发送按钮", new CButtonUI("发送", 870 - 60, 500, 90, 30));
	CEditUI* eb1 = (CEditUI*)GetUI("显示框");
	CEditUI* eb2 = (CEditUI*)GetUI("发送框");
	eb1->m_red = true;
	eb2->m_red = false;

	g_mapX = g_mapY = 0;
}

void CJueDou::Enter()
{	
	//char name[32];
	//_C_S_GP13 csgp13;
	//int len = 0;

	//while(m_Nick_Name.c_str()[len])
	//	len++;
	//for(int i = 0; i < len+1; i++)
	//{
	//	name[i] = m_Nick_Name.c_str()[i];
	//	csgp13.pyr.name[i] = m_Nick_Name.c_str()[i];
	//}
	//m_Hero = new CHero(460,325,400000,400000,name,false,54,144);
	//m_Hero->m_iScene = scene;
	//if(FangZhu)
	//{
	//	m_Hero->m_bFangZhu = true;
	//	m_Hero->m_bZhunBei = true;
	//}
	//m_player.insert(std::pair<std::string,CHero*>(m_Nick_Name,m_Hero));

	////发送玩家数据
	//csgp13.pyr.hp = 400000;
	//csgp13.pyr.image = false;
	//csgp13.pyr.mp = 400000;
	//csgp13.pyr.PicBegin = 0;
	//csgp13.pyr.PicEnd = 22;
	//csgp13.pyr.scene = 0;
	//csgp13.pyr.state = _HERO_STAND;
	//csgp13.pyr.x = m_Hero->m_iX;
	//csgp13.pyr.y = m_Hero->m_iY;
	//csgp13.pyr.FangZhu = m_Hero->m_bFangZhu;
	//csgp13.pyr.ZhunBei = m_Hero->m_bZhunBei;
	//csgp13.pyr.scene = m_Hero->m_iScene;

	//CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&csgp13,sizeof(csgp13));

}

void CJueDou::Quit()
{
	//delete m_Hero;
	/*for(std::vector<CSkill_Move_Shooting*>::iterator it = m_Hero->m_Move_Shooting.begin(); it != m_Hero->m_Move_Shooting.end(); it++)
	delete *it;
	for(std::map<std::string,CHero*>::iterator ik = m_player.begin(); ik != m_player.end(); ik++)
	delete *ik;
	for(std::vector<CSkill_Satellite_Ray*>::iterator it = m_WXSX.begin(); it != m_WXSX.end();it++)
	delete *it;
	for(std::vector<CHram_String*>::iterator it = m_vHram_String.begin(); it != m_vHram_String.end();it++ )
	delete *it;

	m_Hero->m_Move_Shooting.clear();
	m_WXSX.clear();
	m_ThatPlayer.clear();
	m_vHram_String.clear();*/
}

void CJueDou::OutputRun()
{
	CGameEngine::GetGE()->GetGO()->DrawPic2("pic\\城镇\\0.png",g_mapX, g_mapY, 1559, 556,0, 0);

	if(draw)
	{
		std::string s = "";
		for(std::vector<std::string>::iterator i = edit_string.begin();i != edit_string.end(); ++i)
		{
			s += (*i);
			s += "\n";
		}
		CEditUI* eb1 = (CEditUI*)GetUI("显示框");
	/*	if(s.c_str() )*/
		eb1->SetText(s.c_str());
		draw = false;
	}


		////遍历手炮子弹表
		//for(std::map<std::string,CHero*>::iterator ik = m_player.begin(); ik != m_player.end(); ik++)
		//{
		//	for(std::vector<CSkill_Move_Shooting*>::iterator it = ik->second->m_Move_Shooting.begin(); it != ik->second->m_Move_Shooting.end(); it++)
		//	{
		//		for(std::map<std::string,CHero*>::iterator ij = m_player.begin(); ij != m_player.end(); ij++)
		//		{
		//			if(ij == ik || ij->second->m_bDeath)
		//				continue;

		//			RECT r;
		//			bool flag = false;//在同一Y轴
		//			const RECT r1 = (*it)->GetRect();
		//			const RECT r2 = ij->second->GetPicRect();
		//			//判断是否在同一Y轴上
		//			for(int i = ij->second->GetY() + ij->second->GetPicW() - 75; i <  ij->second->GetY() + ij->second->GetPicW() + 75; i++)
		//			{
		//				if(i == ij->second->GetY() + ij->second->GetPicW())
		//				{
		//					flag = true;
		//					break;
		//				}
		//			}

		//			if(IntersectRect(&r,&r1,&r2) && flag)
		//			{
		//				if((*it)->m_iPicBegin == 0)
		//				{
		//					ij->second->m_iJiangZhiDu = 3;
		//					ij->second->m_Hero_State.state = _HERO_JIANG_ZHI;
		//					ij->second->SetPicDir(!(*it)->m_bImage);
		//					ij->second->m_iJiangZhiZT = true;
		//					CHram_String* hstr = new CHram_String(r.left,r.top,(*it)->m_iHarm);
		//					m_vHram_String.push_back(hstr);
		//					ij->second->SetHP(ij->second->GetHP() - (*it)->m_iHarm);
		//				}

		//			}
		//		}
		//	}
		//}

	//遍历手炮子弹表
	for(std::vector<CSkill_Move_Shooting*>::iterator it = m_Hero->m_Move_Shooting.begin(); it != m_Hero->m_Move_Shooting.end(); it++)
	{
		for(std::map<std::string,CHero*>::iterator ik = m_player.begin(); ik != m_player.end(); ik++)
		{
			if(ik->second->m_bDeath && ik->second == m_Hero)
				continue;

			RECT r;
			bool flag = false;//在同一Y轴
			const RECT r1 = (*it)->GetRect();
			const RECT r2 = ik->second->GetPicRect();
			//判断是否在同一Y轴上
			for(int i = m_Hero->GetY() + m_Hero->GetPicW() - 75; i <  m_Hero->GetY() + m_Hero->GetPicW() + 75; i++)
			{
				if(i == ik->second->GetY() + ik->second->GetPicW())
				{
					flag = true;
					break;
				}
			}

			if(IntersectRect(&r,&r1,&r2) && flag)
			{
				if((*it)->m_iPicBegin == 0)
				{
					ik->second->m_iJiangZhiDu = 3;
					ik->second->m_Hero_State.state = _HERO_JIANG_ZHI;
					ik->second->SetPicDir(!(*it)->m_bImage);
					ik->second->m_iJiangZhiZT = true;
					CHram_String* hstr = new CHram_String(r.left,r.top,(*it)->m_iHarm);
					m_vHram_String.push_back(hstr);
					ik->second->SetHP(ik->second->GetHP() - (*it)->m_iHarm);

					//发送技能打中人协议
					_C_S_GP12 cs12;
					cs12.hram = (*it)->m_iHarm;
					cs12.image = !(*it)->m_bImage;
					cs12.JiangZhiDU = 3;
					cs12.JiangZhiZT = true;
					cs12.state = _HERO_JIANG_ZHI;
					cs12.x = r.left;
					cs12.y = r.top;
					cs12.hp = ik->second->GetHP();
					strcpy_s(cs12.name,32,ik->second->GetName());
					CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs12,sizeof(cs12));
				}

			}
		}

	}

	


	////遍历卫星射线表
	//for(std::vector<CSkill_Satellite_Ray*>::iterator it = m_WXSX.begin(); it != m_WXSX.end();)
	//{

	//	if((*it)->m_delete)
	//	{
	//		delete *it;
	//		it = m_WXSX.erase(it);
	//	}
	//	else
	//	{
	//		(*it)->Run();
	//		CVector2D pos;
	//		CVector2D hero;
	//		hero.x = (float)m_Hero->GetX() - 30;
	//		hero.y = (float)m_Hero->GetY() + 50;
	//		pos.x = (float)(*it)->m_iX;
	//		pos.y = (float)(*it)->m_iY;
	//		CVector2D v2 = hero - pos;
	//		CVector2D v3 = v2.Normalize();
	//		v3 *= 3.0;
	//		pos += v3;
	//		(*it)->m_iX = (int)pos.x;
	//		(*it)->m_iY = (int)pos.y;
	//		for(std::map<std::string,CHero*>::iterator ik = m_player.begin(); ik != m_player.end(); ik++)
	//		{
	//			if(ik->second->m_bDeath ||strcmp( m_Hero->GetName(),ik->first.c_str()) == 0)
	//				continue;

	//			RECT r;
	//			bool flag = false;//在同一Y轴
	//			const RECT r1 = (*it)->GetRect();
	//			const RECT r2 = ik->second->GetPicRect();
	//			//判断是否在同一Y轴上
	//			/*			for(int j = (*ik)->GetY(); j <  (*ik)->GetY() + (*ik)->GetPicH();j++)
	//			{
	//			if(j == (*it)->m_iY + (*it)->m_iPicH)
	//			{
	//			flag = true;
	//			break;
	//			}
	//			}*/


	//			if(IntersectRect(&r,&r1,&r2))// && flag)
	//			{
	//				static int i = 0;
	//				if((*it)->m_iPicbeg == 0 || (*it)->m_iPicbeg == 3 || (*it)->m_iPicbeg == 5 && i >= 30)
	//				{
	//					i = 0;
	//					ik->second->m_iJiangZhiDu = 20;
	//					ik->second->m_Hero_State.state = _HERO_JIANG_ZHI;
	//					ik->second->SetPicDir(!(*it)->m_bImage);
	//					ik->second->m_iJiangZhiZT = true;
	//					CHram_String* hstr = new CHram_String(r.left,r.top,(*it)->m_iHarm);
	//					m_vHram_String.push_back(hstr);
	//					ik->second->SetHP(ik->second->GetHP() - (*it)->m_iHarm);

	//				}
	//				i += 1;

	//			}
	//		}
	//		it++;

	//	}

	//}

	//遍历英雄表
	int k = 0;
	for(std::map<std::string,CHero*>::iterator it = m_player.begin(); it != m_player.end(); it++)
	{
		it->second->Run();
		it->second->TieHP_MP(k * 100,35);

		if(it->second->m_WXSX_luoxia)
		{
			it->second->m_WXSX_luoxia = false;
			CSkill_Satellite_Ray* p = new CSkill_Satellite_Ray(it->second->m_WXSX_X + 100,it->second->m_WXSX_Y + 100,0,4,300,0,50,50);
			m_WXSX.push_back(p);
		}
		k += 1;

	}

	//遍历数字伤害表
	for(std::vector<CHram_String*>::iterator it = m_vHram_String.begin(); it != m_vHram_String.end(); )
	{
		if((*it)->m_delete)
		{
			delete *it;
			it = m_vHram_String.erase(it);
		}
		else
		{
			(*it)->Run();
			it++;
		}
	}

	////便利子弹表
	//for(std::vector<CBullet*>::iterator it = m_Vbullet.begin(); it != m_Vbullet.end(); )
	//{
	//	bool flag = false;
	//	for(std::map<std::string,CHero*>::iterator ik = m_player.begin(); ik != m_player.end(); ++ik)
	//	{
	//		if(&*ik->second == m_Hero || (*ik).second->m_bDeath)
	//			continue;

	//		RECT r;
	//		const RECT r1 = (*it)->m_rRect;
	//		const RECT r2 = ik->second->GetPicRect();
	//		if(IntersectRect(&r,&r1,&r2))
	//		{
	//			flag = true;
	//			ik->second->m_iJiangZhiDu = 20;
	//			ik->second->m_Hero_State.state = _HERO_JIANG_ZHI;
	//			ik->second->SetPicDir(!(*it)->m_bImage);
	//			ik->second->m_iJiangZhiZT = true;
	//			CHram_String* hstr = new CHram_String(r.left,r.top,(*it)->m_iHarm);
	//			m_vHram_String.push_back(hstr);
	//			ik->second->SetHP(ik->second->GetHP() - (*it)->m_iHarm);
	//			continue;
	//		}
	//	}
	//	if(flag)
	//	{
	//		delete *it;
	//		it = m_Vbullet.erase(it);
	//	}
	//	else
	//	{
	//		(*it)->Run();
	//		++it;
	//	}
	//}



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

	
	if(m_Hero->m_bFangZhu)
	{
		int renshu = m_player.size();
		int js = 0;
		for(std::map<std::string,CHero*>::iterator it = m_player.begin(); it != m_player.end(); it++)
		{
			if(it->second->GetHP() <= 0)
				js += 1;
		}
		if(js >= renshu - 1)
		{
			//发送比赛结束协议
			_C_S_GP19 cs19;
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs19,sizeof(cs19));
		}
	}


}

void CJueDou::LogicInputRun()
{


	HWND hwnd = CGameEngine::GetGE()->GetHwnd();
	//移动射击
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_A) == _KS_DC 
		&& CSkill_Move_Shooting::m_iCD && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath && GetForegroundWindow() == hwnd && !m_Hero->m_WXSX_anxia && !m_Hero->m_PuTongGongJi.state)
	{
		if(m_Hero->m_Hero_State.state != _HERO_MOVE_SHOOT)
		{
			//发送进入移动射击状态协议
			_C_S_GP7 cs7;
			cs7.bShooting_move = true;
			cs7.image = m_Hero->m_bPicdir;
			cs7.state = _HERO_MOVE_SHOOT;
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs7,sizeof(cs7));
		}

		m_Hero->m_Hero_State.state = _HERO_MOVE_SHOOT;
		CSkill_Move_Shooting::m_iCD = false;
		m_Hero->m_YiDongSheJiAnXia = true;
		m_Hero->m_bShooting_move[0] = true;

	}
	////卫星射线S
	//if(CGameEngine::GetGE()->GetGI()->Get(_GI_K_S) == _KS_DC 
	//	&& !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath && m_Hero->m_bWXSXCD  && GetForegroundWindow() == hwnd && !m_Hero->m_YiDongSheJiAnXia && !m_Hero->m_PuTongGongJi.state)
	//{
	//	m_Hero->m_WXSX_anxia = true;
	//	m_Hero->m_bWXSXCD = false;
	//	m_Hero->m_Hero_State.state = _HERO_WEI_XING_SHE_XIAN;

	//}


	//Z
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_Z) == _KS_DC && GetForegroundWindow() == hwnd)
	{
		if(m_Hero->m_YiDongSheJiAnXia)
		{
			m_Hero->Move_Shooting_Dir();
			//发送移动射击转换方向
			_C_S_GP100 cs100;
			cs100.dir = m_Hero->m_bPicdir;
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs100,sizeof(cs100));
		}
	}

	//X
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_X) == _KS_DC && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd )
	{
		if(m_Hero->m_YiDongSheJiAnXia)
		{
			m_Hero->m_Hero_State.state = _HERO_MOVE_ATTACK;
			m_Hero->MoveShootingAttack();
			CSkill_Move_Shooting::m_iNum += 1;

			//移动射击状态下攻击
			_C_S_GP18 cs18;
			cs18.bShooting_move2 = m_Hero->m_bShooting_move[2];
			cs18.image = m_Hero->GetPicDir();
			cs18.state = m_Hero->m_Hero_State.state;
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs18,sizeof(cs18));
		}
		//else//普通攻击
		//{
		//	m_Hero->m_PuTongGongJi.state = true;
		//	m_Hero->m_Hero_State.state = _HERO_PUTONGGONGJI;

		//}
	}
	if(m_Hero->m_PuTongGongJi.state)
	{
		m_Hero->m_PuTongGongJi.state = false;

		int x = (m_Hero->m_bPicdir) ? m_Hero->m_iX - 100 : m_Hero->m_iX + 100;
		CBullet* p = new CBullet(x,m_Hero->m_iY + 10,5,37,0,0,500,m_Hero->m_bPicdir);
		m_Vbullet.push_back(p);
	}

	//C
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_C) == _KS_DC && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{
		m_Hero->End_MoveShootingk();
	}

	//上下左右按键标记
	static bool falg = true;
	static bool movefalg = true;



	//上
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_UP) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{
		if(m_Hero->m_YiDongSheJiAnXia)//是否是移动射击状态
		{
			m_Hero->m_Hero_State.state = _HERO_MOVE_SHOOTING_MOVE;
			if(movefalg)
			{
				m_Hero->m_bShooting_move[1] = true;
				movefalg = false;
			}
			m_Hero->Heroyidong(_MOVE_UP);

			_C_S_GP8 cs8;//移动射击状态下移动
			cs8.bShooting_move1 = m_Hero->m_bShooting_move[1];
			cs8.image = m_Hero->GetPicDir();
			cs8.state = m_Hero->m_Hero_State.state;
			cs8.x = m_Hero->GetX();
			cs8.y = m_Hero->GetY();
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs8,sizeof(cs8));
		}
		else if(m_Hero->m_WXSX_anxia)
		{
			m_Hero->m_WXSX_Y -= 15;
		}
		else
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

	}
	//下
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_DOWN) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath
		&& !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{
		if(m_Hero->m_YiDongSheJiAnXia)//是否是移动射击状态
		{
			m_Hero->m_Hero_State.state = _HERO_MOVE_SHOOTING_MOVE;
			m_Hero->Heroyidong(_MOVE_DOWN);
			if(movefalg)
			{
				m_Hero->m_bShooting_move[1] = true;
				movefalg = false;
			}
			_C_S_GP8 cs8;//移动射击状态下移动
			cs8.bShooting_move1 = m_Hero->m_bShooting_move[1];
			cs8.image = m_Hero->GetPicDir();
			cs8.state = m_Hero->m_Hero_State.state;
			cs8.x = m_Hero->GetX();
			cs8.y = m_Hero->GetY();
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs8,sizeof(cs8));

		}
		else if(m_Hero->m_WXSX_anxia)
		{
			m_Hero->m_WXSX_Y += 15;
		}
		else
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

	}
	//左
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_LEFT) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{
		if(m_Hero->m_YiDongSheJiAnXia )//是否是移动射击状态
		{
			m_Hero->m_Hero_State.state = _HERO_MOVE_SHOOTING_MOVE;
			m_Hero->Heroyidong(_MOVE_LEFT);
			if(movefalg)
			{
				m_Hero->m_bShooting_move[1] = true;
				movefalg = false;
			}
			_C_S_GP8 cs8;//移动射击状态下移动
			cs8.bShooting_move1 = m_Hero->m_bShooting_move[1];
			cs8.image = m_Hero->GetPicDir();
			cs8.state = m_Hero->m_Hero_State.state;
			cs8.x = m_Hero->GetX();
			cs8.y = m_Hero->GetY();
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs8,sizeof(cs8));
		}
		else if(m_Hero->m_WXSX_anxia)
		{
			m_Hero->m_WXSX_X -= 15;
		}

		else
		{
			if(falg)
			{
				m_Hero->m_bMove = true;
				falg = false;
			}
			m_Hero->m_Hero_State.state = _HERO_MOVE_;
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

	}
	//右
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_RIGHT) == _KS_DH && !m_Hero->m_iJiangZhiZT && !m_Hero->m_bDeath  && !m_Hero->m_PuTongGongJi.state  && GetForegroundWindow() == hwnd)
	{
		if(m_Hero->m_YiDongSheJiAnXia)//是否是移动射击状态
		{
			m_Hero->m_Hero_State.state = _HERO_MOVE_SHOOTING_MOVE;
			m_Hero->Heroyidong(_MOVE_RIGHT);
			if(movefalg)
			{
				m_Hero->m_bShooting_move[1] = true;
				movefalg = false;
			}
			_C_S_GP8 cs8;//移动射击状态下移动
			cs8.bShooting_move1 = m_Hero->m_bShooting_move[1];
			cs8.image = m_Hero->GetPicDir();
			cs8.state = m_Hero->m_Hero_State.state;
			cs8.x = m_Hero->GetX();
			cs8.y = m_Hero->GetY();
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs8,sizeof(cs8));
		}
		else if(m_Hero->m_WXSX_anxia)
		{
			m_Hero->m_WXSX_X += 15;
		}

		else
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
		if(m_Hero->m_YiDongSheJiAnXia)//是否是移动射击状态
		{
			m_Hero->m_Hero_State.state = _HERO_MOVE_SHOOT;

			_C_S_GP8 cs8;//移动射击状态下移动
			cs8.bShooting_move1 = m_Hero->m_bShooting_move[1];
			cs8.image = m_Hero->GetPicDir();
			cs8.state = m_Hero->m_Hero_State.state;
			cs8.x = m_Hero->GetX();
			cs8.y = m_Hero->GetY();
			CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs8,sizeof(cs8));
		}
		else if(m_Hero->m_WXSX_anxia)
			m_Hero->m_Hero_State.state = _HERO_WEI_XING_SHE_XIAN;
		else
			m_Hero->m_Hero_State.state = _HERO_STAND;

		if(m_Hero->m_Inputflag)
		{
			falg = true;
			movefalg = true;
			m_Hero->m_bStand = true;
			m_Hero->m_Inputflag = false;
			
			if(!m_Hero->m_YiDongSheJiAnXia)//移动射击没按下
			{
				//发送待机状态消息
				_C_S_GP17 cs17;
				cs17.bStand = m_Hero->m_bStand;
				cs17.image = m_Hero->m_bPicdir;
				cs17.state = _HERO_STAND;
				CGameEngine::GetGE()->GetTcpClient()->SendData((char*)&cs17,sizeof(cs17));
			}

		}

	}
	if (CGameEngine::GetGE()->GetGI()->Get(_GI_K_X) == _KS_UC && m_Hero->m_PuTongGongJi.state)
	{
		m_Hero->m_Hero_State.state = _HERO_STAND;
		m_Hero->m_bStand = true;
		m_Hero->m_PuTongGongJi.state = false;
	}




}

void CJueDou::OnUIMsg(const char* id, int UIMSG)
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
	}
}
void CJueDou::OnNetEvent(_TC_NET_EVENT* ptne)
{
	switch(ptne->type)
	{
	case _TCNE_S_DISCONNECT:
	case _TCNE_C_DISCONNECT:
	case _TCNE_E_DISCONNECT:
		{
			m_player.clear();
			
		}break;
	case _TCNE_DATA:
		{
			_GAME_PROTOCOL* pgp = (_GAME_PROTOCOL*)ptne->data;
			switch (pgp->id)
			{
			case _NICK_NAME_LEGALITY:
				{
					_S_C_GP3* p = (_S_C_GP3*)ptne->data;
					CGameEngine::GetGE()->SetCurScene("游戏场景");

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

					if(m_player.size() <= 1)
					{
						m_WXSX.clear();
						m_vHram_String.clear();
						m_Vbullet.clear();
						m_player.clear();
						g_fanhui = true;
						Beep(500, 1000);
						CGameEngine::GetGE()->SetCurScene("城镇场景");
					}

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
			case _PLAYER_YI_DONG_SHE_JI://转发进入移动射击状态
				{
					_S_C_GP11* sc11 = (_S_C_GP11*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc11->pyr.name);
					
					if(it == m_player.end())
						return;

					
					it->second->m_bPicdir = sc11->pyr.image;
					it->second->m_bShooting_move[0] = sc11->pyr.bShooting_move;
					it->second->m_Hero_State.state = sc11->pyr.state;
					it->second->m_YiDongSheJiAnXia = true;

				}break;
			case _ZF_YI_DONG_SHE_JI_DIR://移动射击变化方向
				{
					_S_C_GP100* sc100 = (_S_C_GP100*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc100->name);
					
					if(it == m_player.end())
						return;

					
					it->second->m_bPicdir = sc100->dir;

				}break;
			case _ZF_YDSJ_MOVE://移动射击状态下移动
				{
					_S_C_GP13* sc13 = (_S_C_GP13*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc13->pyr.name);
					
					if(it == m_player.end())
						return;

					
					it->second->m_bShooting_move[1] = sc13->pyr.bShooting_move1;
					it->second->m_bPicdir = sc13->pyr.image;
					it->second->m_Hero_State.state = sc13->pyr.state;
					it->second->m_iX = sc13->pyr.x;
					it->second->m_iY = sc13->pyr.y;

				}break;
			case _ZF_YDSJ_GONG_JI://转发移动射击攻击
				{
					_S_C_GP32* sc32 = (_S_C_GP32*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc32->pyr.name);

					if(it == m_player.end())
						return;

					it->second->m_bShooting_move[2] = sc32->pyr.bShooting_move2;
					it->second->m_bPicdir = sc32->pyr.image;
					it->second->m_Hero_State.state = sc32->pyr.state;

				}break;
			case _ZF_YDSJ_DA_ZHONG_REN://转发技能打中人
				{
					_S_C_GP20* sc20 = (_S_C_GP20*)ptne->data;
					std::map<std::string,CHero*>::iterator it = m_player.find(sc20->pyr.name);
					
					if(it == m_player.end())
						return;

					
					it->second->m_bPicdir = sc20->pyr.image;
					it->second->m_Hero_State.state = sc20->pyr.state;
					it->second->m_iJiangZhiDu = sc20->pyr.JiangZhiDU;
					it->second->m_iJiangZhiZT = sc20->pyr.JiangZhiZT;
					it->second->SetHP(it->second->GetHP() - sc20->pyr.hram);

					CHram_String* hstr = new CHram_String(sc20->pyr.x,sc20->pyr.y,sc20->pyr.hram);
					m_vHram_String.push_back(hstr);
			
				}break;
			case _ZF_GAME_END://比赛结束
				{
					m_WXSX.clear();
					m_vHram_String.clear();
					m_Vbullet.clear();
					m_player.clear();
					g_fanhui = true;
					Beep(500, 1000);
					CGameEngine::GetGE()->SetCurScene("城镇场景");
				}break;
		
			}
	
		}break;

	}


}