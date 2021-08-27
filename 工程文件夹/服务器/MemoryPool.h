#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

class CMemoryPool
{
	struct _NODE
	{
		_NODE* pre;//ǰ��ڵ�ָ��
		_NODE* next;//����ڵ�ָ��
		short use;//ʹ�����
		short head;//�Ƿ����ڴ���׽ڵ�
		int size;//�ڴ���С
	};
	int m_Size;//�ڴ��С
	int m_UseByte;//�Ѿ�������ֽ�
	_NODE* m_First;//�ڵ�ָ��
		
	CMemoryPool(const CMemoryPool& that);
	CMemoryPool& operator = (const CMemoryPool& that);
public:
	CMemoryPool(int Size = 1024 * 1024 * 4);
	~CMemoryPool();

	//���ڴ���п����ڴ�
	void* Allocate(int Size);

	//�ͷ��ڴ����û���õ��ڴ�
	void Free(void* Mem);
	
	//��ȡʹ�õ��ڴ�
	//0123����BYTE,KB,MB,GB
	float GetUseMemory(int byte = 2);
};

#endif