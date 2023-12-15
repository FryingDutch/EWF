#pragma once
#include <iostream>
#include <string>
#include "../../../System.h"
#include "../../../Player.h"
#include "../../../SceneManager.h"

namespace EWF
{

	std::string DefaultScene::getResponse() { return response; }
	std::string DefaultScene::getText(uint32_t _index) { return text[_index]; }

	DefaultScene::DefaultScene(std::vector<std::string> _text)
	{
		text = _text;
	}

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
			for (size_t i = 0; i < text.size(); i++)
			{
				// Check if the string provided is not the defaulted empty one
				if (i < text.size() && !text[i].empty())
				{
					if (!responseIsString)
					{
						switch (i)
						{
						case STORY:
							std::cout << text[STORY] << "\n\n";
							break;

						default:
							std::cout << "\t" << i << ") " << text[i] << "\n";
							break;
						}
					}

					else
					{
						switch (i)
						{
						case STORY:
							std::cout << text[STORY] << "\n\n";
							break;

						default:
							break;
						}
					}
				}

				else
				{
					std::cout << "\n";
					break;
				}
			}

			std::cout << "\n";
			answer = System::getInput(_message);

			if (responseIsString)
			{
				response = answer;
				break;
			}
				

		} while (!System::isDigit(answer) || std::stoul(answer) >= text.size() || std::stoi(answer) < 0);

		response = answer;
	}
}

