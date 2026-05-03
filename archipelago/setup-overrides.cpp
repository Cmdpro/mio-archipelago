#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "loot-overrides.h"
#include <map>

void RegisterHas(const char* id, const char* method) {
	AddHasOverride(id, CallbackOverride(method, (std::string("ARCHIPELAGO_") + std::string(id)).c_str()));
}
void RegisterLoot(const char* id, const char* method) {
	AddLootOverride(id, CallbackOverride(method, (std::string("ARCHIPELAGO_") + std::string(id)).c_str()));
}
void RegisterLootNode(const char* id) {
	RegisterHas(id, "public: void __cdecl Game::update_loot(void)");
	RegisterLoot(id, "public: void __cdecl Game::update_loot(void)");
}
void RegisterChest(const char* id) {
	RegisterHas(id, "public: static void __cdecl Chest2_node::activate(struct Node2 *,struct Chest2_node *)");
	RegisterLootNode(id);
}

void SetupOverrides() {
	// HOOK
	RegisterHas("UNLOCK:HOOK", "public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)");
	RegisterLoot("UNLOCK:HOOK", "public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)");
	RegisterHas("UNLOCK:HOOK", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// HIT_RECHARGE
	RegisterHas("UNLOCK:HIT_RECHARGE", "public: void __cdecl GA_manareset::update_hacker_manareset(void)");
	RegisterLoot("UNLOCK:HIT_RECHARGE", "public: void __cdecl GA_manareset::update_hacker_manareset(void)");
	RegisterHas("UNLOCK:HIT_RECHARGE", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// BLOCK
	RegisterHas("UNLOCK:BLOCK", "public: void __cdecl LQ_block_hacker::update_LQ_block_hacker(void)");
	RegisterLoot("UNLOCK:BLOCK", "public: void __cdecl LQ_block_hacker::update_LQ_block_hacker(void)");
	RegisterHas("UNLOCK:BLOCK", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// GLIDE
	RegisterHas("UNLOCK:GLIDE", "public: void __cdecl GA_bou::update_hacker_glide(void)");
	RegisterLoot("UNLOCK:GLIDE", "public: void __cdecl GA_bou::update_hacker_glide(void)");
	RegisterHas("UNLOCK:GLIDE", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// SPIDER
	RegisterHas("UNLOCK:SPIDER", "public: void __cdecl LQ_city_hacker::update_TW_mid_hacker(void)");
	RegisterLoot("UNLOCK:SPIDER", "public: void __cdecl LQ_city_hacker::update_TW_mid_hacker(void)");
	RegisterHas("UNLOCK:SPIDER", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// ORB_SHOOT
	RegisterHas("UNLOCK:ORB_SHOOT", "public: void __cdecl GW_tuto_orbshoot::update(struct Node2 *)");
	RegisterLoot("UNLOCK:ORB_SHOOT", "public: void __cdecl GW_tuto_orbshoot::update(struct Node2 *)");
	RegisterHas("UNLOCK:ORB_SHOOT", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// SPIDER_GOO
	RegisterLoot("UNLOCK:SPIDER_GOO", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");
	RegisterHas("UNLOCK:SPIDER_GOO", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// TRINKETS
	RegisterChest("TRINKET:SHIELD_INC");
}