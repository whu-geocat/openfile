
#include "ShpInternal.h"

#include <memory>
#include <shp/shp.h>

namespace shp {

shp::shp()
    : m_internal(std::make_shared<ShpInternal>())
{
}

shp::~shp() {}

shp::Error shp::open(const std::string& filename)
{
    return m_internal->open(filename);
}

shp::Error shp::save(const std::string& filename)
{
    return m_internal->save(filename);
}

shp::Error shp::close() { return m_internal->close(); }

uint64_t shp::getNumShapes() const { return m_internal->getNumShapes(); }

shp::Error shp::getShapeAt(uint64_t index, ShapePtr shape)
{
    return m_internal->getShapeAt(index, shape);
}

shp::Error shp::setShapeAt(uint64_t index, ShapePtr shape)
{
    return m_internal->setShapeAt(index, shape);
}

const ShpHeader& shp::getHeader() const { return m_internal->getHeader(); }

shp::Error shp::setHeader(const ShpHeader& header)
{
    return m_internal->setHeader(header);
}

}  // namespace shp