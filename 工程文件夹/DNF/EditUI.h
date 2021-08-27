#ifndef _EDIT_UI_H_
#define _EDIT_UI_H_

#include "UI.h"
#include <string>

#define _EDIT_TEXT_CHANGE 1 //�༭�����ݸı�

class CEditUI : public CUI
{
protected:
	int m_Max; //������ֳ���,��Ϊ0���ʾ������
	bool m_Edit; //����״̬
	int m_TextSize; //��������
	int m_TextLength; //���ֳ���
	char* m_Text; //�༭������
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