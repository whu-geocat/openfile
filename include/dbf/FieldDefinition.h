/*******************************************************************************
**     FileName: FieldDefinition.h
**    ClassName: FieldDefinition
**       Author: Geocat & LittleBottle
**  Create Time: 2023/07/12 22:39
**  Description:
*******************************************************************************/

#ifndef FIELDDEFINITION_H
#define FIELDDEFINITION_H

#include <cstdint>
#include <string>

namespace dbf {

class FieldDefinition
{
public:
    enum FieldType : uint8_t
    {
        kInvalid = 0,
        kInteger = 'I',
        kDouble = 'F',
        kString = 'C',
        kLogical = 'L',
        kDate = 'D',
    };
    FieldDefinition(const std::string& name, uint16_t length, uint8_t type);
    FieldDefinition(const FieldDefinition& other);
    FieldDefinition(FieldDefinition&& other);
    ~FieldDefinition();

    FieldDefinition& operator= (const FieldDefinition& other);
    FieldDefinition& operator= (FieldDefinition&& other);

    uint16_t length() const;
    uint8_t type() const;
    std::string name() const;

protected:
    uint16_t m_fieldLength;
    uint8_t m_fieldType;
    std::string m_fieldName;
};

}  // namespace shp

#endif  // FIELDDEFINITION_H
