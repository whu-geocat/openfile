/*******************************************************************************
**     FileName: json.h
**    ClassName: json
**       Author: Geocat & LittleBottle
**  Create Time: 2024/03/06 22:06
**  Description:
*******************************************************************************/

#ifndef JSON_H
#define JSON_H

#include <memory>

namespace json {

enum JsonError
{
    kErrorNone,
    kInvalid,
};

class JsonObject
{
public:
    enum ValueType
    {
        kText,
        kBool,
        kInt,
        kFloat,
        kArray,
        kObject,
    };
    JsonObject(ValueType type);
    ~JsonObject();

    JsonError parseFromString(const std::string& text);

    ValueType type() const;

    std::string getName() const;
    std::string getText() const;
    bool getBool() const;
    int getInt() const;
    float getFloat() const;
    std::vector<JsonObject*> getArray() const;

    void setText(const std::string& text);
    void setBool(bool value);
    void setInt(int value);
    void setFloat(float value);
    void setArray(std::vector<JsonObject*>& array);
    void setName(const std::string&);

protected:
    struct Data;
    std::unique_ptr<Data> m_data;
};

class Json
{
public:
    Json();
    ~Json();

    JsonError loadFromString(const std::string& text);
    JsonError loadFromFile(const std::string& path);
    JsonError saveToFile(const std::string& path, bool format = false);

    JsonObject* findObject(const std::string& name) const;

protected:
    struct Data;
    std::unique_ptr<Data> m_data;
};

}  // namespace json

#endif  // JSON_H