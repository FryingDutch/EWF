#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include "../../../System/System.h"
#include "../../../Manager/Scene/SceneManager.h"
#include "InventoryScene.h"
#include "Model/Item/Item.h"
#include "System/FileParser.h"

namespace EWF
{
	InventoryScene::InventoryScene() :
		m_items(SceneManager::m_player.getOwnedItems()), sizeOflongestNameLeftColumn(0), sizeOflongestNameRightColumn(0), sizeOflongestQtyLeftColumn(0), sizeOflongestQtyRightColumn(0) {}

	void InventoryScene::setLocalItemData(Item& _item, uint32_t& _lastId, uint32_t& sizeOfLongestName, uint32_t& sizeOfLongestQty)
	{
		uint32_t renderId = ++_lastId;
		uint32_t nameSize = _item.getName().size();
		uint32_t qtySize = std::to_string(_item.getQuantity()).size();

		_item.setData("render-id", renderId);
		_item.setData("name-size", nameSize);
		_item.setData("id-size", std::to_string(renderId).size());
		_item.setData("item-qty-string", "(x" + std::to_string(_item.getQuantity()) + ")");

		sizeOfLongestName = (nameSize > sizeOfLongestName)
			? nameSize
			: sizeOfLongestName;

		sizeOfLongestQty = (qtySize > sizeOfLongestQty)
			? qtySize
			: sizeOfLongestQty;
	}

	void InventoryScene::setLocalItemsData()
	{
		uint32_t lastId = 0;
		uint32_t largestLeftId = round(long double(m_items.size()) / 2);

		for (size_t i = 0; lastId < largestLeftId && i < m_items.size(); i+=2)
		{
			m_items[i].setData("isEven", true);
			setLocalItemData(m_items[i], lastId, this->sizeOflongestNameLeftColumn, this->sizeOflongestQtyLeftColumn);
		}

		for (size_t i = 1; lastId < m_items.size() && i <= m_items.size(); i += 2)
		{
			m_items[i].setData("isEven", false);
			setLocalItemData(m_items[i], lastId, this->sizeOflongestNameRightColumn, this->sizeOflongestQtyRightColumn);
		}
	}

	void InventoryScene::renderItems()
	{
		uint32_t insertEnterCounter = 3;
		uint32_t itemRenderCount    = 1;

		uint32_t longestIdSizeLeft  = std::to_string(uint32_t(round(long double(m_items.size()) / 2))).size();
		uint32_t longestIdSizeRight = std::to_string((m_items.size() % 2 == 0) 
			? m_items.size() 
			: m_items.size() - 1).size();

		static const uint32_t providedSpace = 40;

		uint32_t qtyOffsetEven	 = sizeOflongestNameLeftColumn + 2;
		uint32_t qtyOffsetUneven = sizeOflongestNameRightColumn + 2;

		for (size_t i = 0; i < System::getTerminalWidth() / 4; i++)
		{
			std::cout << " ";
		}

		for (size_t i = 0; i < m_items.size(); i++)
		{
			std::cout << "[" << std::to_string(uint32_t(m_items[i].getData("render-id"))) << "] ";

			uint32_t longestIdSize = (m_items[i].getData("isEven") 
				? longestIdSizeLeft 
				: longestIdSizeRight);

			for (size_t j = 0; j < (longestIdSize - m_items[i].getData("id-size")); j++)
			{
				std::cout << " ";
			}
			std::cout << m_items[i].getName();

			uint32_t spacesNeeded = providedSpace - std::string(m_items[i].getData("item-qty-string")).size() - m_items[i].getData("name-size");
			uint32_t qtyOffset = m_items[i].getData("isEven") 
				? qtyOffsetEven 
				: qtyOffsetUneven;

			for (size_t j = 1; j <= spacesNeeded; j++)
			{
				if (j + m_items[i].getData("name-size") == qtyOffset)
				{
					std::cout << std::string(m_items[i].getData("item-qty-string"));
				}
				std::cout << " ";
			}

			if (insertEnterCounter++ % 2 == 0)
			{
				std::cout << "\n";
				for (size_t i = 0; i < System::getTerminalWidth() / 4; i++)
				{
					std::cout << " ";
				}
			}
		}
	}

	void InventoryScene::render(bool responseIsString, std::string _message)
	{	
		this->setLocalItemsData();	

		static std::string response = "";
		std::string renderId;

		do {
			system("cls");
			this->printStatsBanner();
			this->renderItems();

			std::cout << "\n\n";

			for (size_t i = 0; i < System::getTerminalWidth() / 6; i++)
			{
				std::cout << " ";
			}

			renderId = System::getInput(_message);

			for (size_t i = 0; i < m_items.size(); i++)
			{
				if (m_items[i].getData("render-id") == stoi(renderId)) {
					response = std::to_string(m_items[i].getId());
					break;
				}
			}

			if (response == "" || renderId == "0")
			{
				response = renderId;
			}

		} while (!System::isDigit(response) || std::stoul(renderId) > m_items.size() || std::stoi(response) < 0);

		StoryOption nextSceneOption;
		if (std::stoi(response) == 0)
		{
			nextSceneOption.setId(1);
			nextSceneOption.setLink("menu");
		}

		else 
		{
			nextSceneOption.setId(stoi(response));
			nextSceneOption.setLink("item\\spec\\" + response);
		}
		FileParser::file.addOption(nextSceneOption);
		FileParser::file.setResponse(response);
	}
}

