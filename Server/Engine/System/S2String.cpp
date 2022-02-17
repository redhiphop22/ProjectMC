#include "S2Engine.h"
#include "S2String.h"

namespace s2 {
namespace string {

char* strcpy(char* dest, size_t count, const char* src)
{
	strcpy_s(dest, count, src);
	return dest;
}

wchar_t* strcpy(wchar_t* dest, size_t count, const wchar_t* src)
{
	wcscpy_s(dest, count, src);
	return dest;
}

char* tolower(char* str)
{
	while('\0' != *str)
	{
		if('A' > *str || 'Z' < *str)
		{
			++str;
			continue;
		}
		*str += 32;
		++str;
	}
	return str;
}

char* toupper(char* str)
{
	while('\0' != *str)
	{
		if('a' > *str || 'z' < *str)
		{
			++str;
			continue;
		}
		*str -= 32;
		++str;
	}
	return str;
}

}
}