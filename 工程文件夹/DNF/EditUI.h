#ifndef _EDIT_UI_H_
#define _EDIT_UI_H_

#include "UI.h"
#include <string>

#define _EDIT_TEXT_CHANGE 1 //编辑框内容改变

class CEditUI : public CUI
{
protected:
	int m_Max; //最大文字长度,若为0则表示无限制
	bool m_Edit; //输入状态
	int m_TextSize; //文字容量
	int m_TextLength; //文字长度
	char* m_Text; //编辑框文字
	char * m_BackPicture;
	void _Add(int size);

public:
	CEditUI(int max, const char* t, int x, int y, int w, int h,char*pic, bool v = true, bool a = true);
	~CEditUI();
    bool m_red;
	//virtual void Init();
	virtual void ActRender();
	virtual void UnactRender();
	virtual void OnMsg(int Msg, int p1, int p2);
	//virtual void End();

	void SetText(const char* t);
	const char* GetText();

};

#endif