#pragma once

#include <string>

namespace s2 {
namespace string {

	template<typename ... Args>
	size_t Format(std::string& outString, const char* fmt, Args ... args)
	{
		size_t size = snprintf(nullptr, 0, fmt, args ...); // Extra space for '\0' 
		if (0 < size)
		{
			size += 1;
			outString.resize(size);
			std::snprintf(&outString[0], size, fmt, args ...);
		}
		return size;
	}
	template<typename ... Args>
	size_t Format(std::wstring& outString, const wchar_t* fmt, Args ... args)
	{
		size_t size = swprintf(nullptr, 0, fmt, args ...); // Extra space for '\0' 
		if (0 < size)
		{
			size += 1;
			outString.resize(size);
			std::swprintf(&outString[0], size, fmt, args ...);
		}
		return size;
	}
	template<typename ... Args>
	size_t Format(char* outString, size_t maxSize, const char* fmt, Args ... args)
	{
		size_t size = snprintf(outString, maxSize, fmt, args ...);
		outString[size] = '\0';
		return size;
	}
	template<typename ... Args>
	size_t Format(wchar_t* outString, size_t maxSize, const wchar_t* fmt, Args ... args)
	{
		size_t size = swprintf(&outString[0], maxSize, fmt, args ...);
		outString[size] = '\0';
		return size;
	}

	char* strcpy(char* dest, size_t count, const char* src);
	wchar_t* strcpy(wchar_t* dest, size_t count, const wchar_t* src);

	// 대문자 -> 소문자 변환
	char* tolower(char* str);
	// 소문자 -> 대문자 변환
	char* toupper(char* str);

}
}