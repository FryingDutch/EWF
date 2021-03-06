#include <string>
#include "DefaultScene.h"
#include "IntroScene.h"
#include "SceneManager.h"
#include "FileParser.h"

namespace EWF
{
	uint32_t SceneManager::response = 0;

	DefaultScene SceneManager::defaultScene;
	IntroScene SceneManager::introScene;

	void SceneManager::applyStatsChanges(size_t _i)
	{
		for (size_t j = 0; j < FileParser::fileLinks[_i].variableChanges.size(); j++)
		{
			std::string variableToChange = FileParser::fileLinks[_i].variableChanges[j][FileParser::NEW_VARIABLE];
			char log_operator = FileParser::fileLinks[_i].variableChanges[j][FileParser::LOGICAL_OPERATOR][0];
			int32_t value;
			std::string valueStr{ "" };
			if (System::isDigit(FileParser::fileLinks[_i].variableChanges[j][FileParser::VALUE]))
				value = std::stoi(FileParser::fileLinks[_i].variableChanges[j][FileParser::VALUE]);
				
			else
				valueStr = FileParser::fileLinks[_i].variableChanges[j][FileParser::VALUE];

			if (variableToChange == FileParser::variables[FileParser::HP])
			{
				switch (log_operator)
				{
				case '=':
					(value > Player::getMaxHealth()) ? Player::setHealth(Player::getMaxHealth()) : Player::setHealth(value);
					break;

				case '+':
					((Player::getHealth() + value) > Player::getMaxHealth()) ? Player::setHealth(Player::getMaxHealth()) : Player::setHealth(Player::getHealth() + value);
					break;

				case '-':
					((Player::getHealth() - value) <= 0) ? Player::setHealth(0) : Player::setHealth(Player::getHealth() - value);
					break;

				}

				if (Player::getHealth() <= 0)
				{
					std::cout << "You took " << value << " DMG. \n";
					System::errorMessage("You have died...", true);
				}
			}

			else if (variableToChange == FileParser::variables[FileParser::AGE])
			{
				switch (log_operator)
				{
				case '=':
					Player::setAge(value);
					break;

				case '+':
					Player::setAge(Player::getAge() + value);
					break;

				case '-':
					((Player::getAge() - value) < 0) ? Player::setAge(0) : Player::setAge(Player::getAge() - value);
					break;

				}
			}

			else if (variableToChange == FileParser::variables[FileParser::ATK])
			{
				switch (log_operator)
				{
				case '=':
					Player::setAttack(value);
					break;

				case '+':
					Player::setAttack(Player::getAttack() + value);
					break;

				case '-':
					((Player::getAttack() - value) < 0) ? Player::setAttack(0) : Player::setAttack(Player::getAttack() - value);
					break;

				}
			}

			else if (variableToChange == FileParser::variables[FileParser::DEF])
			{
				switch (log_operator)
				{
				case '=':
					Player::setDefense(value);
					break;

				case '+':
					Player::setDefense(Player::getDefense() + value);
					break;

				case '-':
					((Player::getDefense() - value) < 0) ? Player::setDefense(0) : Player::setDefense(Player::getDefense() - value);
					break;

				}
			}

			else if (variableToChange == FileParser::variables[FileParser::NAME])
			{
				if (!valueStr.empty())
				{
					switch (log_operator)
					{
					case '=':
						Player::setName(valueStr);
						break;

					default:
						System::errorMessage("Not a valid operator for NAME");
						break;
					}
				}
			}

		}

	}
	// Build the scene according to the _sceneType (Which is provided by the file with ~ prefix)
	void SceneManager::buildScene(char _sceneType)
	{
		switch (FileParser::sceneType)
		{
		case INTRO:
			introScene.setText(FileParser::textBlocks);
			(FileParser::customMessage.size() > 0) ? introScene.render(FileParser::customMessage) : introScene.render(FileParser::message); // If custom message is found in file, render with that, otherwise default
			response = 1;
			break;

		case DEFAULT:
			defaultScene.setText(FileParser::textBlocks);
			(FileParser::customMessage.size() > 0) ? defaultScene.render(FileParser::customMessage) : defaultScene.render(FileParser::message);
			response = defaultScene.getResponse();
			break;

		// TO-DO add more sceneTemplates to use.
		default:
			System::errorMessage("Not a valid sceneType", true);
			return;
		}

		// Set the next file to read.
		for (size_t i = 0; i < FileParser::fileLinks.size(); i++)
		{
			// Go trough all links, check the bound choices
			for (size_t j = 0; j < FileParser::fileLinks[i].boundChoices.size(); j++)
			{
				if (response == FileParser::fileLinks[i].boundChoices[j])
				{
					FileParser::filePath = FileParser::fileLinks[i].link; // if its a match, return the current link
					applyStatsChanges(i);
				}
			}
		}

		// else if ((response - 1) = 0)
			// Go to menu

		if(FileParser::fileLinks.size() <= 0)
			System::errorMessage("No file link bound to this choice", true);
	}

	void SceneManager::runGame()
	{
		while (System::isRunning)
		{
			FileParser::loadText();
			buildScene(FileParser::sceneType);
		}
	}
}