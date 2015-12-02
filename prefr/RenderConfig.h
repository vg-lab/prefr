/*
 * RenderConfig.h
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
 */

#ifndef RENDERCONFIG_H_
#define RENDERCONFIG_H_

#ifndef PREFR_SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif

#include <vector>

namespace prefr
{

  class RenderConfig
  {
  public:

    // Triangles vertices
    std::vector<GLfloat>* billboardVertices;
    std::vector<GLfloat>* particlePositions;
    std::vector<GLfloat>* particleColors;

    // OpenGL pointers
    GLuint vao;
    GLuint vboBillboardVertex;
    GLuint vboParticlesPositions;
    GLuint vboParticlesColors;

    RenderConfig(unsigned int size)
    : billboardVertices( new std::vector<GLfloat>( size ) )
    , particlePositions( new std::vector<GLfloat>( size ) )
    , particleColors( new std::vector<GLfloat>( size ))
    , vao( 0 )
    , vboBillboardVertex( 0 )
    , vboParticlesPositions( 0 )
    , vboParticlesColors( 0 )
    {}

    virtual ~RenderConfig()
    {
      delete( billboardVertices );
      delete( particlePositions );
      delete( particleColors );
      vao = 0;
      vboBillboardVertex = 0;
      vboParticlesPositions = 0;
      vboParticlesColors = 0;
    }

  };



}


#endif /* RENDERCONFIG_H_ */
