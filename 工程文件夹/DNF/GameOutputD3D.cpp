#include "GameOutputD3D.h"
#include <io.h>
#include "define_.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

CGameOutputD3D::CGameOutputD3D(HWND hWnd)
:
m_hWnd(hWnd),
m_D3D(0),
m_Device(0),
m_Sprite(0),
m_Line(0),
m_DeviceLost(false),
m_Dark(0),
m_DarkTexture(0),
m_DefaultFont(0)
{
	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

	RECT r;
	GetClientRect(m_hWnd, &r);

	m_DeviceP.BackBufferWidth = r.right - r.left; //后备缓冲区宽(像素)
	m_DeviceP.BackBufferHeight = r.bottom - r.top; //后备缓冲区高(像素)
	m_DeviceP.BackBufferFormat = D3DFMT_UNKNOWN; //后备缓冲区格式
	m_DeviceP.BackBufferCount = 1; //后备缓冲数量
	m_DeviceP.MultiSampleType = D3DMULTISAMPLE_NONE; //多重采样类型
	m_DeviceP.MultiSampleQuality = 0; //多重采样质量
	m_DeviceP.SwapEffect = D3DSWAPEFFECT_DISCARD; //交换链效果
	m_DeviceP.hDeviceWindow = m_hWnd; //窗口
	m_DeviceP.Windowed = TRUE; //是否窗口化
	m_DeviceP.EnableAutoDepthStencil = FALSE; //是否计划深度、模板缓冲
	m_DeviceP.AutoDepthStencilFormat = D3DFMT_UNKNOWN; //深度、模板缓冲格式
	m_DeviceP.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //标志(允许锁定后备缓冲)
	m_DeviceP.FullScreen_RefreshRateInHz = 0; //全屏模式的刷新率
	m_DeviceP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //翻页的时间

	m_D3D->CreateDevice(
		D3DADAPTER_DEFAULT, //当前的适配器
		D3DDEVTYPE_HAL, //D3D设备分两种类型(D3DDEVTYPE_HAL是硬件抽象层、D3DDEVTYPE_REF是参考光栅器)
		m_hWnd, //窗口
		D3DCREATE_HARDWARE_VERTEXPROCESSING, //用硬件来进行顶点处理
		&m_DeviceP, //必要的结构体
		&m_Device); //得到的D3D设备

	D3DXCreateSprite(m_Device, &m_Sprite);
	D3DXCreateLine(m_Device, &m_Line);

	//创建混合用的黑色纹理
	m_Device->CreateTexture(
		m_DeviceP.BackBufferWidth,
		m_DeviceP.BackBufferHeight,
		1,
		0,
		D3DFMT_X8R8G8B8,
		D3DPOOL_MANAGED,
		&m_DarkTexture,
		0);

	D3DXMatrixIdentity(&m_Matrix);

	//得到默认字体
	LOGFONT lf;
	HDC dc = GetDC(m_hWnd);
	GetObject(
		(HFONT)GetCurrentObject(dc, OBJ_FONT),
		sizeof(LOGFONT),
		&lf);
	ReleaseDC(m_hWnd, dc);
	D3DXCreateFontA(
		m_Device,
		lf.lfHeight,
		lf.lfWidth,
		lf.lfWeight,
		1,
		lf.lfItalic,
		lf.lfCharSet,
		lf.lfOutPrecision,
		lf.lfQuality,
		lf.lfPitchAndFamily,
		lf.lfFaceName,
		&m_DefaultFont);
}

CGameOutputD3D::~CGameOutputD3D()
{
	std::map<std::string, LPD3DXFONT>::iterator j;
	for (j = m_Font.begin(); j != m_Font.end(); ++j)
		j->second->Release();

	m_DefaultFont->Release();

	std::map<std::string, _TEXTURE>::iterator i;
	for (i = m_Texture.begin(); i != m_Texture.end(); ++i)
	{
		i->second.texture->Release();
		i->second.transparent_texture->Release();
	}

	m_DarkTexture->Release();
	m_Line->Release();
	m_Sprite->Release();
	m_Device->Release();
	m_D3D->Release();
}

