/*******************************************************************************
**     FileName: Field.h
**    ClassName: Field
**       Author: Geocat & LittleBottle
**  Create Time: 2023/07/12 22:25
**  Description:
*******************************************************************************/

#ifndef FIELD_H
#define FIELD_H

#include <cstdint>
#include <dbf/FieldDefinition.h>


namespace dbf {

class Field
{
public:
    Field(const FieldDefinition* definition);
    Field(const Field& other);
    Field(Field&& other);
    ~Field();

    Field& operator= (const Field& other);
    Field& operator= (Field&& other);

    void setInt(int32_t value);
    void setDouble(double value);
    void setString(const char* value);

    int32_t getInt() const;
    double getDouble() const;
    const char* getString() const;

protected:
    union Data
    {
        int32_t intValue;
        double doubleValue;
        char* stringValue;
    } m_data;
    // the filed definition cannot be modified by the field.
    const FieldDefinition* m_definition;
};
}  // namespace shp

#endif  // FIELD_H