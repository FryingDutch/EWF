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
		static std::string getWorkingDirectory();

		static std::string readFile(const char* _name);
		static bool fileFound(const char* _name);

		static std::string getInput(std::string message = "Make a choice: ");
		static bool isDigit(std::string str);
		static std::string getDate() { return date; }
	};
}