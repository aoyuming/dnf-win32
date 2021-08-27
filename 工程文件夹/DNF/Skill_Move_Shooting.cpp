#include "Skill_Move_Shooting.h"

bool CSkill_Move_Shooting::m_iCD = true;//CD
int  CSkill_Move_Shooting::m_iNum = 0;//�ӵ�����
int  CSkill_Move_Shooting::m_dangqianCDTimer = 0;//��ǰCDʱ��

CSkill_Move_Shooting::CSkill_Move_Shooting(int x,int y,int begpic,int endpic,int harm,int image,int h,int w)
	:
	CSkill(x,y,begpic,endpic,harm,image,h,w)
{
	 m_tIntervalTimeBegin = 0;
	 m_tIntervalTimeEnd = 3;
}
void  CSkill_Move_Shooting::Run()
{

	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin == m_tIntervalTimeEnd)
	{
		if(m_iPicBegin != m_iPicEnd)
			m_iPicBegin += 1;
		else
			m_iPicBegin = 0;
		m_tIntervalTimeBegin = 0;
	}

	//��ͼ
	char buf[20];
	m_FileName = "pic\\���ڹ���Ч��\\��\\";
	itoa(m_iPicBegin,buf,10);
	m_FileName += buf;
	m_FileName += ".png";
	std::map<std::string,File_W_H>::iterator i = g_FileName.find(m_FileName);
	m_iPicW = i->second.w;
	m_iPicH = i->second.h;

	//���þ��η�Χ
	m_Rect.top = m_iY;
	m_Rect.bottom = m_iY + m_iPicH;
	m_Rect.left = m_iX;
	m_Rect.right = m_iX + m_iPicW;

	//��ͼ
	CGameEngine::GetGE()->GetGO()->DrawPic2(m_FileName.data(),g_mapX + m_iX,m_iY,m_iPicW,m_iPicH,0,0,m_bImage);
}
//��ȡ������ײ��Χ
RECT CSkill_Move_Shooting::GetRect()
{
	return m_Rect;
}