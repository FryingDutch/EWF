#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "AbstractScene.h"
#include "../../../Model/Player/Player.h"
#include "../../../Manager/Scene/SceneManager.h";

namespace EWF {
	// Renders the banner holding all the statistics
	void AbstractScene::printStatsBanner()
	{
		static const uint8_t bannerWidth = 110;

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
		thirdRowStats += std::to_string(SceneManager::m_player.getAge());

		// Unless age is one, always put yrs after
		thirdRowStats += (SceneManager::m_player.getAge() == 1) ? "yr" : "yrs";

		// Add a whitepsace of 1/3 of the total banner width
		for (size_t i = 0; i < bannerWidth / 3; i++) 
		{
			thirdRowStats += " ";
		}

		// Add the player name
		thirdRowStats += SceneManager::m_player.getName();

		// Add a white space of bannerwidth divided by 3, and add 8 to that only for styling, and retract whatever space the name takes up. 
		for (size_t i = 0; i < ((bannerWidth / 3) + 8) - (SceneManager::m_player.getName().size()); i++)
			thirdRowStats += " ";

		// If name has not been given yet, dont display health either.
		if (!SceneManager::m_player.getName().empty())
		{
			thirdRowStats += std::to_string(SceneManager::m_player.getHealth());
			thirdRowStats += "/" + std::to_string(SceneManager::m_player.getMaxHealth()) + " HP";
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
}

