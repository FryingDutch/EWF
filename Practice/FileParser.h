#pragma once
#include <vector>
#include <string>

namespace EWF
{
	struct FileParser
	{
	private:
		static bool read;
		static char sceneType;
		static std::string fileContent;

		static std::vector<std::string> textBlocks;
		static std::vector<std::string> fileLinks;
		static std::vector<bool> readingFlagValue;

		static std::string block;
		static std::string message;

		static std::string goToFile;
		static std::string filePath;

	public:
		enum : char
		{
			INTRO_SCENETYPE = 'I', DEFAULT_SCENETYPE = 'D'
		};

		enum
		{
			BLOCK_FLAG, FILELINK_FLAG, SCENETYPE_FLAG, MESSAGE_FLAG
		};

	public:
		static bool isStartMessageFlag(size_t _index);
		static bool isEndMessageFlag(size_t _index);

		static bool isStartBlockFlag(size_t _index);
		static bool isEndBlockFlag(size_t _index);
		
		static bool isFileLinkFlag(size_t _index);
		static bool isSceneTypeFlag(size_t _index);
		
		static void handleFlags(size_t& _index);

		static void loadText();

		friend struct SceneManager;
	};
}