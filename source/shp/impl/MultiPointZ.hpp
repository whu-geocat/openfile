#ifndef MULTIPOINTZ_H
#define MULTIPOINTZ_H

#include "MultiPointM.hpp"

#include <vector>

class MultiPointZ : public MultiPointM
{
public:
    MultiPointZ() { m_type = shp::kMultiPointZ; }

    ~MultiPointZ() {}

    std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<MultiPointZ> clone = std::make_shared<MultiPointZ>();
        clone->m_points = m_points;
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
};  // class MultiPointM

#endif  // MULTIPOINTZ_H