void CGameOutputD3D::Begin()
{
	if (m_DeviceLost)
	{
		if (D3DERR_DEVICENOTRESET == m_Device->TestCooperativeLevel())
		{
			m_Sprite->OnLostDevice();
			m_Line->OnLostDevice();
			m_DefaultFont->OnLostDevice();

			std::map<std::string, LPD3DXFONT>::iterator i;

			for (i = m_Font.begin(); i != m_Font.end(); ++i)
				i->second->OnLostDevice();

			m_Device->Reset(&m_DeviceP);

			for (i = m_Font.begin(); i != m_Font.end(); ++i)
				i->second->OnResetDevice();

			m_DefaultFont->OnResetDevice();
			m_Sprite->OnResetDevice();
			m_Line->OnResetDevice();

			//修改设备丢失标志
			m_DeviceLost = false;

			//恢复成功就要转入正常情况
			m_Device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
			m_Device->BeginScene();
		}
	}
	else
	{
		m_Device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
		m_Device->BeginScene();
	}
}

void CGameOutputD3D::End()
{
	if (!m_DeviceLost)
	{
		if (m_Dark > 0)
		{
			m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);
			RECT r = {0, 0, m_DeviceP.BackBufferWidth, m_DeviceP.BackBufferHeight};
			D3DXVECTOR3 c(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 p(0.0f, 0.0f, 0.0f);
			m_Sprite->Draw(
				m_DarkTexture,
				&r,
				&c,
				&p,
				D3DCOLOR_ARGB(m_Dark, 255, 255, 255));
			m_Sprite->End();
		}

		m_Device->EndScene();

		if (D3DERR_DEVICELOST == m_Device->Present(0, 0, 0, 0))
			m_DeviceLost = true;
	}
}

void CGameOutputD3D::SetTransform(CMyXFORM* pMyXFORM)
{
	m_Matrix._11 = pMyXFORM->eM11;	m_Matrix._12 = pMyXFORM->eM12;		m_Matrix._13 = 0.0f;	m_Matrix._14 = 0.0f;
	m_Matrix._21 = pMyXFORM->eM21;	m_Matrix._22 = pMyXFORM->eM22;		m_Matrix._23 = 0.0f;	m_Matrix._24 = 0.0f;
	m_Matrix._31 = 0.0f;			m_Matrix._32 = 0.0f;				m_Matrix._33 = 1.0f;	m_Matrix._34 = 0.0f;
	m_Matrix._41 = pMyXFORM->eDx;	m_Matrix._42 = pMyXFORM->eDy;		m_Matrix._43 = 0.0f;	m_Matrix._44 = 1.0f;
}

void CGameOutputD3D::EndTransform()
{
	D3DXMatrixIdentity(&m_Matrix);
	m_Sprite->SetTransform(&m_Matrix);
}

unsigned char CGameOutputD3D::GetDark()
{
	return m_Dark;
}

void CGameOutputD3D::SetDark(unsigned char d)
{
	m_Dark = d;
}

