#pragma once
#include <vector>
#include <string>

namespace EWF
{
	struct FileParser
	{
	private:
		static char m_sceneType;
		static std::string m_fileContent;
		static size_t m_index;
		static bool m_responseIsString;
		static std::vector<std::string> m_textBlocks;

		struct FileLink
		{
			std::string link;
			std::vector<std::string> boundChoices;
			std::vector<std::vector<std::string>> variableChanges;

			FileLink() = default;
		};

		static std::vector<FileLink> m_fileLinks;
		static std::vector<bool> m_readingFlagValue;
		static std::vector<std::string> m_variables;

		static std::string m_block;
		static std::string m_message;
		static std::string m_customMessage;

		static std::string m_goToFile;
		static std::string m_filePath;

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

		static bool isFlag(std::string, int startFlagToCheck);

		static bool isStartMessageFlag();
		static bool isEndMessageFlag();

		static bool isStartBlockFlag();
		static bool isEndBlockFlag();
		
		static bool isStartFileLinkFlag();
		static bool isEndFileLinkFlag();

		static bool isSceneTypeFlag();
		
		static bool isValidSceneType();
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