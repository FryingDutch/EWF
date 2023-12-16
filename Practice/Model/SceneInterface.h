#pragma once
#include <vector>
#include <string>
#include "Scene/Abstract/AbstractScene.h"

class SceneInterface : public EWF::AbstractScene
{
public:
	virtual std::string getResponse() = 0;
	virtual void setText(std::vector<std::string> _text) = 0;
	virtual std::string getText(uint32_t index) = 0;
	virtual void render(bool responseIsString = false, std::string _message = "Make a choice: ") = 0;
};

