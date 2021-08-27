#ifndef SKILL_SATELLITERAY_H
#define SKILL_SATELLITERAY_H
#include "Skill.h"
#include "define_.h"
#include "Vector2D.h"

enum WXSX : char
{
	RELEASE,//�ͷ�״̬
	ECTPTOMA,//����
	RUN//����
};
//��������
class CSkill_Satellite_Ray : public CSkill
{
	int m_iPicend;
	int m_iluoxiabeg;
	int m_iluoxiaend;
	int m_idixiabeg;
	int m_idixiaend;
	int m_iZKPicbeg;
	int m_iZKPicend;
	int m_zhenliang;
public:
	int m_iPicbeg;
	bool m_delete;
	std::string m_FileName;//�ļ�����
	time_t m_tIntervalTimeBegin;//���ʱ�俪ʼ
	time_t m_tIntervalTimeEnd;//���ʱ�����
	int m_iState;//����״̬



	CSkill_Satellite_Ray(int x,int y,int begpic,int endpic,int harm,int image,int h,int w);
	void ReleaseState();//�ͷ�״̬
	void EctptomaState();//����״̬
	void RunState();//����״̬
	void DrawMotion(int index,char* fm ,int x,int y ,bool image);
	RECT GetRect();//��ȡ������ײ��Χ
	void Run();

};



#endif