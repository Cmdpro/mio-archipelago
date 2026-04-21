// dllmain.cpp : Defines the entry point for the DLL application.
#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "polyhook2/Detour/NatDetour.hpp"
#include "loot-overrides.h"

char* modId;
void LogMessage(const char* message) {
    ModAPI::Util::LogMessage(modId, message);
}

extern "C" __declspec(dllexport) void ModInit(char* id) {
    modId = id;

    LogMessage("Loaded Archipelago");
}
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    return TRUE;
}