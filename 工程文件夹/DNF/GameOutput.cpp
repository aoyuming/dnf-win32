#include "GameOutput.h"
#include <math.h>

CMyXFORM::CMyXFORM()
{
	Identity();
}
void CMyXFORM::Identity()
{
	eM11 = eM22 = 1.0f;
	eM12 = eM21 = eDx = eDy = 0.0f;
}
void CMyXFORM::Translation(float x, float y)
{
	eM11 = eM22 = 1.0f;
	eM12 = eM21 = 0.0f;
	eDx = x;
	eDy = y;
}
void CMyXFORM::Scaling(float x, float y)
{
	eM11 = x;
	eM22 = y;
	eM12 = eM21 = eDx = eDy = 0.0f;
}
void CMyXFORM::Rotation(float a)
{
	eM11 = eM22 = cos(a);
	eM21 = (-(eM12 = sin(a)));
	eDx = eDy = 0.0f;
}
CMyXFORM* CMyXFORM::_MM(const CMyXFORM* m1,
						const CMyXFORM* m2,
						CMyXFORM* m3)
{
	m3->eM11 = m1->eM11 * m2->eM11 + m1->eM12 * m2->eM21;
	m3->eM12 = m1->eM11 * m2->eM12 + m1->eM12 * m2->eM22;
	m3->eM21 = m1->eM21 * m2->eM11 + m1->eM22 * m2->eM21;
	m3->eM22 = m1->eM21 * m2->eM12 + m1->eM22 * m2->eM22;
	m3->eDx = m1->eDx * m2->eM11 + m1->eDy * m2->eM21 + m2->eDx;
	m3->eDy = m1->eDx * m2->eM12 + m1->eDy * m2->eM22 + m2->eDy;
	return m3;
}

CGameOutput::~CGameOutput()
{}
void CGameOutput::Begin()
{}
void CGameOutput::End()
{}
void CGameOutput::SetTransform(CMyXFORM* pMyXFORM)
{}
void CGameOutput::EndTransform()
{}
unsigned char CGameOutput::GetDark()
{return 0;}
void CGameOutput::SetDark(unsigned char d)
{}
void CGameOutput::DrawLine(int x1,
							int y1,
							int x2,
							int y2,
							unsigned int c,
							int w)
{}
void CGameOutput::DrawRectangle1(int x1,
								  int y1,
								  int x2,
								  int y2,
								  unsigned int c)
{}
void CGameOutput::DrawRectangle2(int x1,
								  int y1,
								  int x2,
								  int y2,
								  unsigned int pc,
								  unsigned int bc)
{}
bool CGameOutput::LoadPic(const char* fn)
{return false;}
bool CGameOutput::ReleasePic(const char* fn)
{return false;}
int CGameOutput::LoadDirectory(const char* dn)
{return -1;}
bool CGameOutput::DrawPic1(const char* fn,
							   int dx,
							   int dy,
							   int dw,
							   int dh,
							   int sx,
							   int sy,
							   char m,
							   unsigned char a)
{return false;}
bool CGameOutput::DrawPic2(const char* fn,
							   int dx,
							   int dy,
							   int dw,
							   int dh,
							   int sx,
							   int sy,
							   char m,
							   unsigned char a)
{return false;}
bool CGameOutput::LoadFont(const char* id,
							   int w,
							   int h,
							   int we,
							   bool i,
							   bool u,
							   bool s,
							   const char* fn)
{return false;}
bool CGameOutput::ReleaseFont(const char* id)
{return false;}
bool CGameOutput::DrawString1(const char* id,
								  int x,
								  int y,
								  const char* t,
								  unsigned int c)
{return false;}
bool CGameOutput::DrawString2(const char* id,
								  int x,
								  int y,
								  int w,
								  int h,
								  const char* t,
								  unsigned int c)
{return false;}
bool CGameOutput::DrawString3(const char* id,
								  int x,
								  int y,
								  int w,
								  int h,
								  const char* t,
								  unsigned int c)
{return false;}

void CGameOutput::DrawEllipse1( //ª≠ø’–ƒÕ÷‘≤
	int x1,
	int y1,
	int x2,
	int y2,
	unsigned int c)
{}

void CGameOutput::DrawEllipse2( //ª≠ µ–ƒÕ÷‘≤
	int x1,
	int y1,
	int x2,
	int y2,
	unsigned int pc,
	unsigned int bc)
{}