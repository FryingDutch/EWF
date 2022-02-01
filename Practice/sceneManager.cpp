#include <string>
#include "DefaultScene.h"
#include "IntroScene.h"
#include "SceneManager.h"
#include "FileParser.h"

namespace EWF
{
	uint32_t SceneManager::response = 0;

	DefaultScene SceneManager::defaultScene;
	IntroScene SceneManager::introScene;

	// Build the scene according to the _sceneType (Which is provided by the file with ~ prefix)
	void SceneManager::buildScene(char _sceneType)
	{
		switch (FileParser::sceneType)
		{
		case INTRO:
			introScene.setText(FileParser::textBlocks);
			introScene.render();
			response = 1;
			break;

		case DEFAULT:
			defaultScene.setText(FileParser::textBlocks);
			(FileParser::message.size() > 0) ? defaultScene.render(FileParser::message) : defaultScene.render();
			response = defaultScene.getResponse();
			break;

		// TO-DO add more sceneTemplates to use.
		default:
			System::errorMessage("Not a valid sceneType");
			return;
		}

		// Set the next file to read.
		if ((response - 1) < FileParser::fileLinks.size() && response - 1 >= 0 && FileParser::fileLinks.size() > 0)
			FileParser::filePath = FileParser::fileLinks[response - 1];

		// else if ((response - 1) < 0)
			// Go to menu

		else if(FileParser::fileLinks.size() <= 0)
			System::errorMessage("No file link bound to this choice", true);
	}

	void SceneManager::runGame()
	{
		while (System::isRunning)
		{
			FileParser::loadText();
			buildScene(FileParser::sceneType);
		}
	}
}