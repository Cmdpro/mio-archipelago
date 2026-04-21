// dllmain.cpp : Defines the entry point for the DLL application.
#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "polyhook2/Detour/NatDetour.hpp"

char* modId;
void LogMessage(const char* message) {
    ModAPI::Util::LogMessage(modId, message);
}


uintptr_t baseAddr;

struct CallbackOverride {
    uintptr_t methodAddr;
    size_t methodSize;
    std::string replaceWith;
    CallbackOverride(uintptr_t methodAddr, size_t methodSize, std::string replaceWith) {
        this->methodAddr = methodAddr;
        this->methodSize = methodSize;
        this->replaceWith = replaceWith;
    }
    CallbackOverride() {}
    CallbackOverride(const char* method, const char* replaceWith) {
        methodAddr = baseAddr+ModAPI::Util::GetMethodOffset(method);
        methodSize = ModAPI::Util::GetMethodSize(method);
        this->replaceWith = std::string(replaceWith);
    }
};

uintptr_t loot_trampoline = NULL;
uintptr_t mio_has_trampoline = NULL;
std::map<std::string, std::list<CallbackOverride>> hasOverrides;
std::map<std::string, std::list<CallbackOverride>> lootOverrides;

void AddHasOverride(std::string key, CallbackOverride value) {
    hasOverrides[key].push_back(value);
}
void AddLootOverride(std::string key, CallbackOverride value) {
    lootOverrides[key].push_back(value);
}

NOINLINE void __cdecl LootHook(uintptr_t game, ModAPI::SaveData::GameString* item_id, uint32_t count) {
    ModAPI::SaveData::GameString* id = item_id;
    if (hasOverrides.count(std::string(id->data))) {
        uintptr_t returnAddr = (uintptr_t)_ReturnAddress();
        for (CallbackOverride i : hasOverrides[std::string(id->data)]) {
            if (returnAddr >= i.methodAddr && returnAddr <= i.methodAddr + i.methodSize) {
                ModAPI::SaveData::GameString str = ModAPI::SaveData::GameString((char*)i.replaceWith.c_str());
                id = &str;
            }
        }
    }

    typedef int func(uintptr_t, ModAPI::SaveData::GameString*, uint32_t);
    func* trampoline = (func*)(loot_trampoline);
    int i = trampoline(game, id, count);
}

NOINLINE void __cdecl MioHasHook(uintptr_t mio, ModAPI::SaveData::GameString* item_id) {
    ModAPI::SaveData::GameString* id = item_id;
    if (hasOverrides.count(std::string(id->data))) {
        uintptr_t returnAddr = (uintptr_t)_ReturnAddress();
        for (CallbackOverride i : hasOverrides[std::string(id->data)]) {
            if (returnAddr >= i.methodAddr && returnAddr <= i.methodAddr + i.methodSize) {
                ModAPI::SaveData::GameString str = ModAPI::SaveData::GameString((char*)i.replaceWith.c_str());
                id = &str;
            }
        }
    }

    typedef int func(uintptr_t, ModAPI::SaveData::GameString*);
    func* trampoline = (func*)(mio_has_trampoline);
    int i = trampoline(mio, id);
}
extern "C" __declspec(dllexport) void ModInit(char* id) {
    HMODULE hModule = GetModuleHandleA("mio.exe");
    if (!hModule) {
        ModAPI::Util::LogMessage(modId, "ERROR: Failed to get mio.exe module handle!");
        return;
    }
    LogMessage("Loaded Archipelago");
    baseAddr = (uintptr_t)hModule;

    modId = id;
    AddHasOverride("UNLOCK:HOOK", CallbackOverride("public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)", "UNLOCK:SPIDER"));
    AddLootOverride("UNLOCK:HOOK", CallbackOverride("public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)", "UNLOCK:SPIDER"));

    uintptr_t lootAddr = baseAddr+ModAPI::Util::GetMethodOffset("public: struct Save_entry * __cdecl Game::loot(struct String const &,int,enum Loot_flags)");
    static PLH::NatDetour loot_hook_detour = PLH::NatDetour(lootAddr, (uintptr_t)LootHook, &loot_trampoline);
    loot_hook_detour.hook();

    uintptr_t mioHasAddr = baseAddr+ModAPI::Util::GetMethodOffset("public: bool __cdecl Mio::has(struct String const &)");
    static PLH::NatDetour mio_has_hook_detour = PLH::NatDetour(mioHasAddr, (uintptr_t)MioHasHook, &mio_has_trampoline);
    mio_has_hook_detour.hook();
}
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    return TRUE;
}