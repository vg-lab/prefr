/*
 * OSGRenderConfig.h
 *
 *  Created on: 25/11/2014
 *      Author: sgalindo
 */

#ifndef OSGRENDERCONFIG_H_
#define OSGRENDERCONFIG_H_

#include "../RenderConfig.h"

#ifdef PREFR_USE_OPENSCENEGRAPH

#include <osg/Array>
#include <osg/Drawable>

namespace prefr
{
  class OSGRenderConfig : public RenderConfig
  {
  public:
    // Triangles vertices

    osg::Vec3Array* vertexArray;
    osg::DrawElementsUByte* billboardIndices;

    // OpenGL pointers
    GLuint vboDrawElements;

    osg::Uniform* uCameraUp;
    osg::Uniform* uCameraRight;

    osg::Vec3f eye, center, up, right;

    osg::BoundingBox boundingBox;
    osg::BoundingSphere boundingSphere;

    bool init;

    OSGRenderConfig(unsigned int size)
    : RenderConfig(size)
    , vertexArray( nullptr )
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

#endif

#endif /* OSGRENDERCONFIG_H_ */
