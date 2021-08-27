#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_
#include "Queue.h"
#include "MemoryPool.h"
#include <Winsock2.h>

#define _TCNE_CONNECT_FAIL		0//����ʧ��
#define _TCNE_CONNECT_OK		1//���ӳɹ�
#define _TCNE_S_DISCONNECT		2//�����������Ͽ�
#define _TCNE_C_DISCONNECT		3//�ͻ��������Ͽ�
#define _TCNE_E_DISCONNECT		4//����Ͽ�
#define _TCNE_DATA				5//���յ����Է�����������
//�����¼��ṹ��
struct _TC_NET_EVENT
{
	//0:����
	//1:��������Ϊ�������Ͽ�
	//2:�ͻ��������Ͽ�
	//3:����Ͽ�
	//4:���յ����Է�����������
	int type;//�¼�����
	char* data;//����
	int len;//���ݳ���
};

class CTcpClient
{
	//�����ݽṹ��
	struct _SEND_DATA
	{
		int allsize;//���ݰ��Ĵ�С
		char* data;//����
		int sendsize;//�Ѿ����͵����ݵĴ�С
	};
	//�������ݽṹ��
	struct _RECV_DATA
	{
		int allsize;//���ݰ��ܳ���
		int allsize_bytes;//���ȵ���Ч�ֽ���
		char* data;//����
		int recvsize;//�Ѿ����յĳ���
	};
	int m_State;//״̬
	int m_BeginTime;//������ʼʱ��
	int m_WaitTime;//�ȴ�����ʱ��
	SOCKET m_Socket;//�׽���
	//�����¼�����
	CQueue<_TC_NET_EVENT> m_TCNetEvent;
	//�ڴ��
	CMemoryPool m_MP;
	//�����ݶ���
	CQueue<_SEND_DATA> m_SD_Queue;
	//��������
	_RECV_DATA m_RD;

	void _OperateData(const char* buf,
		int r,_RECV_DATA* rd);
	int _RecvData();
	int _SendData();
	void _Close(int type);
public:
	CTcpClient();
	//����IP����
	bool ConnectByIP(
		const char* IP,//ip��ַ
		unsigned short port,//�˿ں�
		int start_ka_time,//����������ʱ��
		int interval_ka_time,//���������ʱ��
		int wait_time);//�ȴ�����ʱ��
	//������������
	bool ConnectByName(
		const char* name,//����
		unsigned short port,//�˿ں�
		int start_ka_time,//����������ʱ��
		int interval_ka_time,//���������ʱ��
		int wait_time);//�ȴ�����ʱ��

	//����
	//1)��������
	//2)��������
	void Run();

	//�õ������¼�
	bool GetTCNetEvent(_TC_NET_EVENT* ptne);

	//�ͷ������¼���ʹ�õ��ڴ�
	void ReleaseTCNetEvent(_TC_NET_EVENT* ptne);
	//�Ͽ�����
	bool Disconnect();
	//��������
	bool SendData(const char* data,int len);
};
#endif