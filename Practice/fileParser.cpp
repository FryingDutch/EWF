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
	std::vector<std::string> FileParser::variables{ "HP", "ATK", "DEF", "AGE", "NAME" };

	char FileParser::sceneType;
	std::string FileParser::filePath = "intro";
	std::string FileParser::fileContent;

	std::string FileParser::block = "";
	std::string FileParser::message = "Make a choice: ";
	std::string FileParser::customMessage = "";

	const uint32_t FileParser::NUMOFOPERATORS = 4;
	const char FileParser::operators[NUMOFOPERATORS] = { '+', '-', '=', '$' };

	bool FileParser::isVariableFlag(size_t _index)
	{
		if (_index + 1 < fileContent.size())
			return (fileContent[_index] == operators[VARIABLE] && fileContent[_index + 1] != ' ' && fileContent[_index + 1] != '\n' && !std::isdigit(fileContent[_index + 1]));

		return false;
	}

	bool FileParser::isStartMessageFlag(size_t _index)
	{
		if (_index + 1 < fileContent.size())
			return (std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>()) && fileContent[_index] == '-' && fileContent[_index + 1] == '*');

		return false;
	}

	bool FileParser::isEndMessageFlag(size_t _index)
	{
		if (_index + 2 < fileContent.size())
			return (readingFlagValue[FLAG::MESSAGE] && fileContent[_index] == '-' && fileContent[_index + 1] == '/' && fileContent[_index + 2] == '*');

		return false;
	}

	bool FileParser::isStartBlockFlag(size_t _index)
	{
		if (_index + 1 < fileContent.size() && std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>()))
			return (fileContent[_index + 1] == '>');

		return false;
	}

	bool FileParser::isEndBlockFlag(size_t _index)
	{
		if (readingFlagValue[FLAG::BLOCK] && _index + 2 < fileContent.size())
			return (fileContent[_index + 1] == '/' && fileContent[_index + 2] == '>');

		return false;
	}

	bool FileParser::isStartFileLinkFlag(size_t _index)
	{
		return (std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>()) && fileContent[_index] == '#' && (_index + 1) < fileContent.size() && std::isdigit(fileContent[_index + 1]));
	}

	bool FileParser::isEndFileLinkFlag(size_t _index)
	{
		if (readingFlagValue[FLAG::FILELINK])
			return ((_index + 2) < fileContent.size() && !readingFlagValue[FLAG::BLOCK] && fileContent[_index] == ' ' && fileContent[_index + 1] == '#' && fileContent[_index + 2] == '!');

		return false;
	}

	bool FileParser::isSceneTypeFlag(size_t _index)
	{
		return (std::all_of(readingFlagValue.begin(), readingFlagValue.end(), std::logical_not<bool>()) && fileContent[_index] == '~' && _index < (fileContent.size() - 1) && !std::isdigit(fileContent[_index] == '~'));
	}

	std::vector<std::string> FileParser::handleVariableFlag(size_t& _index)
	{
		std::vector<std::string> variableBuildUp;
		static std::string variableChange;
		variableChange = "";

		if (isVariableFlag(_index))
		{
			if (fileContent[_index + 1] != ' ' && fileContent[_index + 1] != '\n' && !std::isdigit(fileContent[_index + 1]))
			{
				static std::string variable;
				variable = "";

				char logic_operator = ' ';
				bool isOperator = false;

				for (; _index + 1 < fileContent.size(); _index++)
				{
					for (size_t i = 0; i < NUMOFOPERATORS; i++)
					{
						if (fileContent[_index + 1] == operators[i])
							isOperator = true;
					}

					if (fileContent[_index + 1] == ' ' || fileContent[_index + 1] == '\n')
						System::errorMessage("Unused variable", true);

					else if (isOperator && fileContent[_index + 2] != ' ' && fileContent[_index + 2] != '\n')
					{
						logic_operator = fileContent[_index + 1];
						_index += 2;
						break;
					}

					else
						variable.push_back(fileContent[_index + 1]);
				}

				std::string value = "";
				if (isOperator)
				{
					for (; _index + 1 < fileContent.size(); _index++)
					{
						if (fileContent[_index] == ' ' || fileContent[_index] == '\n')
							break;

						else
							value.push_back(fileContent[_index]);
					}
				}
				std::string charStr(1, logic_operator);
				variableBuildUp = { variable, charStr, value };
			}
		}

		return variableBuildUp;
	}

	void FileParser::handleFlags(size_t& _index)
	{
		// if at end of file and block is still being read
		if (readingFlagValue[FLAG::BLOCK] && (_index + 1) == fileContent.size())
		{
			System::errorMessage("Block not closed </>", true);
			return;
		}

		else if (readingFlagValue[FLAG::BLOCK])
			block += fileContent[_index];

		if (fileContent[_index] == '<')
		{
			if (isStartBlockFlag(_index))
			{
				readingFlagValue[FLAG::BLOCK] = true;
				_index++;
			}

			else if (isEndBlockFlag(_index))
			{
				readingFlagValue[FLAG::BLOCK] = false;
				_index += 2; // No need to read the next 2 itterations as we already know its the endBlockFlag
				if (block.size() > 0)
					block.pop_back(); // We read the < in because we are still at read mode coming in. So pop one back

				textBlocks.push_back(block);
				block = "";
			}
		}

		else if (isStartFileLinkFlag(_index))
		{
			readingFlagValue[FLAG::FILELINK] = true;

			// Create a fileLink to be filled with both the choices as wel as the link.
			FileLink fileLink;
			size_t _position; // Starting position, relative to current index
			size_t maxSizeNeeded = 19; // Size is 2 for each element entered [#1][,2][,3], with a max of 9. 1 Added for starting position.  			

			// Until the maximum allowed of elements, keep looping, adding 2 to position to get the digit we need.
			for (_position = 1; _position <= maxSizeNeeded; _position += 2)
			{
				// if the second position of the element [#1][,2][,3] is a digit
				if ((_index + _position + 2) < fileContent.size() && std::isdigit(fileContent[_index + _position]))
				{
					fileLink.boundChoices.push_back(fileContent[(_index + _position)] - 48); // Get the digit, but since its read as a char, retract 48 to get its integral value.

					// If the next element fits the standard, do another cycle
					if (std::isdigit(fileContent[_index + _position + 2]) && fileContent[_index + _position + 1] == ',')
						continue;

					// Else if there is a [,non_digit], end program. 
					else if (fileContent[_index + _position + 1] == ',' && !std::isdigit(fileContent[_index + _position + 2]))
						System::errorMessage("Mullti choice operator , not followed by digit", true);

					// else break the loop
					else
						break;
				}
			}

			_index += _position + 2; // Set the index to pass everything we read in, add two for empty space ' ' and the first char to be read next
			bool readingVariables = false;
			while (_index < fileContent.size())
			{
				if (_index == fileContent.size() - 1)
					System::errorMessage("# File flag not closed #", true);

				if (readingVariables)
				{
					std::vector<std::string> variableChange = handleVariableFlag(_index);
					if (!variableChange.empty())
						fileLink.variableChanges.push_back(variableChange);
				}

				if (!isEndFileLinkFlag(_index))
				{
					if (fileContent[_index] == ' ' && fileContent[_index + 1] == '|')
					{
						readingVariables = true;
						_index += 3;
					}

					else if (fileContent[_index] == ' ')
						_index++;

					else
					{
						fileLink.link += fileContent[_index];
						_index++;
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

		else if (isStartMessageFlag(_index))
		{
			readingFlagValue[FLAG::MESSAGE] = true;

			// Add 2 to index to pass the flag and start reading on the next element
			_index += 2;
			while (_index < fileContent.size())
			{
				if (_index == fileContent.size() - 1)
					System::errorMessage("-* Message flag not closed!-/*", true);

				else if (!isEndMessageFlag(_index))
				{
					customMessage += fileContent[_index];
					_index++;
				}

				else
				{
					readingFlagValue[FLAG::MESSAGE] = false;
					break;
				}
			}
		}

		else if (isSceneTypeFlag(_index))
		{
			// Looks redundant, but the switch is the error catcher in this case, defaulting always to a hard error.
			switch (fileContent[_index + 1])
			{
			case INTRO_SCENETYPE:
				sceneType = INTRO_SCENETYPE;
				break;

			case DEFAULT_SCENETYPE:
				sceneType = DEFAULT_SCENETYPE;
				break;

			default:
				System::errorMessage("Not a valid SceneFlag", true);
			}
		}
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
	}

	void FileParser::loadText()
	{
		// clear all data of last scene
		defaultAllData();

		// Construct the path for the next file
		goToFile = System::getWorkingDirectory() + "\\scenes\\" + filePath + ".txt";

		// Parse the content of the file into a string
		fileContent = System::readFile(goToFile.c_str());

		// Start reading the content of the file per char
		for (size_t i = 0; i < fileContent.size(); i++)
		{
			handleFlags(i);
		}
	}
}