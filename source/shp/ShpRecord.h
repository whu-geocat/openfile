/*******************************************************************************
**     FileName: ShpRecord.h
**    ClassName: ShpRecord
**       Author: Geocat & LittleBottle
**  Create Time: 2024/03/08 18:38
**  Description:
*******************************************************************************/

#ifndef SHPRECORD_H
#define SHPRECORD_H

#include <cstdint>
#include <memory>
#include <shp/shp.h>

class ShpRecord
{
public:
    ShpRecord();
    ~ShpRecord();

    struct RecordField
    {
        int32_t recordNumber;   // big
        int32_t contentLength;  // big
        int32_t shapeType;      // little
    };

    std::shared_ptr<shp::Shape> getShape() const;
    void setShape(std::shared_ptr<shp::Shape> shape);

    int32_t getRecordNumber() const;
    void setRecordNumber(int32_t recordNumber);

    int32_t getContentLength() const;
    void setContentLength(int32_t contentLength);

    RecordField getRecordField() const;
    void setRecordField(const RecordField& field);

protected:
    std::shared_ptr<shp::Shape> m_shape;
    RecordField m_field;
};  // class ShpRecord

#endif  // SHPRECORD_H