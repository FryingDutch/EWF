#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include "../DataObject.h"

namespace EWF
{
	class System
	{
	private:
		static std::string date;
		static std::string response;
		static uint32_t terminalWidth;
		static uint32_t terminalHeight;

	private:
		static std::ifstream readFiles;

	public:
		static bool isRunning;

	private:
		static void setTerminalWidth(uint32_t _width) { System::terminalWidth = _width; };
		static void setTerminalHeight(uint32_t _height) { System::terminalHeight = _height; };

	public:
		static void errorMessage(const char* _msg, bool _hardError = false);
		static std::string getWorkingDirectory();

		static uint32_t getTerminalHeight() { return System::terminalHeight; };
		static uint32_t getTerminalWidth() { return System::terminalWidth; };

		static void resetDimensions()
		{
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
			System::setTerminalWidth(csbi.srWindow.Right - csbi.srWindow.Left + 1);
			System::setTerminalHeight(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
		}

		static std::string readFileIntoString(const char* _name);

		static std::string getInput(std::string message = "Make a choice: ");
		static void printDataObject(DataObject _dataObject);
		static bool isDigit(std::string str);
		static std::string getDate() { return date; }
	};
}