#include "Starter.h"

bool StartServer();
void RunServer();
void StopServer();
int _tmainService(int argc, TCHAR* argv[]);

int _tmain(int argc, TCHAR* argv[])
{
	////Set Current Directory; 
	//{
	//	INT32 StrLen; 
	//	char Temp[512]; 

	//	i3String::Copy(Temp, argv[0]);
	//	StrLen = i3String::Length(Temp); 
	//	for(INT32 i = (StrLen - 1); i > 0; i--)
	//	{
	//		if( Temp[i] == '\\')
	//		{
	//			Temp[i] = 0; 
	//			break; 
	//		}
	//	}
	//	g_SetCurrentPath(Temp); 
	//}

	//_tmainCommand(argc, argv[]);
	if(StartServer())
	{
		RunServer();
	}
	else
	{
		system("pause");
	}
	StopServer();

	return 0;
}

bool StartServer()
{
	starterServer = new StarterServer();
	if(nullptr == starterServer)
	{
		printf("[Eror] StartServer Create Error\n");
		return false;
	}
	if(false == starterServer->OnStart())
	{
		printf("[Eror] StartServer OnStart Error\n");
		return false;
	}
	return true;
}

void RunServer()
{
	bool mainContinue = true;
	while(mainContinue)
	{
		printf("\n[q] - quit\n");
		char input = getchar();
		switch(input)
		{
		case 'q':
			printf("System ShutDown\n");
			mainContinue = false;
			break;
		}
	}
}

void StopServer()
{
	if(starterServer)
	{
		starterServer->OnDestroy();
		delete starterServer;
		starterServer = nullptr;
	}
}

int _tmainService(int argc, TCHAR* argv[])
{
	//char pServiceName[512];
	//char lpCmdLineData[512];
	//char pModuleFile[512];
	//char pExeFile[512];
	//std::string cmdLine;

	////1. Pre Set Value 
	//if(2 <= argc)
	//{
	//	cmdLine = argv[1];
	//}
	//std::transform(cmdLine.cbegin(), cmdLine.cend(), cmdLine.begin(), tolower);

	//DWORD dwSize = GetModuleFileName(NULL, pModuleFile, 512);
	//pModuleFile[dwSize] = 0;
	//if(dwSize>4 && pModuleFile[dwSize-4] == '.')
	//{
	//	sprintf(pExeFile,"%s",pModuleFile);
	//	pModuleFile[dwSize-4] = 0;	
	//}
	//strcpy(pServiceName,"PBGameServerService");	

	////2. Run 
	//if(		std::strcmp("-i", cmdLine.c_str()) == 0)	s2::S2Service::Install(pExeFile, pServiceName, nullptr);		//1. 인스톨	
	//else if(std::strcmp("-u", cmdLine.c_str()) == 0)	s2::S2Service::UnInstall(pServiceName);				//2. 언인스톨
	//else if(std::strcmp("-s", cmdLine.c_str()) == 0)	s2::S2Service::RunService(pServiceName);			//3. 서비스 시작.
	//else if(std::strcmp("-e", cmdLine.c_str()) == 0)	s2::S2Service::StopService(pServiceName);			//4. 서비스 중단.
	//else if(std::strcmp("-h", cmdLine.c_str()) == 0)	
	//{
	//	printf("PB Server Help!\n\n"); 
	//	printf("PBGames.exe -i : Service Install \n"); 
	//	printf("PBGames.exe -u : Service Uninstall. \n"); 
	//	printf("PBGames.exe -s : Service Start. \n");
	//	printf("PBGames.exe -e : Service End. \n");
	//	printf("PBGames.exe : It does not use. \n");
	//}
	//else StartServer();						//6. 메인 함수 

	return 0;
}

