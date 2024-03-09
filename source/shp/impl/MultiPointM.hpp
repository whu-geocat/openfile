#ifndef MULTIPOINTM_H
#define MULTIPOINTM_H

#include "MultiPoint.hpp"

#include <vector>

class MultiPointM : public MultiPoint
{
public:
    MultiPointM() { m_type = shp::kMultiPointM; }

    virtual ~MultiPointM() {}

    std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<MultiPointM> clone = std::make_shared<MultiPointM>();
        clone->m_points = m_points;
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
};  // class MultiPointM

#endif  // MULTIPOINTM_H