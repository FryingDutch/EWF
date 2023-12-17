#include "System.h"
#include "FileParser.h"
#include <functional>
#include <algorithm>
#include <map>
#include "../Model/Player/Player.h"
#include "../Optionr.h"
#include "json.hpp"

namespace EWF
{
	std::string FileParser::m_goToFile = "";
	std::vector<std::string> FileParser::m_textBlocks;
	std::vector<FileParser::FileLink> FileParser::m_fileLinks;
	File FileParser::file;

	std::map<std::string, bool> FileParser::m_readingFlagValueMap = {
		{"block", false},
		{"message", false},
		{"filelink", false},
		{"scenetype", false}
	};

	std::map<std::string, std::string> FileParser::m_variablesMap = {
		{"name", "NAME"},
		{"age", "AGE"},
		{"health", "HP"},
		{"maximum-health", "MAXHP"},
		{"attack", "ATK"},
		{"defense", "DEF"},
		{"use-item", "USEITEM"},
		{"get-item", "GETITEM"}
	};

	std::map<std::string, char> FileParser::operatorsMap = {
		{"plus", '+'},
		{"minus", '-'},
		{"equals", '='},
		{"variable", '$'}
	};

	char FileParser::m_sceneType;
	std::string FileParser::m_filePath = "first-scene";
	std::string FileParser::m_fileContent;

	std::string FileParser::m_block = "";
	std::string FileParser::m_message = "Make a choice: ";
	std::string FileParser::m_customMessage = "";

	size_t FileParser::m_index = 0;
	bool FileParser::m_responseIsString{ false };

	FileParser::FileParser() 
	{};


	/// <summary>
	/// Checks whether or not the current char in the file is the start of a flag
	/// </summary>
	/// <param name="flag">The string representation of the flag to check</param>
	/// <param name="startFlagToCheck">Defaults to -1 for no starting flag checking, this param is the index of the flag to be removed for checking</param>
	/// <param name="disableFlagCheck">This makes the function not check any other flags for reading, this is for variable checking spefic</param>
	/// <todo>This is way too comlex and too spefic to support each flag, needs to be generalised</todo>
	/// <returns>boolean</returns>
	bool FileParser::isFlag(std::string flag, std::vector<std::string> flagsToIgnore = {}, bool disableFlagCheck = false)
	{
		uint32_t flagSize = flag.size();
		std::string currentBlock = m_fileContent.substr(m_index, flag.size());

		bool enoughCharsLeftToRead = ((m_index + flagSize) < m_fileContent.size());
		bool readingOtherFlags = false;
		bool ignoreOtherFlags = flagsToIgnore.size() >= 0 || disableFlagCheck;

		if (!ignoreOtherFlags) {
			for (const auto& keyValue : FileParser::m_readingFlagValueMap) {
				std::string flagKey = keyValue.first;
				bool flagIsBeingRead = keyValue.second;
				bool ignoreFlag = false;

				for (size_t i = 0; i < flagsToIgnore.size(); i++) {
					if (flagKey == flagsToIgnore[i])
					{
						ignoreFlag = true;
					}
				}

				if (!ignoreFlag && flagIsBeingRead) {
					readingOtherFlags = true;
					break;
				}
			}
		}

		bool currentBlockEqualsFlag = (currentBlock == flag);

		if (enoughCharsLeftToRead && !readingOtherFlags && currentBlockEqualsFlag) {
			if (flag != std::string(1, operatorsMap["variable"])) {
				m_index += flagSize;
			}
			return true;
		}

		return false;
	}

	bool FileParser::isVariableFlag()
	{
		return FileParser::isFlag(std::string(1, operatorsMap["variable"]), {}, true);
	}

	bool FileParser::isStartMessageFlag()
	{
		return FileParser::isFlag("-*");
	}

	bool FileParser::isEndMessageFlag()
	{
		return FileParser::isFlag("-/*", {"message"});
	}

	bool FileParser::isStartBlockFlag()
	{
		return FileParser::isFlag("<>");
	}

	bool FileParser::isStartStoryBlockFlag()
	{
		return FileParser::isFlag("<story>");
	}

	bool FileParser::isStartOptionBlockFlag()
	{
		return FileParser::isFlag("<option>");
	}

	bool FileParser::isEndBlockFlag()
	{
		return FileParser::isFlag("</>", { "block" });
	}

	bool FileParser::isStartFileLinkFlag()
	{
		return FileParser::isFlag("#");
	}

	bool FileParser::isEndFileLinkFlag()
	{
		return FileParser::isFlag("#!", {"filelink"});
	}

