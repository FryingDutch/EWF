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

		static std::string block;
		static std::string message;

		static std::string goToFile;

		static std::string filePath;

	public:
		enum : char
		{
			INTRO = 'I', DEFAULT = 'D'
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