/*
 * DefaultParticleRenderConfig.cpp
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#include "GLDefaultParticleRenderer.h"

namespace prefr
{

  GLDefaultParticleRenderer::GLDefaultParticleRenderer( const ParticleCollection& particlesArray)
  : ParticleRenderer( particlesArray )
  {

    renderConfig = new RenderConfig( particles->size );

    GLfloat b[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f};

    renderConfig->billboardVertices = new vector<GLfloat>(12);
    renderConfig->particlePositions = new vector<GLfloat>(particles->size * 4);
    renderConfig->particleColors = new vector<GLfloat>(particles->size * 4);

    for (unsigned int i = 0; i < renderConfig->billboardVertices->size(); i++)
    {
      renderConfig->billboardVertices->at(i) = b[i];
    }

    glGenVertexArrays(1, &renderConfig->vao);
    glBindVertexArray(renderConfig->vao);

    GLuint buffersGL[3];
    glGenBuffers(3, buffersGL);

    renderConfig->vboBillboardVertex = buffersGL[0];
    renderConfig->vboParticlesPositions = buffersGL[1];
    renderConfig->vboParticlesColors = buffersGL[2];

    // Assign billboard vertices
    //glGenBuffers(1, &vboBillboardVertex);
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *  renderConfig->billboardVertices->size()
                 , &renderConfig->billboardVertices->front(), GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particlePositions->size(), NULL, GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particleColors->size(), NULL, GL_DYNAMIC_DRAW);

    // Bind vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColors);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, (void *) 0);


    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);


  }

  GLDefaultParticleRenderer::~GLDefaultParticleRenderer()
  {
    delete( particles );
  }

  void GLDefaultParticleRenderer::SetupRender(unsigned int aliveParticles)
  {
    tparticle_ptr currentParticle;
    int idx;

    for (unsigned int i = 0; i < aliveParticles; i++)
    {
      currentParticle = particles->GetElement(distances->getID(i));

      idx = i * 4;

      (*renderConfig->particlePositions)[idx] = currentParticle->position.x;
      (*renderConfig->particlePositions)[idx+1] = currentParticle->position.y;
      (*renderConfig->particlePositions)[idx+2] = currentParticle->position.z;
      (*renderConfig->particlePositions)[idx+3] = currentParticle->size;

      (*renderConfig->particleColors)[idx] = currentParticle->color.x;
      (*renderConfig->particleColors)[idx+1] = currentParticle->color.y;
      (*renderConfig->particleColors)[idx+2] = currentParticle->color.z;
      (*renderConfig->particleColors)[idx+3] = currentParticle->color.w;
    }

    glBindVertexArray(renderConfig->vao);

    // Update positions buffer
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particlePositions->size(), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * aliveParticles * 4, &renderConfig->particlePositions->front());

    // Update colors buffer
    glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColors);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particleColors->size(), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * aliveParticles * 4, &renderConfig->particleColors->front());

    glBindVertexArray(0);
  }

  void GLDefaultParticleRenderer::Paint(unsigned int aliveParticles) const
  {
    glBindVertexArray(renderConfig->vao);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, aliveParticles);

    glBindVertexArray(0);
  }

}
