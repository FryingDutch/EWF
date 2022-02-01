#include "System.h"
#include "FileParser.h"

namespace EWF
{
	bool FileParser::read = false;

	std::string FileParser::goToFile = "";

	std::vector<std::string> FileParser::textBlocks;
	std::vector<std::string> FileParser::fileLinks;
	std::vector<bool> FileParser::readingFlagValue{ false, false, false, false };

	char FileParser::sceneType;
	std::string FileParser::filePath = "intro";
	std::string FileParser::fileContent;

	std::string FileParser::block = "";
	std::string FileParser::message;

	bool FileParser::isStartMessageFlag(size_t _index)
	{
		if (_index + 1 < fileContent.size())
		{
			if (!read && fileContent[_index] == '-' && (_index + 1) < fileContent.size() && fileContent[_index + 1] == '*')
				return true;

			return false;
		}
		return false;
	}

	bool FileParser::isEndMessageFlag(size_t _index)
	{
		if (_index + 2 < fileContent.size())
		{
			if (!read && fileContent[_index] == '-' && (_index + 2) < fileContent.size() && fileContent[_index + 1] == '/' && fileContent[_index + 2] == '*')
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

	bool FileParser::isFileLinkFlag(size_t _index)
	{
		if (!read && fileContent[_index] == '#' && (_index + 1) < fileContent.size() && std::isdigit(fileContent[_index + 1]))
			return true;

		return false;
	}

	bool FileParser::isSceneTypeFlag(size_t _index)
	{
		if (!read && fileContent[_index] == '~' && (_index + 1) < fileContent.size())
			return true;

		return false;
	}

	void FileParser::handleFlags(size_t& _index)
	{
		if (read && (_index + 1) == fileContent.size())
		{
			System::errorMessage("Block not closed </>", true);
			return;
		}

		if (fileContent[_index] == '<')
		{
			if (isStartBlockFlag(_index))
			{
				read = true;
				_index++;
			}

			else if (isEndBlockFlag(_index))
			{
				read = false;
				_index += 2;
				block.pop_back();
				textBlocks.push_back(block);
				block = "";
			}
		}

		else if (isFileLinkFlag(_index))
		{
			std::string link;
			for (_index = _index + 3; _index + 1 <= fileContent.size(); _index++)
			{
				if (fileContent[_index] != '\n' && fileContent[_index] != '#' && fileContent[_index] != ' ')
					link += fileContent[_index];

				else
					break;
			}
			fileLinks.push_back(link);
		}

		else if (isStartMessageFlag(_index))
		{
			_index += 2;
			while (_index < fileContent.size())
			{
				if (!isEndMessageFlag(_index))
				{
					message += fileContent[_index];
					_index++;
				}

				else
					break;
			}
		}

		else if (isSceneTypeFlag(_index))
		{
			switch (fileContent[_index + 1])
			{
			case INTRO:
				sceneType = INTRO;
				break;

			case DEFAULT:
				sceneType = DEFAULT;
				break;

			default:
				System::errorMessage("Not a valid SceneFlag", true);
				return;
			}
		}
	}

	void FileParser::loadText()
	{
		textBlocks.clear();
		textBlocks.shrink_to_fit();

		goToFile = System::getWorkingDirectory() + "\\scenes\\" + filePath + ".txt";

		fileLinks.clear();
		fileLinks.shrink_to_fit();

		fileContent = System::readFile(goToFile.c_str());

		read = false;

		for (size_t i = 0; i < fileContent.size(); i++)
		{
			if (read)
				block += fileContent[i];

			handleFlags(i);
		}
	}
}