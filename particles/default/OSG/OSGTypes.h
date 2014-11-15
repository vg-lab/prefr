/*
 * OSGTypes.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef OSGTYPES_H_
#define OSGTYPES_H_

#include <GL/glew.h>
#include <GL/gl.h>

#include "../DefaultTypes.h"


#include <osg/Array>
#include <osg/Drawable>

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {
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
        osg::Vec3Array* billboardVertices;
        osg::DrawElementsUByte* billboardIndices;
        glvectorf* particlePositions;
        glvectorf* particleColors;

        // OpenGL pointers
        GLuint vao;
        GLuint vboBillboardVertex;
        GLuint vboParticlesPositions;
        GLuint vboParticlesColor;

        osg::Uniform* uCameraUp;
        osg::Uniform* uCameraRight;

        osg::Vec3f eye, center, up, right;

        osg::BoundingBox boundingBox;

        bool init;

        RenderConfig()
        : billboardVertices( nullptr )
        , billboardIndices( nullptr )
        , particlePositions( nullptr )
        , particleColors( nullptr )
        , vao( 0 )
        , vboBillboardVertex( 0 )
        , vboParticlesPositions( 0 )
        , vboParticlesColor( 0 )
        , uCameraUp( nullptr )
        , uCameraRight( nullptr )
        , init( false )
        {}

        ~RenderConfig()
        {
          glDeleteBuffers(1, &vboBillboardVertex);
          glDeleteBuffers(1, &vboParticlesPositions);
          glDeleteBuffers(1, &vboParticlesColor);
          glDeleteVertexArrays(1, &vao);
        }

      };

    }
  }
}

#endif /* OSGTYPES_H_ */
