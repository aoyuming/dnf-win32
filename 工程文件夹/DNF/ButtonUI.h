#ifndef _BUTTON_UI_H_
#define _BUTTON_UI_H_

#include "UI.h"
#include <string>

#define _BUTTON_CLICK 0 //按钮被按下消息

#define _BUTTON_STATE_CURSOR_OUT 0 //光标在外面
#define _BUTTON_STATE_CURSOR_IN_UP 1 //光标在内部是抬起
#define _BUTTON_STATE_CURSOR_IN_DOWN 2 //光标在内部是按下

class CButtonUI : public CUI
{
protected:
	int m_ButtonState; //按钮状态
	bool m_DownIn; //是否在按钮上面按下
	std::string m_Text; //按钮的文字

public:
	CButtonUI(const char* t, int x, int y, int w, int h, bool v = true, bool a = true);

	//virtual void Init();
	virtual void ActRender();
	virtual void UnactRender();
	virtual void OnMsg(int Msg, int p1, int p2);
	//virtual void End();

	void SetText(const char* t);
	const char* GetText();
};

#endif