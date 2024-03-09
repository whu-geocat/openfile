/*******************************************************************************
**     FileName: Writer.h
**    ClassName: Writer
**       Author: Geocat & LittleBottle
**  Create Time: 2024/03/08 17:29
**  Description:
*******************************************************************************/

#ifndef WRITER_H
#define WRITER_H

#include "ShpInternal.h"
#include "ShpRecord.h"

#include <shp/shp.h>
#include <string>

class Writer
{
public:
    Writer();
    ~Writer();

    bool write(const std::string& filename, const shp::shp::ShpInternal& shp);

protected:
    bool writeHeader(std::ofstream& file, const shp::ShpHeader& shp);
    bool writeRecords(std::ofstream& file, const std::vector<ShpRecord>& recs);
    bool WriteRecord(std::ofstream& file, const ShpRecord& rec);

private:
    bool writePoint(std::ofstream& file, const shp::ShapePtr shape);
    bool writePointZ(std::ofstream& file, const shp::ShapePtr shape);
    bool writePointM(std::ofstream& file, const shp::ShapePtr shape);
    bool writePolyLine(std::ofstream& file, const shp::ShapePtr shape);
    bool writePolyLineZ(std::ofstream& file, const shp::ShapePtr shape);
    bool writePolyLineM(std::ofstream& file, const shp::ShapePtr shape);
    bool writePolygon(std::ofstream& file, const shp::ShapePtr shape);
    bool writePolygonZ(std::ofstream& file, const shp::ShapePtr shape);
    bool writePolygonM(std::ofstream& file, const shp::ShapePtr shape);
    bool writeMultiPoint(std::ofstream& file, const shp::ShapePtr shape);
    bool writeMultiPointZ(std::ofstream& file, const shp::ShapePtr shape);
    bool writeMultiPointM(std::ofstream& file, const shp::ShapePtr shape);
    bool writeMultiPatch(std::ofstream& file, const shp::ShapePtr shape);
};  // class Writer

#endif  // WRITER_H