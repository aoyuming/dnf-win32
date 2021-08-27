#ifndef _UI_H_
#define _UI_H_

#include <string>

//UI(User Interface):用户接口,即游戏界面中与用户进行交互的界面元素

//UI的消息
#define _UIM_C_ENTER 0 //光标进入
#define _UIM_C_QUIT 1 //光标退出
#define _UIM_C_MOVE_IN 2 //光标在控件上移动
#define _UIM_C_MOVE_OUT 3 //光标在控件外移动
#define _UIM_C_LBD_IN 4 //左键在控件上按下
#define _UIM_C_LBU_IN 5 //左键在控件上抬起
#define _UIM_C_LBD_OUT 6 //左键在控件外按下
#define _UIM_C_LBU_OUT 7 //左键在控件外抬起
#define _UIM_C_RBD_IN 8 //右键在控件上按下
#define _UIM_C_RBU_IN 9 //右键在控件上抬起
#define _UIM_C_RBD_OUT 10 //右键在控件外按下
#define _UIM_C_RBU_OUT 11 //右键在控件外抬起

//同一时间一个场景中只能有一个控件收到键盘消息、字符消息

#define _UIM_K_D 12 //键按下
#define _UIM_K_U 13 //键抬起
#define _UIM_K_CHAR 14 //打字

class CScene; //场景

class CUI
{
protected:
	friend class CScene;

	int m_X, m_Y, m_W, m_H; //矩形
	bool m_View; //能否看见
	bool m_Act; //是否激活
	std::string m_Name; //名字
	CScene* m_Scene; //所属场景

public:
	CUI(int x, int y, int w, int h, bool v = true, bool a = true);
	virtual ~CUI();

	void SetRect(int x, int y, int w, int h);
	void GetRect(int* x, int* y, int* w, int* h);
	void SetView(bool View);
	bool GetView();
	void SetAct(bool Act);
	bool GetAct();

	//子类需要实现的虚函数
	virtual void Init(); //初始化
	virtual void ActRender(); //激活渲染
	virtual void UnactRender(); //非激活渲染

	//光标消息:p1为x坐标,p2为y坐标(相对于控件矩形而言)
	//键盘消息:p1传入键值
	//字符消息:p1传入字符,p2表示是(1)否(0)为双字节
	virtual void OnMsg(int Msg, int p1, int p2); //得到的消息
	virtual void End(); //初始化
};

#endif