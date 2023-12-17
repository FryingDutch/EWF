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
	std::string DefaultScene::getResponse() { return response; }
	std::string DefaultScene::getText(uint32_t _index) { return text[_index]; }

	DefaultScene::DefaultScene(std::vector<std::string> _text) :
		text(_text) {}

	void DefaultScene::setText(std::vector<std::string> _text)
	{
		text = _text;
	}

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
				response = answer;
				break;
			}
				

		} while (!System::isDigit(answer) || std::stoul(answer) > FileParser::file.getOptions().size() || std::stoi(answer) < 0);

		response = answer;
	}
}

