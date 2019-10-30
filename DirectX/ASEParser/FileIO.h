#pragma once
#include <Windows.h>

class FileIO
{
protected:
	HANDLE		m_hFile;

public:
	FileIO(void);
	~FileIO(void);

	void	Open(const char* wcFileName, bool bRead);
	int		Read(void* pData, const int nSize);
	bool	ReadLine(char* pData);
	bool	Write(void* pData, int nSize);
	void	Close();
};

