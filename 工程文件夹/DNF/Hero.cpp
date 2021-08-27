#include "hero.h"


int CHero::m_HeroNum = 0;
int g_mapX, g_mapY;

CHero::CHero(int x,int y,int hp,int mp,char* name,int dir,int w,int h)//,int scene)
:
m_Hcan(x + w - 20,y + 15,0,0,dir),
m_iHp(hp),
m_iMp(mp),
m_iX(x),
m_iY(y),
m_bPicdir(dir),
m_iPicW(w),
m_iPicH(h),
m_FileName("��ǹ��"),
m_tIntervalTimeEnd(3),
m_iMoveDir(_MOVE_DOWN),
m_YiDongSheJiAnXia(false),
m_Inputflag(true),
m_bStand(false),
m_bMove(false),
m_iPicbegin(0),
m_iPicEnd(22),
m_WXSX_anxia(false),
m_iWXSXpicBegin(0),
m_iWXSXpicEnd(10),
m_WXSX_luoxia(false),
m_WXSX_X(x),
m_WXSX_Y(y),
m_bRush(false),
m_iJiangZhiZT(false),
m_iJiangZhiDuBegin(0),
m_bDeath(false),
m_iNum(0),
m_bWXSXCD(true),
m_dangqianCDTimer(0),
//m_iScene(scene)
m_bZhunBei(true),
m_bFangZhu(false),
_WORLD_W_(890),
_WORLD_H_(556)
{
	int strlen = 0;
	while(name[strlen])
		strlen += 1;
	for(int i = 0; i < strlen + 1; i++)
		m_cName[i] = name[i];

	m_tIntervalTimeBegin = 0;
	m_tIntervalTimeEnd = 5;

	//���þ��η�Χ
	m_rRect.top = m_iY;
	m_rRect.bottom = m_iY + m_iPicH;
	m_rRect.left = m_iX;
	m_rRect.right = m_iX + m_iPicW;

	m_Hero_State.state = _HERO_STAND;
	m_bShooting_move[0] = true;
	m_bShooting_move[1] = true;
	m_bShooting_move[2] = true;
	CSkill_Move_Shooting::m_iCD = true;
	m_HeroNum += 1;
	m_PuTongGongJi.SetPic = true;
	m_PuTongGongJi.state = false;

	m_iScene = -1;

	//m_bRushUP = m_bRushDOWN = m_bRushLETE = m_bRushRIGTH = false;


}
CHero::~CHero()
{
	m_HeroNum -= 1;
}



