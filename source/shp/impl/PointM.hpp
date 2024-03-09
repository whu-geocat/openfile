#ifndef POINTM_H
#define POINTM_H

#include "Point.hpp"

class PointM : public Point
{
public:
    PointM() { m_type = shp::kPointM; }

    virtual ~PointM() {}

    std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<PointM> clone = std::make_shared<PointM>();
        clone->m_point = m_point;
        clone->m_type = m_type;
        clone->m_dirty = m_dirty;
        clone->m_m = m_m;
        return clone;
    }

    std::vector<double> getMeasurements() const override
    {
        return std::vector<double>(1, m_m);
    }

    void setMeasurements(const std::vector<double>& measurements) override
    {
        if (measurements.empty())
            return;
        m_m = measurements[0];
    }

protected:
    double m_m;
};  // class PointM

#endif  // POINTM_H