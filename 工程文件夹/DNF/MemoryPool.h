#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

class CMemoryPool
{
	struct _NODE
	{
		_NODE* pre; //ǰ��ڵ�ָ��
		_NODE* next; //����ڵ�ָ��
		short use; //ʹ�����
		short head; //�Ƿ����ڴ���׽ڵ�
		int size; //�ڴ���С
	};
	//struct _NODE
	//{
	//	_NODE* pre; //ǰ��ڵ�ָ��
	//	_NODE* next; //����ڵ�ָ��
	//	int use; //ʹ�����
	//	int size; //�ڴ���С
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
	float GetUseMemory(int type = 2); //0123����BYTE��KB��MB��GB
};

#endif