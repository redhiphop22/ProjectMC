#include "S2Engine.h"
#include "S2Json.h"
#include "Windows.h"

namespace s2 {

S2Json::S2Json()
{
};

S2Json::~S2Json()
{
}

bool S2Json::Create(const char* fileName)
{
	HANDLE handle = INVALID_HANDLE_VALUE;
	char* file = nullptr;

	auto freeAlloc = [&handle, &file]() -> void
	{
		SAFE_DELETE_ARRAY(file);
		if(INVALID_HANDLE_VALUE != handle)
		{
			::CloseHandle( handle );
			handle = INVALID_HANDLE_VALUE; 
		}
	};

	// Create New File
	handle = ::CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == handle)
	{
		freeAlloc();
		return false;
	}

	int32_t fileSize = GetFileSize(handle, NULL);
	if(0 == fileSize)
	{
		freeAlloc();
		return false;
	}

	file = new char[fileSize+1];
	if(nullptr == file)
	{
		freeAlloc();
		return false;
	}

	::ReadFile(handle, file, fileSize, (DWORD*)&fileSize, NULL );
	file[fileSize] = '\0';

	m_json.Parse(file);
	if(false == m_json.IsObject())
	{
		freeAlloc();
		return false;
	}

	freeAlloc();

	return true;
}

void S2Json::Destroy()
{
	m_json.RemoveAllMembers();
}

}