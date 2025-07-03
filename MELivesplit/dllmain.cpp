#include "stdafx.h"

void Main(LPVOID lpParameter) {
	if (!GetModuleHandleA("MirrorsEdge.exe")) {
		return;
	}


	RenderD3D9();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	switch (reason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(module);
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
		break;
	}
	return TRUE;
}