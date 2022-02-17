#pragma once

#ifdef DLL_EXPORTS
	#define DLL_EXPORT_BASE		extern "C" __declspec( dllexport)
#else
	#define DLL_EXPORT_BASE		extern "C" __declspec( dllimport)
#endif

#define FBB_BASIC_SIZE	4096

#include "..\Engine\S2Engine.h"
#pragma comment(lib, "S2_64d.lib")

#include "..\Engine\System\S2MySql.h"

#include "Result_generated.h"
#include "Common_generated.h"
#include "Protocol_generated.h"
#include "ProtocolSVR_generated.h"
#include "Common_Define.h"

#include ".\System\Define.h"
#include ".\System\MessageProcessor.h"

#include ".\System\IOCPSessionMgr.h"

#include ".\System\UserProcessor.h"
#include ".\System\DataBaseMgr.h"
#include ".\System\ZoneConnector.h"

#include ".\System\DllMain.h"
#include ".\System\MainFrm.h"