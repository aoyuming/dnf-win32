#include "Skill.h"
CSkill::CSkill(int x,int y,int begpic,int endpic,int harm,int image,int h,int w)
	:
	m_iX(x),
	m_iY(y),
	m_iPicBegin(begpic),
	m_iPicEnd(endpic),
	m_bImage(image),
	m_iPicH(h),
	m_iPicW(w)
{
	//…Ë÷√æÿ–Œ∑∂Œß
	m_Rect.top = m_iY;
	m_Rect.bottom = m_iY + m_iPicH;
	m_Rect.left = m_iX;
	m_Rect.right = m_iX + m_iPicW;
	srand((unsigned int)time(0));
	m_iHarm = 500 + harm;

}

RECT CSkill::GetRect()
{
	return m_Rect;
}
void CSkill::Run()
{}