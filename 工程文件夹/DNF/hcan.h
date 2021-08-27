#ifndef _HCAN_H
#define _HCAN_H
#include "define_.h"
#include "GameEngine.h"


//Ӣ�۵�״̬
enum HcanState : char 
{
	_HCAN_STAND,//վ��
	_HCAN_MOVE_SHOOT,//�ƶ����״̬
};
class CHero;
class CHcan
{
	int m_iX,m_iY;//����
	int m_iPicBegin;//��ͼ��ʼ������
	int m_iPicEnd;//��ͼ����������
	int m_iPicW;//��ͼ��
	int m_iPicH;//��ͼ��
	int m_bImage;//����
	int m_iState;//״̬
	time_t m_tIntervalTimeBegin;//���ʱ�俪ʼ
	time_t m_tIntervalTimeEnd;//���ʱ�����
	std::string m_FileName;//�ļ�����
public:
	CHcan(int x,int y,int begin,int end,int dir);
	void Run();
	//���ƶ���
	void DrawMotion(int index);
	//����
	void daiji();
	//�ƶ����״̬
	void Move_Shooting_State();

	friend class CHero;
};



#endif