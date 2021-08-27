#ifndef _SELECT_UI_H_
#define _SELECT_UI_H_

#include "UI.h"
#include <string>

#define _SELECT_NUM_CHANGE 2 //数字被改变消息

class CSelectUI : public CUI
{
protected:
	int m_Min, m_Max, m_Cur; //最小值、最大值、当前值
	std::string m_Text; //按钮的文字
	bool m_In; //光标在内部
	bool m_Down; //按下标志
	int m_Pos; //左中右标志-1、0、1表示左中右

public:
	CSelectUI(const char* t, int min, int max, int cur, int x, int y, int w, int h, bool v = true, bool a = true);

	//virtual void Init();
	virtual void ActRender();
	virtual void UnactRender();
	virtual void OnMsg(int Msg, int p1, int p2);
	//virtual void End();

	void SetText(const char* t);
	const char* GetText();

	bool SetCur(int cur);
	int GetCur();
};

#endif