//��ͨ����
void CHero::PuTongGongJi()
{	
	static bool a = true;
	if(a)
	{
		m_iPicbegin = 25;
		m_iPicEnd = 27;
		m_Hcan.m_iState = _HCAN_MOVE_SHOOT;
	}

	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		if(m_iPicbegin != m_iPicEnd)
			m_iPicbegin += 1;
		else
		{
			a = true;
		}

	}
	//����λ��
	m_Hcan.m_iX = (m_bPicdir == _MIRROR_LR) ? m_iX - m_iPicW + 50 : m_iX + m_iPicW - 25;
	m_Hcan.m_iY = m_iY + 5;


	//����
	DrawMotion(m_iPicbegin);

}
//����
void CHero::death()
{
	m_iPicbegin = 127; 
	//����
	DrawMotion(m_iPicbegin);
}
//��ֱ״̬
void CHero::JiangZhiZhuangTai()
{
	m_iJiangZhiDuBegin += 1;
	m_iPicbegin = 124;
	if(m_YiDongSheJiAnXia)
		CSkill_Move_Shooting::m_iCD = false;

	if(m_iJiangZhiDuBegin >= m_iJiangZhiDu)
	{
		m_iJiangZhiDuBegin = 0;
		m_iJiangZhiZT = false;
		m_Hero_State.state = _HERO_STAND;
		m_bStand = true;
	}
	//����
	DrawMotion(m_iPicbegin);
	
}
//Ӣ�۱���
//void CHero::Rush()
//{
//	static bool b = true;
//	if(b)
//	{
//		m_iPicbegin = 103;
//		m_iPicEnd = 110;
//		b = false;
//	}
//	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
//	{
//		m_tIntervalTimeBegin = 0;
//		if(m_iPicbegin != m_iPicEnd)
//			m_iPicbegin += 1;
//	}
//	DrawMotion(m_iPicbegin);
//
//}
//��������
void CHero::WeiXingSheXian()
{
	static bool falg = true;
	static int i = 0;
	if(falg)
	{
		m_iPicbegin = 193;
		m_iPicEnd = 197;
		m_iWXSXpicBegin = 0;
		m_iWXSXpicEnd = 10;
		m_WXSX_X = m_iX - 100;
		m_WXSX_Y = m_iY - 100;
		falg = false;
	}
	if(i > 8)
	{
		falg = true;
		i = 0;
		m_Hero_State.state = _HERO_STAND;
		m_bStand = true;
		m_WXSX_anxia = false;
		m_WXSX_luoxia = true;
	}
	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		if(m_iPicbegin != m_iPicEnd)
			m_iPicbegin += 1;
		if(m_iWXSXpicBegin != m_iWXSXpicEnd)
			m_iWXSXpicBegin += 1;
		else 
			m_iWXSXpicBegin = 8;
		i += 1;
	}
	//����
	DrawMotion(m_iPicbegin);

	//��ͼ
	char buf[20];
	std::string s  = "pic\\��ǹ������ͼ\\�ͷ�\\";
	itoa(m_iWXSXpicBegin,buf,10);
	s += buf;
	s += "(��ȥ��).png";
	std::map<std::string,File_W_H>::iterator j = g_FileName.find(s);
	int w = j->second.w;
	int h = j->second.h;
	int x = (m_bPicdir == _MIRROR_LR)   ? m_iX - 50 : m_iX - 10;
	int y = (m_bPicdir == _MIRROR_NONE) ? m_iY - 20 : m_iY - 20;

	//��������
	CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),g_mapX + x,y,w,h,0,0,m_bPicdir);

	//��ͼ
	s  = "pic\\��ǹ������ͼ\\����\\";
	itoa(2,buf,10);
	s += buf;
	s += "(��ȥ��).png";
	j = g_FileName.find(s);
	w = j->second.w;
	h = j->second.h;

	//���ƾ���
	CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),g_mapX + m_WXSX_X,m_WXSX_Y,w,h,0,0,false);
	CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),g_mapX + m_WXSX_X + w,m_WXSX_Y,w,h,0,0,true);


}
//��Ѫ������
void CHero::TieHP_MP(int x,int y)
{
	int index[] = {36,35,3,2};
	//��������
	CGameEngine::GetGE()->GetGO()->DrawString1("������������", x, y - 30, m_cName, RGB(255, 255, 255));

	for(int i = 0;i < 4;i++)
	{
		//��ͼ
		int X = x ,Y = y;
		char buf[20];
		std::string s = "pic\\������Ѫ��\\";
		itoa(index[i],buf,10);
		s += buf;
		s += ".png";
		std::map<std::string,File_W_H>::iterator j = g_FileName.find(s);
		double w  = j->second.w;
		double h  = j->second.h;

		switch(i)
		{
		case 0:
			{
				Y = y;			
				//Ѫ������
				CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),X,Y,w,h,0,0);
			}break;
		case 1:
			{
				Y = y + h;		
				//Ѫ������
				CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),X,Y,w,h,0,0);
			}break;
		case 2:
			{
				Y = y - 9;	
				double picw = m_iHp / 400000;
				//w *= picw;
				//Ѫ������
				CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),X,Y,w * picw,h,0,0);
			}break;
		case 3:
			{
				Y = y + h - 15;	
				double picw = m_iMp / 400000;
				//w *= picw;
				//Ѫ������
				CGameEngine::GetGE()->GetGO()->DrawPic2(s.data(),X,Y,w * picw,h,0,0);
			}break;
		}
		
	
	}

}
//���ƶ���
void CHero::DrawMotion(int index)
{

	//��ͼ
	char buf[20];
	m_FileName = "pic\\ǹ��\\";
	itoa(index,buf,10);
	m_FileName += buf;
	m_FileName += ".png";
	std::map<std::string,File_W_H>::iterator i = g_FileName.find(m_FileName);
	m_iPicW = i->second.w;
	m_iPicH = i->second.h;

	//��������
	CGameEngine::GetGE()->GetGO()->DrawPic2(m_FileName.data(),g_mapX + m_iX,m_iY,m_iPicW,m_iPicH,0,0,m_bPicdir);
}
//Ӣ���ƶ�
void CHero::Heroyidong(int dir)
{
	int Xsudu = 5;
	int Ysudu = 5;
	if(m_YiDongSheJiAnXia)//�ƶ����״̬��
	{
		Ysudu = 7;
		Xsudu = 7;
	}
	if(m_bRush)
	{
		Ysudu = 7;
		Xsudu = 12;
	}
	//����
	switch(dir)
	{
	case _MOVE_UP:
		{
			//if(!m_WXSX_anxia && m_iY - Ysudu >= 200)
				m_iY -= Ysudu;

		}break;
	case _MOVE_DOWN:
		{
			//if(!m_WXSX_anxia && m_iY + Ysudu <= 540 - 160 )
				m_iY += Ysudu;

		}break;
	case _MOVE_LEFT:
		{
			//if(g_mapX >= 0 || m_iX > _CLIENT_W_ / 2 && !m_WXSX_anxia)//����
			//{
			//	m_iX -= Xsudu;
			//}
			//else if(!m_WXSX_anxia)
			//{
			if(g_mapX + Xsudu < 0 && m_iX + g_mapX < g_mapX + _CLIENT_W_ + 100 - 100 &&  !m_WXSX_anxia)
			{
				m_iX -= Xsudu;
				g_mapX += Xsudu;
			}
			else  if(!m_WXSX_anxia)
				m_iX -= Xsudu;
			//}
			if(!m_YiDongSheJiAnXia && !m_WXSX_anxia)
				m_bPicdir = _MIRROR_LR;


		}break;
	case _MOVE_RIGHT:
		{
			//if(g_mapX <= -(_WORLD_W_ - _CLIENT_W_) || m_iX < _CLIENT_W_ / 2 && !m_WXSX_anxia)//����
			//{
			//
			//	m_iX += Xsudu;
			//}
			//else if(!m_WXSX_anxia)
			//{
			if(g_mapX - Xsudu > -_WORLD_W_ + _CLIENT_W_ && m_iX +  g_mapX >= g_mapX + _CLIENT_W_ / 2 - 100 && !m_WXSX_anxia)
			{
				m_iX += Xsudu;
				g_mapX -= Xsudu;
			}
			else if(!m_WXSX_anxia)
				m_iX += Xsudu;
			//}
			if(!m_YiDongSheJiAnXia && !m_WXSX_anxia)
				m_bPicdir = _MIRROR_NONE;

		}break;
	}


}
//����վ��״̬
void CHero::Standby()
{
	if(m_bStand)
	{
		m_iPicbegin = 0;
		m_iPicEnd = 22;
		m_bStand = false;
	}
	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		if(m_iPicbegin != m_iPicEnd)
			m_iPicbegin += 1;
		else
			m_iPicbegin = 0;
	}
	//��������λ��
	m_Hcan.m_iState = _HCAN_STAND;
	m_Hcan.m_iY = (m_bPicdir == _MIRROR_NONE) ? m_iY + m_iPicW / 2 + 5 : m_iY + m_iPicW / 2 + 5;
	m_Hcan.m_iX = (m_bPicdir == _MIRROR_NONE) ? m_iX + m_iPicW / 2 : m_iX - m_iPicW / 2;
	//����
	DrawMotion(m_iPicbegin);
	m_Hcan.DrawMotion(m_iPicbegin);
}
//�����ƶ����(����)״̬
void CHero::Move_Shooting()
{
	
	if(m_bShooting_move[0])
	{
		m_iPicbegin = 76;
		m_iPicEnd = 76;
		m_bShooting_move[0] = false;
	}
	m_Hcan.m_iState = _HCAN_MOVE_SHOOT;
	m_Inputflag = true;
	//����
	DrawMotion(m_iPicbegin);

	//����λ��
	m_Hcan.m_iX = (m_bPicdir == _MIRROR_LR) ? m_iX - m_iPicW + 35 : m_iX + m_iPicW - 25;
	m_Hcan.m_iY = m_iY + 5;

}

