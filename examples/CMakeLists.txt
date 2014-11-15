

add_executable(default default.cpp)
target_link_libraries(default particles)

add_executable(defaultosg osg.cpp)
target_link_libraries(defaultosg particles)