//图元相关
void CGameOutputD3D::DrawLine(int x1,
							  int y1,
							  int x2,
							  int y2,
							  unsigned int c,
							  int w)
{
	D3DXVECTOR2 v[2];
	v[0].x = (float)x1; v[0].y = (float)y1;
	v[1].x = (float)x2; v[1].y = (float)y2;
	m_Line->SetWidth((float)w);
	m_Line->Begin();
	m_Line->Draw(v, 2, D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	m_Line->End();
	m_Line->SetWidth(1.0f);
}

void CGameOutputD3D::DrawRectangle1(int x1,
									int y1,
									int x2,
									int y2,
									unsigned int c)
{
	D3DXVECTOR2 v[5];
	v[0].x = (float)x1; v[0].y = (float)y1;
	v[1].x = (float)x2; v[1].y = (float)y1;
	v[2].x = (float)x2; v[2].y = (float)y2;
	v[3].x = (float)x1; v[3].y = (float)y2;
	v[4].x = (float)x1; v[4].y = (float)y1;
	m_Line->Begin();
	m_Line->Draw(v, 5, D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	m_Line->End();
}
void CGameOutputD3D::DrawRectangle2(int x1,
									int y1,
									int x2,
									int y2,
									unsigned int pc,
									unsigned int bc)
{
	D3DRECT r = {x1, y1, x2, y2};
	m_Device->Clear(1, &r, D3DCLEAR_TARGET, D3DCOLOR_XRGB(GetRValue(bc), GetGValue(bc), GetBValue(bc)), 1.0f, 0);

	D3DXVECTOR2 v[5];
	v[0].x = (float)x1; v[0].y = (float)y1;
	v[1].x = (float)x2; v[1].y = (float)y1;
	v[2].x = (float)x2; v[2].y = (float)y2;
	v[3].x = (float)x1; v[3].y = (float)y2;
	v[4].x = (float)x1; v[4].y = (float)y1;
	m_Line->Begin();
	m_Line->Draw(v, 5, D3DCOLOR_XRGB(GetRValue(pc), GetGValue(pc), GetBValue(pc)));
	m_Line->End();
}

bool CGameOutputD3D::LoadPic(const char* fn)
{
	std::string s = fn;
	if (m_Texture.find(s) != m_Texture.end())
		return false;

	_TEXTURE t = {};

	//加载纹理
	D3DXIMAGE_INFO mi;
	D3DXCreateTextureFromFileExA(
		m_Device,
		fn,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_FROM_FILE,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		&mi,
		0,
		&t.texture);
	if (!t.texture)
		return false;
	D3DXCreateTextureFromFileExA(
		m_Device,
		fn,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_FROM_FILE,
		0,

		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 255, 0),
		&mi,
		0,
		&t.transparent_texture);

	File_W_H wh = {mi.Width,mi.Height};
	g_FileName.insert(std::pair<std::string,File_W_H>(s,wh));
	m_Texture.insert(std::pair<std::string, _TEXTURE>(s, t));

	return true;
}

bool CGameOutputD3D::ReleasePic(const char* fn)
{
	std::string s = fn;
	std::map<std::string, _TEXTURE>::iterator i = m_Texture.find(s);
	if (i == m_Texture.end())
		return false;

	i->second.texture->Release();
	i->second.transparent_texture->Release();
	m_Texture.erase(i);

	return true;
}

int CGameOutputD3D::LoadDirectory(const char* dn)
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
				std::string s4 = dn;
				s4 += "\\";
				s4 += fd.name;
				if (LoadPic(s4.c_str()))
					r += 1;
			}
		} while (0 == _findnext(fr, &fd));
		_findclose(fr);
	}

	return r;
}

