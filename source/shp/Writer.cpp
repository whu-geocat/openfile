#include "Writer.h"

#include "shp/shp.h"

#include <cstdint>
#include <fstream>

Writer::Writer() {}

Writer::~Writer() {}

bool Writer::write(const std::string& filename,
                   const shp::shp::ShpInternal& shp)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }
    bool res = writeHeader(file, shp.getHeader());
    if (!res)
    {
        return res;
    }
    res = writeRecords(file, shp.getRecords());
    return res;
}

bool Writer::writeHeader(std::ofstream& file, const shp::ShpHeader& header)
{
    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    return true;
}

bool Writer::writeRecords(std::ofstream& file,
                          const std::vector<ShpRecord>& records)
{
    for (const auto& record : records)
    {
        bool res = WriteRecord(file, record);
        if (!res)
            return res;
    }
    return true;
}

bool Writer::WriteRecord(std::ofstream& file, const ShpRecord& record)
{
    const auto& field = record.getRecordField();
    file.write(reinterpret_cast<const char*>(&field), sizeof(field));

    auto shape = record.getShape();
    switch (shape->getType())
    {
    case shp::ShapeType::kPoint :
        writePoint(file, shape);
        break;
    case shp::ShapeType::kPointM :
        writePointM(file, shape);
        break;
    case shp::ShapeType::kPointZ :
        writePointZ(file, shape);
        break;
    case shp::ShapeType::kPolyLine :
        writePolyLine(file, shape);
        break;
    case shp::ShapeType::kPolyLineM :
        writePolyLineM(file, shape);
        break;
    case shp::ShapeType::kPolyLineZ :
        writePolyLineZ(file, shape);
        break;
    case shp::ShapeType::kPolygon :
        writePolygon(file, shape);
        break;
    case shp::ShapeType::kPolygonM :
        writePolygonM(file, shape);
        break;
    case shp::ShapeType::kPolygonZ :
        writePolygonZ(file, shape);
        break;
    case shp::ShapeType::kMultiPoint :
        writeMultiPoint(file, shape);
        break;
    case shp::ShapeType::kMultiPointM :
        writeMultiPointM(file, shape);
        break;
    case shp::ShapeType::kMultiPointZ :
        writeMultiPointZ(file, shape);
        break;
    case shp::ShapeType::kMultiPatch :
        writeMultiPatch(file, shape);
        break;

    default :
        break;
    }

    return true;
}

bool Writer::writePoint(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& pnts = shape->getPoints2d();
    if (pnts.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&pnts[0]), sizeof(pnts[0]));
    return true;
}

bool Writer::writePointZ(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& pnts = shape->getPoints2d();
    const auto& z = shape->getZs();
    const auto& m = shape->getMeasurements();
    if (pnts.empty() || z.empty() || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&pnts[0]), sizeof(pnts[0]));
    file.write(reinterpret_cast<const char*>(&z[0]), sizeof(z[0]));
    file.write(reinterpret_cast<const char*>(&m[0]), sizeof(m[0]));
    return true;
}

bool Writer::writePointM(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& pnts = shape->getPoints2d();
    const auto& m = shape->getMeasurements();
    if (pnts.empty() || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&pnts[0]), sizeof(pnts[0]));
    file.write(reinterpret_cast<const char*>(&m[0]), sizeof(m[0]));
    return true;
}

bool Writer::writePolyLine(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& parts = shape->getParts();
    int32_t numParts = parts.size();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    if (numPoints == 0 || numParts == 0)
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numParts), sizeof(numParts));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(parts.data()),
               numParts * sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    return true;
}

bool Writer::writePolyLineZ(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& parts = shape->getParts();
    int32_t numParts = parts.size();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    const auto& z = shape->getZs();
    const auto& m = shape->getMeasurements();
    shp::Range2d mrange = shape->getMRange();
    shp::Range2d zrange = shape->getZRange();

    if (numPoints == 0 || numParts == 0 || z.empty() || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numParts), sizeof(numParts));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(parts.data()),
               numParts * sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    file.write(reinterpret_cast<const char*>(&zrange), sizeof(zrange));
    file.write(reinterpret_cast<const char*>(z.data()),
               numPoints * sizeof(double));
    file.write(reinterpret_cast<const char*>(&mrange), sizeof(mrange));
    file.write(reinterpret_cast<const char*>(m.data()),
               numPoints * sizeof(double));
    return true;
}

