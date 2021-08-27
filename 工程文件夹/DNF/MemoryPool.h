#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

class CMemoryPool
{
	struct _NODE
	{
		_NODE* pre; //前序节点指针
		_NODE* next; //后序节点指针
		short use; //使用与否
		short head; //是否是内存块首节点
		int size; //内存块大小
	};
	//struct _NODE
	//{
	//	_NODE* pre; //前序节点指针
	//	_NODE* next; //后序节点指针
	//	int use; //使用与否
	//	int size; //内存块大小
	//};
	int m_Size;
	int m_UseByte;
	_NODE* m_First;

	CMemoryPool(const CMemoryPool& that);
	CMemoryPool& operator = (const CMemoryPool& that);

public:
	CMemoryPool(int Size = 1024 * 1024 * 4);
	~CMemoryPool();

	void* Allocate(int Size);
	void Free(void* Mem);
	float GetUseMemory(int type = 2); //0123代表BYTE、KB、MB、GB
};

#endif