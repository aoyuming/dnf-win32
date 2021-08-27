#ifndef _GAME_OUTPUT_H_
#define _GAME_OUTPUT_H_

#include <windows.h>

#define _MIRROR_NONE 0 //������
#define _MIRROR_LR 1 //���Ҿ���
#define _MIRROR_TB 2 //���¾���
#define _MIRROR_LRTB 3 //�������¾���

class CMyXFORM : public XFORM
{
public:
	//����
	CMyXFORM();
	//�����Լ�Ϊ��λ����
	void Identity();
	//�����Լ�Ϊƽ�ƾ���
	void Translation(float x, float y);
	//�����Լ�Ϊ���ž���
	void Scaling(float x, float y);
	//�����Լ�Ϊ��ת����
	void Rotation(float a);

	//��������
	static CMyXFORM* _MM(
		const CMyXFORM* m1,
		const CMyXFORM* m2,
		CMyXFORM* m3);
};

class CGameOutput
{
public:
	virtual ~CGameOutput();

	//��ʼ����������
	virtual void Begin();
	virtual void End();

	//���ñ任�������任
	virtual void SetTransform(CMyXFORM* pMyXFORM);
	virtual void EndTransform();

	//��ȡ����������
	virtual unsigned char GetDark();
	virtual void SetDark(unsigned char d);

	//ͼԪ���
	virtual void DrawLine( //����
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0),
		int w = 1);
	virtual void DrawRectangle1( //�����ľ���
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0));
	virtual void DrawRectangle2( //��ʵ�ľ���
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int pc = RGB(0, 0, 0),
		unsigned int bc = RGB(255, 255, 255));

	//λͼ���
	virtual bool LoadPic(const char* fn);
	virtual bool ReleasePic(const char* fn);
	virtual int LoadDirectory(const char* dn);
	virtual bool DrawPic1( //����͸��ɫλͼ
		const char* fn,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		char m = _MIRROR_NONE,
		unsigned char a = 255);
	virtual bool DrawPic2( //����λͼ
		const char* fn,
		int dx,
		int dy,
		int dw,
		int dh,
		int sx,
		int sy,
		char m = _MIRROR_NONE,
		unsigned char a = 255);

	//�������
	virtual bool LoadFont(
		const char* id, //����id
		int w = 0, //�����
		int h = 0, //�����
		int we = 400, //�����ϸ
		bool i = false, //�Ƿ�Ϊб��
		bool u = false, //�Ƿ����»���
		bool s = false, //�Ƿ��д�Խ��
		const char* fn = "����"); //�����������
	virtual bool ReleaseFont(const char* id);
	virtual bool DrawString1( //���Ƶ�������
		const char* id, //����id(����Ϊ0)
		int x, //�����������Ͻ�x
		int y, //�����������Ͻ�y
		const char* t, //����
		unsigned int c); //��ɫ
	virtual bool DrawString2( //���ƶ�������
		const char* id, //����id(����Ϊ0)
		int x, //���־������Ͻ�x
		int y, //���־������Ͻ�y
		int w, //���־��ο�
		int h, //���־��θ�
		const char* t, //����
		unsigned int c); //��ɫ
	virtual bool DrawString3( //���ƾ��ж����������
		const char* id, //����id(����Ϊ0)
		int x, //���־������Ͻ�x
		int y, //���־������Ͻ�y
		int w, //���־��ο�
		int h, //���־��θ�
		const char* t, //����
		unsigned int c); //��ɫ

		//��������
	virtual void DrawEllipse1( //��������Բ
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int c = RGB(0, 0, 0));
	virtual void DrawEllipse2( //��ʵ����Բ
		int x1,
		int y1,
		int x2,
		int y2,
		unsigned int pc = RGB(0, 0, 0),
		unsigned int bc = RGB(255, 255, 255));
};

#endif