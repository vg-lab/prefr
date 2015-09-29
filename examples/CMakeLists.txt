IF (WITH_EXAMPLES)

  FILE(GLOB examples_SOURCES
  CShader.cpp
  FPS.cpp
  )
  
  set(PREFR_LIBRARY_BASE_PATH ${CMAKE_SOURCE_DIR}/prefr)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/shaderPath.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/shaderPath.h)
  
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  
  SET(PREFR_LINK_LIBRARIES
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLUT_LIBRARIES}
  )

  add_executable(defaultgl ${examples_SOURCES} gl.cpp)
  target_link_libraries(defaultgl ${PREFR_LINK_LIBRARIES} prefr)
  
  IF( WITH_OPENSCENEGRAPH )

    SET(PREFR_LINK_LIBRARIES
      ${PREFR_LINK_LIBRARIES}
      ${OPENSCENEGRAPH_LIBRARIES}
    )

    add_executable(defaultosg osg.cpp)
    target_link_libraries(defaultosg ${PREFR_LINK_LIBRARIES} prefr)
    
    add_executable(compositeosg compositeosg.cpp)
    target_link_libraries(compositeosg ${PREFR_LINK_LIBRARIES} prefr)
    
    add_executable(animation animation.cpp)
    target_link_libraries(animation ${PREFR_LINK_LIBRARIES} prefr)
    
    add_executable(electric electric.cpp)
    target_link_libraries(electric ${PREFR_LINK_LIBRARIES} prefr)
  
  ENDIF()

  add_executable(profile ${examples_SOURCES} profile.cpp)
  target_link_libraries(profile ${PREFR_LINK_LIBRARIES} prefr)
  
  IF (QT_FOUND)  
    SET(PREFR_LINK_LIBRARIES
      ${PREFR_LINK_LIBRARIES}
      ${QT_LIBRARIES}
    )
    add_executable(qtosg qtcompositeosg.cpp)
    target_link_libraries(qtosg ${PREFR_LINK_LIBRARIES} prefr)
  ENDIF()

ENDIF()