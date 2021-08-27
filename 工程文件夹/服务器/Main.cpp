#include <iostream>
#include "TcpServer.h"
#include <windows.h>
#include "DNFprotocol.h"
#include <map>
#include <time.h>
#include <string>
#include <vector>

int FangJianHao = 1;

//Ӣ�۵�״̬
enum HeroState : char 
{
	_HERO_STAND,//վ��
	_HERO_MOVE_,//�ƶ�
	_HERO_MOVE_SHOOTING_MOVE,//�ƶ����״̬���ƶ�
	_HERO_MOVE_SHOOT,//�ƶ����
	_HERO_MOVE_ATTACK,//�ƶ��������
	_HERO_END_MOVESHOOTINGK,//ֹͣ�ƶ����
	_HERO_MOVE_SHOOTING_DIR,//�任�ƶ��������
	_HERO_RUSH,//����
	_HERO_WEI_XING_SHE_XIAN,//��������
	_HERO_JIANG_ZHI,//��ֱ״̬
	_HERO_DEATH,//����״̬
	_HERO_PUTONGGONGJI//��ͨ����
};

CTcpServer ts;//������

std::map<SOCKET,PLAYER>  allPLAYER;//���
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
			//����ǳƵĺϷ���
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
				//�Ϸ�
				_S_C_GP1 scgp1;
				ts.SendData(s,(char*)&scgp1,sizeof(scgp1));

			}
			else
			{
				//�Ƿ�
				_S_C_GP2 scgp2;
				ts.SendData(s,(char*)&scgp2,sizeof(scgp2));
			}

		}break;
	case _SEND_FOUND_ROOM://��Ҵ�������
		{
			//���÷�������
			_C_S_GP5* cs5 = (_C_S_GP5*)data;
			FangJianXinXi fjxx;
			fjxx.FangJianHao = FangJianHao;
			strcpy_s(fjxx.mima,32,cs5->mima);
			strcpy_s(fjxx.room_name,32,cs5->room_name);
			fjxx.PYRnum = 1;
			fjxx.ZhanDou = false;
			FJXX.insert(std::pair<int,FangJianXinXi>(fjxx.FangJianHao,fjxx));

			//���ش�������
			_S_C_GP7 sc7;
			sc7.room_index = FangJianHao;
			ts.SendData(s,(char*)&sc7,sizeof(sc7));

			//ת����������
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
	case _SEND_WALK://����ƶ�
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
	
			//ת���ƶ�
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
	case _SEND_ENTER_TOWN://��ҽ������
		{
			_C_S_GP13* csgp13 = (_C_S_GP13*)data;
			if(!csgp13->fanhui)
			{
					if(!csgp13->pyr.FangZhu)
					{
						//���÷�������
						std::map<int,FangJianXinXi>::iterator ij = FJXX.find(csgp13->pyr.scene);

						if( ij == FJXX.end() )
							return;

						ij->second.PYRnum += 1;
						//�������������仯�������ͻ���
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

					//�����������
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

					//�����������
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

					//����ҽ���ת��
					for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
					{
						if(it->second.scene == csgp13->pyr.scene && it->first != s )
						{ 
							_S_C_GP22 scgp22;//ת����ҽ�����Ϣ
							scgp22.pyr = ik->second;
							ts.SendData(it->first,(char*)&scgp22,sizeof(scgp22));
						}
					}
			}
			else
			{
				//�����������
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
	case _SEND_TALK://����
		{
			_C_S_GP3* p = (_C_S_GP3*)data;
			//��ȡ���
			std::map<SOCKET,PLAYER>::iterator it = allPLAYER.find(s);

			if(it == allPLAYER.end())
				return;

			//ת��
			_S_C_GP5 scgp5;
			strcpy_s(scgp5.str,128,p->str);
			scgp5.scene = p->scene;

			for(std::map<SOCKET,PLAYER>::iterator it1 = allPLAYER.begin();it1 != allPLAYER.end(); ++it1 )
			{
				//�ж��Ƿ���ͬһ����
				if(it1->second.scene == scgp5.scene && it1->first != s)
				{
					ts.SendData(it1->first,(char*)&scgp5,sizeof(scgp5));
				}
			}


		}break;
	case _SEND_PREPARE://���׼��
		{
			_C_S_GP6* cs6 = (_C_S_GP6*)data;
			//��ȡ���
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);
			if(ik == allPLAYER.end())
				return;
			ik->second.ZhunBei = cs6->prepare;
			ik->second.FangZhu = cs6->master;
			ik->second.scene = cs6->scene;

			//ת��
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->first != s && it->second.scene == ik->second.scene)
				{
					//����ת��׼��Э��
					_S_C_GP9 sc9;
					sc9.prepare = ik->second.ZhunBei;
					strcpy_s(sc9.name,32,ik->second.name);
					ts.SendData(it->first,(char*)&sc9,sizeof(sc9));
				}
			}

		}break;
	case _FANG_ZHU_ZHUN_BEI://����׼��
		{
			_C_S_GP15* cs15 = (_C_S_GP15*)data;

			//��ʼ����
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

			//�����Ƿ�Ϊս��״̬
			std::map<int,FangJianXinXi>::iterator ik = FJXX.find(cs15->scene);

			if(ik == FJXX.end())
				return;

			ik->second.ZhanDou = true;

			//���ͽ������Э��
			_S_C_GP27 sc27;
			sc27.fangjianhao = cs15->scene;
			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->second.scene == 	cs15->scene)//��ͬһ����
					ts.SendData(it->first,(char*)&sc25,sizeof(sc25));
				else if(it->second.scene == 0)//����ѡ�񷿼�������Ҹ������ŷ�����ʱ���ܽ���
					ts.SendData(it->first,(char*)&sc27,sizeof(sc27));
			}

		}break;
	case _ENTER_XUE_ZHE_FANG_JIAN://��ҽ���ѡ�񷿼����
		{
			//��ȡ���
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
	case _SEND_QUIT_ROOM://����˳�����
		{
			_C_S_GP10* cs10 = (_C_S_GP10*)data;
			std::map<int,FangJianXinXi>::iterator ij = FJXX.find(cs10->room_index);

			if(ij == FJXX.end())
				return;
			ij->second.PYRnum -= 1;
			//���ͷ�����Ϣ����Э��
			_S_C_GP29 sc29;
			sc29.fangjianhao = ij->second.FangJianHao;
			sc29.renshu = ij->second.PYRnum;

			//��������Ϊ1��ɾ������
			if(ij->second.PYRnum <= 0)
			{
				FJXX.erase(ij);
				//��ȡ���
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
						ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//���͸��·�����Ϣ
				}

			}
			if(cs10->fangzhu)//�Ƿ��Ƿ����˳�����
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
						ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//���ͷ��������Ϣ
					}
				}
			}


			//ת���˳�������Ϣ
			_S_C_GP16 sc16;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;

			ik->second.scene = 0;
			sc16.scene = cs10->room_index;
			strcpy_s(sc16.name,32,ik->second.name);

			for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
			{
				if(it->second.scene == 	cs10->room_index && it->first != s)//��ͬһ����
					ts.SendData(it->first,(char*)&sc16,sizeof(sc16));//ת���˳���Ϣ
				else if(it->second.scene == 0)
					ts.SendData(it->first,(char*)&sc29,sizeof(sc29));//���͸��·�����Ϣ
			}
		}break;
	case _DAI_JI_ZHUANG_TAI://����״̬
		{
			_C_S_GP17* cs17 = (_C_S_GP17*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;
			ik->second.image = cs17->image;
			ik->second.state = cs17->state;
			ik->second.bStand = cs17->bStand;

			//ת������״̬
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
	case _SEND_YI_DONG_SHE_JI://�����ƶ����״̬
		{
			_C_S_GP7* cs7 = (_C_S_GP7*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;
			ik->second.bShooting_move = cs7->bShooting_move;
			ik->second.image = cs7->image;
			ik->second.state = cs7->state;

			//ת�������ƶ����״̬
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
	case _YI_DONG_SHE_JI_DIR://�ƶ�����任����
		{
			_C_S_GP100* cs100 = (_C_S_GP100*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);

			if(ik == allPLAYER.end())
				return;

			ik->second.image = cs100->dir;

			//ת��
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
	case _SEND_YDSJ_MOVE://�ƶ����״̬�ƶ�
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

			//ת��
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
	case _YDSJ_GONG_JI://�ƶ����״̬����
		{
			_C_S_GP18* cs18 = (_C_S_GP18*)data;
			std::map<SOCKET,PLAYER>::iterator ik = allPLAYER.find(s);
			if(ik == allPLAYER.end())
				return;
			ik->second.bShooting_move2 = cs18->bShooting_move2;
			ik->second.image = cs18->image;
			ik->second.state = cs18->state;
			
			//ת��
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
	case _YDSJ_DA_ZHONG_REN://�ƶ����������
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
			

			//ת��
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
	case _GAME_END://��������
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

			//���ͷ�����Խ�����Ϣ
			_S_C_GP28 sc28;
			sc28.fangjianhao = ik->second.scene;

			//ת��
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



	//ҵ��ѭ��
	while(ts.Continue())
	{
		////ͬ������
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
		//��ȡ�����¼�
		_TS_NET_EVENT tne;
		while(ts.GetTSNetEvent(&tne))
		{
			char ip[32];
			ts.GetStrIP(tne.ip,ip);
			switch(tne.type)
			{
			case 0:
				{
					//�����������
					PLAYER player = {};
					player.scene = -1;
					allPLAYER.insert(std::pair<SOCKET,PLAYER>(tne.s,player));
					std::cout<<ip<<"���ӽ�����"<<std::endl;
				}
				break;
			case 1:
				{

					std::cout<<"�����������Ͽ��˺�"<<ip<<"������"<<std::endl;

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
						//���ͷ�����Ϣ����Э��
						_S_C_GP29 sc29;
						sc29.fangjianhao = ij->second.FangJianHao;
						sc29.renshu = ij->second.PYRnum;

						//��������Ϊ1��ɾ������
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
									ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//���͸��·�����Ϣ
							}

						}
						if(ik->second.FangZhu)//�Ƿ��Ƿ����˳�����
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
									ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//���ͷ��������Ϣ
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
					std::cout<<ip<<"�����Ͽ��˺ͷ�����������"<<std::endl;
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
						//���ͷ�����Ϣ����Э��
						_S_C_GP29 sc29;
						sc29.fangjianhao = ij->second.FangJianHao;
						sc29.renshu = ij->second.PYRnum;

						//��������Ϊ1��ɾ������
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
									ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//���͸��·�����Ϣ
							}

						}
						if(ik->second.FangZhu)//�Ƿ��Ƿ����˳�����
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
									ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//���ͷ��������Ϣ
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
						//���ͷ�����Ϣ����Э��
						_S_C_GP29 sc29;
						sc29.fangjianhao = ij->second.FangJianHao;
						sc29.renshu = ij->second.PYRnum;

						//��������Ϊ1��ɾ������
						if(ij->second.PYRnum <= 0)
						{
							FJXX.erase(ij);
							//��ȡ���

							_S_C_GP26 sc26;
							sc26.num = 0;
							for(std::map<int,FangJianXinXi>::iterator it = FJXX.begin(); it != FJXX.end(); ++it)
							{
								sc26.fjxx[sc26.num++] = it->second;
							}

							for(std::map<SOCKET,PLAYER>::iterator it = allPLAYER.begin();it != allPLAYER.end(); ++it )
							{
								if(it->second.scene == 0)
									ts.SendData(it->first,(char*)&sc26,sizeof(sc26));//���͸��·�����Ϣ
							}

						}
						if(ik->second.FangZhu)//�Ƿ��Ƿ����˳�����
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
									ts.SendData(it->first,(char*)&sc30,sizeof(sc30));//���ͷ��������Ϣ
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

					std::cout<<"��������Ϊ����Ͽ��˺�"<<ip<<"������"<<std::endl;
				}
				break;
			case 4:
				{
					NetData(tne.s,tne.data,tne.len);
					std::cout<<"���������յ�"<<tne.len<<"�ֽڵ�����"<<std::endl;
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
