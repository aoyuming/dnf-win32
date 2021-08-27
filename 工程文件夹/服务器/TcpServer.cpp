#include "TcpServer.h"
#include <MSTcpIP.h>
#pragma comment(lib,"Ws2_32.lib")

#define _CS_CONNECTED				0 //����״̬
#define _CS_PREPARE_DISCONNECT		1//������׼�������Ͽ�
#define _CS_BE_DISCONNECTED			2//�����������Ͽ�
#define _CS_WAIT_FINISH				3//�������ȴ�����

//�ͻ������ӷ�����
int  CTcpServer::_AcceptClient()
{
	if(!m_Continue || m_Client.Length() == m_MaxClient)
		return 0;
	//���տͻ��˵�����
	sockaddr_in ci;
	int ci_len = sizeof(ci);
	SOCKET c = accept(m_Listen,(sockaddr*)&ci,&ci_len);
	//��������
	if(INVALID_SOCKET == c)
	{
		if(WSAEWOULDBLOCK == WSAGetLastError())
			return 1;
		else
			return -1;
	}
	else
	{
		//����������
		BOOL b = TRUE;
		setsockopt(c,SOL_SOCKET,SO_KEEPALIVE,(const char*)&b,sizeof(b));
		tcp_keepalive tk = {1,m_StartKAYime,m_IntervalKAYime};
		DWORD w;
		WSAIoctl(c,SIO_KEEPALIVE_VALS,&tk,sizeof(tk),0,0,&w,0,0);
		//װ��ӳ����
		CLIENT client;
		client.state = _CS_CONNECTED;
		client.ip = ci.sin_addr.S_un.S_addr;
		client.rd.allsize = 0;
		client.rd.allsize_bytes = 0;
		client.rd.data = 0;
		client.rd.recvsize = 0;
		m_Client.Insert(c,client);
		//�����¼������
		_TS_NET_EVENT tne = {_NE_CONNECT,c,0,0,client.ip};
		m_TSNetEvent.Enter(tne);
		return 2;
	}
}

