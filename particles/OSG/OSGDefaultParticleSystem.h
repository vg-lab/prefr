/*
 * OSGDefaultParticleSystem.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef OSGDEFAULTPARTICLESYSTEM_H_
#define OSGDEFAULTPARTICLESYSTEM_H_

#include "OSGTypes.h"
#include "../ParticleSystem.h"

#include <osg/Array>
#include <osg/Geometry>

#include <osg/NodeVisitor>

#include <osgGA/StandardManipulator>
#include <osgGA/TrackballManipulator>

#include <osgDB/FileUtils>

#include <osg/State>
#include <osg/GL>
#include <osg/BlendFunc>

#include "OSGDefaultParticleSorter.h"
#include "OSGDefaultParticleRenderer.h"

namespace particles
{
  namespace defaultParticleSystem
  {

    namespace OSGParticleSystem
    {

      #define ATRIB_ARRAY_POSITIONS 1

      class OSGPSNodeCallBack : public osg::NodeCallback
      {
      public:

        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

      };

      class OSGDefaultParticleSystem  : public ParticleSystem
                                      , public osg::Drawable
      {
      public:

        osgGA::StandardManipulator* cameraManipulator;

        distanceArray* distances;

        RenderConfig* renderConfig;

        osg::Geode* rootNode;

        OSGDefaultParticleSystem();
        ~OSGDefaultParticleSystem();

        OSGDefaultParticleSystem(const OSGDefaultParticleSystem& other, 
                                 const osg::CopyOp& copyOp);

        OSGDefaultParticleSystem(unsigned int initialParticlesNumber,
                                 unsigned int _maxParticles, bool _loop = true);

        META_Object(particles::defaultParticleSystem::OSGParticleSystem, OSGDefaultParticleSystem)

        virtual void SetCameraManipulator(osgGA::StandardManipulator* cam);
        void ConfigureProgram(const std::string& shaderPathVert, const std::string& shaderPathFrag);

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
