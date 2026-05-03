#include "mio-archipelago.h";
#include "mio-modding-api.h"
#include <string>
#include <vector>
#include <Archipelago.h>

std::map<int64_t, std::string> idsToLocation;
std::map<std::string, int64_t> locationToIds;

int GetIdForLocation(std::string location) {
    return locationToIds[location];
}
std::string GetLocationForId(int id) {
    return idsToLocation[id];
}

void AddLocation(int id, std::string location) {
    idsToLocation[id] = location;
    locationToIds[location] = id;
}

void SetupLocationIds() {
    AddLocation(0, "ARCHIPELAGO_UNLOCK:HOOK");
    AddLocation(1, "ARCHIPELAGO_UNLOCK:HIT_RECHARGE");
    AddLocation(2, "ARCHIPELAGO_UNLOCK:BLOCK");
    AddLocation(3, "ARCHIPELAGO_UNLOCK:GLIDE");
    AddLocation(4, "ARCHIPELAGO_UNLOCK:SPIDER");
    AddLocation(5, "ARCHIPELAGO_UNLOCK:ORB_SHOOT");
    AddLocation(6, "ARCHIPELAGO_UNLOCK:SPIDER_GOO");

    //TRINKETS
    AddLocation(7, "ARCHIPELAGO_TRINKET:SHIELD_INC");
}

void ResetAllLocations() {
    for (auto& i : idsToLocation) {
        ModAPI::SaveData::RemoveSaveEntry(i.second.c_str());
    }
}