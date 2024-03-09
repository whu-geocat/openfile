#include <json/json.h>

namespace json {

struct JsonObject::Data
{
};

JsonObject::JsonObject(JsonObject::ValueType type) {}

JsonObject::~JsonObject() {}

JsonError JsonObject::parseFromString(const std::string& text) {}

ValueType JsonObject::type() const {}

std::string JsonObject::getName() const {}

std::string JsonObject::getText() const {}

bool JsonObject::getBool() const {}

int JsonObject::getInt() const {}

float JsonObject::getFloat() const {}

std::vector<JsonObject*> JsonObject::getArray() const {}

void JsonObject::setText(const std::string& text) {}

void JsonObject::setBool(bool value) {}

void JsonObject::setInt(int value) {}

void JsonObject::setFloat(float value) {}

void JsonObject::setArray(std::vector<JsonObject*>& array) {}

void JsonObject::setName(const std::string&) {}

}  // namespace json