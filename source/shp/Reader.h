/*******************************************************************************
**     FileName: Reader.h
**    ClassName: Reader
**       Author: Geocat & LittleBottle
**  Create Time: 2024/03/08 17:26
**  Description:
*******************************************************************************/

#ifndef READER_H
#define READER_H

#include "ShpInternal.h"
#include "ShpRecord.h"

#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

class MiniBuffer;

class Reader
{
public:
    Reader();
    ~Reader();

    bool read(const std::string& filename, shp::shp::ShpInternal& shp);

protected:
    bool readHeader(std::ifstream& file, shp::ShpHeader& shp);
    bool readRecords(std::ifstream& file, std::vector<ShpRecord>& recs);
    bool ReadRecord(std::ifstream& file, ShpRecord& rec);

private:
    bool readPoint(const char* buf, shp::ShapePtr shape);
    bool readPointZ(const char* buf, shp::ShapePtr shape);
    bool readPointM(const char* buf, shp::ShapePtr shape);
    bool readPolyLine(const char* buf, shp::ShapePtr shape);
    bool readPolyLineZ(const char* buf, shp::ShapePtr shape);
    bool readPolyLineM(const char* buf, shp::ShapePtr shape);
    bool readPolygon(const char* buf, shp::ShapePtr shape);
    bool readPolygonZ(const char* buf, shp::ShapePtr shape);
    bool readPolygonM(const char* buf, shp::ShapePtr shape);
    bool readMultiPoint(const char* buf, shp::ShapePtr shape);
    bool readMultiPointZ(const char* buf, shp::ShapePtr shape);
    bool readMultiPointM(const char* buf, shp::ShapePtr shape);
    bool readMultiPatch(const char* buf, shp::ShapePtr shape);

protected:
    std::shared_ptr<MiniBuffer> m_buffer;
    std::map<shp::ShapeType, std::function<bool(const char*, shp::ShapePtr)>>
        m_readers;
};  // class Reader

#endif  // READER_H