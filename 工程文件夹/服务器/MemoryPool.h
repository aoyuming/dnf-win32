#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

class CMemoryPool
{
	struct _NODE
	{
		_NODE* pre;//前序节点指针
		_NODE* next;//后序节点指针
		short use;//使用与否
		short head;//是否是内存块首节点
		int size;//内存块大小
	};
	int m_Size;//内存大小
	int m_UseByte;//已经分配的字节
	_NODE* m_First;//节点指针
		
	CMemoryPool(const CMemoryPool& that);
	CMemoryPool& operator = (const CMemoryPool& that);
public:
	CMemoryPool(int Size = 1024 * 1024 * 4);
	~CMemoryPool();

	//在内存池中开辟内存
	void* Allocate(int Size);

	//释放内存池中没有用的内存
	void Free(void* Mem);
	
	//获取使用的内存
	//0123代表BYTE,KB,MB,GB
	float GetUseMemory(int byte = 2);
};

#endif