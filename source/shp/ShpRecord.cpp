#include "ShpRecord.h"

ShpRecord::ShpRecord()
    : m_shape(nullptr)
{
}

ShpRecord::~ShpRecord() {}

std::shared_ptr<shp::Shape> ShpRecord::getShape() const { return m_shape; }
void ShpRecord::setShape(std::shared_ptr<shp::Shape> shape) { m_shape = shape; }

int32_t ShpRecord::getRecordNumber() const { return m_field.recordNumber; }
void ShpRecord::setRecordNumber(int32_t recordNumber)
{
    m_field.recordNumber = recordNumber;
}

int32_t ShpRecord::getContentLength() const { return m_field.contentLength; }
void ShpRecord::setContentLength(int32_t contentLength)
{
    m_field.contentLength = contentLength;
}

ShpRecord::RecordField ShpRecord::getRecordField() const { return m_field; }
void ShpRecord::setRecordField(const RecordField& field) { m_field = field; }