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
		static size_t index;
		static bool responseIsString;
		static std::vector<std::string> textBlocks;

		struct FileLink
		{
			std::string link;
			std::vector<std::string> boundChoices;
			std::vector<std::vector<std::string>> variableChanges;

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
			NEW_VARIABLE = 0, LOGICAL_OPERATOR, VALUE
		};

		enum
		{
			HP = 0, ATK, DEF, AGE, NAME, MAXHP
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
		static bool isVariableFlag();

		static bool isStartMessageFlag();
		static bool isEndMessageFlag();

		static bool isStartBlockFlag();
		static bool isEndBlockFlag();
		
		static bool isStartFileLinkFlag();
		static bool isEndFileLinkFlag();

		static bool isSceneTypeFlag();
		
		static void handleBlock();
		static void handleFileLink();
		static void handleMessage();
		static void handleSceneType();
		static std::vector<std::string> handleVariableFlag();
		static void handleFlags();

		static void loadText();

		friend struct SceneManager;
	};
}