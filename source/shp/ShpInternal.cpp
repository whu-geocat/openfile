#include "ShpInternal.h"

#include "Reader.h"
#include "ShpRecord.h"
#include "Writer.h"

#include <vector>

struct shp::shp::ShpInternal::Data
{
    ShpHeader header;
    std::vector<ShpRecord> records;

    bool dirty;

    std::shared_ptr<Reader> reader;
    std::shared_ptr<Writer> writer;
};

shp::shp::ShpInternal::ShpInternal()
    : m_data(std::make_shared<Data>())
{
}

shp::shp::ShpInternal::~ShpInternal() {}

shp::shp::Error shp::shp::ShpInternal::open(const std::string& filename)
{
    m_data->reader = std::make_shared<Reader>();
    bool res = m_data->reader->read(filename, *this);
    return res ? kErrorNone : kErrorRead;
}

shp::shp::Error shp::shp::ShpInternal::save(const std::string& filename)
{
    m_data->writer = std::make_shared<Writer>();
    bool res = m_data->writer->write(filename, *this);
    return res ? kErrorNone : kErrorWrite;
}

shp::shp::Error shp::shp::ShpInternal::close()
{
    if (m_data->dirty)
    {
        return kDataDirty;
    }
    return kErrorNone;
}

uint64_t shp::shp::ShpInternal::getNumShapes() const
{
    return m_data->records.size();
}
shp::shp::Error shp::shp::ShpInternal::getShapeAt(uint64_t index,
                                                  ShapePtr shape)
{
    if (index >= m_data->records.size())
        return kIndexOutOfRange;
    shape = (m_data->records[index].getShape());
    return kErrorNone;
}

std::vector<shp::ShapePtr> shp::shp::ShpInternal::getAllShapes() const
{
    std::vector<ShapePtr> ret;
    for (const auto& rec : m_data->records)
    {
        ret.push_back(rec.getShape());
    }
    return ret;
}

shp::shp::Error shp::shp::ShpInternal::setShapeAt(uint64_t index,
                                                  ShapePtr shape)
{
    if (index >= m_data->records.size())
        return kIndexOutOfRange;
    bool addrSame = m_data->records[index].getShape() == shape;
    if (!addrSame)
    {
        m_data->dirty = true;
        m_data->records[index].setShape(shape->clone());
        return kErrorNone;
    }
    else
    {
        if (shape->dirty())
        {
            m_data->dirty = true;
        }
    }
    return kErrorNone;
}

const shp::ShpHeader& shp::shp::ShpInternal::getHeader() const
{
    return m_data->header;
}

shp::ShpHeader& shp::shp::ShpInternal::getHeader() { return m_data->header; }

shp::shp::Error shp::shp::ShpInternal::setHeader(const ShpHeader& header)
{
    m_data->header = header;
    m_data->dirty = true;
    return kErrorNone;
}

std::vector<ShpRecord>& shp::shp::ShpInternal::getRecords()
{
    return m_data->records;
}
const std::vector<ShpRecord>& shp::shp::ShpInternal::getRecords() const
{
    return m_data->records;
}