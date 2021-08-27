#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_
#include "Queue.h"
#include "MemoryPool.h"
#include <Winsock2.h>

#define _TCNE_CONNECT_FAIL		0//连接失败
#define _TCNE_CONNECT_OK		1//连接成功
#define _TCNE_S_DISCONNECT		2//服务器主动断开
#define _TCNE_C_DISCONNECT		3//客户端主动断开
#define _TCNE_E_DISCONNECT		4//错误断开
#define _TCNE_DATA				5//接收到来自服务器的数据
//网络事件结构体
struct _TC_NET_EVENT
{
	//0:连接
	//1:服务器作为主动方断开
	//2:客户端主动断开
	//3:错误断开
	//4:接收到来自服务器的数据
	int type;//事件类型
	char* data;//数据
	int len;//数据长度
};

class CTcpClient
{
	//发数据结构体
	struct _SEND_DATA
	{
		int allsize;//数据包的大小
		char* data;//数据
		int sendsize;//已经发送的数据的大小
	};
	//接收数据结构体
	struct _RECV_DATA
	{
		int allsize;//数据包总长度
		int allsize_bytes;//长度的有效字节数
		char* data;//数据
		int recvsize;//已经接收的长度
	};
	int m_State;//状态
	int m_BeginTime;//连接起始时间
	int m_WaitTime;//等待连接时间
	SOCKET m_Socket;//套接字
	//网络事件队列
	CQueue<_TC_NET_EVENT> m_TCNetEvent;
	//内存池
	CMemoryPool m_MP;
	//发数据队列
	CQueue<_SEND_DATA> m_SD_Queue;
	//接收数据
	_RECV_DATA m_RD;

	void _OperateData(const char* buf,
		int r,_RECV_DATA* rd);
	int _RecvData();
	int _SendData();
	void _Close(int type);
public:
	CTcpClient();
	//根据IP连接
	bool ConnectByIP(
		const char* IP,//ip地址
		unsigned short port,//端口号
		int start_ka_time,//心跳包启动时间
		int interval_ka_time,//心跳包间隔时间
		int wait_time);//等待连接时间
	//根据域名连接
	bool ConnectByName(
		const char* name,//域名
		unsigned short port,//端口号
		int start_ka_time,//心跳包启动时间
		int interval_ka_time,//心跳包间隔时间
		int wait_time);//等待连接时间

	//运行
	//1)接收数据
	//2)发送数据
	void Run();

	//得到网络事件
	bool GetTCNetEvent(_TC_NET_EVENT* ptne);

	//释放网络事件中使用的内存
	void ReleaseTCNetEvent(_TC_NET_EVENT* ptne);
	//断开连接
	bool Disconnect();
	//发送数据
	bool SendData(const char* data,int len);
};
#endif