project(json)

option(ENABLE_SHARED_LIBS "Enable shared libs" ON)
option(ENABLE_STATIC_LIBS "Enable static libs" OFF)

set(LIB_TYPE)
if(ENABLE_SHARED_LIBS)
    set(LIB_TYPE SHARED)
elseif(ENABLE_STATIC_LIBS)
    set(LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME}  
    ${LIB_TYPE}
        json.cpp
        JsonObject.cpp
        JsonReader.cpp
        JsonReader.h
        JsonWriter.h
)

set_target_properties(${PROJECT_NAME}  
    PROPERTIES
        PREFIX ""
        SUFFIX ""
)

set(export_file_path "${CMAKE_BINARY_DIR}/include/json/json_export.h")
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
    EXPORT_FILE_NAME ${export_file_path}
)

target_include_directories(${PROJECT_NAME}  
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)

