#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include "../../../System/System.h"
#include "System/FileParser.h"
#include "../../../Manager/Scene/SceneManager.h"
#include "ItemSpecScene.h"

namespace EWF
{
	void ItemSpecScene::render(bool responseIsString, std::string _message)
	{
		Item item = SceneManager::m_player.getItemById(stoi(FileParser::m_fileArgument));
		std::string response;

		do {
			system("cls");
			this->printStatsBanner();

			for (size_t i = 0; i < System::getTerminalWidth() / 7; i++)
			{
				std::cout << " ";
			}

			std::cout << "\"" << item.getDescription() << "\"\n\n";
			for (size_t i = 0; i < System::getTerminalWidth() / 3; i++)
			{
				std::cout << " ";
			}

			std::cout << "Name:     " << item.getName() << "\n";
			for (size_t i = 0; i < System::getTerminalWidth() / 3; i++)
			{
				std::cout << " ";
			}

			std::cout << "Quantity: " << item.getQuantity() << "\n\n";
			for (size_t i = 1; i <= FileParser::file.getOptions().size(); i++)
			{
				for (size_t i = 0; i < System::getTerminalWidth() / 6; i++)
				{
					std::cout << " ";
				}
				std::cout << "[" << i << "] " << FileParser::file.getOptionById(i).getText() << "\n";
			}
			std::cout << "\n";

			for (size_t i = 0; i < System::getTerminalWidth() / 6; i++)
			{
				std::cout << " ";
			}
			response = System::getInput(_message);
		} while (!System::isDigit(response) || std::stoul(response) > FileParser::file.getOptionsAsJson().size() || std::stoi(response) < 0);

		if (stoi(response) == 0) {
			StoryOption nextSceneOption;
			uint32_t newId = FileParser::file.getOptionsAsJson().size() + 1;
			nextSceneOption.setId(newId);
			nextSceneOption.setLink("inventory");
			FileParser::file.addOption(nextSceneOption);
			response = std::to_string(newId);
		}
		FileParser::file.setResponse(response);
	}
}

