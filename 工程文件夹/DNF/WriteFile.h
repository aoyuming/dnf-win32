#ifndef _WRITE_FILE_H_
#define _WRITE_FILE_H_

class CWriteFile
{
private:
	int m_iFileSize;
	int m_iFileLength;
	void* m_pFileData;

	void _AddMemory(int AddSize);

public:
	CWriteFile();
	CWriteFile(const CWriteFile& that);
	CWriteFile& operator = (const CWriteFile& that);
	~CWriteFile();

	bool Create();
	void Write(const char* FileName);
	const void* GetData();
	int GetLength();

	template <typename T> void WriteType(T D);
	void WriteString(const char* String);
	void WriteMemory(void* Address, int Count);
};

template <typename T>
void CWriteFile::WriteType(T D)
{
	_AddMemory(sizeof(T));

	*((T*)((char*)m_pFileData + m_iFileLength)) = D;

	m_iFileLength += sizeof(T);
}

#endif