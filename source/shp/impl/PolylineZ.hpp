#ifndef POLYLINEZ_H
#define POLYLINEZ_H

#include "PolylineM.hpp"

class PolylineZ : public PolylineM
{
public:
    PolylineZ() { m_type = shp::kPolyLineZ; }

    virtual ~PolylineZ() {}

    std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<PolylineZ> clone = std::make_shared<PolylineZ>();
        clone->m_points = m_points;
        clone->m_parts = m_parts;
        clone->m_box = m_box;
        clone->m_type = m_type;
        clone->m_dirty = m_dirty;
        clone->m_zs = m_zs;
        clone->m_zRange = m_zRange;
        return clone;
    }

    shp::Range2d getZRange() const override { return m_zRange; }
    void setZRange(const shp::Range2d& zRange) override { m_zRange = zRange; }

    std::vector<double> getZs() const override { return m_zs; }

    void setZs(const std::vector<double>& zs) override { m_zs = zs; }

protected:
    std::vector<double> m_zs;
    shp::Range2d m_zRange;
};  // class PolylineZ

#endif  // POLYLINEZ_H