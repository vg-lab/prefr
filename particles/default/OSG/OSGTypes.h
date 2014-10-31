/*
 * OSGTypes.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef OSGTYPES_H_
#define OSGTYPES_H_

#include "../DefaultTypes.h"

#include <osg/Array>

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {
      typedef DistanceArray distanceArray;
      typedef vector<GLfloat> glvectorf;
      typedef vector<GLchar> glvectorch;

      class RenderConfig
      {
      public:

        // Triangles vertices
        osg::Vec3Array* billboardVertices;
        osg::Vec4Array* particlePositions;
        osg::Vec4Array* particleColors;

        // OpenGL pointers
        GLuint vao;
        GLuint vboBillboardVertex;
        GLuint vboParticlesPositions;
        GLbyte vboParticlesColor;

      };

    }
  }
}

#endif /* OSGTYPES_H_ */
