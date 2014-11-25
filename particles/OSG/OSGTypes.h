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

#include <particles/config.h>


#include <osg/Array>
#include <osg/Drawable>

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {
//      class DistanceUnit
//      {
//      public:
//        int* id;
//        float* distance;
//
//        DistanceUnit(int* id = nullptr, float* distance = nullptr)
//	  : id(id)
//	  , distance(distance)
//	{
//	}
//
//        int& Id(void)
//	{
//	  #ifdef DEBUG
//	  if ( !id )
//	    PARTICLES_THROW( "id pointer is null");
//          #endif
//
//	  return (int&)*id;
//	}
//
//        float& Distance(void)
//	{
//	  #ifdef DEBUG
//	  if ( !distance )
//	    PARTICLES_THROW( "distance pointer is null");
//          #endif
//
//	  return (float&)*distance;
//	}
//
//        const int& getID(void)
//	{
//	  #ifdef DEBUG
//	  if ( !id )
//	    PARTICLES_THROW( "id pointer is null");
//          #endif
//
//	  return (const int&)*id;
//	}
//
//	const float& getDistance(void)
//	{
//	  #ifdef DEBUG
//	  if ( !distance )
//	    PARTICLES_THROW( "distance pointer is null");
//          #endif
//
//	  return (const float&)*distance;
//	}
//
//      };
//
//      typedef DistanceUnit tdunit;
//      typedef std::vector<tdunit> tdcontainter;
//
//      class DistanceArray
//      {
//      public:
//        vector<int>* ids;
//        vector<float>* distances;
//        tdcontainter* elements;
//
//        DistanceArray(unsigned int size)
//        {
//          ids = new vector<int>(size);
//          distances = new vector<float>(size);
//          elements = new tdcontainter(size);
//
//          for (unsigned int i = 0; i < size; i++)
//          {
//            elements->at(i) = tdunit(&ids->at(i), &distances->at(i));
//          }
//        }
//
//        vector<DistanceUnit>::iterator begin()
//        {
//          return elements->begin();
//        }
//
//        vector<DistanceUnit>::iterator end()
//        {
//          return elements->end();
//        }
//
//        DistanceUnit& at(unsigned int i)
//        {
//          return elements->at(i);
//        }
//
//        DistanceUnit& operator[](unsigned int i)
//        {
//          return elements->at(i);
//        }
//
//        static bool sortDescending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance > *rhs.distance;}
//        static bool sortAscending (const DistanceUnit& lhs, const DistanceUnit& rhs){return *lhs.distance < *rhs.distance;}
//      };
//
//      typedef DistanceArray distanceArray;
//      typedef vector<GLfloat> glvectorf;
//      typedef vector<GLchar> glvectorch;
//
//      class RenderConfig
//      {
//      public:
//
//        // Triangles vertices
//        osg::Vec3Array* billboardVertices;
//        osg::DrawElementsUByte* billboardIndices;
//        glvectorf* particlePositions;
//        glvectorf* particleColors;
//
//        // OpenGL pointers
//        GLuint vao;
//        GLuint vboBillboardVertex;
//        GLuint vboDrawElements;
//        GLuint vboParticlesPositions;
//        GLuint vboParticlesColor;
//
//        osg::Uniform* uCameraUp;
//        osg::Uniform* uCameraRight;
//
//        osg::Vec3f eye, center, up, right;
//
//        osg::BoundingBox boundingBox;
//
//        bool init;
//
//        RenderConfig()
//        : billboardVertices( nullptr )
//        , billboardIndices( nullptr )
//        , particlePositions( nullptr )
//        , particleColors( nullptr )
//        , vao( 0 )
//        , vboBillboardVertex( 0 )
//        , vboDrawElements( 0 )
//        , vboParticlesPositions( 0 )
//        , vboParticlesColor( 0 )
//        , uCameraUp( nullptr )
//        , uCameraRight( nullptr )
//        , init( false )
//        {}
//
//        ~RenderConfig()
//        {}
//
//      };
//
    }
  }
}

#endif /* OSGTYPES_H_ */
