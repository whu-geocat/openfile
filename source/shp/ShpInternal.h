#ifndef SHPINTERNAL_H
#define SHPINTERNAL_H

#include <memory>
#include <shp/shp.h>

class shp::shp::ShpInternal
{
protected:
    friend class Reader;
    friend class Writer;

public:
    ShpInternal();
    ~ShpInternal();

    Error open(const std::string& filename);

    Error save(const std::string& filename);
    Error close();
    uint64_t getNumShapes() const;
    Error getShapeAt(uint64_t index, ShapePtr shape);
    std::vector<ShapePtr> getAllShapes() const;
    Error setShapeAt(uint64_t index, ShapePtr);

    const ShpHeader& getHeader() const;
    ShpHeader& getHeader();
    Error setHeader(const ShpHeader& header);

    std::vector<ShpRecord>& getRecords();
    const std::vector<ShpRecord>& getRecords() const;

protected:
    struct Data;
    std::shared_ptr<Data> m_data;
};  // class ShpInternal

#endif  // SHPINTERNAL_H