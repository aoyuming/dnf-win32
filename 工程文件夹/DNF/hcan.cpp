#include "hcan.h"
CHcan::CHcan(int x,int y,int begin,int end,int dir)
	:
	 m_iX(x),
	 m_iY(y),
	 m_iPicBegin(begin),
	 m_iPicEnd(end),
	 m_bImage(dir),
	 m_iState(_HCAN_STAND)
{
}
//´ý»ú×´Ì¬
void CHcan::daiji()
{
	static int PicBegin = 0;
	static int PicEnd = 22;

	m_tIntervalTimeBegin += 1;
	if(m_tIntervalTimeBegin == m_tIntervalTimeEnd)
	{
		m_tIntervalTimeBegin = 0;
		if(PicBegin != PicEnd)
			PicBegin += 1;
		else
			PicBegin = 0;
	}
	DrawMotion(PicBegin);
}
//»æÖÆ¶¯×÷
void CHcan::DrawMotion(int index)
{
	//ÌùÍ¼
	char buf[20];
	m_FileName = "pic\\ÊÖÅÚ\\ÏÂ\\";
	itoa(index,buf,10);
	m_FileName += buf;
	m_FileName += ".png";
	std::map<std::string,File_W_H>::iterator i = g_FileName.find(m_FileName);
	m_iPicW = i->second.w;
	m_iPicH = i->second.h;
	//ÌùÍ¼
	CGameEngine::GetGE()->GetGO()->DrawPic2(m_FileName.data(),g_mapX + m_iX,m_iY,m_iPicW,m_iPicH,0,0,m_bImage);
}

//ÒÆ¶¯Éä»÷×´Ì¬
void CHcan::Move_Shooting_State()
{
	static int PicBegin = 78;
	DrawMotion(PicBegin);
}
void CHcan::Run()
{
	switch(m_iState)
	{
	case _HCAN_STAND:daiji();						 break;
	case _HCAN_MOVE_SHOOT:Move_Shooting_State();	 break;
	}
}