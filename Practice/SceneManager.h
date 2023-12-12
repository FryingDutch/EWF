#pragma once
#include <string>
#include "System.h"
#include "DefaultScene.h"
#include "IntroScene.h"

namespace EWF
{
	struct SceneManager
	{
	private:
		static std::string response;

		static DefaultScene defaultScene;
		static IntroScene introScene;

	public:
		enum : char
		{
			INTRO = 'I', DEFAULT = 'D'
		};
		
		static void applyStatsChanges(size_t _i);
		static void buildScene(char _sceneType);
		static void runGame();
	};
}