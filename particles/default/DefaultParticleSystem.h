/*
 * GLParticleSystem.h
 *
 *  Created on: 13/10/2014
 *      Author: sgalindo
 */

#ifndef GLPARTICLESYSTEM_H_
#define GLPARTICLESYSTEM_H_

#include "../ParticleSystem.h"

using namespace std;
using namespace particles;

namespace particles
{
  namespace defaultParticleSystem
  {

    class SortUnit
    {
    public:
      int idx;
      float distance;

      static bool sortDescending (const SortUnit& lhs, const SortUnit& rhs){return lhs.distance > rhs.distance;}
      static bool sortAscending (const SortUnit& lhs, const SortUnit& rhs){return lhs.distance < rhs.distance;}
    };

    typedef vector<SortUnit> distanceArray;
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

    class DefaultGLParticleSystem : public ParticleSystem
    {
    public:

      distanceArray* distances;

      RenderConfig* renderConfig;

      bool updateLoopUnified;

      DefaultGLParticleSystem(int initialParticlesNumber, int _maxParticles, float _emissionRate
                   , bool _loop = true);



      virtual void Start();
      virtual void Update(float deltaTime);
      virtual void UpdateUnified(float deltaTime);
      virtual void UpdateSeparated(float deltaTime);
      virtual void UpdateRender();
      virtual void Render();
    };

  }
}
#endif /* GLPARTICLESYSTEM_H_ */
