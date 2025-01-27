#include <string>
#include <vector>

#pragma once
class Event
{
private:
	std::string m_type;
	std::vector<void*> m_data;

public:
	Event(std::string _type, void* _data1);
	Event(std::string _type, void* _data1, void* _data2);
	~Event();
	std::string AsXMLString();
};

