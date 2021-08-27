#ifndef _DNF_PROTOCOL_H
#define _DNF_PROTOCOL_H

#define _TRY_NICK_NAME				 1//(客户端-服务器)检测昵称
#define _NICK_NAME_LAWLESS			 2//(服务器-客户端)昵称非法
#define _NICK_NAME_LEGALITY			 3//(服务器-客户端)昵称合法

#define _SEND_TALK					 4//(客户端-服务器)聊天
#define _PLAYER_TALK				 5//(服务器(群)-客户端)聊天

#define _SEND_CHOICE_ROLE			 6//(客户端-服务器)选择角色
#define	_RETURN_CHOICE_ROLE			 7//(服务器-客户端)返回下一个场景
#define _NEW_PLAYER_CHOICE_ROLE	     8//(服务器(群)-客户端)新角色加入

#define _SEND_WALK					10//(客户端-服务器)移动
#define _PLAYER_WALK				11//(服务器(群)-客户端)移动

#define _SEND_FOUND_ROOM			12//(客户端-服务器)创建房间
#define _RETURN_FOUND_ROOM			13//(客户端-服务器)返回创建房间
#define _PLAYER_FOUND_ROOM		    14//(服务器(群)-客户端)玩家创建房间

#define _SEND_PREPARE				15//(客户端-服务器)准备
#define	_PLAYER_PREPARE				16//(服务器(群)-客户端)转发准备
#define	_BEGIN_DULE					17//(服务器(群)-客户端)开始决斗

#define _SEND_YI_DONG_SHE_JI		18//(客户端-服务器)发动进入移动射击(技能)状态
#define _PLAYER_YI_DONG_SHE_JI		19//(服务器(群)-客户端)转发进入移动射击(技能)状态

#define _YI_DONG_SHE_JI_DIR			20//(客户端-服务器)移动射击变换方向
#define _ZF_YI_DONG_SHE_JI_DIR		99//(服务器(群)-客户端)转发移动射击变换方向

#define _SEND_YDSJ_MOVE				21//(客户端-服务器)移动射击状态下移动
#define _ZF_YDSJ_MOVE				22//(服务器(群)-客户端)转发移动射击状态移动

#define _SEND_DEATH					23//(客户端-服务器)死亡
#define _PLAYER_DEATH				24//(服务器(群)-客户端)转发玩家死亡
#define _DULE_END					25//(服务器(群)-客户端)比赛结束

#define _SEND_QUIT_ROOM				26//(客户端-服务器)退出房间
#define _PLAYER_QUIT_ROOM			27//(服务器(群)-客户端)转发玩家退出房间
#define _RETURN_SCENE				28//(客户端-服务器)返回上一个场景

#define _SEND_QUIT_GAME				29//(客户端-服务器)退出游戏
#define _RETURN_QUIT_GAME			30//(服务器-客户端)退出游戏
#define _PLAYER_QUIT_GAME			31//(服务器(群)-客户端)转发玩家退出游戏

#define _YDSJ_DA_ZHONG_REN			32//(客户端-服务器)移动射击打中人
#define _ZF_YDSJ_DA_ZHONG_REN       33//(服务器(群)-客户端)转发玩家进入房间
#define _RETURN_ENTER_ROOM			34//(服务器-客户端)返回房间信息，与玩家信息

#define _SEND_ENTER_TOWN			35//(客户端-服务器)进入城镇
#define _PLAYER_ENTER_TOWN			36 //(服务器(群)-客户端)转发玩家进入城镇
#define _RETURN_PLAYER_TOWN			37//(服务器-客户端)发送玩家数据给客户断

#define _SEND_TONG_BU				38//(客户端服务器)发送同步消息
#define _RETURN_TONG_BU				39//(服务器——客户端)同步玩家数据

