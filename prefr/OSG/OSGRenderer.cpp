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
#include "OSGRenderer.h"

#ifndef PREFR_SKIP_GLEW_INCLUDE
#include <GL/glew.h>
#endif

#ifdef PREFR_USE_OPENSCENEGRAPH

#include <osg/Array>
#include <osg/Geometry>

#include <osg/NodeVisitor>

#include <osgGA/StandardManipulator>
#include <osgGA/TrackballManipulator>

#include <osgDB/FileUtils>

#include <osgViewer/View>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osg/State>
#include <osg/GL>
#include <osg/BlendFunc>

#include "../utils/Log.h"

#include "OSGRenderConfig.h"

namespace prefr
{

  OSGRenderer::OSGRenderer( )
  : Renderer( )
  { }

  OSGRenderer::~OSGRenderer( )
  { }

  void OSGRenderer::_init( void )
  {
    _renderConfig = new OSGRenderConfig( _particles.size );

     GLfloat b[ ] = { -0.5f, -0.5f, 0.0f,
                       0.5f, -0.5f, 0.0f,
                      -0.5f,  0.5f, 0.0f,
                       0.5f,  0.5f, 0.0f };

     OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( _renderConfig );

     osgrc->_billboardVertices = new std::vector< GLfloat >;
     osgrc->_vertexArray = new osg::Vec3Array;
     osgrc->_billboardIndices = new osg::DrawElementsUByte( GL_TRIANGLE_STRIP );

     osgrc->_particlePositions =
         new std::vector< GLfloat >( _particles.size * 4 );

     osgrc->_particleColors = new std::vector< GLfloat >( _particles.size * 4 );

     for( unsigned int i = 0; i < 4; i++ )
     {
       osgrc->_vertexArray->push_back( osg::Vec3( b[ i * 3 ],
                                                 b[ i * 3 + 1 ],
                                                 b[ i * 3 + 2 ] ));

       osgrc->_billboardVertices->push_back( b[ i * 3 ]);
       osgrc->_billboardVertices->push_back( b[ i * 3 + 1 ]);
       osgrc->_billboardVertices->push_back( b[ i * 3 + 2 ]);

       osgrc->_billboardIndices->push_back( i );

     }
  }



  void OSGRenderer::setupRender( void )
  {
    OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( _renderConfig );

    osgrc->_boundingBox.init( );

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for
#endif
    for( int i = 0; i < ( int ) _renderConfig->aliveParticles; ++i )
    {
      tparticle currentParticle =
          _particles.GetElement( _distances->getID( i ));

      unsigned int idx = i * 4;

      std::vector< GLfloat >::iterator posit =
          osgrc->_particlePositions->begin( ) + idx;

      *posit = currentParticle.position( ).x;
      ++posit;

      *posit = currentParticle.position( ).y;
      ++posit;

      *posit = currentParticle.position( ).z;
      ++posit;

      *posit = currentParticle.size( );
      ++posit;

      osgrc->_boundingBox.expandBy( osg::Vec3( currentParticle.position( ).x,
                                              currentParticle.position( ).y,
                                              currentParticle.position( ).z ));

      std::vector< GLfloat >::iterator colorit =
          osgrc->_particleColors->begin( ) + idx;

      *colorit = currentParticle.color( ).x;
      ++colorit;

      *colorit = currentParticle.color( ).y;
      ++colorit;

      *colorit = currentParticle.color( ).z;
      ++colorit;

      *colorit = currentParticle.color( ).w;
      ++colorit;
    }

    if( osgrc->_boundingBox.radius( ) == 0 )
      osgrc->_boundingBox.expandBy( osg::Vec3( 1, 1, 1 ));

    osgrc->_boundingSphere.expandBy( osgrc->_boundingBox );

    osgrc->_billboardIndices->setNumInstances( _renderConfig->aliveParticles );
  }



  void OSGRenderer::paint( void ) const
  {
    OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( _renderConfig );

    glBindVertexArray( osgrc->_vao );

    glBindBuffer( GL_ARRAY_BUFFER, osgrc->_vboParticlesPositions );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _renderConfig->aliveParticles * 4,
                     &osgrc->_particlePositions->front( ));

    glBindBuffer( GL_ARRAY_BUFFER, osgrc->_vboParticlesColors );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _renderConfig->aliveParticles * 4,
                     &osgrc->_particleColors->front( ));

    glDrawElementsInstanced( osgrc->_billboardIndices->getMode( ),
                             osgrc->_billboardIndices->getNumIndices( ),
                             GL_UNSIGNED_BYTE, nullptr,
                             osgrc->_billboardIndices->getNumInstances( ));
    glBindVertexArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  }

}
#endif
