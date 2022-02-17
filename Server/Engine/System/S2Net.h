#pragma once

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <mswsock.h>
#pragma comment(lib, "Mswsock.lib")

namespace s2 {

typedef uint16_t				packet_size_t;
typedef uint16_t				packet_protocol_t;
typedef uint16_t				packet_seed_t;

#define PACKET_HEAD_SIZE		6			//((sizeof(s2::packet_size_t) + sizeof(s2::packet_protocol_t) + sizeof(s2::packet_seed_t))

#define S2_PACKET_MAX_SIZE		8192
#define S2_PACKET_BUFFER_SIZE	8192 * 3

namespace S2Net
{
	bool			Create();
	void			Destroy();

	//BOOL			GetMyAddress( TCHAR * pAdress );

	void			DestroySocket(SOCKET hSocket);

	//char*		GetMyAddressList( INT32 * pOutCount);
	//void			GetName( INT32 Socket, SOCKADDR_IN * plocaladdr ); 
	//INT32		GetMACAddress(char* pMAC,INT32 nNICCount = 1);
	//INT32		GetMACAddress(UINT64 * pMAC,INT32 nNICCount = 1);
	//INT32		GetError( char * pString );
	uint32_t		GetIPToLong(const char* strIP);
	uint32_t		GetIPToLong(const wchar_t* wstrIP);
	//void			GetIPToLongA(uint32_t ui32IP, char* strIP);
	//void			GetIPToLongW(uint32_t ui32IP, wchar_t* wstrIP);
}
}