#define _JIN_RU_JUE_DOU				40//(服务器——客户端)进入决斗场景
#define _SUO_YOU_PLAYER_ZHUN_BEI	41//(服务器——客户端)所有玩家准备
#define _FANG_ZHU_ZHUN_BEI          42//(客户端——服务器)房主准备

#define _ENTER_XUE_ZHE_FANG_JIAN	43//(客户端_服务器)进入选择房间界面
#define _RETURN_XUE_ZHE_FANG_JIAN	44//(服务器_客户端)返回房间数据给客户端

#define _ROOM_NO_ENTER				45//(服务器-客户端)房间暂时不能进入
#define _ROOM_OK_ENTER				46//(服务器_客户端)房间可以进入

//#define _ROOM_RENSHU_ZENGJIA		47//(服务器-客户端)房间人数增加
//#define _ROOM_RENSHU_JIANSHAO		48//(服务器-客户端)房间人数减少
#define _ROOM_RENSHU_BIAN_HUA		47//(服务器-客户端)房间人数减少
#define _SET_FANG_ZHU				48//(服务器-客户端)设置房主


#define _DAI_JI_ZHUANG_TAI			49//(客户端-服务器)待机状态
#define _ZHUN_FA_DAI_JI				50//(服务器-客户端)转发待机状态

#define _YDSJ_GONG_JI				51//(客户端-服务器)移动射击攻击
#define _ZF_YDSJ_GONG_JI			52//(服务器-客户端)转发移动射击攻击


#define _GAME_END					53//(客户端-服务器)比赛结束
#define	_ZF_GAME_END				54//(服务器-客户端)转发比赛结束

#define _FANHUI_PLY					55//(服务器-客户端)返回玩家数据


//矩形范围
struct _RECT
{
	int up;
	int down;
	int left;
	int right;
};

//玩家结构体
struct PLAYER
{
	int state;//状态
	char name[32];//名字
	int hp,mp;//血，蓝
	int x,y;//坐标
	int PicBegin;//贴图开始下标
	int PicEnd;//贴图结束下标
	bool image;//镜像
	int role;//使用的角色	
	int scene;//场景
	bool ZhunBei;//是否准备
	bool FangZhu;//是否是房主
	bool bMove;//移动状态
	bool bMovedir;//移动状态
	bool bStand;//待机状态
	bool bShooting_move;//进入移动射击状态
	bool bShooting_move1;//移动射击状态下移动
	bool bShooting_move2;//移动射击状态下攻击
	int JiangZhiDU;//僵直度
	bool JiangZhiZT;//僵直状态
	int hram;//技能伤害
};

//技能信息结构体
struct SKILL
{
	int x,y;//坐标
	int PicBegin;//贴图开始下标
	int PicEnd;//贴图结束下标
	_RECT r;//碰撞范围
	int harm;//技能伤害
	bool image;//镜像
};
//房间信息结构体
struct FangJianXinXi
{
	int FangJianHao;
	char room_name[32];
	char mima[32];
	int PYRnum;
	int buttonI;
	bool Return;
	bool ZhanDou;//是否在战斗
};

struct _GAME_PROTOCOL
{
	int id;//协议ID
};

//检测昵称
struct _C_S_GP1 : public _GAME_PROTOCOL
{
	_C_S_GP1()
	{
		id = _TRY_NICK_NAME;
	}
	char nick_name[32];
};
//昵称合法
struct _S_C_GP1 : public _GAME_PROTOCOL
{
	_S_C_GP1()
	{
		id = _NICK_NAME_LEGALITY;
	}
	//char scene[32];//下一个场景
};
//昵称非法
struct _S_C_GP2 : public _GAME_PROTOCOL
{
	_S_C_GP2()
	{
		id = _NICK_NAME_LAWLESS;
	}
};


