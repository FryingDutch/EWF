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
		system("cls");
		this->printStatsBanner();

		Item item = SceneManager::m_player.getItemById(stoi(FileParser::m_fileArgument));
		std::cout << "\t\t\"" << item.getDescription() << "\"\n\n";
		std::cout << "\t\tID: " << item.getId() << "\n\t\tName: " << item.getName() << "\n\n";

		for (size_t i = 1; i <= FileParser::file.getOptions().size(); i++)
		{
			std::cout << "\t\t" << "[" << i << "] " << FileParser::file.getOptionById(i).getText() << "\n";
		}
		std::cout << "\n";

		std::string response = System::getInput("\t\t" + _message);
		FileParser::file.setResponse(response);
	}
}

