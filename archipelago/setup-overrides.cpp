#include "Windows.h"
#include <stdio.h>
#include "mio-modding-api.h"
#include "loot-overrides.h"
#include <map>

void SetupOverrides(std::map<std::string, std::string> map) {
	const char* input;
	const char* output;

	// HOOK
	input = "UNLOCK:HOOK";
	if (map.contains(input)) {
		output = map[std::string(input)].c_str();
		AddHasOverride(input, CallbackOverride("public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)", output));
		AddLootOverride(input, CallbackOverride("public: void __cdecl GW_tuto_hook::update_GW_tuto_hook(void)", output));
		AddHasOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
	}

	// HIT_RECHARGE
	input = "UNLOCK:HIT_RECHARGE";
	if (map.contains(input)) {
		output = map[std::string(input)].c_str();
		AddHasOverride(input, CallbackOverride("public: void __cdecl GA_manareset::update_hacker_manareset(void)", output));
		AddLootOverride(input, CallbackOverride("public: void __cdecl GA_manareset::update_hacker_manareset(void)", output));
		AddHasOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
	}

	// BLOCK
	input = "UNLOCK:BLOCK";
	if (map.contains(input)) {
		output = map[std::string(input)].c_str();
		AddHasOverride(input, CallbackOverride("public: void __cdecl LQ_block_hacker::update_LQ_block_hacker(void)", output));
		AddLootOverride(input, CallbackOverride("public: void __cdecl LQ_block_hacker::update_LQ_block_hacker(void)", output));
		AddHasOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
	}

	// GLIDE
	input = "UNLOCK:GLIDE";
	if (map.contains(input)) {
		output = map[std::string(input)].c_str();
		AddHasOverride(input, CallbackOverride("public: void __cdecl GA_bou::update_hacker_glide(void)", output));
		AddLootOverride(input, CallbackOverride("public: void __cdecl GA_bou::update_hacker_glide(void)", output));
		AddHasOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
	}

	// SPIDER
	input = "UNLOCK:SPIDER";
	if (map.contains(input)) {
		output = map[std::string(input)].c_str();
		AddHasOverride(input, CallbackOverride("public: void __cdecl LQ_city_hacker::update_TW_mid_hacker(void)", output));
		AddLootOverride(input, CallbackOverride("public: void __cdecl LQ_city_hacker::update_TW_mid_hacker(void)", output));
		AddHasOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
	}

	// ORB_SHOOT
	input = "UNLOCK:ORB_SHOOT";
	if (map.contains(input)) {
		output = map[std::string(input)].c_str();
		AddHasOverride(input, CallbackOverride("public: void __cdecl GW_tuto_orbshoot::update(struct Node2 *)", output));
		AddLootOverride(input, CallbackOverride("public: void __cdecl GW_tuto_orbshoot::update(struct Node2 *)", output));
		AddHasOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
	}

	// SPIDER_GOO
	input = "UNLOCK:SPIDER_GOO";
	if (map.contains(input)) {
		output = map[std::string(input)].c_str();
		AddLootOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
		AddHasOverride(input, CallbackOverride("public: void __cdecl Npc_node::update_hacker(struct Node2 *)", output));
	}
}