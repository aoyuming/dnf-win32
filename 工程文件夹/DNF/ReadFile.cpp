#include "ReadFile.h"
#include <iostream>

CReadFile::CReadFile()
:
m_iFileSize(0),
m_iFilePosition(0),
m_pFileData(0)
{}

CReadFile::CReadFile(const CReadFile& that)
:
m_iFileSize(that.m_iFileSize),
m_iFilePosition(that.m_iFilePosition),
m_pFileData(m_iFileSize > 0 ? malloc(m_iFileSize) : 0)
{
	if (m_iFileSize > 0)
	{
		for (int i = 0; i < m_iFileSize; ++i)
			((char*)m_pFileData)[i] = ((char*)that.m_pFileData)[i];
	}
}

CReadFile& CReadFile::operator = (const CReadFile& that)
{
	if (this != &that)
	{
		if (m_iFileSize > 0 && that.m_iFileSize > 0)
		{
			if (m_iFileSize < that.m_iFileSize)
			{
				free(m_pFileData);
				m_pFileData = malloc(that.m_iFileSize);
			}
			m_iFileSize = that.m_iFileSize;
			for (int i = 0; i < m_iFileSize; ++i)
				((char*)m_pFileData)[i] = ((char*)that.m_pFileData)[i];
			m_iFilePosition = that.m_iFilePosition;
		}
		else if (m_iFileSize == 0 && that.m_iFileSize > 0)
		{
			m_iFilePosition = that.m_iFilePosition;
			m_pFileData = malloc(m_iFileSize = that.m_iFileSize);
			for (int i = 0; i < m_iFileSize; ++i)
				((char*)m_pFileData)[i] = ((char*)that.m_pFileData)[i];
		}
		else if (m_iFileSize > 0 && that.m_iFileSize == 0)
		{
			free(m_pFileData);
			m_iFileSize = 0;
			m_iFilePosition = 0;
			m_pFileData = 0;
		}
	}
	return *this;
}

CReadFile::~CReadFile()
{
	if (m_pFileData)
		free(m_pFileData);
}

bool CReadFile::Open(const char* FileName)
{
	FILE* pFile;
	fopen_s(&pFile, FileName, "rb");
	if (!pFile)
		return false;

	if (m_pFileData)
	{
		free(m_pFileData);
		m_iFilePosition = 0;
	}

	fseek(pFile, 0, SEEK_END);
	m_iFileSize = ftell(pFile);
	rewind(pFile);

	m_pFileData = malloc(m_iFileSize);
	fread(m_pFileData, 1, m_iFileSize, pFile);
	fflush(pFile);
	fclose(pFile);

	return true;
}

const void* CReadFile::GetData()
{
	return m_pFileData;
}

int CReadFile::GetSize()
{
	return m_iFileSize;
}

int CReadFile::GetPosition()
{
	return m_iFilePosition;
}

bool CReadFile::SetPosition(int Position)
{
	if (Position < 0 || Position > m_iFileSize - 1)
		return false;

	m_iFilePosition = Position;

	return true;
}

bool CReadFile::ReadString(char* String)
{
	const char* p = (char*)m_pFileData + m_iFilePosition;
	int c = 0;
	for (; p[c]; ++c)
		;

	if (m_iFilePosition + c + 1 > m_iFileSize)
		return false;

	for (c = 0; String[c] = p[c]; ++c)
		;
	m_iFilePosition += c + 1;

	return true;
}

bool CReadFile::ReadMemory(void* Address, int Count)
{
	if (m_iFilePosition + Count > m_iFileSize)
		return false;

	const char* p = (char*)m_pFileData + m_iFilePosition;
	for (int i = 0; i < Count; ++i)
		((char*)Address)[i] = p[i];
	m_iFilePosition += Count;

	return true;
}