/*
 * GLTypes.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef GLTYPES_H_
#define GLTYPES_H_

#include "../DefaultTypes.h"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace GL
    {
      typedef DistanceArray distanceArray;
      typedef vector<GLfloat> glvectorf;
      typedef vector<GLchar> glvectorch;

      class RenderConfig
      {
      public:

        // Triangles vertices
        glvectorf* billboardVertices;
        glvectorf* particlePositions;
        glvectorch* particleColors;

        // OpenGL pointers
        GLuint vao;
        GLuint vboBillboardVertex;
        GLuint vboParticlesPositions;
        GLbyte vboParticlesColor;

      };
    }
  }
}

#endif /* GLTYPES_H_ */
