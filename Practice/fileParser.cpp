#include "System.h"
#include "FileParser.h"
#include <functional>
#include <algorithm>
#include "Player.h"

namespace EWF
{
	std::string FileParser::goToFile = "";

	std::vector<std::string> FileParser::textBlocks;

	std::vector<FileParser::FileLink> FileParser::fileLinks;
	std::vector<bool> FileParser::readingFlagValue{ false, false, false, false };
	std::vector<std::string> FileParser::variables{ "HP", "ATK", "DEF", "AGE", "NAME", "MAXHP"};

	char FileParser::sceneType;
	std::string FileParser::filePath = "intro";
	std::string FileParser::fileContent;

	std::string FileParser::block = "";
	std::string FileParser::message = "Make a choice: ";
	std::string FileParser::customMessage = "";

	size_t FileParser::index = 0;
	bool FileParser::responseIsString{ false };

	const uint32_t FileParser::NUMOFOPERATORS = 4;
	const char FileParser::operators[NUMOFOPERATORS] = { '+', '-', '=', '$' };

	bool FileParser::isVariableFlag()
	{
		if (index + 1 < fileContent.size() 
			&& (fileContent[index] == operators[VARIABLE] 
				&& fileContent[index + 1] != ' ' 
				&& fileContent[index + 1] != '\n' 
				&& !std::isdigit(fileContent[index + 1]))
			){
			return true;
		}

		return false;
	}

	bool FileParser::isStartMessageFlag()
	{
		if (index + 1 < fileContent.size()
			&& std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>())
			&& fileContent[index] == '-'
			&& fileContent[index + 1] == '*')
		{
			index += 2;
			return true;
		}

