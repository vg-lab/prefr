/*
 * OSGRenderConfig.h
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
 */

#ifndef OSGRENDERCONFIG_H_
#define OSGRENDERCONFIG_H_

#include "../RenderConfig.h"

#include <osg/Array>
#include <osg/Drawable>

namespace particles
{
  class OSGRenderConfig : public RenderConfig
  {
  public:
    // Triangles vertices

     osg::DrawElementsUByte* billboardIndices;

     // OpenGL pointers
     GLuint vboDrawElements;

     osg::Uniform* uCameraUp;
     osg::Uniform* uCameraRight;

     osg::Vec3f eye, center, up, right;

     osg::BoundingBox boundingBox;

     bool init;

     OSGRenderConfig(unsigned int size)
     : RenderConfig(size)
     , billboardIndices( nullptr )
     , vboDrawElements( 0 )
     , uCameraUp( nullptr )
     , uCameraRight( nullptr )
     , init( false )
     {}

     virtual ~OSGRenderConfig()
     {
       vboDrawElements = 0;
       init = false;
     }

  };


}

#endif /* OSGRENDERCONFIG_H_ */
