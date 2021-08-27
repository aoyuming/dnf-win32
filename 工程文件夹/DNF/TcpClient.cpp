#include "TcpClient.h"
#include <MSTcpIP.h>
#include <time.h>
#pragma comment(lib, "Ws2_32.lib")

#define _S_NO_CONNECT 0 //未连接
#define _S_TRY_CONNECT 1 //尝试连接
#define _S_CONNECTED 2 //已经连接
#define _S_PREPARE_DISCONNECT 3 //准备主动断开
#define _S_BE_DISCONNECTED 4 //被动断开
#define _S_WAIT_FINISH 5 //等待结束

void CTcpClient::_OperateData(const char* buf, int r, _RECV_DATA* rd)
{
	//当前分析下标
	int cur_i = 0;
	while (cur_i < r)
	{
		//当前接收长度部分
		if (rd->allsize_bytes < 4)
		{
			//当前不能收完长度部分
			if (4 - rd->allsize_bytes > r - cur_i)
			{
				//将字节拷贝到数据总长中
				memcpy(&rd->allsize, buf + cur_i, r - cur_i);

				//数字总长的有效字节数递增
				rd->allsize_bytes += r - cur_i;

				//当前分析下标递增
				cur_i += r - cur_i;
			}
			//当前可以收完长度部分
			else
			{
				//将字节拷贝到数据总长中
				memcpy(&rd->allsize, buf + cur_i, 4 - rd->allsize_bytes);

				//当前分析下标递增
				cur_i += 4 - rd->allsize_bytes;

				//数字总长的有效字节数设置为4
				rd->allsize_bytes = 4;

				//开辟堆内存
				rd->data = (char*)m_MP.Allocate(rd->allsize);
			}
		}
		//当前接收数据部分
		else
		{
			//当前不能接收完数据
			if (rd->allsize - rd->recvsize > r - cur_i)
			{
				//拷贝剩下的所有字节到数据中
				memcpy(rd->data + rd->recvsize, buf + cur_i, r - cur_i);

				//当前已经接收的数据长度递增
				rd->recvsize += r - cur_i;

				//当前分析下标递增
				cur_i += r - cur_i;
			}
			else
			{
				//拷贝当前完整包需要的字节到数据中
				memcpy(rd->data + rd->recvsize, buf + cur_i, rd->allsize - rd->recvsize);

				//当前分析下标递增
				cur_i += rd->allsize - rd->recvsize;

				//设置收数据事件
				_TC_NET_EVENT tne = {_TCNE_DATA, rd->data, rd->allsize};
				m_TCNetEvent.Enter(tne);

				//清空接收
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

	//如果不是被动断开状态那么对套接字进行接收数据
	if (m_State != _S_BE_DISCONNECTED)
	{
		//接收数据
		int r = recv(m_Socket, buf, len, 0);

		//出错
		if (r == SOCKET_ERROR)
		{
			//真的出错了
			if (WSAEWOULDBLOCK != WSAGetLastError())
			{
				//因为错误删除当前客户端及其一切相关资源
				_Close(_TCNE_E_DISCONNECT);
				return 0;
			}
		}
		//优雅断开
		else if (r == 0)
		{
			//客户端等待结束状态下就关闭连接
			if (m_State == _S_WAIT_FINISH)
			{
				//因为客户端主动断开删除当前客户端及其一切相关资源
				_Close(_TCNE_C_DISCONNECT);

				return 2;
			}
			//连接状态或准备主动断开状态下就修改为被动断开
			else
				m_State = _S_BE_DISCONNECTED;
		}
		//收到数据了
		else
			_OperateData(buf, r, &m_RD);
	}

	return 1;
}

int CTcpClient::_SendData()
{
	//如果不是等待结束状态那么对套接字进行发送数据
	if (m_State != _S_WAIT_FINISH)
	{
		//跳出下面循环时候的标记
		//0表示本客户的所有数据正常发送完毕
		//1表示暂时不能发送
		//2表示出错
		int flag1 = 0;

		//发送队头数据
		_SEND_DATA sd;
		while (m_SD_Queue.Length() > 0)
		{
			//得到队头数据
			m_SD_Queue.GetHead(&sd);

			//跳出下面循环时候的标记
			//0表示本数据包正常发送完毕
			//1表示暂时不能发送
			//2表示出错
			int flag2 = 0;

			//循环发送队头数据
			while (sd.sendsize < sd.allsize)
			{
				//发送
				int r = send(
					m_Socket,
					sd.data + sd.sendsize,
					sd.allsize - sd.sendsize,
					0);

				//出错
				if (SOCKET_ERROR == r)
				{
					//当前暂不能发
					if (WSAEWOULDBLOCK == WSAGetLastError())
						flag2 = 1;
					//当前出错
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

		//根据状态值进行处理

		//当前客户没有待发送的数据了
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

					//因为服务器主动断开删除当前客户端及其一切相关资源
					_Close(_TCNE_S_DISCONNECT);

					return 3;
				}
			}
		}
		//暂时不能发的话等下一次循环
		else if (flag1 == 1)
			;
		//出错那么删除该客户
		else
		{
			//因为错误删除当前客户端及其一切相关资源
			_Close(_TCNE_E_DISCONNECT);
			return 0;
		}
	}

	return 1;
}

void CTcpClient::_Close(int type)
{
	//删除发送数据
	while (m_SD_Queue.Length() > 0)
	{
		_SEND_DATA sd;
		m_SD_Queue.Quit(&sd);
		m_MP.Free(sd.data);
	}

	//删除该客户的接收数据
	if (m_RD.data)
		m_MP.Free(m_RD.data);
	m_RD.allsize = 0;
	m_RD.allsize_bytes = 0;
	m_RD.data = 0;
	m_RD.recvsize = 0;

	//关闭套接字
	closesocket(m_Socket);

	//退出网络环境
	WSACleanup();

	//设置状态为未连接
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
	m_RD.data = 0;
	m_RD.recvsize = 0;
}

bool CTcpClient::ConnectByIP(const char* ip,
							 unsigned short port,
							 int start_ka_time,
							 int interval_ka_time,
							 int wait_time) 
{
	//参数检查
	if (m_State != _S_NO_CONNECT || !ip || port <= 1024 || start_ka_time < 1 || interval_ka_time < 1 || wait_time < 1)
		return false;

	//得到参数
	m_WaitTime = wait_time;

	//开启网络环境
	WSADATA wd;
	WSAStartup(MAKEWORD(1, 1), &wd);

	//创建套接字并设置为非阻塞
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long nonblocking = 1;
	ioctlsocket(m_Socket, FIONBIO, &nonblocking);

	//设置心跳包
	BOOL b = TRUE;
	setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&b, sizeof(b));
	tcp_keepalive tk = {1, start_ka_time * 1000, interval_ka_time * 1000};
	DWORD w;
	WSAIoctl(m_Socket, SIO_KEEPALIVE_VALS, &tk, sizeof(tk), 0, 0, &w, 0, 0);

	//填充地址信息结构体
	sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_port = htons(port);
	si.sin_addr.S_un.S_addr = inet_addr(ip);

	//尝试连接服务器
	if (SOCKET_ERROR == connect(m_Socket, (sockaddr*)&si, sizeof(si)))
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
			return false;
	}

	//记录当前时间
	m_BeginTime = (int)time(0);

	//修改状态为尝试连接
	m_State = _S_TRY_CONNECT;

	return true;
}

bool CTcpClient::ConnectByName(const char* name,
							   unsigned short port,
							   int start_ka_time,
							   int interval_ka_time,
							   int wait_time)
{
	//参数检查
	if (m_State != _S_NO_CONNECT || !name || port <= 1024 || start_ka_time < 1 || interval_ka_time < 1 || wait_time < 1)
		return false;

	//得到参数
	m_WaitTime = wait_time;

	//开启网络环境
	WSADATA wd;
	WSAStartup(MAKEWORD(1, 1), &wd);

	//根据域名得到IP地址
	hostent* h = gethostbyname(name);
	if (0 == h)
	{
		WSACleanup();
		return false; //未联网
	}
	char* ip = inet_ntoa(*(in_addr*)h->h_addr_list[0]);
	if (0 == ip || 0 == strcmp(ip, "0.0.0.0"))
	{
		WSACleanup();
		return false; //没有这个域名的服务器或者服务器未开启
	}

	//创建套接字并设置为非阻塞
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	unsigned long nonblocking = 1;
	ioctlsocket(m_Socket, FIONBIO, &nonblocking);

	//设置心跳包
	BOOL b = TRUE;
	setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (const char*)&b, sizeof(b));
	tcp_keepalive tk = {1, start_ka_time * 1000, interval_ka_time * 1000};
	DWORD w;
	WSAIoctl(m_Socket, SIO_KEEPALIVE_VALS, &tk, sizeof(tk), 0, 0, &w, 0, 0);

	//填充地址信息结构体
	sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_port = htons(port);
	si.sin_addr.S_un.S_addr = inet_addr(ip);

	//尝试连接服务器
	if (SOCKET_ERROR == connect(m_Socket, (sockaddr*)&si, sizeof(si)))
	{
		if (WSAEWOULDBLOCK != WSAGetLastError())
			return false;
	}

	//记录当前时间
	m_BeginTime = (int)time(0);

	//修改状态为尝试连接
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
			//判断是否连接成功
			fd_set fs;
			FD_ZERO(&fs);
			FD_SET(m_Socket, &fs);
			timeval t = {0, 1};
			select(0, 0, &fs, 0, &t);

			//连接成功
			if (fs.fd_count == 1)
			{
				//修改状态为已经连接状态
				m_State = _S_CONNECTED;

				//投递连接成功事件
				_TC_NET_EVENT tne = {_TCNE_CONNECT_OK};
				m_TCNetEvent.Enter(tne);
			}
			else
			{
				//连接失败
				if ((int)time(0) - m_BeginTime > m_WaitTime)
				{
					//修改状态为未连接状态
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

	//修改状态为准备主动断开状态
	m_State = _S_PREPARE_DISCONNECT;

	return true;
}

bool CTcpClient::SendData(const char* data, int len)
{
	if (m_State != _S_CONNECTED)
		return false;

	//准备发送数据
	_SEND_DATA sd;
	sd.allsize = sizeof(int) + len;
	sd.data = (char*)m_MP.Allocate(sd.allsize);
	*((int*)sd.data) = len;
	memcpy(sd.data + sizeof(int), data, len);
	sd.sendsize = 0;

	//发送数据入队列
	m_SD_Queue.Enter(sd);

	return true;
}