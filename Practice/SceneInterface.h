#pragma once
#include <vector>
#include <string>

class SceneInterface {
public:
	virtual void setText(std::vector<std::string> _text) = 0;
	virtual std::string getText(uint32_t index) = 0;
};

