#ifndef MULTIPOINT_H
#define MULTIPOINT_H

#include <memory>
#include <shp/shp.h>
#include <vector>

class MultiPoint : public shp::Shape
{
public:
    MultiPoint()
        : shp::Shape(shp::kMultiPoint)
    {
    }
    virtual ~MultiPoint() {}

    uint64_t numPoints() const override { return m_points.size(); }

    virtual std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<MultiPoint> clone = std::make_shared<MultiPoint>();
        clone->m_points = m_points;
        clone->m_box = m_box;
        clone->m_type = m_type;
        clone->m_dirty = m_dirty;
        return clone;
    }

    shp::BoundingBox2d getBoundingBox2d() const override { return m_box; }
    void setBoundingBox2d(const shp::BoundingBox2d& bbox) override
    {
        m_box = bbox;
    }

    std::vector<shp::Point2d> getPoints2d() const override { return m_points; }

    void setPoints2d(const std::vector<shp::Point2d>& points) override
    {
        m_points = points;
    }

protected:
    std::vector<shp::Point2d> m_points;
    shp::BoundingBox2d m_box;
};  // class MultiPoint

#endif  // MULTIPOINT_H