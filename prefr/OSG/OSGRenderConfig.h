/*
 * Copyright (c) 2014-2016 GMRV/URJC.
 *
 * Authors: Sergio Galindo <sergio.galindo@urjc.es>
 *
 * This file is part of PReFr <https://gmrv.gitlab.com/nsviz/prefr>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#ifndef OSGRENDERCONFIG_H_
#define OSGRENDERCONFIG_H_

#include "../core/RenderConfig.h"

#ifdef PREFR_USE_OPENSCENEGRAPH

#include <osg/Array>
#include <osg/Drawable>

namespace prefr
{
  class OSGRenderConfig : public RenderConfig
  {
    friend class OSGRenderer;
    friend class OSGManager;

  public:

    OSGRenderConfig( unsigned int size )
    : RenderConfig( size )
    , billboardVertices( new std::vector< GLfloat >( size ) )
    , particlePositions( new std::vector< GLfloat >( size ) )
    , particleColors( new std::vector< GLfloat >( size ))
    , vao( 0 )
    , vboBillboardVertex( 0 )
    , vboParticlesPositions( 0 )
    , vboParticlesColors( 0 )
    , vertexArray( nullptr )
    , billboardIndices( nullptr )
    , vboDrawElements( 0 )
    , uCameraUp( nullptr )
    , uCameraRight( nullptr )
    , init( false )
    { }

    virtual ~OSGRenderConfig( )
    {
      vboDrawElements = 0;
      init = false;
    }

  protected:

    // Triangles vertices
    std::vector< GLfloat >* billboardVertices;
    std::vector< GLfloat >* particlePositions;
    std::vector< GLfloat >* particleColors;

    // OpenGL pointers
    GLuint vao;
    GLuint vboBillboardVertex;
    GLuint vboParticlesPositions;
    GLuint vboParticlesColors;

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

  };


}

#endif

#endif /* OSGRENDERCONFIG_H_ */
