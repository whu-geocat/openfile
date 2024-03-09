#ifndef POLYLINEM_H
#define POLYLINEM_H

#include "Polyline.hpp"

class PolylineM : public Polyline
{
public:
    PolylineM() { m_type = shp::kPolyLineM; }

    ~PolylineM() {}

    std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<PolylineM> clone = std::make_shared<PolylineM>();
        clone->m_points = m_points;
        clone->m_parts = m_parts;
        clone->m_box = m_box;
        clone->m_type = m_type;
        clone->m_dirty = m_dirty;
        clone->m_measurements = m_measurements;
        clone->m_mRange = m_mRange;
        return clone;
    }

    shp::Range2d getMRange() const override { return m_mRange; }
    void setMRange(const shp::Range2d& mRange) override { m_mRange = mRange; }

    std::vector<double> getMeasurements() const override
    {
        return m_measurements;
    }

    void setMeasurements(const std::vector<double>& measurements) override
    {
        m_measurements = measurements;
    }

protected:
    std::vector<double> m_measurements;
    shp::Range2d m_mRange;
};  // class PolylineM

#endif  // POLYLINEM_H