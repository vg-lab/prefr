
FILE(GLOB examples_SOURCES
CShader.cpp
)

add_executable(default ${examples_SOURCES} default.cpp)
target_link_libraries(default particles)

add_executable(defaultosg osg.cpp)
target_link_libraries(defaultosg particles)