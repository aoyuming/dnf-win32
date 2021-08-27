#include "TcpClient.h"
#include <MSTcpIP.h>
#include <time.h>
#pragma comment(lib,"Ws2_32.lib")

#define _S_NO_CONNECT			0 //未连接
#define _S_TRY_CONNECT			1 //尝试连接
#define _S_CONNECTED			2 //已经连接
#define _S_PREPARE_DISCONNECT	3 //准备主动断开
#define _S_BE_DISCONNECTED		4 //被动断开
#define _S_WAIT_FINISH			5 //等待结束

void CTcpClient::_OperateData(const char* buf,
		int r,_RECV_DATA* rd)
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
				_TC_NET_EVENT tne = {_TCNE_DATA,rd->data,rd->allsize};
				m_TCNetEvent.Enter(tne);
				//清空
				rd->allsize = 0;
				rd->allsize_bytes = 0;
				rd->recvsize = 0;
				rd->data = 0;
			}
		}
	}
}
int CTcpClient::_RecvData()
{
	const int len = 1024 * 64;
	char buf[len];
	//如果不是被动断开那么就对套接字进行数据接收
	if(m_State != _S_BE_DISCONNECTED)
	{
		//接收数据
		int r = recv(m_Socket,buf,len,0);
		if(r == SOCKET_ERROR)
		{
			if(WSAEWOULDBLOCK != WSAGetLastError())
			{
				//真的出错,删除当前客户端及一切相关的资源
				_Close(_TCNE_E_DISCONNECT);
				return 0;
			}
		}
		//优雅断开
		else if( r == 0 )
		{
			//服务器等待结束状态
			if(m_State == _S_WAIT_FINISH)
			{
				//删除当前客户端及一切相关的资源
				_Close(_TCNE_C_DISCONNECT);
				return 2;
			}
			//连接状态或者主动断开
			else
			{
				m_State = _S_BE_DISCONNECTED;
			}
		}
		//收到数据
		else
		{
			_OperateData(buf,r,&m_RD);
		}
	}
	return 1;
}
int CTcpClient::_SendData()
{
	if( m_State != _S_WAIT_FINISH)
	{
		//0表示本客户端的数据正常发送完毕
		//1暂时不能发送
		//2出错
		int flag1 = 0;
		//发送队头数据
		_SEND_DATA sd;
		while(m_SD_Queue.Length() > 0)
		{
			//得到队头数据
			m_SD_Queue.GetHead(&sd);
			//0表示本客户端的数据正常发送完毕
			//1暂时不能发送
			//2出错
			int flag2 = 0;
			while(sd.sendsize < sd.allsize)
			{
				int r = send(m_Socket,
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
				m_SD_Queue.Quit(&sd);
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
			switch(m_State)
			{
			case _S_CONNECTED:break;
			case _S_PREPARE_DISCONNECT:
				{
					shutdown(m_Socket,SD_SEND);
					m_State = _S_WAIT_FINISH;
					break;
				}
			case _S_BE_DISCONNECTED:
				{
					shutdown(m_Socket,SD_SEND);
					//因为服务器主动断开删除相关的数据及资源
					_Close(_TCNE_S_DISCONNECT);
					return 3;
				}
			}
		}
		//如果暂时不能发送，就换下一个人发
		else if( flag1 == 1 )
		{}
		else 
		{
			//出错.删除当前客户端
			_Close(_TCNE_E_DISCONNECT);
			return 0;
		}
	}
	return 1;
}
void CTcpClient::_Close(int type)
{
	//删除发送数据
	while(m_SD_Queue.Length() > 0)
	{
		_SEND_DATA sd;
		m_SD_Queue.Quit(&sd);
		m_MP.Free(sd.data);
	}
	//删除客户端的接收数据
	if(m_RD.data)
		m_MP.Free(m_RD.data);
	m_RD.allsize = 0;
	m_RD.allsize_bytes = 0;
	m_RD.recvsize = 0;
	m_RD.data = 0;
	//关闭对应的套接字
	closesocket(m_Socket);
	//退出网络环境
	WSACleanup();
	//状态改为未连接
	m_State = _S_NO_CONNECT;
	//投递网络事件
	_TC_NET_EVENT tne = {type};
	m_TCNetEvent.Enter(tne);
}

CTcpClient::CTcpClient()
:
m_State(_S_NO_CONNECT)
{
	m_RD.allsize = 0;
	m_RD.allsize_bytes = 0;
	m_RD.recvsize = 0;
	m_RD.data = 0; 
}
//根据IP连接
bool CTcpClient::ConnectByIP(
	const char* IP,//ip地址
	unsigned short port,//端口号
	int start_ka_time,//心跳包启动时间
	int interval_ka_time,//心跳包间隔时间
	int wait_time)//等待连接时间
{
	if(m_State != _S_NO_CONNECT || !IP || port <= 1024 
		|| start_ka_time < 1 || interval_ka_time < 1 
		|| wait_time < 1)
		return false;
	m_WaitTime = wait_time;
	//启动网络环境
	WSADATA  wd;
	WSAStartup(MAKEWORD(1,1),&wd);

	//2)创建套接字
	m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//设置套接字为非阻塞工作模式
	unsigned long nonblocking = 1;
	ioctlsocket(m_Socket,FIONBIO,&nonblocking);

	//设置心跳包
	BOOL b = TRUE;
	setsockopt(m_Socket,SOL_SOCKET,SO_KEEPALIVE,(const char*)&b,sizeof(b));
	tcp_keepalive tk = {1,start_ka_time * 1000,interval_ka_time * 1000};
	DWORD w;
	WSAIoctl(m_Socket,SIO_KEEPALIVE_VALS,&tk,sizeof(tk),0,0,&w,0,0);
	
	//3)填充地址信息结构体
	sockaddr_in si;
	si.sin_family = AF_INET;//表示创建的是用于在internet上工作的套接字
	si.sin_port = htons(port);//端口号
	si.sin_addr.S_un.S_addr = inet_addr(IP);
	//尝试连接
	if(SOCKET_ERROR == connect(m_Socket,(sockaddr*)&si,sizeof(si)))
	{
		if(WSAEWOULDBLOCK != WSAGetLastError())
			return false;
	}
	//记录当前时间
	m_BeginTime = (int)time(0);
	//修改状态为尝试连接
	m_State = _S_TRY_CONNECT;
	return true;
}
//根据域名连接
bool CTcpClient::ConnectByName(
	const char* name,//域名
	unsigned short port,//端口号
	int start_ka_time,//心跳包启动时间
	int interval_ka_time,//心跳包间隔时间
	int wait_time)//等待连接时间
{
	if(m_State != _S_NO_CONNECT || !name || port <= 1024 
		|| start_ka_time < 1 || interval_ka_time < 1 
		|| wait_time < 1)
		return false;
	m_WaitTime = wait_time;
	//启动网络环境
	WSADATA  wd;
	WSAStartup(MAKEWORD(1,1),&wd);

	//根据域名得到IP
	hostent* h = gethostbyname(name);
	if(0 == h)
	{
		WSACleanup();
		return false;//未联网
	}
	char* ip = inet_ntoa(*((in_addr*)(h->h_addr_list[0])));
	if( 0 == ip || 0 == strcmp(ip,"0.0.0.0"))
	{
		WSACleanup();
		return false;//没有这个域名的IP或者是服务器没开
	}

	//2)创建套接字
	m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	//设置套接字为非阻塞工作模式
	unsigned long nonblocking = 1;
	ioctlsocket(m_Socket,FIONBIO,&nonblocking);

	//设置心跳包
	BOOL b = TRUE;
	setsockopt(m_Socket,SOL_SOCKET,SO_KEEPALIVE,(const char*)&b,sizeof(b));
	tcp_keepalive tk = {1,start_ka_time * 1000,interval_ka_time * 1000};
	DWORD w;
	WSAIoctl(m_Socket,SIO_KEEPALIVE_VALS,&tk,sizeof(tk),0,0,&w,0,0);
	
	//3)填充地址信息结构体
	sockaddr_in si;
	si.sin_family = AF_INET;//表示创建的是用于在internet上工作的套接字
	si.sin_port = htons(port);//端口号
	si.sin_addr.S_un.S_addr = inet_addr(ip);
	//尝试连接
	if(SOCKET_ERROR == connect(m_Socket,(sockaddr*)&si,sizeof(si)))
	{
		if(WSAEWOULDBLOCK != WSAGetLastError())
			return false;
	}
	//记录当前时间
	m_BeginTime = (int)time(0);
	//修改状态为尝试连接
	m_State = _S_TRY_CONNECT;
	return true;
}

//运行
//1)接收数据
//2)发送数据
void CTcpClient::Run()
{
	switch(m_State)
	{
	case _S_NO_CONNECT:break;
	case _S_TRY_CONNECT:
		{
			//判断是否连接成功
			fd_set fs;
			FD_ZERO(&fs);
			FD_SET(m_Socket,&fs);
			timeval t = {0,1};
			select(0,0,&fs,0,&t);
			//连接成功
			if(fs.fd_count == 1)
			{
				//修改状态-连接
				m_State = _S_CONNECTED;
				//投递连接成功事件
				_TC_NET_EVENT tne = {_TCNE_CONNECT_OK};
				m_TCNetEvent.Enter(tne);
			}
			else//连接失败
			{
				if((int)time(0) - m_BeginTime > m_WaitTime)
				{
					//修改状态-未连接
					m_State = _S_NO_CONNECT;
					//投递连接失败事件
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

//得到网络事件
bool CTcpClient::GetTCNetEvent(_TC_NET_EVENT* ptne)
{
	return m_TCNetEvent.Quit(ptne);
}

//释放网络事件中使用的内存
void CTcpClient::ReleaseTCNetEvent(_TC_NET_EVENT* ptne)
{
	if(ptne->type == _TCNE_DATA)
		m_MP.Free(ptne->data);
}
//断开连接
bool CTcpClient::Disconnect()
{
	if(m_State != _S_CONNECTED)
		return false;
	//修改状态为准备主动断开
	m_State = _S_PREPARE_DISCONNECT;
	return true;
}
//发送数据
bool CTcpClient::SendData(const char* data,int len)
{
	if( m_State != _S_CONNECTED )
			return false;
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
		m_SD_Queue.Enter(sd);
		return true;
	}
}