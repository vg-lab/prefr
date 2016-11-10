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
    , _billboardVertices( new std::vector< GLfloat >( size ) )
    , _particlePositions( new std::vector< GLfloat >( size ) )
    , _particleColors( new std::vector< GLfloat >( size ))
    , _vao( 0 )
    , _vboBillboardVertex( 0 )
    , _vboParticlesPositions( 0 )
    , _vboParticlesColors( 0 )
    , _vertexArray( nullptr )
    , _billboardIndices( nullptr )
    , _vboDrawElements( 0 )
    , _uCameraUp( nullptr )
    , _uCameraRight( nullptr )
    , _init( false )
    { }

    virtual ~OSGRenderConfig( )
    {
      _vboDrawElements = 0;
      _init = false;
    }

  protected:

    // Triangles vertices
    std::vector< GLfloat >* _billboardVertices;
    std::vector< GLfloat >* _particlePositions;
    std::vector< GLfloat >* _particleColors;

    // OpenGL pointers
    GLuint _vao;
    GLuint _vboBillboardVertex;
    GLuint _vboParticlesPositions;
    GLuint _vboParticlesColors;

    osg::Vec3Array* _vertexArray;
    osg::DrawElementsUByte* _billboardIndices;

    // OpenGL pointers
    GLuint _vboDrawElements;

    osg::Uniform* _uCameraUp;
    osg::Uniform* _uCameraRight;

    osg::Vec3f _eye;
    osg::Vec3f _center;
    osg::Vec3f _up;
    osg::Vec3f _right;

    osg::BoundingBox _boundingBox;
    osg::BoundingSphere _boundingSphere;

    bool _init;

  };


}

#endif

#endif /* OSGRENDERCONFIG_H_ */
