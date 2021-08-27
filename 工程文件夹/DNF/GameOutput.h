#ifndef _GAME_OUTPUT_H_
#define _GAME_OUTPUT_H_

#include <windows.h>

#define _MIRROR_NONE 0 //不镜像
#define _MIRROR_LR 1 //左右镜像
#define _MIRROR_TB 2 //上下镜像
#define _MIRROR_LRTB 3 //左右上下镜像

class CMyXFORM : public XFORM
{
public:
	//构造
	CMyXFORM();
	//设置自己为单位矩阵
	void Identity();
	//设置自己为平移矩阵
	void Translation(float x, float y);
	//设置自己为缩放矩阵
	void Scaling(float x, float y);
	//设置自己为旋转矩阵
	void Rotation(float a);

	//矩阵联乘
	static CMyXFORM* _MM(
		const CMyXFORM* m1,
		const CMyXFORM* m2,
		CMyXFORM* m3);
};

class CGameOutput
{
public:
	virtual ~CGameOutput();

	//开始、结束绘制
	virtual void Begin();
	virtual void End();

	//设置变换、结束变换
	virtual void SetTransform(CMyXFORM* pMyXFORM);
	virtual void EndTransform();

	//获取、设置亮度
	virtual unsigned char GetDark();
	virtual void SetDark(unsigned char d);

	//图元相关
	virtual void DrawLine( //画线
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0),
		int w = 1);
	virtual void DrawRectangle1( //画空心矩形
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0));
	virtual void DrawRectangle2( //画实心矩形
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int pc = RGB(0, 0, 0),
		unsigned int bc = RGB(255, 255, 255));

	//位图相关
	virtual bool LoadPic(const char* fn);
	virtual bool ReleasePic(const char* fn);
	virtual int LoadDirectory(const char* dn);
	virtual bool DrawPic1( //绘制透明色位图
		const char* fn,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		char m = _MIRROR_NONE,
		unsigned char a = 255);
	virtual bool DrawPic2( //绘制位图
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
	virtual bool LoadFont(
		const char* id, //字体id
		int w = 0, //字体宽
		int h = 0, //字体高
		int we = 400, //字体粗细
		bool i = false, //是否为斜体
		bool u = false, //是否有下划线
		bool s = false, //是否有穿越线
		const char* fn = "宋体"); //字体外观名称
	virtual bool ReleaseFont(const char* id);
	virtual bool DrawString1( //绘制单行文字
		const char* id, //字体id(允许为0)
		int x, //单行文字左上角x
		int y, //单行文字左上角y
		const char* t, //文字
		unsigned int c); //颜色
	virtual bool DrawString2( //绘制多行文字
		const char* id, //字体id(允许为0)
		int x, //文字矩形左上角x
		int y, //文字矩形左上角y
		int w, //文字矩形宽
		int h, //文字矩形高
		const char* t, //文字
		unsigned int c); //颜色
	virtual bool DrawString3( //绘制居中对齐多行文字
		const char* id, //字体id(允许为0)
		int x, //文字矩形左上角x
		int y, //文字矩形左上角y
		int w, //文字矩形宽
		int h, //文字矩形高
		const char* t, //文字
		unsigned int c); //颜色

		//本类特有
	virtual void DrawEllipse1( //画空心椭圆
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0));
	virtual void DrawEllipse2( //画实心椭圆
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int pc = RGB(0, 0, 0),
		unsigned int bc = RGB(255, 255, 255));
};

#endif