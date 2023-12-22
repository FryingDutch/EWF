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
		static const uint32_t divider = System::getTerminalWidth() > 140 ? 3 : 4;

		do
		{
			// Clear the screen at start, or after a bad input
			system("cls");
			this->printStatsBanner();

			std::vector<std::string> lines;

			std::cout << FileParser::file.formatStoryText(divider) << "\n";
			for (size_t i = 1; i <= FileParser::file.getOptions().size(); i++)
			{
				for (size_t i = 0; i < System::getTerminalWidth() / divider; i++)
				{
					std::cout << " ";
				}

				std::cout << "[" << i << "] " << FileParser::file.getOptionById(i).getText() << "\n";
			}

			std::cout << "\n\n";
			for (size_t i = 0; i < System::getTerminalWidth() / divider; i++)
			{
				std::cout << " ";
			}
			answer = System::getInput(_message);

			if (responseIsString)
			{
				break;
			}
				

		} while (!System::isDigit(answer) || std::stoul(answer) > FileParser::file.getOptions().size() || std::stoi(answer) < 0);

		FileParser::file.setResponse(answer);
	}
}

