#pragma once
#include <map>
#include <string>

void InitializeHooks();
void SetupOverrides();

void SetupItemIds();
int GetIdForItem(std::string item);
std::string GetItemForId(int id);
void ResetAllItems();

void SetupLocationIds();
int GetIdForLocation(std::string location);
std::string GetLocationForId(int id);
void ResetAllLocations();