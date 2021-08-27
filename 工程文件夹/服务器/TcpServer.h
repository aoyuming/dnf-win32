#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#define _WIN32_WINNT  0x0500
#include <Winsock2.h>
#include "MemoryPool.h"
#include "Queue.h"
#include "Map.h"

#define _NE_CONNECT			0 //连接
#define _NE_S_DISCONNECT	1//服务器作为主动方断开
#define _NE_C_DISCONNECT	2//客户端主动断开
#define _NE_E_DISCONNECT	3//错误断开
#define _NE_DATA			4//接收到来自客户端的数据

//网络事件结构体
struct _TS_NET_EVENT
{
	//0:连接
	//1:服务器作为主动方断开
	//2:客户端主动断开
	//3:错误断开
	//4:接收到来自客户端的数据
	int type;//事件类型
	SOCKET s;//客户端标记
	char* data;//数据
	int len;//数据长度
	unsigned long ip;//客户IP地址
};

class CTcpServer
{
	//端口号
	unsigned short m_Port;
	//最大连接数
	int m_MaxClient;
	//心跳包启动时间
	int m_StartKAYime;
	//心跳包间隔时间
	int m_IntervalKAYime;
	//循环标记
	bool m_Continue;
	//监听套接字
	SOCKET m_Listen;
	//网络事件队列
	CQueue<_TS_NET_EVENT> m_TSNetEvent;
	//发送数据结构体
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
	//客户端数据
	//{
	//  状态:连接，准备主动断开，被动断开，等待结束
	//  ip地址
	//  发送数据队列
	//	接收数据
	//}
	struct CLIENT
	{
		int state;
		unsigned long ip;
		CQueue<_SEND_DATA> sd_queue;
		_RECV_DATA rd;
	};
	//客户端映射<SOCKET,客户端数据>;
	CMap<SOCKET,CLIENT> m_Client;
	//内存池
	CMemoryPool m_MP;

	//
	int  _AcceptClient();
	void _OperateData(const char* buf,
		int r,SOCKET s,unsigned long ip,_RECV_DATA* rd);
	int _RecvData();
	int _SendData();
	void _Close(int index,SOCKET s,CLIENT* c,int type);

public:
	//整数IP转字符串IP
	static void GetStrIP(unsigned long ul_ip,char* str_ip);

	float GetUseMemoryMB();

	bool Init(
		unsigned short port,//端口号
		int max_client,//最大连接数
		int start_ka_time,//心跳包启动时间
		int interval_ka_time);//心跳包间隔时间
	//终止
	void Stop();
	//循环标记
	bool Continue();
	//结束
	void End();
	//运行
	//1)接收连接
	//2)接收数据
	//3)发送数据
	void Run();
	//得到网络事件
	bool GetTSNetEvent(_TS_NET_EVENT* ptne);
	//释放网络事件中使用的内存
	void ReleaseTSNetEvent(_TS_NET_EVENT* ptne);
	//断开连接
	int Disconnect(SOCKET s);
	//发送数据
	int SendData(SOCKET s,const char* data,int len);
};

#endif