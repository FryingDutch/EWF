#include <string>
#include "DefaultScene.h"
#include "SceneManager.h"
#include "FileParser.h"
#include "ItemImporter.h"

namespace EWF
{
	std::string SceneManager::response = "";

	DefaultScene SceneManager::defaultScene;

	void SceneManager::applyStatsChanges(size_t _i)
	{
		for (size_t j = 0; j < FileParser::m_fileLinks[_i].variableChanges.size(); j++)
		{
			std::string variableToChange = FileParser::m_fileLinks[_i].variableChanges[j][FileParser::NEW_VARIABLE];
			char log_operator = FileParser::m_fileLinks[_i].variableChanges[j][FileParser::LOGICAL_OPERATOR][0];
			uint32_t value{ 0 };
			std::string valueStr{ "" };

			if (System::isDigit(FileParser::m_fileLinks[_i].variableChanges[j][FileParser::VALUE]))
			{
				value = std::stoi(FileParser::m_fileLinks[_i].variableChanges[j][FileParser::VALUE]);
			}

			else 
			{
				valueStr = FileParser::m_fileLinks[_i].variableChanges[j][FileParser::VALUE];
			}

			if (variableToChange == FileParser::m_variablesMap["health"])
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

			else if (variableToChange == FileParser::m_variablesMap["age"])
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

			else if (variableToChange == FileParser::m_variablesMap["attack"])
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

			else if (variableToChange == FileParser::m_variablesMap["defense"])
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

			else if (variableToChange == FileParser::m_variablesMap["maximum-health"])
			{
				switch (log_operator)
				{
				case '=':
					Player::setMaxHealth(value);
					break;

				case '+':
					Player::setMaxHealth(Player::getMaxHealth() + value);
					break;

				case '-':
					((Player::getMaxHealth() - value) < 1) ? Player::setMaxHealth(1) : Player::setMaxHealth(Player::getMaxHealth() - value);
					break;

				}
			}

			else if (variableToChange == FileParser::m_variablesMap["get-item"])
			{
				Player::setItemActive(valueStr);
			}

			else if (variableToChange == FileParser::m_variablesMap["name"])
			{
				if (!valueStr.empty() && valueStr != "RESPONSE")
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

				else if(valueStr == "RESPONSE"){
					Player::setName(response);
				}
			}

		}

	}
	// Build the scene according to the _sceneType (Which is provided by the file with ~ prefix)
	void SceneManager::buildScene(char _sceneType)
	{
		switch (FileParser::m_sceneType)
		{
		case DEFAULT:
			defaultScene.setText(FileParser::m_textBlocks);
			(FileParser::m_customMessage.size() > 0) 
				? defaultScene.render(FileParser::m_responseIsString, FileParser::m_customMessage) 
				: defaultScene.render(FileParser::m_responseIsString, FileParser::m_message);

			response = defaultScene.getResponse();
			break;

		// TO-DO add more sceneTemplates to use.
		default:
			System::errorMessage("Not a valid sceneType", true);
			return;
		}

		// Set the next file to read.
		if (FileParser::m_responseIsString)
		{
			FileParser::m_filePath = FileParser::m_fileLinks[0].link;
			applyStatsChanges(0);
		}

		else if (response == "0")
		{
			FileParser::m_filePath = "menu";
		}

		else
		{
			for (size_t i = 0; i < FileParser::m_fileLinks.size(); i++)
			{
				for (size_t j = 0; j < FileParser::m_fileLinks[i].boundChoices.size(); j++)
				{
					if (response == FileParser::m_fileLinks[i].boundChoices[j])
					{
						FileParser::m_filePath = FileParser::m_fileLinks[i].link; // if its a match, set the current link
						applyStatsChanges(i);
					}
				}
			}
		}

		if(FileParser::m_fileLinks.size() <= 0)
			System::errorMessage("No file link bound to this choice", true);
	}

	void SceneManager::runGame()
	{
		if (ItemImporter::import()) 
		{
			while (System::isRunning)
			{
				FileParser::loadText();
				buildScene(FileParser::m_sceneType);
			}
		}
	}
}