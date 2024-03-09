cmake_minimum_required(VERSION 3.10)

project(shp)

option(BUILD_SHARED_LIBS "Build shp as a shared library" ON)
option(BUILD_STATIC_LIBS "Build shp as a static library" OFF)

set(LIB_TYPE)
if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
elseif(BUILD_STATIC_LIBS)
    set(LIB_TYPE STATIC)
endif()

set(SHP_HEADER
    ${CMAKE_CURRENT_LIST_DIR}/../../include/shp/shp.h
)

set(SHP_SOURCES
    impl/MultiPoint.hpp
    impl/MultiPointM.hpp
    impl/MultiPointZ.hpp
    impl/Point.hpp
    impl/PointM.hpp
    impl/PointZ.hpp
    impl/Polygon.hpp
    impl/PolygonM.hpp
    impl/PolygonZ.hpp
    impl/Polyline.hpp
    impl/PolylineM.hpp
    impl/PolylineZ.hpp

    impl.h
    Reader.cpp
    Reader.h
    Shape.cpp
    shp.cpp
    ShpInternal.cpp
    ShpInternal.h
    ShpRecord.cpp
    ShpRecord.h
    Writer.cpp
    Writer.h
)

add_library(shp ${LIB_TYPE} 
    ${SHP_HEADER}
    ${SHP_SOURCES}
)

include(GenerateExportHeader)
generate_export_header(shp
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/shp/shp_export.h
)

target_include_directories(shp PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../../include
    ${CMAKE_BINARY_DIR}/include
)