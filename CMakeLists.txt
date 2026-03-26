cmake_minimum_required(VERSION 3.28)

project(tineye)

set(PRIVATE_SOURCES
   ./src/tineye/tineye.cpp
)
set(PUBLIC_HEADERS
   ./include/tineye/tineye.h
)

add_library(libtineye_static STATIC)
add_library(libtineye_shared SHARED)

target_sources(libtineye_static
    PRIVATE
      ${PRIVATE_SOURCES}
    
    PUBLIC FILE_SET public_headers TYPE HEADERS
        BASE_DIRS include
        FILES ${PUBLIC_HEADERS}
)
target_sources(libtineye_shared
    PRIVATE
      ${PRIVATE_SOURCES}
    
    PUBLIC FILE_SET public_headers TYPE HEADERS
        BASE_DIRS include
        FILES ${PUBLIC_HEADERS}
)

add_executable(tineye 
   ${PRIVATE_SOURCES}
   ${PUBLIC_HEADERS}
   ./src/main.cpp
)

target_include_directories(libtineye_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") 
target_include_directories(libtineye_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") 
target_include_directories(tineye PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") 

target_link_libraries(tineye PRIVATE libtineye_static) 


install(TARGETS tineye libtineye_shared libtineye_static
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/tineye
    PUBLIC_HEADER DESTINATION include
)

