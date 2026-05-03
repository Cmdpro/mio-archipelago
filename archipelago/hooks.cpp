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
uintptr_t stringfv_trampoline = NULL;
uintptr_t loot_popup_start_trampoline = NULL;

NOINLINE void* __cdecl LootHook(uintptr_t game, ModAPI::SaveData::GameString* item_id, int32_t count) {
    ModAPI::SaveData::GameString* id = item_id;
    std::map<std::string, std::list<CallbackOverride>> lootOverrides = GetLootOverrides();
    std::string dataStr = std::string(id->data);
    if (lootOverrides.find(dataStr) != lootOverrides.end()) {
        uintptr_t returnAddr = (uintptr_t)_ReturnAddress();
        for (CallbackOverride i : lootOverrides[dataStr]) {
            if (returnAddr >= i.methodAddr && returnAddr <= i.methodAddr + i.methodSize) {
                ModAPI::SaveData::GameString str = ModAPI::SaveData::GameString((char*)i.replaceWith.c_str());
                id = &str;
                AP_SendItem(GetIdForLocation(i.replaceWith));
            }
        }
    }

    typedef void* func(uintptr_t, ModAPI::SaveData::GameString*, int32_t);
    func* trampoline = (func*)(loot_trampoline);
    return trampoline(game, id, count);
}

NOINLINE uint8_t __cdecl MioHasHook(uintptr_t mio, ModAPI::SaveData::GameString* item_id) {
    ModAPI::SaveData::GameString* id = item_id;
    std::map<std::string, std::list<CallbackOverride>> hasOverrides = GetHasOverrides();
    std::string dataStr = std::string(id->data);
    if (hasOverrides.find(dataStr) != hasOverrides.end()) {
        uintptr_t returnAddr = (uintptr_t)_ReturnAddress();
        for (CallbackOverride i : hasOverrides[dataStr]) {
            if (returnAddr >= i.methodAddr && returnAddr <= i.methodAddr + i.methodSize) {
                ModAPI::SaveData::GameString str = ModAPI::SaveData::GameString((char*)i.replaceWith.c_str());
                id = &str;
            }
        }
    }

    typedef uint8_t func(uintptr_t, ModAPI::SaveData::GameString*);
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
    typedef void func();
    func* trampoline = (func*)(glitch_state_update_trampoline);
    trampoline();
}
NOINLINE ModAPI::SaveData::GameString* __cdecl StringfvHook(ModAPI::SaveData::GameString* __return, char const* fmt, void* args) {
    typedef ModAPI::SaveData::GameString* func(ModAPI::SaveData::GameString*, char const*, void*);
    func* trampoline = (func*)(stringfv_trampoline);

    if (!fmt) {
        return trampoline(__return, fmt, args);
    }
    std::string fmtStr = std::string(fmt);
    std::string fmtNew = fmtStr;
    if (fmtStr == "slot_%.save" || fmtStr == "slot_%_bck") {
        fmtNew = "archipelago_" + fmtStr;
        return trampoline(__return, fmtNew.c_str(), args);
    }
    return trampoline(__return, fmt, args);
}
NOINLINE void __cdecl LootPopupStartHook(void* self, ModAPI::SaveData::GameString* loot_id) {
    std::map<std::string, std::list<CallbackOverride>> lootOverrides = GetLootOverrides();
    std::string dataStr = std::string(loot_id->data);
    if (lootOverrides.find(dataStr) != lootOverrides.end()) {
        return;
    }

    typedef ModAPI::SaveData::GameString* func(void*, ModAPI::SaveData::GameString*);
    func* trampoline = (func*)(loot_popup_start_trampoline);
    trampoline(self, loot_id);
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

    uintptr_t stringfvAddr = baseAddr + ModAPI::Util::GetMethodOffset("struct String __cdecl stringfv(char const *,struct Array<struct Val_ref> const &)");
    static PLH::NatDetour stringfv_hook_detour = PLH::NatDetour(stringfvAddr, (uintptr_t)StringfvHook, &stringfv_trampoline);
    stringfv_hook_detour.hook();

    uintptr_t lootPopupStartAddr = baseAddr + ModAPI::Util::GetMethodOffset("public: void __cdecl Ui_loot_popup::start(struct String const &)");
    static PLH::NatDetour loot_popup_start_hook_detour = PLH::NatDetour(lootPopupStartAddr, (uintptr_t)LootPopupStartHook, &loot_popup_start_trampoline);
    loot_popup_start_hook_detour.hook();

    exitGlitchAddr = (void*)(baseAddr + ModAPI::Util::GetMethodOffset("public: void __cdecl Game::exit_glitch(void)"));
    insideGlitchAddr = (void*)((uintptr_t)ModAPI::Addresses::g_GameAddr + ModAPI::Util::GetVariableOffset("Game", "glitch") + ModAPI::Util::GetVariableOffset("Game::Glitch", "_inside"));
}