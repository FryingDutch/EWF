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
		const std::string MESSAGE = "message";
		const std::string RESPONSE = "response";
		const std::string SCENETYPE = "scenetype";

	public:
		File()
		{
			this->setData(File::STORY, std::string{});
			this->setData(File::STORY_OPTIONS, nlohmann::json::array());
			this->setData(File::MESSAGE, std::string{});
			this->setData(File::RESPONSE, std::string{});
			this->setData(File::SCENETYPE, std::string{});
		}

		void resetData()
		{
			this->setData(File::STORY, std::string{});
			this->setData(File::STORY_OPTIONS, nlohmann::json::array());
			this->setData(File::MESSAGE, std::string{});
			this->setData(File::RESPONSE, std::string{});
			this->setData(File::SCENETYPE, std::string{});
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

		nlohmann::json getOptionsAsJson()
		{
			return this->getData(File::STORY_OPTIONS);
		}

		std::vector<StoryOption> getOptions()
		{
			nlohmann::json options = this->getData(File::STORY_OPTIONS);
			std::vector<StoryOption> optionObjects;

			for (const auto& option : options) {
				optionObjects.push_back(StoryOption{ option });
			}

			return optionObjects;
		}

		StoryOption getOptionById(uint32_t _id)
		{
			nlohmann::json options = this->getData(File::STORY_OPTIONS);

			for (const auto& option : options) {
				if (option["id"] == _id) {
					return StoryOption{ option };
				}
			}

			return StoryOption{};
		}

		void updateOption(StoryOption _option)
		{
			nlohmann::json options = this->getData(File::STORY_OPTIONS);

			for (auto& option : options) {
				if (option["id"] == _option.getId()) {
					option = _option.getData();
				}
			}

			this->setData(File::STORY_OPTIONS, options);
		}

		void addOption(StoryOption _option)
		{
			nlohmann::json data = this->getOptionsAsJson();
			data.push_back(_option.getData());
			this->setData(File::STORY_OPTIONS, data);
		}

		void setMessage(std::string _message)
		{
			this->setData(File::MESSAGE, _message);
		}

		std::string getMessage()
		{
			return this->getData(File::MESSAGE);
		}

		void setSceneType(char _sceneType)
		{
			std::string sceneType{ _sceneType };
			this->setData(File::SCENETYPE, sceneType);
		}

		char getSceneType()
		{
			std::string sceneType = this->getData(File::SCENETYPE);
			return sceneType.size() > 0 ? sceneType[0] : char{};
		}

		void setResponse(std::string _response)
		{
			this->setData(File::RESPONSE, _response);
		}

		std::string getResponse()
		{
			return this->getData(File::RESPONSE);
		}
	};
}