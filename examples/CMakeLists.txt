if( PREFR_WITH_EXAMPLES )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/shaderPath.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/shaderPath.h )

  include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

  if ( GLUT_FOUND )

  set( PREFROGL_HEADERS )
  set( PREFROGL_SOURCES ogl.cpp )
  set( PREFROGL_LINK_LIBRARIES prefr )
  common_application( prefrOGL )

  set( PREFRPICKOGL_HEADERS )
  set( PREFRPICKOGL_SOURCES oglPick.cpp )
  set( PREFRPICKOGL_LINK_LIBRARIES prefr )
  common_application( prefrPickOGL )

  endif( GLUT_FOUND )

  add_definitions(-DGL_SILENCE_DEPRECATION)

  set( PREFRCOLLECTION_HEADERS )
  set( PREFRCOLLECTION_SOURCES collection.cpp )
  set( PREFRCOLLECTION_LINK_LIBRARIES prefr )
  common_application( prefrCollection )

  if ( QT5OPENGL_FOUND )
    set( PREFRQT_HEADERS )
    set( PREFRQT_SOURCES qt.cpp )
    set( PREFRQT_LINK_LIBRARIES prefr
      Qt5::OpenGL
      )
    common_application( prefrQt )
  endif( )

endif()
