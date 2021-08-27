#include "GameOutputGDI.h"
#include <math.h>
#include <io.h>

#pragma comment(lib, "msimg32.lib")

CGameOutputGDI::CGameOutputGDI(HWND hWnd)
:
m_hWnd(hWnd),
m_Dark(0)
{
	//得到客户区宽、高
	RECT r;
	GetClientRect(m_hWnd, &r);
	m_ClientW = r.right - r.left;
	m_ClientH = r.bottom - r.top;

	//得到主设备
	m_MainDC = GetDC(m_hWnd);

	HBITMAP hbmp = 0;

	//得到后备设备
	m_BackDC = CreateCompatibleDC(m_MainDC);
	hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
	DeleteObject(SelectObject(m_BackDC, hbmp));
	DeleteObject(hbmp);

	//设置后备设备字体背景透明
	SetBkMode(m_BackDC, TRANSPARENT);

	//得到镜像设备
	m_MirrorDC = CreateCompatibleDC(m_MainDC);
	hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
	DeleteObject(SelectObject(m_MirrorDC, hbmp));
	DeleteObject(hbmp);

	//得到亮度设备
	m_DarkDC = CreateCompatibleDC(m_MainDC);
	hbmp = CreateCompatibleBitmap(m_MainDC, m_ClientW, m_ClientH);
	DeleteObject(SelectObject(m_DarkDC, hbmp));
	DeleteObject(hbmp);
	BitBlt(m_DarkDC, 0, 0, m_ClientW, m_ClientH, 0, 0, 0, BLACKNESS);

	//默认设置为高级渲染模式
	SetGraphicsMode(m_BackDC, GM_ADVANCED);
}

CGameOutputGDI::~CGameOutputGDI()
{
	std::map<std::string, HFONT>::iterator i1;
	for (i1 = m_Font.begin(); i1 != m_Font.end(); ++i1)
		DeleteObject(i1->second);

	std::map<std::string, HDC>::iterator i2;
	for (i2 = m_Bmp.begin(); i2 != m_Bmp.end(); ++i2)
		DeleteDC(i2->second);

	DeleteDC(m_DarkDC);
	DeleteDC(m_MirrorDC);
	DeleteDC(m_BackDC);
	ReleaseDC(m_hWnd, m_MainDC);
}

void CGameOutputGDI::Begin()
{
	BitBlt(m_BackDC, 0, 0, m_ClientW, m_ClientH, 0, 0, 0, WHITENESS);
}

void CGameOutputGDI::End()
{
	if (m_Dark != 0)
	{
		BLENDFUNCTION bf = {AC_SRC_OVER, 0, m_Dark, 0};
		AlphaBlend(m_BackDC, 0, 0, m_ClientW, m_ClientH, m_DarkDC, 0, 0, m_ClientW, m_ClientH, bf);
	}
	BitBlt(m_MainDC, 0, 0, m_ClientW, m_ClientH, m_BackDC, 0, 0, SRCCOPY);
}

void CGameOutputGDI::SetTransform(CMyXFORM* pMyXFORM)
{
	SetWorldTransform(m_BackDC, pMyXFORM);
}

void CGameOutputGDI::EndTransform()
{
	CMyXFORM m;
	SetWorldTransform(m_BackDC, &m);
}

unsigned char CGameOutputGDI::GetDark()
{
	return m_Dark;
}

void CGameOutputGDI::SetDark(unsigned char d)
{
	m_Dark = d;
}

void CGameOutputGDI::DrawLine(int x1,
						   int y1,
						   int x2,
						   int y2,
						   unsigned int c,
						   int w)
{
	HPEN newpen = 0, oldpen = 0;
	if (RGB(0, 0, 0) != c || 1 != w)
	{
		newpen = CreatePen(PS_SOLID, w, c);
		oldpen = (HPEN)SelectObject(m_BackDC, newpen);
	}

	POINT p;
	MoveToEx(m_BackDC, x1, y1, &p);
	LineTo(m_BackDC, x2, y2);

	if (newpen)
	{
		SelectObject(m_BackDC, oldpen);
		DeleteObject(newpen);
	}
}

