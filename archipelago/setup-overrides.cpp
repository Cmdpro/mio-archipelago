#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "loot-overrides.h"
#include <map>

void AddHasOverride(const char* id, const char* method) {
	AddHasOverride(id, CallbackOverride(method, (std::string("ARCHIPELAGO_") + std::string(id)).c_str()));
}
void AddLootOverride(const char* id, const char* method) {
	AddLootOverride(id, CallbackOverride(method, (std::string("ARCHIPELAGO_") + std::string(id)).c_str()));
}

void SetupOverrides() {
	const char* input;
	const char* output;

	// HOOK
	AddHasOverride("UNLOCK:HOOK", "public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)");
	AddLootOverride("UNLOCK:HOOK", "public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)");
	AddHasOverride("UNLOCK:HOOK", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// HIT_RECHARGE
	AddHasOverride("UNLOCK:HIT_RECHARGE", "public: void __cdecl GA_manareset::update_hacker_manareset(void)");
	AddLootOverride("UNLOCK:HIT_RECHARGE", "public: void __cdecl GA_manareset::update_hacker_manareset(void)");
	AddHasOverride("UNLOCK:HIT_RECHARGE", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// BLOCK
	AddHasOverride("UNLOCK:BLOCK", "public: void __cdecl LQ_block_hacker::update_LQ_block_hacker(void)");
	AddLootOverride("UNLOCK:BLOCK", "public: void __cdecl LQ_block_hacker::update_LQ_block_hacker(void)");
	AddHasOverride("UNLOCK:BLOCK", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// GLIDE
	AddHasOverride("UNLOCK:GLIDE", "public: void __cdecl GA_bou::update_hacker_glide(void)");
	AddLootOverride("UNLOCK:GLIDE", "public: void __cdecl GA_bou::update_hacker_glide(void)");
	AddHasOverride("UNLOCK:GLIDE", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// SPIDER
	AddHasOverride("UNLOCK:SPIDER", "public: void __cdecl LQ_city_hacker::update_TW_mid_hacker(void)");
	AddLootOverride("UNLOCK:SPIDER", "public: void __cdecl LQ_city_hacker::update_TW_mid_hacker(void)");
	AddHasOverride("UNLOCK:SPIDER", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// ORB_SHOOT
	AddHasOverride("UNLOCK:ORB_SHOOT", "public: void __cdecl GW_tuto_orbshoot::update(struct Node2 *)");
	AddLootOverride("UNLOCK:ORB_SHOOT", "public: void __cdecl GW_tuto_orbshoot::update(struct Node2 *)");
	AddHasOverride("UNLOCK:ORB_SHOOT", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");

	// SPIDER_GOO
	AddLootOverride("UNLOCK:SPIDER_GOO", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");
	AddHasOverride("UNLOCK:SPIDER_GOO", "public: void __cdecl Npc_node::update_hacker(struct Node2 *)");
}