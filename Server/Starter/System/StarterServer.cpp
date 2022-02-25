#include "Starter.h"
#include "StarterServer.h"

StarterServer* starterServer = nullptr;

StarterServer::StarterServer()
{	
	//m_moduleName.push_back(".\\Login.dll");
	m_moduleName.push_back(".\\Zone.dll");
	m_moduleName.push_back(".\\World.dll");
}

StarterServer::~StarterServer()
{
}

bool StarterServer::OnStart()
{
	S2ExceptionHandler::GetInstance().Create("dumpFile");
	if(false == m_log.Create("..\\Log\\MCLog"))
	{
		printf("[Eror] Log Create");
		return false;
	}
	m_log.SetVersion(1, 1, 1, 1);
	printf("[Succ] Log Create");

	for(auto& iter : m_moduleName)
	{
		HMODULE hMod = ::LoadLibrary(iter.c_str());
		if(nullptr == hMod)
		{
			DWORD error = ::GetLastError();
			printf("[Eror] Module Error (Module:%s, Code:%d)\n", iter.c_str(), error);
			return false;
		}
		printf("[Succ] Module Load (Module:%s)\n", iter.c_str());
		m_moduleList.push_back(std::make_tuple(hMod, iter.c_str()));
	}

	typedef bool(*pDLLFunction)(s2::S2LogFile* log);
	pDLLFunction pFunc = NULL;
	for(auto& module : m_moduleList)
	{
		pFunc = (pDLLFunction)GetProcAddress(std::get<0>(module), "Start");
		if(false == pFunc(&m_log))
		{
			printf("[Eror] Module Start (Module:%s)\n", std::get<1>(module).c_str());
			return false;
		}
		printf("[Succ] Module Start (Module:%s)\n", std::get<1>(module).c_str());
	}

	return true;
}

void StarterServer::OnDestroy()
{
	//printf("[Info] Module Destroy (Module:%s)\n", std::get<1>(module).c_str());
	typedef void(*pDLLFunction)();
	pDLLFunction pFunc = NULL;
	for(auto& module : m_moduleList)
	{
		pFunc = (pDLLFunction)GetProcAddress(std::get<0>(module), "Destroy");
		pFunc();
		printf("[Succ] Module Destroy (Module:%s)\n", std::get<1>(module).c_str());
	}
	m_moduleList.clear();
	m_log.Destroy();
}