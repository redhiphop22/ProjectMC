#pragma once

#include "..\..\3thParty\rapidjson\document.h"
#include "..\..\3thParty\rapidjson\error\en.h"

namespace s2 {

class S2Json : public S2Singleton<S2Json>
{
public:
	S2Json();
	~S2Json();

	bool					Create(const char* fileName);
	void					Destroy();

	rapidjson::Document&	GetJson()	{	return m_json;	}

protected:
	rapidjson::Document		m_json;
};

}

#define CONFIG s2::S2Json::GetInstance()