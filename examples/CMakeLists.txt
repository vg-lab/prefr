IF (WITH_EXAMPLES)

  FILE(GLOB examples_SOURCES
  CShader.cpp
  FPS.cpp
  )
  
  set(PREFR_LIBRARY_BASE_PATH ${CMAKE_SOURCE_DIR}/prefr)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaderPath.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/shaderPath.h)
  
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  
  add_executable(defaultgl ${examples_SOURCES} gl.cpp)
  target_link_libraries(defaultgl prefr)
  
  IF( WITH_OPENSCENEGRAPH )

    add_executable(defaultosg osg.cpp)
    target_link_libraries(defaultosg prefr)
    
    add_executable(compositeosg compositeosg.cpp)
    target_link_libraries(compositeosg prefr)
    
    add_executable(animation animation.cpp)
    target_link_libraries(animation prefr)
    
    add_executable(electric electric.cpp)
    target_link_libraries(electric prefr)
  
  ENDIF()

  add_executable(profile ${examples_SOURCES} profile.cpp)
  target_link_libraries(profile prefr)
  
  IF (QT_FOUND)  
    add_executable(qtosg qtcompositeosg.cpp)
    target_link_libraries(qtosg ${QT_LIBRARIES} prefr)
  ENDIF()

ENDIF()