//�ƶ����(����)״̬���ƶ�
void CHero::Move_Shooting_State(int dir)
{
	m_tIntervalTimeBegin += 1;
	if(m_bShooting_move[1])
	{
		m_iPicbegin = 76;
		m_iPicEnd = 102;
		m_bShooting_move[1] = false;
	}
	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
	{
		if(m_iPicbegin != m_iPicEnd)
			m_iPicbegin += 1;
		else
			m_iPicbegin = 76;
		m_tIntervalTimeBegin = 0;
	}
	//����
	DrawMotion(m_iPicbegin);

	//����״̬
	m_Hcan.m_iState = _HCAN_MOVE_SHOOT;

	//����λ��
	m_Hcan.m_iX = (m_bPicdir == _MIRROR_LR) ? m_iX - m_iPicW + 35 : m_iX + m_iPicW - 25;
	m_Hcan.m_iY = m_iY + 5;


}
//�ƶ��������
void CHero::Move_Shooting_Dir()
{
	if(!m_YiDongSheJiAnXia  && !m_YiDongSheJiAnXia && !m_PuTongGongJi.state && !m_iJiangZhiZT)
		return;

	m_bPicdir = ((m_bPicdir == _MIRROR_LR) ? _MIRROR_NONE : _MIRROR_LR);


}

