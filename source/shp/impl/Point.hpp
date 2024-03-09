#ifndef POINT_H
#define POINT_H

#include <shp/shp.h>

class Point : public shp::Shape
{
public:
    Point()
        : shp::Shape(shp::kPoint)
    {
    }

    virtual ~Point() {}

    uint64_t numPoints() const override { return 1; }

    std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<Point> clone = std::make_shared<Point>();
        clone->m_point = m_point;
        clone->m_type = m_type;
        clone->m_dirty = m_dirty;
        return clone;
    }

    std::vector<shp::Point2d> getPoints2d() const override
    {
        return std::vector<shp::Point2d>(1, m_point);
    }

    void setPoints2d(const std::vector<shp::Point2d>& points) override
    {
        if (points.empty())
            return;
        m_point = points[0];
    }

protected:
    shp::Point2d m_point;
};  // class Point

#endif  // POINT_H