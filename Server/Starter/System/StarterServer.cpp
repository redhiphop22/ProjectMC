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

	typedef bool(*pDLLFunction)();
	pDLLFunction pFunc = NULL;
	for(auto& module : m_moduleList)
	{
		printf("[Info] Module Start (Module:%s)\n", std::get<1>(module).c_str());
		pFunc = (pDLLFunction)GetProcAddress(std::get<0>(module), "Start");
		pFunc();
	}

	return true;
}

void StarterServer::OnDestroy()
{
	typedef void(*pDLLFunction)();
	pDLLFunction pFunc = NULL;
	for(auto& module : m_moduleList)
	{
		printf("[Info] Module Destroy (Module:%s)\n", std::get<1>(module).c_str());
		pFunc = (pDLLFunction)GetProcAddress(std::get<0>(module), "Destroy");
		pFunc();
	}
	m_moduleList.clear();
}