//�ƶ��������
void CHero::MoveShootingAttack()
{

	if(m_bShooting_move[2])
	{	
		m_iPicbegin = 76;
		m_iPicEnd = 78;
		m_bShooting_move[2] = false;
	}
	
	CSkill_Move_Shooting* ms = new CSkill_Move_Shooting(m_iX + m_iPicW + 40,m_iY,0,4,2000,m_bPicdir,90,190);
	ms->m_bImage = m_bPicdir;
	ms->m_iX = (m_bPicdir == _MIRROR_NONE) ? m_iX + m_iPicW + 40: m_iX - 120 - m_iPicW;
	ms->m_iY = m_iY - 10;
	m_Move_Shooting.push_back(ms);


	//����λ��
	m_Hcan.m_iX = (m_bPicdir == _MIRROR_LR) ? m_iX - m_iPicW + 35 : m_iX + m_iPicW - 25;
	m_Hcan.m_iY = m_iY + 5;
	//����
	DrawMotion(m_iPicbegin);

}
//ֹͣ�ƶ����
void CHero::End_MoveShootingk()
{

	m_Hero_State.state = _HERO_STAND;	
	//����
	m_bStand = true;
	m_YiDongSheJiAnXia = false;
	m_Hcan.m_iState = -1;

}
//�����ƶ�
void CHero::Move(int dir)
{
	if(m_bMove)
	{
		m_iPicbegin = 55;
		m_iPicEnd = 62;
		m_bMove = false;
	}
	m_Inputflag = true;

	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin == m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		if(m_iPicbegin != m_iPicEnd)
			m_iPicbegin += 1;
		else
			m_iPicbegin = 55;
	}
	//����
	DrawMotion(m_iPicbegin);

}

