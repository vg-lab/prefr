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

#include "../Log.h"

#ifdef PREFR_USE_OPENSCENEGRAPH

namespace prefr
{

  OSGRenderer::OSGRenderer( )
  : Renderer( )
  { }

  OSGRenderer::~OSGRenderer()
  { }

  void OSGRenderer::init( void )
  {
    _renderConfig = new OSGRenderConfig( _particles.size );

     GLfloat b[ ] = { -0.5f, -0.5f, 0.0f,
                       0.5f, -0.5f, 0.0f,
                      -0.5f,  0.5f, 0.0f,
                       0.5f,  0.5f, 0.0f };

     OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( _renderConfig );

     osgrc->billboardVertices = new std::vector< GLfloat >;
     osgrc->vertexArray = new osg::Vec3Array;
     osgrc->billboardIndices = new osg::DrawElementsUByte( GL_TRIANGLE_STRIP );

     osgrc->particlePositions =
         new std::vector< GLfloat >( _particles.size * 4 );

     osgrc->particleColors = new std::vector< GLfloat >( _particles.size * 4 );

     for (unsigned int i = 0; i < 4; i++)
     {
       osgrc->vertexArray->push_back( osg::Vec3( b[ i*3 ],
                                                 b[ i*3 + 1 ],
                                                 b[ i*3 + 2 ] ));

       osgrc->billboardVertices->push_back( b[ i*3 ]);
       osgrc->billboardVertices->push_back( b[ i*3 + 1 ]);
       osgrc->billboardVertices->push_back( b[ i*3 + 2 ]);

       osgrc->billboardIndices->push_back( i );

     }
  }



  void OSGRenderer::SetupRender( void )
  {
    OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( _renderConfig );

    osgrc->boundingBox.init( );

#ifdef PREFR_USE_OPENMP
    #pragma omp parallel for
#endif
    for( int i = 0; i < ( int ) _renderConfig->aliveParticles; ++i )
    {
      tparticle currentParticle = _particles.GetElement( _distances->getID( i ));

      unsigned int idx = i * 4;

      std::vector< GLfloat >::iterator posit =
          _renderConfig->particlePositions->begin( ) + idx;

      *posit = currentParticle.position( ).x;
      ++posit;

      *posit = currentParticle.position( ).y;
      ++posit;

      *posit = currentParticle.position( ).z;
      ++posit;

      *posit = currentParticle.size( );
      ++posit;

      osgrc->boundingBox.expandBy(osg::Vec3(  currentParticle.position( ).x,
                                              currentParticle.position( ).y,
                                              currentParticle.position( ).z));

      std::vector< GLfloat >::iterator colorit =
          _renderConfig->particleColors->begin( ) + idx;

      *colorit = currentParticle.color( ).x;
      ++colorit;

      *colorit = currentParticle.color( ).y;
      ++colorit;

      *colorit = currentParticle.color( ).z;
      ++colorit;

      *colorit = currentParticle.color( ).w;
      ++colorit;
    }

    if( osgrc->boundingBox.radius( ) == 0 )
      osgrc->boundingBox.expandBy( osg::Vec3( 1,1,1 ));

    osgrc->boundingSphere.expandBy( osgrc->boundingBox );

    osgrc->billboardIndices->setNumInstances( _renderConfig->aliveParticles );
  }



  void OSGRenderer::Paint( void ) const
  {
    OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( _renderConfig );

    glBindVertexArray( osgrc->vao );

    glBindBuffer( GL_ARRAY_BUFFER, osgrc->vboParticlesPositions );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _renderConfig->aliveParticles * 4,
                     &osgrc->particlePositions->front( ));

    glBindBuffer( GL_ARRAY_BUFFER, osgrc->vboParticlesColors );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof( GLfloat ) * _renderConfig->aliveParticles * 4,
                     &osgrc->particleColors->front( ));

    glDrawElementsInstanced( osgrc->billboardIndices->getMode( ),
                             osgrc->billboardIndices->getNumIndices( ),
                             GL_UNSIGNED_BYTE, nullptr,
                             osgrc->billboardIndices->getNumInstances( ));
    glBindVertexArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  }

}
#endif