//��������
void CTcpServer::_OperateData(const char* buf,//���յ����ڴ�
	int r,//ʵ�ʽ��յ��ֽ�
	SOCKET s,//�׽���
	unsigned long ip,//ip��ַ
	_RECV_DATA* rd)
{
	//�����±�
	int cur_i = 0;
	while(cur_i < r)
	{
		//��ǰ���ճ��Ȳ���
		if(rd->allsize_bytes < 4)
		{
			//��ǰ�������곤�Ȳ���
			if( 4 - rd->allsize_bytes  > r - cur_i )
			{
				//���ֽڿ����������ܳ�����
				memcpy(&rd->allsize,buf + cur_i,r - cur_i);
				rd->allsize_bytes += r - cur_i;
				//�±����
				cur_i += r - cur_i;
			}
			//�ܽ�����
			else
			{
				//���ֽڿ����������ܳ�����
				memcpy(&rd->allsize,buf + cur_i,
					4 - rd->allsize_bytes);
				//x�±����
				cur_i += 4 - rd->allsize_bytes;
				//���ܳ��ȵ���Ч�ֽ�������Ϊ4
				rd->allsize_bytes = 4;
				//�����ڴ�
				rd->data = (char*)m_MP.Allocate(rd->allsize);
			}
		}
		//�������ݲ���
		else
		{
			//���ݲ��ܽ������
			if(rd->allsize - rd->recvsize > r - cur_i)
			{
				//����ʣ�µ��ֽڵ�������
				memcpy(rd->data + rd->recvsize,
					buf + cur_i,r - cur_i);
				//��ǰ�������ݳ��ȵ���
				rd->recvsize += r - cur_i;
				//�±����
				cur_i += r - cur_i;
			}
			//�ܽ�����
			else
			{
				//�����ֽ�
				memcpy(rd->data + rd->recvsize,
					buf + cur_i,rd->allsize - rd->recvsize);
				//�±����
				cur_i += rd->allsize - rd->recvsize;
				//���������¼�
				_TS_NET_EVENT tne = {_NE_DATA,s,rd->data,rd->allsize,ip};
				m_TSNetEvent.Enter(tne);
				//���
				rd->allsize = 0;
				rd->allsize_bytes = 0;
				rd->recvsize = 0;
				rd->data = 0;
			}
		}
	}
}
int CTcpServer::_RecvData()
{
	const int len = 1024 * 64;
	char buf[len];
	for( int i = 0; i < m_Client.Length(); ++i )
	{
		//������Ǳ����Ͽ���ô�Ͷ��׽��ֽ������ݽ���
		if(m_Client.At(i)->data.state != _CS_BE_DISCONNECTED)
		{
			//��������
			int r = recv(m_Client.At(i)->key,buf,len,0);
			if(r == SOCKET_ERROR)
			{
				if(WSAEWOULDBLOCK != WSAGetLastError())
				{
					//��ĳ���,ɾ����Ӧ�Ŀͻ��˼�һ����ص���Դ
					_Close(i,m_Client.At(i)->key,
						&m_Client.At(i)->data,
						_NE_E_DISCONNECT);
					--i;
				}
			}
			//���ŶϿ�
			else if( r == 0 )
			{
				//�������ȴ�����״̬
				if(m_Client.At(i)->data.state == _CS_WAIT_FINISH)
				{
					_Close(i,m_Client.At(i)->key,
						&m_Client.At(i)->data,
						_NE_S_DISCONNECT);
					--i;
				}
				//����״̬���߷�����׼�������Ͽ�
				else
				{
					m_Client.At(i)->data.state = _CS_BE_DISCONNECTED;
				}
			}
			//�յ�����
			else
			{
				_OperateData(buf,r,m_Client.At(i)->key,
					m_Client.At(i)->data.ip,
					&m_Client.At(i)->data.rd);
			}
		}
	}
	return 1;
}
int CTcpServer::_SendData()
{
	for( int i = 0; i < m_Client.Length(); ++i )
	{
		//������ǵȴ�������״̬��ô���׽��ַ�������
		if( m_Client.At(i)->data.state != _CS_WAIT_FINISH)
		{
			//0��ʾ���ͻ��˵����������������
			//1��ʱ���ܷ���
			//2����
			int flag1 = 0;
			//���Ͷ�ͷ����
			_SEND_DATA sd;
			while(m_Client.At(i)->data.sd_queue.Length() > 0)
			{
				//�õ���ͷ����
				m_Client.At(i)->data.sd_queue.GetHead(&sd);
				//0��ʾ���ͻ��˵����������������
				//1��ʱ���ܷ���
				//2����
				int flag2 = 0;
				while(sd.sendsize < sd.allsize)
				{
					int r = send(m_Client.At(i)->key,
						sd.data + sd.sendsize,
						sd.allsize - sd.sendsize,
						0);
					//����
					if(r == SOCKET_ERROR)
					{
						if(WSAEWOULDBLOCK == WSAGetLastError())
						{
							//�ݲ��ܷ�
							flag2 = 1;
						}
						else
							flag2 = 2;
						break;
					}
					else
						sd.sendsize += r;
				}
				if(flag2 == 0)
				{
					m_MP.Free(sd.data);
					m_Client.At(i)->data.sd_queue.Quit(&sd);
				}
				else
				{
					flag1 = flag2;
					break;
				}
			}
			if(flag1 == 0)
			{
				//�����Ϣ������ϣ�û����Ҫ���͵�����
				switch(m_Client.At(i)->data.state)
				{
				case _CS_CONNECTED:break;
				case _CS_PREPARE_DISCONNECT:
					{
						shutdown(m_Client.At(i)->key,SD_SEND);
						m_Client.At(i)->data.state = _CS_WAIT_FINISH;
						break;
					}
				case _CS_BE_DISCONNECTED:
					{
						shutdown(m_Client.At(i)->key,SD_SEND);
						//��Ϊ�ͻ��������Ͽ�ɾ����ص����ݼ���Դ
						_Close(i,m_Client.At(i)->key,
							&m_Client.At(i)->data,
							_NE_C_DISCONNECT);
						--i;
						break;
					}
				}
			}
			//�����ʱ���ܷ��ͣ��ͻ���һ���˷�
			else if( flag1 == 1 )
			{}
			else 
			{
				//����.ɾ����ǰ�ͻ���
				_Close(i,m_Client.At(i)->key,
						&m_Client.At(i)->data,
						_NE_E_DISCONNECT);
				--i;
			}
		}
	}
	return 1;
}
void CTcpServer::_Close(int index,SOCKET s,CLIENT* c,int type)
{
	//ɾ���ͻ��˵ķ�������
	while(c->sd_queue.Length() > 0)
	{
		_SEND_DATA sd;
		c->sd_queue.Quit(&sd);
		m_MP.Free(sd.data);
	}
	//ɾ���ͻ��˵Ľ�������
	if(c->rd.data)
		m_MP.Free(c->rd.data);
	//�رն�Ӧ���׽���
	closesocket(s);
	//Ͷ�������¼�
	_TS_NET_EVENT tne = {type,s,0,0,c->ip};
	m_TSNetEvent.Enter(tne);
	m_Client.Erase(index);
}

