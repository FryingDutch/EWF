#pragma once
#include <json.hpp>

namespace EWF
{
	class DataObject
	{
	private:
		nlohmann::json m_data;

	public:
		DataObject(nlohmann::json _data = nlohmann::json()):
			m_data(_data) {}

		nlohmann::json getData() 
		{ 
			return this->m_data; 
		}

		auto getData(std::string _key)
		{
			return this->m_data[_key];
		}

		void setData(nlohmann::json _data) 
		{ 
			this->m_data = _data; 
		}

		template<typename T>
		void setData(std::string _key, const T& arg)
		{
			this->m_data[_key] = arg;
		}
	};
}