#pragma once

#include <sstream>
#include <string>

class JsonArchive {
public:
	JsonArchive();
	JsonArchive& operator&(const uint8_t& value);
	JsonArchive& operator&(const char(&value)[32]);
	JsonArchive& operator&(const uint16_t& value);
	JsonArchive& operator&(const bool& value);
	
	void endObject();
	std::string str() const;

private:
	std::ostringstream jsonStream;
};