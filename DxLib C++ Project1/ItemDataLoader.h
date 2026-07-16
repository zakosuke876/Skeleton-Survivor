#pragma once
#include "ItemDropConfig.h"
#include "RecoveryItem.h"
#include "ExpItem.h"
#include <string>
#include <array>

class ItemDataLoader {
public:

	// ¬Œ÷‚µ‚½‚çtrue‚ğ•Ô‚µAoutTable‚ÉŒ‹‰Ê‚ğŠi”[‚·‚é
	static bool LoadRecoveryItemData(const char* jsonPath, std::array<RecoveryItemData, ToInt(RecoveryItemType::ITEM_TYPE_MAX)>& outTable);
	static bool LoadExpItemData(const char* jsonPath, ExpItemData& outData);

	static bool LoadDropConfig(const char* jsonPath, ItemDropConfig& outData);

	static bool Load(ItemDropConfig& outData);

private:

	// Json’†‚Ì•¶š—ñ‚Æenum‚ğ‘Î‰•t‚¯‚é
	static RecoveryItemType StringToRecoveryType(const std::string& str);
};