void CGameOutputGDI::DrawRectangle1(int x1,
								 int y1,
								 int x2,
								 int y2,
								 unsigned int c)
{
	HPEN newpen = 0, oldpen = 0;
	if (RGB(0, 0, 0) != c)
	{
		newpen = CreatePen(PS_SOLID, 1, c);
		oldpen = (HPEN)SelectObject(m_BackDC, newpen);
	}

	POINT p[]
	=
	{
		{x1, y1},
		{x2, y1},
		{x2, y2},
		{x1, y2},
		{x1, y1}
	};
	POINT q;
	for (int i = 0; i < 4; ++i)
	{
		MoveToEx(m_BackDC, p[i].x, p[i].y, &q);
		LineTo(m_BackDC, p[i + 1].x, p[i + 1].y);
	}

	if (newpen)
	{
		SelectObject(m_BackDC, oldpen);
		DeleteObject(newpen);
	}
}

void CGameOutputGDI::DrawRectangle2(int x1,
								 int y1,
								 int x2,
								 int y2,
								 unsigned int pc,
								 unsigned int bc)
{
	HPEN newpen = 0, oldpen = 0;
	HBRUSH newbrush = 0, oldbrush = 0;
	if (RGB(0, 0, 0) != pc)
	{
		newpen = CreatePen(PS_SOLID, 1, pc);
		oldpen = (HPEN)SelectObject(m_BackDC, newpen);
	}
	if (RGB(255, 255, 255) != bc)
	{
		newbrush = CreateSolidBrush(bc);
		oldbrush = (HBRUSH)SelectObject(m_BackDC, newbrush);
	}

	Rectangle(m_BackDC, x1, y1, x2, y2);

	if (newpen)
	{
		SelectObject(m_BackDC, oldpen);
		DeleteObject(newpen);
	}
	if (newbrush)
	{
		SelectObject(m_BackDC, oldbrush);
		DeleteObject(newbrush);
	}
}

void CGameOutputGDI::DrawEllipse1(int x1,
							   int y1,
							   int x2,
							   int y2,
							   unsigned int c)
{
	HPEN newpen = 0, oldpen = 0;
	if (RGB(0, 0, 0) != c)
	{
		newpen = CreatePen(PS_SOLID, 1, c);
		oldpen = (HPEN)SelectObject(m_BackDC, newpen);
	}
	HBRUSH newbrush = (HBRUSH)GetStockObject(NULL_BRUSH); //得到操作系统预先设置好的一些GDI对象
	HBRUSH oldbrush = (HBRUSH)SelectObject(m_BackDC, newbrush);

	Ellipse(m_BackDC, x1, y1, x2, y2);

	SelectObject(m_BackDC, oldbrush);
	if (newpen)
	{
		SelectObject(m_BackDC, oldpen);
		DeleteObject(newpen);
	}
}

void CGameOutputGDI::DrawEllipse2(int x1,
							   int y1,
							   int x2,
							   int y2,
							   unsigned int pc,
							   unsigned int bc)
{
	HPEN newpen = 0, oldpen = 0;
	HBRUSH newbrush = 0, oldbrush = 0;
	if (RGB(0, 0, 0) != pc)
	{
		newpen = CreatePen(PS_SOLID, 1, pc);
		oldpen = (HPEN)SelectObject(m_BackDC, newpen);
	}
	if (RGB(255, 255, 255) != bc)
	{
		newbrush = CreateSolidBrush(bc);
		oldbrush = (HBRUSH)SelectObject(m_BackDC, newbrush);
	}

	Ellipse(m_BackDC, x1, y1, x2, y2);

	if (newpen)
	{
		SelectObject(m_BackDC, oldpen);
		DeleteObject(newpen);
	}
	if (newbrush)
	{
		SelectObject(m_BackDC, oldbrush);
		DeleteObject(newbrush);
	}
}

