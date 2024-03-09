/*******************************************************************************
**     FileName: tiff.h
**    ClassName: tiff
**       Author: Geocat & LittleBottle
**  Create Time: 2023/07/13 00:00
**  Description:
*******************************************************************************/

#ifndef TIFF_H
#define TIFF_H

#include <cstdint>
#include <string>

namespace tif {

class tiff
{
public:
    tiff();
    tiff(const std::string& filename);
    ~tiff();

    bool open(const std::string& filename);
    bool close();
    bool isOpen() const;

    uint8_t bandCount() const;
    uint32_t width() const;
    uint32_t height() const;
    uint8_t bitsPerSample() const;
    uint8_t samplePerPixel() const;

protected:
};  // class tiff

}  // namespace tif

#endif  // TIFF_H