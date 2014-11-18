/*
 * OSGDefaultParticleSystem.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef OSGDEFAULTPARTICLESYSTEM_H_
#define OSGDEFAULTPARTICLESYSTEM_H_

#include "OSGTypes.h"
#include "../DefaultParticleSystem.h"

#include <osg/Array>
#include <osg/Geometry>

#include <osg/NodeVisitor>

#include <osgGA/StandardManipulator>

#include <osgDB/FileUtils>

#include <osg/State>
#include <osg/GL>

#include "OSGDefaultParticleSorter.h"
#include "OSGDefaultParticleRenderer.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    namespace OSGParticleSystem
    {

      #define ATRIB_ARRAY_POSITIONS 1

      class OSGPSNodeCallBack : public osg::Drawable::UpdateCallback
      {
      public:

        void update(osg::NodeVisitor* nv, osg::Drawable* node);

      };

      class OSGDefaultParticleSystem : public DefaultParticleSystem, public osg::Drawable
      {
      public:

        osgGA::StandardManipulator* cameraManipulator;

        distanceArray* distances;

        RenderConfig* renderConfig;

        osg::Geode* rootNode;

        OSGDefaultParticleSystem();
        OSGDefaultParticleSystem(const OSGDefaultParticleSystem& other, const osg::CopyOp& copyOp);
        OSGDefaultParticleSystem(int initialParticlesNumber, int _maxParticles
                                 , float _emissionRate, bool _loop = true
                                 , osgGA::StandardManipulator* cam = nullptr);

        META_Object(particles::defaultParticleSystem::OSGParticleSystem, OSGDefaultParticleSystem)

        virtual void SetCameraManipulator(osgGA::StandardManipulator* cam);
        virtual void LoadProgram();

        virtual void SetRenderer(ParticleRenderer* renderConfig);

        virtual void Update(float deltaTime);
        virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);

        virtual void UpdateRender();
        virtual void Render() const;

        virtual osg::BoundingBox computeBound() const;
        virtual void compileGLObjects(osg::RenderInfo& renderInfo) const;
        virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
        virtual void accept(osg::PrimitiveFunctor& functor) const;
        virtual void releaseGLObjects(osg::State* state) const;
      };
    }

  }
}


#endif /* OSGDEFAULTPARTICLESYSTEM_H_ */
