#include "UI.h"

CUI::CUI(int x, int y, int w, int h, bool v, bool a)
:
m_X(x),
m_Y(y),
m_W(w),
m_H(h),
m_View(v),
m_Act(a),
m_Scene(0)
{}

CUI::~CUI()
{}

void CUI::SetRect(int x, int y, int w, int h)
{
	m_X = x;
	m_Y = y;
	m_W = w;
	m_H = h;
}

void CUI::GetRect(int* x, int* y, int* w, int* h)
{
	*x = m_X;
	*y = m_Y;
	*w = m_W;
	*h = m_H;
}

void CUI::SetView(bool View)
{
	m_View = View;
}

bool CUI::GetView()
{
	return m_View;
}

void CUI::SetAct(bool Act)
{
	m_Act = Act;
}

bool CUI::GetAct()
{
	return m_Act;
}

void CUI::Init(){}
void CUI::ActRender(){}
void CUI::UnactRender(){}
void CUI::OnMsg(int Msg, int p1, int p2){}
void CUI::End(){}