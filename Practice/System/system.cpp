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
	uint32_t System::terminalHeight;
	uint32_t System::terminalWidth;

	void System::errorMessage(const char* message, bool _hardError)
	{
		isRunning = !_hardError;
		const char* terminatingText = isRunning ? "\n" : " Terminating...\n";

		std::cerr << "[ERROR]: " << message << "!" << terminatingText;

		if (!isRunning)
		{
			exit(EXIT_FAILURE);
		}
	}

	std::string System::getWorkingDirectory()
	{
		const char* wd = _getcwd(NULL, 0);
		return wd;
	}

	std::string System::readFileIntoString(const char* _name)
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
		{
			std::string fileName{ _name };
			std::string msg = "File " + fileName + " not found";
			errorMessage(msg.c_str(), true);
		}
		return ss.str(); // Will never reach, but muffles compiler for non returning path.
	}

	bool System::isDigit(std::string str)
	{
		if (str.empty())
		{
			return false;
		}
			

		for (size_t i = 0; i < str.length(); i++)
		{
			if (!std::isdigit(str[i]))
			{
				return false;
			}
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

	void System::printDataObject(DataObject _dataObject)
	{
		std::cout << std::setw(4) << _dataObject.getData() << "\n";
	}
}