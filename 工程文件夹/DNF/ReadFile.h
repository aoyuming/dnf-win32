#ifndef _READ_FILE_H_
#define _READ_FILE_H_

class CReadFile
{
private:
	int m_iFileSize;
	int m_iFilePosition;
	void* m_pFileData;

public:
	CReadFile();
	CReadFile(const CReadFile& that);
	CReadFile& operator = (const CReadFile& that);
	~CReadFile();

	bool Open(const char* FileName);
	const void* GetData();
	int GetSize();
	int GetPosition();
	bool SetPosition(int Position);

	template <typename T> bool ReadType(T* pD);
	bool ReadString(char* String);
	bool ReadMemory(void* Address, int Count);
};

template <typename T> bool CReadFile::ReadType(T* pD)
{
	if (m_iFilePosition + (int)sizeof(T) > m_iFileSize)
		return false;

	*pD = *((T*)((char*)m_pFileData + m_iFilePosition));
	m_iFilePosition += sizeof(T);

	return true;
}

#endif