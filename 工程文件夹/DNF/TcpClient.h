#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <Winsock2.h>
#include "Queue.h"
#include "MemoryPool.h"

#define _TCNE_CONNECT_FAIL 0 //����ʧ��
#define _TCNE_CONNECT_OK 1 //���ӳɹ�
#define _TCNE_S_DISCONNECT 2 //��������Ϊ�������ĶϿ�
#define _TCNE_C_DISCONNECT 3 //�ͻ�����Ϊ�������ĶϿ�
#define _TCNE_E_DISCONNECT 4 //��Ϊ����ĶϿ�
#define _TCNE_DATA 5 //�յ����Է�����������

struct _TC_NET_EVENT
{
	int type; //�¼�����
	char* data; //����
	int len; //���ݳ���
};

class CTcpClient
{
	//ip��ַ
	//�˿ں�
	//����������ʱ��
	//���������ʱ��
	//�����¼�����
	//�ͻ�������
	//{
	//	״̬��δ���ӡ��������ӡ����ӡ�׼�������Ͽ��������Ͽ����ȴ�����
	//	�������ݶ���
	//	��������
	//}
	//�ڴ��

	struct _SEND_DATA //�������ݽṹ��
	{
		int allsize; //���ݰ��ܴ�С
		char* data; //����
		int sendsize; //�Ѿ����͵Ĵ�С
	};

	struct _RECV_DATA //�������ݽṹ��
	{
		int allsize; //�����ܳ���
		int allsize_bytes; //�����ܳ��ȵ���Ч�ֽ�
		char* data; //����
		int recvsize; //�Ѿ��յ������ݳ���
	};

	int m_State; //״̬
	int m_BeginTime; //������ʼʱ��
	int m_WaitTime; //�ȴ�����ʱ��
	SOCKET m_Socket; //�׽���
	CQueue<_TC_NET_EVENT> m_TCNetEvent; //�����¼�����
	CMemoryPool m_MP; //�ڴ��
	CQueue<_SEND_DATA> m_SD_Queue; //�������ݶ���
	_RECV_DATA m_RD; //��������

	void _OperateData(const char* buf, int r, _RECV_DATA* rd);
	int _RecvData();
	int _SendData();
	void _Close(int type);

public:
	CTcpClient();

	bool ConnectByIP(
		const char* ip, //ip��ַ
		unsigned short port, //�˿ں�
		int start_ka_time, //����������ʱ��
		int interval_ka_time, //���������ʱ��
		int wait_time); //�ȴ�����ʱ��

	bool ConnectByName(
		const char* name, //����
		unsigned short port, //�˿ں�
		int start_ka_time, //����������ʱ��
		int interval_ka_time, //���������ʱ��
		int wait_time); //�ȴ�����ʱ��

	//����
	//1����������
	//2����������
	void Run();

	//�õ������¼�
	bool GetTCNetEvent(_TC_NET_EVENT* ptne);

	//�ͷ������¼���ʹ�õ��ڴ�
	void ReleaseTSNetEvent(_TC_NET_EVENT* ptne);

	//�Ͽ�����
	bool Disconnect();

	//��������
	bool SendData(const char* data, int len);
};

#endif