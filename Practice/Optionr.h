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
			this->setData(StoryOption::TEXT, "");
			this->setData(StoryOption::LINK, "");
			this->setData(StoryOption::VAR_CHANGES, nlohmann::json::array());
		}

		void setText(std::string _text)
		{
			this->setData(StoryOption::TEXT, _text);
		};

		void setLink(std::string _link)
		{
			this->setData(StoryOption::LINK, _link);
		}

		std::string getLink()
		{
			return this->getData(StoryOption::LINK);
		}

		void setVariableChanges(nlohmann::json _linkedChoices)
		{
			this->setData(StoryOption::VAR_CHANGES, _linkedChoices);
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