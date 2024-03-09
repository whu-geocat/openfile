/*******************************************************************************
**     FileName: shp.h
**    ClassName: shp
**       Author: Geocat & LittleBottle
**  Create Time: 2023/07/12 12:43
**  Description:
*******************************************************************************/

#ifndef SHP_H
#define SHP_H

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class ShpRecord;  // internal interface

namespace shp {

enum ShapeType
{
    kNullShape = 0,
    kPoint = 1,
    kPolyLine = 3,
    kPolygon = 5,
    kMultiPoint = 8,
    kPointZ = 11,
    kPolyLineZ = 13,
    kPolygonZ = 15,
    kMultiPointZ = 18,
    kPointM = 21,
    kPolyLineM = 23,
    kPolygonM = 25,
    kMultiPointM = 28,
    kMultiPatch = 31
};

struct ShpHeader;
class Shape;
using ShapePtr = std::shared_ptr<Shape>;

using Point3d = std::array<double, 3>;
using Point3f = std::array<float, 3>;
using Point2d = std::array<double, 2>;
using Point2f = std::array<float, 2>;
using BoundingBox2d = std::array<Point2d, 2>;
using BoundingBox3d = std::array<Point3d, 2>;
using Range2d = std::array<double, 2>;

template <int>
bool IsBigEndian()
{
    constexpr union
    {
        uint16_t val;
        uint8_t bytes;
    } val = { 0x0102 };
    return val.bytes != 0x02;
}

template <typename T>
T SwapEndian(const T& value)
{
    T result(0);
    constexpr size_t bytes = sizeof(T);
    constexpr size_t traits = 0xff;
    for (size_t i = 0; i < bytes; ++i)
    {
        result |= (value & (traits << i)) << (bytes - i - 1);
    }
    return result;
}

class shp
{
public:
    shp();
    ~shp();

    enum Error
    {
        kErrorNone,
        kDataDirty,
        kErrorRead,
        kErrorWrite,
        kIndexOutOfRange
    };

    Error open(const std::string& filename);
    Error save(const std::string& filename);
    Error close();

    uint64_t getNumShapes() const;
    Error getShapeAt(uint64_t index, ShapePtr shape);
    std::vector<ShapePtr> getAllShapes() const;
    std::vector<ShapePtr>& getAllShapes();
    Error setShapeAt(uint64_t index, ShapePtr);

    const ShpHeader& getHeader() const;
    ShpHeader& getHeader();
    Error setHeader(const ShpHeader& header);

    class ShpInternal;

protected:  // internal interfaces
    std::shared_ptr<ShpInternal> m_internal;
};  // class shp

struct ShpHeader
{
    int32_t fileCode;        // big
    int32_t unused[5];       // big
    int32_t fileLength;      // big
    int32_t version = 1000;  // little
    int32_t shapeType;       // little
    double rangeXY[4];       // little, xmin, ymin, xmax, ymax
    double rangeZM[4];       // little, zmin, zmax, mmin, mmax
};

/**
 * @brief This class is the shape pulic interface, all concrete shapes
 * are derived from this class.
 * Shapes cannot be copied, only moved.
 */
class Shape
{
    Shape(const Shape&) = delete;
    Shape& operator= (const Shape&) = delete;

protected:
    Shape(ShapeType type);

public:
    enum ShapeFlag
    {
        kHasZ = 0x0001,
        kHasM = 0x0002,
        kHasZM = 0x0004,
        kIsRing = 0x0008,
        kIs3D = 0x0020,
        kIsMBRValid = 0x0040,
    };
    virtual ~Shape() = 0;

    static ShapePtr CreateShape(ShapeType type);

    ShapeType getType() const;
    bool dirty() const;

public:  // the public interfaces
    virtual uint64_t numPoints() const;
    virtual ShapePtr clone() const;
    // for polyline and polygon
    virtual uint64_t numParts() const;
    virtual std::vector<int32_t> getParts() const;
    virtual void setParts(const std::vector<int32_t>& parts);

    virtual BoundingBox2d getBoundingBox2d() const;
    virtual void setBoundingBox2d(const BoundingBox2d& bbox);
    virtual Range2d getMRange() const;
    virtual void setMRange(const Range2d& range);
    virtual Range2d getZRange() const;
    virtual void setZRange(const Range2d& range);

    // for points and multipoints
    virtual std::vector<Point2d> getPoints2d() const;
    virtual void setPoints2d(const std::vector<Point2d>& points);

    virtual std::vector<double> getMeasurements() const;
    virtual void setMeasurements(const std::vector<double>& measurements);
    virtual std::vector<double> getZs() const;
    virtual void setZs(const std::vector<double>& zs);

protected:
    ShapeType m_type;
    bool m_dirty;
};

}  // namespace shp

#endif  // SHP_H