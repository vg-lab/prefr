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

#include <prefr/log.h>

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
    renderConfig = new OSGRenderConfig( _particles.size );

     GLfloat b[] = {-0.5f, -0.5f, 0.0f,
                    0.5f,  -0.5f, 0.0f,
                    -0.5f, 0.5f, 0.0f,
                    0.5f, 0.5f, 0.0f};

     OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( renderConfig );

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

       osgrc->billboardVertices->push_back(b[ i*3 ]);
       osgrc->billboardVertices->push_back(b[ i*3 + 1 ]);
       osgrc->billboardVertices->push_back(b[ i*3 + 2 ]);

       osgrc->billboardIndices->push_back( i );

     }
  }

  void OSGRenderer::compileGLObjects(
    osg::RenderInfo& /* renderInfo */ ) const
  {
    OSGRenderConfig* osgrc = static_cast<OSGRenderConfig*>(renderConfig);

    glGenVertexArrays( 1, &osgrc->vao );

    GLuint buffersGL[ 4 ];
    glGenBuffers( 4, buffersGL );

    osgrc->vboBillboardVertex = buffersGL[ 0 ];
    osgrc->vboParticlesPositions = buffersGL[ 1 ];
    osgrc->vboParticlesColors = buffersGL[ 2 ];
    osgrc->vboDrawElements = buffersGL[ 3 ];

    osgrc->init = true;

    // Assign billboard vertices
    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboBillboardVertex);

    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(GLfloat) * osgrc->billboardVertices->size(),
                 &osgrc->billboardVertices->front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesPositions);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * osgrc->particlePositions->size(),
                 nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesColors);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * osgrc->particleColors->size(),
                 nullptr, GL_DYNAMIC_DRAW);


    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, osgrc->vboDrawElements);

    glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                  osgrc->billboardIndices->getTotalDataSize( ),
                  osgrc->billboardIndices->getDataPointer( ), GL_STATIC_DRAW) ;

    glBindVertexArray( osgrc->vao );

    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, osgrc->vboBillboardVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesPositions);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesColors);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, (void *) 0);

//        glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, osgrc->vboDrawElements);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  }

  void OSGRenderer::SetupRender( void )
  {
    OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( renderConfig );

    osgrc->boundingBox.init( );

    for (unsigned int i = 0; i < renderConfig->aliveParticles; i++)
    {
      tparticle currentParticle = _particles.GetElement(distances->getID(i));

      unsigned int idx = i * 4;

      std::vector< GLfloat >::iterator posit =
          renderConfig->particlePositions->begin( ) + idx;

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
          renderConfig->particleColors->begin( ) + idx;

      *colorit = currentParticle.color( ).x;
      ++colorit;

      *colorit = currentParticle.color( ).y;
      ++colorit;

      *colorit = currentParticle.color( ).z;
      ++colorit;

      *colorit = currentParticle.color( ).w;
      ++colorit;
    }

    if (osgrc->boundingBox.radius() == 0)
      osgrc->boundingBox.expandBy(osg::Vec3(1,1,1));

    osgrc->billboardIndices->setNumInstances( renderConfig->aliveParticles );
  }



  void OSGRenderer::Paint( void ) const
  {
    OSGRenderConfig* osgrc = static_cast< OSGRenderConfig* >( renderConfig );

    glBindVertexArray(osgrc->vao);

    glBindBuffer( GL_ARRAY_BUFFER, osgrc->vboParticlesPositions );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof(GLfloat) * renderConfig->aliveParticles * 4,
                     &osgrc->particlePositions->front());

    glBindBuffer( GL_ARRAY_BUFFER, osgrc->vboParticlesColors );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     sizeof(GLfloat) * renderConfig->aliveParticles * 4,
                     &osgrc->particleColors->front());

    glDrawElementsInstanced(osgrc->billboardIndices->getMode(),
                            osgrc->billboardIndices->getNumIndices(),
                            GL_UNSIGNED_BYTE, nullptr,
                            osgrc->billboardIndices->getNumInstances());
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }





  osg::BoundingBox OSGRenderer::computeBound() const
  {
    PREFR_DEBUG_CHECK( _renderer->renderConfig, "renderConfig is nullptr" );

    return static_cast<OSGRenderConfig*>(_renderer->renderConfig)->boundingBox;

  }

  void OSGRenderer::compileGLObjects(
    osg::RenderInfo& renderInfo ) const
  {
    PREFR_DEBUG_CHECK(
      static_cast<OSGRenderer*>(this->_renderer),
      "casting failed" );

    static_cast<OSGRenderer*>
      (this->_renderer)->osgCompileGLObjects(renderInfo);
  }

  void OSGRenderer::drawImplementation(
    osg::RenderInfo& renderInfo) const
  {
    osg::State* state = renderInfo.getState();
    state->setUseVertexAttributeAliasing(true);
    state->setUseModelViewAndProjectionUniforms(true);

    Paint();
  }

  void OSGRenderer::releaseGLObjects(osg::State* /* state */ )
    const
  {
    OSGRenderConfig* osgrc =
      static_cast<OSGRenderConfig*>(_renderer->renderConfig);

    glDeleteBuffers(1, &osgrc->vboBillboardVertex);
    glDeleteBuffers(1, &osgrc->vboDrawElements);
    glDeleteBuffers(1, &osgrc->vboParticlesPositions);
    glDeleteBuffers(1, &osgrc->vboParticlesColors);
    glDeleteVertexArrays(1, &osgrc->vao);
  }

  void OSGRenderer::accept(osg::PrimitiveFunctor& functor) const
  {
    OSGRenderConfig* osgrc =
          static_cast<OSGRenderConfig*>(_renderer->renderConfig);

    if (!osgrc->vertexArray|| !osgrc->billboardIndices)
      return;

    // add drawable to the stats
    functor.setVertexArray(osgrc->vertexArray->size(),
                           static_cast<const osg::Vec3*>(
                             osgrc->vertexArray->getDataPointer()));
    osgrc->billboardIndices->accept(functor);
  }




}
#endif
