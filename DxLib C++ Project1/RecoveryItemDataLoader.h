#pragma once
#include "RecoveryItem.h"
#include <array>
#include <string>

class RecoveryItemDataLoader {
public:

	static bool Load(std::array<RecoveryItemData, ToInt(RecoveryItemType::ITEM_TYPE_MAX)>& outTable);

private:

	static bool LoadData(const char* jsonPath, std::array<RecoveryItemData, ToInt(RecoveryItemType::ITEM_TYPE_MAX)>& outTable);

	// Json’†‚Ì•¶Žš—ñ‚Æenum‚ð‘Î‰ž•t‚¯‚é
	static RecoveryItemType StringToRecoveryType(const std::string& str);
};