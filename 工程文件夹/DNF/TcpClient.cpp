#include "TcpClient.h"
#include <MSTcpIP.h>
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")

#define _S_NO_CONNECT 0 //δ����
#define _S_TRY_CONNECT 1 //��������
#define _S_CONNECTED 2 //�Ѿ�����
#define _S_PREPARE_DISCONNECT 3 //׼�������Ͽ�
#define _S_BE_DISCONNECTED 4 //�����Ͽ�
#define _S_WAIT_FINISH 5 //�ȴ�����

void CTcpClient::_OperateData(const char* buf, int r, _RECV_DATA* rd)
{
	//��ǰ�����±�
	int cur_i = 0;
	while (cur_i < r)
	{
		//��ǰ���ճ��Ȳ���
		if (rd->allsize_bytes < 4)
		{
			//��ǰ�������곤�Ȳ���
			if (4 - rd->allsize_bytes > r - cur_i)
			{
				//���ֽڿ����������ܳ���
				memcpy(&rd->allsize, buf + cur_i, r - cur_i);

				//�����ܳ�����Ч�ֽ�������
				rd->allsize_bytes += r - cur_i;

				//��ǰ�����±����
				cur_i += r - cur_i;
			}
			//��ǰ�������곤�Ȳ���
			else
			{
				//���ֽڿ����������ܳ���
				memcpy(&rd->allsize, buf + cur_i, 4 - rd->allsize_bytes);

				//��ǰ�����±����
				cur_i += 4 - rd->allsize_bytes;

				//�����ܳ�����Ч�ֽ�������Ϊ4
				rd->allsize_bytes = 4;

				//���ٶ��ڴ�
				rd->data = (char*)m_MP.Allocate(rd->allsize);
			}
		}
		//��ǰ�������ݲ���
		else
		{
			//��ǰ���ܽ���������
			if (rd->allsize - rd->recvsize > r - cur_i)
			{
				//����ʣ�µ������ֽڵ�������
				memcpy(rd->data + rd->recvsize, buf + cur_i, r - cur_i);

				//��ǰ�Ѿ����յ����ݳ��ȵ���
				rd->recvsize += r - cur_i;

				//��ǰ�����±����
				cur_i += r - cur_i;
			}
			else
			{
				//������ǰ��������Ҫ���ֽڵ�������
				memcpy(rd->data + rd->recvsize, buf + cur_i, rd->allsize - rd->recvsize);

				//��ǰ�����±����
				cur_i += rd->allsize - rd->recvsize;

				//�����������¼�
				_TC_NET_EVENT tne = {_TCNE_DATA, rd->data, rd->allsize};
				m_TCNetEvent.Enter(tne);

				//��ս���
				rd->allsize = 0;
				rd->allsize_bytes = 0;
				rd->data = 0;
				rd->recvsize = 0;
			}
		}
	}
}

int CTcpClient::_RecvData()
{
	const int len = 1024 * 64;
	char buf[len];

	//������Ǳ����Ͽ�״̬��ô���׽��ֽ��н�������
	if (m_State != _S_BE_DISCONNECTED)
	{
		//��������
		int r = recv(m_Socket, buf, len, 0);

		//����
		if (r == SOCKET_ERROR)
		{
			//��ĳ�����
			if (WSAEWOULDBLOCK != WSAGetLastError())
			{
				//��Ϊ����ɾ����ǰ�ͻ��˼���һ�������Դ
				_Close(_TCNE_E_DISCONNECT);
				return 0;
			}
		}
		//���ŶϿ�
		else if (r == 0)
		{
			//�ͻ��˵ȴ�����״̬�¾͹ر�����
			if (m_State == _S_WAIT_FINISH)
			{
				//��Ϊ�ͻ��������Ͽ�ɾ����ǰ�ͻ��˼���һ�������Դ
				_Close(_TCNE_C_DISCONNECT);

				return 2;
			}
			//����״̬��׼�������Ͽ�״̬�¾��޸�Ϊ�����Ͽ�
			else
				m_State = _S_BE_DISCONNECTED;
		}
		//�յ�������
		else
			_OperateData(buf, r, &m_RD);
	}

	return 1;
}

