#pragma once

#include <chrono>
#include <unordered_map>
#include <deque>

//#define USE_CONTROLLER_SINGLE
#define USE_CONTROLLER_MULTI

#define REGIST_PACKET(p) { auto result = p; \
	if(false == result.second) return false; }

typedef uint16_t						packet_size_t;
typedef uint16_t						packet_protocol_t;
typedef uint16_t						packet_seed_t;

typedef uint64_t						uid_t;

#define PACKET_HEAD_SIZE				6	//(sizeof(packet_size_t) + sizeof(packet_protocol_t) + sizeof(packet_seed_t))
#define FBB_BASIC_SIZE					1024
#define PACKET_BUFFER_SIZE				4096
#define CLIENT_SOCKET_BUFFER_SIZE		(PACKET_BUFFER_SIZE * 3)

enum class COLTROLLER_TYPE
{
	SINGLE,
	MULTI,
};


enum class VIEW_PAGE
{
	SELECTOR	= 0,

	CONNECT,
	LOGIN,
	SERVERMOVE,
	LOBBY,
	CREATE_CHARACTER,
	MAP,
	//SHOP,
	//ROOM,
	//LOADING,
	//BATTLE,
	//LOADOUT,
	//EQUIPMENT,

	BOT_SETTING,
	BOT_RUN,

	MAX,
};
#define view_page_max_t static_cast<int32_t>(VIEW_PAGE::MAX)

#include "Common_generated.h"
#include "Protocol_generated.h"

#include "ControllerMulti.h"
