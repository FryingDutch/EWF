#pragma once
#include <iostream>
#include <vector>
#include "Model/SceneInterface.h"
#include "json.hpp"
#include "Model/Player/Player.h"

namespace EWF
{
	class InventoryScene : public SceneInterface
	{
	private:
		uint32_t sizeOflongestNameLeftColumn;
		uint32_t sizeOflongestNameRightColumn;

		uint32_t sizeOflongestQtyLeftColumn;
		uint32_t sizeOflongestQtyRightColumn;

		std::vector<Item> m_items;

	public:
		InventoryScene();
		void render(bool responseIsString = false, std::string _message = "Make a choice: ");
		void setLocalItemData(Item& _item, uint32_t& _lastId, uint32_t& sizeOfLongestName, uint32_t& sizeOfLongestQty);
		void setLocalItemsData();
		void renderItems();
	};
}