int CTcpClient::_SendData()
{
	//������ǵȴ�����״̬��ô���׽��ֽ��з�������
	if (m_State != _S_WAIT_FINISH)
	{
		//��������ѭ��ʱ��ı��
		//0��ʾ���ͻ����������������������
		//1��ʾ��ʱ���ܷ���
		//2��ʾ����
		int flag1 = 0;

		//���Ͷ�ͷ����
		_SEND_DATA sd;
		while (m_SD_Queue.Length() > 0)
		{
			//�õ���ͷ����
			m_SD_Queue.GetHead(&sd);

			//��������ѭ��ʱ��ı��
			//0��ʾ�����ݰ������������
			//1��ʾ��ʱ���ܷ���
			//2��ʾ����
			int flag2 = 0;

			//ѭ�����Ͷ�ͷ����
			while (sd.sendsize < sd.allsize)
			{
				//����
				int r = send(
					m_Socket,
					sd.data + sd.sendsize,
					sd.allsize - sd.sendsize,
					0);

				//����
				if (SOCKET_ERROR == r)
				{
					//��ǰ�ݲ��ܷ�
					if (WSAEWOULDBLOCK == WSAGetLastError())
						flag2 = 1;
					//��ǰ����
					else
						flag2 = 2;
					break;
				}
				else
					sd.sendsize += r;
			}

			if (flag2 == 0)
			{
				m_MP.Free(sd.data);
				m_SD_Queue.Quit(&sd);
			}
			else
			{
				flag1 = flag2;
				break;
			}
		}

		//����״ֵ̬���д���

		//��ǰ�ͻ�û�д����͵�������
		if (flag1 == 0)
		{
			switch (m_State)
			{
			case _S_CONNECTED: break;
			case _S_PREPARE_DISCONNECT:
				{
					shutdown(m_Socket, SD_SEND);
					m_State = _S_WAIT_FINISH;
					break;
				}
			case _S_BE_DISCONNECTED:
				{
					shutdown(m_Socket, SD_SEND);

					//��Ϊ�����������Ͽ�ɾ����ǰ�ͻ��˼���һ�������Դ
					_Close(_TCNE_S_DISCONNECT);

					return 3;
				}
			}
		}
		//��ʱ���ܷ��Ļ�����һ��ѭ��
		else if (flag1 == 1)
			;
		//������ôɾ���ÿͻ�
		else
		{
			//��Ϊ����ɾ����ǰ�ͻ��˼���һ�������Դ
			_Close(_TCNE_E_DISCONNECT);
			return 0;
		}
	}

	return 1;
}

void CTcpClient::_Close(int type)
{
	//ɾ����������
	while (m_SD_Queue.Length() > 0)
	{
		_SEND_DATA sd;
		m_SD_Queue.Quit(&sd);
		m_MP.Free(sd.data);
	}

	//ɾ���ÿͻ��Ľ�������
	if (m_RD.data)
		m_MP.Free(m_RD.data);
	m_RD.allsize = 0;
	m_RD.allsize_bytes = 0;
	m_RD.data = 0;
	m_RD.recvsize = 0;

	//�ر��׽���
	closesocket(m_Socket);

	//�˳����绷��
	WSACleanup();

	//����״̬Ϊδ����
	m_State = _S_NO_CONNECT;

	//Ͷ�������¼�
	_TC_NET_EVENT tne = {type};
	m_TCNetEvent.Enter(tne);
}

CTcpClient::CTcpClient()
:
m_State(_S_NO_CONNECT)
{
	m_RD.allsize = 0;
	m_RD.allsize_bytes = 0;
	m_RD.data = 0;
	m_RD.recvsize = 0;
}

bool CTcpClient::ConnectByIP(const char* ip,
							 unsigned short port,
							 int start_ka_time,
							 int interval_ka_time,
							 int wait_time) 
{
	//�������
	if (m_State != _S_NO_CONNECT || !ip || port <= 1024 || start_ka_time < 1 || interval_ka_time < 1 || wait_time < 1)
		return false;

	//�õ�����
	m_WaitTime = wait_time;

	//�������绷��
	WSADATA wd;
	WSAStartup(MAKEWORD(1, 1), &wd);

	//�����׽��ֲ�����Ϊ������
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long nonblocking = 1;
	ioctlsocket(m_Socket, FIONBIO, &nonblocking);

	//����������
	BOOL b = TRUE;
	setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&b, sizeof(b));
	tcp_keepalive tk = {1, start_ka_time * 1000, interval_ka_time * 1000};
	DWORD w;
	WSAIoctl(m_Socket, SIO_KEEPALIVE_VALS, &tk, sizeof(tk), 0, 0, &w, 0, 0);

	//����ַ��Ϣ�ṹ��
	sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_port = htons(port);
	si.sin_addr.S_un.S_addr = inet_addr(ip);

	//�������ӷ�����
	if (SOCKET_ERROR == connect(m_Socket, (sockaddr*)&si, sizeof(si)))
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
			return false;
	}

	//��¼��ǰʱ��
	m_BeginTime = (int)time(0);

	//�޸�״̬Ϊ��������
	m_State = _S_TRY_CONNECT;

	return true;
}

