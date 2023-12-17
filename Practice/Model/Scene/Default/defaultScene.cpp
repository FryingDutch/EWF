#pragma once
#include <iostream>
#include <string>
#include "../../../System/System.h"
#include "../../../System/FileParser.h"
#include "../../../Model/Player/Player.h"
#include "../../../Manager/Scene/SceneManager.h"
#include "../../SceneInterface.h"

namespace EWF
{
	// Renders the scene, customized trough arguments
	void DefaultScene::render(bool responseIsString, std::string _message)
	{
		static std::string answer;
		static int32_t argc;
		
		do
		{
			// Clear the screen at start, or after a bad input
			system("cls");
			this->printStatsBanner();

			std::cout << FileParser::file.getStory() << "\n\n";
			for (size_t i = 1; i <= FileParser::file.getOptions().size(); i++)
			{
				std::cout << "\t" << i << ") " << FileParser::file.getOptionById(i).getText() << "\n";
			}

			std::cout << "\n";
			answer = System::getInput(_message);

			if (responseIsString)
			{
				break;
			}
				

		} while (!System::isDigit(answer) || std::stoul(answer) > FileParser::file.getOptions().size() || std::stoi(answer) < 0);

		FileParser::file.setResponse(answer);
	}
}