//����
void CHero::Run()
{
	if(m_iHp <= 0 && !m_bDeath)
	{
		m_bDeath = true;
		m_Hero_State.state = _HERO_DEATH;
		m_YiDongSheJiAnXia = false;
		m_bStand = false;
		m_bMove = false;
		m_WXSX_anxia = false;
		m_WXSX_luoxia = false;
		m_iJiangZhiZT = false;
		m_bShooting_move[0] = m_bShooting_move[1] = m_bShooting_move[2] = false;
		m_iY += m_iPicW;

	}

	
	//����Ӣ��״̬���Ʋ�ͬ��������
	switch(m_Hero_State.state)
	{
	case _HERO_STAND:Standby();										break;//վ��
	case _HERO_MOVE_:Move(m_iMoveDir);								break;//�ƶ�
	case _HERO_MOVE_SHOOTING_MOVE:Move_Shooting_State(m_iMoveDir);  break;//�ƶ������״̬���ƶ�
	case _HERO_MOVE_SHOOT:Move_Shooting();							break;//�����ƶ����(����)״̬
	case _HERO_MOVE_ATTACK:MoveShootingAttack();					break;//�ƶ����(����)����
	case _HERO_WEI_XING_SHE_XIAN:WeiXingSheXian();					break;//��������
	case _HERO_RUSH:/*Rush();	*/									break;//����״̬
	case _HERO_JIANG_ZHI:JiangZhiZhuangTai();						break;//��ֱ״̬
	case _HERO_DEATH:death();										break;//����״̬
	case _HERO_PUTONGGONGJI:PuTongGongJi();							break;//��ͨ����
	}

	//���������ӵ���
	for(std::vector<CSkill_Move_Shooting*>::iterator it = m_Move_Shooting.begin(); it != m_Move_Shooting.end();)
	{
		//������ͼ����
		if((*it)->m_iPicBegin == (*it)->m_iPicEnd)
		{
			delete *it;
			it = m_Move_Shooting.erase(it);
		}
		else
		{
			(*it)->Run();
			it++;
		}
		//��෢���ӵ�����
		if(CSkill_Move_Shooting::m_iNum == 20)
		{
			CSkill_Move_Shooting::m_iNum = 0;
			CSkill_Move_Shooting::m_iCD = false;
			//ֹͣ�ƶ����
			End_MoveShootingk();
			m_Hero_State.flag = true;
			m_Move_Shooting.clear();
			break;
		}
	}

	//�ƶ����CDʱ�����
	if(!CSkill_Move_Shooting::m_iCD)
		CSkill_Move_Shooting::m_dangqianCDTimer += 1;

	if(CSkill_Move_Shooting::m_dangqianCDTimer == 600)
	{
		CSkill_Move_Shooting::m_dangqianCDTimer = 0;
		CSkill_Move_Shooting::m_iCD = true;
	}

	//��������CDʱ�����
	if(!m_bWXSXCD)
		m_dangqianCDTimer += 1;

	if(m_dangqianCDTimer >= 999)
	{
		m_dangqianCDTimer = 0;
		m_bWXSXCD = true;
	}
	
	//���þ��η�Χ
	m_rRect.top = m_iY;
	m_rRect.bottom = m_iY + m_iPicH;
	m_rRect.left = m_iX;
	m_rRect.right = m_iX + m_iPicW;

	//��������
	CGameEngine::GetGE()->GetGO()->DrawString1("ѡ�񷿼�����",g_mapX +  m_iX, m_iY - 30, m_cName, RGB(0, 255, 0));
	
	//����λ��
	m_Hcan.m_bImage = m_bPicdir;

	//����
	m_Hcan.Run();




}
//��ȡ�ƶ����������Χ
//RECT CHero::GetMoveShootingAttackRect()
//{
//	return m_Move_Shooting.GetRect();
//}
//��ȡѪ��
double CHero::GetHP()
{
	return m_iHp;
}
//����Ѫ��
void CHero::SetHP(double hp)
{
	m_iHp = hp;
}
//��ȡ����
double CHero::GetMP()
{
	return m_iMp;
}
//��������
void CHero::SetMP(double mp)
{
	m_iMp = mp;
}
//��ȡ����X
int CHero::GetX()
{
	return m_iX;
}
//��������X
void CHero::SetX(int x)
{
	m_iX = x;
}
//��ȡ����Y
int CHero::GetY()
{
	return m_iY;
}
//��������y
void CHero::SetY(int y)
{
	m_iY = y;
}

//��ȡ����
char* CHero::GetName()
{
	return m_cName;
}
//��������
void CHero::SetName(char* name)
{
	int strlen = 0;
	while(name[strlen])
		strlen += 1;
	for(int i = 0; i < strlen + 1; i++)
		m_cName[i] = name[i];
}
//��ȡ��ͼ����
int CHero::GetPicDir()
{
	return m_bPicdir;
}
//������ͼ����
void CHero::SetPicDir(bool dir)
{
	m_bPicdir = dir;
}
//��ȡ��ͼ��
int CHero::GetPicW()
{
	return m_iPicW;
}
//������ͼ��
void CHero::SetPicW(int w)
{
	m_iPicW = w;
}
//��ȡ��ͼ��
int CHero::GetPicH()
{
	return m_iPicH;
}
//������ͼ��
void CHero::SetPicH(int h)
{
	m_iPicH = h;
}
//��ȡ��ͼ��ײ��Χ
RECT CHero::GetPicRect()
{
	return m_rRect;
}
