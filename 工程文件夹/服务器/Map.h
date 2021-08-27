#ifndef _MAP_H_
#define _MAP_H_

template <typename KEY, typename DATA>
class CMap //KEY不能有重复且必须支持<运算符，DATA可以有重复
{
public:
	struct PAIR //键值对
	{
		KEY key;
		DATA data;
	};

private:
	int m_Size; //容量
	int m_Length;  //长度
	PAIR* m_Data; //数据地址

public:
	CMap();
	CMap(const CMap& that);
	CMap& operator = (const CMap& that);
	~CMap();

	bool Insert(KEY key, DATA data);
	bool Erase(KEY key);
	PAIR* Find(KEY key);
	PAIR* At(int index);
	bool Erase(int index);
	void Clear();
	int Length();
};

template <typename KEY, typename DATA>
CMap<KEY, DATA>::CMap()
:
m_Size(8),
m_Length(0),
m_Data(new PAIR[m_Size])
{}

template <typename KEY, typename DATA>
CMap<KEY, DATA>::CMap(const CMap& that)
:
m_Size(that.m_Size),
m_Length(that.m_Length),
m_Data(new PAIR[m_Size])
{
	for (int i = 0; i < m_Length; ++i)
		m_Data[i] = that.m_Data[i];
}

template <typename KEY, typename DATA>
CMap<KEY, DATA>& CMap<KEY, DATA>::operator = (const CMap& that)
{
	if (this != &that)
	{
		delete [] m_Data;
		m_Size = that.m_Size;
		m_Length = that.m_Length;
		m_Data = new PAIR[m_Size];
		for (int i = 0; i < m_Length; ++i)
			m_Data[i] = that.m_Data[i];
	}

	return *this;
}

template <typename KEY, typename DATA>
CMap<KEY, DATA>::~CMap()
{
	delete [] m_Data;
}

template <typename KEY, typename DATA>
bool CMap<KEY, DATA>::Insert(KEY key, DATA data)
{
	int begin = 0, end = m_Length - 1;
	while (begin <= end)
	{
		int half = (begin + end) / 2;
		if (key < m_Data[half].key)
			end = half - 1;
		else if (m_Data[half].key < key)
			begin = half + 1;
		else
			return false;
	}

	if (m_Length == m_Size)
	{
		PAIR* p = new PAIR[m_Size *= 2];
		for (int i = 0; i < m_Length; ++i)
			p[i] = m_Data[i];
		delete [] m_Data;
		m_Data = p;
	}

	int m = m_Length - begin;
	for (int i = 0; i < m; ++i)
		m_Data[m_Length - i] = m_Data[m_Length - 1 - i];
	m_Data[begin].key = key;
	m_Data[begin].data = data;
	m_Length += 1;

	return true;
}

template <typename KEY, typename DATA>
bool CMap<KEY, DATA>::Erase(KEY key)
{
	int begin = 0, end = m_Length - 1;
	while (begin <= end)
	{
		int half = (begin + end) / 2;
		if (key < m_Data[half].key)
			end = half - 1;
		else if (m_Data[half].key < key)
			begin = half + 1;
		else
		{
			int m = m_Length - half - 1;
			for (int i = 0; i < m; ++i)
				m_Data[half + i] = m_Data[half + 1 + i];
			m_Length -= 1;

			return false;
		}
	}
	return false;
}

template <typename KEY, typename DATA>
typename CMap<KEY, DATA>::PAIR* CMap<KEY, DATA>::Find(KEY key)
{
	int begin = 0, end = m_Length - 1;
	while (begin <= end)
	{
		int half = (begin + end) / 2;
		if (key < m_Data[half].key)
			end = half - 1;
		else if (m_Data[half].key < key)
			begin = half + 1;
		else
			return &m_Data[half];
	}
	return 0;
}

template <typename KEY, typename DATA>
typename CMap<KEY, DATA>::PAIR* CMap<KEY, DATA>::At(int index)
{
	if (index >= 0 && index < m_Length)
		return &m_Data[index];
	else
		return 0;
}

template <typename KEY, typename DATA>
bool CMap<KEY, DATA>::Erase(int index)
{
	if (index < 0 || index >= m_Length)
		return false;

	int m = m_Length - index - 1;
	for (int i = 0; i < m; ++i)
		m_Data[index + i] = m_Data[index + 1 + i];
	m_Length -= 1;

	return true;
}

template <typename KEY, typename DATA>
void CMap<KEY, DATA>::Clear()
{
	m_Length = 0;
}

template <typename KEY, typename DATA>
int CMap<KEY, DATA>::Length()
{
	return m_Length;
}

#endif