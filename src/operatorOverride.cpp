#include "operatorOverride.hpp"

JsonArchive::JsonArchive() {
	jsonStream << "{";
}

JsonArchive& JsonArchive::operator&(const uint8_t& value) {
	jsonStream << static_cast<int>(value);
	return *this;
}

JsonArchive& JsonArchive::operator&(const char(&value)[32]) {
	jsonStream << "\"" << value << "\"";
	return *this;
}

JsonArchive& JsonArchive::operator&(const uint16_t& value) {
	jsonStream << value;
	return *this;
}

JsonArchive& JsonArchive::operator&(const bool& value) {
	jsonStream << (value ? "true" : "false");
	return *this;
}

void JsonArchive::endObject() {
    std::string jsonStr = jsonStream.str();
    if (!jsonStr.empty() && jsonStr.back() == ',') {
        jsonStr.pop_back();
    }
    jsonStr += "}";
    jsonStream.str("");
    jsonStream.clear();
    jsonStream << jsonStr;
}

std::string JsonArchive::str() const {
    return jsonStream.str();
}