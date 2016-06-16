if( PREFR_WITH_EXAMPLES )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaderPath.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/shaderPath.h )

  include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

  if ( GLUT_FOUND )
    set( examples_SOURCES
      CShader.cpp
      FPS.cpp )

    set( PREFRDEFAULTGL_HEADERS )
    set( PREFRDEFAULTGL_SOURCES ${examples_SOURCES} gl.cpp )
    set( PREFRDEFAULTGL_LINK_LIBRARIES PReFr )
    common_application( prefrDefaultGL )

    set( PREFRPROFILEGL_HEADERS )
    set( PREFRPROFILEGL_SOURCES ${examples_SOURCES} profile.cpp )
    set( PREFRPROFILEGL_LINK_LIBRARIES PReFr )
    common_application( prefrProfileGL )

  endif( GLUT_FOUND )

  if( PREFR_WITH_OPENSCENEGRAPH AND OPENSCENEGRAPH_FOUND )

    #TODO: fix build and change to common app
    add_executable(prefrOSG osg.cpp)
    target_link_libraries(prefrOSG PReFr)

#    add_executable(compositeosg compositeosg.cpp)
#    target_link_libraries(compositeosg prefr)

#    add_executable(animation animation.cpp)
#    target_link_libraries(animation prefr)

#    add_executable(electric electric.cpp)
#    target_link_libraries(electric prefr)

  endif( PREFR_WITH_OPENSCENEGRAPH AND OPENSCENEGRAPH_FOUND)


  if (QT_FOUND)
    #TODO: fix build and change to common app
#    add_executable(qtosg qtcompositeosg.cpp)
#    target_link_libraries(qtosg ${QT_LIBRARIES} prefr)
  endif()

endif()
