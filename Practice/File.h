#pragma once
#include <json.hpp>
#include "DataObject.h"
#include "Optionr.h"

namespace EWF
{
	class File : public DataObject
	{
	public:
		const std::string STORY = "story";
		const std::string STORY_OPTIONS = "story-options";
		const std::string OUTCOMES = "outcomes";

	public:
		File()
		{
			this->setData(File::STORY, "");
			this->setData(File::STORY_OPTIONS, nlohmann::json::array());
			this->setData(File::OUTCOMES, nlohmann::json::array());
		}

		void setStory(std::string _story)
		{
			this->setData(File::STORY, _story);
		}

		std::string getStory()
		{
			return this->getData(File::STORY);
		}

		void setOptions(nlohmann::json _options)
		{
			this->setData(File::STORY_OPTIONS, _options);
		}

		nlohmann::json getOptions()
		{
			return this->getData(File::STORY_OPTIONS);
		}

		void addOption(StoryOption _option)
		{
			nlohmann::json data = this->getOptions();
			data.push_back(_option.getData());
			this->setData(File::STORY_OPTIONS, data);
		}

		void setOutcomes(nlohmann::json _options)
		{
			this->setData(File::OUTCOMES, _options);
		}

		nlohmann::json getOutcomes()
		{
			return this->getData(File::OUTCOMES);
		}

		void addOutcome(StoryOption _option)
		{
			nlohmann::json data = this->getOutcomes();
			data.push_back(_option.getData());
			this->setData(File::OUTCOMES, data);
		}
	};
}