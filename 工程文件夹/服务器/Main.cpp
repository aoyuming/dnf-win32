#include <iostream>
#include "TcpServer.h"
#include <windows.h>
#include "DNFprotocol.h"
#include <map>
#include <time.h>
#include <string>
#include <vector>

int FangJianHao = 1;

//英雄的状态
enum HeroState : char 
{
	_HERO_STAND,//站立
	_HERO_MOVE_,//移动
	_HERO_MOVE_SHOOTING_MOVE,//移动射击状态下移动
	_HERO_MOVE_SHOOT,//移动射击
	_HERO_MOVE_ATTACK,//移动射击攻击
	_HERO_END_MOVESHOOTINGK,//停止移动射击
	_HERO_MOVE_SHOOTING_DIR,//变换移动射击方向
	_HERO_RUSH,//奔跑
	_HERO_WEI_XING_SHE_XIAN,//卫星射线
	_HERO_JIANG_ZHI,//僵直状态
	_HERO_DEATH,//死亡状态
	_HERO_PUTONGGONGJI//普通攻击
};

CTcpServer ts;//服务器

std::map<SOCKET,PLAYER>  allPLAYER;//玩家
std::map<int,FangJianXinXi> FJXX;
int x = 0;
void NetData(SOCKET s,char* data,int len)
{
	_GAME_PROTOCOL* pgp = (_GAME_PROTOCOL*)data;
	switch(pgp->id)
	{
	case _TRY_NICK_NAME:
		{
			_C_S_GP1* p = (_C_S_GP1*)data;
			//检测昵称的合法性
			bool ok = true;
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();
				it != allPLAYER.end(); ++it )
			{
				if(strcmp(p->nick_name, it->second.name) == 0)
				{
					ok = false;
					break;
				}
			}
			if(ok)
			{
				//合法
				_S_C_GP1 scgp1;
				ts.SendData(s,(char*)&scgp1,sizeof(scgp1));

			}
			else
			{
				//非法
				_S_C_GP2 scgp2;
				ts.SendData(s,(char*)&scgp2,sizeof(scgp2));
			}

		}break;
	case _SEND_FOUND_ROOM://玩家创建房间
		{
			//设置房间属性
			_C_S_GP5* cs5 = (_C_S_GP5*)data;
			FangJianXinXi fjxx;
			fjxx.FangJianHao = FangJianHao;
			strcpy_s(fjxx.mima,32,cs5->mima);
			strcpy_s(fjxx.room_name,32,cs5->room_name);
			fjxx.PYRnum = 1;
			fjxx.ZhanDou = false;
			FJXX.insert(std::pair<int,FangJianXinXi>(fjxx.FangJianHao,fjxx));

			//返回创建房间
			_S_C_GP7 sc7;
			sc7.room_index = FangJianHao;
			ts.SendData(s,(char*)&sc7,sizeof(sc7));

			//转发创建房间
			_S_C_GP8 sc8;
			strcpy_s(sc8.room_name,32,cs5->room_name);
			strcpy_s(sc8.mima,32,cs5->mima);
			sc8.num = 1;
			sc8.FangJianHao = FangJianHao;
			sc8.ZhanDou = false;
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == 0)
					ts.SendData(it->first,(char*)&sc8,sizeof(sc8));
			}
			FangJianHao += 1;

		}break;
	case _SEND_WALK://玩家移动
		{
			_C_S_GP4* csgp4 = (_C_S_GP4*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);
			if( ik == allPLAYER.end() )
				return;
			ik->second.image = csgp4->image;
			ik->second.x = csgp4->x;
			ik->second.y = csgp4->y;
			ik->second.state = csgp4->state;
			ik->second.bMove = csgp4->bMove;
			ik->second.bMovedir = csgp4->MoveDir;
	
			//转发移动
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					_S_C_GP6 csgp6;
					csgp6.pyr = ik->second;
					ts.SendData(it->first,(char*)&csgp6,sizeof(csgp6));
				}
			}

		}break;
	case _SEND_ENTER_TOWN://玩家进入城镇
		{
			_C_S_GP13* csgp13 = (_C_S_GP13*)data;
			if(!csgp13->fanhui)
			{
					if(!csgp13->pyr.FangZhu)
					{
						//设置房间人数
						std::map<int,FangJianXinXi>::iterator ij = FJXX.find(csgp13->pyr.scene);

						if( ij == FJXX.end() )
							return;

						ij->second.PYRnum += 1;
						//房间人数发生变化，发给客户端
						_S_C_GP29 sc29;
						sc29.fangjianhao = csgp13->pyr.scene;
						sc29.renshu = ij->second.PYRnum;
						for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
						{
							if(it->second.scene == 0)
							{
								ts.SendData(it->first,(char*)&sc29,sizeof(sc29));
							}
						}
					}

					//设置玩家属性
					std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

					if(ik == allPLAYER.end())
						return;

					ik->second.hp = csgp13->pyr.hp;
					ik->second.image = csgp13->pyr.image;
					ik->second.mp = csgp13->pyr.mp;
					ik->second.PicBegin = csgp13->pyr.PicBegin;
					ik->second.PicEnd = csgp13->pyr.PicEnd;
					ik->second.x = csgp13->pyr.x;
					ik->second.y = csgp13->pyr.y;
					ik->second.FangZhu = csgp13->pyr.FangZhu;
					ik->second.ZhunBei = csgp13->pyr.ZhunBei;
					ik->second.scene = csgp13->pyr.scene;
					int len = 0;
					while(csgp13->pyr.name[len])
						len++;
					for(int i = 0;i < len + 1; ++i)
						ik->second.name[i] = csgp13->pyr.name[i];

					//返回玩家数据
					_S_C_GP23 scgp23;

					int j = 0;
					for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
					{
						if(it->first != s && it->second.scene == csgp13->pyr.scene)
						{
							scgp23.pyr[j++] = it->second;
						}
					}
					scgp23.num = j;
					if(scgp23.num >= 1)
						ts.SendData(s,(char*)&scgp23,sizeof(scgp23));

					//新玩家进入转发
					for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
					{
						if(it->second.scene == csgp13->pyr.scene && it->first != s )
						{ 
							_S_C_GP22 scgp22;//转发玩家进入消息
							scgp22.pyr = ik->second;
							ts.SendData(it->first,(char*)&scgp22,sizeof(scgp22));
						}
					}
			}
			else
			{
				//返回玩家数据
				_S_C_GP34 sc34;
				sc34.num = 0;
				for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
				{
					if(it->second.scene == csgp13->pyr.scene)
					{
						sc34.pyr[sc34.num++] = it->second;
					}
				}
			
				if(sc34.num >= 1)
					ts.SendData(s,(char*)&sc34,sizeof(sc34));


			}


		}break;
	case _SEND_TALK://聊天
		{
			_C_S_GP3* p = (_C_S_GP3*)data;
			//获取玩家
			std::map<SOCKET,PLAYER>::iterator it = allPLAYER.find(s);

			if(it == allPLAYER.end())
				return;

			//转发
			_S_C_GP5 scgp5;
			strcpy_s(scgp5.str,128,p->str);
			scgp5.scene = p->scene;

			for(std::map<SOCKET,PLAYER>::iterator it1 = allPLAYER.begin();it1 != allPLAYER.end(); ++it1 )
			{
				//判断是否在同一场景
				if(it1->second.scene == scgp5.scene && it1->first != s)
				{
					ts.SendData(it1->first,(char*)&scgp5,sizeof(scgp5));
				}
			}


		}break;
	case _SEND_PREPARE://玩家准备
		{
			_C_S_GP6* cs6 = (_C_S_GP6*)data;
			//获取玩家
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);
			if(ik == allPLAYER.end())
				return;
			ik->second.ZhunBei = cs6->prepare;
			ik->second.FangZhu = cs6->master;
			ik->second.scene = cs6->scene;

			//转发
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					//发送转发准备协议
					_S_C_GP9 sc9;
					sc9.prepare = ik->second.ZhunBei;
					strcpy_s(sc9.name,32,ik->second.name);
					ts.SendData(it->first,(char*)&sc9,sizeof(sc9));
				}
			}

		}break;
	case _FANG_ZHU_ZHUN_BEI://房主准备
		{
			_C_S_GP15* cs15 = (_C_S_GP15*)data;

			//开始决斗
			_S_C_GP25 sc25;
			int i = 0;
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->second.scene == cs15->scene)
				{
					sc25.pyr[i] = it->second;
					sc25.pyr[i].x = i * 200;
					sc25.pyr[i].y = 260;
					i += 1;
				}
			}
			sc25.num = i;

			//设置是否为战斗状态
			std::map<int,FangJianXinXi>::iterator ik = FJXX.find(cs15->scene);

			if(ik == FJXX.end())
				return;

			ik->second.ZhanDou = true;

			//发送进入决斗协议
			_S_C_GP27 sc27;
			sc27.fangjianhao = cs15->scene;
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->second.scene == 	cs15->scene)//在同一场景
					ts.SendData(it->first,(char*)&sc25,sizeof(sc25));
				else if(it->second.scene == 0)//发给选择房间界面的玩家告诉他门房间暂时不能进入
					ts.SendData(it->first,(char*)&sc27,sizeof(sc27));
			}

		}break;
	case _ENTER_XUE_ZHE_FANG_JIAN://玩家进入选择房间界面
		{
			//获取玩家
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;

			ik->second.scene = 0;

			_S_C_GP26 sc26;
			sc26.num = 0;
			for(std::map<int,FangJianXinXi>::iterator it = FJXX.begin(); it != FJXX.end(); ++it)
			{
				sc26.fjxx[sc26.num++] = it->second;
			}

			if(sc26.num >= 1)
				ts.SendData(s,(char*)&sc26,sizeof(sc26));

		}break;
	case _SEND_QUIT_ROOM://玩家退出房间
		{
			_C_S_GP10* cs10 = (_C_S_GP10*)data;
			std::map<int,FangJianXinXi>::iterator ij = FJXX.find(cs10->room_index);

			if(ij == FJXX.end())
				return;
			ij->second.PYRnum -= 1;
			//发送房间信息更新协议
			_S_C_GP29 sc29;
			sc29.fangjianhao = ij->second.FangJianHao;
			sc29.renshu = ij->second.PYRnum;

			//房间人数为1，删除房间
			if(ij->second.PYRnum <= 0)
			{
				FJXX.erase(ij);
				//获取玩家
				std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

				if(ik == allPLAYER.end())
					return;

				ik->second.scene = 0;

				_S_C_GP26 sc26;
				sc26.num = 0;
				for(std::map<int,FangJianXinXi>::iterator it = FJXX.begin(); it != FJXX.end(); ++it)
				{
					sc26.fjxx[sc26.num++] = it->second;
				}

				for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
				{
					if(it->second.scene == 0)
						ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//发送更新房间信息
				}

			}
			if(cs10->fangzhu)//是否是房主退出房间
			{
				_S_C_GP30 sc30;
				int rd = rand() % (int)allPLAYER.size();
				int js = 1;

				std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

				if(ik == allPLAYER.end())
					return;

				ik->second.scene = 0;

				for(std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.begin(); ik != allPLAYER.end(); ++ik)
				{
					if(ik->second.scene == cs10->room_index)
					{
						if(js == rd)
						{
							strcpy_s(sc30.name,32,ik->second.name);
							ik->second.FangZhu = true;
							break;
						}
						js += 1;
					}
				}

				for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
				{
					if(it->second.scene == cs10->room_index)
					{
						ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//发送房主变更消息
					}
				}
			}


			//转发退出房间消息
			_S_C_GP16 sc16;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;

			ik->second.scene = 0;
			sc16.scene = cs10->room_index;
			strcpy_s(sc16.name,32,ik->second.name);

			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->second.scene == 	cs10->room_index && it->first != s)//在同一场景
					ts.SendData(it->first,(char*)&sc16,sizeof(sc16));//转发退出消息
				else if(it->second.scene == 0)
					ts.SendData(it->first,(char*)&sc29,sizeof(sc29));//发送更新房间信息
			}
		}break;
	case _DAI_JI_ZHUANG_TAI://待机状态
		{
			_C_S_GP17* cs17 = (_C_S_GP17*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;
			ik->second.image = cs17->image;
			ik->second.state = cs17->state;
			ik->second.bStand = cs17->bStand;

			//转发待机状态
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					_S_C_GP31 csgp31;
					csgp31.pyr = ik->second;
					ts.SendData(it->first,(char*)&csgp31,sizeof(csgp31));
				}
			}
		}break;
	case _SEND_YI_DONG_SHE_JI://进入移动射击状态
		{
			_C_S_GP7* cs7 = (_C_S_GP7*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;
			ik->second.bShooting_move = cs7->bShooting_move;
			ik->second.image = cs7->image;
			ik->second.state = cs7->state;

			//转发进入移动射击状态
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					_S_C_GP11 csgp11;
					csgp11.pyr = ik->second;
					ts.SendData(it->first,(char*)&csgp11,sizeof(csgp11));
				}
			}

		}break;
	case _YI_DONG_SHE_JI_DIR://移动射击变换方向
		{
			_C_S_GP100* cs100 = (_C_S_GP100*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;

			ik->second.image = cs100->dir;

			//转发
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					_S_C_GP100 sc100;
					sc100.dir = cs100->dir;
					strcpy_s(sc100.name,32,ik->second.name);
					ts.SendData(it->first,(char*)&sc100,sizeof(sc100));
				}
			}

		}break;
	case _SEND_YDSJ_MOVE://移动射击状态移动
		{
			_C_S_GP8* cs8 = (_C_S_GP8*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;
			ik->second.x = cs8->x;
			ik->second.y = cs8->y;
			ik->second.state = cs8->state;
			ik->second.image = cs8->image;
			ik->second.bShooting_move1 = cs8->bShooting_move1;

			//转发
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					_S_C_GP13 sc13;
					sc13.pyr = ik->second;
					ts.SendData(it->first,(char*)&sc13,sizeof(sc13));
				}
			}
		}break;
	case _YDSJ_GONG_JI://移动射击状态攻击
		{
			_C_S_GP18* cs18 = (_C_S_GP18*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);
			if(ik == allPLAYER.end())
				return;
			ik->second.bShooting_move2 = cs18->bShooting_move2;
			ik->second.image = cs18->image;
			ik->second.state = cs18->state;
			
			//转发
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					_S_C_GP32 sc32;
					sc32.pyr = ik->second;
					ts.SendData(it->first,(char*)&sc32,sizeof(sc32));
				}
			}
		}break;
	case _YDSJ_DA_ZHONG_REN://移动射击打中人
		{
			_C_S_GP12* cs12 = (_C_S_GP12*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);
			if(ik == allPLAYER.end())
				return;
			ik->second.hram = cs12->hram;
			ik->second.image = cs12->image;
			ik->second.JiangZhiDU = cs12->JiangZhiDU;
			ik->second.JiangZhiZT = cs12->JiangZhiZT;
			ik->second.state = cs12->state;
			ik->second.x = cs12->x;
			ik->second.y = cs12->y;
			ik->second.hp = ik->second.hp;
			

			//转发
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					_S_C_GP20 sc20;
					sc20.pyr = ik->second;
					strcpy_s(sc20.pyr.name,32,cs12->name);
					ts.SendData(it->first,(char*)&sc20,sizeof(sc20));
				}
			}

		}break;
	case _GAME_END://比赛结束
		{
			_C_S_GP19* cs19 = (_C_S_GP19*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;

			std::map<int,FangJianXinXi>::iterator ij = FJXX.find(ik->second.scene);

			if(ij == FJXX.end())
				return;

			ij->second.ZhanDou = false;
			//ij->second.PYRnum = 0;

			//发送房间可以进入信息
			_S_C_GP28 sc28;
			sc28.fangjianhao = ik->second.scene;

			//转发
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end();++it)
			{
				if(it->second.scene == ik->second.scene)
				{
					_S_C_GP33 sc33;
					ts.SendData(it->first,(char*)&sc33,sizeof(sc33));

					it->second.hp = 400000;
					it->second.mp = 400000;
					it->second.state = _HERO_STAND;
					it->second.bStand = true;
					it->second.x = rand() % 500;
					it->second.y = rand() % 300;
				}
				else if(it->second.scene == 0)
				{
					ts.SendData(it->first,(char*)&sc28,sizeof(sc28));
				}
			}
		}
	}
}


