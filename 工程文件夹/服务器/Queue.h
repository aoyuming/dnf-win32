#ifndef _QUEUE_H_
#define _QUEUE_H_

//template <typename T> //T必须支持<运算符，且T是键值
//class CQueue
//{
//	int m_Size;
//	int m_Length;
//	T* m_Data;
//
//public:
//	CQueue(int Size = 8);
//	CQueue(const CQueue& that);
//	CQueue& operator = (const CQueue& that);
//	~CQueue();
//
//	void Enter(T data);
//	bool Quit(T* pdata);
//	bool GetHead(T* pdata);
//	int Length();
//	void Clear();
//};
//
//template <typename T>
//CQueue<T>::CQueue(int Size)
//:
//m_Size(Size),
//m_Length(0),
//m_Data(new T[m_Size]),
//m_Head(0),
//m_Tail(0)
//{}
//
//template <typename T>
//CQueue<T>::CQueue(const CQueue& that)
//:
//m_Size(that.m_Size),
//m_Length(that.m_Length),
//m_Data(new T[m_Size]),
//m_Head(that.m_Head),
//m_Tail(that.m_Tail)
//{
//	int j = m_Head;
//	for (int i = 0; i < m_Length; ++i)
//	{
//		if(j >= m_Size)
//			j=0;
//		m_Data[j] = that.m_Data[j];
//		j++;
//	}
//}
//
//template <typename T>
//CQueue<T>& CQueue<T>::operator = (const CQueue& that)
//{
//	if (this != &that)
//	{
//		delete [] m_Data;
//		m_Size = that.m_Size;
//		m_Length = that.m_Length;
//		m_Data = new T[m_Size];
//
//		int j = m_Head;
//		for (int i = 0; i < m_Length; ++i)
//		{
//			if(j >= m_Size)
//				j=0;
//			m_Data[j] = that.m_Data[j];
//			j++;
//		}
//	}
//	return *this;
//}
//
//template <typename T>
//CQueue<T>::~CQueue()
//{
//	delete [] m_Data;
//}
//
//template <typename T>
//void CQueue<T>::Enter(T data)
//{
//	if (m_Length == m_Size)
//	{
//		T* p = new T[m_Size *= 2];
//		int j = m_Head;
//		for (int i = 0; i < m_Length; ++i)
//		{
//			if(j >= m_Size / 2)
//			{
//				j = 0;
//			}
//			p[j] = m_Data[j];
//			j++;
//		}
//		delete [] m_Data;
//		m_Data = p;
//	}
//	m_Data[m_Tail++] = data;
//	m_Length++;
//}	
//
//template <typename T>
//bool CQueue<T>::Quit(T* pdata)
//{
//	if (m_Length > 0)
//	{
//		*pdata = m_Data[m_Head++];
//		m_Length -= 1;
//
//		if(m_Head  >= m_Size)
//			m_Head = 0;
//		return true;
//	}
//	else
//		return false;
//}
//
//template <typename T>
//bool CQueue<T>::GetHead(T* pdata)
//{
//	if (m_Length > 0)
//	{
//		*pdata = m_Data[m_Head];
//		return true;
//	}
//	else
//		return false;
//}
//
//template <typename T>
//int CQueue<T>::Length()
//{
//	return m_Length;
//}
//
//template <typename T>
//void CQueue<T>::Clear()
//{
//	m_Length = 0;
//	m_Head = m_Tail = 0;
//}

template <typename T> //T必须支持<运算符，且T是键值
class CQueue
{
	int m_Size;
	int m_Length;
	T* m_Data;
	int m_Head, m_Tail; //队头下标、队尾下标

public:
	CQueue(int Size = 32);
	CQueue(const CQueue& that);
	CQueue& operator = (const CQueue& that);
	~CQueue();

	void Enter(T data);
	bool Quit(T* pdata);
	bool GetHead(T* pdata);
	int Length();
	void Clear();
};

template <typename T>
CQueue<T>::CQueue(int Size)
:
m_Size(Size),
m_Length(0),
m_Data(new T[m_Size]),
m_Head(0),
m_Tail(0)
{}

template <typename T>
CQueue<T>::CQueue(const CQueue& that)
:
m_Size(that.m_Size),
m_Length(that.m_Length),
m_Data(new T[m_Size]),
//m_Head(that.m_Head),
//m_Tail(that.m_Tail)
//{
//	for (int i = 0; i < m_Length; ++i)
//		m_Data[(m_Head + i) % m_Size] = that.m_Data[(that.m_Head + i) % that.m_Size];
//}
m_Head(0),
m_Tail(m_Length)
{
	for (int i = 0; i < m_Length; ++i)
		m_Data[i] = that.m_Data[(that.m_Head + i) % that.m_Size];
}

template <typename T>
CQueue<T>& CQueue<T>::operator = (const CQueue& that)
{
	if (this != &that)
	{
		delete [] m_Data;
		m_Size = that.m_Size;
		m_Length = that.m_Length;
		m_Data = new T[m_Size];
		m_Head = 0;
		m_Tail = m_Length;
		for (int i = 0; i < m_Length; ++i)
			m_Data[i] = that.m_Data[(that.m_Head + i) % that.m_Size];
	}
	return *this;
}

template <typename T>
CQueue<T>::~CQueue()
{
	delete [] m_Data;
}

template <typename T>
void CQueue<T>::Enter(T data)
{
	if (m_Length == m_Size)
	{
		T* p = new T[m_Size *= 2];
		for (int i = 0; i < m_Length; ++i)
			p[i] = m_Data[(m_Head + i) % m_Length];
		delete [] m_Data;
		m_Data = p;
		m_Head = 0;
		m_Tail = m_Length;
	}
	m_Data[m_Tail] = data;
	m_Tail = (m_Tail + 1) % m_Size;
	m_Length++;
}	

template <typename T>
bool CQueue<T>::Quit(T* pdata)
{
	if (m_Length > 0)
	{
		*pdata = m_Data[m_Head];
		m_Head = (m_Head + 1) % m_Size;
		m_Length -= 1;
		return true;
	}
	else
		return false;
}

template <typename T>
bool CQueue<T>::GetHead(T* pdata)
{
	if (m_Length > 0)
	{
		*pdata = m_Data[m_Head];
		return true;
	}
	else
		return false;
}

template <typename T>
int CQueue<T>::Length()
{
	return m_Length;
}

template <typename T>
void CQueue<T>::Clear()
{
	m_Length = 0;
	m_Head = m_Tail = 0;
}

#endif