//选择角色
struct _C_S_GP2 : public _GAME_PROTOCOL
{
	_C_S_GP2()
	{
		id = _SEND_CHOICE_ROLE;
	}
	short role_flag;
};
//返回选择角色
struct _S_C_GP3 : public _GAME_PROTOCOL
{
	_S_C_GP3()
	{
		id = _RETURN_CHOICE_ROLE;
	}
	char scene[32];
	PLAYER pyr;
};
//选择角色，新玩家加入
struct _S_C_GP4 : public _GAME_PROTOCOL
{
	_S_C_GP4()
	{
		id = _NEW_PLAYER_CHOICE_ROLE;
	}
	PLAYER pyr;//玩家信息
	//char scene[32];//下一个场景
};

//发送聊天
struct _C_S_GP3 : public _GAME_PROTOCOL
{
	_C_S_GP3()
	{
		id = _SEND_TALK;
	}
	char str[128];//发送的字符串
	char scene;//哪个场景发的
};
//转发聊天
struct _S_C_GP5 :  public _GAME_PROTOCOL
{
	_S_C_GP5()
	{
		id = _PLAYER_TALK;
	}
	char str[128];//发送的字符串
	char scene;//哪个场景发的
};

//移动
struct _C_S_GP4 : public _GAME_PROTOCOL
{
	_C_S_GP4()
	{
		id = _SEND_WALK;
	}
	int state;//状态
	bool bMove;//内部函数标记
	int MoveDir;//移动的方向
	int x,y;
	bool image;//镜像
};
//转发移动
struct _S_C_GP6 : public _GAME_PROTOCOL
{
	_S_C_GP6()
	{
		id = _PLAYER_WALK;
	}
	PLAYER pyr;
};

//创建房间
struct _C_S_GP5 : public _GAME_PROTOCOL
{
	_C_S_GP5()
	{
		id = _SEND_FOUND_ROOM;
	}
	char room_name[32];//房间名
	char mima[32];//密码
};
//返回创建房间
struct _S_C_GP7 : public _GAME_PROTOCOL
{
	_S_C_GP7()
	{
		id = _RETURN_FOUND_ROOM;
	}
	int room_index;//房间号
};
//转发创建房间
struct _S_C_GP8 : public _GAME_PROTOCOL
{
	_S_C_GP8()
	{
		id = _PLAYER_FOUND_ROOM;
	}
	char room_name[32];
	char mima[32];
	int FangJianHao;
	int num;
	bool ZhanDou;
};

//准备
struct _C_S_GP6 : public _GAME_PROTOCOL
{
	_C_S_GP6()
	{
		id = _SEND_PREPARE;
	}
	bool prepare;//true:为准备，false:为取消准备
	int scene;//房间号
	bool master;//是否是房主
};
//转发准备
struct _S_C_GP9 : public _GAME_PROTOCOL
{
	_S_C_GP9()
	{
		id = _PLAYER_PREPARE;
	}
	char name[32];//哪个玩家
	bool prepare;//true:为准备，false:为取消准备
};
//开始决斗
struct _S_C_GP10 : public _GAME_PROTOCOL
{
	_S_C_GP10()
	{
		id = _BEGIN_DULE;
	}
	char scene[32];//下一个场景
	PLAYER pyr;
};


//发动进入移动射击(技能)状态
struct _C_S_GP7 : public _GAME_PROTOCOL
{
	_C_S_GP7()
	{
		id = _SEND_YI_DONG_SHE_JI;
	}
	int state;
	bool bShooting_move;
	int image;
};
//转发进入移动射击(技能)状态
struct _S_C_GP11 : public _GAME_PROTOCOL
{
	_S_C_GP11()
	{
		id = _PLAYER_YI_DONG_SHE_JI;
	}
	PLAYER pyr;
};
//移动射击变换方向
struct _C_S_GP100 : public _GAME_PROTOCOL
{
	_C_S_GP100()
	{
		id = _YI_DONG_SHE_JI_DIR;
	}
	bool dir;
};
//转发移动射击变换方向
struct _S_C_GP100 : public _GAME_PROTOCOL
{
	_S_C_GP100()
	{
		id = _ZF_YI_DONG_SHE_JI_DIR;
	}
	bool dir;
	char name[32];
};
//移动射击状态下移动
struct _C_S_GP8 : public _GAME_PROTOCOL
{
	_C_S_GP8()
	{
		id = _SEND_YDSJ_MOVE;
	}
	int x,y;
	int state;
	bool bShooting_move1;
	int image;
};
//转发移动射击状态移动
struct _S_C_GP13 : public _GAME_PROTOCOL
{
	_S_C_GP13()
	{
		id = _ZF_YDSJ_MOVE;
	}
	PLAYER pyr;
};

