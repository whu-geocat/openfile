/*******************************************************************************
**     FileName: DbfHeader.h
**    ClassName: DbfHeader
**       Author: Geocat & LittleBottle
**  Create Time: 2023/07/12 23:02
**  Description:
*******************************************************************************/

#ifndef DBFHEADER_H
#define DBFHEADER_H

#include <cstdint>

namespace dbf {

class DbfHeader
{
    friend class Table;
    DbfHeader();

public:
    ~DbfHeader();

    uint32_t recordCount() const;
    uint16_t fieldCount() const;

    void setRecordCount(uint32_t count);
    void setFieldCount(uint16_t count);

protected:
};  // class DbfHeader

}  // namespace shp

#endif  // DBFHEADER_H