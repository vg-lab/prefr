FILE(GLOB examples_SOURCES
CShader.cpp
)

set(PREFR_LIBRARY_BASE_PATH ${CMAKE_SOURCE_DIR}/prefr)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaderPath.h.in
${CMAKE_CURRENT_BINARY_DIR}/shaderPath.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(default ${examples_SOURCES} default.cpp)
target_link_libraries(default ${PREFR_LINK_LIBRARIES} prefr)

add_executable(defaultosg osg.cpp)
target_link_libraries(defaultosg ${PREFR_LINK_LIBRARIES} prefr)
