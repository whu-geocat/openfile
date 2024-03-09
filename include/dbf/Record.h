/*******************************************************************************
**     FileName: Record.h
**    ClassName: Record
**       Author: Geocat & LittleBottle
**  Create Time: 2023/07/12 22:28
**  Description:
*******************************************************************************/

#ifndef RECORD_H
#define RECORD_H

#include <cstdint>
#include <dbf/Field.h>

namespace dbf {

class Record
{
    friend class Table;
    Record();

public:
    Record(const Record& other);
    Record(Record&& other);
    ~Record();

    Record& operator= (const Record& other);
    Record& operator= (Record&& other);

    uint16_t fieldCount() const;

    void setInt(uint16_t index, int32_t value);
    void setDouble(uint16_t index, double value);
    void setString(uint16_t index, const char* value);

    int32_t getInt(uint16_t index) const;
    double getDouble(uint16_t index) const;
    const char* getString(uint16_t index) const;

    void addField(const Field& field);
    void removeField(uint16_t index);

    Field getField(uint16_t index) const;
    Field& getField(uint16_t index);

protected:
    std::vector<Field> m_fields;
};  // class Record

}  // namespace shp

#endif  // RECORD_H