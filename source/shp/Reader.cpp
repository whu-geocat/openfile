#include "Reader.h"

#include "ShpRecord.h"

#include <cstdint>
#include <cstring>
#include <fstream>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

// mini buffer management: auto reuse and reallocate
class MiniBuffer
{
public:
    char* alloc(uint64_t size, bool reserve = false)
    {
        if (size > m_size)
        {
            char* tmp = m_buffer;
            if (m_buffer == nullptr)
            {
                m_buffer = new char[size];
            }
            if (reserve && tmp != nullptr)
            {
                std::memcpy(m_buffer, tmp, m_size);
            }
            m_size = size;
        }
    }
    void free()
    {
        if (m_buffer != nullptr)
        {
            delete[] m_buffer;
        }
        m_buffer = nullptr;
        m_size = 0;
    }

    char* getBuffer(uint64_t beg = 0) const { return m_buffer + beg; }

protected:
    char* m_buffer = nullptr;
    uint64_t m_size = 0;
};

Reader::Reader()
    : m_buffer(std::make_shared<MiniBuffer>())
{
    // m_readers.insert(std::make_pair(shp::ShapeType::kPoint,
    //                                 ));
    m_readers[shp::ShapeType::kPoint] = std::bind(
        &Reader::readPoint, this, std::placeholders::_1, std::placeholders::_2);
    m_readers[shp::ShapeType::kPointZ]
        = std::bind(&Reader::readPointZ, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kPointM]
        = std::bind(&Reader::readPointM, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kMultiPoint]
        = std::bind(&Reader::readMultiPoint, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kMultiPointZ]
        = std::bind(&Reader::readMultiPointZ, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kMultiPointM]
        = std::bind(&Reader::readMultiPointM, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kPolyLine]
        = std::bind(&Reader::readPolyLine, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kPolyLineZ]
        = std::bind(&Reader::readPolyLineZ, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kPolyLineM]
        = std::bind(&Reader::readPolyLineM, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kPolygon]
        = std::bind(&Reader::readPolygon, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kPolygonZ]
        = std::bind(&Reader::readPolygonZ, this, std::placeholders::_1,
                    std::placeholders::_2);
    m_readers[shp::ShapeType::kPolygonM]
        = std::bind(&Reader::readPolygonM, this, std::placeholders::_1,
                    std::placeholders::_2);
}

Reader::~Reader()
{
    if (m_buffer)
    {
        m_buffer->free();
    }
}

bool Reader::read(const std::string& filename, shp::shp::ShpInternal& shp)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }
    bool res = readHeader(file, shp.getHeader());
    if (!res)
        return false;
    res = readRecords(file, shp.getRecords());
    return res;
}

bool Reader::readHeader(std::ifstream& file, shp::ShpHeader& hdr)
{
    constexpr uint8_t HEADER_SIZE = 100;
    // char header[HEADER_SIZE];
    file.read((char*)(&hdr), HEADER_SIZE);

    return true;
}

bool Reader::ReadRecord(std::ifstream& file, ShpRecord& rec)
{
    if (file.bad() || file.eof())
    {
        return false;
    }
    // the first 3 * sizeof(int32_t) = 12 bytes, are the record fields.
    constexpr uint8_t REC_HEAD_SIZE = 3 * sizeof(int32_t);
    ShpRecord::RecordField rf;
    file.read((char*)&rf, REC_HEAD_SIZE);
    rec.setRecordField(rf);
    // convert RecordField.contentLength from big endian into little endian
    int32_t contentLen = shp::SwapEndian(rf.contentLength);
    auto shape = shp::Shape::CreateShape((shp::ShapeType)rf.shapeType);
    if (shape)
    {
        char* buf = m_buffer->alloc(contentLen);
        file.read(buf, contentLen);
        /*
         * 这里当然可以用switch语句，Writer中正是这么做的。但是，这个库可能会被其他初学者使用，
         * 因此，这里介绍一种现代C++的写法，避免以后的工作和学习过程中大量的switch或if语句。
         * m_readers的元素，是在Reader构造函数中添加的。
         */
        m_readers[(shp::ShapeType)rf.shapeType](buf, shape);
    }
    rec.setShape(shape);
    return true;
}

bool Reader::readRecords(std::ifstream& file, std::vector<ShpRecord>& recs)
{
    ShpRecord rec;
    while (ReadRecord(file, rec))
    {
        recs.push_back(rec);
    }
    return true;
}

bool Reader::readPoint(const char* buf, shp::ShapePtr shape)
{
    shp::Point2d point;
    const double* pointPtr = (const double*)buf;
    if (!pointPtr)
    {
        return false;
    }
    point[0] = pointPtr[0];
    point[1] = pointPtr[1];
    shape->setPoints2d(std::vector<shp::Point2d>(1, point));
    return true;
}

