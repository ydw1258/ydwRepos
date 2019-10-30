#include "FileIO.h"



FileIO::FileIO()
{
}


FileIO::~FileIO()
{
}


void FileIO::Open(const char* wcFileName, bool bRead)
{
	if (bRead)
	{
		m_hFile = CreateFile(wcFileName, GENERIC_READ, 0, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	}
	else
	{
		m_hFile = CreateFile(wcFileName, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	}
}

int FileIO::Read(void* pData, const int nSize)
{
	DWORD	dwTemp = 0;
	ReadFile(m_hFile, pData, nSize, &dwTemp, NULL);

	return dwTemp;
}

bool FileIO::ReadLine(char* pData)
{
	const int nSize = 1024;
	char		szLine[nSize] = { 0, };

	int nRealSize = Read(szLine, nSize);

	if (nRealSize == 0)
		return false;

	int		nReadSize = 0;
	for (int i = 0; i < nRealSize; i++)
	{
		//엔터기호		0x0D 0x0A	
		if (szLine[i] == 0x0D && szLine[i + 1] == 0x0A)
		{
			memcpy(pData, szLine, i);
			pData[i] = 0;
			nReadSize = i;
			break;
		}
	}

	int nOffset = nRealSize - (nReadSize + 2);
	DWORD dwFF = SetFilePointer(m_hFile, -nOffset, 0, FILE_CURRENT);
	if (dwFF == INVALID_SET_FILE_POINTER)
		return false;

	return true;
}

bool FileIO::Write(void* pData, int nSize)
{
	DWORD	dwTemp = 0;
	WriteFile(m_hFile, pData, nSize, &dwTemp, NULL);

	if (dwTemp == 0)
		return false;

	return true;
}

void FileIO::Close()
{
	CloseHandle(m_hFile);
}