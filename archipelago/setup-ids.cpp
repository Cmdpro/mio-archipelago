#include "mio-archipelago.h";
#include "mio-modding-api.h"
#include <string>
#include <vector>

std::map<int64_t, std::string> idsToItem;
std::map<std::string, int64_t> itemToIds;

int GetIdForItem(std::string item) {
    return itemToIds[item];
}
std::string GetItemForId(int id) {
    return idsToItem[id];
}

void AddItem(int id, std::string item) {
    idsToItem[id] = item;
    itemToIds[item] = id;
}

void SetupItemIds() {
    AddItem(0, "UNLOCK:HOOK");
    AddItem(1, "UNLOCK:HIT_RECHARGE");
    AddItem(2, "UNLOCK:BLOCK");
    AddItem(3, "UNLOCK:GLIDE");
    AddItem(4, "UNLOCK:SPIDER");
    AddItem(5, "UNLOCK:ORB_SHOOT");
    AddItem(6, "UNLOCK:SPIDER_GOO");

    AddItem(7, "TRINKET:SHIELD_INC");

    AddItem(8, "ARCHIPELAGO_SPECIAL:RANDOM_NACRE");
}

void ResetAllItems() {
    for (auto& i : idsToItem) {
        ModAPI::SaveData::RemoveSaveEntry(i.second.c_str());
    }
}