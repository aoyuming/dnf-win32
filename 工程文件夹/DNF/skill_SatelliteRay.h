#ifndef SKILL_SATELLITERAY_H
#define SKILL_SATELLITERAY_H
#include "Skill.h"
#include "define_.h"
#include "Vector2D.h"

enum WXSX : char
{
	RELEASE,//释放状态
	ECTPTOMA,//落下
	RUN//运行
};
//卫星射线
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
	std::string m_FileName;//文件名称
	time_t m_tIntervalTimeBegin;//间隔时间开始
	time_t m_tIntervalTimeEnd;//间隔时间结束
	int m_iState;//觉醒状态



	CSkill_Satellite_Ray(int x,int y,int begpic,int endpic,int harm,int image,int h,int w);
	void ReleaseState();//释放状态
	void EctptomaState();//落下状态
	void RunState();//运行状态
	void DrawMotion(int index,char* fm ,int x,int y ,bool image);
	RECT GetRect();//获取矩形碰撞范围
	void Run();

};



#endif