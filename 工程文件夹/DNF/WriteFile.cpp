#include "WriteFile.h"
#include <iostream>

void CWriteFile::_AddMemory(int AddSize)
{
	if (m_iFileLength + AddSize >= m_iFileSize)
	{
		char* p = (char*)malloc(m_iFileSize += 8 * AddSize);
		char* q = (char*)m_pFileData;

		for (int i = 0; i < m_iFileLength; ++i)
			p[i] = q[i];

		free(m_pFileData);

		m_pFileData = p;
	}
}

CWriteFile::CWriteFile()
:
m_iFileSize(0),
m_iFileLength(0),
m_pFileData(0)
{}

CWriteFile::CWriteFile(const CWriteFile& that)
:
m_iFileSize(that.m_iFileSize),
m_iFileLength(that.m_iFileLength),
m_pFileData(m_iFileSize > 0 ? malloc(m_iFileSize) : 0)
{
	if (m_iFileSize > 0)
	{
		for (int i = 0; i < m_iFileLength; ++i)
			((char*)m_pFileData)[i] = ((char*)that.m_pFileData)[i];
	}
}

CWriteFile& CWriteFile::operator = (const CWriteFile& that)
{
	if (this != &that)
	{
		if (m_iFileSize > 0 && that.m_iFileSize > 0)
		{
			if (m_iFileSize != that.m_iFileSize)
			{
				free(m_pFileData);
				m_pFileData = malloc(m_iFileSize = that.m_iFileSize);
			}
			m_iFileLength = that.m_iFileLength;
			for (int i = 0; i < m_iFileLength; ++i)
				((char*)m_pFileData)[i] = ((char*)that.m_pFileData)[i];
		}
		else if (m_iFileSize == 0 && that.m_iFileSize > 0)
		{
			m_pFileData = malloc(m_iFileSize = that.m_iFileSize);
			m_iFileLength = that.m_iFileLength;
			for (int i = 0; i < m_iFileLength; ++i)
				((char*)m_pFileData)[i] = ((char*)that.m_pFileData)[i];
		}
		else if (m_iFileSize > 0 && that.m_iFileSize == 0)
		{
			free(m_pFileData);
			m_iFileSize = 0;
			m_iFileLength = 0;
			m_pFileData = 0;
		}
	}
	return *this;
}

CWriteFile::~CWriteFile()
{
	if (m_pFileData)
		free(m_pFileData);
}

bool CWriteFile::Create()
{
	if (m_pFileData)
		free(m_pFileData);

	m_pFileData = malloc(m_iFileSize = 8);
	m_iFileLength = 0;

	return true;
}

void CWriteFile::Write(const char* FileName)
{
	if (FileName && m_pFileData)
	{
		FILE* pFile;
		fopen_s(&pFile, FileName, "wb");
		fwrite(m_pFileData, 1, m_iFileLength, pFile);
		fflush(pFile);
		fclose(pFile);
	}
}

const void* CWriteFile::GetData()
{
	return m_pFileData;
}

int CWriteFile::GetLength()
{
	return m_iFileLength;
}

void CWriteFile::WriteString(const char* String)
{
	int c = 0;
	for (; String[c]; ++c)
		;

	_AddMemory(c + 1);

	char* p = (char*)m_pFileData + m_iFileLength;
	for (int i = 0; p[i] = String[i]; ++i)
		;
	m_iFileLength += c + 1;
}

void CWriteFile::WriteMemory(void* Address, int Count)
{
	_AddMemory(Count);

	char* p = (char*)m_pFileData + m_iFileLength;
	for (int i = 0; i < Count; ++i)
		p[i] = ((char*)Address)[i];

	m_iFileLength += Count;
}