#ifndef HARM_STRING_H
#define HARM_STRING_H
#include "define_.h"
#include "GameEngine.h"

//�����˺��ַ���
class CHram_String
{
	int m_iX,m_iY;//����
	int m_iHarm;//�˺���Сֵ
	int m_iHarmMax;//�˺����ֵ
public:
	int m_Y;
	bool m_delete;//�Ƿ�����
	CHram_String(int x,int y,int harm);
	void Run();
};


#endif