bool CGameOutputGDI::LoadPic(const char* fn)
{
	std::string s = fn;
	if (m_Bmp.find(s) != m_Bmp.end())
		return false;

	HBITMAP hbmp = (HBITMAP)LoadImage(0, fn, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hbmp)
		return false;

	HDC dc = CreateCompatibleDC(m_MainDC);
	DeleteObject(SelectObject(dc, hbmp));
	DeleteObject(hbmp);

	m_Bmp.insert(std::pair<std::string, HDC>(s, dc));

	return true;
}

bool CGameOutputGDI::ReleasePic(const char* fn)
{
	std::string s = fn;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end())
		return false;

	DeleteDC(i->second);
	m_Bmp.erase(i);

	return true;
}

int CGameOutputGDI::LoadDirectory(const char* dn)
{
	int r = 0;

	std::string s1 = dn;
	s1 += "\\*.*";

	_finddata_t fd;
	intptr_t fr = _findfirst(s1.c_str(), &fd);
	if (-1 != fr)
	{
		do
		{
			//子目录
			if (fd.attrib & _A_SUBDIR)
			{
				if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0)
				{
					std::string s2 = dn;
					s2 += "\\";
					s2 += fd.name;
					r += LoadDirectory(s2.c_str());
				}
			}
			//文件
			else
			{
				//检查是否为位图文件
				char* p = strrchr(fd.name, '.');
				if (p)
				{
					//得到后缀名
					std::string s3 = p + 1;

					//将所有的字符归于小写
					for (unsigned int i = 0; i < s3.length(); ++i)
					{
						if (s3[i] >= 'A' && s3[i] <= 'Z')
							s3[i] += ('a' - 'A');
					}

					//确实为位图文件
					if (strcmp(s3.c_str(), "bmp") == 0)
					{
						std::string s4 = dn;
						s4 += "\\";
						s4 += fd.name;
						if (LoadPic(s4.c_str()))
							r += 1;
					}
				}
			}
		} while (0 == _findnext(fr, &fd));
		_findclose(fr);
	}

	return r;
}

bool CGameOutputGDI::DrawPic1(const char* fn,
						   int dx,
						   int dy,
						   int dw,
						   int dh,
						   int sx,
						   int sy,
						   char m,
						   unsigned char a)
{
	if (m < _MIRROR_NONE || m > _MIRROR_LRTB)
		return false;

	std::string s = fn;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end())
		return false;

	switch (m)
	{
	case _MIRROR_NONE: TransparentBlt(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, dw, dh, RGB(0, 255, 0)); break;
	case _MIRROR_LR:
		{
			StretchBlt(m_MirrorDC, dw - 1, 0, -dw, dh, i->second, sx, sy, dw, dh, SRCCOPY);
			TransparentBlt(m_BackDC, dx, dy, dw, dh, m_MirrorDC, 0, 0, dw, dh, RGB(0, 255, 0));
			break;
		}
	case _MIRROR_TB:
		{
			StretchBlt(m_MirrorDC, 0, dh - 1, dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY);
			TransparentBlt(m_BackDC, dx, dy, dw, dh, m_MirrorDC, 0, 0, dw, dh, RGB(0, 255, 0));
			break;
		}
	case _MIRROR_LRTB: 
		{
			StretchBlt(m_MirrorDC, dw - 1, dh - 1, -dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY);
			TransparentBlt(m_BackDC, dx, dy, dw, dh, m_MirrorDC, 0, 0, dw, dh, RGB(0, 255, 0));
			break;
		}
	}

	return true;
}

bool CGameOutputGDI::DrawPic2(const char* fn,
						   int dx,
						   int dy,
						   int dw,
						   int dh,
						   int sx,
						   int sy,
						   char m,
						   unsigned char a)
{
	if (m < _MIRROR_NONE || m > _MIRROR_LRTB)
		return false;

	std::string s = fn;
	std::map<std::string, HDC>::iterator i = m_Bmp.find(s);
	if (i == m_Bmp.end())
		return false;

	switch (m)
	{
	case _MIRROR_NONE: BitBlt(m_BackDC, dx, dy, dw, dh, i->second, sx, sy, SRCCOPY); break;
	case _MIRROR_LR: StretchBlt(m_BackDC, dx + dw - 1, dy, -dw, dh, i->second, sx, sy, dw, dh, SRCCOPY); break;
	case _MIRROR_TB: StretchBlt(m_BackDC, dx, dy + dh - 1, dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY); break;
	case _MIRROR_LRTB: StretchBlt(m_BackDC, dx + dw - 1, dy + dh - 1, -dw, -dh, i->second, sx, sy, dw, dh, SRCCOPY); break;
	}

	return true;
}