void main()
{
	srand((unsigned int)time(0));
	ts.Init(12345,256,10,10);



	//业务循环
	while(ts.Continue())
	{
		////同步数据
		//if(allPLAYER.size() >= 2)
		//{
		//	_S_C_GP24 sc24;
		//	int i = 0;
		//	for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
		//	{
		//		if(it->second.scene == 0)
		//			sc24.pyr[i++] = it->second;
		//	}
		//	sc24.num = i;
		//	for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
		//	{
		//		if(it->second.scene == 0)
		//			ts.SendData(it->first,(char*)&sc24,sizeof(sc24));
		//	}
		//}
		ts.Run();
		//获取网络事件
		_TS_NET_EVENT tne;
		while(ts.GetTSNetEvent(&tne))
		{
			char ip[32];
			ts.GetStrIP(tne.ip,ip);
			switch(tne.type)
			{
			case 0:
				{
					//设置玩家数据
					PLAYER player = {};
					player.scene = -1;
					allPLAYER.insert(std::pair<SOCKET,PLAYER>(tne.s,player));
					std::cout<<ip<<"连接进来了"<<std::endl;
				}
				break;
			case 1:
				{

					std::cout<<"服务器主动断开了和"<<ip<<"的连接"<<std::endl;

					std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(tne.s);
					if(ik == allPLAYER.end())
						return;

					_S_C_GP19 sc19;
					strcpy_s(sc19.name,32,ik->second.name);
					int scene = ik->second.scene;

					if(ik->second.scene != -1 && ik->second.scene != 0)
					{
						std::map<int,FangJianXinXi>::iterator ij = FJXX.find(ik->second.scene);

						if(ij == FJXX.end())
							return;

						ij->second.PYRnum -= 1;
						if(ij->second.PYRnum == 1 && ij->second.ZhanDou)
						{
							ij->second.ZhanDou = false;
						}
						//发送房间信息更新协议
						_S_C_GP29 sc29;
						sc29.fangjianhao = ij->second.FangJianHao;
						sc29.renshu = ij->second.PYRnum;

						//房间人数为1，删除房间
						if(ij->second.PYRnum <= 0)
						{
							FJXX.erase(ij);

							_S_C_GP26 sc26;
							sc26.num = 0;
							for(std::map<int,FangJianXinXi>::iterator it = FJXX.begin(); it != FJXX.end(); ++it)
							{
								sc26.fjxx[sc26.num++] = it->second;
							}

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
							{
								if(it->second.scene == 0)
									ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//发送更新房间信息
							}

						}
						if(ik->second.FangZhu)//是否是房主退出房间
						{
							_S_C_GP30 sc30;
							int rd = rand() % (int)allPLAYER.size();
							int js = 1;

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin(); it != allPLAYER.end(); ++it)
							{
								if(it->second.scene == ik->second.scene)
								{
									if(js == rd)
									{
										strcpy_s(sc30.name,32,it->second.name);
										it->second.FangZhu = true;
										break;
									}
									js += 1;
								}
							}

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
							{
								if(it->second.scene == ik->second.scene)
								{
									ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//发送房主变更消息
								}
							}
						}
					}




					for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
					{
						if(it->second.scene == scene)
						{
							ts.SendData(it->first,(char*)&sc19,sizeof(sc19));
						}
					}
					allPLAYER.erase(ik);
				}
				break;
			case 2:
				{
					std::cout<<ip<<"主动断开了和服务器的连接"<<std::endl;
					std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(tne.s);
					if(ik == allPLAYER.end())
						return;

					if(ik->second.scene != -1 && ik->second.scene != 0)
					{
						std::map<int,FangJianXinXi>::iterator ij = FJXX.find(ik->second.scene);

						if(ij == FJXX.end())
							return;


						ij->second.PYRnum -= 1;
						if(ij->second.PYRnum == 1 && ij->second.ZhanDou)
						{
							ij->second.ZhanDou = false;
						}
						//发送房间信息更新协议
						_S_C_GP29 sc29;
						sc29.fangjianhao = ij->second.FangJianHao;
						sc29.renshu = ij->second.PYRnum;

						//房间人数为1，删除房间
						if(ij->second.PYRnum <= 0)
						{
							FJXX.erase(ij);

							_S_C_GP26 sc26;
							sc26.num = 0;
							for(std::map<int,FangJianXinXi>::iterator it = FJXX.begin(); it != FJXX.end(); ++it)
							{
								sc26.fjxx[sc26.num++] = it->second;
							}

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
							{
								if(it->second.scene == 0)
									ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//发送更新房间信息
							}

						}
						if(ik->second.FangZhu)//是否是房主退出房间
						{
							_S_C_GP30 sc30;
							int rd = rand() % (int)allPLAYER.size();
							int js = 1;

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin(); it != allPLAYER.end(); ++it)
							{
								if(it->second.scene == ik->second.scene)
								{
									if(js == rd)
									{
										strcpy_s(sc30.name,32,it->second.name);
										it->second.FangZhu = true;
										break;
									}
									js += 1;
								}
							}

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
							{
								if(it->second.scene == ik->second.scene)
								{
									ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//发送房主变更消息
								}
							}
						}
					}


					_S_C_GP19 sc19;
					strcpy_s(sc19.name,32,ik->second.name);
					int scene = ik->second.scene;



					for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
					{
						if(it->second.scene == scene)
						{
							ts.SendData(it->first,(char*)&sc19,sizeof(sc19));
						}
					}
					allPLAYER.erase(ik);
				}break;
			case 3:
				{
					std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(tne.s);
					if(ik == allPLAYER.end())
						return;

					if(ik->second.scene != -1 && ik->second.scene != 0)
					{
						std::map<int,FangJianXinXi>::iterator ij = FJXX.find(ik->second.scene);
						if(ij == FJXX.end())
							return;
						ij->second.PYRnum -= 1;
						if(ij->second.PYRnum == 1 && ij->second.ZhanDou)
						{
							ij->second.ZhanDou = false;
						}
						//发送房间信息更新协议
						_S_C_GP29 sc29;
						sc29.fangjianhao = ij->second.FangJianHao;
						sc29.renshu = ij->second.PYRnum;

						//房间人数为1，删除房间
						if(ij->second.PYRnum <= 0)
						{
							FJXX.erase(ij);
							//获取玩家

							_S_C_GP26 sc26;
							sc26.num = 0;
							for(std::map<int,FangJianXinXi>::iterator it = FJXX.begin(); it != FJXX.end(); ++it)
							{
								sc26.fjxx[sc26.num++] = it->second;
							}

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
							{
								if(it->second.scene == 0)
									ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//发送更新房间信息
							}

						}
						if(ik->second.FangZhu)//是否是房主退出房间
						{
							_S_C_GP30 sc30;
							int rd = rand() % (int)allPLAYER.size();
							int js = 1;

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin(); it != allPLAYER.end(); ++it)
							{
								if(it->second.scene == ik->second.scene)
								{
									if(js == rd)
									{
										strcpy_s(sc30.name,32,it->second.name);
										it->second.FangZhu = true;
										break;
									}
									js += 1;
								}
							}

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
							{
								if(it->second.scene == ik->second.scene)
								{
									ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//发送房主变更消息
								}
							}
						}
					}


					_S_C_GP19 sc19;
					strcpy_s(sc19.name,32,ik->second.name);
					int scene = ik->second.scene;



					for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
					{
						if(it->second.scene == scene)
						{
							ts.SendData(it->first,(char*)&sc19,sizeof(sc19));
						}
					}
					allPLAYER.erase(ik);

					std::cout<<"服务器因为错误断开了和"<<ip<<"的连接"<<std::endl;
				}
				break;
			case 4:
				{
					NetData(tne.s,tne.data,tne.len);
					std::cout<<"服务器接收到"<<tne.len<<"字节的数据"<<std::endl;
				}
				break;
			}
			ts.ReleaseTSNetEvent(&tne);
		}
		if(GetAsyncKeyState(VK_ESCAPE) & 1)
			ts.Stop();
		if(GetAsyncKeyState(VK_RETURN) & 1)
			std::cout<<ts.GetUseMemoryMB()<<std::endl;
		Sleep(1);
	}
	ts.End();

	system("pause");
}
