#pragma once

class StarterServer
{
public:
	StarterServer();
	~StarterServer();
	
	bool					OnStart();
	void					OnDestroy();

private:
	std::vector<std::string> m_moduleName;
	std::vector<std::tuple<HMODULE, std::string>> m_moduleList;

	s2::S2LogFile			m_log;
};

extern StarterServer* starterServer;