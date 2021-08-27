#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include <Winsock2.h>
#include "Queue.h"
#include "MemoryPool.h"

#define _TCNE_CONNECT_FAIL 0 //连接失败
#define _TCNE_CONNECT_OK 1 //连接成功
#define _TCNE_S_DISCONNECT 2 //服务器作为主动方的断开
#define _TCNE_C_DISCONNECT 3 //客户端作为主动方的断开
#define _TCNE_E_DISCONNECT 4 //因为错误的断开
#define _TCNE_DATA 5 //收到来自服务器的数据

struct _TC_NET_EVENT
{
	int type; //事件类型
	char* data; //数据
	int len; //数据长度
};

class CTcpClient
{
	//ip地址
	//端口号
	//心跳包启动时间
	//心跳包间隔时间
	//网络事件队列
	//客户端数据
	//{
	//	状态：未连接、尝试连接、连接、准备主动断开、被动断开、等待结束
	//	发送数据队列
	//	接收数据
	//}
	//内存池

	struct _SEND_DATA //发送数据结构体
	{
		int allsize; //数据包总大小
		char* data; //数据
		int sendsize; //已经发送的大小
	};

	struct _RECV_DATA //接收数据结构体
	{
		int allsize; //数据总长度
		int allsize_bytes; //数据总长度的有效字节
		char* data; //数据
		int recvsize; //已经收到的数据长度
	};

	int m_State; //状态
	int m_BeginTime; //连接起始时间
	int m_WaitTime; //等待连接时间
	SOCKET m_Socket; //套接字
	CQueue<_TC_NET_EVENT> m_TCNetEvent; //网络事件队列
	CMemoryPool m_MP; //内存池
	CQueue<_SEND_DATA> m_SD_Queue; //发送数据队列
	_RECV_DATA m_RD; //接收数据

	void _OperateData(const char* buf, int r, _RECV_DATA* rd);
	int _RecvData();
	int _SendData();
	void _Close(int type);

public:
	CTcpClient();

	bool ConnectByIP(
		const char* ip, //ip地址
		unsigned short port, //端口号
		int start_ka_time, //心跳包启动时间
		int interval_ka_time, //心跳包间隔时间
		int wait_time); //等待连接时间

	bool ConnectByName(
		const char* name, //域名
		unsigned short port, //端口号
		int start_ka_time, //心跳包启动时间
		int interval_ka_time, //心跳包间隔时间
		int wait_time); //等待连接时间

	//运行
	//1）接收数据
	//2）发送数据
	void Run();

	//得到网络事件
	bool GetTCNetEvent(_TC_NET_EVENT* ptne);

	//释放网络事件中使用的内存
	void ReleaseTSNetEvent(_TC_NET_EVENT* ptne);

	//断开连接
	bool Disconnect();

	//发送数据
	bool SendData(const char* data, int len);
};

#endif