#include "System.h"
#include "FileParser.h"

namespace EWF
{
	std::string FileParser::goToFile = "";

	std::vector<std::string> FileParser::textBlocks;

	std::vector<FileParser::FileLink> FileParser::fileLinks;
	std::vector<bool> FileParser::readingFlagValue{ false, false, false, false };

	char FileParser::sceneType;
	std::string FileParser::filePath = "intro";
	std::string FileParser::fileContent;

	std::string FileParser::block = "";
	std::string FileParser::message = "Make a choice: ";
	std::string FileParser::customMessage = "";

	bool FileParser::isStartMessageFlag(size_t _index)
	{
		if (_index + 1 < fileContent.size())
		{
			if (!readingFlagValue[FLAG::BLOCK] && fileContent[_index] == '-' && (_index + 1) < fileContent.size() && fileContent[_index + 1] == '*')
				return true;

			return false;
		}
		return false;
	}

	bool FileParser::isEndMessageFlag(size_t _index)
	{
		if (_index + 2 < fileContent.size())
		{
			if (!readingFlagValue[FLAG::BLOCK] && fileContent[_index] == '-' && (_index + 2) < fileContent.size() && fileContent[_index + 1] == '/' && fileContent[_index + 2] == '*')
				return true;

			return false;
		}
		return false;
	}

	bool FileParser::isStartBlockFlag(size_t _index)
	{
		if (_index + 1 < fileContent.size())
		{
			if (fileContent[_index + 1] == '>')
				return true;
		}
		return false;
	}

	bool FileParser::isEndBlockFlag(size_t _index)
	{
		if (_index + 2 < fileContent.size())
		{
			if (fileContent[_index + 1] == '/' && fileContent[_index + 2] == '>')
				return true;
		}
		return false;
	}

	bool FileParser::isStartFileLinkFlag(size_t _index)
	{
		if (!readingFlagValue[FLAG::BLOCK] && fileContent[_index] == '#' && (_index + 1) < fileContent.size() && std::isdigit(fileContent[_index + 1]))
			return true;

		return false;
	}

	bool FileParser::isEndFileLinkFlag(size_t _index)
	{
		if (!readingFlagValue[FLAG::BLOCK] && fileContent[_index] == '#' && (_index + 1) < fileContent.size() && fileContent[_index + 1] == '!')
			return true;

		return false;
	}

	bool FileParser::isSceneTypeFlag(size_t _index)
	{
		if (!readingFlagValue[FLAG::BLOCK] && fileContent[_index] == '~' && _index < (fileContent.size() - 1) )
			return true;

		return false;
	}

	void FileParser::handleFlags(size_t& _index)
	{
		// if at end of file and block is still being read
		if (readingFlagValue[FLAG::BLOCK] && (_index + 1) == fileContent.size())
		{
			System::errorMessage("Block not closed </>", true);
			return;
		}

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
				block.pop_back(); // We read the < in because we are still at read mode coming in. So pop one back
				textBlocks.push_back(block);
				block = "";
			}
		}

		else if (isStartFileLinkFlag(_index))
		{
			FileLink fileLink;
			size_t maxSizeNeeded = 19;
			size_t _position;

			for (_position = 1; _position <= maxSizeNeeded; _position += 2)
			{
				if ((_index + _position + 1) < fileContent.size() && std::isdigit(fileContent[_index + _position]))
				{
					fileLink.boundChoices.push_back(fileContent[(_index + _position)] - 48);

					if (fileContent[_index + _position + 1] == ',')
					{
						continue;
					}

					else
						break;
				}
			}
			
			_index += _position + 2;

			while (_index < fileContent.size())
			{
				if (!isEndFileLinkFlag(_index))
				{
					fileLink.link += fileContent[_index];
					_index++;
				}

				else if (_index == fileContent.size() - 1)
					System::errorMessage("# File flag not closed#");

				else
					break;
			}			
			
			fileLinks.push_back(fileLink);
		}

		else if (isStartMessageFlag(_index))
		{
			customMessage = "";
			// Add 2 to index to pass the flag and start reading on the next element
			_index += 2;
			while (_index < fileContent.size())
			{
				if (!isEndMessageFlag(_index))
				{
					customMessage += fileContent[_index];
					_index++;
				}

				else if(_index == fileContent.size() - 1)
					System::errorMessage("Message flag not closed!-/*");

				else
					break;
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
		readingFlagValue[0] = false;

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
			if (readingFlagValue[FLAG::BLOCK])
				block += fileContent[i];

			handleFlags(i);
		}
	}
}