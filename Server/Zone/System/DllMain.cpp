#include "Zone.h"
#include "DllMain.h"

namespace main {

bool Start()
{
	mainFrm = new MainFrm();
	if(nullptr == mainFrm)
		return false;

	if(false == mainFrm->OnInit())
	{
		printf("[Eror] Zone Init\n");
		return false;
	}

	printf("[Succ] Zone Init\n");
	mainFrm->OnUpdate();

	return true;
}

void Destroy()
{
	mainFrm->OnDestroy();
}

}