//玩家死亡
struct _C_S_GP9 : public _GAME_PROTOCOL
{
	_C_S_GP9()
	{
		id = _SEND_DEATH;
	}
	PLAYER pyr;
};

//玩家死亡转发
struct _S_C_GP14 : public _GAME_PROTOCOL
{
	_S_C_GP14()
	{
		id = _PLAYER_DEATH;
	}
	PLAYER pyr;
};
//比赛结束
struct _S_C_GP15 : public _GAME_PROTOCOL
{
	_S_C_GP15()
	{
		id = _RETURN_SCENE;
	}
	char scene[32];
	PLAYER pyr;
};

//退出房间
struct _C_S_GP10 : public _GAME_PROTOCOL
{
	_C_S_GP10()
	{
		id = _SEND_QUIT_ROOM;
	}
	int room_index;
	bool fangzhu;
};
//退出房间
struct _S_C_GP16 : public _GAME_PROTOCOL
{
	_S_C_GP16()
	{
		id = _PLAYER_QUIT_ROOM;
	}
	int scene;
	char name[32];
};
//转发退出房间
struct _S_C_GP17 : public _GAME_PROTOCOL
{
	_S_C_GP17()
	{
		id = _RETURN_SCENE;
	}
	char scene[32];
	PLAYER pyr;
	int room_index;
};
//退出游戏
struct _C_S_GP11 : public _GAME_PROTOCOL
{
	_C_S_GP11()
	{
		id = _SEND_QUIT_GAME;
	}
	char scene[32];
	char name[32];
};
//退出游戏
struct _S_C_GP18 : public _GAME_PROTOCOL
{
	_S_C_GP18()
	{
		id = _RETURN_QUIT_GAME;
	}
};
//转发退出游戏
struct _S_C_GP19 : public _GAME_PROTOCOL
{
	_S_C_GP19()
	{
		id = _PLAYER_QUIT_GAME;
	}
	char name[32];
};

//移动射击打中人
struct _C_S_GP12 : public _GAME_PROTOCOL
{
	_C_S_GP12()
	{
		id = _YDSJ_DA_ZHONG_REN;
	}
	int x,y;
	int hram;
	int hp;
	char name[32];
	bool image;
	int state;
	int JiangZhiDU;
	bool JiangZhiZT;
};
//转发移动射击打中人
struct _S_C_GP20 : public _GAME_PROTOCOL
{
	_S_C_GP20()
	{
		id = _ZF_YDSJ_DA_ZHONG_REN;
	}

	PLAYER pyr;
};
////转发进入房间
//struct _S_C_GP21 : public _GAME_PROTOCOL
//{
//	_S_C_GP21()
//	{
//		id = _PLAYER_ENTER_ROOM;
//	}
//	PLAYER pyr;
//};

//玩家进入城镇
struct _C_S_GP13 : public _GAME_PROTOCOL
{
	_C_S_GP13()
	{
		id = _SEND_ENTER_TOWN;
	}
	bool fanhui;
	PLAYER pyr;
};

//转发进入城镇
struct _S_C_GP22 : public _GAME_PROTOCOL
{
	_S_C_GP22()
	{
		id = _PLAYER_ENTER_TOWN;
	}
	PLAYER pyr;
};