bool Reader::readPointZ(const char* buf, shp::ShapePtr shape)
{
    shp::Point2d point;
    double z = 0;
    double m = 0;
    memcpy(point.data(), buf, sizeof(point));
    memcpy(&z, buf + sizeof(point), sizeof(z));
    memcpy(&m, buf + sizeof(point) + sizeof(z), sizeof(m));
    shape->setPoints2d(std::vector<shp::Point2d>(1, point));
    shape->setZs(std::vector<double>(1, z));
    shape->setMeasurements(std::vector<double>(1, m));
    return true;
}

bool Reader::readPointM(const char* buf, shp::ShapePtr shape)
{
    shp::Point2d point;
    const double* pointPtr = (const double*)buf;
    if (!pointPtr)
    {
        return false;
    }
    point[0] = pointPtr[0];
    point[1] = pointPtr[1];
    shape->setPoints2d(std::vector<shp::Point2d>(1, point));
    double m = 0;
    memcpy(&m, buf + sizeof(shp::Point2d), sizeof(m));
    shape->setMeasurements(std::vector<double>(1, m));
    return true;
}

bool Reader::readPolyLine(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numParts = 0;
    int32_t numPnts = 0;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numParts, buf, sizeof(numParts));
    buf += sizeof(numParts);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    std::vector<int32_t> parts(numParts);
    memcpy(parts.data(), buf, numParts * sizeof(int32_t));
    buf += numParts * sizeof(int32_t);
    std::vector<shp::Point2d> pnts(numPnts);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    shape->setBoundingBox2d(box);
    shape->setParts(parts);
    shape->setPoints2d(pnts);
    return true;
}

bool Reader::readPolyLineZ(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numParts = 0;
    int32_t numPnts = 0;
    std::vector<int32_t> parts;
    std::vector<shp::Point2d> pnts;
    shp::Range2d zrange;
    std::vector<double> zarr;
    shp::Range2d mrange;
    std::vector<double> marr;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numParts, buf, sizeof(numParts));
    buf += sizeof(numParts);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    parts.resize(numParts);
    pnts.resize(numPnts);
    marr.resize(numPnts);
    zarr.resize(numPnts);
    memcpy(parts.data(), buf, numParts * sizeof(int32_t));
    buf += numParts * sizeof(int32_t);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    buf += numPnts * sizeof(shp::Point2d);
    memcpy(zrange.data(), buf, sizeof(zrange));
    buf += sizeof(zrange);
    memcpy(zarr.data(), buf, numPnts * sizeof(double));
    buf += numPnts * sizeof(double);
    memcpy(mrange.data(), buf, sizeof(mrange));
    buf += sizeof(mrange);
    memcpy(marr.data(), buf, numParts * sizeof(double));
    shape->setParts(parts);
    shape->setPoints2d(pnts);
    shape->setZs(zarr);
    shape->setMeasurements(marr);
    shape->setZRange(zrange);
    shape->setMRange(mrange);
    shape->setBoundingBox2d(box);
    return true;
}

bool Reader::readPolyLineM(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numParts = 0;
    int32_t numPnts = 0;
    std::vector<int32_t> parts;
    std::vector<shp::Point2d> pnts;
    shp::Range2d mrange;
    std::vector<double> marr;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numParts, buf, sizeof(numParts));
    buf += sizeof(numParts);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    parts.resize(numParts);
    pnts.resize(numPnts);
    memcpy(parts.data(), buf, numParts * sizeof(int32_t));
    buf += numParts * sizeof(int32_t);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    buf += numPnts * sizeof(shp::Point2d);
    memcpy(mrange.data(), buf, sizeof(mrange));
    buf += sizeof(mrange);
    marr.resize(numPnts);
    memcpy(marr.data(), buf, numPnts * sizeof(double));
    shape->setBoundingBox2d(box);
    shape->setParts(parts);
    shape->setPoints2d(pnts);
    shape->setMeasurements(marr);
    shape->setMRange(mrange);
    return true;
}

bool Reader::readPolygon(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numParts = 0;
    int32_t numPnts = 0;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numParts, buf, sizeof(numParts));
    buf += sizeof(numParts);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    std::vector<int32_t> parts(numParts);
    memcpy(parts.data(), buf, numParts * sizeof(int32_t));
    buf += numParts * sizeof(int32_t);
    std::vector<shp::Point2d> pnts(numPnts);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    shape->setBoundingBox2d(box);
    shape->setParts(parts);
    shape->setPoints2d(pnts);
    return true;
}

