/*
 * OSGDefaultParticleSystem.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef OSGDEFAULTPARTICLESYSTEM_H_
#define OSGDEFAULTPARTICLESYSTEM_H_

#include "../ParticleSystem.h"

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

#include "OSGDefaultParticleRenderer.h"

namespace particles
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

    osg::Geode* rootNode;

    OSGDefaultParticleSystem();
    ~OSGDefaultParticleSystem();

    OSGDefaultParticleSystem(const OSGDefaultParticleSystem& other,
                             const osg::CopyOp& copyOp);

    OSGDefaultParticleSystem(unsigned int initialParticlesNumber,
                             unsigned int _maxParticles, bool _loop = true);

    META_Object(particles::defaultParticleSystem::OSGParticleSystem, OSGDefaultParticleSystem)

    virtual void OSGDefaultParticleSystem::SetCameraManipulator(osgViewer::ViewerBase* _viewer, unsigned int contextNumber = 0, unsigned int viewNumber = 0);
    void ConfigureProgram(const std::string& shaderPathVert, const std::string& shaderPathFrag);

    virtual void Update(float deltaTime);
    virtual void UpdateUnified(float deltaTime);
    virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);

    virtual void UpdateRender();
    virtual void Render() const;

    virtual osg::BoundingBox computeBound() const;
    virtual void compileGLObjects(osg::RenderInfo& renderInfo) const;
    virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
    virtual void releaseGLObjects(osg::State* state) const;

  protected:

    virtual void UpdateUniformVariables(float deltaTime);
    virtual void AcquireGraphicsContext(osg::GraphicsContext* context);

  };

}


#endif /* OSGDEFAULTPARTICLESYSTEM_H_ */
