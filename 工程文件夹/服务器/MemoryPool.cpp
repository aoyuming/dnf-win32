#include "MemoryPool.h"
#include <iostream>

#define _NO_USE  0//δʹ��
#define _IN_USE  1//��ʹ��
#define _NO_HEAD 0//�����׽ڵ�
#define _IS_HEAD 1//���׽ڵ�

CMemoryPool::CMemoryPool(const CMemoryPool& that)
{}
CMemoryPool& CMemoryPool::operator = (const CMemoryPool& that)
{
	return *this;
}

CMemoryPool::CMemoryPool(int Size)
:
m_Size(Size),
m_UseByte(0),
m_First((_NODE*)malloc(m_Size))
{
	m_First->pre = 0;
	m_First->next = 0;
	m_First->use = _NO_USE;
	m_First->head = _IS_HEAD;
	m_First->size = m_Size - sizeof(_NODE);
}
CMemoryPool::~CMemoryPool()
{
	//free(m_First);
	_NODE* p = m_First;
	while(p)
	{
		_NODE* q = p->next;
		while(q && q->head == _NO_HEAD)
		{
			q = q->next;
		}
		free(p);
		p = q;
	}
}

//���ڴ���п����ڴ�
void* CMemoryPool::Allocate(int Size)
{
	//�õ�һ�����Է����ڴ���ڴ�飬���½ڵ��ǰ��ڵ�
	_NODE* p = NULL,*last = NULL;
	for( p = m_First; p ; p = p->next )
	{
		last = p;
		if(p->use == _NO_USE && p->size >= Size + (int)sizeof(_NODE))
		{
			break;
		}
	}
	m_UseByte += Size + (int)sizeof(_NODE);
	if(p)
	{
		//�õ��µĽڵ�
		_NODE* q = (_NODE*)((char*)p + p->size - Size);
		//���ýڵ�
		q->pre = p;
		q->next = p->next;
		q->use = _IN_USE;
		q->head = _NO_HEAD;
		q->size = Size;
		//�����½ڵ��ǰ��ڵ�
		p->next = q;
		p->size -= Size + (int)sizeof(_NODE);
		//�����½ڵ�ĺ���ڵ��ǰ��ڵ�
		if(q->next)
			q->next->pre = q;
		return q + 1;
	}
	else
	{
		//�����µ��ڴ��
		m_Size *= 2;
		m_Size += 2 * sizeof(_NODE) + Size;
		_NODE* q = (_NODE*)malloc(m_Size);
		//�������ڴ����׽ڵ�
		q->pre = last;
		q->next = 0;
		q->use = _NO_USE;
		q->head = _IS_HEAD;
		q->size = m_Size - (int)sizeof(_NODE);
		last->next = q;
		//�����½ڵ�
		_NODE* r = (_NODE*)((char*)q + q->size - Size);
		r->pre = q;
		r->next = q->next;
		r->use = _IN_USE;
		r->head = _NO_HEAD;
		r->size = Size;
		//�����½ڵ��ǰ��ڵ�
		q->next = r;
		q->size -= Size + (int)sizeof(_NODE);
		return r + 1;
	}
}

//�ͷ��ڴ����û���õ��ڴ�
void CMemoryPool::Free(void* Mem)
{
	//�ж�Mem�Ƿ��Ǳ��ڴ�ط�����ڴ�
	_NODE* p = NULL;
	for( p = m_First; p ; p = p->next )
	{
		if(p + 1 == Mem)
			break;
	}

	//��������Ƿ���ģ���һ������ǰ��ڵ�
	//����һ���к���ڵ�
	if(p)
	{
		m_UseByte -= p->size + (int)sizeof(_NODE);
		//���ñ��ڵ�
		p->use = _NO_USE;
		_NODE* pre = p->pre;
		_NODE* next = p->next;
		//���ǰ��ڵ�δʹ��
		if(pre->use == _NO_USE)
		{
			//�ϲ�ǰ��ڵ�ͱ��ڵ�
			pre->next = p->next;
			pre->size += p->size + (int)sizeof(_NODE);
			if(next)
				next->pre = pre;
			//����p
			p = pre;
		}
		//����к���Ľڵ㲢��û��ʹ��
		if(next && next->use == _NO_USE && next->head == _NO_HEAD)
		{
			p->next = next->next;
			p->size += next->size + (int)sizeof(_NODE);
			if(p->next)
				p->next->pre = p;
		}
	}
	else
		free(Mem);
}

float CMemoryPool::GetUseMemory(int byte)
{
	switch(byte)
	{
	case 1:return m_UseByte/1024.0f;
	case 2:return m_UseByte/(1024.0f * 1024);
	case 3:return m_UseByte/(1024.0f * 1024 * 1024);
	}
	return (float)m_UseByte;
}