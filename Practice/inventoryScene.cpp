#pragma once
#include <iostream>
#include <string>
#include "../../../System/System.h"
#include "../../../Manager/Scene/SceneManager.h"
#include "InventoryScene.h"
#include "Model/Item/Item.h"
#include "System/FileParser.h"

namespace EWF
{
	void InventoryScene::render(bool responseIsString, std::string _message)
	{
		system("cls");
		this->printStatsBanner();
		
		std::vector<Item> items = SceneManager::m_player.getOwnedItems();
		uint32_t insertEnterCounter = 3;
		uint32_t itemRenderCount = 1;
		uint32_t longestNameSizeEven = 0;
		uint32_t longestQtySizeEven = 0;
		uint32_t longestNameSizeUneven = 0;
		uint32_t longestQtySizeUneven = 0;
		uint32_t longestIdSize = std::to_string(items.size()).size();
		uint32_t lastEvenId = 0;
		uint32_t lastUnevenId = 0;


		uint32_t lastId = 0;
		std::vector<Item> unevenIdItems;

		for (size_t i = 0; i < items.size(); i++)
		{

			if ((i + 2) % 2 == 0)
			{
				items[i].setData("render-id", ++lastId);
				items[i].setData("isEven", true);
				lastEvenId = lastId;
			}

			else
			{
				items[i].setData("original-index", i);
				items[i].setData("isEven", false);
				unevenIdItems.push_back(items[i]);
			}
		}

		for (size_t i = 0; i < unevenIdItems.size(); i++)
		{
			items[unevenIdItems[i].getData("original-index")].setData("render-id", ++lastId);
			lastUnevenId = lastId;
		}

		uint32_t longestIdSizeEven = std::to_string(lastEvenId).size();
		uint32_t longestIdSizeUneven = std::to_string(lastUnevenId).size();

		for (size_t i = 0; i < items.size(); i++)
		{
			uint32_t renderId = uint32_t(items[i].getData("render-id"));
			uint32_t nameSize = items[i].getName().size();
			uint32_t qtySize = std::to_string(items[i].getQuantity()).size();
			uint32_t idSize = std::to_string(renderId).size();

			items[i].setData("name-size", nameSize);
			items[i].setData("id-size", idSize);
			items[i].setData("item-qty-string", "(x" + std::to_string(items[i].getQuantity()) + ")");

			if (items[i].getData("isEven"))
			{
				longestNameSizeEven = (nameSize > longestNameSizeEven) ? nameSize : longestNameSizeEven;
				longestQtySizeEven = (qtySize > longestQtySizeEven) ? qtySize : longestQtySizeEven;
			}
			else
			{
				longestNameSizeUneven = (nameSize > longestNameSizeUneven) ? nameSize : longestNameSizeUneven;
				longestQtySizeUneven = (qtySize > longestQtySizeUneven) ? qtySize : longestQtySizeUneven;
			}
		}

		static const uint32_t providedSpace = 25;
		uint32_t providedSpaceEven = providedSpace + longestNameSizeEven + longestQtySizeEven;
		uint32_t providedSpaceUneven = providedSpace + longestNameSizeUneven + longestQtySizeUneven;

		uint32_t qtyOffsetEven = longestNameSizeEven + 2;
		uint32_t qtyOffsetUneven = longestNameSizeUneven + 2;

		std::cout << "\t";
		for (size_t i = 0; i < items.size(); i++)
		{
			std::cout << "[" << std::to_string(uint32_t(items[i].getData("render-id"))) << "] ";

			uint32_t longestIdSize = (items[i].getData("isEven") ? longestIdSizeEven : longestIdSizeUneven);
			for (size_t j = 0; j < (longestIdSize - items[i].getData("id-size")); j++)
			{
				std::cout << " ";
			}
			std::cout << items[i].getName();

			uint32_t spacesNeeded = providedSpace - std::string(items[i].getData("item-qty-string")).size() - items[i].getData("name-size");
			uint32_t qtyOffset = (items[i].getData("isEven") ? qtyOffsetEven : qtyOffsetUneven);

			for (size_t j = 1; j <= spacesNeeded; j++)
			{
				if (j + items[i].getData("name-size") == qtyOffset)
				{
					std::cout << std::string(items[i].getData("item-qty-string"));
				}
				std::cout << " ";
			}

			if (insertEnterCounter++ % 2 == 0)
			{
				std::cout << "\n\t";
			}
		}

		std::cout << "\n\n";
		FileParser::file.setResponse(System::getInput(_message));
	}
}

