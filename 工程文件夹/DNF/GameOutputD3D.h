#ifndef _GAME_OUTPUT_D3D_H_
#define _GAME_OUTPUT_D3D_H_

#include "GameOutput.h"
#include <windows.h>
#include <string>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>
//#include "town.h"
//class Ctown_Scene;
class CGameOutputD3D : public CGameOutput
{
	HWND m_hWnd;
	LPDIRECT3D9 m_D3D;
	D3DPRESENT_PARAMETERS m_DeviceP;
	LPDIRECT3DDEVICE9 m_Device;
	LPD3DXSPRITE m_Sprite;
	LPD3DXLINE m_Line;
	bool m_DeviceLost;
	unsigned char m_Dark;
	LPDIRECT3DTEXTURE9 m_DarkTexture;
	struct _TEXTURE
	{
		LPDIRECT3DTEXTURE9 texture;
		LPDIRECT3DTEXTURE9 transparent_texture;
	};
	D3DXMATRIX m_Matrix;
	std::map<std::string, _TEXTURE> m_Texture;
	LPD3DXFONT m_DefaultFont;
	std::map<std::string, LPD3DXFONT> m_Font;

public:
	CGameOutputD3D(HWND hWnd);
	~CGameOutputD3D();

	//开始、结束绘制
	void Begin();
	void End();

	//设置变换、结束变换
	void SetTransform(CMyXFORM* pMyXFORM);
	void EndTransform();

	//获取、设置亮度
	unsigned char GetDark();
	void SetDark(unsigned char d);

	//图元相关
	void DrawLine( //画线
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0),
		int w = 1);
	void DrawRectangle1( //画空心矩形
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0));
	void DrawRectangle2( //画实心矩形
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int pc = RGB(0, 0, 0),
		unsigned int bc = RGB(255, 255, 255));

	//位图相关
	bool LoadPic(const char* fn);
	bool ReleasePic(const char* fn);
	int LoadDirectory(const char* dn);
	bool DrawPic1( //绘制透明色位图
		const char* fn,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		char m = _MIRROR_NONE,
		unsigned char a = 255);
	bool DrawPic2( //绘制位图
		const char* fn,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		char m = _MIRROR_NONE,
		unsigned char a = 255);

	//字体相关
	bool LoadFont(
		const char* id, //字体id
		int w = 0, //字体宽
		int h = 0, //字体高
		int we = 400, //字体粗细
		bool i = false, //是否为斜体
		bool u = false, //是否有下划线
		bool s = false, //是否有穿越线
		const char* fn = "宋体"); //字体外观名称
	bool ReleaseFont(const char* id);
	bool DrawString1( //绘制单行文字
		const char* id, //字体id(允许为0)
		int x, //单行文字左上角x
		int y, //单行文字左上角y
		const char* t, //文字
		unsigned int c); //颜色
	bool DrawString2( //绘制多行文字
		const char* id, //字体id(允许为0)
		int x, //文字矩形左上角x
		int y, //文字矩形左上角y
		int w, //文字矩形宽
		int h, //文字矩形高
		const char* t, //文字
		unsigned int c); //颜色
	bool DrawString3( //绘制居中对齐多行文字
		const char* id, //字体id(允许为0)
		int x, //文字矩形左上角x
		int y, //文字矩形左上角y
		int w, //文字矩形宽
		int h, //文字矩形高
		const char* t, //文字
		unsigned int c); //颜色
	
};

#endif