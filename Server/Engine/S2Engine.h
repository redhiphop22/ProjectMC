#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS		

#define USE_MYSQL
//#define USE_MSSQL

#define SAFE_RELEASE(p)			if( p ) { p->Release();		p = NULL; }
#define SAFE_DELETE(p)			if( p ) { delete p;			p = NULL; }
#define SAFE_DELETE_ARRAY(p)	if( p ) { delete[] p;		p = NULL; }
#define S2TRACE(p)

// typedef
typedef signed char				int8_t;
typedef unsigned char			uint8_t;
typedef signed short int		int16_t;
typedef unsigned short int		uint16_t;
typedef signed int				int32_t;
typedef unsigned int			uint32_t;
typedef signed __int64			int64_t;
typedef unsigned __int64		uint64_t;

#include <stdio.h>
#include <thread>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <mutex>
#include <algorithm>

#include ".\DataType\S2Date32.h"
#include ".\DataType\S2Vector3.h"

#include ".\System\S2Singleton.h"
#include ".\System\S2Mutex.h"
#include ".\System\S2SpinLock.h"
#include ".\System\S2ElementBase.h"
#include ".\System\S2Net.h"
#include ".\System\S2String.h"
#include ".\System\S2Thread.h"
#include ".\System\S2ThreadMgr.h"
#include ".\System\S2Time.h"
#include ".\System\S2MiniDump.h"
#include ".\System\S2RingBuffer.h"
#include ".\System\S2LogFileMgr.h"
#include ".\System\S2LogFileA.h"
#include ".\System\S2LogFileW.h"

#include ".\System\S2MessageReceiver.h"
#include ".\System\S2MessageProcessor.h"

#include ".\System\S2IocpSession.h"
#include ".\System\S2IocpSessionMgr.h"
#include ".\System\S2IocpWorker.h"
#include ".\System\S2IocpWorkerMgr.h"
#include ".\System\S2Iocp.h"
#include ".\System\S2SocketConnector.h"

#include ".\System\S2ADO.h"
//#include ".\System\S2MySql.h"

#include ".\Service\S2Server.h"
#include ".\Service\S2Service.h"