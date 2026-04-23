// dllmain.cpp : Defines the entry point for the DLL application.
#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "polyhook2/Detour/NatDetour.hpp"
#include "loot-overrides.h"
#include "mio-archipelago.h"
#include "Archipelago.h"
#include <nlohmann/json.hpp>
#include <random>

char* modId;
std::mt19937 random;

void LogMessage(const char* message) {
    ModAPI::Util::LogMessage(modId, message);
}

nlohmann::json CreateDefaultJson() {
    nlohmann::json data = nlohmann::json();
    nlohmann::json singleplayerData = nlohmann::json();
    singleplayerData["file"] = "singleplayer";
    data["singleplayer"] = singleplayerData;
    nlohmann::json multiplayerData = nlohmann::json();
    multiplayerData["ip"] = "localhost:38281";
    multiplayerData["slotName"] = "PLEASE_FILL_IN";
    multiplayerData["password"] = "";
    data["multiplayer"] = multiplayerData;
    data["isSingleplayer"] = false;
    return data;
}

DWORD WaitToConnect(LPVOID lpParam) {
    ModAPI::Util::WaitForSaveLoaded();
    AP_Start();
    return 0;
}

extern "C" __declspec(dllexport) void ModInit(char* id) {
    modId = id;

    InitializeHooks();
    SetupLocationIds();
    SetupItemIds();
    SetupOverrides();
    CreateDirectoryA(ModAPI::Util::GetFolderPathForModConfig(modId).string().c_str(), NULL);

    std::filesystem::path configPath = ModAPI::Util::GetFolderPathForModConfig(modId) / std::filesystem::path("config.json");
    nlohmann::json data = CreateDefaultJson();
    if (std::filesystem::exists(configPath)) {
        std::ifstream file(configPath);
        data = nlohmann::json::parse(file);
        file.close();
    }
    else {
        std::ofstream file(configPath);
        file << data.dump().c_str();
        file.close();
        std::exit(0);
        return;
    }
    bool singleplayer = data["isSingleplayer"].get<bool>();

    if (singleplayer) {
        nlohmann::json singleplayerData = data["singleplayer"].get<nlohmann::json>();
        std::string fileStr = singleplayerData["file"].get<std::string>();
        std::filesystem::path singleplayerPath = ModAPI::Util::GetFolderPathForModConfig(modId) / std::filesystem::path("singleplayer");
        CreateDirectoryA(singleplayerPath.string().c_str(), NULL);
        std::filesystem::path file = singleplayerPath / std::filesystem::path(fileStr);
        AP_Init(file.string().c_str());
    } else {
        nlohmann::json multiplayerData = data["multiplayer"].get<nlohmann::json>();
        std::string ip = multiplayerData["ip"].get<std::string>();
        std::string slotName = multiplayerData["slotName"].get<std::string>();
        std::string password = multiplayerData["password"].get<std::string>();
        AP_Init(ip.c_str(), "Memories in Orbit", slotName.c_str(), password.c_str());
    }

    std::random_device rd;
    random = std::mt19937(rd());

    AP_SetItemClearCallback([]() {
        ResetAllItems();
        ResetAllLocations();
    });
    AP_SetItemRecvCallback([](int64_t id, bool notify) {
        std::string item = GetItemForId(id);
        if (item == "ARCHIPELAGO_SPECIAL:RANDOM_NACRE") {
            std::uniform_int_distribution<> dist(1, 100);
            ModAPI::SaveData::GiveFlag("RESOURCE:PEARL_SHARDS", dist(random));
            return;
        }
        ModAPI::SaveData::GiveFlag(item.c_str(), 1);
        });
    AP_SetLocationCheckedCallback([](int64_t id) {
        std::string location = GetLocationForId(id);
        ModAPI::SaveData::GiveFlag(location.c_str(), 1);
    });

    LogMessage("Loaded Archipelago");

    CreateThread(nullptr, 0, WaitToConnect, nullptr, 0, nullptr);
}
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    return TRUE;
}