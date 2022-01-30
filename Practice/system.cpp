#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "System.h"
#include <direct.h>

namespace EWF
{
	std::ifstream System::readFiles;
	bool System::isRunning = true;

	std::string System::getWorkingDirectory()
	{
		const char* wd = _getcwd(NULL, 0);
		return wd;
	}

	bool System::fileFound(const char* _name)
	{
		std::string fullPath{ _name };

		readFiles.open(fullPath);
		if (readFiles)
		{
			readFiles.close();
			return true;
		}
		return false;
	}

	std::string System::readFile(const char* _name)
	{
		std::ostringstream ss;
		std::string result;

		std::ifstream fileToRead(_name);
		if (fileToRead)
		{
			ss << fileToRead.rdbuf();
			fileToRead.close();
			return ss.str();
		}
		else
		{
			result = "";
			return result;
		}
	}

	bool System::isDigit(std::string str)
	{
		if (str.empty())
			return false;

		for (size_t i = 0; i < str.length(); i++)
		{
			if (!std::isdigit(str[i]))
				return false;
		}
		return true;
	}

	std::string System::getInput(std::string message)
	{
		std::cout << message;
		static std::string answer;
		std::getline(std::cin, answer);

		return answer;
	}
}