		return false;
	}

	bool FileParser::isEndMessageFlag()
	{
		if (index + 2 < fileContent.size()
			&& readingFlagValue[FLAG::MESSAGE]
			&& fileContent[index] == '-'
			&& fileContent[index + 1] == '/'
			&& fileContent[index + 2] == '*')
		{
			index += 3;
			return true;
		}

		return false;
	}

	bool FileParser::isStartBlockFlag()
	{
		static const int FLAG_SIZE = std::string("<>").size();
		if (index + 1 < fileContent.size()
			&& std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>())
			&& fileContent[index] == '<'
			&& fileContent[index + 1] == '>')
		{
			index += FLAG_SIZE;
			return true;
		}

		return false;
	}

	bool FileParser::isEndBlockFlag()
	{
		static const int FLAG_SIZE = std::string("</>").size();
		if (readingFlagValue[FLAG::BLOCK]
			&& index + 2 < fileContent.size()
			&& fileContent[index] == '<'
			&& fileContent[index + 1] == '/'
			&& fileContent[index + 2] == '>')
		{
			index += FLAG_SIZE;
			return true;
		}

		return false;
	}

	bool FileParser::isStartFileLinkFlag()
	{
		if (std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>()) 
			&& fileContent[index] == '#' 
			&& (index + 1) < fileContent.size() 
			&& std::isdigit(fileContent[index + 1]))
		{
			return true;
		}

		return false;
	}

	bool FileParser::isEndFileLinkFlag()
	{
		static const int FLAG_SIZE = std::string("#!").size();
		if (readingFlagValue[FLAG::FILELINK]
			&& (index + 2) < fileContent.size()
			&& !readingFlagValue[FLAG::BLOCK]
			&& fileContent[index] == ' '
			&& fileContent[index + 1] == '#'
			&& fileContent[index + 2] == '!')
		{
			index += FLAG_SIZE;
			return true;
		}

		return false;
	}

	bool FileParser::isSceneTypeFlag()
	{
		if (std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>()) 
			&& fileContent[index] == '~' 
			&& index < (fileContent.size() - 1) 
			&& !std::isdigit(fileContent[index] == '~'))
		{
			index += 1;
			return true;
		}
		return false;
	}

	void FileParser::handleBlock()
	{
		readingFlagValue[FLAG::BLOCK] = true;
		while (!isEndBlockFlag())
		{
			// if at end of file and block is still being read
			if ((index + 1) == fileContent.size())
			{
				System::errorMessage("Block not closed </>", true);
				return;
			}

			block += fileContent[index];
			index++;
		}

		readingFlagValue[FLAG::BLOCK] = false;
		textBlocks.push_back(block);
		block = "";
	}

	void FileParser::handleFileLink()
	{
		readingFlagValue[FLAG::FILELINK] = true;

		// Create a fileLink to be filled with both the choices as well as the link and variable changes.
		FileLink fileLink;
		static const char MULTI_CHOICE_OPERATOR = ',';

		// Until the maximum allowed of elements, keep looping, adding 2 to position to get the digit we need.
		for (index += 1; index <= fileContent.size(); index += 2)
		{
			// if the second position of the element [#1][,2][,3] is a digit
			if ((index + 2) < fileContent.size() && std::isdigit(fileContent[index]))
			{
				std::string choice{ fileContent[index] };
				fileLink.boundChoices.push_back(choice);

				// If the next element fits the standard, do another cycle
				if (std::isdigit(fileContent[index + 2]) && fileContent[index + 1] == MULTI_CHOICE_OPERATOR)
					continue;

				// Else if there is a [,non_digit], end program. 
				else if (fileContent[index + 1] == MULTI_CHOICE_OPERATOR && !std::isdigit(fileContent[index + 2]))
				{
					static std::string msg = ("Mullti choice operator " + MULTI_CHOICE_OPERATOR) + std::string(" not followed by digit");
					System::errorMessage(msg.c_str(), true);
				}

				// else break the loop
				else
					break;
			}
		}

		index += 2; // Set the index to pass everything we read in, add two for empty space ' ' and the first char to be read next
		bool readingVariables = false;
		while (index < fileContent.size())
		{
			if (index == fileContent.size() - 1)
				System::errorMessage("# File flag not closed #", true);

			if (readingVariables)
			{
				std::vector<std::string> variableChange = handleVariableFlag();
				if (!variableChange.empty())
					fileLink.variableChanges.push_back(variableChange);
			}

			if (!isEndFileLinkFlag())
			{
				if (fileContent[index] == ' ' && fileContent[index + 1] == '|')
				{
					readingVariables = true;
					index += 3;
				}

				else if (fileContent[index] == ' ')
					index++;

				else
				{
					fileLink.link += fileContent[index];
					index++;
				}
			}

			else
			{
				readingFlagValue[FLAG::FILELINK] = false;
				break;
			}
		}

		fileLinks.push_back(fileLink);
	}

	void FileParser::handleMessage()
	{
		readingFlagValue[FLAG::MESSAGE] = true;

		// Add 2 to index to pass the flag and start reading on the next element
		while (index < fileContent.size())
		{
			if (index == fileContent.size() - 1)
				System::errorMessage("-* Message flag not closed!-/*", true);

			else if (!isEndMessageFlag())
			{
				customMessage += fileContent[index];
				index++;
			}

			else
			{
				readingFlagValue[FLAG::MESSAGE] = false;
				break;
			}
		}
	}

	void FileParser::handleSceneType()
	{
		// Looks redundant, but the switch is the error catcher in this case, defaulting always to a hard error.
		switch (fileContent[index])
		{
		case INTRO_SCENETYPE:
			sceneType = INTRO_SCENETYPE;
			index++;
			break;

		case DEFAULT_SCENETYPE:
			sceneType = DEFAULT_SCENETYPE;
			index++;
			break;

		default:
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

			for (; index + 1 < fileContent.size(); index++)
			{
				for (size_t i = 0; i < NUMOFOPERATORS; i++)
				{
					if (fileContent[index + 1] == operators[i])
						isOperator = true;
				}

				if (fileContent[index + 1] == ' ' || fileContent[index + 1] == '\n')
					System::errorMessage("Unused variable", true);

				else if (isOperator && fileContent[index + 2] != ' ' && fileContent[index + 2] != '\n')
				{
					logic_operator = fileContent[index + 1];
					index += 2;
					break;
				}

				else
					variable.push_back(fileContent[index + 1]);
			}

			std::string value = "";
			if (isOperator)
			{
				for (; index + 1 < fileContent.size(); index++)
				{
					if (fileContent[index] == ' ' || fileContent[index] == '\n')
						break;

					else
						value.push_back(fileContent[index]);
				}
			}
			std::string charStr(1, logic_operator);
			if (value == "RESPONSE")
				FileParser::responseIsString = true;

			variableBuildUp = { variable, charStr, value };
		}

		return variableBuildUp;
	}

	void FileParser::defaultAllData()
	{
		for (size_t i = 0; i < readingFlagValue.size(); i++)
			readingFlagValue[i] = false;

		customMessage = "";

		textBlocks.clear();
		textBlocks.shrink_to_fit();

		fileLinks.clear();
		fileLinks.shrink_to_fit();

		index = 0;
		responseIsString = false;
	}

	void FileParser::loadText()
	{
		// clear all data of last scene
		defaultAllData();

		// Construct the path for the next file
		goToFile = System::getWorkingDirectory() + "\\scenes\\" + filePath + ".txt";

		// Parse the content of the file into a string
		fileContent = System::readFileIntoString(goToFile.c_str());

		// Start reading the content of the file per char
		for (; index < fileContent.size(); index++)
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