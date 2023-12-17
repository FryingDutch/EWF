#pragma once
#include <string>
#include "../Model/Entity/Entity.h"
#include "../Model/Player/Player.h"
#include "../Model/Item/Item.h"
#include "../../../Manager/Scene/SceneManager.h"
#include "json.hpp"
#include <iostream>
#include <fstream>

namespace EWF
{
	class ItemImporter
	{
	public:
		static bool import() 
		{ 
			std::ifstream itemFile("items.json");
			if (!itemFile.is_open()) {
				std::cerr << "Error opening file." << std::endl;
				return false;
			}

			nlohmann::json items;
			itemFile >> items;

			for (size_t i = 0; i < items.size(); i++)
			{
				Item item{ items[i] };
				SceneManager::m_player.addItem(item);
			}

			return true; 
		}
	};
}