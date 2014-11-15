/*
 * GLCUDAParticleRenderer.cpp
 *
 *  Created on: 31/10/2014
 *      Author: sgalindo
 */

#include "GLCUDAParticleRenderer.cuh"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace CUDATHRUST
    {


      GLCUDAParticleRenderer::GLCUDAParticleRenderer(ParticleCollection* particlesArray, distanceArray* distancesArray
                                                               , RenderConfig* renderConfiguration)
      : ParticleRenderer( particlesArray )
      , distances( distancesArray)
      , renderConfig( renderConfiguration)
      {

        GLfloat b[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.0f};

        renderConfig->billboardVertices = new vector<GLfloat>(12);
        renderConfig->particlePositions = new vector<GLfloat>(particles->size * 4);
        renderConfig->particleColors = new vector<GLchar>(particles->size * 4);

        for (unsigned int i = 0; i < renderConfig->billboardVertices->size(); i++)
        {
          renderConfig->billboardVertices->at(i) = b[i];
        }

        glGenVertexArrays(1, &renderConfiguration->vao);
        glBindVertexArray(renderConfiguration->vao);

        GLuint buffersGL[3];
        glGenBuffers(3, buffersGL);

        renderConfig->vboBillboardVertex = buffersGL[0];
        renderConfig->vboParticlesPositions = buffersGL[1];
        renderConfig->vboParticlesColor = buffersGL[2];

        // Assign billboard vertices
        //glGenBuffers(1, &vboBillboardVertex);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) *  renderConfig->billboardVertices->size()
                     , &renderConfig->billboardVertices->front(), GL_STATIC_DRAW);

        //glGenBuffers(1, &vboParticlesPositions);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particlePositions->size(), NULL, GL_STATIC_DRAW);

        //glGenBuffers(1, &vboParticlesColor);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColor);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLbyte) * renderConfig->particleColors->size(), NULL, GL_STATIC_DRAW);

      }

      GLCUDAParticleRenderer::~GLCUDAParticleRenderer()
      {
        delete( particles );
      }

      void GLCUDAParticleRenderer::SetupRender(unsigned int aliveParticles)
      {
        tparticle_ptr currentParticle;
        int idx;

        for (unsigned int i = 0; i < aliveParticles; i++)
        {
          currentParticle = particles->elements->at(distances->hostID[i]);

          idx = i * 4;

          renderConfig->particlePositions->at(idx) = currentParticle->position.x;
          renderConfig->particlePositions->at(idx+1) = currentParticle->position.y;
          renderConfig->particlePositions->at(idx+2) = currentParticle->position.z;
          renderConfig->particlePositions->at(idx+3) = currentParticle->size;

          renderConfig->particleColors->at(idx) = (int)currentParticle->color.x;
          renderConfig->particleColors->at(idx+1) = (int)currentParticle->color.y;
          renderConfig->particleColors->at(idx+2) = (int)currentParticle->color.z;
          renderConfig->particleColors->at(idx+3) = (int)currentParticle->color.w;
        }

        // Update positions buffer
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particlePositions->size(), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * aliveParticles * 4, &renderConfig->particlePositions->front());

        // Update colors buffer
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColor);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLbyte) * renderConfig->particleColors->size(), NULL, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLbyte) * aliveParticles * 4, &renderConfig->particleColors->front());


      }

      void GLCUDAParticleRenderer::Paint(unsigned int aliveParticles) const
      {
        // Bind vertices
          glEnableVertexAttribArray(0);
          glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

          glEnableVertexAttribArray(1);
          glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
          glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

          glEnableVertexAttribArray(2);
          glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColor);
          glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void *) 0);


          glVertexAttribDivisor(0, 0);
          glVertexAttribDivisor(1, 1);
          glVertexAttribDivisor(2, 1);

          glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, aliveParticles);

          glDisableVertexAttribArray(0);
          glDisableVertexAttribArray(1);
          glDisableVertexAttribArray(2);
      }


    }

  }

}


