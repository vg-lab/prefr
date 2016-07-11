if( PREFR_WITH_EXAMPLES )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaderPath.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/shaderPath.h )

  include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

  if ( GLUT_FOUND )

    set( PREFROGL_HEADERS )
    set( PREFROGL_SOURCES ogl.cpp )
    set( PREFROGL_LINK_LIBRARIES PReFr )
    common_application( prefrOGL )

  endif( GLUT_FOUND )

  if( PREFR_WITH_OPENSCENEGRAPH AND OPENSCENEGRAPH_FOUND )

    set( PREFROSG_HEADERS )
    set( PREFROSG_SOURCES osg.cpp )
    set( PREFROSG_LINK_LIBRARIES PReFr )
    common_application( prefrOSG )

  endif( PREFR_WITH_OPENSCENEGRAPH AND OPENSCENEGRAPH_FOUND)

endif()