	bool FileParser::isSceneTypeFlag()
	{
		return FileParser::isFlag("~");
	}

	void FileParser::handleBlock()
	{
		m_readingFlagValueMap["block"] = true;
		StoryOption storyOption;

		while (!isEndBlockFlag())
		{
			// if at end of file and block is still being read
			if ((m_index + 1) == m_fileContent.size())
			{
				System::errorMessage("Block not closed </>", true);
				return;
			}

			m_block += m_fileContent[m_index];
			m_index++;
		}

		m_readingFlagValueMap["block"] = false;
		m_textBlocks.push_back(m_block);
		m_block = "";
	}

	void FileParser::handleStoryBlock()
	{
		m_readingFlagValueMap["block"] = true;
		while (!isEndBlockFlag())
		{
			// if at end of file and block is still being read
			if ((m_index + 1) == m_fileContent.size())
			{
				System::errorMessage("Block not closed </>", true);
				return;
			}

			FileParser::file.setStory(FileParser::file.getStory() + m_fileContent[m_index]);
			m_index++;
		}
		m_readingFlagValueMap["block"] = false;
	}

	void FileParser::handleOptionBlock()
	{
		m_readingFlagValueMap["block"] = true;
		StoryOption storyOption;

		uint32_t currentOptionCount = FileParser::file.getOptions().size();
		storyOption.setId(++currentOptionCount);

		while (!isEndBlockFlag())
		{
			// if at end of file and block is still being read
			if ((m_index + 1) == m_fileContent.size())
			{
				System::errorMessage("Block not closed </>", true);
				return;
			}

			storyOption.setText(storyOption.getText() + m_fileContent[m_index]);
			
			m_index++;
		}

		FileParser::file.addOption(storyOption);
		m_readingFlagValueMap["block"] = false;
	}

	void FileParser::handleFileLink()
	{
		m_readingFlagValueMap["filelink"] = true;

		// Create a fileLink to be filled with both the choices as well as the link and variable changes.
		FileLink fileLink;

		static const char MULTI_CHOICE_OPERATOR = ',';

		std::vector<uint32_t> optionIds;

		// Until the maximum allowed of elements, keep looping, adding 2 to position to get the digit we need.
		for (; m_index <= m_fileContent.size(); m_index += 2)
		{
			// if the second position of the element [#1][,2][,3] is a digit
			if ((m_index + 2) < m_fileContent.size() && std::isdigit(m_fileContent[m_index]))
			{
				std::string choice{ m_fileContent[m_index] };
				fileLink.boundChoices.push_back(choice);

				uint32_t optionId = stoi(choice);
				optionIds.push_back(optionId);

				// If the next element fits the standard, do another cycle
				if (std::isdigit(m_fileContent[m_index + 2]) && m_fileContent[m_index + 1] == MULTI_CHOICE_OPERATOR)
				{
					continue;
				}

				// Else if there is a [,non_digit], end program. 
				else if (m_fileContent[m_index + 1] == MULTI_CHOICE_OPERATOR && !std::isdigit(m_fileContent[m_index + 2]))
				{
					static std::string msg = ("Mullti choice operator " + MULTI_CHOICE_OPERATOR) + std::string(" not followed by digit");
					System::errorMessage(msg.c_str(), true);
				}

				// else break the loop
				else
					break;
			}
		}

		nlohmann::json optionsAsJson = FileParser::file.getOptions();
		std::vector<StoryOption> optionsToApplyOn;

		for (size_t i = 0; i < optionIds.size(); i++)
		{
			StoryOption option = FileParser::file.getOptionById(optionIds[i]);
			optionsToApplyOn.push_back(option);
		}

		m_index += 2; // Set the index to pass everything we read in, add two for empty space ' ' and the first char to be read next
		bool readingVariables = false;
		while (m_index < m_fileContent.size())
		{
			if (m_index == m_fileContent.size() - 1)
				System::errorMessage("# File flag not closed #", true);

			if (readingVariables)
			{
				std::vector<std::string> variableChange = handleVariableFlag();
				if (!variableChange.empty()) {
					fileLink.variableChanges.push_back(variableChange);
					for (size_t i = 0; i < optionsToApplyOn.size(); i++)
					{
						optionsToApplyOn[i].addVariableChange(variableChange);
					}
				}
			}

			if (!isEndFileLinkFlag())
			{
				if (m_fileContent[m_index] == ' ' && m_fileContent[m_index + 1] == '|')
				{
					readingVariables = true;
					m_index += 3;
				}

				else if (m_fileContent[m_index] == ' ')
				{
					m_index++;
				}

				else
				{
					fileLink.link += m_fileContent[m_index];
					for (size_t i = 0; i < optionsToApplyOn.size(); i++)
					{
						optionsToApplyOn[i].setLink(optionsToApplyOn[i].getLink() + m_fileContent[m_index]);
					}
					m_index++;
				}
			}

			else
			{
				m_readingFlagValueMap["filelink"] = false;
				break;
			}
		}

		m_fileLinks.push_back(fileLink);

		for (size_t i = 0; i < optionsToApplyOn.size(); i++)
		{
			FileParser::file.updateOption(optionsToApplyOn[i]);
		}
	}

