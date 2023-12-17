#include <string>
#include "../../../Model/Scene/Default/DefaultScene.h"
#include "../../System/FileParser.h"
#include "../../System/ItemImporter.h"
#include "../../System/System.h"
#include "../../Optionr.h"
#include "json.hpp"
#include "../../Model/Player/Player.h"

/*implementation for >*/ #include "SceneManager.h"
namespace EWF
{
	std::string SceneManager::response = "";

	DefaultScene SceneManager::defaultScene;

	Player SceneManager::m_player;
	

	void SceneManager::applyStatsChanges(size_t _i)
	{
		StoryOption option = FileParser::file.getOptionById(_i);
		std::vector<std::vector<std::string>> varChanges = option.getVariableChanges();

		for (size_t j = 0; j < varChanges.size(); j++)
		{
			std::string variableToChange = varChanges[j][FileParser::NEW_VARIABLE];
			char log_operator = varChanges[j][FileParser::LOGICAL_OPERATOR][0];
			uint32_t value{ 0 };
			std::string valueStr{ "" };

			if (System::isDigit(varChanges[j][FileParser::VALUE]))
			{
				value = std::stoi(varChanges[j][FileParser::VALUE]);
			}

			else 
			{
				valueStr = varChanges[j][FileParser::VALUE];
			}

			if (valueStr.size() > 0) {
				switch (log_operator)
				{
				case '=':
					SceneManager::m_player.setData(variableToChange, valueStr);
					break;

				default:
					break;

				}
			}

			else 
			{
				switch (log_operator)
				{
				case '=':
					SceneManager::m_player.setData(variableToChange, value);
					break;

				case '+':
					SceneManager::m_player.setData(variableToChange, SceneManager::m_player.getData(variableToChange) + value);
					break;

				case '-':
					SceneManager::m_player.setData(variableToChange, SceneManager::m_player.getData(variableToChange) + value);
					break;

				}
			}

			if (SceneManager::m_player.getHealth() <= 0)
			{
				std::cout << "You took " << value << " DMG. \n";
				System::errorMessage("You have died...", true);
			}

			else if (variableToChange == FileParser::m_variablesMap["get-item"])
			{
				if(valueStr == "RESPONSE"){
					Player::setItemActive(response);
				} else {
					Player::setItemActive(valueStr);
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
			defaultScene.render(FileParser::m_responseIsString, 
				(FileParser::m_customMessage.size() > 0) ? FileParser::m_customMessage : FileParser::m_message);

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
			FileParser::m_filePath = FileParser::file.getOptionById(1).getLink();
			applyStatsChanges(1);
		}

		else if (response == "0")
		{
			FileParser::m_filePath = "menu";
		}

		else
		{
			FileParser::m_filePath = FileParser::file.getOptionById(stoi(response)).getLink();
			applyStatsChanges(stoi(response));
		}

		if(FileParser::file.getOptions().size() <= 0)
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