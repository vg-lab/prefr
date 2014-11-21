/*
 * DefaultParticleRenderConfig.cpp
 *
 *  Created on: 15/10/2014
 *      Author: sergio
 */

#include "OSGDefaultParticleRenderer.h"

namespace particles
{
  namespace defaultParticleSystem
  {
    namespace OSGParticleSystem
    {


      OSGDefaultParticleRenderer::OSGDefaultParticleRenderer(ParticleCollection* particlesArray, 
							     distanceArray* distancesArray,
                                                             RenderConfig* renderConfiguration)
      : ParticleRenderer( particlesArray )
      , distances( distancesArray)
      , renderConfig( renderConfiguration)
      , currentAliveParticles( 0 )
      {

        GLfloat b[] = {-0.5f, -0.5f, 0.0f, 
                       0.5f,  -0.5f, 0.0f,
                       -0.5f, 0.5f, 0.0f,
                       0.5f, 0.5f, 0.0f};

        renderConfig->billboardVertices = new osg::Vec3Array();
        renderConfig->billboardIndices = new osg::DrawElementsUByte(GL_TRIANGLE_STRIP);
        renderConfig->particlePositions = new vector<GLfloat>(particles->size * 4);
        renderConfig->particleColors = new vector<GLfloat>(particles->size * 4);

        for (unsigned int i = 0; i < 4; i++)
        {
          renderConfig->billboardVertices->push_back(osg::Vec3( b[i*3],
                                                                b[i*3 + 1],
                                                                b[i*3 + 2] ));
//          std::cout << renderConfig->billboardVertices->at(i) << std::endl;
          renderConfig->billboardIndices->push_back(i);
        }


      }

      OSGDefaultParticleRenderer::~OSGDefaultParticleRenderer()
      {
        delete( distances );
      }

      void OSGDefaultParticleRenderer::osgCompileGLObjects(osg::RenderInfo& renderInfo) const
      {
          glGenVertexArrays(1, &renderConfig->vao);


          GLuint buffersGL[4];
          glGenBuffers(4, buffersGL);

          renderConfig->vboBillboardVertex = buffersGL[0];
          renderConfig->vboParticlesPositions = buffersGL[1];
          renderConfig->vboParticlesColor = buffersGL[2];
          renderConfig->vboDrawElements = buffersGL[3];

          renderConfig->init = true;



        // Assign billboard vertices
        //glGenBuffers(1, &vboBillboardVertex);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
        glBufferData(GL_ARRAY_BUFFER, renderConfig->billboardVertices->getTotalDataSize()
                     , renderConfig->billboardVertices->getDataPointer(), GL_STATIC_DRAW);


        std::cout << renderConfig->billboardVertices->getTotalDataSize() << " -> " << renderConfig->billboardVertices->size() * sizeof(float) * 3 << std::endl;

        //glGenBuffers(1, &vboParticlesPositions);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particlePositions->size(), NULL, GL_DYNAMIC_DRAW);

        //glGenBuffers(1, &vboParticlesColor);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColor);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * renderConfig->particleColors->size(), NULL, GL_DYNAMIC_DRAW);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderConfig->vboDrawElements);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderConfig->billboardIndices->getTotalDataSize(), renderConfig->billboardIndices->getDataPointer(), GL_STATIC_DRAW);

        for (unsigned int i = 0; i < renderConfig->billboardIndices->size(); i++)
        {
          std::cout << int(renderConfig->billboardIndices->at(i)) << std::endl;
        }

        glBindVertexArray(renderConfig->vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboBillboardVertex);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColor);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, 0, (void *) 0);

//        glVertexAttribDivisor(0, 0);
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderConfig->vboDrawElements);

        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//        std::cout << "Compile GL objects" << std::endl;
      }

      void OSGDefaultParticleRenderer::SetupRender(unsigned int aliveParticles)
      {
        tparticle_ptr currentParticle;
        int idx;

        renderConfig->boundingBox.init();

        for (unsigned int i = 0; i < aliveParticles; i++)
        {
          currentParticle = particles->elements->at(distances->at(i).Id());

          idx = i * 4;

          renderConfig->particlePositions->at(idx) = currentParticle->position.x;
          renderConfig->particlePositions->at(idx+1) = currentParticle->position.y;
          renderConfig->particlePositions->at(idx+2) = currentParticle->position.z;
          renderConfig->particlePositions->at(idx+3) = currentParticle->size;

          renderConfig->boundingBox.expandBy(osg::Vec3(  currentParticle->position.x
                                , currentParticle->position.y
                                , currentParticle->position.z));

          renderConfig->particleColors->at(idx) = currentParticle->color.x;
          renderConfig->particleColors->at(idx+1) = currentParticle->color.y;
          renderConfig->particleColors->at(idx+2) = currentParticle->color.z;
          renderConfig->particleColors->at(idx+3) = currentParticle->color.w;
        }

        currentAliveParticles = aliveParticles;

        renderConfig->billboardIndices->setNumInstances(aliveParticles);
      }



      void OSGDefaultParticleRenderer::Paint(unsigned int aliveParticles) const
      {

        glBindVertexArray(renderConfig->vao);

//        std::cout << "Paint " << aliveParticles << std::endl;

        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesPositions);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * aliveParticles * 4, &renderConfig->particlePositions->front());

        glBindBuffer(GL_ARRAY_BUFFER, renderConfig->vboParticlesColor);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * aliveParticles * 4, &renderConfig->particleColors->front());

//        glDrawElements(renderConfig->billboardIndices->getMode()
//                       , renderConfig->billboardIndices->getNumIndices()
//                       , GL_UNSIGNED_BYTE
//                       , NULL);

        glDrawElementsInstanced(renderConfig->billboardIndices->getMode()
                                , renderConfig->billboardIndices->getNumIndices()
                                , GL_UNSIGNED_BYTE, NULL
                                , renderConfig->billboardIndices->getNumInstances());
        glBindVertexArray(0);
      }

    }
  }
}
