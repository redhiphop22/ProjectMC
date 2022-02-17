#include "World.h"
#include "DllMain.h"

namespace main {

bool Start()
{
	mainFrm = new MainFrm();
	if(nullptr == mainFrm)
		return false;

	if(false == mainFrm->OnInit())
	{
		printf("[Eror] World Init\n");
		return false;
	}

	printf("[Succ] World Init\n");
	mainFrm->OnUpdate();

	return true;
}

void Destroy()
{
	mainFrm->OnDestroy();
}

}
