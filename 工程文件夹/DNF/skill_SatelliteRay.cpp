#include "skill_SatelliteRay.h"

CSkill_Satellite_Ray::CSkill_Satellite_Ray(int x,int y,int begpic,int endpic,int harm,int image,int h,int w)
	:
	CSkill(x,y,begpic,endpic,harm,image,h,w),
	m_iState(ECTPTOMA),
	m_delete(false)

{
	 m_tIntervalTimeBegin = 0;
	 m_tIntervalTimeEnd = 5;
	 m_iPicbeg = 0;
	 m_iPicend = 6;
	 m_iluoxiabeg = 0;
	 m_iluoxiaend = 2;
	 m_idixiabeg = 0;
	 m_idixiaend = 1;
	 m_iZKPicbeg = 4;
	 m_iZKPicend = 8;
	 m_zhenliang = 0;

}
//���ƶ���
void CSkill_Satellite_Ray::DrawMotion(int index,char* fm ,int x,int y,bool image)
{
	//��ͼ
	char buf[20];
	
	m_FileName = fm;
	itoa(index,buf,10);
	m_FileName += buf;
	m_FileName += "(��ȥ��).png";
	std::map<std::string,File_W_H>::iterator i = g_FileName.find(m_FileName);
	m_iPicW = i->second.w;
	m_iPicH = i->second.h;

	//��������
	CGameEngine::GetGE()->GetGO()->DrawPic1(m_FileName.data(),g_mapX + x,y,m_iPicW,m_iPicH,0,0,image);
}
//�ͷ�״̬
void CSkill_Satellite_Ray::ReleaseState()
{
	static int Picbeg = 0;
	static int Picend = 10;
	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		if(Picbeg != Picend)
			Picbeg += 1;
		else
		{
			m_iState = ECTPTOMA;
			return;
		}
	}
	DrawMotion(Picbeg,"pic\\��ǹ������ͼ\\�ͷ�\\",m_iX,m_iY,false);

}
//����״̬
void CSkill_Satellite_Ray::EctptomaState()
{
	static int Picbeg = 0;
	static int Picend = 4;
	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		if(Picbeg != Picend)
			Picbeg += 1;
		else
		{
			m_iState = RUN;
			return;
		}
	}

	DrawMotion(Picbeg,"pic\\��ǹ������ͼ\\����\\",m_iX - m_iPicW / 2,m_iY - m_iPicH / 2,false);
	DrawMotion(Picbeg,"pic\\��ǹ������ͼ\\����\\",m_iX + m_iPicW - m_iPicW / 2,m_iY - m_iPicH / 2,true);
}
//����״̬
void CSkill_Satellite_Ray::RunState()
{
	
	static bool baozhafalg = true;
	m_tIntervalTimeBegin += 1;

	if(m_tIntervalTimeBegin >= m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		m_zhenliang += 1;
		if(m_iPicbeg != m_iPicend)
			m_iPicbeg += 1;
		else
			m_iPicbeg = 0;
		if(m_iluoxiabeg != m_iluoxiaend)
			m_iluoxiabeg += 1;
		else
			m_iluoxiabeg = 0;
		if(m_idixiabeg != m_idixiaend)
			m_idixiabeg += 1;
		else
			m_idixiabeg = 0;

		if(m_iZKPicbeg != m_iZKPicend)
			m_iZKPicbeg += 1;
		else
			baozhafalg = false;
	}
	int x = m_iX;
	int y = m_iY;
	if(baozhafalg)
	{
		DrawMotion(m_iPicbeg,"pic\\��ǹ������ͼ\\����\\",m_iX - 151 / 2,m_iY - 255 / 2,false);
		DrawMotion(m_iPicbeg,"pic\\��ǹ������ͼ\\����\\",m_iX + 151 - 151 / 2,m_iY - 255 / 2,true);
	}
	

	DrawMotion(m_idixiabeg,"pic\\��ǹ������ͼ\\floor\\",m_iX - 10,m_iY + 40,false);
	for(int i = 0;i < 10;i++)
		DrawMotion(m_iPicbeg,"pic\\��ǹ������ͼ\\����\\",x,y -= m_iPicH,false);

	DrawMotion(m_iluoxiabeg,"pic\\��ǹ������ͼ\\����\\",m_iX - 10,m_iY,false);
	
	
	
	


}
void CSkill_Satellite_Ray::Run()
{


	//״̬
	switch(m_iState)
	{
	case RELEASE:  ReleaseState();		  break;
	case ECTPTOMA: EctptomaState();		  break;
	case RUN:	   RunState();			  break;
	}


	
	//���þ��η�Χ
	m_Rect.top = m_iY;// - m_iPicH * 10 ;
	m_Rect.bottom = m_iY + m_iPicH;
	m_Rect.left = m_iX;
	m_Rect.right = m_iX + m_iPicW;

	if(m_zhenliang >= 30)
		m_delete = true;

}
RECT CSkill_Satellite_Ray::GetRect()
{
	return m_Rect;
}