#ifndef _DNF_PROTOCOL_H
#define _DNF_PROTOCOL_H

#define _TRY_NICK_NAME				 1//(�ͻ���-������)����ǳ�
#define _NICK_NAME_LAWLESS			 2//(������-�ͻ���)�ǳƷǷ�
#define _NICK_NAME_LEGALITY			 3//(������-�ͻ���)�ǳƺϷ�

#define _SEND_TALK					 4//(�ͻ���-������)����
#define _PLAYER_TALK				 5//(������(Ⱥ)-�ͻ���)����

#define _SEND_CHOICE_ROLE			 6//(�ͻ���-������)ѡ���ɫ
#define	_RETURN_CHOICE_ROLE			 7//(������-�ͻ���)������һ������
#define _NEW_PLAYER_CHOICE_ROLE	     8//(������(Ⱥ)-�ͻ���)�½�ɫ����

#define _SEND_WALK					10//(�ͻ���-������)�ƶ�
#define _PLAYER_WALK				11//(������(Ⱥ)-�ͻ���)�ƶ�

#define _SEND_FOUND_ROOM			12//(�ͻ���-������)��������
#define _RETURN_FOUND_ROOM			13//(�ͻ���-������)���ش�������
#define _PLAYER_FOUND_ROOM		    14//(������(Ⱥ)-�ͻ���)��Ҵ�������

#define _SEND_PREPARE				15//(�ͻ���-������)׼��
#define	_PLAYER_PREPARE				16//(������(Ⱥ)-�ͻ���)ת��׼��
#define	_BEGIN_DULE					17//(������(Ⱥ)-�ͻ���)��ʼ����

#define _SEND_YI_DONG_SHE_JI		18//(�ͻ���-������)���������ƶ����(����)״̬
#define _PLAYER_YI_DONG_SHE_JI		19//(������(Ⱥ)-�ͻ���)ת�������ƶ����(����)״̬

#define _YI_DONG_SHE_JI_DIR			20//(�ͻ���-������)�ƶ�����任����
#define _ZF_YI_DONG_SHE_JI_DIR		99//(������(Ⱥ)-�ͻ���)ת���ƶ�����任����

#define _SEND_YDSJ_MOVE				21//(�ͻ���-������)�ƶ����״̬���ƶ�
#define _ZF_YDSJ_MOVE				22//(������(Ⱥ)-�ͻ���)ת���ƶ����״̬�ƶ�

#define _SEND_DEATH					23//(�ͻ���-������)����
#define _PLAYER_DEATH				24//(������(Ⱥ)-�ͻ���)ת���������
#define _DULE_END					25//(������(Ⱥ)-�ͻ���)��������

#define _SEND_QUIT_ROOM				26//(�ͻ���-������)�˳�����
#define _PLAYER_QUIT_ROOM			27//(������(Ⱥ)-�ͻ���)ת������˳�����
#define _RETURN_SCENE				28//(�ͻ���-������)������һ������

#define _SEND_QUIT_GAME				29//(�ͻ���-������)�˳���Ϸ
#define _RETURN_QUIT_GAME			30//(������-�ͻ���)�˳���Ϸ
#define _PLAYER_QUIT_GAME			31//(������(Ⱥ)-�ͻ���)ת������˳���Ϸ

#define _YDSJ_DA_ZHONG_REN			32//(�ͻ���-������)�ƶ����������
#define _ZF_YDSJ_DA_ZHONG_REN       33//(������(Ⱥ)-�ͻ���)ת����ҽ��뷿��
#define _RETURN_ENTER_ROOM			34//(������-�ͻ���)���ط�����Ϣ���������Ϣ

#define _SEND_ENTER_TOWN			35//(�ͻ���-������)�������
#define _PLAYER_ENTER_TOWN			36 //(������(Ⱥ)-�ͻ���)ת����ҽ������
#define _RETURN_PLAYER_TOWN			37//(������-�ͻ���)����������ݸ��ͻ���

#define _SEND_TONG_BU				38//(�ͻ��˷�����)����ͬ����Ϣ
#define _RETURN_TONG_BU				39//(�����������ͻ���)ͬ���������

#define _JIN_RU_JUE_DOU				40//(�����������ͻ���)�����������
#define _SUO_YOU_PLAYER_ZHUN_BEI	41//(�����������ͻ���)�������׼��
#define _FANG_ZHU_ZHUN_BEI          42//(�ͻ��ˡ���������)����׼��

#define _ENTER_XUE_ZHE_FANG_JIAN	43//(�ͻ���_������)����ѡ�񷿼����
#define _RETURN_XUE_ZHE_FANG_JIAN	44//(������_�ͻ���)���ط������ݸ��ͻ���

