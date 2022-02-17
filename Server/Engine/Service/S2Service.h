#pragma once

namespace s2 {
namespace S2Service {

void					Install(TCHAR* path, TCHAR* name, TCHAR* description);
void					UnInstall(TCHAR* name);
bool					RunService(TCHAR* name);
bool					StopService(TCHAR* name);

}
}