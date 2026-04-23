#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "polyhook2/Detour/NatDetour.hpp"
#include "mio-archipelago.h"
#include "loot-overrides.h"
#include "Archipelago.h"

uintptr_t loot_trampoline = NULL;
uintptr_t mio_has_trampoline = NULL;
uintptr_t glitch_state_update_trampoline = NULL;

NOINLINE auto __cdecl LootHook(uintptr_t game, ModAPI::SaveData::GameString* item_id, uint32_t count) {
    ModAPI::SaveData::GameString* id = item_id;
    std::map<std::string, std::list<CallbackOverride>> lootOverrides = GetHasOverrides();
    if (lootOverrides.count(std::string(id->data))) {
        uintptr_t returnAddr = (uintptr_t)_ReturnAddress();
        for (CallbackOverride i : lootOverrides[std::string(id->data)]) {
            if (returnAddr >= i.methodAddr && returnAddr <= i.methodAddr + i.methodSize) {
                ModAPI::SaveData::GameString str = ModAPI::SaveData::GameString((char*)i.replaceWith.c_str());
                id = &str;
                AP_SendItem(GetIdForLocation(i.replaceWith));
            }
        }
    }

    typedef int func(uintptr_t, ModAPI::SaveData::GameString*, uint32_t);
    func* trampoline = (func*)(loot_trampoline);
    return trampoline(game, id, count);
}

NOINLINE bool __cdecl MioHasHook(uintptr_t mio, ModAPI::SaveData::GameString* item_id) {
    ModAPI::SaveData::GameString* id = item_id;
    std::map<std::string, std::list<CallbackOverride>> hasOverrides = GetHasOverrides();
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
    return trampoline(mio, id);
}

void* insideGlitchAddr;
void* exitGlitchAddr;
NOINLINE void __cdecl GlitchStateUpdateHook() {
    std::string zoneId = std::string(ModAPI::Util::GetCurrentZoneId());
    if (zoneId != std::string("GW_intro_jump_P1") && ModAPI::Util::ReadMemoryTyped<uint8_t>(insideGlitchAddr)) {
        ModAPI::Util::CallAssembly<void>(exitGlitchAddr, ModAPI::Addresses::g_GameAddr);
    }
    typedef int func();
    func* trampoline = (func*)(glitch_state_update_trampoline);
    trampoline();
}

void InitializeHooks() {
    uintptr_t baseAddr = ModAPI::Addresses::g_BaseAddr;

    uintptr_t lootAddr = baseAddr + ModAPI::Util::GetMethodOffset("public: struct Save_entry * __cdecl Game::loot(struct String const &,int,enum Loot_flags)");
    static PLH::NatDetour loot_hook_detour = PLH::NatDetour(lootAddr, (uintptr_t)LootHook, &loot_trampoline);
    loot_hook_detour.hook();

    uintptr_t mioHasAddr = baseAddr + ModAPI::Util::GetMethodOffset("public: bool __cdecl Mio::has(struct String const &)");
    static PLH::NatDetour mio_has_hook_detour = PLH::NatDetour(mioHasAddr, (uintptr_t)MioHasHook, &mio_has_trampoline);
    mio_has_hook_detour.hook();

    uintptr_t glitchStateUpdateAddr = baseAddr + ModAPI::Util::GetMethodOffset("void __cdecl glitch_state_update(void)");
    static PLH::NatDetour glitch_state_update_hook_detour = PLH::NatDetour(glitchStateUpdateAddr, (uintptr_t)GlitchStateUpdateHook, &glitch_state_update_trampoline);
    glitch_state_update_hook_detour.hook();

    exitGlitchAddr = (void*)(baseAddr + ModAPI::Util::GetMethodOffset("public: void __cdecl Game::exit_glitch(void)"));
    insideGlitchAddr = (void*)((uintptr_t)ModAPI::Addresses::g_GameAddr + ModAPI::Util::GetVariableOffset("Game", "glitch") + ModAPI::Util::GetVariableOffset("Game::Glitch", "_inside"));
}