#define _ROOM_NO_ENTER				45//(������-�ͻ���)������ʱ���ܽ���
#define _ROOM_OK_ENTER				46//(������_�ͻ���)������Խ���

//#define _ROOM_RENSHU_ZENGJIA		47//(������-�ͻ���)������������
//#define _ROOM_RENSHU_JIANSHAO		48//(������-�ͻ���)������������
#define _ROOM_RENSHU_BIAN_HUA		47//(������-�ͻ���)������������
#define _SET_FANG_ZHU				48//(������-�ͻ���)���÷���


#define _DAI_JI_ZHUANG_TAI			49//(�ͻ���-������)����״̬
#define _ZHUN_FA_DAI_JI				50//(������-�ͻ���)ת������״̬

#define _YDSJ_GONG_JI				51//(�ͻ���-������)�ƶ��������
#define _ZF_YDSJ_GONG_JI			52//(������-�ͻ���)ת���ƶ��������


#define _GAME_END					53//(�ͻ���-������)��������
#define	_ZF_GAME_END				54//(������-�ͻ���)ת����������

#define _FANHUI_PLY					55//(������-�ͻ���)�����������


//���η�Χ
struct _RECT
{
	int up;
	int down;
	int left;
	int right;
};

//��ҽṹ��
struct PLAYER
{
	int state;//״̬
	char name[32];//����
	int hp,mp;//Ѫ����
	int x,y;//����
	int PicBegin;//��ͼ��ʼ�±�
	int PicEnd;//��ͼ�����±�
	bool image;//����
	int role;//ʹ�õĽ�ɫ	
	int scene;//����
	bool ZhunBei;//�Ƿ�׼��
	bool FangZhu;//�Ƿ��Ƿ���
	bool bMove;//�ƶ�״̬
	bool bMovedir;//�ƶ�״̬
	bool bStand;//����״̬
	bool bShooting_move;//�����ƶ����״̬
	bool bShooting_move1;//�ƶ����״̬���ƶ�
	bool bShooting_move2;//�ƶ����״̬�¹���
	int JiangZhiDU;//��ֱ��
	bool JiangZhiZT;//��ֱ״̬
	int hram;//�����˺�
};

//������Ϣ�ṹ��
struct SKILL
{
	int x,y;//����
	int PicBegin;//��ͼ��ʼ�±�
	int PicEnd;//��ͼ�����±�
	_RECT r;//��ײ��Χ
	int harm;//�����˺�
	bool image;//����
};
//������Ϣ�ṹ��
struct FangJianXinXi
{
	int FangJianHao;
	char room_name[32];
	char mima[32];
	int PYRnum;
	int buttonI;
	bool Return;
	bool ZhanDou;//�Ƿ���ս��
};

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
//�ǳƺϷ�
struct _S_C_GP1 : public _GAME_PROTOCOL
{
	_S_C_GP1()
	{
		id = _NICK_NAME_LEGALITY;
	}
	//char scene[32];//��һ������
};
//�ǳƷǷ�
struct _S_C_GP2 : public _GAME_PROTOCOL
{
	_S_C_GP2()
	{
		id = _NICK_NAME_LAWLESS;
	}
};


//ѡ���ɫ
struct _C_S_GP2 : public _GAME_PROTOCOL
{
	_C_S_GP2()
	{
		id = _SEND_CHOICE_ROLE;
	}
	short role_flag;
};
//����ѡ���ɫ
struct _S_C_GP3 : public _GAME_PROTOCOL
{
	_S_C_GP3()
	{
		id = _RETURN_CHOICE_ROLE;
	}
	char scene[32];
	PLAYER pyr;
};
//ѡ���ɫ������Ҽ���
struct _S_C_GP4 : public _GAME_PROTOCOL
{
	_S_C_GP4()
	{
		id = _NEW_PLAYER_CHOICE_ROLE;
	}
	PLAYER pyr;//�����Ϣ
	//char scene[32];//��һ������
};

//��������
struct _C_S_GP3 : public _GAME_PROTOCOL
{
	_C_S_GP3()
	{
		id = _SEND_TALK;
	}
	char str[128];//���͵��ַ���
	char scene;//�ĸ���������
};
//ת������
struct _S_C_GP5 :  public _GAME_PROTOCOL
{
	_S_C_GP5()
	{
		id = _PLAYER_TALK;
	}
	char str[128];//���͵��ַ���
	char scene;//�ĸ���������
};