bool CTcpClient::ConnectByName(const char* name,
							   unsigned short port,
							   int start_ka_time,
							   int interval_ka_time,
							   int wait_time)
{
	//�������
	if (m_State != _S_NO_CONNECT || !name || port <= 1024 || start_ka_time < 1 || interval_ka_time < 1 || wait_time < 1)
		return false;

	//�õ�����
	m_WaitTime = wait_time;

	//�������绷��
	WSADATA wd;
	WSAStartup(MAKEWORD(1, 1), &wd);

	//���������õ�IP��ַ
	hostent* h = gethostbyname(name);
	if (0 == h)
	{
		WSACleanup();
		return false; //δ����
	}
	char* ip = inet_ntoa(*(in_addr*)h->h_addr_list[0]);
	if (0 == ip || 0 == strcmp(ip, "0.0.0.0"))
	{
		WSACleanup();
		return false; //û����������ķ��������߷�����δ����
	}

	//�����׽��ֲ�����Ϊ������
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long nonblocking = 1;
	ioctlsocket(m_Socket, FIONBIO, &nonblocking);

	//����������
	BOOL b = TRUE;
	setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&b, sizeof(b));
	tcp_keepalive tk = {1, start_ka_time * 1000, interval_ka_time * 1000};
	DWORD w;
	WSAIoctl(m_Socket, SIO_KEEPALIVE_VALS, &tk, sizeof(tk), 0, 0, &w, 0, 0);

	//����ַ��Ϣ�ṹ��
	sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_port = htons(port);
	si.sin_addr.S_un.S_addr = inet_addr(ip);

	//�������ӷ�����
	if (SOCKET_ERROR == connect(m_Socket, (sockaddr*)&si, sizeof(si)))
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
			return false;
	}

	//��¼��ǰʱ��
	m_BeginTime = (int)time(0);

	//�޸�״̬Ϊ��������
	m_State = _S_TRY_CONNECT;

	return true;
}

void CTcpClient::Run()
{
	switch (m_State)
	{
	case _S_NO_CONNECT: break;
	case _S_TRY_CONNECT:
		{
			//�ж��Ƿ����ӳɹ�
			fd_set fs;
			FD_ZERO(&fs);
			FD_SET(m_Socket, &fs);
			timeval t = {0, 1};
			select(0, 0, &fs, 0, &t);

			//���ӳɹ�
			if (fs.fd_count == 1)
			{
				//�޸�״̬Ϊ�Ѿ�����״̬
				m_State = _S_CONNECTED;

				//Ͷ�����ӳɹ��¼�
				_TC_NET_EVENT tne = {_TCNE_CONNECT_OK};
				m_TCNetEvent.Enter(tne);
			}
			else
			{
				//����ʧ��
				if ((int)time(0) - m_BeginTime > m_WaitTime)
				{
					//�޸�״̬Ϊδ����״̬
					m_State = _S_NO_CONNECT;

					//Ͷ������ʧ���¼�
					_TC_NET_EVENT tne = {_TCNE_CONNECT_FAIL};
					m_TCNetEvent.Enter(tne);
				}
			}

			break;
		}
	case _S_CONNECTED:
	case _S_PREPARE_DISCONNECT:
	case _S_BE_DISCONNECTED:
	case _S_WAIT_FINISH:
		{
			_RecvData();
			_SendData();
			break;
		}
	}
}

bool CTcpClient::GetTCNetEvent(_TC_NET_EVENT* ptne)
{
	return m_TCNetEvent.Quit(ptne);
}

void CTcpClient::ReleaseTSNetEvent(_TC_NET_EVENT* ptne)
{
	if (ptne->type == _TCNE_DATA)
		m_MP.Free(ptne->data);
}

bool CTcpClient::Disconnect()
{
	if (m_State != _S_CONNECTED)
		return false;

	//�޸�״̬Ϊ׼�������Ͽ�״̬
	m_State = _S_PREPARE_DISCONNECT;

	return true;
}

bool CTcpClient::SendData(const char* data, int len)
{
	if (m_State != _S_CONNECTED)
		return false;

	//׼����������
	_SEND_DATA sd;
	sd.allsize = sizeof(int) + len;
	sd.data = (char*)m_MP.Allocate(sd.allsize);
	*((int*)sd.data) = len;
	memcpy(sd.data + sizeof(int), data, len);
	sd.sendsize = 0;

	//�������������
	m_SD_Queue.Enter(sd);

	return true;
}