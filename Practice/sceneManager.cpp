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


	void SceneManager::buildScene(char _sceneType)
	{
		if (FileParser::sceneType == INTRO)
		{
			introScene.setText(FileParser::textBlocks);
			introScene.render();
			response = 1;
		}

		else if (FileParser::sceneType == DEFAULT)
		{
			defaultScene.setText(FileParser::textBlocks);
			(FileParser::message.size() > 0) ? defaultScene.render(FileParser::message) : defaultScene.render();
			response = defaultScene.getResponse();
		}

		// Set the next file to read.
		FileParser::filePath = FileParser::fileLinks[response - 1];
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