bool Reader::readPolygonZ(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numParts = 0;
    int32_t numPnts = 0;
    std::vector<int32_t> parts;
    std::vector<shp::Point2d> pnts;
    shp::Range2d zrange;
    std::vector<double> zarr;
    shp::Range2d mrange;
    std::vector<double> marr;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numParts, buf, sizeof(numParts));
    buf += sizeof(numParts);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    parts.resize(numParts);
    pnts.resize(numPnts);
    marr.resize(numPnts);
    zarr.resize(numPnts);
    memcpy(parts.data(), buf, numParts * sizeof(int32_t));
    buf += numParts * sizeof(int32_t);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    buf += numPnts * sizeof(shp::Point2d);
    memcpy(zrange.data(), buf, sizeof(zrange));
    buf += sizeof(zrange);
    memcpy(zarr.data(), buf, numPnts * sizeof(double));
    buf += numPnts * sizeof(double);
    memcpy(mrange.data(), buf, sizeof(mrange));
    buf += sizeof(mrange);
    memcpy(marr.data(), buf, numParts * sizeof(double));
    shape->setParts(parts);
    shape->setPoints2d(pnts);
    shape->setZs(zarr);
    shape->setMeasurements(marr);
    shape->setZRange(zrange);
    shape->setMRange(mrange);
    shape->setBoundingBox2d(box);
    return true;
}

bool Reader::readPolygonM(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numParts = 0;
    int32_t numPnts = 0;
    std::vector<int32_t> parts;
    std::vector<shp::Point2d> pnts;
    shp::Range2d mrange;
    std::vector<double> marr;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numParts, buf, sizeof(numParts));
    buf += sizeof(numParts);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    parts.resize(numParts);
    pnts.resize(numPnts);
    memcpy(parts.data(), buf, numParts * sizeof(int32_t));
    buf += numParts * sizeof(int32_t);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    buf += numPnts * sizeof(shp::Point2d);
    memcpy(mrange.data(), buf, sizeof(mrange));
    buf += sizeof(mrange);
    marr.resize(numPnts);
    memcpy(marr.data(), buf, numPnts * sizeof(double));
    shape->setBoundingBox2d(box);
    shape->setParts(parts);
    shape->setPoints2d(pnts);
    shape->setMeasurements(marr);
    shape->setMRange(mrange);
    return true;
}

bool Reader::readMultiPoint(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    int32_t numPnts = 0;
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    uint64_t size = numPnts * sizeof(shp::Point2d);
    std::vector<shp::Point2d> pnts(numPnts);
    memcpy(pnts.data(), buf, size);
    shape->setPoints2d(pnts);
    shape->setBoundingBox2d(box);
    return true;
}

bool Reader::readMultiPointZ(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numPnts = 0;
    std::vector<shp::Point2d> pnts;
    shp::Range2d zrange;
    std::vector<double> zarr;
    shp::Range2d mrange;
    std::vector<double> marr;

    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    pnts.resize(numPnts);
    zarr.resize(numPnts);
    marr.resize(numPnts);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    buf += numPnts * sizeof(shp::Point2d);
    memcpy(zrange.data(), buf, sizeof(zrange));
    buf += sizeof(zrange);
    memcpy(zarr.data(), buf, numPnts * sizeof(double));
    buf += numPnts * sizeof(double);
    memcpy(mrange.data(), buf, sizeof(mrange));
    buf += sizeof(mrange);
    memcpy(marr.data(), buf, numPnts * sizeof(double));
    shape->setPoints2d(pnts);
    shape->setBoundingBox2d(box);
    shape->setZs(zarr);
    shape->setZRange(zrange);
    shape->setMeasurements(marr);
    shape->setMRange(mrange);
    return true;
}

bool Reader::readMultiPointM(const char* buf, shp::ShapePtr shape)
{
    shp::BoundingBox2d box;
    int32_t numPnts = 0;
    std::vector<shp::Point2d> pnts;
    shp::Range2d mrange;
    std::vector<double> marr;
    memcpy(box.data(), buf, sizeof(box));
    buf += sizeof(box);
    memcpy(&numPnts, buf, sizeof(numPnts));
    buf += sizeof(numPnts);
    pnts.resize(numPnts);
    marr.resize(numPnts);
    memcpy(pnts.data(), buf, numPnts * sizeof(shp::Point2d));
    buf += numPnts * sizeof(shp::Point2d);
    memcpy(mrange.data(), buf, sizeof(mrange));
    buf += sizeof(mrange);
    memcpy(marr.data(), buf, numPnts * sizeof(double));
    shape->setPoints2d(pnts);
    shape->setBoundingBox2d(box);
    shape->setMeasurements(marr);
    shape->setMRange(mrange);
    return true;
}

bool Reader::readMultiPatch(const char* buf, shp::ShapePtr shape)
{
    std::runtime_error("MultiPatch not supported");
    return false;
}
