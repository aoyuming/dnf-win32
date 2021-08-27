#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#define _WIN32_WINNT  0x0500
#include <Winsock2.h>
#include "MemoryPool.h"
#include "Queue.h"
#include "Map.h"

#define _NE_CONNECT			0 //����
#define _NE_S_DISCONNECT	1//��������Ϊ�������Ͽ�
#define _NE_C_DISCONNECT	2//�ͻ��������Ͽ�
#define _NE_E_DISCONNECT	3//����Ͽ�
#define _NE_DATA			4//���յ����Կͻ��˵�����

//�����¼��ṹ��
struct _TS_NET_EVENT
{
	//0:����
	//1:��������Ϊ�������Ͽ�
	//2:�ͻ��������Ͽ�
	//3:����Ͽ�
	//4:���յ����Կͻ��˵�����
	int type;//�¼�����
	SOCKET s;//�ͻ��˱��
	char* data;//����
	int len;//���ݳ���
	unsigned long ip;//�ͻ�IP��ַ
};

class CTcpServer
{
	//�˿ں�
	unsigned short m_Port;
	//���������
	int m_MaxClient;
	//����������ʱ��
	int m_StartKAYime;
	//���������ʱ��
	int m_IntervalKAYime;
	//ѭ�����
	bool m_Continue;
	//�����׽���
	SOCKET m_Listen;
	//�����¼�����
	CQueue<_TS_NET_EVENT> m_TSNetEvent;
	//�������ݽṹ��
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
	//�ͻ�������
	//{
	//  ״̬:���ӣ�׼�������Ͽ��������Ͽ����ȴ�����
	//  ip��ַ
	//  �������ݶ���
	//	��������
	//}
	struct CLIENT
	{
		int state;
		unsigned long ip;
		CQueue<_SEND_DATA> sd_queue;
		_RECV_DATA rd;
	};
	//�ͻ���ӳ��<SOCKET,�ͻ�������>;
	CMap<SOCKET,CLIENT> m_Client;
	//�ڴ��
	CMemoryPool m_MP;

	//
	int  _AcceptClient();
	void _OperateData(const char* buf,
		int r,SOCKET s,unsigned long ip,_RECV_DATA* rd);
	int _RecvData();
	int _SendData();
	void _Close(int index,SOCKET s,CLIENT* c,int type);

public:
	//����IPת�ַ���IP
	static void GetStrIP(unsigned long ul_ip,char* str_ip);

	float GetUseMemoryMB();

	bool Init(
		unsigned short port,//�˿ں�
		int max_client,//���������
		int start_ka_time,//����������ʱ��
		int interval_ka_time);//���������ʱ��
	//��ֹ
	void Stop();
	//ѭ�����
	bool Continue();
	//����
	void End();
	//����
	//1)��������
	//2)��������
	//3)��������
	void Run();
	//�õ������¼�
	bool GetTSNetEvent(_TS_NET_EVENT* ptne);
	//�ͷ������¼���ʹ�õ��ڴ�
	void ReleaseTSNetEvent(_TS_NET_EVENT* ptne);
	//�Ͽ�����
	int Disconnect(SOCKET s);
	//��������
	int SendData(SOCKET s,const char* data,int len);
};

#endif