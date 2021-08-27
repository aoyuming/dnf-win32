#include "MemoryPool.h"
#include <iostream>

#define _NO_USE  0//未使用
#define _IN_USE  1//在使用
#define _NO_HEAD 0//不是首节点
#define _IS_HEAD 1//是首节点

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

//在内存池中开辟内存
void* CMemoryPool::Allocate(int Size)
{
	//得到一个可以分配内存的内存块，即新节点的前序节点
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
		//得到新的节点
		_NODE* q = (_NODE*)((char*)p + p->size - Size);
		//设置节点
		q->pre = p;
		q->next = p->next;
		q->use = _IN_USE;
		q->head = _NO_HEAD;
		q->size = Size;
		//设置新节点的前序节点
		p->next = q;
		p->size -= Size + (int)sizeof(_NODE);
		//设置新节点的后序节点的前序节点
		if(q->next)
			q->next->pre = q;
		return q + 1;
	}
	else
	{
		//创建新的内存块
		m_Size *= 2;
		m_Size += 2 * sizeof(_NODE) + Size;
		_NODE* q = (_NODE*)malloc(m_Size);
		//设置新内存块的首节点
		q->pre = last;
		q->next = 0;
		q->use = _NO_USE;
		q->head = _IS_HEAD;
		q->size = m_Size - (int)sizeof(_NODE);
		last->next = q;
		//设置新节点
		_NODE* r = (_NODE*)((char*)q + q->size - Size);
		r->pre = q;
		r->next = q->next;
		r->use = _IN_USE;
		r->head = _NO_HEAD;
		r->size = Size;
		//设置新节点的前序节点
		q->next = r;
		q->size -= Size + (int)sizeof(_NODE);
		return r + 1;
	}
}

//释放内存池中没有用的内存
void CMemoryPool::Free(void* Mem)
{
	//判断Mem是否是本内存池分配的内存
	_NODE* p = NULL;
	for( p = m_First; p ; p = p->next )
	{
		if(p + 1 == Mem)
			break;
	}

	//如果是我们分配的，则一定是由前序节点
	//但不一定有后序节点
	if(p)
	{
		m_UseByte -= p->size + (int)sizeof(_NODE);
		//设置本节点
		p->use = _NO_USE;
		_NODE* pre = p->pre;
		_NODE* next = p->next;
		//如果前序节点未使用
		if(pre->use == _NO_USE)
		{
			//合并前序节点和本节点
			pre->next = p->next;
			pre->size += p->size + (int)sizeof(_NODE);
			if(next)
				next->pre = pre;
			//更新p
			p = pre;
		}
		//如果有后序的节点并且没有使用
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