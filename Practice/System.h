#pragma once
#include <iostream>
#include <fstream>
#include <vector>

namespace EWF
{
	class System
	{
	private:
		static std::string date;
		static std::string response;

	private:
		static std::ifstream readFiles;

	public:
		static bool isRunning;

	public:
		static void errorMessage(const char* _msg, bool _hardError = false);
		static std::string getWorkingDirectory();

		static std::string readFileIntoString(const char* _name);

		static std::string getInput(std::string message = "Make a choice: ");
		static bool isDigit(std::string str);
		static std::string getDate() { return date; }
	};
}