	void FileParser::handleMessage()
	{
		FileParser::m_readingFlagValueMap["message"] = true;

		// Add 2 to index to pass the flag and start reading on the next element
		while (m_index < m_fileContent.size())
		{
			if (m_index == m_fileContent.size() - 1)
				System::errorMessage("-* Message flag not closed!-/*", true);

			else if (!isEndMessageFlag())
			{
				FileParser::file.setMessage(FileParser::file.getMessage() + m_fileContent[m_index]);
				m_customMessage += m_fileContent[m_index];
				m_index++;
			}

			else
			{
				FileParser::m_readingFlagValueMap["message"] = false;
				break;
			}
		}
	}

	bool FileParser::isValidSceneType()
	{
		return (m_sceneType == DEFAULT_SCENETYPE);
	}

	void FileParser::handleSceneType()
	{
		m_sceneType = m_fileContent[m_index];
		m_index++;

		if (!FileParser::isValidSceneType())
		{
			System::errorMessage("Not a valid SceneFlag", true);
		}
	}

	std::vector<std::string> FileParser::handleVariableFlag()
	{
		std::vector<std::string> variableBuildUp;
		static std::string variableChange;
		variableChange = "";

		if (isVariableFlag())
		{
			static std::string variable;
			variable = "";

			char logic_operator = ' ';
			bool isOperator = false;

			for (; m_index + 1 < m_fileContent.size(); m_index++)
			{
				for (const auto& keyValue : operatorsMap) {
					if (m_fileContent[m_index + 1] == keyValue.second)
					{
						isOperator = true;
					}
				}


				if (m_fileContent[m_index + 1] == ' ' || m_fileContent[m_index + 1] == '\n')
				{
					System::errorMessage("Unused variable", true);
				}

				else if (isOperator && m_fileContent[m_index + 2] != ' ' && m_fileContent[m_index + 2] != '\n')
				{
					logic_operator = m_fileContent[m_index + 1];
					m_index += 2;
					break;
				}

				else
				{
					variable.push_back(m_fileContent[m_index + 1]);
				}
			}

			std::string value = "";
			if (isOperator)
			{
				for (; m_index + 1 < m_fileContent.size(); m_index++)
				{
					if (m_fileContent[m_index] == ' ' || m_fileContent[m_index] == '\n')
						break;

					else
						value.push_back(m_fileContent[m_index]);
				}
			}

			std::string charStr(1, logic_operator);
			if (value == "RESPONSE") 
			{
				FileParser::m_responseIsString = true;
			}
			variableBuildUp = { variable, charStr, value };
		}

		return variableBuildUp;
	}

	void FileParser::defaultAllData()
	{

		FileParser::file.resetData();

		for (auto& keyValue : FileParser::m_readingFlagValueMap) {
			keyValue.second = false;
		}

		m_customMessage = "";

		m_textBlocks.clear();
		m_textBlocks.shrink_to_fit();

		m_fileLinks.clear();
		m_fileLinks.shrink_to_fit();

		m_index = 0;
		m_responseIsString = false;
	}

	void FileParser::loadText()
	{
		// clear all data of last scene
		FileParser::defaultAllData();

		// Construct the path for the next file
		m_goToFile = System::getWorkingDirectory() + "\\scenes\\" + m_filePath + ".txt";

		// Parse the content of the file into a string
		m_fileContent = System::readFileIntoString(m_goToFile.c_str());

		// Start reading the content of the file per char
		for (; m_index < m_fileContent.size(); m_index++)
		{
			if (FileParser::isStartStoryBlockFlag())
				FileParser::handleStoryBlock();

			else if (FileParser::isStartOptionBlockFlag())
				FileParser::handleOptionBlock();

			else if (FileParser::isStartFileLinkFlag())
				FileParser::handleFileLink();

			else if (FileParser::isStartMessageFlag())
				FileParser::handleMessage();

			else if (FileParser::isSceneTypeFlag())
				FileParser::handleSceneType();
		}
	}
}