//�ƶ�
struct _C_S_GP4 : public _GAME_PROTOCOL
{
	_C_S_GP4()
	{
		id = _SEND_WALK;
	}
	int state;//״̬
	bool bMove;//�ڲ��������
	int MoveDir;//�ƶ��ķ���
	int x,y;
	bool image;//����
};
//ת���ƶ�
struct _S_C_GP6 : public _GAME_PROTOCOL
{
	_S_C_GP6()
	{
		id = _PLAYER_WALK;
	}
	PLAYER pyr;
};

//��������
struct _C_S_GP5 : public _GAME_PROTOCOL
{
	_C_S_GP5()
	{
		id = _SEND_FOUND_ROOM;
	}
	char room_name[32];//������
	char mima[32];//����
};
//���ش�������
struct _S_C_GP7 : public _GAME_PROTOCOL
{
	_S_C_GP7()
	{
		id = _RETURN_FOUND_ROOM;
	}
	int room_index;//�����
};
//ת����������
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

//׼��
struct _C_S_GP6 : public _GAME_PROTOCOL
{
	_C_S_GP6()
	{
		id = _SEND_PREPARE;
	}
	bool prepare;//true:Ϊ׼����false:Ϊȡ��׼��
	int scene;//�����
	bool master;//�Ƿ��Ƿ���
};
//ת��׼��
struct _S_C_GP9 : public _GAME_PROTOCOL
{
	_S_C_GP9()
	{
		id = _PLAYER_PREPARE;
	}
	char name[32];//�ĸ����
	bool prepare;//true:Ϊ׼����false:Ϊȡ��׼��
};
//��ʼ����
struct _S_C_GP10 : public _GAME_PROTOCOL
{
	_S_C_GP10()
	{
		id = _BEGIN_DULE;
	}
	char scene[32];//��һ������
	PLAYER pyr;
};


//���������ƶ����(����)״̬
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
//ת�������ƶ����(����)״̬
struct _S_C_GP11 : public _GAME_PROTOCOL
{
	_S_C_GP11()
	{
		id = _PLAYER_YI_DONG_SHE_JI;
	}
	PLAYER pyr;
};
//�ƶ�����任����
struct _C_S_GP100 : public _GAME_PROTOCOL
{
	_C_S_GP100()
	{
		id = _YI_DONG_SHE_JI_DIR;
	}
	bool dir;
};
//ת���ƶ�����任����
struct _S_C_GP100 : public _GAME_PROTOCOL
{
	_S_C_GP100()
	{
		id = _ZF_YI_DONG_SHE_JI_DIR;
	}
	bool dir;
	char name[32];
};
//�ƶ����״̬���ƶ�
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
//ת���ƶ����״̬�ƶ�
struct _S_C_GP13 : public _GAME_PROTOCOL
{
	_S_C_GP13()
	{
		id = _ZF_YDSJ_MOVE;
	}
	PLAYER pyr;
};

//�������
struct _C_S_GP9 : public _GAME_PROTOCOL
{
	_C_S_GP9()
	{
		id = _SEND_DEATH;
	}
	PLAYER pyr;
};

//�������ת��
struct _S_C_GP14 : public _GAME_PROTOCOL
{
	_S_C_GP14()
	{
		id = _PLAYER_DEATH;
	}
	PLAYER pyr;
};
//��������
struct _S_C_GP15 : public _GAME_PROTOCOL
{
	_S_C_GP15()
	{
		id = _RETURN_SCENE;
	}
	char scene[32];
	PLAYER pyr;
};

//�˳�����
struct _C_S_GP10 : public _GAME_PROTOCOL
{
	_C_S_GP10()
	{
		id = _SEND_QUIT_ROOM;
	}
	int room_index;
	bool fangzhu;
};
//�˳�����
struct _S_C_GP16 : public _GAME_PROTOCOL
{
	_S_C_GP16()
	{
		id = _PLAYER_QUIT_ROOM;
	}
	int scene;
	char name[32];
};
//ת���˳�����
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
//�˳���Ϸ
struct _C_S_GP11 : public _GAME_PROTOCOL
{
	_C_S_GP11()
	{
		id = _SEND_QUIT_GAME;
	}
	char scene[32];
	char name[32];
};
//�˳���Ϸ
struct _S_C_GP18 : public _GAME_PROTOCOL
{
	_S_C_GP18()
	{
		id = _RETURN_QUIT_GAME;
	}
};
//ת���˳���Ϸ
struct _S_C_GP19 : public _GAME_PROTOCOL
{
	_S_C_GP19()
	{
		id = _PLAYER_QUIT_GAME;
	}
	char name[32];
};

