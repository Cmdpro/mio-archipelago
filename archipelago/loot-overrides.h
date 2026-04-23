#pragma once
#include <string>
#include <mio-modding-api.h>
#include <map>

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
        methodAddr = ModAPI::Addresses::g_BaseAddr + ModAPI::Util::GetMethodOffset(method);
        methodSize = ModAPI::Util::GetMethodSize(method);
        this->replaceWith = std::string(replaceWith);
    }
};

void AddHasOverride(std::string key, CallbackOverride value);
void AddLootOverride(std::string key, CallbackOverride value);
std::map<std::string, std::list<CallbackOverride>> GetLootOverrides();
std::map<std::string, std::list<CallbackOverride>> GetHasOverrides();
void ClearAllOverrides();