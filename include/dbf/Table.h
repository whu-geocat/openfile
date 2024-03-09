/*******************************************************************************
**     FileName: Table.h
**    ClassName: Table
**       Author: Geocat & LittleBottle
**  Create Time: 2023/07/12 22:18
**  Description:
*******************************************************************************/

#ifndef TABLE_H
#define TABLE_H

#include <cstdint>
#include <dbf/DbfHeader.h>
#include <dbf/Record.h>
#include <vector>

namespace dbf {

class Table
{
public:
    Table();
    ~Table();

    uint64_t recordCount() const;
    uint64_t fieldCount() const;

    void addFieldDefinition(const FieldDefinition& definition);

    bool setInt(uint64_t recordIndex, uint16_t fieldIndex, int32_t value);
    bool setDouble(uint64_t recordIndex, uint16_t fieldIndex, double value);
    bool setString(uint64_t recordIndex, uint16_t fieldIndex,
                   const char* value);

    bool getInt(uint64_t recordIndex, uint16_t fieldIndex,
                int32_t& value) const;
    bool getDouble(uint64_t recordIndex, uint16_t fieldIndex,
                   double& value) const;
    bool getString(uint64_t recordIndex, uint16_t fieldIndex,
                   const char* value) const;

    bool addRecord(const Record& record);
    bool removeRecord(uint64_t recordIndex);

    Record getRecord(uint64_t recordIndex) const;
    Record& getRecord(uint64_t recordIndex);

    static Record createRecord(const std::vector<FieldDefinition>& fds);
    static DbfHeader createHeader(const std::vector<FieldDefinition>& fds);

protected:
    std::vector<FieldDefinition> m_definitions;
    std::vector<Record> m_records;
    DbfHeader* m_header;
};  // class Table

}  // namespace dbf

#endif  // TABLE_H