#pragma once
#include <json.hpp>
#include "DataObject.h"

namespace EWF
{
	class StoryOption : public DataObject
	{
	public:
		const std::string TEXT = "text";
		const std::string LINK = "link";
		const std::string VAR_CHANGES = "variable-changes";

	public:
		StoryOption()
		{
			this->setData("id", NULL);
			this->setData(StoryOption::TEXT, "");
			this->setData(StoryOption::LINK, "");
			this->setData(StoryOption::VAR_CHANGES, nlohmann::json::array());
		}

		StoryOption(nlohmann::json _data):
			DataObject(_data)
		{}

		~StoryOption()
		{
		}

		void setId(uint32_t _id)
		{
			this->setData("id", _id);
		};

		size_t getId()
		{
			return this->getData("id");
		};

		void setText(std::string _text)
		{
			this->setData(StoryOption::TEXT, _text);
		};

		std::string getText()
		{
			return this->getData(StoryOption::TEXT);
		};

		void setLink(std::string _link)
		{
			this->setData(StoryOption::LINK, _link);
		}

		std::string getLink()
		{
			return this->getData(StoryOption::LINK);
		}

		void setVariableChanges(nlohmann::json _varChanges)
		{
			this->setData(StoryOption::VAR_CHANGES, _varChanges);
		}

		nlohmann::json getVariableChanges()
		{
			return this->getData(StoryOption::VAR_CHANGES);
		}

		void addVariableChange(std::vector<std::string> _varChange)
		{
			nlohmann::json varChanges = this->getVariableChanges();
			varChanges.push_back(_varChange);
			this->setVariableChanges(varChanges);
		}
	};
}