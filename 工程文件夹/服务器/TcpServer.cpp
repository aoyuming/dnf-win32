#include "TcpServer.h"
#include <MSTcpIP.h>
#pragma comment(lib,"Ws2_32.lib")

#define _CS_CONNECTED				0 //连接状态
#define _CS_PREPARE_DISCONNECT		1//服务器准备主动断开
#define _CS_BE_DISCONNECTED			2//服务器被动断开
#define _CS_WAIT_FINISH				3//服务器等待结束

//客户端连接服务器
int  CTcpServer::_AcceptClient()
{
	if(!m_Continue || m_Client.Length() == m_MaxClient)
		return 0;
	//接收客户端的连接
	sockaddr_in ci;
	int ci_len = sizeof(ci);
	SOCKET c = accept(m_Listen,(sockaddr*)&ci,&ci_len);
	//发生错误
	if(INVALID_SOCKET == c)
	{
		if(WSAEWOULDBLOCK == WSAGetLastError())
			return 1;
		else
			return -1;
	}
	else
	{
		//设置心跳包
		BOOL b = TRUE;
		setsockopt(c,SOL_SOCKET,SO_KEEPALIVE,(const char*)&b,sizeof(b));
		tcp_keepalive tk = {1,m_StartKAYime,m_IntervalKAYime};
		DWORD w;
		WSAIoctl(c,SIO_KEEPALIVE_VALS,&tk,sizeof(tk),0,0,&w,0,0);
		//装入映射中
		CLIENT client;
		client.state = _CS_CONNECTED;
		client.ip = ci.sin_addr.S_un.S_addr;
		client.rd.allsize = 0;
		client.rd.allsize_bytes = 0;
		client.rd.data = 0;
		client.rd.recvsize = 0;
		m_Client.Insert(c,client);
		//网络事件入队列
		_TS_NET_EVENT tne = {_NE_CONNECT,c,0,0,client.ip};
		m_TSNetEvent.Enter(tne);
		return 2;
	}
}

