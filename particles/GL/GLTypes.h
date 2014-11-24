/*
 * GLTypes.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef GLTYPES_H_
#define GLTYPES_H_

#include <GL/glew.h>

#include <vector>

using namespace std;

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace GL
    {
      class SortUnit
      {
      public:
        int idx;
        float distance;

        static bool sortDescending (const SortUnit& lhs, const SortUnit& rhs){return lhs.distance > rhs.distance;}
        static bool sortAscending (const SortUnit& lhs, const SortUnit& rhs){return lhs.distance < rhs.distance;}
      };


      class DistanceUnit
      {
      public:
        int* id;
        float* distance;

        DistanceUnit(void):id(nullptr), distance(nullptr){}
        DistanceUnit(int* id, float* distance) : id(id), distance(distance){}
        int& Id(void){return (int&)*id;}
        float& Distance(void){return (float&)*distance;}

        const int& getID(void){return (const int&)*id;}
        const float& getDistance(void){return (const float&)*distance;}
      };

      typedef DistanceUnit tdunit;
      typedef vector<tdunit> tdcontainter;

      class DistanceArray
      {
      public:
        vector<int>* ids;
        vector<float>* distances;
        tdcontainter* elements;

        DistanceArray(unsigned int size)
        {
          ids = new vector<int>(size);
          distances = new vector<float>(size);
          elements = new tdcontainter(size);

          for (unsigned int i = 0; i < size; i++)
          {
            elements->at(i) = tdunit(&ids->at(i), &distances->at(i));
          }
        }

        vector<DistanceUnit>::iterator begin()
        {
          return elements->begin();
        }

        vector<DistanceUnit>::iterator end()
        {
          return elements->end();
        }

        DistanceUnit& at(unsigned int i)
        {
          return elements->at(i);
        }

        DistanceUnit& operator[](unsigned int i)
        {
          return elements->at(i);
        }

        static bool sortDescending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance > *rhs.distance;}
        static bool sortAscending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance < *rhs.distance;}
      };


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
        GLuint vboParticlesColor;

      };
    }
  }
}

#endif /* GLTYPES_H_ */
