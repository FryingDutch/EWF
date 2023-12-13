#include "System.h"
#include "FileParser.h"
#include <functional>
#include <algorithm>
#include "Player.h"

namespace EWF
{
	std::string FileParser::m_goToFile = "";

	std::vector<std::string> FileParser::m_textBlocks;

	std::vector<FileParser::FileLink> FileParser::m_fileLinks;
	std::vector<bool> FileParser::m_readingFlagValue{ false, false, false, false };
	std::vector<std::string> FileParser::m_variables{ "HP", "ATK", "DEF", "AGE", "NAME", "MAXHP"};

	char FileParser::m_sceneType;
	std::string FileParser::m_filePath = "first-scene";
	std::string FileParser::m_fileContent;

	std::string FileParser::m_block = "";
	std::string FileParser::m_message = "Make a choice: ";
	std::string FileParser::m_customMessage = "";

	size_t FileParser::m_index = 0;
	bool FileParser::m_responseIsString{ false };

	const uint32_t FileParser::NUMOFOPERATORS = 4;
	const char FileParser::operators[NUMOFOPERATORS] = { '+', '-', '=', '$' };

	bool FileParser::isVariableFlag()
	{
		if (m_index + 1 < m_fileContent.size() 
			&& (m_fileContent[m_index] == operators[VARIABLE] 
				&& m_fileContent[m_index + 1] != ' ' 
				&& m_fileContent[m_index + 1] != '\n' 
				&& !std::isdigit(m_fileContent[m_index + 1]))
			){
			return true;
		}

		return false;
	}

	bool FileParser::isFlag(std::string flag, int startFlagToCheck = -1)
	{
		uint32_t flagSize = flag.size();
		std::string currentBlock = m_fileContent.substr(m_index, flag.size());

		bool enoughCharsLeftToRead = ((m_index + flagSize) < m_fileContent.size());
		std::vector flagsToCheck = m_readingFlagValue;
		bool noStartFlagToCheck = startFlagToCheck == -1;

		if (!noStartFlagToCheck) {
			flagsToCheck.erase(flagsToCheck.begin() + startFlagToCheck);
		}

		bool notReadingOtherFlags = std::all_of(flagsToCheck.begin(), flagsToCheck.end(), std::logical_not<bool>());
		bool currentBlockEqualsFlag = (currentBlock == flag);
		bool readingCurrentFlag = (noStartFlagToCheck) ? false : m_readingFlagValue[startFlagToCheck];

		if (enoughCharsLeftToRead && notReadingOtherFlags && currentBlockEqualsFlag) {
			m_index += flagSize;
			return true;
		}

		return false;
	}

	bool FileParser::isStartMessageFlag()
	{
		return FileParser::isFlag("-*");
	}

	bool FileParser::isEndMessageFlag()
	{
		return FileParser::isFlag("-/*", FLAG::MESSAGE);
	}

	bool FileParser::isStartBlockFlag()
	{
		return FileParser::isFlag("<>");
	}

	bool FileParser::isEndBlockFlag()
	{
		return FileParser::isFlag("</>", FLAG::BLOCK);
	}

	bool FileParser::isStartFileLinkFlag()
	{
		return FileParser::isFlag("#");
	}

	bool FileParser::isEndFileLinkFlag()
	{
		return FileParser::isFlag("#!", FLAG::FILELINK);
	}

	bool FileParser::isSceneTypeFlag()
	{
		return FileParser::isFlag("~");
	}

	void FileParser::handleBlock()
	{
		m_readingFlagValue[FLAG::BLOCK] = true;
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

		m_readingFlagValue[FLAG::BLOCK] = false;
		m_textBlocks.push_back(m_block);
		m_block = "";
	}