bool CGameOutputGDI::LoadFont(const char* id,
						   int w,
						   int h,
						   int we,
						   bool i,
						   bool u,
						   bool s,
						   const char* fn)
{
	std::string sid = id;
	if (m_Font.find(sid) != m_Font.end())
		return false;

	HFONT font = CreateFontA(
		h,
		w,
		0,
		0,
		we,
		i,
		u,
		s,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		fn);

	if (!font)
		return false;

	m_Font.insert(std::pair<std::string, HFONT>(sid, font));

	return true;
}

bool CGameOutputGDI::ReleaseFont(const char* id)
{
	std::string s = id;
	std::map<std::string, HFONT>::iterator i = m_Font.find(s);
	if (i == m_Font.end())
		return false;

	DeleteObject(i->second);
	m_Font.erase(i);

	return true;
}

bool CGameOutputGDI::DrawString1(const char* id,
							  int x,
							  int y,
							  const char* t,
							  unsigned int c)
{
	if (id)
	{
		std::string s = id;
		std::map<std::string, HFONT>::iterator i = m_Font.find(s);
		if (i == m_Font.end())
			return false;

		HFONT oldfont = (HFONT)SelectObject(m_BackDC, i->second);
		unsigned int pc = SetTextColor(m_BackDC, c);
		TextOutA(m_BackDC, x, y, t, (int)strlen(t));
		SetTextColor(m_BackDC, pc);
		SelectObject(m_BackDC, oldfont);
	}
	else
	{
		unsigned int pc = SetTextColor(m_BackDC, c);
		TextOutA(m_BackDC, x, y, t, (int)strlen(t));
		SetTextColor(m_BackDC, pc);
	}

	return true;
}

bool CGameOutputGDI::DrawString2(const char* id,
							  int x,
							  int y,
							  int w,
							  int h,
							  const char* t,
							  unsigned int c)
{
	if (id)
	{
		std::string s = id;
		std::map<std::string, HFONT>::iterator i = m_Font.find(s);
		if (i == m_Font.end())
			return false;

		HFONT oldfont = (HFONT)SelectObject(m_BackDC, i->second);
		unsigned int pc = SetTextColor(m_BackDC, c);
		RECT r = {x, y, x + w, y + h};
		DrawTextA(m_BackDC, t, (int)strlen(t), &r, DT_WORDBREAK);
		SetTextColor(m_BackDC, pc);
		SelectObject(m_BackDC, oldfont);
	}
	else
	{
		unsigned int pc = SetTextColor(m_BackDC, c);
		RECT r = {x, y, x + w, y + h};
		DrawTextA(m_BackDC, t, (int)strlen(t), &r, DT_WORDBREAK);
		SetTextColor(m_BackDC, pc);
	}

	return true;
}

bool CGameOutputGDI::DrawString3(const char* id,
							  int x,
							  int y,
							  int w,
							  int h,
							  const char* t,
							  unsigned int c)
{
	if (id)
	{
		std::string s = id;
		std::map<std::string, HFONT>::iterator i = m_Font.find(s);
		if (i == m_Font.end())
			return false;

		HFONT oldfont = (HFONT)SelectObject(m_BackDC, i->second);
		unsigned int pc = SetTextColor(m_BackDC, c);
		RECT r = {x, y, x + w, y + h};
		DrawTextA(m_BackDC, t, (int)strlen(t), &r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		SetTextColor(m_BackDC, pc);
		SelectObject(m_BackDC, oldfont);
	}
	else
	{
		unsigned int pc = SetTextColor(m_BackDC, c);
		RECT r = {x, y, x + w, y + h};
		DrawTextA(m_BackDC, t, (int)strlen(t), &r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		SetTextColor(m_BackDC, pc);
	}

	return true;
}