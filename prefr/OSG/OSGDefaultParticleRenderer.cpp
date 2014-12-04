/*
 * DefaultParticleRenderConfig.cpp
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#include "OSGDefaultParticleRenderer.h"

namespace prefr
{

  OSGDefaultParticleRenderer::OSGDefaultParticleRenderer(
      const ParticleCollection& particlesArray)
  : ParticleRenderer( particlesArray )
  , currentAliveParticles( 0 )
  {

    renderConfig = new OSGRenderConfig( particles->size );

    GLfloat b[] = {-0.5f, -0.5f, 0.0f,
                   0.5f,  -0.5f, 0.0f,
                   -0.5f, 0.5f, 0.0f,
                   0.5f, 0.5f, 0.0f};

    OSGRenderConfig* osgrc = static_cast<OSGRenderConfig*>(renderConfig);

    osgrc->billboardVertices = new vector<GLfloat>;
    osgrc->vertexArray = new osg::Vec3Array;
    osgrc->billboardIndices = new osg::DrawElementsUByte(GL_TRIANGLE_STRIP);
    osgrc->particlePositions = new vector<GLfloat>(particles->size * 4);
    osgrc->particleColors = new vector<GLfloat>(particles->size * 4);

    for (unsigned int i = 0; i < 12; i++)
    {
      osgrc->vertexArray->push_back(osg::Vec3( b[i*3],
                                               b[i*3 + 1],
                                               b[i*3 + 2] ));

      osgrc->billboardVertices->push_back(b[i]);



      if (i % 3 == 0)
        osgrc->billboardIndices->push_back(i / 3);

    }


  }

  OSGDefaultParticleRenderer::~OSGDefaultParticleRenderer()
  {

  }

  void OSGDefaultParticleRenderer::osgCompileGLObjects(
    osg::RenderInfo& /* renderInfo */ ) const
  {
    OSGRenderConfig* osgrc = static_cast<OSGRenderConfig*>(renderConfig);

    glGenVertexArrays(1, &osgrc->vao);


    GLuint buffersGL[4];
    glGenBuffers(4, buffersGL);

    osgrc->vboBillboardVertex = buffersGL[0];
    osgrc->vboParticlesPositions = buffersGL[1];
    osgrc->vboParticlesColors = buffersGL[2];
    osgrc->vboDrawElements = buffersGL[3];

    osgrc->init = true;

    // Assign billboard vertices
    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboBillboardVertex);
    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(GLfloat) * osgrc->billboardVertices->size(),
                 &osgrc->billboardVertices->front(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesPositions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * osgrc->particlePositions->size(), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * osgrc->particleColors->size(), NULL, GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, osgrc->vboDrawElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, osgrc->billboardIndices->getTotalDataSize(), osgrc->billboardIndices->getDataPointer(), GL_STATIC_DRAW);

    glBindVertexArray(osgrc->vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboBillboardVertex);
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

  void OSGDefaultParticleRenderer::SetupRender(unsigned int aliveParticles)
  {
    OSGRenderConfig* osgrc = static_cast<OSGRenderConfig*>(renderConfig);

    tparticle_ptr currentParticle;
    int idx;

    osgrc->boundingBox.init();

    for (unsigned int i = 0; i < aliveParticles; i++)
    {
      currentParticle = particles->elements->at(distances->getID(i));

      idx = i * 4;

      osgrc->particlePositions->at(idx) = currentParticle->position.x;
      osgrc->particlePositions->at(idx+1) = currentParticle->position.y;
      osgrc->particlePositions->at(idx+2) = currentParticle->position.z;
      osgrc->particlePositions->at(idx+3) = currentParticle->size;

      osgrc->boundingBox.expandBy(osg::Vec3(  currentParticle->position.x
                            , currentParticle->position.y
                            , currentParticle->position.z));

      osgrc->particleColors->at(idx) = currentParticle->color.x;
      osgrc->particleColors->at(idx+1) = currentParticle->color.y;
      osgrc->particleColors->at(idx+2) = currentParticle->color.z;
      osgrc->particleColors->at(idx+3) = currentParticle->color.w;
    }

    currentAliveParticles = aliveParticles;

    osgrc->billboardIndices->setNumInstances(aliveParticles);
  }



  void OSGDefaultParticleRenderer::Paint(unsigned int aliveParticles) const
  {
    OSGRenderConfig* osgrc = static_cast<OSGRenderConfig*>(renderConfig);

    glBindVertexArray(osgrc->vao);

    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesPositions);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * aliveParticles * 4, &osgrc->particlePositions->front());

    glBindBuffer(GL_ARRAY_BUFFER, osgrc->vboParticlesColors);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * aliveParticles * 4, &osgrc->particleColors->front());



    glDrawElementsInstanced(osgrc->billboardIndices->getMode()
                            , osgrc->billboardIndices->getNumIndices()
                            , GL_UNSIGNED_BYTE, NULL
                            , osgrc->billboardIndices->getNumInstances());
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }



}
