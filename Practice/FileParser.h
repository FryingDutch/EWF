#pragma once
#include <vector>
#include <string>

namespace EWF
{
	struct FileParser
	{
	private:
		static char sceneType;
		static std::string fileContent;

		static std::vector<std::string> textBlocks;

		struct FileLink
		{
			std::string link;
			std::vector<uint32_t> boundChoices;

			FileLink() = default;
		};

		static std::vector<FileLink> fileLinks;
		static std::vector<bool> readingFlagValue;
		static std::vector<std::string> variables;

		static std::string block;
		static std::string message;
		static std::string customMessage;

		static std::string goToFile;
		static std::string filePath;

		static const uint32_t NUMOFOPERATORS;
		static const char operators[];

	public:
		enum
		{
			HP = 0, ATK, DEF, AGE, NAME
		};
		enum
		{
			PLUS = 0, MINUS, EQUALS, VARIABLE
		};

		enum : char
		{
			INTRO_SCENETYPE = 'I', DEFAULT_SCENETYPE = 'D'
		};

		enum FLAG
		{
			BLOCK = 0, FILELINK, SCENETYPE, MESSAGE 
		};

	public:
		static void defaultAllData();
		static bool isVariableFlag(size_t _index);
		static bool isOperator(size_t _index);

		static bool isStartMessageFlag(size_t _index);
		static bool isEndMessageFlag(size_t _index);

		static bool isStartBlockFlag(size_t _index);
		static bool isEndBlockFlag(size_t _index);
		
		static bool isStartFileLinkFlag(size_t _index);
		static bool isEndFileLinkFlag(size_t _index);

		static bool isSceneTypeFlag(size_t _index);
		
		static void handleVariableFlag(size_t& _index);
		static void handleFlags(size_t& _index);

		static void loadText();

		friend struct SceneManager;
	};
}