cmake_minimum_required(VERSION 3.28)

project(tineye)

set(PRIVATE_SOURCES
   ./src/tineye/tineye.cpp
   ./src/tineye/thread_pool.cpp
)
set(PUBLIC_HEADERS
   ./include/tineye/tineye.h
   ./include/tineye/thread_pool.h
)

add_library(tineye_static STATIC)
add_library(tineye_shared SHARED)

target_sources(tineye_static
    PRIVATE
      ${PRIVATE_SOURCES}
    
    PUBLIC FILE_SET public_headers TYPE HEADERS
        BASE_DIRS include
        FILES ${PUBLIC_HEADERS}
)
target_sources(tineye_shared
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

set_property(TARGET tineye PROPERTY CXX_STANDARD 20)
set_property(TARGET tineye_static PROPERTY CXX_STANDARD 20)
set_property(TARGET tineye_shared PROPERTY CXX_STANDARD 20)

target_include_directories(tineye_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") 
target_include_directories(tineye_shared PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") 
target_include_directories(tineye PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include") 

target_link_libraries(tineye PRIVATE tineye_static) 

install(TARGETS tineye tineye_shared tineye_static
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
    LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/lib/tineye
    PUBLIC_HEADER DESTINATION ${CMAKE_SOURCE_DIR}/lib/include
)

