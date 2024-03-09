#ifndef POLYLINE_H
#define POLYLINE_H

#include <cstdint>
#include <shp/shp.h>

class Polyline : public shp::Shape
{
public:
    Polyline()
        : shp::Shape(shp::kPolyLine)
    {
    }

    virtual ~Polyline() {}

    uint64_t numPoints() const override { return m_points.size(); }

    virtual std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<Polyline> clone = std::make_shared<Polyline>();
        clone->m_points = m_points;
        clone->m_parts = m_parts;
        clone->m_box = m_box;
        clone->m_type = m_type;
        clone->m_dirty = m_dirty;
        return clone;
    }

    uint64_t numParts() const override { return m_parts.size(); }

    std::vector<int32_t> getParts() const override { return m_parts; }

    void setParts(const std::vector<int32_t>& parts) override
    {
        m_parts = parts;
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
    std::vector<int32_t> m_parts;
    shp::BoundingBox2d m_box;
};  // class Polyline

#endif  // POLYLINE_H