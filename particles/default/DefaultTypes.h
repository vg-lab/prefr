/*
 * DefaultTypes.h
 *
 *  Created on: 15/10/2014
 *      Author: sgalindo
 */

#ifndef DEFAULTTYPES_H_
#define DEFAULTTYPES_H_

using namespace std;

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

  }
}
#endif /* DEFAULTTYPES_H_ */
