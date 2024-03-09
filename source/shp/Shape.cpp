#include "impl.h"

#include <cstdint>
#include <memory>
#include <shp/shp.h>
#include <stdexcept>
#include <vector>

namespace shp {

Shape::Shape(ShapeType type)
    : m_type(type)
{
}

Shape::~Shape() {}

std::shared_ptr<Shape> Shape::CreateShape(ShapeType type)
{
    switch (type)
    {
    case kNullShape :
        return nullptr;
    case kPoint :
        return std::make_shared<Point>();
        break;
    case kPolyLine :
        return std::make_shared<Polyline>();
        break;
    case kPolygon :
        return std::make_shared<Polygon>();
        break;
    case kMultiPoint :
        return std::make_shared<MultiPoint>();
        break;
    case kPointZ :
        return std::make_shared<PointZ>();
        break;
    case kPolyLineZ :
        return std::make_shared<PolylineZ>();
        break;
    case kPolygonZ :
        return std::make_shared<PolygonZ>();
        break;
    case kMultiPointZ :
        return std::make_shared<MultiPointZ>();
        break;
    case kPointM :
        return std::make_shared<PointM>();
        break;
    case kPolyLineM :
        return std::make_shared<PolylineM>();
        break;
    case kPolygonM :
        return std::make_shared<PolygonM>();
        break;
    case kMultiPointM :
        return std::make_shared<MultiPointM>();
        break;
    case kMultiPatch :
        std::runtime_error("Not implemented");
        break;
    }
    return nullptr;
}

ShapeType Shape::getType() const { return m_type; }

uint64_t Shape::numPoints() const { return 0; }

bool Shape::dirty() const { return m_dirty; }

std::shared_ptr<Shape> Shape::clone() const { return nullptr; }

// for polyline and polygon
uint64_t Shape::numParts() const { return 0; }

std::vector<int32_t> Shape::getParts() const { return std::vector<int32_t>(); }

void Shape::setParts(const std::vector<int32_t>& parts) {}

BoundingBox2d Shape::getBoundingBox2d() const { return BoundingBox2d(); }

void Shape::setBoundingBox2d(const BoundingBox2d& bbox) {}

Range2d Shape::getMRange() const { return Range2d(); }

void Shape::setMRange(const Range2d& range) {}

Range2d Shape::getZRange() const { return Range2d(); }

void Shape::setZRange(const Range2d& range) {}

// for points and multipoints
std::vector<Point2d> Shape::getPoints2d() const
{
    return std::vector<Point2d>();
}

void Shape::setPoints2d(const std::vector<Point2d>& points) {}

std::vector<double> Shape::getMeasurements() const
{
    return std::vector<double>();
}

void Shape::setMeasurements(const std::vector<double>& measurements) {}

std::vector<double> Shape::getZs() const { return std::vector<double>(); }

void Shape::setZs(const std::vector<double>& zs) {}

}  // namespace shp