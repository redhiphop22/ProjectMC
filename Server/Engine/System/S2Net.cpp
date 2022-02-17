#include "S2Engine.h"
#include "S2Net.h"

namespace s2 {

bool S2Net::Create()
{
	WSADATA wsaData;
	if(0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		//S2TRACE(_T("WSAStartup Failed. \n"));
		return false;
	}

	if (wsaData.wVersion != MAKEWORD(2, 2))
	{
		::WSACleanup();
		//S2TRACE(_T("WSAStart Version Failed. \n"));
		return false;
	}

	return true;
}

void S2Net::Destroy()
{
	WSACleanup();
}

//BOOL S2Net::GetMyAddress( TCHAR * pAdress)
//{
//	struct	hostent *pHosten = NULL;
//	TCHAR	strHostName[ 30 ];
//	if( SOCKET_ERROR != gethostname( strHostName, 30 ) )
//	{
//		pHosten = gethostbyname((const char FAR*)szHostName);
//		if(pHosten)
//		{	
//			memcpy( pAdress ,pHosten->h_addr,4);
//		}else return FALSE; 
//	}else return FALSE; 
//	return TRUE; 
//}

void S2Net::DestroySocket(SOCKET hSocket)
{
	if (INVALID_SOCKET != hSocket)
	{
		::shutdown(hSocket, SD_BOTH);
		::closesocket(hSocket);
		hSocket = INVALID_SOCKET;
	}
}

uint32_t S2Net::GetIPToLong(const char* strIP)
{
	uint32_t numIP = 0;
	uint32_t tempIP = 0;
	while(*strIP)
	{
		if ('.' == *strIP)
		{
			numIP = (numIP << 8) | tempIP;
			tempIP = 0;
		}
		else
		{
			tempIP = (tempIP*10) + (*strIP - '0');
		}
		++strIP;
	}

	numIP = (numIP << 8) | tempIP;

	return numIP;
}

uint32_t S2Net::GetIPToLong(const wchar_t* strIP)
{
	uint32_t numIP = 0;
	uint32_t tempIP = 0;
	while (*strIP)
	{
		if ('.' == *strIP)
		{
			numIP = (numIP << 8) | tempIP;
			tempIP = 0;
		}
		else
		{
			tempIP = (tempIP * 10) + (*strIP - '0');
		}
		++strIP;
	}

	numIP = (numIP << 8) | tempIP;

	return numIP;
}

//void S2Net::GetIPToLongA(uint32_t ui32IP, char* strIP)
//{
//	int32_t i32Count = 0;
//	int32_t i32Count2;
//	uint32_t ui32Temp;
//	char strTemp[4];
//
//	
//	for (int32_t i = 0; i < 4; i++)
//	{
//		ui32Temp = (ui32IP >> (8 * i)) & 0x000000FF;
//
//		i32Count2 = 0;
//		do
//		{
//			strTemp[i32Count2] = (ui32Temp % 10) + '0';
//			ui32Temp /= 10;
//			i32Count2++;
//		} while (0 < ui32Temp);
//
//		do
//		{
//			i32Count2--;
//			*strIP = strTemp[i32Count2];
//			strIP++;
//		} while (0 < i32Count2);
//		*strIP = '.';
//		strIP++;
//	}
//	*strIP = '\0';
//}
//
//void S2Net::GetIPToLongW(uint32_t ui32IP, wchar_t* wstrIP)
//{
//	int32_t i32Count = 0;
//	int32_t i32Count2;
//	uint32_t ui32Temp;
//	wchar_t wstrTemp[4];
//
//	for (int32_t i = 0; i < 4; i++)
//	{
//		ui32Temp = (ui32IP >> (8 * i)) & 0x000000FF;
//
//		i32Count2 = 0;
//		do
//		{
//			wstrTemp[i32Count2] = (ui32Temp % 10) + L'0';
//			ui32Temp /= 10;
//			i32Count2++;
//		} while (0 < ui32Temp);
//
//		do
//		{
//			i32Count2--;
//			wstrIP[i32Count] = wstrTemp[i32Count2];
//			i32Count++;
//		} while (0 < i32Count2);
//		wstrIP[i32Count] = '.';
//		i32Count++;
//	}
//	wstrIP[i32Count - 1] = L'\0';
//}

}