bool CGameOutputD3D::DrawPic1(const char* fn,
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
	std::map<std::string, _TEXTURE>::iterator i = m_Texture.find(s);
	if (i == m_Texture.end())
		return false;

	RECT r = {sx, sy, sx + dw, sy + dh};
	D3DXVECTOR3 c((float)(sx + dw / 2), (float)(sy + dh / 2), 0.0f);
	D3DXVECTOR3 p(0.0f, 0.0f, 0.0f);

	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX tm;

	switch (m)
	{
	case _MIRROR_NONE:
		{
			D3DXMatrixTranslation(&tm, (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			break;
		}
	case _MIRROR_LR:
		{
			D3DXMATRIX tmt[2];
			D3DXMatrixScaling(&tmt[0], -1.0f, 1.0f, 0.0f);
			D3DXMatrixTranslation(&tmt[1], (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			tm = tmt[0] * tmt[1];
			break;
		}
	case _MIRROR_TB:
		{
			D3DXMATRIX tmt[2];
			D3DXMatrixScaling(&tmt[0], 1.0f, -1.0f, 0.0f);
			D3DXMatrixTranslation(&tmt[1], (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			tm = tmt[0] * tmt[1];
			break;
		}
	case _MIRROR_LRTB: 
		{
			D3DXMATRIX tmt[2];
			D3DXMatrixScaling(&tmt[0], -1.0f, -1.0f, 0.0f);
			D3DXMatrixTranslation(&tmt[1], (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			tm = tmt[0] * tmt[1];
			break;
		}
	}

	D3DXMATRIX transform = tm * m_Matrix;
	m_Sprite->SetTransform(&transform);
	m_Sprite->Draw(
		i->second.transparent_texture,
		&r,
		&c,
		&p,
		D3DCOLOR_ARGB(a, 255, 255, 255));
	D3DXMatrixIdentity(&transform);
	m_Sprite->SetTransform(&transform);

	m_Sprite->End();

	return true;
}

bool CGameOutputD3D::DrawPic2(const char* fn,
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
	std::map<std::string, _TEXTURE>::iterator i = m_Texture.find(s);
	if (i == m_Texture.end())
		return false;

	RECT r = {sx, sy, sx + dw, sy + dh};
	D3DXVECTOR3 c((float)(sx + dw / 2), (float)(sy + dh / 2), 0.0f);
	D3DXVECTOR3 p(0.0f, 0.0f, 0.0f);

	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX tm;

	switch (m)
	{
	case _MIRROR_NONE:
		{
			D3DXMatrixTranslation(&tm, (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			break;
		}
	case _MIRROR_LR:
		{
			D3DXMATRIX tmt[2];
			D3DXMatrixScaling(&tmt[0], -1.0f, 1.0f, 0.0f);
			D3DXMatrixTranslation(&tmt[1], (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			tm = tmt[0] * tmt[1];
			break;
		}
	case _MIRROR_TB:
		{
			D3DXMATRIX tmt[2];
			D3DXMatrixScaling(&tmt[0], 1.0f, -1.0f, 0.0f);
			D3DXMatrixTranslation(&tmt[1], (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			tm = tmt[0] * tmt[1];
			break;
		}
	case _MIRROR_LRTB: 
		{
			D3DXMATRIX tmt[2];
			D3DXMatrixScaling(&tmt[0], -1.0f, -1.0f, 0.0f);
			D3DXMatrixTranslation(&tmt[1], (float)(dx + dw / 2), (float)(dy + dh / 2), 0.0f);
			tm = tmt[0] * tmt[1];
			break;
		}
	}

	m_Sprite->SetTransform(&tm);
	m_Sprite->Draw(
		i->second.texture,
		&r,
		&c,
		&p,
		D3DCOLOR_ARGB(a, 255, 255, 255));
	D3DXMatrixIdentity(&tm);
	m_Sprite->SetTransform(&tm);

	m_Sprite->End();

	return true;
}

bool CGameOutputD3D::LoadFont(const char* id,
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

	LPD3DXFONT f;
	D3DXCreateFontA(
		m_Device,
		h,
		w,
		we,
		1,
		i,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		fn,
		&f);

	if (!f)
		return false;

	m_Font.insert(std::pair<std::string, LPD3DXFONT>(sid, f));

	return true;
}

bool CGameOutputD3D::ReleaseFont(const char* id)
{
	std::string s = id;
	std::map<std::string, LPD3DXFONT>::iterator i = m_Font.find(s);
	if (i == m_Font.end())
		return false;

	i->second->Release();
	m_Font.erase(i);

	return true;
}

bool CGameOutputD3D::DrawString1(const char* id,
								 int x,
								 int y,
								 const char* t,
								 unsigned int c)
{
	RECT r = {x, y, m_DeviceP.BackBufferWidth, m_DeviceP.BackBufferHeight};
	if (id)
	{
		std::string s = id;
		std::map<std::string, LPD3DXFONT>::iterator i = m_Font.find(s);
		if (i == m_Font.end())
			return false;
		i->second->DrawTextA(
			0,
			t,
			(int)strlen(t),
			&r,
			DT_SINGLELINE,
			D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	}
	else
	{
		m_DefaultFont->DrawTextA(
			0,
			t,
			(int)strlen(t),
			&r,
			DT_SINGLELINE,
			D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	}

	return true;
}

bool CGameOutputD3D::DrawString2(const char* id,
								 int x,
								 int y,
								 int w,
								 int h,
								 const char* t,
								 unsigned int c)
{
	RECT r = {x, y, x + w, y + h};
	if (id)
	{
		std::string s = id;
		std::map<std::string, LPD3DXFONT>::iterator i = m_Font.find(s);
		if (i == m_Font.end())
			return false;
		i->second->DrawTextA(
			0,
			t,
			(int)strlen(t),
			&r,
			DT_WORDBREAK,
			D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	}
	else
	{
		m_DefaultFont->DrawTextA(
			0,
			t,
			(int)strlen(t),
			&r,
			DT_WORDBREAK,
			D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	}

	return true;
}

bool CGameOutputD3D::DrawString3(const char* id,
								 int x,
								 int y,
								 int w,
								 int h,
								 const char* t,
								 unsigned int c)
{
	RECT r = {x, y, x + w, y + h};
	if (id)
	{
		std::string s = id;
		std::map<std::string, LPD3DXFONT>::iterator i = m_Font.find(s);
		if (i == m_Font.end())
			return false;
		i->second->DrawTextA(
			0,
			t,
			(int)strlen(t),
			&r,
			DT_VCENTER | DT_CENTER | DT_SINGLELINE,
			D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	}
	else
	{
		m_DefaultFont->DrawTextA(
			0,
			t,
			(int)strlen(t),
			&r,
			DT_VCENTER | DT_CENTER | DT_SINGLELINE,
			D3DCOLOR_XRGB(GetRValue(c), GetGValue(c), GetBValue(c)));
	}

	return false;
}