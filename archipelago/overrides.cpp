#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "loot-overrides.h"
#include <map>

std::map<std::string, std::list<CallbackOverride>> hasOverrides;
std::map<std::string, std::list<CallbackOverride>> lootOverrides;

void ClearAllOverrides() {
    hasOverrides.clear();
    lootOverrides.clear();
}
void AddHasOverride(std::string key, CallbackOverride value) {
    hasOverrides[key].push_back(value);
}
void AddLootOverride(std::string key, CallbackOverride value) {
    lootOverrides[key].push_back(value);
}
std::map<std::string, std::list<CallbackOverride>> GetLootOverrides() {
    return lootOverrides;
}
std::map<std::string, std::list<CallbackOverride>> GetHasOverrides() {
    return hasOverrides;
}