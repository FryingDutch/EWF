#include <string>
#include "../../../Model/Scene/Default/DefaultScene.h"
#include "../../System/FileParser.h"
#include "../../System/ItemImporter.h"
#include "../../System/System.h"
#include "../../Optionr.h"
#include "json.hpp"
#include "../../Model/Player/Player.h"
#include "../../../InventoryScene.h"
#include "../../../ItemSpecScene.h"

/*implementation for >*/ #include "SceneManager.h"
namespace EWF
{
	DefaultScene SceneManager::defaultScene;
	InventoryScene SceneManager::inventoryScene;

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

			if (variableToChange == FileParser::m_variablesMap["get-item"])
			{
				Item newItem = SceneManager::m_player.getItemByName((valueStr == "RESPONSE") 
					? FileParser::file.getResponse() 
					: valueStr);

				newItem.setOwned(true);
				newItem.setQuantity(1);
				SceneManager::m_player.updateItem(newItem);
			}

			else if (valueStr.size() > 0) 
			{
				switch (log_operator)
				{
				case '=':
					SceneManager::m_player.setData(variableToChange, (valueStr == "RESPONSE") 
						? FileParser::file.getResponse() 
						: valueStr);
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
		}

		if (SceneManager::m_player.getHealth() > SceneManager::m_player.getMaxHealth())
		{
			SceneManager::m_player.setHealth(SceneManager::m_player.getMaxHealth());
		}
	}

	// Build the scene according to the _sceneType (Which is provided by the file with ~ prefix)
	void SceneManager::renderScene()
	{
		ItemSpecScene itemSpecScene;

		switch (FileParser::file.getSceneType())
		{
		case DEFAULT:
			defaultScene.render(FileParser::m_responseIsString,
				(FileParser::file.getMessage().size() > 0) 
					? FileParser::file.getMessage()
					: FileParser::m_message);
			break;

		case INTRO:
			inventoryScene = InventoryScene();
			inventoryScene.render(FileParser::m_responseIsString,
				(FileParser::file.getMessage().size() > 0)
				? FileParser::file.getMessage()
				: FileParser::m_message);
			break;

		case 'S':
			itemSpecScene.render(FileParser::m_responseIsString,
				(FileParser::file.getMessage().size() > 0)
				? FileParser::file.getMessage()
				: FileParser::m_message);
			break;

		// TO-DO add more sceneTemplates to use.
		default:
			System::errorMessage("Not a valid sceneType", true);
			return;
		}
	}

	void SceneManager::setNextFileToRead()
	{
		// Set the next file to read.
		if (FileParser::m_responseIsString)
		{
			FileParser::m_filePath = FileParser::file.getOptionById(1).getLink();
		}

		else if (FileParser::file.getResponse() == "0")
		{
			FileParser::m_filePath = "menu";
		}

		else
		{
			uint32_t response = stoi(FileParser::file.getResponse());
			FileParser::m_filePath = FileParser::file.getOptionById(response).getLink();
			static const std::string itemSpecFilePath = "item\\spec\\";
			size_t _indexPos = FileParser::m_filePath.find(itemSpecFilePath);

			if (_indexPos != std::string::npos)
			{
				FileParser::m_fileArgument = FileParser::m_filePath.substr(_indexPos + itemSpecFilePath.length());
				FileParser::m_filePath = "item\\spec\\id";
			}
		}

		if (FileParser::file.getOptions().size() <= 0)
		{
			System::errorMessage("No file link bound to this choice", true);
		}
	}

	void SceneManager::applyNewStatChanges()
	{
		if (FileParser::m_responseIsString)
		{
			applyStatsChanges(1);
		}

		else
		{
			uint32_t response = stoi(FileParser::file.getResponse());
			applyStatsChanges(response);
		}
	}

	void SceneManager::runGame()
	{
		if (ItemImporter::import()) 
		{
			while (System::isRunning)
			{
				System::resetDimensions();
				FileParser::loadFileContent();
				SceneManager::renderScene();
				SceneManager::applyNewStatChanges();
				SceneManager::setNextFileToRead();
			}
		}
	}
}