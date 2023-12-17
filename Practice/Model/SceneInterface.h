#pragma once
#include <vector>
#include <string>
#include "Scene/Abstract/AbstractScene.h"

class SceneInterface : public EWF::AbstractScene
{
public:
	virtual void render(bool responseIsString = false, std::string _message = "Make a choice: ") = 0;
};