bool Writer::writePolyLineM(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& parts = shape->getParts();
    int32_t numParts = parts.size();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    const auto& m = shape->getMeasurements();
    shp::Range2d mrange = shape->getMRange();

    if (numPoints == 0 || numParts == 0 || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numParts), sizeof(numParts));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(parts.data()),
               numParts * sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    file.write(reinterpret_cast<const char*>(&mrange), sizeof(mrange));
    file.write(reinterpret_cast<const char*>(m.data()),
               numPoints * sizeof(double));
    return true;
}

bool Writer::writePolygon(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& parts = shape->getParts();
    int32_t numParts = parts.size();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    if (numPoints == 0 || numParts == 0)
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numParts), sizeof(numParts));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(parts.data()),
               numParts * sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    return true;
}

bool Writer::writePolygonZ(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& parts = shape->getParts();
    int32_t numParts = parts.size();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    const auto& z = shape->getZs();
    const auto& m = shape->getMeasurements();
    shp::Range2d mrange = shape->getMRange();
    shp::Range2d zrange = shape->getZRange();

    if (numPoints == 0 || numParts == 0 || z.empty() || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numParts), sizeof(numParts));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(parts.data()),
               numParts * sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    file.write(reinterpret_cast<const char*>(&zrange), sizeof(zrange));
    file.write(reinterpret_cast<const char*>(z.data()),
               numPoints * sizeof(double));
    file.write(reinterpret_cast<const char*>(&mrange), sizeof(mrange));
    file.write(reinterpret_cast<const char*>(m.data()),
               numPoints * sizeof(double));
    return true;
}

bool Writer::writePolygonM(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& parts = shape->getParts();
    int32_t numParts = parts.size();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    const auto& m = shape->getMeasurements();
    shp::Range2d mrange = shape->getMRange();

    if (numPoints == 0 || numParts == 0 || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numParts), sizeof(numParts));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(parts.data()),
               numParts * sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    file.write(reinterpret_cast<const char*>(&mrange), sizeof(mrange));
    file.write(reinterpret_cast<const char*>(m.data()),
               numPoints * sizeof(double));
    return true;
}

bool Writer::writeMultiPoint(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    if (numPoints == 0)
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    return true;
}

bool Writer::writeMultiPointZ(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    const auto& z = shape->getZs();
    const auto& m = shape->getMeasurements();
    shp::Range2d zrange = shape->getZRange();
    shp::Range2d mrange = shape->getMRange();
    if (numPoints == 0 || z.empty() || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    file.write(reinterpret_cast<const char*>(&zrange), sizeof(zrange));
    file.write(reinterpret_cast<const char*>(z.data()),
               numPoints * sizeof(double));
    file.write(reinterpret_cast<const char*>(&mrange), sizeof(mrange));
    file.write(reinterpret_cast<const char*>(m.data()),
               numPoints * sizeof(double));
    return true;
}

bool Writer::writeMultiPointM(std::ofstream& file, const shp::ShapePtr shape)
{
    const auto& box = shape->getBoundingBox2d();
    const auto& pnts = shape->getPoints2d();
    int32_t numPoints = pnts.size();
    const auto& m = shape->getMeasurements();
    shp::Range2d mrange = shape->getMRange();
    if (numPoints == 0 || m.empty())
        return false;
    file.write(reinterpret_cast<const char*>(&box), sizeof(box));
    file.write(reinterpret_cast<const char*>(&numPoints), sizeof(numPoints));
    file.write(reinterpret_cast<const char*>(pnts.data()),
               numPoints * sizeof(shp::Point2d));
    file.write(reinterpret_cast<const char*>(&mrange), sizeof(mrange));
    file.write(reinterpret_cast<const char*>(m.data()),
               numPoints * sizeof(double));
    return true;
}

bool Writer::writeMultiPatch(std::ofstream& file, const shp::ShapePtr shape)
{
    std::runtime_error("MultiPatch not supported");
    return false;
}
