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
		static nlohmann::json m_files;

		static std::string m_fileContent;
		static size_t m_index;
		static bool m_responseIsString;
		static std::map<std::string, char> operatorsMap;
		static std::map<std::string, std::string> m_variablesMap;
		static std::map<std::string, bool> m_readingFlagValueMap;

		static std::string m_message;
		static std::string m_goToFile;
		static std::string m_filePath;

	public:
		FileParser();
		static File file;
		
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

		static bool isStartStoryBlockFlag();
		static bool isStartOptionBlockFlag();
		static bool isEndBlockFlag();

		static void handleStoryBlock();
		static void handleOptionBlock();
		
		static bool isStartFileLinkFlag();
		static bool isEndFileLinkFlag();

		static bool isSceneTypeFlag();
		
		static bool isValidSceneType();
		static void handleFileLink();
		static void handleMessage();
		static void handleSceneType();
		static std::vector<std::string> handleVariableFlag();

		static void loadFileContent();

		friend struct SceneManager;
	};
}