//接收数据
void CTcpServer::_OperateData(const char* buf,//接收到的内存
	int r,//实际接收的字节
	SOCKET s,//套接字
	unsigned long ip,//ip地址
	_RECV_DATA* rd)
{
	//分析下标
	int cur_i = 0;
	while(cur_i < r)
	{
		//当前接收长度部分
		if(rd->allsize_bytes < 4)
		{
			//当前不能收完长度部分
			if( 4 - rd->allsize_bytes  > r - cur_i )
			{
				//将字节拷贝到数据总长度中
				memcpy(&rd->allsize,buf + cur_i,r - cur_i);
				rd->allsize_bytes += r - cur_i;
				//下标递增
				cur_i += r - cur_i;
			}
			//能接收完
			else
			{
				//将字节拷贝到数据总长度中
				memcpy(&rd->allsize,buf + cur_i,
					4 - rd->allsize_bytes);
				//x下标递增
				cur_i += 4 - rd->allsize_bytes;
				//将总长度的有效字节数设置为4
				rd->allsize_bytes = 4;
				//开辟内存
				rd->data = (char*)m_MP.Allocate(rd->allsize);
			}
		}
		//接收数据部分
		else
		{
			//数据不能接收完毕
			if(rd->allsize - rd->recvsize > r - cur_i)
			{
				//拷贝剩下的字节到数据中
				memcpy(rd->data + rd->recvsize,
					buf + cur_i,r - cur_i);
				//当前接收数据长度递增
				rd->recvsize += r - cur_i;
				//下标递增
				cur_i += r - cur_i;
			}
			//能接收完
			else
			{
				//拷贝字节
				memcpy(rd->data + rd->recvsize,
					buf + cur_i,rd->allsize - rd->recvsize);
				//下标递增
				cur_i += rd->allsize - rd->recvsize;
				//设置网络事件
				_TS_NET_EVENT tne = {_NE_DATA,s,rd->data,rd->allsize,ip};
				m_TSNetEvent.Enter(tne);
				//清空
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
		//如果不是被动断开那么就对套接字进行数据接收
		if(m_Client.At(i)->data.state != _CS_BE_DISCONNECTED)
		{
			//接收数据
			int r = recv(m_Client.At(i)->key,buf,len,0);
			if(r == SOCKET_ERROR)
			{
				if(WSAEWOULDBLOCK != WSAGetLastError())
				{
					//真的出错,删除对应的客户端及一切相关的资源
					_Close(i,m_Client.At(i)->key,
						&m_Client.At(i)->data,
						_NE_E_DISCONNECT);
					--i;
				}
			}
			//优雅断开
			else if( r == 0 )
			{
				//服务器等待结束状态
				if(m_Client.At(i)->data.state == _CS_WAIT_FINISH)
				{
					_Close(i,m_Client.At(i)->key,
						&m_Client.At(i)->data,
						_NE_S_DISCONNECT);
					--i;
				}
				//连接状态或者服务器准备主动断开
				else
				{
					m_Client.At(i)->data.state = _CS_BE_DISCONNECTED;
				}
			}
			//收到数据
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
		//如果不是等待结束的状态那么对套接字发送数据
		if( m_Client.At(i)->data.state != _CS_WAIT_FINISH)
		{
			//0表示本客户端的数据正常发送完毕
			//1暂时不能发送
			//2出错
			int flag1 = 0;
			//发送队头数据
			_SEND_DATA sd;
			while(m_Client.At(i)->data.sd_queue.Length() > 0)
			{
				//得到队头数据
				m_Client.At(i)->data.sd_queue.GetHead(&sd);
				//0表示本客户端的数据正常发送完毕
				//1暂时不能发送
				//2出错
				int flag2 = 0;
				while(sd.sendsize < sd.allsize)
				{
					int r = send(m_Client.At(i)->key,
						sd.data + sd.sendsize,
						sd.allsize - sd.sendsize,
						0);
					//出错
					if(r == SOCKET_ERROR)
					{
						if(WSAEWOULDBLOCK == WSAGetLastError())
						{
							//暂不能发
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
				//如果消息发送完毕，没有需要发送的数据
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
						//因为客户端主动断开删除相关的数据及资源
						_Close(i,m_Client.At(i)->key,
							&m_Client.At(i)->data,
							_NE_C_DISCONNECT);
						--i;
						break;
					}
				}
			}
			//如果暂时不能发送，就换下一个人发
			else if( flag1 == 1 )
			{}
			else 
			{
				//出错.删除当前客户端
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
	//删除客户端的发送数据
	while(c->sd_queue.Length() > 0)
	{
		_SEND_DATA sd;
		c->sd_queue.Quit(&sd);
		m_MP.Free(sd.data);
	}
	//删除客户端的接收数据
	if(c->rd.data)
		m_MP.Free(c->rd.data);
	//关闭对应的套接字
	closesocket(s);
	//投递网络事件
	_TS_NET_EVENT tne = {type,s,0,0,c->ip};
	m_TSNetEvent.Enter(tne);
	m_Client.Erase(index);
}

//整数IP转字符串IP
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
	unsigned short port,//端口号
	int max_client,//最大连接数
	int start_ka_time,//心跳包启动时间
	int interval_ka_time)//心跳包间隔时间
{
	//参数检查
	if( port <= 1024 || max_client < 1 
	|| start_ka_time < 1 || interval_ka_time < 1)
		return false;
	m_Port = port;
	m_MaxClient = max_client;
	m_StartKAYime = start_ka_time * 1000;
	m_IntervalKAYime = interval_ka_time * 1000;
	m_Continue = true;
	//启动网络环境
	WSADATA  wd;
	WSAStartup(MAKEWORD(1,1),&wd);

	//2)创监听套接字
	m_Listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//设置套接字为非阻塞工作模式
	unsigned long nonblocking = 1;
	ioctlsocket(m_Listen,FIONBIO,&nonblocking);

	//3)填充地址信息结构体
	sockaddr_in si;
	si.sin_family = AF_INET;//表示创建的是用于在internet上工作的套接字
	si.sin_port = htons(m_Port);//端口号
	si.sin_addr.S_un.S_addr = ADDR_ANY;//表示用于服务器的地址信息

	//4)绑定监听套接字和地址信息结构体
	bind(m_Listen,(sockaddr*)&si,sizeof(si));
	
	//5)设置监听套接字的工作方式为监听
	listen(m_Listen,8);

	return true;
}
//终止
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
//循环标记
bool CTcpServer::Continue()
{
	return m_Continue || m_Client.Length() > 0;
}
//结束
void CTcpServer::End()
{
	closesocket(m_Listen);
	WSACleanup();
}
//运行
//1)接收连接
//2)接收数据
//3)发送数据
void CTcpServer::Run()
{
	_AcceptClient();
	_RecvData();
	_SendData();
}
//得到网络事件
bool CTcpServer::GetTSNetEvent(_TS_NET_EVENT* ptne)
{
	return m_TSNetEvent.Quit(ptne);
}
//释放网络事件中使用的内存
void CTcpServer::ReleaseTSNetEvent(_TS_NET_EVENT* ptne)
{
	if(_NE_DATA == ptne->type)
		m_MP.Free(ptne->data);
}
//断开连接
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
//发送数据
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
			//准备发送数据
			_SEND_DATA sd;
			sd.allsize = sizeof(int) + len;
			sd.data = (char*)m_MP.Allocate(sd.allsize);
			*((int*)sd.data) = len;
			memcpy(sd.data + sizeof(int),data,len);
			sd.sendsize = 0;
			//放入发数据队列
			pair->data.sd_queue.Enter(sd);
			return 1;
		}
	}
}