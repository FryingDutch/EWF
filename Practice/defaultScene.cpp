#pragma once
#include <iostream>
#include <string>
#include "System.h"
#include "Player.h"
#include "SceneManager.h"

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

	// Renders the banner holding all the statistics
	void DefaultScene::printStatsBanner()
	{
		static const uint8_t bannerWidth = 100;

		// First row
		for (size_t i = 0; i < bannerWidth; i++)
			std::cout << "-";
		std::cout << "\n";

		// Second row
		std::cout << "|";

		// minus 2 for printing "|" at start and end.
		for (size_t i = 0; i < bannerWidth - 2; i++)
			std::cout << " ";

		std::cout << "|";
		std::cout << "\n";

		// Third Row
		std::cout << "|";

		// Make whitespace for styling
		std::string thirdRowStats = "    ";

		// Add the age
		thirdRowStats += std::to_string(Player::getAge());

		// Unless age is one, always put yrs after
		thirdRowStats += (Player::getAge() == 1) ? "yr" : "yrs";

		// Add a whitepsace of 1/3 of the total banner width
		for (size_t i = 0; i < bannerWidth / 3; i++)
			thirdRowStats += " ";

		// Add the player name
		thirdRowStats += Player::getName();

		// Add a white space of bannerwidth divided by 3, and add 8 to that only for styling, and retract whatever space the name takes up. 
		for (size_t i = 0; i < ((bannerWidth / 3) + 8) - (Player::getName().size()); i++)
			thirdRowStats += " ";

		// If name has not been given yet, dont display health either.
		if (!Player::getName().empty())
		{
			thirdRowStats += std::to_string(Player::getHealth());
			thirdRowStats += "/" + std::to_string(Player::getMaxHealth()) + " HP";
		}

		std::cout << thirdRowStats;

		for (size_t i = 0; i < (bannerWidth - thirdRowStats.size() - 2); i++)
			std::cout << " ";

		std::cout << "|";
		std::cout << "\n";

		// Fourth Row
		std::cout << "|";

		for (size_t i = 0; i < (bannerWidth - 2); i++)
			std::cout << " ";

		std::cout << "|";
		std::cout << "\n";

		// Fifth row
		for (size_t i = 0; i < bannerWidth; i++)
			std::cout << "-";

		// Leave with a row of white space
		std::cout << "\n\n";
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
			printStatsBanner();
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