//返回玩家数据（进入城镇）
struct _S_C_GP23 : public _GAME_PROTOCOL
{
	_S_C_GP23()
	{
		id = _RETURN_PLAYER_TOWN;
	}
	int num;
	PLAYER pyr[32];
};

//发送同步消息
struct _C_S_GP14 : public _GAME_PROTOCOL
{
	_C_S_GP14()
	{
		id = _SEND_TONG_BU;
	}
	PLAYER pyr;
};

//转发同步消息
struct _S_C_GP24 : public _GAME_PROTOCOL
{
	_S_C_GP24()
	{
		id = _RETURN_TONG_BU;
	}
	int num;
	PLAYER pyr[32];
};
//进入决斗场景
struct _S_C_GP25 : public _GAME_PROTOCOL
{
	_S_C_GP25()
	{
		id = _JIN_RU_JUE_DOU;
	}
	int num;
	PLAYER pyr[32];
};

//房主准备
struct _C_S_GP15 : public _GAME_PROTOCOL
{
	_C_S_GP15()
	{
		id = _FANG_ZHU_ZHUN_BEI;
	}
	int scene;
};


//进入选择房间界面
struct _C_S_GP16 : public _GAME_PROTOCOL
{
	_C_S_GP16()
	{
		id = _ENTER_XUE_ZHE_FANG_JIAN;
	}
};
//返回房间数据
struct _S_C_GP26 : public _GAME_PROTOCOL
{
	_S_C_GP26()
	{
		id = _RETURN_XUE_ZHE_FANG_JIAN;
	}
	int num;
	FangJianXinXi fjxx[32];
};

//房间暂时不能进入
struct _S_C_GP27 : public _GAME_PROTOCOL
{
	_S_C_GP27()
	{
		id = _ROOM_NO_ENTER;
	}
	int fangjianhao;
};

//房间可以进入S
struct _S_C_GP28 : public _GAME_PROTOCOL
{
	_S_C_GP28()
	{
		id = _ROOM_OK_ENTER;
	}
	int fangjianhao;
};

//房间人数增加
struct _S_C_GP29 : public _GAME_PROTOCOL
{
	_S_C_GP29()
	{
		id = _ROOM_RENSHU_BIAN_HUA;
	}
	int fangjianhao;
	int renshu;
};

//设置房主
struct _S_C_GP30 : public _GAME_PROTOCOL
{
	_S_C_GP30()
	{
		id = _SET_FANG_ZHU;
	}
	char name[32];
};
//待机状态
struct _C_S_GP17 : public _GAME_PROTOCOL
{
	_C_S_GP17()
	{
		id = _DAI_JI_ZHUANG_TAI;
	}
	bool bStand;//内部函数标记
	bool image;//镜像
	int state;//状态
};
//转发待机状态 
struct _S_C_GP31 : public _GAME_PROTOCOL
{
	_S_C_GP31()
	{
		id = _ZHUN_FA_DAI_JI;
	}
	PLAYER pyr;
};
//移动射击攻击
struct _C_S_GP18 : public _GAME_PROTOCOL
{
	_C_S_GP18()
	{
		id = _YDSJ_GONG_JI;
	}
	bool bShooting_move2;
	int image;
	int state;

};
//转发移动射击攻击
struct _S_C_GP32 : public _GAME_PROTOCOL
{
	_S_C_GP32()
	{
		id = _ZF_YDSJ_GONG_JI;
	}
	PLAYER pyr;
};
//比赛结束
struct _C_S_GP19 : public _GAME_PROTOCOL
{
	_C_S_GP19()
	{
		id = _GAME_END;
	}

};
//转发比赛结束
struct _S_C_GP33 : public _GAME_PROTOCOL
{
	_S_C_GP33()
	{
		id = _ZF_GAME_END;
	}
};
//返回玩家数据
struct _S_C_GP34 : public _GAME_PROTOCOL
{
	_S_C_GP34()
	{
		id = _FANHUI_PLY;
	}
	int num;
	PLAYER pyr[8];
};


















#endif