#ifndef POINTZ_H
#define POINTZ_H

#include "PointM.hpp"

#include <memory>

class PointZ : public PointM
{
public:
    PointZ() { m_type = shp::kPointZ; }

    virtual ~PointZ() {}

    std::shared_ptr<Shape> clone() const override
    {
        std::shared_ptr<PointZ> clone = std::make_shared<PointZ>();
        clone->m_point = m_point;
        clone->m_type = m_type;
        clone->m_dirty = m_dirty;
        clone->m_z = m_z;
        return clone;
    }

    std::vector<double> getZs() const override
    {
        return std::vector<double>(1, m_z);
    }

    void setZs(const std::vector<double>& zs) override
    {
        if (zs.empty())
            return;
        m_z = zs[0];
    }

protected:
    double m_z;
};  // class PointZ

#endif  // POINTZ_H