	void FileParser::handleFileLink()
	{
		m_readingFlagValue[FLAG::FILELINK] = true;

		// Create a fileLink to be filled with both the choices as well as the link and variable changes.
		FileLink fileLink;
		static const char MULTI_CHOICE_OPERATOR = ',';

		// Until the maximum allowed of elements, keep looping, adding 2 to position to get the digit we need.
		for (m_index += 0; m_index <= m_fileContent.size(); m_index += 2)
		{
			// if the second position of the element [#1][,2][,3] is a digit
			if ((m_index + 2) < m_fileContent.size() && std::isdigit(m_fileContent[m_index]))
			{
				std::string choice{ m_fileContent[m_index] };
				fileLink.boundChoices.push_back(choice);

				// If the next element fits the standard, do another cycle
				if (std::isdigit(m_fileContent[m_index + 2]) && m_fileContent[m_index + 1] == MULTI_CHOICE_OPERATOR)
					continue;

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

		m_index += 2; // Set the index to pass everything we read in, add two for empty space ' ' and the first char to be read next
		bool readingVariables = false;
		while (m_index < m_fileContent.size())
		{
			if (m_index == m_fileContent.size() - 1)
				System::errorMessage("# File flag not closed #", true);

			if (readingVariables)
			{
				std::vector<std::string> variableChange = handleVariableFlag();
				if (!variableChange.empty())
					fileLink.variableChanges.push_back(variableChange);
			}

			if (!isEndFileLinkFlag())
			{
				if (m_fileContent[m_index] == ' ' && m_fileContent[m_index + 1] == '|')
				{
					readingVariables = true;
					m_index += 3;
				}

				else if (m_fileContent[m_index] == ' ')
					m_index++;

				else
				{
					fileLink.link += m_fileContent[m_index];
					m_index++;
				}
			}

			else
			{
				m_readingFlagValue[FLAG::FILELINK] = false;
				break;
			}
		}

		m_fileLinks.push_back(fileLink);
	}

	void FileParser::handleMessage()
	{
		m_readingFlagValue[FLAG::MESSAGE] = true;

		// Add 2 to index to pass the flag and start reading on the next element
		while (m_index < m_fileContent.size())
		{
			if (m_index == m_fileContent.size() - 1)
				System::errorMessage("-* Message flag not closed!-/*", true);

			else if (!isEndMessageFlag())
			{
				m_customMessage += m_fileContent[m_index];
				m_index++;
			}

			else
			{
				m_readingFlagValue[FLAG::MESSAGE] = false;
				break;
			}
		}
	}

	bool FileParser::isValidSceneType()
	{
		return (m_sceneType == INTRO_SCENETYPE || m_sceneType == DEFAULT_SCENETYPE);
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
				for (size_t i = 0; i < NUMOFOPERATORS; i++)
				{
					if (m_fileContent[m_index + 1] == operators[i])
						isOperator = true;
				}

				if (m_fileContent[m_index + 1] == ' ' || m_fileContent[m_index + 1] == '\n')
					System::errorMessage("Unused variable", true);

				else if (isOperator && m_fileContent[m_index + 2] != ' ' && m_fileContent[m_index + 2] != '\n')
				{
					logic_operator = m_fileContent[m_index + 1];
					m_index += 2;
					break;
				}

				else
					variable.push_back(m_fileContent[m_index + 1]);
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
				FileParser::m_responseIsString = true;

			variableBuildUp = { variable, charStr, value };
		}

		return variableBuildUp;
	}

	void FileParser::defaultAllData()
	{
		for (size_t i = 0; i < m_readingFlagValue.size(); i++)
			m_readingFlagValue[i] = false;

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
		defaultAllData();

		// Construct the path for the next file
		m_goToFile = System::getWorkingDirectory() + "\\scenes\\" + m_filePath + ".txt";

		// Parse the content of the file into a string
		m_fileContent = System::readFileIntoString(m_goToFile.c_str());

		// Start reading the content of the file per char
		for (; m_index < m_fileContent.size(); m_index++)
		{
			if (isStartBlockFlag())
				handleBlock();

			else if (isStartFileLinkFlag())
				handleFileLink();

			else if (isStartMessageFlag())
				handleMessage();

			else if (isSceneTypeFlag())
				handleSceneType();
		}
	}
}