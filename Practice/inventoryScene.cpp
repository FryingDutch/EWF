#pragma once
#include <iostream>
#include <string>
#include "../../../System/System.h"
#include "../../../Manager/Scene/SceneManager.h"
#include "InventoryScene.h"
#include "Model/Item/Item.h"

namespace EWF
{
	void InventoryScene::render(bool responseIsString, std::string _message)
	{
		this->printStatsBanner();
		
		std::vector<Item> items = SceneManager::m_player.getItemsAsObjects();
		uint32_t counter = 5;
		uint32_t counter2 = 1;

		for (size_t i = 0; i < items.size(); i++)
		{
			if (items[i].getOwned()) {
				std::cout << counter2++ << ") " << items[i].getName() << " x" << items[i].getQuantity();

				for (size_t i = 0; i < items[i].getName().size(); i++)
				{
					std::cout << " ";
				}

				std::cout << "    ";

				if (counter++ % 4 == 0)
				{
					std::cout << "\n";
				}
			}
		}

		std::cout << "\n\n";

		System::getInput();
	}
}

