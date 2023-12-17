#pragma once
#include <vector>
#include <string>
#include <map>
#include <json.hpp>
#include "../File.h"

namespace EWF
{
	struct FileParser
	{
	private:
		struct FileLink
		{
			std::string link;
			std::vector<std::string> boundChoices;
			std::vector<std::vector<std::string>> variableChanges;

			FileLink() = default;
		};

		static File file;
		static nlohmann::json m_files;

		static char m_sceneType;
		static std::string m_fileContent;
		static size_t m_index;
		static bool m_responseIsString;
		static std::vector<std::string> m_textBlocks;
		static std::map<std::string, char> operatorsMap;
		static std::map<std::string, std::string> m_variablesMap;
		static std::map<std::string, bool> m_readingFlagValueMap;

		static std::vector<FileLink> m_fileLinks;

		static std::string m_block;
		static std::string m_message;
		static std::string m_customMessage;

		static std::string m_goToFile;
		static std::string m_filePath;

	public:
		FileParser();
		
		enum
		{
			NEW_VARIABLE = 0, LOGICAL_OPERATOR, VALUE
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

		static bool isFlag(std::string flag, std::vector<std::string> flagsToIgnore, bool disableFlagCheck);

		static bool isStartMessageFlag();
		static bool isEndMessageFlag();

		static bool isStartBlockFlag();
		static bool isEndBlockFlag();

		static bool isStartStoryBlockFlag();
		static bool isStartOptionBlockFlag();

		static void handleStoryBlock();
		static void handleOptionBlock();
		
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