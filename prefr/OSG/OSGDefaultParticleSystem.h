/*
 * OSGDefaultParticleSystem.h
 *
 *  Created on: 28/10/2014
 *      Author: sgalindo
 */

#ifndef OSGDEFAULTPARTICLESYSTEM_H_
#define OSGDEFAULTPARTICLESYSTEM_H_

#include <prefr/api.h>
#include "../ParticleSystem.h"

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

#include "OSGDefaultParticleRenderer.h"

namespace prefr
{

  #define ATRIB_ARRAY_POSITIONS 1

  class OSGPSNodeCallBack : public osg::NodeCallback
  {
  public:

    PREFR_API virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

  };

  class OSGDefaultParticleSystem  : public ParticleSystem
                                  , public osg::Drawable
  {
  public:

    osgGA::StandardManipulator* cameraManipulator;

    osg::Geode* rootNode;

    osg::BlendFunc::BlendFuncMode blendFunctionSrc, blendFunctionDst;

    PREFR_API OSGDefaultParticleSystem();
    PREFR_API ~OSGDefaultParticleSystem();

    PREFR_API OSGDefaultParticleSystem(const OSGDefaultParticleSystem& other,
				       const osg::CopyOp& copyOp);

    PREFR_API OSGDefaultParticleSystem(unsigned int initialParticlesNumber,
				       unsigned int _maxParticles, bool _loop = true,
				       osg::BlendFunc::BlendFuncMode blendFuncSrc = osg::BlendFunc::/*BlendFuncMode::*/SRC_ALPHA,
				       osg::BlendFunc::BlendFuncMode blendFuncDst = osg::BlendFunc::/*BlendFuncMode::*/ONE_MINUS_CONSTANT_ALPHA);

    META_Object(particles::defaultParticleSystem::OSGParticleSystem, OSGDefaultParticleSystem)

    PREFR_API virtual void SetCameraManipulator(osgViewer::ViewerBase* _viewer, unsigned int contextNumber = 0, unsigned int viewNumber = 0);
    PREFR_API virtual void ConfigureProgram(const std::string& shaderPathVert, const std::string& shaderPathFrag);

    PREFR_API virtual void Update(float deltaTime);
    PREFR_API virtual void UpdateUnified(float deltaTime);
//    virtual void UpdateCameraDistances(const glm::vec3& cameraPosition);

    PREFR_API virtual void UpdateRender();
    PREFR_API virtual void Render() const;

    PREFR_API virtual osg::BoundingBox computeBound() const;
    PREFR_API virtual void compileGLObjects(osg::RenderInfo& renderInfo) const;
    PREFR_API virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
    PREFR_API virtual void releaseGLObjects(osg::State* state) const;
    PREFR_API virtual void accept(osg::PrimitiveFunctor& functor) const;

    PREFR_API virtual void SetAlphaBlendingFunction(osg::BlendFunc::BlendFuncMode src, osg::BlendFunc::BlendFuncMode dst);

  protected:

    PREFR_API virtual void UpdateUniformVariables(float deltaTime);
    PREFR_API virtual void AcquireGraphicsContext(osg::GraphicsContext* context);

  };

}
#endif

#endif /* OSGDEFAULTPARTICLESYSTEM_H_ */
