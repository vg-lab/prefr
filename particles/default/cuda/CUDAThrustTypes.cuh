/*
 * CUDATHRUSTTypes.h
 *
 *  Created on: 31/10/2014
 *      Author: sgalindo
 */

#ifndef CUDATHRUSTTYPES_H_
#define CUDATHRUSTTYPES_H_

#include "../../config.h"
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

using namespace std;

namespace particles
{
  namespace defaultParticleSystem
  {

    namespace CUDATHRUST
    {

      class CUDADistanceArray
      {
      public:

        thrust::host_vector<int> hostID;
        thrust::host_vector<float> hostDistances;

        thrust::device_vector<int> deviceID;
        thrust::device_vector<float> deviceDistances;

      };

      typedef CUDADistanceArray distanceArray;
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
#endif /* CUDATHRUSTTYPES_H_ */
