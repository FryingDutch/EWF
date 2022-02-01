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

	void System::errorMessage(const char* message, bool _hardError)
	{
		isRunning = !_hardError;
		std::cerr << "[ERROR]: " << message << "! Terminating...\n";
		if (!isRunning)
			exit(EXIT_FAILURE);
	}

	std::string System::getWorkingDirectory()
	{
		const char* wd = _getcwd(NULL, 0);
		return wd;
	}

	std::string System::readFile(const char* _name)
	{
		std::ostringstream ss;

		std::ifstream fileToRead(_name);
		if (fileToRead)
		{
			ss << fileToRead.rdbuf();
			fileToRead.close();
			return ss.str();
		}
		else
			errorMessage("File not found", true);

		return ss.str(); // Will never reach, but muffles compiler for non returning path.
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