//�ƶ����������
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
//ת���ƶ����������
struct _S_C_GP20 : public _GAME_PROTOCOL
{
	_S_C_GP20()
	{
		id = _ZF_YDSJ_DA_ZHONG_REN;
	}

	PLAYER pyr;
};
////ת�����뷿��
//struct _S_C_GP21 : public _GAME_PROTOCOL
//{
//	_S_C_GP21()
//	{
//		id = _PLAYER_ENTER_ROOM;
//	}
//	PLAYER pyr;
//};

//��ҽ������
struct _C_S_GP13 : public _GAME_PROTOCOL
{
	_C_S_GP13()
	{
		id = _SEND_ENTER_TOWN;
	}
	bool fanhui;
	PLAYER pyr;
};

//ת���������
struct _S_C_GP22 : public _GAME_PROTOCOL
{
	_S_C_GP22()
	{
		id = _PLAYER_ENTER_TOWN;
	}
	PLAYER pyr;
};

//����������ݣ��������
struct _S_C_GP23 : public _GAME_PROTOCOL
{
	_S_C_GP23()
	{
		id = _RETURN_PLAYER_TOWN;
	}
	int num;
	PLAYER pyr[32];
};

//����ͬ����Ϣ
struct _C_S_GP14 : public _GAME_PROTOCOL
{
	_C_S_GP14()
	{
		id = _SEND_TONG_BU;
	}
	PLAYER pyr;
};

//ת��ͬ����Ϣ
struct _S_C_GP24 : public _GAME_PROTOCOL
{
	_S_C_GP24()
	{
		id = _RETURN_TONG_BU;
	}
	int num;
	PLAYER pyr[32];
};
//�����������
struct _S_C_GP25 : public _GAME_PROTOCOL
{
	_S_C_GP25()
	{
		id = _JIN_RU_JUE_DOU;
	}
	int num;
	PLAYER pyr[32];
};

//����׼��
struct _C_S_GP15 : public _GAME_PROTOCOL
{
	_C_S_GP15()
	{
		id = _FANG_ZHU_ZHUN_BEI;
	}
	int scene;
};


//����ѡ�񷿼����
struct _C_S_GP16 : public _GAME_PROTOCOL
{
	_C_S_GP16()
	{
		id = _ENTER_XUE_ZHE_FANG_JIAN;
	}
};
//���ط�������
struct _S_C_GP26 : public _GAME_PROTOCOL
{
	_S_C_GP26()
	{
		id = _RETURN_XUE_ZHE_FANG_JIAN;
	}
	int num;
	FangJianXinXi fjxx[32];
};

//������ʱ���ܽ���
struct _S_C_GP27 : public _GAME_PROTOCOL
{
	_S_C_GP27()
	{
		id = _ROOM_NO_ENTER;
	}
	int fangjianhao;
};

//������Խ���S
struct _S_C_GP28 : public _GAME_PROTOCOL
{
	_S_C_GP28()
	{
		id = _ROOM_OK_ENTER;
	}
	int fangjianhao;
};

//������������
struct _S_C_GP29 : public _GAME_PROTOCOL
{
	_S_C_GP29()
	{
		id = _ROOM_RENSHU_BIAN_HUA;
	}
	int fangjianhao;
	int renshu;
};

//���÷���
struct _S_C_GP30 : public _GAME_PROTOCOL
{
	_S_C_GP30()
	{
		id = _SET_FANG_ZHU;
	}
	char name[32];
};
//����״̬
struct _C_S_GP17 : public _GAME_PROTOCOL
{
	_C_S_GP17()
	{
		id = _DAI_JI_ZHUANG_TAI;
	}
	bool bStand;//�ڲ��������
	bool image;//����
	int state;//״̬
};
//ת������״̬ 
struct _S_C_GP31 : public _GAME_PROTOCOL
{
	_S_C_GP31()
	{
		id = _ZHUN_FA_DAI_JI;
	}
	PLAYER pyr;
};
//�ƶ��������
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
//ת���ƶ��������
struct _S_C_GP32 : public _GAME_PROTOCOL
{
	_S_C_GP32()
	{
		id = _ZF_YDSJ_GONG_JI;
	}
	PLAYER pyr;
};
//��������
struct _C_S_GP19 : public _GAME_PROTOCOL
{
	_C_S_GP19()
	{
		id = _GAME_END;
	}

};
//ת����������
struct _S_C_GP33 : public _GAME_PROTOCOL
{
	_S_C_GP33()
	{
		id = _ZF_GAME_END;
	}
};
//�����������
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