//����IPת�ַ���IP
void CTcpServer::GetStrIP(unsigned long ul_ip,char* str_ip)
{
	in_addr ia;
	ia.S_un.S_addr = ul_ip;
	char* str = inet_ntoa(ia);
	memcpy(str_ip,str,strlen(str) + 1);
}

float CTcpServer::GetUseMemoryMB()
{
	return m_MP.GetUseMemory();
}

bool CTcpServer::Init(
	unsigned short port,//�˿ں�
	int max_client,//���������
	int start_ka_time,//����������ʱ��
	int interval_ka_time)//���������ʱ��
{
	//�������
	if( port <= 1024 || max_client < 1 
	|| start_ka_time < 1 || interval_ka_time < 1)
		return false;
	m_Port = port;
	m_MaxClient = max_client;
	m_StartKAYime = start_ka_time * 1000;
	m_IntervalKAYime = interval_ka_time * 1000;
	m_Continue = true;
	//�������绷��
	WSADATA  wd;
	WSAStartup(MAKEWORD(1,1),&wd);

	//2)�������׽���
	m_Listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//�����׽���Ϊ����������ģʽ
	unsigned long nonblocking = 1;
	ioctlsocket(m_Listen,FIONBIO,&nonblocking);

	//3)����ַ��Ϣ�ṹ��
	sockaddr_in si;
	si.sin_family = AF_INET;//��ʾ��������������internet�Ϲ������׽���
	si.sin_port = htons(m_Port);//�˿ں�
	si.sin_addr.S_un.S_addr = ADDR_ANY;//��ʾ���ڷ������ĵ�ַ��Ϣ

	//4)�󶨼����׽��ֺ͵�ַ��Ϣ�ṹ��
	bind(m_Listen,(sockaddr*)&si,sizeof(si));
	
	//5)���ü����׽��ֵĹ�����ʽΪ����
	listen(m_Listen,8);

	return true;
}
//��ֹ
void CTcpServer::Stop()
{
	for( int i = 0; i < m_Client.Length(); ++i )
	{
		if( m_Client.At(i)->data.state == _CS_CONNECTED )
		{
			m_Client.At(i)->data.state = _CS_PREPARE_DISCONNECT;
		}
	}
	m_Continue = false;
}
//ѭ�����
bool CTcpServer::Continue()
{
	return m_Continue || m_Client.Length() > 0;
}
//����
void CTcpServer::End()
{
	closesocket(m_Listen);
	WSACleanup();
}
//����
//1)��������
//2)��������
//3)��������
void CTcpServer::Run()
{
	_AcceptClient();
	_RecvData();
	_SendData();
}
//�õ������¼�
bool CTcpServer::GetTSNetEvent(_TS_NET_EVENT* ptne)
{
	return m_TSNetEvent.Quit(ptne);
}
//�ͷ������¼���ʹ�õ��ڴ�
void CTcpServer::ReleaseTSNetEvent(_TS_NET_EVENT* ptne)
{
	if(_NE_DATA == ptne->type)
		m_MP.Free(ptne->data);
}
//�Ͽ�����
int CTcpServer::Disconnect(SOCKET s)
{
	CMap<SOCKET,CLIENT>::PAIR* pair = m_Client.Find(s);
	if(pair == 0)
		return 0;
	else
	{
		if( pair->data.state != _CS_CONNECTED )
			return -1;
		else
		{
			pair->data.state = _CS_PREPARE_DISCONNECT;
			return 1;
		}
	}
}
//��������
int CTcpServer::SendData(SOCKET s,const char* data,int len)
{
	CMap<SOCKET,CLIENT>::PAIR* pair = m_Client.Find(s);
	if(pair == 0)
		return 0;
	else
	{
		if( pair->data.state != _CS_CONNECTED )
			return -1;
		else
		{
			//׼����������
			_SEND_DATA sd;
			sd.allsize = sizeof(int) + len;
			sd.data = (char*)m_MP.Allocate(sd.allsize);
			*((int*)sd.data) = len;
			memcpy(sd.data + sizeof(int),data,len);
			sd.sendsize = 0;
			//���뷢���ݶ���
			pair->data.sd_queue.Enter(sd);
			return 1;
		}
	}
}