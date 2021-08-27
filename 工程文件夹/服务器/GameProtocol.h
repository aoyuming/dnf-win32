#ifndef _GAME_PROTOCOL_H_
#define _GAME_PROTOCOL_H_

#define _TRY_NICK_NAME			1//(�ͻ���-������)����ǳ�
#define _NICK_NAME_LAWLESS      2//(������-�ͻ���)�ǳƷǷ�
#define _NICK_NAME_LEGALITY     3//(������-�ͻ���)�ǳƺϷ�
#define _NEW_PLAYER_ENTER		4//(������(Ⱥ)-�ͻ���)����ҽ���

#define _SEND_TALK				5//(�ͻ���-������)����
#define _PLAYER_TALK			6//(������(Ⱥ)-�ͻ���)����

#define _SEND_WALK				7//(�ͻ���-������)�ƶ�
#define _PLAYER_WALK			8//(������(Ⱥ)-�ͻ���)�ƶ�

#define _PLAYER_QUIT			9//(������(Ⱥ)-�ͻ���)�˳�


struct _GAME_PROTOCOL
{
	int id;//Э��ID
};
//����ǳ�
struct _C_S_GP1 : public _GAME_PROTOCOL
{
	_C_S_GP1()
	{
		id = _TRY_NICK_NAME;
	}
	char nick_name[32];
};
//�ǳƷǷ�
struct _S_C_GP2 : public _GAME_PROTOCOL
{
	_S_C_GP2()
	{
		id = _NICK_NAME_LAWLESS;
	}
}; 

#define _GAME_MAP_W			32//��
#define _GAME_MAP_H			32//��
#define _GAME_MAP_S			(_GAME_MAP_W * _GAME_MAP_H)//��ͼ�ܴ�С

#define _ZA					1//�ϰ�
#define _TL					0//ͨ·

#define _MAX_PLAYER			32//��������

//��ҽṹ��
struct _PLAYER
{
	char nick_name[32];
	int x,y;
};

//�ǳƺϷ�
struct _S_C_GP3 : public _GAME_PROTOCOL
{
	_S_C_GP3()
	{
		id = _NICK_NAME_LEGALITY;
	}
	char map[_GAME_MAP_S];//��ͼ����
	int player_num;//��ǰ�������
	_PLAYER player[_MAX_PLAYER];//�������
};

//����ҽ�����Ϣ��ת��
struct _S_C_GP4 : public _GAME_PROTOCOL
{
	_S_C_GP4()
	{
		id = _NEW_PLAYER_ENTER;
	}
	_PLAYER new_player;
};

//����(�ͻ���-������)
struct _C_S_GP5 : public _GAME_PROTOCOL
{
	_C_S_GP5()
	{
		id = _SEND_TALK;
	}
	char talk[256];//���˵�Ļ�
};

//����(������(Ⱥ)-�ͻ���)
struct _S_C_GP6 : public _GAME_PROTOCOL
{
	_S_C_GP6()
	{
		id = _PLAYER_TALK;
	}
	char talk[256];//�����˵�Ļ��Ļ����ϼ����ǳ�
};


//λ��(�ͻ���-������)
struct _C_S_GP7 : public _GAME_PROTOCOL
{
	_C_S_GP7()
	{
		id = _SEND_WALK;
	}
	int x,y;//λ��
};

//λ��(������(Ⱥ)-�ͻ���)
struct _S_C_GP8 : public _GAME_PROTOCOL
{
	_S_C_GP8()
	{
		id = _PLAYER_WALK;
	}
	_PLAYER player;
};

//����˳�(������(Ⱥ)-�ͻ���)
struct _S_C_GP9 : public _GAME_PROTOCOL
{
	_S_C_GP9()
	{
		id = _PLAYER_QUIT;
	}
	char nick_name[32];
};


#endif